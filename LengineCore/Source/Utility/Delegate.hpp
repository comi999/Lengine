#pragma once
#include <vector>

#include "Invoker.hpp"

template < typename Return, typename... Args >
class Delegate;

namespace std
{
    template < typename T >
    struct is_delegate : public std::false_type {};

    template < typename Return, typename... Args >
    struct is_delegate< Invoker< Return, Args... > > : public std::true_type {};

    template < typename T >
    static constexpr bool is_delegate_v = is_delegate< T >::value;

    template < typename T >
    struct as_delegate { using type = typename as_delegate< function_signature_t< T > >::type; };

    template <>
    struct as_delegate< void > { using type = void; };

    template < typename Return, typename... Args >
    struct as_delegate< Return( Args... ) > { using type = Delegate< Return, Args... >; };

    template < typename Return, typename... Args >
    struct as_delegate< Delegate< Return, Args... > > { using type = Delegate< Return, Args... >; };

    template < typename T >
    using as_delegate_t = typename as_delegate< T >::type;
}

//==========================================================================
// Delegates are a collection of stored invokers.
//==========================================================================
template < typename Return = void, typename... Args >
class Delegate
{
private:

    using InvokerType = Invoker< Return, Args... >;
    using ReturnType = Return;
    using ArgumentTypes = std::tuple< Args... >;
    using ContainerType = std::vector< Invoker< Return, Args... > >;
    using IteratorType = typename ContainerType::iterator;
    using CIteratorType = typename ContainerType::const_iterator;
    using RIteratorType = typename ContainerType::reverse_iterator;
    using CRIteratorType = typename ContainerType::const_reverse_iterator;

public:

    // Create an empty delegate.
    Delegate()
        : m_IsBroadcasting( false )
        , m_Index( -1 )
    {}

    // Copies from a provided delegate.
    Delegate( const Delegate& a_Delegate )
        : m_Invokers( a_Delegate.m_Invokers )
        , m_IsBroadcasting( false )
        , m_Index( -1 )
    {}

    // Moves from a provided delegate.
    Delegate( Delegate&& a_Delegate ) noexcept
        : m_Invokers( std::move( a_Delegate.m_Invokers ) )
        , m_IsBroadcasting( false )
        , m_Index( -1 )
    {
        a_Delegate.m_Index = -1;
    }

    // Add a functor or function to the delegate.
    template < typename T >
    void Add( T&& a_Function ) { m_Invokers.emplace_back( std::forward< T >( a_Function ) ); }

    // Add an instance and member function to the delegate.
    template < auto _Function, typename Object >
    void Add( Object* a_Object, MemberFunction< _Function > a_Function = MemberFunction< _Function >{} ) { m_Invokers.emplace_back( a_Object, a_Function ); }

    // Add a functor or function to the delegate at the given index.
    template < typename T >
    void Add( size_t a_Index, T&& a_Function )
    {
        if ( m_IsBroadcasting && a_Index <= m_Index )
        {
            ++m_Index;
        }

        m_Invokers.emplace( m_Invokers.begin() + a_Index, std::forward< T >( a_Function ) );
    }

    // Add an instance and member function to the delegate at the given index.
    template < auto _Function, typename Object >
    void Add( size_t a_Index, Object* a_Object, MemberFunction< _Function > a_Function = MemberFunction< _Function >{} )
    {
        if ( m_IsBroadcasting && a_Index <= m_Index )
        {
            ++m_Index;
        }

        m_Invokers.emplace_back( a_Object, a_Function );
    }

    // Add a functor or function to the delegate if it isn't already added to the delegate.
    template < typename T >
    void AddUnique( T&& a_Function )
    {
        if ( std::find( m_Invokers.begin(), m_Invokers.end(), a_Function ) != m_Invokers.end() )
        {
            return;
        }

        m_Invokers.emplace_back( std::forward< T >( a_Function ) );
    }

