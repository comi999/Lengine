#pragma once
#include <tuple>
#include <type_traits>

namespace std
{
	template < typename T >
	struct is_static_function : false_type {};

	template < typename Return, typename... Args >
	struct is_static_function< Return( * )( Args... ) > : true_type {};

	template < typename Return, typename... Args >
	struct is_static_function< Return( & )( Args... ) > : true_type {};

	template < typename Return, typename... Args >
	struct is_static_function< Return( Args... ) > : true_type {};

	template < typename T >
	static constexpr bool is_static_function_v = is_static_function< T >::value;

	template < typename T >
	struct is_member_function : false_type {};

	template < typename T, typename Return, typename... Args >
	struct is_member_function< Return( T::* )( Args... ) > : true_type {};

	template < typename T, typename Return, typename... Args >
	struct is_member_function< Return( T::* )( Args... ) const > : true_type {};

	template < typename T, typename Return, typename... Args >
	struct is_member_function< Return( T::* )( Args... ) volatile > : true_type {};

	template < typename T, typename Return, typename... Args >
	struct is_member_function< Return( T::* )( Args... ) const volatile > : true_type {};

	template < typename T >
	static constexpr bool is_member_function_v = is_member_function< T >::value;
	
	template < typename T >
	struct is_const_member_function : false_type {};

	template < typename T, typename Return, typename... Args >
	struct is_const_member_function< Return( T::* )( Args... ) const > : true_type {};

	template < typename T, typename Return, typename... Args >
	struct is_const_member_function< Return( T::* )( Args... ) const volatile > : true_type {};

	template < typename T >
	static constexpr bool is_const_member_function_v = is_const_member_function< T >::value;

	template < typename T >
	struct is_volatile_member_function : false_type {};

	template < typename T, typename Return, typename... Args >
	struct is_volatile_member_function< Return( T::* )( Args... ) volatile > : true_type {};

	template < typename T, typename Return, typename... Args >
	struct is_volatile_member_function< Return( T::* )( Args... ) const volatile > : true_type {};

	template < typename T >
	static constexpr bool is_volatile_member_function_v = is_volatile_member_function< T >::value;

	template < typename T, typename = void >
	struct is_callable : bool_constant< is_static_function_v< T > || is_member_function_v< T > > {};

	template < typename T >
	struct is_callable< T, void_t< decltype( &T::operator() ) > > : true_type {};

	template < typename T >
	static constexpr bool is_callable_v = is_callable< T >::value;

	template < typename T >
	struct is_lambda : public bool_constant< !is_static_function_v< T > && !is_member_function_v< T >&& is_callable_v< T > > {};

	template < typename T >
	static constexpr bool is_lambda_v = is_lambda< T >::value;

	template < typename T, bool _IsLambda = is_lambda_v< T > >
	struct function_call
	{ 
		static constexpr nullptr_t value = nullptr;
		using type = void;
	};

	template < typename T >
	struct function_call< T, true > 
	{
		static constexpr auto value = &T::operator();
		using type = decltype( value );
	};

	template < typename T >
	static constexpr auto function_call_v = function_call< T >::value;

	template < typename T >
	using function_call_t = typename function_call< T >::type;

	template < typename T >
	struct function_traits
	{
		using return_type = typename function_traits< typename function_call< T >::type >::return_type;
		using object_type = typename function_traits< typename function_call< T >::type >::object_type;
		using arguments_type = typename function_traits< typename function_call< T >::type >::arguments_type;
		using signature_type = typename function_traits< typename function_call< T >::type >::signature_type;
	};

	template <>
	struct function_traits< void >
	{
		using return_type = void;
		using object_type = void;
		using arguments_type = void;
		using signature_type = void;
	};

