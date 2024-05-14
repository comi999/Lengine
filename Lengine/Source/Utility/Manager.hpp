#pragma once
#include <type_traits>
#include <Core/Core.hpp>
#include <Utility/Delegate.hpp>
#include <Utility/Singleton.hpp>
#include <Utility/Initializer.hpp>
#include <Utility/ClassTraits.hpp>

//==========================================================================
template < typename T >
class Manager;

//==========================================================================
class ManagerCore
{
	template < class > friend class Manager;

	static Delegate<> OnStartup;
	static Delegate< void, float > OnUpdate;
	static Delegate<> OnRestart;
	static Delegate<> OnShutdown;

	static Delegate<> Destructors;

	DEFINE_INITIALIZER_IN_PLACE( ManagerCoreInitialisation, CoreInitialisation )
	{
		if ( ActionType == EInitializerActionType::Startup )
		{
			CoreDelegates::OnStartup.Add( OnStartup );
			CoreDelegates::OnUpdate.Add( OnUpdate );
			CoreDelegates::OnRestart.Add( OnRestart );
			CoreDelegates::OnShutdown.Add( OnShutdown );
		}
	};

	template < typename T >
	struct HasManagerFunctions
	{
#define HAS_MANAGER_FUNCTION( Function, Return, ... ) \
		template < typename U > static auto Has##Function##Test( std::enable_if_t< std::is_same_v< decltype( std::declval< U& >().Function( __VA_ARGS__ ) ), Return > >* ) -> std::true_type; \
		template < typename U > static auto Has##Function##Test( ... ) -> std::false_type; \
		static constexpr bool Has##Function = decltype( Has##Function##Test< T >( nullptr ) )::value;

		HAS_MANAGER_FUNCTION( OnStartup, void );
		HAS_MANAGER_FUNCTION( OnShutdown, void );
		HAS_MANAGER_FUNCTION( OnRestart, void );
		HAS_MANAGER_FUNCTION( OnUpdate, void, std::declval< float& >() );

#undef HAS_MANAGER_FUNCTION
	};

	template < typename T >
	static void RegisterManager( T* a_Manager )
	{
		using Type = std::decay_t< T >;

#define ADD_MANAGER_FUNCTION( Function ) \
	if constexpr ( HasManagerFunctions< Type >::Has##Function ) { CoreDelegates::Function.Add< &Type::Function >( a_Manager ); }

		ADD_MANAGER_FUNCTION( OnStartup );
		ADD_MANAGER_FUNCTION( OnShutdown );
		ADD_MANAGER_FUNCTION( OnRestart );
		ADD_MANAGER_FUNCTION( OnUpdate );

#undef ADD_MANAGER_FUNCTION

		Destructors += T::Destroy;
	}

public:

	static void DestroyAll()
	{
		Destructors.Broadcast();
	}
};

//==========================================================================
template < typename T >
class Manager : public ISingleton< T, false >
{
	using BaseType = ISingleton< T, false >;

	DEFINE_INITIALIZER_IN_PLACE_ANONYMOUS( ManagerCoreInitialization )
	{
		ManagerCore::RegisterManager( BaseType::Get() );
	};
};