    // Add an instance and member function to the delegate if it isn't already added to the delegate.
    template < auto _Function, typename Object >
    void AddUnique( Object* a_Object, MemberFunction< _Function > a_Function = MemberFunction< _Function >{} )
    {
        if ( std::find( m_Invokers.begin(), m_Invokers.end(), InvokerType( a_Object, a_Function ) ) != m_Invokers.end() )
        {
            return;
        }

        m_Invokers.emplace_back( a_Object, a_Function );
    }

    // Add a functor or function to the delegate if it isn't already added to the delegate, at the given index.
    template < typename T >
    void AddUnique( size_t a_Index, T&& a_Function )
    {
        if ( std::find( m_Invokers.begin(), m_Invokers.end(), a_Function ) != m_Invokers.end() )
        {
            return;
        }

        if ( m_IsBroadcasting && a_Index <= m_Index )
        {
            ++m_Index;
        }

        m_Invokers.emplace( m_Invokers.begin() + a_Index, std::forward< T >( a_Function ) );
    }

    // Add an instance and member function to the delegate if it isn't already added to the delegate, at the given index.
    template < auto _Function, typename Object >
    void AddUnique( size_t a_Index, Object* a_Object, MemberFunction< _Function > a_Function = MemberFunction< _Function >{} )
    {
        if ( std::find( m_Invokers.begin(), m_Invokers.end(), InvokerType( a_Object, a_Function ) ) != m_Invokers.end() )
        {
            return;
        }

        if ( m_IsBroadcasting && a_Index <= m_Index )
        {
            ++m_Index;
        }

        m_Invokers.emplace_back( a_Object, a_Function );
    }

    // Remove a functor or function from the delegate.
    template < typename T >
    void Remove( T&& a_Function )
    {
        auto Found = std::find( m_Invokers.begin(), m_Invokers.end(), a_Function );

        if ( Found != m_Invokers.end() )
        {
            if ( m_IsBroadcasting && ( Found - m_Invokers.begin() ) <= m_Index )
            {
                --m_Index;
            }

            *Found = m_Invokers.back();
            m_Invokers.pop_back();
        }
    }

    // Remove an instance and member function from the delegate.
    template < auto _Function, typename Object >
    void Remove( Object* a_Object, MemberFunction< _Function > a_Function = MemberFunction< _Function >{} )
    {
        auto Found = std::find( m_Invokers.begin(), m_Invokers.end(), InvokerType( a_Object, a_Function ) );

        if ( Found != m_Invokers.end() )
        {
            if ( m_IsBroadcasting && ( Found - m_Invokers.begin() ) <= m_Index )
            {
                --m_Index;
            }

            *Found = m_Invokers.back();
            m_Invokers.pop_back();
        }
    }

    // Remove an invoker from the delegate at the given index.
    void Remove( size_t a_Index )
    {
        if ( m_IsBroadcasting && a_Index <= m_Index )
        {
            --m_Index;
        }

        m_Invokers[ a_Index ] = m_Invokers.back();
        m_Invokers.pop_back();
    }

    // Remove all invokers from the delegate that match the given functor or function.
    template < typename T >
    void RemoveAll( T&& a_Function )
    {
        for ( int32_t i = ( int32_t )m_Invokers.size() - 1; i > m_Index; --i )
        {
            if ( m_Invokers[ i ] == a_Function )
            {
                m_Invokers[ i ] = m_Invokers.back();
                m_Invokers.pop_back();
            }
        }

        for ( int32_t i = m_Index; i >= 0; --i )
        {
            if ( m_Invokers[ i ] == a_Function )
            {
                m_Invokers[ i ] = m_Invokers.back();
                m_Invokers.pop_back();
                --m_Index;
            }
        }
    }

