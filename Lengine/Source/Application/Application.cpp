#include <Application/Application.hpp>
#include <Application/CommandLine.hpp>
#include <Application/Commandlet.hpp>
#include <Common/Platform.hpp>
#include <Client/Client.hpp>
#include <Engine/Engine.hpp>
#include <Utility/Initializer.hpp>

#if IS_EDITOR
#include <Editor/Editor.hpp>
#endif

using std::make_shared;

template < typename _Integer = uint8_t >
class DeltaTimeTracker
{
public:

	using IntegerType = uint8_t;
	static constexpr size_t Entries = 1u << sizeof( IntegerType ) * 8u;
	static_assert( std::is_unsigned_v< IntegerType >, "IntegerType must be an unsigned type." );

	explicit DeltaTimeTracker( const float a_Target )
		: DeltaTimes{ a_Target }
	{}

	void Commit( const float a_DeltaTime )
	{
		DeltaTimes[ ++DeltaTimeIndex ] = a_DeltaTime;
	}

	float Get()
	{
		return Get( std::make_integer_sequence< IntegerType, static_cast< IntegerType >( -1 ) >{} );
	}

private:

	template < IntegerType... _Idxs >
	float Get( std::integer_sequence< IntegerType, _Idxs... > )
	{
		return ( float )( DeltaTimes[ _Idxs ] + ... ) / Entries;
	}

public:

	// Store a buffer of delta times. Keeping this as the size of
	// an integer type allows integer overflow to replace % operations.
	float DeltaTimes[ Entries ];
	IntegerType DeltaTimeIndex = 0u;
};

Application::Application( const int a_ArgC, const char** a_ArgV, const char* a_Name )
	: m_QuitRequested( false )
	, m_Name( a_Name )
	, m_LogsFolder( "." )
	, m_ArgC( a_ArgC )
	, m_ArgV( a_ArgV )
#if IS_EDITOR
	, m_IsCommandlet( false )
#endif
{}

int Application::Run()
{
	// Invoke all Initializer's to track initialization stage requests.
	Initializer::Startup();

	// Load all configs first.
	Config::Load();

	// Override configs if commandline asks for it.
	CommandLine::Construct( m_ArgC, m_ArgV );

	// If the above commandline invoked a commandlet, return after this.
#if IS_EDITOR
	if ( IsCommandlet() )
	{
		TODO( "Make this return something meaninful considering Run reports directly out of int main(...)." );
		return 0;
	}
#endif

	// Create the engine.
	m_Engine = make_shared< Engine >( this );

	// Create a client/editor.
#if IS_EDITOR
	m_Editor = make_shared< Editor >( this );
#elif IS_CLIENT
	m_Client = make_shared< Client >( this );
#endif

	// Call OnCreate for the application first.
	OnCreate();

#if IS_EDITOR
	// Call OnCreate for the editor second if this is an editor build.
	m_Editor->OnCreate();
#elif IS_CLIENT
	// Call OnCreate for the client second if this is an client build.
	m_Client->OnCreate();
#endif

	// This should come from somewhere else.
	constexpr float TargetFPS = 30.0f;
	constexpr float TargetDeltaTime = 1.0f / TargetFPS;

	DeltaTimeTracker DeltaTime( TargetDeltaTime );

	// While quit is not requested, keep looping.
	while ( !IsQuitting() )
	{
		// Get the last delta time.
		const float CurrentDeltaTime = DeltaTime.Get();

		// Get the start time
		const uint64_t TimeBegin = Platform::GetEpochTime();

		// Update the application first.
		OnUpdate( CurrentDeltaTime );

#if IS_EDITOR
		// Call update for editor second if editor is available so that the client update afterwards can
		// take any values coming from editor.
		m_Editor->OnUpdate( CurrentDeltaTime );

		// If this is an editor build, there might not be a client running currently.
		// Check if running first.
		if ( m_Client )
#endif // IS_EDITOR
		{
			// Call update for client last.
			m_Client->OnUpdate( CurrentDeltaTime );
		}

		// Commit this frames delta time to the tracker.
		DeltaTime.Commit( ( Platform::GetEpochTime() - TimeBegin ) / 1000000000.0f /*Epoch time is in nanoseconds.*/ );
	}

	// Destroy any active client first.
#if IS_EDITOR
	if ( m_Client )
#endif // IS_EDITOR
	{
		m_Client->OnDestroy();
		m_Client.reset();
	}

#if IS_EDITOR
	// Destroy editor second.
	m_Editor->OnDestroy();
	m_Editor.reset();	
#endif // IS_EDITOR

	// Destroy application layer last.
	OnDestroy();

	// Tear down all on starts.
	Initializer::Teardown();

	return 0;
}