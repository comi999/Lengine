#if USING_RENDERING_VULKAN
struct RHITextureData
{
	GLuint Handle;
};

RHITexture::RHITexture( const RHITextureParams& a_Params )
{
	
}

void RHITexture::Bind()
{
//	unsigned int texture;
//glGenTextures(1, &texture);
//glBindTexture(GL_TEXTURE_2D, texture);
//// set the texture wrapping/filtering options (on the currently bound texture object)
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//// load and generate the texture
//int width, height, nrChannels;
////unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
////if (data)
//{
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, /*data*/nullptr);
//    glGenerateMipmap(GL_TEXTURE_2D);
////}
////else
//{
//    Platform::Print( "Failed to load texture\n" );
//}
//stbi_image_free(data);
}
#endif