#pragma once
#include <iterator>

template < typename _Iter, typename _IterCategory = typename std::iterator_traits< _Iter >::iterator_category >
struct DefaultIteratorAdvance {};

template < typename _Iter >
struct DefaultIteratorAdvance< _Iter, std::forward_iterator_tag >
{
    _Iter& Iterator;

    void operator++() const { ( void )++Iterator; }
    _Iter operator++( int ) const { _Iter Temp = Iterator; ( void )++Iterator; return Temp; }
};

template < typename _Iter >
struct DefaultIteratorAdvance< _Iter, std::bidirectional_iterator_tag > : DefaultIteratorAdvance< _Iter, std::forward_iterator_tag >
{
    void operator--() const { ( void )--this->Iterator; }
    _Iter operator--( int ) const { _Iter Temp = this->Iterator; ( void )--this->Iterator; return Temp; }
};

template < typename _Iter >
struct DefaultIteratorAdvance< _Iter, std::random_access_iterator_tag > : DefaultIteratorAdvance< _Iter, std::bidirectional_iterator_tag >
{
    _Iter operator+( ptrdiff_t a_Offset ) const { return this->Iterator + a_Offset; }
    void operator+=( ptrdiff_t a_Offset ) const { ( void )( this->Iterator += a_Offset ); }
    _Iter operator-( ptrdiff_t a_Offset ) const { return this->Iterator - a_Offset; }
    void operator-=( ptrdiff_t a_Offset ) const { ( void )( this->Iterator -= a_Offset ); }
    ptrdiff_t operator-( const _Iter& a_Iterator ) const { return this->Iterator - a_Iterator; }
};

template < typename _Iter >
struct DefaultIteratorDeref
{
    _Iter& Iterator;

    decltype( auto ) operator->() const { return Iterator.operator->(); }
    decltype( auto ) operator*() const { return Iterator.operator*(); }
};

template < typename _Iter >
struct DefaultIteratorDeref< _Iter* >
{
    _Iter* Iterator;

    _Iter* operator->() const { return Iterator; }
    _Iter& operator*() const { return *Iterator; }
};

template < typename _Iter, typename _IterCategory = typename std::iterator_traits< _Iter >::iterator_category >
struct DefaultIteratorComp {};

template < typename _Iter >
struct DefaultIteratorComp< _Iter, std::forward_iterator_tag >
{
    _Iter& Iterator;

    bool operator==( const _Iter& a_Iterator ) const { return Iterator == a_Iterator; }
    bool operator!=( const _Iter& a_Iterator ) const { return Iterator != a_Iterator; }
};

template < typename _Iter >
struct DefaultIteratorComp< _Iter, std::bidirectional_iterator_tag > : DefaultIteratorComp< _Iter, std::forward_iterator_tag >
{};

template < typename _Iter >
struct DefaultIteratorComp< _Iter, std::random_access_iterator_tag > : DefaultIteratorComp< _Iter, std::bidirectional_iterator_tag >
{
    bool operator<=( const _Iter& a_Iterator ) const { return this->Iterator <= a_Iterator; }
    bool operator>=( const _Iter& a_Iterator ) const { return this->Iterator >= a_Iterator; }
    bool operator<( const _Iter& a_Iterator ) const { return this->Iterator < a_Iterator; }
    bool operator>( const _Iter& a_Iterator ) const { return this->Iterator > a_Iterator; }
};

template < typename _Proxy, typename _Iter = _Proxy, typename _Advance = DefaultIteratorAdvance< _Proxy >, typename _Deref = DefaultIteratorDeref< _Proxy >, typename _Comp = DefaultIteratorComp< _Proxy > >
class IteratorProxyBase
{
public:

    using IteratorType = _Iter;
    using AdvanceType = _Advance;
    using ProxyType = _Proxy;
    using DerefType = _Deref;
    using CompType = _Comp;

    IteratorProxyBase() = default;
    IteratorProxyBase( const IteratorProxyBase& ) = default;
    IteratorProxyBase( IteratorProxyBase&& ) = default;
    IteratorProxyBase( const ProxyType& a_Proxy ) : m_Proxy( a_Proxy ) {}
    IteratorProxyBase& operator=( const IteratorProxyBase& ) = default;
    IteratorProxyBase& operator=( IteratorProxyBase&& ) = default;
    IteratorProxyBase& operator=( const ProxyType& a_Proxy ) { m_Proxy = a_Proxy; return *this; }

    IteratorType& operator++() { ( void )++AdvanceType{ m_Proxy }; return ( IteratorType& )*this; }
    IteratorType operator++( int ) { return IteratorType{ AdvanceType{ m_Proxy }++ }; }
    IteratorType& operator+=( ptrdiff_t a_Offset ) { AdvanceType{ m_Proxy } += a_Offset; return ( IteratorType& )*this; }
    IteratorType& operator--() { ( void )--AdvanceType{ m_Proxy }; return ( IteratorType& )*this; }
    IteratorType operator--( int ) { return IteratorType{ AdvanceType{ m_Proxy }-- }; }
    IteratorType& operator-=( ptrdiff_t a_Offset ) { AdvanceType{ m_Proxy } -= a_Offset; return ( IteratorType& )*this; }
    decltype( auto ) operator*() const { return DerefType{ m_Proxy }.operator*(); }
    decltype( auto ) operator->() const { return DerefType{ m_Proxy }.operator->(); }
    bool operator==( const IteratorType& a_Iterator ) const { return CompType{ m_Proxy }.operator==( a_Iterator ); }
    bool operator!=( const IteratorType& a_Iterator ) const { return CompType{ m_Proxy }.operator!=( a_Iterator ); }
    bool operator<=( const IteratorType& a_Iterator ) const { return CompType{ m_Proxy }.operator<=( a_Iterator ); }
    bool operator>=( const IteratorType& a_Iterator ) const { return CompType{ m_Proxy }.operator>=( a_Iterator ); }
    bool operator<( const IteratorType& a_Iterator ) const { return CompType{ m_Proxy }.operator<( a_Iterator ); }
    bool operator>( const IteratorType& a_Iterator ) const { return CompType{ m_Proxy }.operator>( a_Iterator ); }

private:

    _Proxy m_Proxy;
};