    // Remove all invokers from the delegate that match the given instance and member function.
    template < auto _Function, typename Object >
    void RemoveAll( Object* a_Object, MemberFunction< _Function > a_Function = MemberFunction< _Function >{} )
    {
        InvokerType Check( a_Object, a_Function );

        for ( uint32_t i = m_Invokers.size() - 1; i > m_Index; --i )
        {
            if ( m_Invokers[ i ] == Check )
            {
                m_Invokers[ i ] = m_Invokers.back();
                m_Invokers.pop_back();
            }
        }

        for ( uint32_t i = m_Index; i >= 0; --i )
        {
            if ( m_Invokers[ i ] == Check )
            {
                m_Invokers[ i ] = m_Invokers.back();
                m_Invokers.pop_back();
                --m_Index;
            }
        }
    }

    // Call all contained invokers with the given arguments.
    void Broadcast( Args... a_Args ) const
    {
        ASSERT( !m_IsBroadcasting );

        m_IsBroadcasting = true;
        m_Index = 0;

        for ( ; m_Index < m_Invokers.size(); ++m_Index )
        {
            ( void )m_Invokers[ m_Index ].Invoke( std::forward< Args >( a_Args )... );
        }

        m_IsBroadcasting = false;
        m_Index = -1;
    }

    // Call all contained invokers with the given arguments. Will ignore unset invokers.
    void BroadcastSafe( Args... a_Args ) const
    {
        ASSERT( !m_IsBroadcasting );

        m_IsBroadcasting = true;
        m_Index = 0;

        for ( ; m_Index < m_Invokers.size(); ++m_Index )
        {
            ( void )m_Invokers[ m_Index ].InvokeSafe( std::forward< Args >( a_Args )... );
        }

        m_IsBroadcasting = false;
        m_Index = -1;
    }

    // Call all contained invokers with the given arguments. Invokers will be called unsafely.
    void operator()( Args... a_Args ) const
    {
        if ( m_IsBroadcasting )
        {
            return;
        }

        m_IsBroadcasting = true;
        m_Index = 0;

        for ( ; m_Index < m_Invokers.size(); ++m_Index )
        {
            ( void )m_Invokers[ m_Index ].Invoke( std::forward< Args >( a_Args )... );
        }

        m_IsBroadcasting = false;
        m_Index = -1;
    }

    // Clear the delegate.
    void Clear() { m_Invokers.clear(); m_Index = -1; }

    // Is the delegate currently broadcasting.
    bool IsBroadcasting() const { return m_IsBroadcasting; }

    // The count of stored invokers.
    size_t Size() const { return m_Invokers.size(); }

    // Is this delegate empty?
    bool Empty() const { return m_Invokers.empty(); }

    // Get the collection of all invokers.
    const ContainerType& GetInvocationList() const { return m_Invokers; }

    // Get begin iterator.
    IteratorType Begin() { return m_Invokers.begin(); }

    // Get begin iterator.
    CIteratorType Begin() const { return m_Invokers.begin(); }

    // Get begin iterator.
    CIteratorType CBegin() const { return m_Invokers.cbegin(); }

    // Get reverse begin iterator.
    RIteratorType RBegin() { return m_Invokers.rbegin(); }

    // Get reverse begin iterator.
    CRIteratorType RBegin() const { return m_Invokers.rbegin(); }

    // Get reverse begin iterator.
    CRIteratorType CRBegin() const { return m_Invokers.crbegin(); }

    // Get end iterator.
    IteratorType End() { return m_Invokers.end(); }

    // Get end iterator.
    CIteratorType End() const { return m_Invokers.end(); }

    // Get end iterator.
    CIteratorType CEnd() const { return m_Invokers.cend(); }

    // Get reverse end iterator.
    RIteratorType REnd() { return m_Invokers.rend(); }

    // Get reverse end iterator.
    CRIteratorType REnd() const { return m_Invokers.rend(); }

    // Get reverse end iterator.
    CRIteratorType CREnd() const { return m_Invokers.crend(); }

    // Copy from another delegate.
    Delegate& operator=( const Delegate& a_Delegate )
    {
        m_Invokers = a_Delegate.m_Invokers;
        m_IsBroadcasting = false;
        m_Index = -1;
        return *this;
    }

