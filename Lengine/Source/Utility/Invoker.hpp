#pragma once
#include <vector>
#include <Utility/FunctionTraits.hpp>

template < typename Return, typename... Args >
class Invoker;

namespace std
{
    template < typename T >
    struct is_invoker : public std::false_type {};

    template < typename Return, typename... Args >
    struct is_invoker< Invoker< Return, Args... > > : public std::true_type {};

    template < typename T >
    static constexpr bool is_invoker_v = is_invoker< T >::value;

    template < typename T >
    struct is_action : public std::false_type {};

    template < typename... Args >
    struct is_action< Invoker< void, Args... > > : public std::true_type {};

    template < typename T >
    static constexpr bool is_action_v = is_action< T >::value;

    template < typename T >
    struct is_predicate : public std::false_type {};

    template < typename... Args >
    struct is_predicate< Invoker< bool, Args... > > : public std::true_type {};

    template < typename T >
    static constexpr bool is_predicate_v = is_predicate< T >::value;

    template < typename T >
    struct as_invoker { using type = typename as_invoker< function_signature_t< T > >::type; };

    template <>
    struct as_invoker< void > { using type = void; };

    template < typename Return, typename... Args >
    struct as_invoker< Return( Args... ) > { using type = Invoker< Return, Args... >; };

    template < typename Return, typename... Args >
    struct as_invoker< Invoker< Return, Args... > > { using type = Invoker< Return, Args... >; };

    template < typename T >
    using as_invoker_t = typename as_invoker< T >::type;
}

// Static storage object for a member function. Use as MemberFunction<&Object::Member>{}.
template < auto _Function, typename _Signature = std::function_signature_t< decltype( _Function ) > >
struct MemberFunction
{
    static_assert( std::is_member_function_v< decltype( _Function ) >, "Only member functions can be stored in MemberFunction." );

    using TraitsType = std::function_traits< decltype( _Function ) >;
    using ReturnType = typename TraitsType::return_type;
    using ObjectType = typename TraitsType::object_type;
    using ArgumentsType = typename TraitsType::arguments_type;
    using SignatureType = _Signature;

    static constexpr auto Value = _Function;
};

// Static storage object for a static function. Use as StaticFunction<&Object::Static>{},
// StaticFunction<Object::Static>{} or StaticFunction<Static>{}
template < auto _Function, typename _Signature = std::function_signature_t< decltype( _Function ) > >
struct StaticFunction
{
    static_assert( std::is_static_function_v< decltype( _Function ) >, "Only static functions can be stored in StaticFunction." );

	using TraitsType = std::function_traits< decltype( _Function ) >;
    using ReturnType = typename TraitsType::return_type;
    using ObjectType = typename TraitsType::object_type;
    using ArgumentsType = typename TraitsType::arguments_type;
    using SignatureType = _Signature;

    static constexpr auto Value = _Function;
};

// Helpers for invoker types.
namespace InvokerHelpers
{
    using LambdaTagType = uintptr_t;
    using LambdaProcessorType = void( * )( void*&, bool );
    using LambdaOffsetType = size_t;

    static constexpr LambdaTagType LambdaStorageTag = ( LambdaOffsetType )0xABCDEFABCDEFABCD;
    static constexpr LambdaOffsetType LambdaTagOffset = 0u;
    static constexpr LambdaOffsetType LambdaProcessorOffset = LambdaTagOffset + sizeof( LambdaTagOffset );
    static constexpr LambdaOffsetType LambdaOffset = LambdaProcessorOffset + sizeof( LambdaProcessorOffset );

    template < typename T >
    static void LambdaProcessor( void*&, bool );

    template < typename T >
    struct LambdaStorage
    {
        using LambdaType = T;

        LambdaTagType Tag = LambdaStorageTag;
        LambdaProcessorType Processor = LambdaProcessor< T >;
        LambdaType Lambda;

        template < typename... Args >
        LambdaStorage( Args&&... a_Args )
            : Lambda( std::forward< Args >( a_Args )... )
        {}
    };

    template < typename T >
    static void LambdaProcessor( void*& a_Pointer, bool a_Copy )
    {
        if ( a_Copy )
        {
            a_Pointer = new LambdaStorage< T >( ( ( LambdaStorage< T >*& )a_Pointer )->Lambda );
        }
        else
        {
            delete ( LambdaStorage< T >*& )a_Pointer;
        }
    }

    static bool IsLambdaStorage( void* a_Pointer )
    {
        return *( LambdaTagType* )a_Pointer == LambdaStorageTag;
    }

    static void* GetLambda( void* a_Pointer )
    {
        return ( uint8_t* )a_Pointer + LambdaOffset;
    }

    static LambdaProcessorType GetLambdaProcessor( void* a_Pointer )
    {
        return *( LambdaProcessorType* )( ( uint8_t* )a_Pointer + LambdaProcessorOffset );
    }

