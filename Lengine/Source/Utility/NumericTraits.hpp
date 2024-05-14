#pragma once
#include <limits>
#include <cstdint>
#include <type_traits>

namespace std
{
	template < typename T >
	struct numeric_traits
	{
		static constexpr T min = std::numeric_limits< T >::min();
		static constexpr T max = std::numeric_limits< T >::max();
		static constexpr T epsilon = std::numeric_limits< T >::epsilon();
	};

	template < typename T >
	static constexpr T numeric_min_v = numeric_traits< T >::min;

	template < typename T >
	static constexpr T numeric_max_v = numeric_traits< T >::max;

	template < typename T >
	static constexpr T numeric_epsilon_v = numeric_traits< T >::epsilon;

	template < uint8_t _Bits, bool _Signed = false, bool _Integral = true >
	struct numeric_type
	{
	private:

		static constexpr auto Get()
		{
			if constexpr ( !_Integral && _Bits <= 32 )
			{
				return float{};
			}
			else if constexpr ( !_Integral )
			{
				return double{};
			}
			else if constexpr ( _Bits <= 8u )
			{
				return uint8_t{};
			}
			else if constexpr ( _Bits <= 16u )
			{
				return uint16_t{};
			}
			else if constexpr ( _Bits <= 32u )
			{
				return uint32_t{};
			}
			else
			{
				return uint64_t{};
			}
		}

	public:

		using type = std::conditional_t< _Signed&& _Integral, std::make_signed_t< decltype( Get() ) >, decltype( Get() ) >;
	};

	template < uint8_t _Bits, bool _Signed = false, bool _Integral = true >
	using numeric_type_t = typename numeric_type< _Bits, _Signed, _Integral >::type;
}

template< uint8_t _Bits, bool _Signed = false, bool _Integral = true >
using NumericTypeT = std::numeric_type_t< _Bits, _Signed, _Integral >;

template< typename T >
using NumericTraits = std::numeric_traits< T >;