    // Move from another delegate.
    Delegate& operator=( Delegate&& a_Delegate ) noexcept
    {
        m_Invokers = std::move( a_Delegate.m_Invokers );
        m_IsBroadcasting = false;
        m_Index = -1;
        a_Delegate.m_Index = -1;
        return *this;
    }

    // Add a functor or function object to the delegate.
    template < typename T >
    Delegate& operator+=( T&& a_Function ) { Add( std::forward< T >( a_Function ) ); return *this; }

    // Remove a functor or function object to the delegate.
    template < typename T >
    Delegate& operator-=( T&& a_Function ) { Remove( std::forward< T >( a_Function ) ); return *this; }

    // Get the stored invoker at a given index.
    template < typename T >
    InvokerType& operator[]( size_t a_Index ) { return m_Invokers[ a_Index ]; }

    // Get the stored invoker at a given index.
    template < typename T >
    const InvokerType& operator[]( size_t a_Index ) const { return m_Invokers[ a_Index ]; }

protected:

    ContainerType   m_Invokers;
    mutable bool    m_IsBroadcasting;
    mutable int32_t m_Index;
};

enum class EConditionType
{
	OR, AND
};

//==========================================================================
// Conmditions are a collection of stored invokers. Will evaluate using a
// a set logic operation.
//==========================================================================
template < typename Return = bool, typename... Args >
class Condition : public Delegate< Return, Args... >
{
public:

    static_assert( !std::is_void_v< Return >, "Condition return type must not be void." );

    using BaseType = Delegate< Return, Args... >;
    using EvaluatorType = Predicate< Return >;

    // Create an empty delegate.
    Condition()
        : BaseType()
		, m_Condition( EConditionType::OR )
    {}

    Condition( const Condition& ) = default;
    Condition( Condition&& ) = default;

    explicit Condition( const EConditionType a_ConditionType )
	    : BaseType()
		, m_Condition( a_ConditionType )
    {}

    Condition& operator=( const Condition& ) = default;
    Condition& operator=( Condition&& ) = default;

    // Get the evaluation logic for this condition.
    EConditionType GetCondition() const { return m_Condition; }

    // Set the evaluation logic for this condition.
    void SetCondition( const EConditionType a_Condition ) { m_Condition = a_Condition; }

    // Set the evaluator. This is useful for when the return type is not convertible to bool.
    template < typename _Evaluator >
    void SetEvaluator( _Evaluator&& a_Evaluator )
    {
        m_Evaluator = std::forward< _Evaluator >( a_Evaluator );
    }

    // Does this condition have an evaluator set?
    bool IsEvaluatorSet() const { return m_Evaluator.IsBound(); }

    // Evaluate condition by calling all contained invokers with the given arguments.
    // Will use evaluator if set. If no evaluator set, will cast return type as bool.
    bool Evaluate( Args... a_Args ) const
    {
        ASSERT( !this->m_IsBroadcasting );
        ASSERT( ( m_Evaluator.IsBound() || std::is_convertible_v< Return, bool > ) );

        this->m_IsBroadcasting = true;
        this->m_Index = 0;
        bool Success = false;

        if ( !m_Evaluator.IsBound() )
        {
            switch ( m_Condition )
            {
            case EConditionType::OR:
            {
                for ( ; this->m_Index < this->m_Invokers.size(); ++this->m_Index )
                {
                    if ( ( bool )this->m_Invokers[ this->m_Index ].Invoke( std::forward< Args >( a_Args )... ) )
                    {
                        Success = true;
                        break;
                    }
                }

                break;
            }
            case EConditionType::AND:
            {
                for ( ; this->m_Index < this->m_Invokers.size(); ++this->m_Index )
                {
                    if ( !( bool )this->m_Invokers[ this->m_Index ].Invoke( std::forward< Args >( a_Args )... ) )
                    {
                        break;
                    }
                }

                break;
            }
            }
        }
        else
        {
            switch ( m_Condition )
            {
            case EConditionType::OR:
            {
                for ( ; this->m_Index < this->m_Invokers.size(); ++this->m_Index )
                {
                    if ( m_Evaluator( this->m_Invokers[ this->m_Index ].Invoke( std::forward< Args >( a_Args )... ) ) )
                    {
                        Success = true;
                        break;
                    }
                }

                break;
            }
            case EConditionType::AND:
            {
                for ( ; this->m_Index < this->m_Invokers.size(); ++this->m_Index )
                {
                    if ( !m_Evaluator( this->m_Invokers[ this->m_Index ].Invoke( std::forward< Args >( a_Args )... ) ) )
                    {
                        break;
                    }
                }

                break;
            }
            }
        }

        this->m_IsBroadcasting = false;
        this->m_Index = -1;
        return Success;
    }

