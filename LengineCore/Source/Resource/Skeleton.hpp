#pragma once
#include <string>
#include <vector>
#include <map>
#include <Math/Math.h>
#include <Resource/Resource.hpp>

using std::string;
using std::vector;
using std::map;

struct Bone
{
    static constexpr uint32_t NoParent = -1;

    // Parent bone index into the skeleton array.
    uint32_t Parent = NoParent;

    // The name of the bone. Used to match up bone data to mesh data.
    string Name;

    // The transformation of this bone in the bind pose
    mat4 Local = mat4( 1 );

    // Multiplying a vertex in Model space will move it into Local space.
    mat4 ModelToLocal = mat4( 1 );

    // Multiplying a vertex in Local space will move it into Model space.
    mat4 LocalToModel = mat4( 1 );
};

DEFINE_RESOURCE_TYPE( Skeleton )
{
public:

    DEFINE_RESOURCE_BODY( Skeleton );

    size_t GetBoneCount() const { return m_Bones.size(); }
    const Bone& GetBone( const size_t a_Index ) const { return const_cast< RSkeleton* >( this )->GetBone( a_Index ); }
    Bone& GetBone( const size_t a_Index ) { return m_Bones[ a_Index ]; }
    const Bone& GetBone( const string& a_Name ) const { return const_cast< RSkeleton* >( this )->GetBone( a_Name ); }
    Bone& GetBone( const string& a_Name ) { return m_Bones[ m_BoneLookup.find( a_Name )->second ]; }

private:

    vector< Bone > m_Bones;
    map< string, uint32_t > m_BoneLookup;
};