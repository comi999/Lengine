#pragma once
#include <complex>

template < typename _Resource >
class RHIRef
{
	
};

class RHI
{
	template < typename _Resource >
	static RHIRef< _Resource > CreateResource()
	{
		return {};
	}
};