    // Evaluate condition by calling all contained invokers with the given arguments.
	// Will use evaluator if set. If no evaluator set, will cast return type as bool. Will ignore unset invokers.
    bool EvaluateSafe( Args... a_Args ) const
    {
        ASSERT( !this->m_IsBroadcasting );
        ASSERT( ( m_Evaluator.IsBound() || std::is_convertible_v< Return, bool > ) );

        this->m_IsBroadcasting = true;
        this->m_Index = 0;
        bool Success = false;

        if ( !m_Evaluator.IsBound() )
        {
            switch ( m_Condition )
            {
            case EConditionType::OR:
            {
                for ( ; this->m_Index < this->m_Invokers.size(); ++this->m_Index )
                {
                    if ( ( bool )this->m_Invokers[ this->m_Index ].InvokeSafe( std::forward< Args >( a_Args )... ) )
                    {
                        Success = true;
                        break;
                    }
                }

                break;
            }
            case EConditionType::AND:
            {
                for ( ; this->m_Index < this->m_Invokers.size(); ++this->m_Index )
                {
                    if ( !( bool )this->m_Invokers[ this->m_Index ].InvokeSafe( std::forward< Args >( a_Args )... ) )
                    {
                        break;
                    }
                }

                break;
            }
            }
        }
        else
        {
            switch ( m_Condition )
            {
            case EConditionType::OR:
            {
                for ( ; this->m_Index < this->m_Invokers.size(); ++this->m_Index )
                {
                    if ( m_Evaluator( this->m_Invokers[ this->m_Index ].InvokeSafe( std::forward< Args >( a_Args )... ) ) )
                    {
                        Success = true;
                        break;
                    }
                }

                break;
            }
            case EConditionType::AND:
            {
                for ( ; this->m_Index < this->m_Invokers.size(); ++this->m_Index )
                {
                    if ( !m_Evaluator( this->m_Invokers[ this->m_Index ].InvokeSafe( std::forward< Args >( a_Args )... ) ) )
                    {
                        break;
                    }
                }

                break;
            }
            }
        }

        this->m_IsBroadcasting = false;
        this->m_Index = -1;
        return Success;
    }

