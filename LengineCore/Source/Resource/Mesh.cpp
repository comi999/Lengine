#include <Resource/Mesh.hpp>
#include <Resource/Skeleton.hpp>

SkeletonHandle RMesh::GetSkeleton() const
{
	return GetChildren().GetResourceByAlias< RSkeleton >( "Skeleton" );
}

vector< vec2 >& RMesh::GetUVChannel( const size_t a_Index )
{
	return m_UVChannels[ a_Index ];
}





















































//
//void RMesh::Draw()
//{
//	for ( unsigned int i = 0; i < m_MeshChunks.size(); i++ )
//		m_MeshChunks[ i ].Draw();
//}
//
//void RMesh::ExtractBoneWeightForVertices( std::vector<Vertex>& a_Vertices, const aiMesh* a_Mesh, const aiScene* a_Scene )
//{
//	// Iterate through each bone
//	for ( int boneIndex = 0; boneIndex < a_Mesh->mNumBones; ++boneIndex )
//	{
//		int boneID = -1;
//		std::string boneName = a_Mesh->mBones[ boneIndex ]->mName.C_Str();
//		if ( m_BoneInfoMap.find( boneName ) == m_BoneInfoMap.end() )
//		{
//			BoneInfo newBoneInfo;
//			newBoneInfo.ID = m_BoneCounter;
//			newBoneInfo.Offset = glm::transpose( glm::make_mat4x4( ( (ai_real*)&a_Mesh->mBones[ boneIndex ]->mOffsetMatrix ) ) );
//			m_BoneInfoMap[ boneName ] = newBoneInfo;
//			boneID = m_BoneCounter;
//			m_BoneCounter++;
//		}
//		else
//		{
//			boneID = m_BoneInfoMap[ boneName ].ID;
//		}
//
//		assert( boneID != -1 );
//		auto weights = a_Mesh->mBones[ boneIndex ]->mWeights;
//		int numWeights = a_Mesh->mBones[ boneIndex ]->mNumWeights;
//
//		for ( int weightIndex = 0; weightIndex < numWeights; ++weightIndex )
//		{
//			int vertexId = weights[ weightIndex ].mVertexId;
//			float weight = weights[ weightIndex ].mWeight;
//			assert( vertexId <= a_Vertices.size() );
//			SetVertexBoneData( a_Vertices[ vertexId ], boneID, weight );
//		}
//	}
//}
//
//void RMesh::SetVertexBoneData( Vertex& a_Vertex, int a_BoneID, float a_Weight )
//{
//	for ( int i = 0; i < MAX_BONE_INFLUENCE; ++i )
//	{
//		if ( a_Vertex.m_BoneIDs[ i ] < 0 )
//		{
//			a_Vertex.m_Weights[ i ] = a_Weight;
//			a_Vertex.m_BoneIDs[ i ] = a_BoneID;
//			break;
//		}
//	}
//}
//
//bool RMesh::Load( string Path, bool a_GenerateMaterials, int ProcessSteps )
//{
//	Assimp::Importer Import;
//	const aiScene* Scene = Import.ReadFile( Path, ProcessSteps );
//
//	if ( !Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode )
//	{
//		LOG( Error, Import.GetErrorString() );
//		return false;
//	}
//
//	ConstuctResourceInfo( Path );
//
//	ProcessNode( Scene->mRootNode, Scene );
//	return true;
//}
//
//void RMesh::Load( aiScene* a_Scene )
//{
//	
//}
//
//void RMesh::ProcessNode( aiNode* a_Node, const aiScene* a_Scene )
//{
//	m_MeshChunks.resize( a_Scene->mNumMeshes );
//
//	// process all the node's meshes (if any)
//	for ( unsigned int i = 0; i < a_Node->mNumMeshes; i++ )
//	{
//		aiMesh* mesh = a_Scene->mMeshes[ a_Node->mMeshes[ i ] ];
//		ProcessMeshChunk( m_MeshChunks[ i ], mesh, a_Scene );
//	}
//	// then do the same for each of its children
//	for ( unsigned int i = 0; i < a_Node->mNumChildren; i++ )
//	{
//		ProcessNode( a_Node->mChildren[ i ], a_Scene );
//	}
//}
//
//void RMesh::ProcessMeshChunk( MeshChunk& o_Mesh, const aiMesh* a_Mesh, const aiScene* a_Scene )
//{
//	std::vector<unsigned int>& indices = o_Mesh.Indices;
//
//	o_Mesh.Vertices.resize( a_Mesh->mNumVertices );
//
//	// Process Verticies
//	for ( unsigned int i = 0; i < a_Mesh->mNumVertices; ++i )
//	{
//		ProcessVertex( o_Mesh.Vertices[i], a_Mesh, i);
//	}
//
//	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
//	for ( unsigned int i = 0; i < a_Mesh->mNumFaces; i++ )
//	{
//		aiFace face = a_Mesh->mFaces[ i ];
//		// retrieve all indices of the face and store them in the indices vector
//		for ( unsigned int j = 0; j < face.mNumIndices; j++ )
//			indices.push_back( face.mIndices[ j ] );
//	}
//
//	if ( a_Mesh->mMaterialIndex >= 0 )
//	{
//		TODO( "Process Materials when loading meshes" );
//	}
//
//	ExtractBoneWeightForVertices( o_Mesh.Vertices, a_Mesh, a_Scene );
//
//	o_Mesh.Initialise();
//}
//
//void RMesh::ProcessVertex( Vertex& o_Vertex, const aiMesh* a_Mesh, const int a_Index )
//{
//	o_Vertex.Position = vec4( a_Mesh->mVertices[ a_Index ].x, a_Mesh->mVertices[ a_Index ].y, a_Mesh->mVertices[ a_Index ].z, 1 );
//
//	if ( a_Mesh->HasNormals() )
//	{
//		o_Vertex.Normal = vec4( a_Mesh->mNormals[ a_Index ].x, a_Mesh->mNormals[ a_Index ].y, a_Mesh->mNormals[ a_Index ].z, 1 );
//	}
//
//	if ( a_Mesh->HasTangentsAndBitangents() )
//	{
//		o_Vertex.Tangent = vec3( a_Mesh->mTangents[ a_Index ].x, a_Mesh->mTangents[ a_Index ].y, a_Mesh->mTangents[ a_Index ].z );
//		o_Vertex.Bitangent = vec3( a_Mesh->mBitangents[ a_Index ].x, a_Mesh->mBitangents[ a_Index ].y, a_Mesh->mBitangents[ a_Index ].z );
//	}
//
//	if ( a_Mesh->mTextureCoords[ 0 ] )
//	{
//		TODO( "Currently only supports one UV channel" );
//		o_Vertex.TexCoord = vec2( a_Mesh->mTextureCoords[ 0 ][ a_Index ].x, a_Mesh->mTextureCoords[ 0 ][ a_Index ].y );
//	}
//}

