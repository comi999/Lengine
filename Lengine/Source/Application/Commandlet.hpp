#pragma once
#if IS_EDITOR
#include <string>
#include <map>
#include <memory>
#include <Utility/Initializer.hpp>

using std::string;
using std::map;
using std::shared_ptr;

using std::make_shared;

#define DEFINE_COMMANDLET( Name, Alias ) \
	class Name; \
	DEFINE_INITIALIZER_IN_PLACE( Name ) \
	{ \
		( void )Commandlet::RegisterCommandlet< Name >( Alias ); \
	}; \
	class Name : public Commandlet

#define DEFINE_COMMANDLET_BODY( Name ) \
	Name() \
		: Commandlet( #Name ) \
	{}

class Commandlet
{
public:

	Commandlet( const string& a_Name )
		: m_Name( a_Name )
	{}

	virtual ~Commandlet() = default;

	const string& GetName() const { return m_Name; }

	virtual void Run( const int32_t a_ArgC, const char** a_ArgV ) = 0;

	template < typename _Commandlet >
	static _Commandlet* RegisterCommandlet( const string& a_Name )
	{
		auto& Lookup = GetLookup();
		auto& Entry = Lookup[ a_Name ];
		Entry = make_shared< _Commandlet >();
		return ( _Commandlet* )Entry.get();
	}

	static Commandlet* GetCommandlet( const string& a_Name ) { const auto Iter = GetLookup().find( a_Name ); if ( Iter == GetLookup().end() ) return nullptr; return Iter->second.get(); }

private:

	string m_Name;

	static map< string, shared_ptr< Commandlet > >& GetLookup();
};
#endif