    // Call all contained invokers with the given arguments. Invokers will be called unsafely.
    bool operator()( Args... a_Args ) const
    {
        ASSERT( !this->m_IsBroadcasting );
        ASSERT( ( m_Evaluator.IsBound() || std::is_convertible_v< Return, bool > ) );

        this->m_IsBroadcasting = true;
        this->m_Index = 0;
        bool Success = false;

        if ( !m_Evaluator.IsBound() )
        {
            switch ( m_Condition )
            {
            case EConditionType::OR:
            {
                for ( ; this->m_Index < this->m_Invokers.size(); ++this->m_Index )
                {
                    if ( ( bool )this->m_Invokers[ this->m_Index ].Invoke( std::forward< Args >( a_Args )... ) )
                    {
                        Success = true;
                        break;
                    }
                }

                break;
            }
            case EConditionType::AND:
            {
                for ( ; this->m_Index < this->m_Invokers.size(); ++this->m_Index )
                {
                    if ( !( bool )this->m_Invokers[ this->m_Index ].Invoke( std::forward< Args >( a_Args )... ) )
                    {
                        break;
                    }
                }

                break;
            }
            }
        }
        else
        {
            switch ( m_Condition )
            {
            case EConditionType::OR:
            {
                for ( ; this->m_Index < this->m_Invokers.size(); ++this->m_Index )
                {
                    if ( m_Evaluator( this->m_Invokers[ this->m_Index ].InvokeSafe( std::forward< Args >( a_Args )... ) ) )
                    {
                        Success = true;
                        break;
                    }
                }

                break;
            }
            case EConditionType::AND:
            {
                for ( ; this->m_Index < this->m_Invokers.size(); ++this->m_Index )
                {
                    if ( !m_Evaluator( this->m_Invokers[ this->m_Index ].InvokeSafe( std::forward< Args >( a_Args )... ) ) )
                    {
                        break;
                    }
                }

                break;
            }
            }
        }

        this->m_IsBroadcasting = false;
        this->m_Index = -1;
        return Success;
    }

protected:

    EvaluatorType m_Evaluator;
    EConditionType m_Condition;

};

namespace std
{
    template < typename T >
    static auto make_delegate( T&& a_Object ) { return std::move( as_delegate_t< remove_pointer_t< remove_reference_t< T > > >() += forward< T >( a_Object ) ); }

    template < auto _Function, typename T >
    static auto make_delegate( T&& a_Object, MemberFunction< _Function > a_Function = MemberFunction< _Function >{} )
    {
        using FunctionType = decltype( _Function );
        return std::move( as_delegate_t< FunctionType >() += as_invoker_t< FunctionType >( forward< T >( a_Object ), a_Function ) );
    }

    template < typename Return, typename... Args > auto empty( const Delegate< Return, Args... >& a_Delegate ) { return a_Delegate.Empty(); }
    template < typename Return, typename... Args > auto size( const Delegate< Return, Args... >& a_Delegate ) { return a_Delegate.Size(); }
    template < typename Return, typename... Args > auto begin( Delegate< Return, Args... >& a_Delegate ) { return a_Delegate.Begin(); }
    template < typename Return, typename... Args > auto begin( const Delegate< Return, Args... >& a_Delegate ) { return a_Delegate.Begin(); }
    template < typename Return, typename... Args > auto cbegin( const Delegate< Return, Args... >& a_Delegate ) { return a_Delegate.CBegin(); }
    template < typename Return, typename... Args > auto rbegin( Delegate< Return, Args... >& a_Delegate ) { return a_Delegate.RBegin(); }
    template < typename Return, typename... Args > auto rbegin( const Delegate< Return, Args... >& a_Delegate ) { return a_Delegate.RBegin(); }
    template < typename Return, typename... Args > auto crbegin( const Delegate< Return, Args... >& a_Delegate ) { return a_Delegate.CRBegin(); }
    template < typename Return, typename... Args > auto end( Delegate< Return, Args... >& a_Delegate ) { return a_Delegate.End(); }
    template < typename Return, typename... Args > auto end( const Delegate< Return, Args... >& a_Delegate ) { return a_Delegate.End(); }
    template < typename Return, typename... Args > auto cend( const Delegate< Return, Args... >& a_Delegate ) { return a_Delegate.CEnd(); }
    template < typename Return, typename... Args > auto rend( Delegate< Return, Args... >& a_Delegate ) { return a_Delegate.REnd(); }
    template < typename Return, typename... Args > auto rend( const Delegate< Return, Args... >& a_Delegate ) { return a_Delegate.REnd(); }
    template < typename Return, typename... Args > auto crend( const Delegate< Return, Args... >& a_Delegate ) { return a_Delegate.CREnd(); }
}