//void RMesh::Initialise(unsigned int a_VertexCount, const Vertex* a_Vertices, unsigned int a_IndexCount, unsigned int* a_Indices)
//{
//	assert(m_MeshChunk.VAO == 0);
//
//	// generate buffers
//	glGenBuffers(1, &m_MeshChunk.VBO);
//	glGenVertexArrays(1, &m_MeshChunk.VAO);
//
//	// bind vertex array aka a mesh wrapper
//	glBindVertexArray(m_MeshChunk.VAO);
//
//	// bind vertex buffer
//	glBindBuffer(GL_ARRAY_BUFFER, m_MeshChunk.VBO);
//
//	// fill vertex buffer
//	glBufferData(GL_ARRAY_BUFFER, a_VertexCount * sizeof(Vertex), a_Vertices, GL_STATIC_DRAW);
//
//	// enable first element as position
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//
//	// bind indices if there are any
//	if (a_IndexCount != 0) {
//		glGenBuffers(1, &m_MeshChunk.IBO);
//
//		// bind vertex buffer
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshChunk.IBO);
//
//		// fill vertex buffer
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_IndexCount * sizeof(unsigned int), a_Indices, GL_STATIC_DRAW);
//
//		m_MeshChunk.m_TriCount = a_IndexCount / 3;
//	}
//	else {
//		m_MeshChunk.m_TriCount = a_VertexCount / 3;
//	}
//
//	// unbind buffers
//	glBindVertexArray(0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//}
//
//void RMesh::Initialise(const char* a_FileName)
//{
//	std::string StringFilePath = std::string(RESOURCE_PATH) + MESH_FILE_PATH + a_FileName;
//	const char* FilePath = StringFilePath.c_str();
//	// Read vertices from the model
//	const aiScene* Scene = aiImportFile(FilePath, 0);
//
//	aiMesh* Mesh = Scene->mMeshes[0];
//
//	// Excract indicies from the Mesh
//	int NumFaces = Mesh->mNumFaces;
//	std::vector<unsigned int> Indices;
//	for (int i = 0; i < NumFaces; i++)
//	{
//		Indices.push_back(Mesh->mFaces[i].mIndices[0]);
//		Indices.push_back(Mesh->mFaces[i].mIndices[2]);
//		Indices.push_back(Mesh->mFaces[i].mIndices[1]);
//		// generate a second triangle for quads
//		if (Mesh->mFaces[i].mNumIndices == 4)
//		{
//			Indices.push_back(Mesh->mFaces[i].mIndices[0]);
//			Indices.push_back(Mesh->mFaces[i].mIndices[3]);
//			Indices.push_back(Mesh->mFaces[i].mIndices[2]);
//		}
//	}
//
//	// Extract vertex data
//	int NumV = Mesh->mNumVertices;
//	Vertex* Vertices = new Vertex[NumV];
//	for (int i = 0; i < NumV; i++)
//	{
//		Vertices[i].Position = glm::vec4(Mesh->mVertices[i].x,
//			Mesh->mVertices[i].y, Mesh->mVertices[i].z, 1);
//		// TODO, normals and UVs
//	}
//	Initialise(NumV, Vertices, Indices.size(), Indices.data());
//	delete[] Vertices;
//
//}
//
//void RMesh::Initialise(EPrimativeShape a_PrimativeMesh)
//{
//	std::string PrimativeFileName = "Primitives/";
//	// Get num of verticies
//	switch (a_PrimativeMesh)
//	{
//	case RMesh::EPS_Quad:
//		InitialiseQuad();
//		break;
//	case RMesh::EPS_Box:
//		PrimativeFileName.append("Box");
//		break;
//	case RMesh::EPS_Cylinder:
//		PrimativeFileName.append("Cylinder");
//		break;
//	case RMesh::EPS_Pyramid:
//		PrimativeFileName.append("Pyramid");
//		break;
//	case RMesh::EPS_Sphere:
//		PrimativeFileName.append("Sphere");
//		break;
//	case RMesh::EPS_Cone:
//		PrimativeFileName.append("Cone");
//		break;
//	default:
//		break;
//	}
//
//	PrimativeFileName.append(".obj");
//	Initialise(PrimativeFileName.c_str());
//}
//
//void RMesh::InitialiseQuad()
//{
//	// Check that it has not already been initialised
//	assert(m_MeshChunk.VAO == 0);
//
//	// Generate the buffer
//	glGenBuffers(1, &m_MeshChunk.VBO);
//	glGenVertexArrays(1, &m_MeshChunk.VAO);
//
//	// Bind the vertex array, this will be our mesh buffer
//	glBindVertexArray(m_MeshChunk.VAO);
//
//	// Bind the vertex buffer
//	glBindBuffer(GL_ARRAY_BUFFER, m_MeshChunk.VBO);
//
//	// Define the 6 verticies for 2 triangles
//	Vertex Vertices[6];
//	Vertices[0].Position = { -0.5f, 0.f,  0.5f, 1 };
//	Vertices[1].Position = {  0.5f, 0.f,  0.5f, 1 };
//	Vertices[2].Position = { -0.5f, 0.f, -0.5f, 1 };
//
//	Vertices[3].Position = { -0.5f, 0.f, -0.5f, 1 };
//	Vertices[4].Position = {  0.5f, 0.f,  0.5f, 1 };
//	Vertices[5].Position = {  0.5f, 0.f, -0.5f, 1 };
//
//	Vertices[0].TexCoord = { 0,1 }; // Bottom Left
//	Vertices[1].TexCoord = { 1,1 }; // Bottom Right
//	Vertices[2].TexCoord = { 0,0 }; // Top Left
//	Vertices[3].TexCoord = { 0,0 }; // Top Right
//	Vertices[4].TexCoord = { 1,1 };
//	Vertices[5].TexCoord = { 1,0 };
//
//	// Fill Vertex Buffer
//	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), Vertices, GL_STATIC_DRAW);
//
//	// Set the first element to be a position
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//
//	// Unbind buffers
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	// Set the Tri Count
//	m_MeshChunk.m_TriCount = 2;
//}
//
//void RMesh::InitialiseFullScreenQuad()
//{
//	assert(m_MeshChunk.VAO == 0);
//
//	// generate buffers
//	glGenBuffers(1, &m_MeshChunk.VBO);
//	glGenVertexArrays(1, &m_MeshChunk.VAO);
//
//	// bind vertex array aka a mesh wrapper
//	glBindVertexArray(m_MeshChunk.VAO);
//	// bind vertex buffer
//	glBindBuffer(GL_ARRAY_BUFFER, m_MeshChunk.VBO);
//
//	// define vertices
//	float vertices[] =
//	{
//	-1,1, // left top
//	-1,-1, // left bottom
//	1,1, // right top
//	-1,-1, // left bottom
//	1,-1, // right bottom
//	1, 1 // right top
//	};
//
//	// fill vertex buffer
//	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices,
//		GL_STATIC_DRAW);
//	// enable first element as position
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);
//	// unbind buffers
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	// quad has 2 triangles
//	m_MeshChunk.m_TriCount = 2;
//}
//
//bool RMesh::Load(const char* a_FileName, bool a_LoadTextures, bool a_FlipTextures)
//{
//	//std::string StringFilePath = std::string(RESOURCE_PATH) + MESH_FILE_PATH + a_FileName;
//	//a_FileName = StringFilePath.c_str();
//	//if (m_meshChunks.empty() == false) {
//	//	printf("Mesh already initialised, can't re-initialise!\n");
//	//	return false;
//	//}
//	//return false;
//
//	std::string StringFilePath = std::string(RESOURCE_PATH) + MESH_FILE_PATH + a_FileName;
//	const char* FilePath = StringFilePath.c_str();
//	// Read vertices from the model
//	const aiScene* Scene = aiImportFile(FilePath, 0);
//
//	aiMesh* Mesh = Scene->mMeshes[0];
//
//	// Excract indicies from the Mesh
//	int NumFaces = Mesh->mNumFaces;
//	std::vector<unsigned int> Indices;
//	for (int i = 0; i < NumFaces; i++)
//	{
//		Indices.push_back(Mesh->mFaces[i].mIndices[0]);
//		Indices.push_back(Mesh->mFaces[i].mIndices[2]);
//		Indices.push_back(Mesh->mFaces[i].mIndices[1]);
//		// generate a second triangle for quads
//		if (Mesh->mFaces[i].mNumIndices == 4)
//		{
//			Indices.push_back(Mesh->mFaces[i].mIndices[0]);
//			Indices.push_back(Mesh->mFaces[i].mIndices[3]);
//			Indices.push_back(Mesh->mFaces[i].mIndices[2]);
//		}
//	}
//
//	// Extract vertex data
//	int NumV = Mesh->mNumVertices;
//	Vertex* Vertices = new Vertex[NumV];
//	for (int i = 0; i < NumV; i++)
//	{
//		Vertices[i].Position = glm::vec4(Mesh->mVertices[i].x,
//			Mesh->mVertices[i].y, Mesh->mVertices[i].z, 1);
//		// TODO, normals and UVs
//	}
//	Initialise(NumV, Vertices, Indices.size(), Indices.data());
//	delete[] Vertices;
//
//	return true;
//}

