#if USING_RENDERING_OPENGL
#include <Rendering/RHITexture.hpp>

// We are linking opengl statically.
#define GLEW_STATIC

// We will include opengl32.lib using the pragma below from
// opengl provided by Windows SDK.
#pragma comment(lib, "opengl32")
#include <glew/GL/glew.h>

struct RHITextureData
{
	GLuint Handle;
};

RHITexture::RHITexture( const RHITextureParams& a_Params )
	: m_Implementation( make_shared< RHITextureData >() )
{
	// Create a single texture.
	glGenTextures( 1, &m_Implementation->Handle );
	glBindTexture( GL_TEXTURE_2D, m_Implementation->Handle );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexImage2D(
		GL_TEXTURE_2D, 
		0, GL_RGBA, 
		a_Params.Resource->GetWidth(), 
		a_Params.Resource->GetHeight(), 
		0, 
		GL_RGBA, 
		GL_UNSIGNED_BYTE, a_Params.Resource->GetPixels() );

	glGenerateMipmap( GL_TEXTURE_2D );

	glBindTexture( GL_TEXTURE_2D, 0u );

}

RHITexture::~RHITexture()
{
	glDeleteTextures( 1u, &m_Implementation->Handle );
}

bool RHITexture::Bind( const TextureSlot a_Slot ) const
{
	glActiveTexture( GL_TEXTURE0 + a_Slot );
	glBindTexture( GL_TEXTURE_2D, m_Implementation->Handle );
	return true;
}
#endif