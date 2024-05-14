#pragma once
#include <string>
#include <list>
#include <Utility/Hash.hpp>

using std::string;

//==========================================================================
#ifdef _DEBUG
// Force name strings to stick around for human-friendly debugging purposes.
#define ENABLE_NAME_STORAGE
#endif

//==========================================================================
class Name
{
#ifdef ENABLE_NAME_STORAGE
    static auto& GetNameStorage()
    {
        static std::list< string > Storage; return Storage;
    }
#endif

public:

    static const Name Null;

    Name() = default;
    constexpr Name( const Name& ) = default;
    constexpr Name( Name&& ) = default;

    template < size_t _Length >
    constexpr Name( const char( &a_String )[ _Length ] )
        : m_String( a_String )
        , m_Length( _Length - 1u )
        , m_Hash( Hashing::Hash( a_String ) )
    {}

    constexpr Name( const char* a_String, const size_t a_Length, const hash_t a_Hash )
        : m_String( a_String )
        , m_Length( a_Length )
        , m_Hash( a_Hash )
    {}

    Name( const char* a_String )
        : m_String( a_String )
        , m_Length( a_String ? strlen( a_String ) : 0u )
        , m_Hash( a_String ? Hashing::Hash( a_String ) : 0 )
    {
#ifdef ENABLE_NAME_STORAGE
        if ( a_String )
        {
            GetNameStorage().push_back( a_String );
            m_String = GetNameStorage().back().c_str();
        }
#endif
    }

    Name( const char* a_String, const size_t a_Length )
        : m_String( a_String )
        , m_Length( a_Length )
        , m_Hash( Hashing::Hash( a_String ) )
    {
#ifdef ENABLE_NAME_STORAGE
        GetNameStorage().push_back( a_String );
        m_String = GetNameStorage().back().c_str();
#endif
    }

    Name( const std::string& a_String )
        : m_String( a_String.c_str() )
        , m_Length( a_String.length() )
        , m_Hash( Hashing::Hash( a_String ) )
    {
#ifdef ENABLE_NAME_STORAGE
        GetNameStorage().push_back( a_String );
        m_String = GetNameStorage().back().c_str();
#endif
    }

    Name( const hash_t a_Hash )
        : m_String( nullptr )
		, m_Length( 0u )
		, m_Hash( a_Hash ) 
    {}

    operator std::string() const { return { m_String, m_Length }; }
    operator std::string_view() const { return { m_String, m_Length }; }
    operator hash_t() const { return m_Hash; }
    operator const char*() const { return m_String; }
    constexpr char operator []( const size_t a_Pos ) const { return m_String[ a_Pos ]; }

    [[nodiscard]] constexpr const char* CStr() const { return m_String; }
    [[nodiscard]] constexpr size_t Length() const { return m_Length; }
    [[nodiscard]] constexpr hash_t Hash() const { return m_Hash; }
    constexpr Name& operator=( const Name& ) = default;
    constexpr Name& operator=( Name&& ) = default;

    constexpr bool operator==( const hash_t a_Hash ) const { return m_Hash == a_Hash; }
    constexpr bool operator==( const Name& a_Name ) const { return m_Hash == a_Name.m_Hash; }
    constexpr bool operator==( const char* a_Name ) const { return ( std::string_view )*this == a_Name; }
    constexpr bool operator==( const std::string& a_Name ) const { return ( std::string_view )*this == a_Name; }
    constexpr bool operator==( const std::string_view& a_Name ) const { return ( std::string_view )*this == a_Name; }
    constexpr bool operator!=( const hash_t a_Hash ) const { return m_Hash != a_Hash; }
    constexpr bool operator!=( const Name& a_Name ) const { return m_Hash != a_Name.m_Hash; }
    constexpr bool operator!=( const char* a_Name ) const { return ( std::string_view )*this != a_Name; }
    constexpr bool operator!=( const std::string& a_Name ) const { return ( std::string_view )*this != a_Name; }
    constexpr bool operator!=( const std::string_view& a_Name ) const { return ( std::string_view )*this != a_Name; }

private:

    const char* m_String;
    size_t m_Length;
    hash_t m_Hash;
};

string ToString( const Name& a_Object, const char* a_Style = nullptr );

constexpr Name operator""_N( const char* a_String, const size_t a_Length )
{
    return { a_String, a_Length, Hashing::Hash(a_String, a_Length) };
}

template<>
struct std::hash< Name >
{
    size_t operator()( const Name& a_Value ) const noexcept { return a_Value.Hash(); }
};