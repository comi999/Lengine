#pragma once
#include <Resource/Resource.hpp>
#include <Resource/Skeleton.hpp>
#include <Math/Math.h>

// --- OpenGL ---
//#include "gl_core_4_4.h"

// --- STD ---
#include <map>
#include <vector>

using std::map;
using std::vector;

// --- Engine ---
//#include "RMaterialInstance.h"


//constexpr auto MAX_BONE_INFLUENCE = 4;

//struct BoneInfo
//{
//	/* The the index in FinalBoneMatrices */
//	unsigned int ID;
//
//	/* The offset matrix transforms vertex from model space to bone space */
//	mat4 Offset;
//};

//struct Vertex
//{
//	Vertex()
//	{
//		for ( int i = 0; i < MAX_BONE_INFLUENCE; i++ )
//		{
//			m_BoneIDs[ i ] = -1;
//			m_Weights[ i ] = 0.0f;
//		}
//	}
//
//	vec4 Position;	// Added to attribute location 0
//	vec4 Normal;	// Added to attribute location 1
//	vec2 TexCoord;	// Added to attribute location 2
//
//	// Normal-Mapping Data
//	vec3 Tangent;	// Added to attribute location 3
//	vec3 Bitangent;	// Added to attribute location 4
//
//	//bone indexes which will influence this vertex
//	int m_BoneIDs[ MAX_BONE_INFLUENCE ];
//	//weights from each bone
//	float m_Weights[ MAX_BONE_INFLUENCE ];
//};

//class MeshChunk
//{
//public:
//	MeshChunk() = default;
//	MeshChunk( std::vector<Vertex> a_Vertices, std::vector<unsigned int> a_Indices );
//	~MeshChunk();
//
//	void Draw();
//	void Initialise();
//
//	std::vector<Vertex>       Vertices;
//	std::vector<unsigned int> Indices;
//
//	size_t MaterialID;
//
//private:
//	unsigned int VAO = 0; // The Vertex Array Object
//	unsigned int VBO = 0; // The Vertex Buffer Object
//	unsigned int IBO = 0; // The Index Buffer Object
//};

// Describes a single vertex in a mesh.
struct MeshVertex
{
	vec3 Position;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
};

// Describes the three indices into the vertex array that ties together a triangle.
using MeshTriangle = uvec3;

// Describes the bone weights for a vertex.
struct MeshBoneWeightPair
{
	static constexpr uint32_t Unset = -1;

	uint32_t Index = Unset;
	float Weight = 0.0f;
};

struct MeshBoneWeight
{
	static constexpr size_t MaxBoneWeights = 4u;

	MeshBoneWeightPair Pairs[ MaxBoneWeights ];
};

DEFINE_RESOURCE_TYPE( Mesh )
{
public:

	DEFINE_RESOURCE_BODY( Mesh );

	// Does this Mesh have a Skeleton associated with it?
	bool IsSkeletal() const { return m_BoneWeights.size() > 0u; }

	// Get the associated skeleton that makes this mesh a skeletal mesh.
	SkeletonHandle GetSkeleton() const;

	const vector< MeshVertex >& GetVertices() const { return const_cast< Mesh* >( this )->GetVertices(); }
	vector< MeshVertex >& GetVertices() { return m_Vertices; }
	const vector< MeshTriangle >& GetTriangles() const { return const_cast< Mesh* >( this )->GetTriangles(); }
	vector< MeshTriangle >& GetTriangles() { return m_Triangles; }
	const vector< vec2 >& GetUVChannel( const size_t a_Index ) const { return const_cast< Mesh* >( this )->GetUVChannel( a_Index ); }
	vector< vec2 >& GetUVChannel( const size_t a_Index );
	size_t GetUVChannelCount() const { return m_UVChannelCount; }
	const vector< MeshBoneWeight >& GetBoneWeights() const { return const_cast< Mesh* >( this )->GetBoneWeights(); }
	vector< MeshBoneWeight >& GetBoneWeights() { return m_BoneWeights; }
	size_t GetUniqueBoneCount() const { return m_BoneNames.size(); }
	const string& GetBoneName( const size_t a_Index ) const { return m_BoneNames[ a_Index ]; }

private:

	vector< MeshVertex > m_Vertices;
	vector< MeshTriangle > m_Triangles;
	size_t m_UVChannelCount;
	vector< vec2 > m_UVChannels[ 8u ];
	string m_UVChannelNames[ 8u ];
	vector< MeshBoneWeight > m_BoneWeights;
	vector< string > m_BoneNames;

//	std::vector<MeshChunk> m_MeshChunks;
//	std::vector<MaterialInstanceHandle> m_Materials;
//
//	bool Load( string Path, bool a_GenerateMaterials, int ProcessSteps = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
//	void Load( aiScene* a_Scene );
//	// Assimp
//	void ProcessNode( aiNode* a_Node, const aiScene* a_Scene );
//	virtual void ProcessMeshChunk( MeshChunk& o_Mesh, const aiMesh* a_Mesh, const aiScene* a_Scene );
//	virtual void ProcessVertex( Vertex& o_Vertex, const aiMesh* a_Mesh, const int a_Index );
//
//public:
//	void Draw();
//
//	// --- SK MESH ---
//public:
//	auto& GetBoneInfoMap() { return m_BoneInfoMap; }
//	int&  GetBoneCount()   { return m_BoneCounter; }
//	auto& GetMeshChunks()  { return m_MeshChunks; }
//
//private:
//	std::map<string, BoneInfo> m_BoneInfoMap;
//	int m_BoneCounter = 0;
//
//
//	void ExtractBoneWeightForVertices( std::vector<Vertex>& a_Vertices, const aiMesh* a_Mesh, const aiScene* a_Scene );
//	void SetVertexBoneData( Vertex& a_Vertex, int a_BoneID, float a_Weight );
//
//#pragma region Legacy
//
//	// Legacy
//public:
//
//	enum EPrimativeShape
//	{
//		EPS_Quad,
//		EPS_Box,
//		EPS_Cylinder,
//		EPS_Pyramid,
//		EPS_Sphere,
//		EPS_Cone,
//	};
//
//	void Initialise( unsigned int a_VertexCount, const Vertex* a_Vertices, unsigned int a_IndexCount = 0, unsigned int* a_Indices = nullptr ) {}
//
//	void InitialiseQuad() {}
//
//	void InitialiseFullScreenQuad() {}

	// Will fail if it cannot find the file OR a mesh has already been loaded in this instance
	//bool Load( const char* a_FileName, bool a_LoadTextures = true, bool a_FlipTextures = false );

	//MaterialInstanceHandle GetMaterial() { return m_Material; }
	//void SetMaterial( MaterialInstanceHandle a_Material ) { m_Material = a_Material; }

	// Material access
	//size_t GetMaterialCount() const { return m_Materials.size(); }
	//RMaterial& GetMaterial(size_t index = 0) { return m_Materials[index]; }
#pragma endregion
};

//using MeshHandle = std::shared_ptr< RMesh >;

//class RSkeletalMesh
//	: public RMesh
//{
//	virtual void ProcessMeshChunk( MeshChunk& o_Mesh, const aiMesh* a_Mesh, const aiScene* a_Scene ) override {}
//};
//
//using SkeletalMeshHandle = std::shared_ptr< RSkeletalMesh >;