    static void DestroyLambdaStorage( void* a_Pointer )
    {
        GetLambdaProcessor( a_Pointer )( a_Pointer, false );
    }

    static void CopyLambdaStorage( void*& a_Pointer )
    {
        GetLambdaProcessor( a_Pointer )( a_Pointer, true );
    }
}

//==========================================================================
// An invoker is a function object that can be bound to the following:
// - Static functions
// - Member functions and accompanying objects
// - Static lambda
// - Capture lambda
// - Invocable object.
//==========================================================================
template < typename Return = void, typename... Args >
class Invoker
{
private:

    template < typename, typename... > friend class Invoker;
    template < typename, typename... > friend class Delegate;

    template < auto _Function, bool _UsingLambdaStorage = false >
    static Return Invocation( void* a_Object, Args... a_Args )
    {
        using ObjectType = std::function_object_t< decltype( _Function ) >;

        if constexpr ( _UsingLambdaStorage )
        {
            a_Object = InvokerHelpers::GetLambda( a_Object );
        }

        return ( static_cast< ObjectType* >( a_Object )->*_Function )( std::forward< Args >( a_Args )... );
    }

public:

    // Create an empty invoker.
    Invoker()
        : m_Object( nullptr )
        , m_Function( nullptr )
    {}

    // Create an empty invoker.
    Invoker( std::nullptr_t )
        : Invoker()
    {}

    // Copy from another Invoker.
    Invoker( const Invoker& a_Invoker ) : Invoker() { Bind( a_Invoker ); }

    // Move from another Invoker.
    Invoker( Invoker&& a_Invoker ) noexcept : Invoker() { Bind( std::move( a_Invoker ) ); }

    // Create from lambda, functor or static funciton.
    template < typename T >
    Invoker( T&& a_Object ) : Invoker() { Bind( std::forward< T >( a_Object ) ); }

    // Create from an object and member function pair.
    template < typename T, auto _Function >
    Invoker( T&& a_Object, MemberFunction< _Function > ) : Invoker() { Bind( std::forward< T >( a_Object ), MemberFunction< _Function >{} ); }

    // Clear invoker binding.
    ~Invoker() { Unbind(); }

    // Clear invoker binding.
    void Bind( std::nullptr_t ) { Unbind(); }

    // Bind lambda, functor or static function to Invoker. Will move from and store r-value referenced objects.
    template < typename T >
    void Bind( T&& a_Object )
    {
        using ObjectType = std::decay_t< T >;
        using StaticFunction = Return( * )( Args... );

        // If binding a static function.
        if constexpr ( std::is_convertible_v< ObjectType, StaticFunction > )
        {
            Unbind();
            m_Function = ( void* )static_cast< StaticFunction >( a_Object );
        }

        // If binding a pointer, rebind as a reference.
        else if constexpr ( std::is_pointer_v< ObjectType > )
        {
            Bind( *a_Object );
        }

        // If binding another Invoker, copy or move from it.
        else if constexpr ( std::is_invoker_v< ObjectType > )
        {
            static_assert( std::is_same_v< ObjectType, Invoker >, "Invoker must have the same argument types." );

            Unbind();
            m_Function = a_Object.m_Function;

            if constexpr ( std::is_rvalue_reference_v< decltype( a_Object ) > )
            {
                m_Object = a_Object.m_Object;
                a_Object.m_Object = nullptr;
                a_Object.m_Function = nullptr;
            }
            else
            {
                if ( a_Object.m_Object && InvokerHelpers::IsLambdaStorage( a_Object.m_Object ) )
                {
                    InvokerHelpers::CopyLambdaStorage( m_Object = a_Object.m_Object );
                }
                else
                {
                    m_Object = a_Object.m_Object;
                }
            }
        }

        // Lambda or Object/Member
        else
        {
            Bind< &ObjectType::operator() >( std::forward< T >( a_Object ) );
        }
    }

    // Bind an object and member function pair to Invoker. Will move from and store r-value referenced objects.
    template < auto _Function, typename T >
    void Bind( T&& a_Object, MemberFunction< _Function > = MemberFunction< _Function >{} )
    {
        using ObjectType = std::decay_t< T >;

        static_assert( std::is_member_function_compatible_v< decltype( _Function ), std::remove_pointer_t< std::remove_reference_t< T > > >, "Function type is not callable on given object." );

        Unbind();

        // If pointer, store reference to object and function.
        if constexpr ( std::is_pointer_v< ObjectType > )
        {
            m_Object = a_Object;
            m_Function = ( void* )Invocation< _Function >;
        }
        
        // If r-value reference, move from object to lambda storage and store accompanying function.
        else if constexpr ( std::is_rvalue_reference_v< decltype( a_Object ) > )
        {
            m_Object = new InvokerHelpers::LambdaStorage< ObjectType >( std::move( a_Object ) );
            m_Function = ( void* )Invocation< _Function, true >;
        }

        // Store reference to object and function.
        else
        {
            m_Object = &a_Object;
            m_Function = ( void* )Invocation< _Function >;
        }
    }

