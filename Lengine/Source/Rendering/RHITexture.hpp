#pragma once
#include <memory>
#include <Rendering/RHIResource.hpp>
#include <Resource/Texture.hpp>

using std::shared_ptr;
using std::make_shared;

enum class ERHITextureWrapMode
{
	
};

struct RHITextureParams
{
	TextureHandle Resource;
};

using TextureSlot = uint32_t;

class RHITexture : public RHIResource
{
public:

	RHITexture( const RHITextureParams& a_Params );
	~RHITexture();
	bool Bind( const TextureSlot a_Slot ) const;

private:

	friend struct RHITextureData;

	shared_ptr< RHITextureData > m_Implementation;
};

using RHITextureRef = RHIRef< RHITexture >;