	template < typename Return, typename... Args >
	struct function_traits< Return( Args... ) >
	{
		using return_type = Return;
		using object_type = void;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Return, typename... Args >
	struct function_traits< Return( * )( Args... ) >
	{
		using return_type = Return;
		using object_type = void;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Return, typename... Args >
	struct function_traits< Return( & )( Args... ) >
	{
		using return_type = Return;
		using object_type = void;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* )( Args... ) >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* )( Args... ) const >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* )( Args... ) volatile >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* )( Args... ) const volatile >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* const )( Args... ) >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* const )( Args... ) const >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* const )( Args... ) volatile >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* const )( Args... ) const volatile >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* volatile )( Args... ) >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* volatile )( Args... ) const >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* volatile )( Args... ) volatile >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* volatile )( Args... ) const volatile >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* const volatile )( Args... ) >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* const volatile )( Args... ) const >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* const volatile )( Args... ) volatile >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename Object, typename Return, typename... Args >
	struct function_traits< Return( Object::* const volatile )( Args... ) const volatile >
	{
		using return_type = Return;
		using object_type = Object;
		using arguments_type = tuple< Args... >;
		using signature_type = Return( Args... );
	};

	template < typename T >
	struct function_return { using type = typename function_traits< T >::return_type; };

	template < typename T >
	using function_return_t = typename function_return< T >::type;

	template < typename T >
	struct function_object { using type = typename function_traits< T >::object_type; };

	template < typename T >
	using function_object_t = typename function_object< T >::type;

	template < typename T >
	struct function_arguments { using type = typename function_traits< T >::arguments_type; };

	template < typename T >
	using function_arguments_t = typename function_arguments< T >::type;

	template < size_t _Index, typename T >
	struct function_argument { using type = tuple_element_t< _Index, typename function_traits< T >::arguments_type >; };

	template < size_t _Index, typename T >
	using function_argument_t = typename function_argument< _Index, T >::type;

	template < typename T >
	struct function_signature { using type = typename function_traits< T >::signature_type; };

	template < typename T >
	using function_signature_t = typename function_signature< T >::type;

	template < typename T >
	struct function_arity : integral_constant< size_t, function_arguments_t< T >::arity > {};

	template < typename T >
	static constexpr size_t function_arity_v = function_arity< T >::value;

	template < typename T >
	struct is_capture_lambda : bool_constant< is_lambda_v< T > && !is_convertible_v< T, add_pointer_t< function_signature_t< T > > > > {};

	template < typename T >
	static constexpr bool is_capture_lambda_v = is_capture_lambda< T >::value;

	template < typename T, typename Object >
	struct is_member_function_compatible : public bool_constant<
		is_member_function_v< T >&&
		is_base_of_v< function_object_t< T >, Object > &&
		( is_const_member_function_v< T > || !is_const_v< Object > ) &&
		( is_volatile_member_function_v< T > || !is_volatile_v< Object > )
	> {};

	template < typename T, typename Object >
	static constexpr bool is_member_function_compatible_v = is_member_function_compatible< T, Object >::value;

	template < auto _Function, typename T = decltype( _Function ) >
	struct as_static_function {};

	template < auto _Function, typename Return, typename... Args >
	struct as_static_function< _Function, Return( * )( Args... ) >
	{
		using type = Return( * )( void*, Args... );
		static constexpr type value = []( void*, Args... a_Args ) -> Return { return ( *_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Return, typename... Args >
	struct as_static_function< _Function, Return( & )( Args... ) >
	{
		using type = Return( * )( void*, Args... );
		static constexpr type value = []( void*, Args... a_Args ) -> Return { return ( *_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* )( Args... ) >
	{
		using type = Return( * )( void*, Args... );
		static constexpr type value = []( void* a_Object, Args... a_Args ) -> Return { return ( ( Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* )( Args... ) const >
	{
		using type = Return( * )( const void*, Args... );
		static constexpr type value = []( const void* a_Object, Args... a_Args ) -> Return { return ( ( const Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* )( Args... ) volatile >
	{
		using type = Return( * )( volatile void*, Args... );
		static constexpr type value = []( volatile void* a_Object, Args... a_Args ) -> Return { return ( ( volatile Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* )( Args... ) const volatile >
	{
		using type = Return( * )( const volatile void*, Args... );
		static constexpr type value = []( const volatile void* a_Object, Args... a_Args ) -> Return { return ( ( const volatile Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* const )( Args... ) >
	{
		using type = Return( * )( void*, Args... );
		static constexpr type value = []( void* a_Object, Args... a_Args ) -> Return { return ( ( Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* const )( Args... ) const >
	{
		using type = Return( * )( const void*, Args... );
		static constexpr type value = []( const void* a_Object, Args... a_Args ) -> Return { return ( ( const Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* const )( Args... ) volatile >
	{
		using type = Return( * )( volatile void*, Args... );
		static constexpr type value = []( volatile void* a_Object, Args... a_Args ) -> Return { return ( ( volatile Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* const )( Args... ) const volatile >
	{
		using type = Return( * )( const volatile void*, Args... );
		static constexpr type value = []( const volatile void* a_Object, Args... a_Args ) -> Return { return ( ( const volatile Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* volatile )( Args... ) >
	{
		using type = Return( * )( void*, Args... );
		static constexpr type value = []( void* a_Object, Args... a_Args ) -> Return { return ( ( Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* volatile )( Args... ) const >
	{
		using type = Return( * )( const void*, Args... );
		static constexpr type value = []( const void* a_Object, Args... a_Args ) -> Return { return ( ( const Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* volatile )( Args... ) volatile >
	{
		using type = Return( * )( volatile void*, Args... );
		static constexpr type value = []( volatile void* a_Object, Args... a_Args ) -> Return { return ( ( volatile Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* volatile )( Args... ) const volatile >
	{
		using type = Return( * )( const volatile void*, Args... );
		static constexpr type value = []( const volatile void* a_Object, Args... a_Args ) -> Return { return ( ( const volatile Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* const volatile )( Args... ) >
	{
		using type = Return( * )( volatile void*, Args... );
		static constexpr type value = []( volatile void* a_Object, Args... a_Args ) -> Return { return ( ( volatile Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* const volatile )( Args... ) const >
	{
		using type = Return( * )( const void*, Args... );
		static constexpr type value = []( const void* a_Object, Args... a_Args ) -> Return { return ( ( const Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* const volatile )( Args... ) volatile >
	{
		using type = Return( * )( volatile void*, Args... );
		static constexpr type value = []( volatile void* a_Object, Args... a_Args ) -> Return { return ( ( volatile Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};

	template < auto _Function, typename Object, typename Return, typename... Args >
	struct as_static_function< _Function, Return( Object::* const volatile )( Args... ) const volatile >
	{
		using type = Return( * )( const volatile void*, Args... );
		static constexpr type value = []( const volatile void* a_Object, Args... a_Args ) -> Return { return ( ( const volatile Object* )a_Object->*_Function )( std::forward< Args >( a_Args )... ); };
	};
	
	template < auto _Function >
	using as_static_function_t = typename as_static_function< _Function >::type;

	template < auto _Function >
	static constexpr as_static_function_t< _Function > as_static_function_v = as_static_function< _Function >::value;
}