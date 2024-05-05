#pragma once
#include <tuple>
#include <typeinfo>
#include <Utility/Hash.hpp>


#define NON_COPYABLE( Type )\
    Type( const Type& ) = delete;\
    Type& operator=( const Type& ) = delete;

#define NON_MOVABLE( Type )\
    Type( Type&& ) = delete;\
    Type& operator=( Type&& ) = delete;

#define NON_COPYABLE_OR_MOVABLE( Type )\
    NON_COPYABLE( Type )\
    NON_MOVABLE( Type )

//==========================================================================
// Helper for making component classes friends.
//==========================================================================
#define FRIEND_COMPONENT( Name ) \
	friend class Name

//==========================================================================
// Helper for forward declaring component classes.
//==========================================================================
#define FORWARD_DECLARE_CLASS( Name ) \
	class Name

//==========================================================================
// Helper to define a class that is not inherited.
//==========================================================================
#define DEFINE_UNINHERITED_CLASS( Name ) \
	class Name

//==========================================================================
// Helper to define a class that is inherited.
//==========================================================================
#define DEFINE_INHERITED_CLASS( Name, Base ) \
	FORWARD_DECLARE_CLASS( Name ); \
	template <> struct std::base_of< Name > { using type = Base; }; \
	class Name : public Base

//==========================================================================
// Define a class. Provide a second argument as a Base class.
//==========================================================================
#define DEFINE_CLASS( ... ) EXPAND( GET_MACRO_2( __VA_ARGS__, DEFINE_UNINHERITED_CLASS, DEFINE_INHERITED_CLASS )( __VA_ARGS__ ) )

//==========================================================================
// Adds useful internals to the component. Such as .AsBase(), for casting
// this component to its parent component class type.
// e.g. WriteToBuffer( a_Buffer, a_Data.AsBase() ); will write the component's 
// parent-class data.
//==========================================================================
#define DEFINE_CLASS_BODY( Name ) \
	public: \
	using This = Name; \
	using Super = std::base_of_t< Name >; \
    inline Super* GetSuper() { return reinterpret_cast< Super* >( this ); } \
    inline const Super* GetSuper() const { return reinterpret_cast< const Super* >( this ); } \
	private:

namespace std
{
    template < typename T >
    struct base_of { using type = void; };

    template < typename T >
    using base_of_t = typename base_of< T >::type;

    //template < typename T, typename _Super = base_of< T > >
    //struct trace { using type = decltype( std::tuple_cat( std::declval< typename trace< _Super >::type& >(), std::declval< std::tuple< T >& >() ) ); };

    //template < typename T >
    //struct trace< T, void > { using type = std::tuple< T >; };

    //template < typename T >
    //using trace_t = typename trace< T >::type;
}

template < hash_t _Alias >
struct TypeAlias
{
	using Type = void;
};

#define DECLARE_TYPE_ALIAS( Alias, ... ) \
    template <> \
    struct TypeAlias< #Alias##_H > \
    { \
		using Type = __VA_ARGS__; \
    }

template < typename _Alias = void >
struct TypeIndexer
{
    template < typename T >
    static size_t Value()
    {
        auto n = typeid( TypeIndexer< _Alias > ).name();
        static size_t Index = Next();
        return Index;
    }

private:

    static size_t Next()
    {
        static size_t Index = 0;
        return Index++;
    }
};

template < typename _Alias = void >
struct HashIndexer
{
    template < hash_t H >
    static size_t Value()
    {
        static size_t Index = Next();
        return Index;
    }

private:

    static size_t Next()
    {
        static size_t Index = 0;
        return Index++;
    }
};

template < typename _Type, typename _First, typename... Args >
struct TypeExtractor
{
    static constexpr bool IsExtractable = std::is_convertible_v< _First, _Type >;
    static _Type Extract( const _First& a_First, const Args&... ) { return _Type( a_First ); }
};

template < typename _Type >
struct TypeExtractor< _Type, std::tuple<> >
{
    static constexpr bool IsExtractable = false;
};

template < typename _Type, typename... Args >
struct TypeExtractor< _Type, std::tuple< Args... > >
{
    static constexpr bool IsExtractable = std::is_convertible_v< std::tuple_element_t< 0, std::tuple< Args... > >, _Type >;
    static _Type Extract( const std::tuple< Args... >& a_Args ) { return _Type( std::get< 0 >( a_Args ) ); }
};

template < typename T >
struct ClassTraits
{
    static hash_t GetTypeHash()
    {
        static const hash_t Name = Hashing::Hash( typeid( T ).name() );
        return Name;
    }
};

#define DEFINE_ENUM_CHECK( Enum, Member ) \
	namespace Detail \
	{ \
		namespace En##umChecks \
		{ \
			struct does_##Enum##_have_##Member \
			{ \
				template < typename U > static auto test( decltype( U::Member )* ) -> std::true_type; \
                template < typename U > static auto test( ... ) -> std::false_type; \
                static constexpr bool value = decltype( test< Enum >( nullptr ) )::value; \
            }; \
		} \
	}

#define DOES_ENUM_HAVE_MEMBER( Enum, Member ) \
    Detail::En##umChecks::does_##Enum##_have_##Member::value