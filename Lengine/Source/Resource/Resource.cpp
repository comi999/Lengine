#include <filesystem>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <Resource/Resource.hpp>
#include <Resource/Mesh.hpp>
#include <Resource/Skeleton.hpp>
#include <Resource/Animation.hpp>
#include <Utility/Invoker.hpp>

namespace fs = std::filesystem;

ResourcePack Resource::s_LoadedResources;

enum class ESupportedResourceFileTypes
{
	Unsupported,
	Fbx,
};

ESupportedResourceFileTypes DetectFileType( const std::string& a_Path )
{
	const std::string FileExtension = fs::path( a_Path ).extension().string();

	if ( FileExtension == "" ) return ESupportedResourceFileTypes::Unsupported;
	if ( FileExtension == ".fbx" ) return ESupportedResourceFileTypes::Fbx;

	return ESupportedResourceFileTypes::Unsupported;
}

ResourcePack Resource::LoadFile( const string& a_Name, const string& a_Path )
{
	ResourcePack ResultResourcePack;
	
	size_t TotalMeshes = 0u;
	size_t TotalSkeletons = 0u;
	size_t TotalAnimations = 0u;
	// ...
	
	auto HandleFBX_Mesh = [ &ResultResourcePack, &TotalSkeletons ]( const aiScene* a_Scene, const size_t a_Index, const string& a_Name, const string& a_Path )
	{
		const aiMesh* AssimpMesh = a_Scene->mMeshes[ a_Index ];
		const MeshHandle Result = MakeResource< Mesh >( a_Name, a_Path );

		// Handles vertices.
		Result->m_Vertices.resize( AssimpMesh->mNumVertices );
		for ( size_t i = 0; i < AssimpMesh->mNumVertices; ++i )
		{
			MeshVertex Vertex;
			Vertex.Position.x = AssimpMesh->mVertices[ i ].x;
			Vertex.Position.y = AssimpMesh->mVertices[ i ].y;
			Vertex.Position.z = AssimpMesh->mVertices[ i ].z;

			// We are assuming that normals, tangents and bitangents are provided.
			Vertex.Normal.x = AssimpMesh->mNormals[ i ].x;
			Vertex.Normal.y = AssimpMesh->mNormals[ i ].y;
			Vertex.Normal.z = AssimpMesh->mNormals[ i ].z;
			Vertex.Tangent.x = AssimpMesh->mTangents[ i ].x;
			Vertex.Tangent.y = AssimpMesh->mTangents[ i ].y;
			Vertex.Tangent.z = AssimpMesh->mTangents[ i ].z;
			Vertex.Bitangent.x = AssimpMesh->mBitangents[ i ].x;
			Vertex.Bitangent.y = AssimpMesh->mBitangents[ i ].y;
			Vertex.Bitangent.z = AssimpMesh->mBitangents[ i ].z;

			Result->m_Vertices[ i ] = Vertex;
		}

		// Handle triangles.
		Result->m_Triangles.resize( AssimpMesh->mNumFaces );
		if ( AssimpMesh->HasFaces() )
		{
			for ( size_t i = 0; i < AssimpMesh->mNumFaces; ++i )
			{
				MeshTriangle Triangle;

				// We are assuming that all faces have been triangulated,
				// so we can ignore number of indices and assume 3.
				Triangle.x = AssimpMesh->mFaces[ i ].mIndices[ 0u ];
				Triangle.y = AssimpMesh->mFaces[ i ].mIndices[ 1u ];
				Triangle.z = AssimpMesh->mFaces[ i ].mIndices[ 2u ];
			}
		}

		// Handle UV's.
		auto HandleUVChannel = [ &Result, AssimpMesh ]( const size_t a_ChannelIndex )
		{
			// This is the number of components in a UV channel.
			// eg. 2 -> UV -> x,y per vertex.
			// We will just assume 2 but should be logging when we encounter
			// a mesh with more than 2 uv components in any of its channels.

			Result->m_UVChannelNames[ a_ChannelIndex ] = AssimpMesh->mTextureCoordsNames ? AssimpMesh->mTextureCoordsNames[ a_ChannelIndex ]->C_Str() : "";
			vector< vec2 >& UVChannel = Result->m_UVChannels[ a_ChannelIndex ];
			const aiVector3D* From = AssimpMesh->mTextureCoords[ a_ChannelIndex ];
			UVChannel.resize( AssimpMesh->mNumVertices );

			for ( size_t i = 0; i < AssimpMesh->mNumVertices; ++i )
			{
				UVChannel[ i ].x = From[ i ].x;
			}
		};

		Result->m_UVChannelCount = AssimpMesh->GetNumUVChannels();
		for ( size_t i = 0; i < AssimpMesh->GetNumUVChannels(); ++i )
			HandleUVChannel( i );

		// Handle bones.
		if ( AssimpMesh->HasBones() )
		{
			Result->m_BoneNames.resize( AssimpMesh->mNumBones );
			Result->m_BoneWeights.resize( AssimpMesh->mNumVertices );

			// We need to track what nodes in the scene are bone nodes for the mesh.
			vector< const aiNode* > BoneNodes( AssimpMesh->mNumBones );

			for ( size_t i = 0; i < AssimpMesh->mNumBones; ++i )
			{
				const aiBone* Bone = AssimpMesh->mBones[ i ];

				// Record down the bone node we are using here.
				BoneNodes[ i ] = a_Scene->mRootNode->FindNode( Bone->mName );

				// We care about the names of the bones that this mesh is referencing.
				// This means nothing with just a mesh, but if it has an assigned skeleton, then
				// the names are used to match up mesh bone data with skeleton/animation data.
				Result->m_BoneNames[ i ] = Bone->mName.C_Str();

				for ( size_t j = 0; j < Bone->mNumWeights; ++j )
				{
					const aiVertexWeight& VertexWeight = Bone->mWeights[ j ];
					const uint32_t Index = VertexWeight.mVertexId;
					const ai_real Weight = VertexWeight.mWeight;
					auto [ BoneWeight ] = Result->m_BoneWeights[ Index ];

					// We want to look through all of the weights for a given vertex, and find the
					// next one that is unset and set that.
					for ( size_t k = 0; k < MeshBoneWeight::MaxBoneWeights; ++k )
					{
						// Once we've found one with an index of -1, then we can insert the data at
						// this slot.
						if ( BoneWeight[ k ].Index == MeshBoneWeightPair::Unset )
						{
							// Index here is the index into the bone array that this vertex is influenced by.
							BoneWeight[ k ].Index = ( uint32_t )i;

							// Weight here is how much by the bone it is influenced.
							BoneWeight[ k ].Weight = Weight;
							break;
						}

						// CHECK( "Found a vertex that is being influenced by more than 4 bones." );
					}
				}
			}

			// We now can construct a skeleton resource from the above bone nodes we recorded down as
			// we processed the meshes bone weights.
			struct Node
			{
				const aiNode* Reference;
				vector< Node* > Children;
			};

			map< const aiNode*, Node > BoneNodeMap;

			Invoker< Node*, const aiNode* > CreateNode;
			CreateNode = [ &BoneNodeMap, &CreateNode ]( const aiNode* a_Node ) -> Node*
			{
				if ( BoneNodeMap.contains( a_Node ) )
				{
					return nullptr;
				}

				auto& BoneNodeMapEntry = BoneNodeMap[ a_Node ];
				BoneNodeMapEntry.Reference = a_Node;

				return &BoneNodeMapEntry;
			};

			for ( const aiNode* BoneNode : BoneNodes )
			{
				const aiNode* Current = BoneNode;
				Node* Last = nullptr;

				while ( Current )
				{
					Node* CreatedNode = nullptr;

					if ( !BoneNodeMap.contains( Current ) )
					{
						auto& BoneNodeMapEntry = BoneNodeMap[ Current ];
						BoneNodeMapEntry.Reference = Current;
						CreatedNode = &BoneNodeMapEntry;
					}

					if ( !CreatedNode )
					{
						break;
					}

					if ( Last )
					{
						CreatedNode->Children.push_back( Last );
					}

					Last = CreatedNode;
					Current = Current->mParent;
				}
			}


			// Lets also capture a skeleton here.
			//SkeletonHandle SkeletonResult = MakeResource< RSkeleton >( a_Name + "_Skeleton" + std::to_string( TotalSkeletons++ ) );
			//SkeletonResult->m_Bones.resize( Mesh->mNumBones );
			
				// Add the node for this bone to the NodeToIndices map.
				//NodeToIndices[ BoneNode ] = i;

				// Populate bone data too.
				//SkeletonResult->m_BoneLookup[ BoneNode->mName.C_Str() ] = i;
				//::Bone ToAdd;
				//ToAdd.Name = BoneNode->mName.C_Str();
				//ToAdd.Local = glm::transpose( glm::make_mat4( ( ai_real* )&BoneNode->mTransformation ) );
				//ToAdd.LocalToModel = glm::transpose( glm::make_mat4( ( ai_real* )&Bone->mOffsetMatrix ) );
				//ToAdd.ModelToLocal = glm::inverse( ToAdd.LocalToModel );
				//
				//// Add the Bone to the Skeleton.
				//SkeletonResult->m_Bones[ i ] = ToAdd;

			// The above has a problem. Although the mesh tells us what bones have weights for the mesh specifically, it doesn't
			// include bones that are in between. It only tells us the ones who have vertices assigned to them.
			// What we effectively have is a tree structure that has gaps in it. We need to complete the tree.
			// We can do this by recursing downwards for each bone node we have a record of, and stop the recursion when we hit
			// a node we already have recorded down.
			

			// The other technique is to recurse downwards t

			// We now need to go back over the whole list of bones and set parent indices.
			/*for ( auto& NodeToIndex : NodeToIndices )
			{
				const aiNode* Node = NodeToIndex.first;
				const uint32_t BoneIndex = NodeToIndex.second;
				auto& Bone = SkeletonResult->m_Bones[ BoneIndex ];
				const auto ParentIter = NodeToIndices.find( Node->mParent );

				if ( ParentIter == NodeToIndices.end() )
				{
					Bone.Parent = Bone::NoParent;
				}
				else
				{
					Bone.Parent = ParentIter->second;
				}
			}*/

			// Should verify here that all bone weights of all vertices all add to approximately 1.0f.

			// Register the skeleton as a child resource for the Mesh. Label it as "Skeleton" so the Mesh knows it's the
			// skeleton that makes the mesh a skeletal mesh.
			//Result->m_Children.AddResource( SkeletonResult, "Skeleton" );

			// Add SkeletonResult to the ResultResourcePack too.
			//ResultResourcePack.AddResource( SkeletonResult );
		}

		// We want to add the resource by its name in the fbx file so we can refer back to it when we load skeletons and link them.
		ResultResourcePack.AddResource( Result, AssimpMesh->mName.C_Str() );
	};

	auto HandleFBX_Skeleton = [ &ResultResourcePack ]( const aiScene* a_Scene, const size_t a_Index, const string& a_Name, const string& a_Path )
	{
		const aiSkeleton* AssimpSkeleton = a_Scene->mSkeletons[ a_Index ];
		const SkeletonHandle Result = MakeResource< Skeleton >( a_Name, a_Path );

		Result->m_Bones.resize( AssimpSkeleton->mNumBones );
		for ( size_t i = 0; i < AssimpSkeleton->mNumBones; ++i )
		{
			const aiSkeletonBone* SkeletonBone = AssimpSkeleton->mBones[ i ];
			Result->m_BoneLookup[ SkeletonBone->mNode->mName.C_Str() ] = ( uint32_t )i;

			Bone ToAdd;
			ToAdd.Name = SkeletonBone->mNode->mName.C_Str();
			ToAdd.Local = glm::transpose( glm::make_mat4( ( ai_real* )&SkeletonBone->mLocalMatrix ) );
			ToAdd.Parent = SkeletonBone->mParent;
			ToAdd.LocalToModel = glm::transpose( glm::make_mat4( ( ai_real* )&SkeletonBone->mOffsetMatrix ) );
			ToAdd.ModelToLocal = glm::inverse( ToAdd.LocalToModel );

			Result->m_Bones[ i ] = ToAdd;
		}

		ResultResourcePack.AddResource( Result, AssimpSkeleton->mName.C_Str() );

		// Look for a mesh with same name and add this skeleton to it.
		const MeshHandle SkeletalMesh = ResultResourcePack.GetResourceByAlias< Mesh >( AssimpSkeleton->mName.C_Str() );

		// Add this skeleton as a child to the mesh, making it a skeletal mesh.
		SkeletalMesh->m_Children.AddResource( Result, "Skeleton" );
	};

	auto HandleFBX_Animation = [ &ResultResourcePack ]( const aiScene* a_Scene, const size_t a_Index, const string& a_Name, const string& a_Path )
	{
		const aiAnimation* AssimpAnimation = a_Scene->mAnimations[ a_Index ];
		const AnimationHandle Result = MakeResource< Animation >( a_Name, a_Path );
		Result->m_Channels.reserve( AssimpAnimation->mNumChannels );
		Result->m_Duration = ( Animation::TimeType )( AssimpAnimation->mDuration / AssimpAnimation->mTicksPerSecond );

		// Animations don't have a concept of playback speed, only ticks/second and ticks.
		Result->m_PlaybackSpeed = 1.0f;

		for ( size_t i = 0; i < AssimpAnimation->mNumChannels; ++i )
		{
			const aiNodeAnim* Channel = AssimpAnimation->mChannels[ i ];
			Result->m_Lookup[ Channel->mNodeName.C_Str() ] = Result->m_Channels.size();
			AnimationChannel& NewChannel = Result->m_Channels.emplace_back();

			NewChannel.Positions.KeyFrames.reserve( Channel->mNumPositionKeys );
			for ( size_t j = 0; j < Channel->mNumPositionKeys; ++j )
			{
				const aiVectorKey& PositionKey = Channel->mPositionKeys[ j ];
				auto& NewPosition = NewChannel.Positions.KeyFrames.emplace_back();
				NewPosition.Time = ( Animation::TimeType )PositionKey.mTime;
				NewPosition.Value.x = PositionKey.mValue.x;
				NewPosition.Value.y = PositionKey.mValue.y;
				NewPosition.Value.z = PositionKey.mValue.z;
			}

			NewChannel.Rotations.KeyFrames.reserve( Channel->mNumRotationKeys );
			for ( size_t j = 0; j < Channel->mNumRotationKeys; ++j )
			{
				const aiQuatKey& RotationKey = Channel->mRotationKeys[ j ];
				auto& NewRotation = NewChannel.Rotations.KeyFrames.emplace_back();
				NewRotation.Time = ( Animation::TimeType )RotationKey.mTime;
				NewRotation.Value.w = RotationKey.mValue.w;
				NewRotation.Value.x = RotationKey.mValue.x;
				NewRotation.Value.y = RotationKey.mValue.y;
				NewRotation.Value.z = RotationKey.mValue.z;
			}

			NewChannel.Scales.KeyFrames.reserve( Channel->mNumScalingKeys );
			for ( size_t j = 0; j < Channel->mNumScalingKeys; ++j )
			{
				const aiVectorKey& ScaleKey = Channel->mScalingKeys[ j ];
				auto& NewScale = NewChannel.Scales.KeyFrames.emplace_back();
				NewScale.Time = ( Animation::TimeType )ScaleKey.mTime;
				NewScale.Value.x = ScaleKey.mValue.x;
				NewScale.Value.y = ScaleKey.mValue.y;
				NewScale.Value.z = ScaleKey.mValue.z;
			}

			// Need to support these.
			// aiMeshAnim;
			// aiMeshMorphAnim;

			ResultResourcePack.AddResource( Result, AssimpAnimation->mName.C_Str() );
		}
	};

	auto HandleFBX = [ &ResultResourcePack, &a_Name, &a_Path, 
		&HandleFBX_Mesh, &TotalMeshes, 
		&HandleFBX_Skeleton, &TotalSkeletons,
		&HandleFBX_Animation, &TotalAnimations ]
	{
		Assimp::Importer Importer;

		// Collapse the Pre/Post Rotation/Translation... etc nodes.
		Importer.SetPropertyBool( AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false );

		constexpr int PostProcessFlags = 
			aiProcess_GlobalScale | 
			aiProcess_OptimizeMeshes | 
			aiProcess_RemoveRedundantMaterials | 
			aiProcess_PopulateArmatureData | 
			aiProcess_Triangulate | 
			aiProcess_LimitBoneWeights | 
			aiProcess_PopulateArmatureData |
			aiProcess_GenSmoothNormals | 
			aiProcess_SplitByBoneCount | 
			aiProcess_CalcTangentSpace;

		const aiScene* Scene = Importer.ReadFile( a_Path.c_str(), PostProcessFlags );

		/*static void( *DebugPrintScene )( const aiScene*, const aiNode*, const int ) = nullptr;
		DebugPrintScene = +[]( const aiScene* a_Scene, const aiNode* a_Node, const int a_Level )
		{
			std::cout << string( a_Level * 2, ' ' ) << a_Node->mName.C_Str() << std::endl;

			if ( a_Node->mNumMeshes > 0u )
			{
				std::cout << string( a_Level * 2, ' ' ) << "Meshes: ";
				for ( int i = 0; i < a_Node->mNumMeshes; ++i )
				{
					std::cout << a_Scene->mMeshes[ a_Node->mMeshes[ i ] ]->mName.C_Str() << ", ";
				}
				std::cout << std::endl;
			}

			for ( int i = 0; i < a_Node->mNumChildren; ++i )
			{
				DebugPrintScene( a_Scene, a_Node->mChildren[ i ], a_Level + 1 );
			}
		};

		DebugPrintScene( Scene, Scene->mRootNode, 0 );*/
		
		// MESH
		for ( size_t i = 0; i < Scene->mNumMeshes; ++i )
			HandleFBX_Mesh( Scene, i, a_Name + "_Mesh" + std::to_string( TotalMeshes++ ), a_Path );

		// SKELETON
		for ( size_t i = 0; i < Scene->mNumSkeletons; ++i )
			HandleFBX_Skeleton( Scene, i, a_Name + "_Skeleton" + std::to_string( TotalSkeletons++ ), a_Path );

		// ANIMATION
		for ( size_t i = 0; i < Scene->mNumAnimations; ++i )
			HandleFBX_Animation( Scene, i, a_Name + "_Animation" + std::to_string( TotalAnimations++ ), a_Path );

		// MATERIALS

		// TEXTURES

		// ...
	};

	switch ( DetectFileType( a_Path ) )
	{
	case ESupportedResourceFileTypes::Fbx:
		HandleFBX();
		break;
	default:
		break;
	}

	return ResultResourcePack;
}