    // Clear invoker binding.
    void Unbind()
    {
        if ( !IsBound() )
        {
            return;
        }

        m_Function = nullptr;

        if ( m_Object && InvokerHelpers::IsLambdaStorage( m_Object ) )
        {
            InvokerHelpers::DestroyLambdaStorage( m_Object );
        }

        m_Object = nullptr;
    }

    // Is the invoker bound to a functor or function?
    bool IsBound() const { return m_Function; }

    // Is the bound function, if any at all, a static function?
    bool IsStatic() const { return IsBound() && !m_Object; }

    // Is the bound function, if any at all, a stored function? This applies to capture-lambda's, moved from object/member function pairs or other moved from functors.
    bool IsLambda() const { return m_Object && InvokerHelpers::IsLambdaStorage( m_Object ); }

    // Invoke the stored callable.
    Return Invoke( Args... a_Args ) const
    {
        return m_Object ?
            reinterpret_cast< Return( * )( void*, Args... ) >( m_Function )( m_Object, std::forward< Args >( a_Args )... ) :
            reinterpret_cast< Return( * )( Args... ) >( m_Function )( std::forward< Args >( a_Args )... );
    }

    // Invoke the stored callable if it is bound. If not, default Return type will be returned.
    Return InvokeSafe( Args... a_Args ) const
    {
        if ( !m_Function )
        {
            return Return();
        }

        return m_Object ?
            reinterpret_cast< Return( * )( void*, Args... ) >( m_Function )( m_Object, std::forward< Args >( a_Args )... ) :
            reinterpret_cast< Return( * )( Args... ) >( m_Function )( std::forward< Args >( a_Args )... );
    }

    // Invoke the stored callable. Will not check if invoker is bound beforehand.
    Return operator()( Args... a_Args ) const
    {
        return m_Object ?
            reinterpret_cast< Return( * )( void*, Args... ) >( m_Function )( m_Object, std::forward< Args >( a_Args )... ) :
            reinterpret_cast< Return( * )( Args... ) >( m_Function )( std::forward< Args >( a_Args )... );
    }

    // Is the invoker bound to a functor or function?
    operator bool() const { return m_Function; }

    // Checks to see if the invoker is bound to the same functor or function and instance as another invoker.
    bool operator==( const Invoker& a_Invoker ) const { return m_Function == a_Invoker.m_Function && m_Object == a_Invoker.m_Object; }

    // Checks to see if the invokers bound function is the same as given static function.
    bool operator==( Return( *a_Function )( Args... ) ) const { return m_Function == a_Function; }

    // Checks to see if the invokers bound object is the same as the given object.
    template < typename T >
    bool operator==( T* a_Object ) const { return m_Object == a_Object; }

    // Checks to see if the invoker is bound at all. Same as !IsBound and operator bool.
    bool operator==( std::nullptr_t ) const { return !IsBound(); }

    // Checks to see if the invoker is bound to the same functor or function and instance as the one given.
    template < typename T >
    bool operator==( T&& a_Object ) const { return *this == Invoker( std::forward< T >( a_Object ) ); }

    // Checks to see if the invoker is bound to the same member function as the one provided.
    template < auto _Function >
    bool operator==( MemberFunction< _Function > ) const { return m_Function == static_cast< void* >( Invocation< _Function > ); }

    // Checks to see if the invoker is bound to a different function or functor than the one given.
    template < typename T >
    bool operator!=( T&& a_Object ) const { return !( *this == std::forward< T >( a_Object ) ); }

    // Clear invoker binding. Same as Unbind.
    Invoker& operator=( std::nullptr_t ) { Unbind(); return *this; }

    // Copy from an invoker.
    Invoker& operator=( const Invoker& a_Invoker ) { Bind( a_Invoker ); return *this; }

    // Move from an invoker.
    Invoker& operator=( Invoker&& a_Invoker ) { Bind( std::move( a_Invoker ) ); return *this; }

    // Assign a functor or function to the invoker.
    template < typename T >
    Invoker& operator=( T&& a_Object ) { Bind( std::forward< T >( a_Object ) ); return *this; }

private:

    void* m_Object;
    void* m_Function;
};

// An Action is an invoker that returns void.
template < typename... Args >
using Action = Invoker< void, Args... >;

// A Predicate is an invoker that returns bool.
template < typename... Args >
using Predicate = Invoker< bool, Args... >;

namespace std
{
    template < typename T >
    static auto make_invoker( T&& a_Object ) { return as_invoker_t< remove_pointer_t< remove_reference_t< T > > >( forward< T >( a_Object ) ); }

    template < auto _Function, typename T >
    static auto make_invoker( T&& a_Object, MemberFunction< _Function > a_Function = MemberFunction< _Function >{} ) { return as_invoker_t< decltype( _Function ) >( forward< T >( a_Object ), a_Function ); }
}