//void RMesh::Draw()
//{
//	glBindVertexArray(m_MeshChunk.VAO);
//	if (m_MeshChunk.IBO != 0)
//	{
//		glDrawElements(GL_TRIANGLES, 3 * m_MeshChunk.m_TriCount, GL_UNSIGNED_INT, 0);
//	}
//	{
//		glDrawArrays(GL_TRIANGLES, 0, 3 * m_MeshChunk.m_TriCount);
//	}
//}

//MeshChunk::MeshChunk( std::vector<Vertex> a_Vertices, std::vector<unsigned int> a_Indices )
//{
//	Vertices = a_Vertices;
//	Indices = a_Indices;
//
//	Initialise();
//}
//
//MeshChunk::~MeshChunk()
//{
//	glDeleteVertexArrays( 1, &VAO );
//	glDeleteBuffers( 1, &VBO );
//	glDeleteBuffers( 1, &IBO );
//}
//
//void MeshChunk::Draw()
//{
//	glBindVertexArray( VAO );
//	glDrawElements( GL_TRIANGLES, static_cast<unsigned int>( Indices.size() ), GL_UNSIGNED_INT, 0 );
//	glBindVertexArray( 0 );
//}
//
//void MeshChunk::Initialise()
//{
//	glGenVertexArrays( 1, &VAO );
//	glGenBuffers( 1, &VBO );
//	glGenBuffers( 1, &IBO );
//
//	glBindVertexArray( VAO );
//	glBindBuffer( GL_ARRAY_BUFFER, VBO );
//
//	glBufferData( GL_ARRAY_BUFFER, Vertices.size() * sizeof( Vertex ), &Vertices[ 0 ], GL_STATIC_DRAW );
//
//	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IBO );
//	glBufferData( GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof( unsigned int ), &Indices[ 0 ], GL_STATIC_DRAW );
//
//	// vertex positions
//	glEnableVertexAttribArray( 0 );
//	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)0 );
//	// vertex normals
//	glEnableVertexAttribArray( 1 );
//	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Vertex::Normal ) );
//	// vertex texture coords
//	glEnableVertexAttribArray( 2 );
//	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Vertex::TexCoord ) );
//	// vertex tangent
//	glEnableVertexAttribArray( 3 );
//	glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Vertex::Tangent ) );
//	// vertex bitangent
//	glEnableVertexAttribArray( 4 );
//	glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Vertex::Bitangent ) );
//	// ids
//	glEnableVertexAttribArray( 5 );
//	glVertexAttribIPointer( 5, 4, GL_INT,			 sizeof( Vertex ), (void*)offsetof( Vertex, Vertex::m_BoneIDs ) );
//	// weights
//	glEnableVertexAttribArray( 6 );
//	glVertexAttribPointer( 6, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Vertex::m_Weights ) );
//
//	glBindVertexArray( 0 );
//}
