#pragma once
#include <vector>
#include <Resource/Resource.hpp>
#include <Math/Math.h>

using std::vector;

DEFINE_RESOURCE_TYPE( Texture )
{
public:

	DEFINE_RESOURCE_BODY( Texture );

	uvec2 GetSize() const { return m_Size; }
	uint32_t GetWidth() const { return m_Size.x; }
	uint32_t GetHeight() const { return m_Size.y; }
	const u8vec4* GetPixels() const { return m_Data.data(); }
	u8vec4* GetPixels() { return m_Data.data(); }

private:

	vector< u8vec4 > m_Data;
	uvec2				  m_Size;
};