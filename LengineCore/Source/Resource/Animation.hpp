#pragma once
#include <Resource/Resource.hpp>
#include <Math/Math.h>

#undef GetCurrentTime

#include <algorithm>
#include <map>
#include <vector>

using std::map;
using std::vector;

template < typename _Value, typename _Time >
struct KeyFrame
{
	using ValueType = _Value;
	using TimeType = _Time;
	ValueType Value;
	TimeType Time;

	// Given two bounding key frames, find the value at the given time.
	static ValueType Interpolate( const KeyFrame& a_From, const KeyFrame& a_To, const TimeType a_Time )
	{
		return glm::mix( a_From.Value, a_To.Value, (quat::value_type)(( a_Time - a_From.Time ) / ( a_To.Time - a_From.Time )));
	}
};

template < typename _Value, typename _Time >
struct AnimationTrack
{
	using ValueType = _Value;
	using TimeType = _Time;
	using KeyFrameType = KeyFrame< ValueType, TimeType >;

	// This represents the known values at given times of the animation.
	vector< KeyFrameType > KeyFrames;

	// Get a keyframe at a given index.
	const KeyFrameType& GetKeyFrame( const size_t a_Index ) const { return const_cast< AnimationTrack* >( this )->GetKeyFrame( a_Index ); }
	// Get a keyframe at a given index.
	KeyFrameType& GetKeyFrame( const size_t a_Index ) { return KeyFrames[ a_Index ]; }

	// Find the keyframe at, or directly before a given time in ticks.
	const KeyFrameType& FindBefore( const TimeType a_Time, const size_t a_From = 0u ) const { return const_cast< AnimationTrack* >( this )->FindBefore( a_Time, a_From ); }
	// Find the keyframe at, or directly before a given time in ticks.
	KeyFrameType& FindBefore( const TimeType a_Time, const size_t a_From = 0u );

	// Find the keyframe at, or directly after a given time in ticks.
	const KeyFrameType& FindAfter( const TimeType a_Time, const size_t a_From = 0u ) const { return const_cast< AnimationTrack* >( this )->FindAfter( a_Time, a_From ); }
	// Find the keyframe at, or directly after a given time in ticks.
	KeyFrameType& FindAfter( const TimeType a_Time, const size_t a_From = 0u );

	// Get the total number of keyframes.
	size_t Size() const { return KeyFrames.size(); }
};

template < typename _Data, typename _Time >
typename AnimationTrack< _Data, _Time >::KeyFrameType& AnimationTrack< _Data, _Time >::FindBefore( const TimeType a_Time, const size_t a_From )
{
	KeyFrameType* LastKeyFrame = &KeyFrames.front() + a_From;

	for ( size_t i = a_From; i < KeyFrames.size(); ++i )
	{
		KeyFrameType& KeyFrame = KeyFrames[ i ];

		if ( KeyFrame.Time > a_Time )
		{
			return *LastKeyFrame;
		}

		LastKeyFrame = &KeyFrame;
	}

	return *LastKeyFrame;
}

template < typename _Data, typename _Time >
typename AnimationTrack< _Data, _Time >::KeyFrameType& AnimationTrack< _Data, _Time >::FindAfter( const TimeType a_Time, const size_t a_From )
{
	KeyFrameType* LastKeyFrame = &KeyFrames.front() + a_From;

	for ( size_t i = KeyFrames.size() - 1u; i >= a_From; --i )
	{
		KeyFrameType& KeyFrame = KeyFrames[ i ];

		if ( KeyFrame.Time < a_Time )
		{
			return *LastKeyFrame;
		}

		LastKeyFrame = &KeyFrame;
	}

	return *LastKeyFrame;
}

struct AnimationFrame
{
	using PositionType			= vec3;
	using RotationType			= quat;
	using ScaleType				= vec3;
	using TimeType				= float;

	PositionType Position;
	RotationType Rotation;
	ScaleType Scale;
	TimeType Time;

	mat4 AsTransform() const { return glm::translate( mat4(1), Position ) * glm::toMat4( Rotation ) * glm::scale( mat4(1), Scale ); }
};

struct AnimationChannel
{
	using FrameType				= AnimationFrame;
	using PositionType			= FrameType::PositionType;
	using RotationType			= FrameType::RotationType;
	using ScaleType				= FrameType::ScaleType;
	using TimeType				= FrameType::TimeType;
	using PositionTrackType		= AnimationTrack< PositionType, TimeType >;
	using RotationTrackType		= AnimationTrack< RotationType, TimeType >;
	using ScaleTrackType		= AnimationTrack< ScaleType, TimeType >;

	PositionTrackType	Positions;
	RotationTrackType	Rotations;
	ScaleTrackType		Scales;
};

DEFINE_RESOURCE_TYPE( Animation )
{
public:

	DEFINE_RESOURCE_BODY( Animation );

	using FrameType				= AnimationFrame;
	using PositionType			= FrameType::PositionType;
	using RotationType			= FrameType::RotationType;
	using ScaleType				= FrameType::ScaleType;
	using TimeType				= FrameType::TimeType;
	using PositionTrackType		= AnimationTrack< PositionType, TimeType >;
	using RotationTrackType		= AnimationTrack< RotationType, TimeType >;
	using ScaleTrackType		= AnimationTrack< ScaleType, TimeType >;
	using PositionKeyFrameType	= PositionTrackType::KeyFrameType;
	using RotationKeyFrameType	= RotationTrackType::KeyFrameType;
	using ScaleKeyFrameType		= ScaleTrackType::KeyFrameType;

	// Duration of the animation given playback speed.
	float GetPlaybackLength() const { return m_Duration / m_PlaybackSpeed; }

	// Duration of the animation clip in seconds.
	float GetDuration() const { return m_Duration; }

	// The playback speed of the animation clip.
	float GetPlaybackSpeed() const { return m_PlaybackSpeed; }

	// Get channel by index.
	const AnimationChannel& GetChannel( const size_t a_Index ) const { return const_cast< RAnimation* >( this )->GetChannel( a_Index ); }
	// Get channel by index.
	AnimationChannel& GetChannel( const size_t a_Index ) { return m_Channels[ a_Index ]; }

	// Get channel by index.
	const AnimationChannel& GetChannel( const string& a_Name ) const { return const_cast< RAnimation* >( this )->GetChannel( a_Name ); }
	// Get channel by index.
	AnimationChannel& GetChannel( const string& a_Name ) { return m_Channels[ m_Lookup.at( a_Name ) ]; }
	
	// Get all channels.
	const vector< AnimationChannel >& GetChannels() const { return m_Channels; }
	// Get all channels.
	vector< AnimationChannel >& GetChannels() { return m_Channels; }

	// Get the count of channels.
	size_t GetChannelCount() const { return m_Channels.size(); }

protected:

	float m_Duration;
	float m_PlaybackSpeed;

	vector< AnimationChannel >	m_Channels;
	map< string, size_t >		m_Lookup;
};

// Stores current location in animation tracks and
// allows to get interpolated position, rotation and scale at current time.
class AnimationTrackHead
{
public:
	
	using FrameType						= AnimationFrame;
	using PositionType					= RAnimation::PositionType;
	using RotationType					= RAnimation::RotationType;
	using ScaleType						= RAnimation::ScaleType;
	using TimeType						= RAnimation::TimeType;
	using PositionTrackType				= RAnimation::PositionTrackType;
	using RotationTrackType				= RAnimation::RotationTrackType;
	using ScaleTrackType				= RAnimation::ScaleTrackType;
	using PositionKeyFrameType			= RAnimation::PositionKeyFrameType;
	using RotationKeyFrameType			= RAnimation::RotationKeyFrameType;
	using ScaleKeyFrameType				= RAnimation::ScaleKeyFrameType;

	AnimationTrackHead() = default;
	AnimationTrackHead( const AnimationTrackHead& ) = default;
	AnimationTrackHead( const AnimationChannel& a_Channel, const TimeType a_Duration )
		: m_Channel( &a_Channel )
		, m_CurrentPositionIndex( a_Channel.Positions.Size() > 0u ? 0u : -1 )
		, m_CurrentRotationIndex( a_Channel.Rotations.Size() > 0u ? 0u : -1 )
		, m_CurrentScaleIndex( a_Channel.Scales.Size() > 0u ? 0u : -1 )
		, m_CurrentTime( 0.0f )
		, m_Duration( a_Duration )
	{}
	AnimationTrackHead& operator=( const AnimationTrackHead& ) = default;
	AnimationTrackHead& operator=( const AnimationChannel& a_Channel ) { m_Channel = &a_Channel; return *this; }

	// Get the animation being read.
	const AnimationChannel* GetChannel() const { return m_Channel; }

	// Get the current frame of the animation's values.
	FrameType GetCurrentFrame() const { return FrameType{ GetCurrentPosition(), GetCurrentRotation(), GetCurrentScale() }; }

	// Get the current frame as a transform.
	mat4 GetCurrentTransform() const { return GetCurrentFrame().AsTransform(); }

	// Get the current interpolated position of the animation clip.
	PositionType GetCurrentPosition() const;

	// Get the current interpolated rotation of the animation clip.
	RotationType GetCurrentRotation() const;

	// Get the current interpolated scale of the animation clip.
	ScaleType GetCurrentScale() const;

	// Get the current time of the head in the clip.
	TimeType GetCurrentTime() const { return m_CurrentTime; }

	// Is the head at the end of the clip?
	bool IsFinished() const;

	// Seek the head to the start of the clip.
	void SeekStart();

	// Seek the head to the end of the clip such that IsFinished will be true.
	void SeekEnd();

	// Seek from the current position.
	void Seek( const TimeType a_Time );
	
	// Seek from the current position.
	AnimationTrackHead& operator+=( const TimeType a_Offset ) { Seek( a_Offset ); return *this; }
	
	// Seek from the current position.
	AnimationTrackHead operator+( const TimeType a_Offset ) const { AnimationTrackHead Temp = *this; Temp += a_Offset; return Temp; }

	// Seek from the current position.
	AnimationTrackHead& operator-=( const TimeType a_Offset ) { Seek( -a_Offset ); return *this; }
	
	// Seek from the current position.
	AnimationTrackHead operator-( const TimeType a_Offset ) const { AnimationTrackHead Temp = *this; Temp -= a_Offset; return Temp; }

	// Get the current frame of the animation.
	FrameType operator*() const { return GetCurrentFrame(); }

private:

	const AnimationChannel* m_Channel;
	size_t m_CurrentPositionIndex;
	size_t m_CurrentRotationIndex;
	size_t m_CurrentScaleIndex;
	TimeType m_CurrentTime;
	TimeType m_Duration;
};

class AnimationHead
{
public:

	using FrameType						= AnimationFrame;
	using PositionType					= RAnimation::PositionType;
	using RotationType					= RAnimation::RotationType;
	using ScaleType						= RAnimation::ScaleType;
	using TimeType						= RAnimation::TimeType;
	using PositionTrackType				= RAnimation::PositionTrackType;
	using RotationTrackType				= RAnimation::RotationTrackType;
	using ScaleTrackType				= RAnimation::ScaleTrackType;
	using PositionKeyFrameType			= RAnimation::PositionKeyFrameType;
	using RotationKeyFrameType			= RAnimation::RotationKeyFrameType;
	using ScaleKeyFrameType				= RAnimation::ScaleKeyFrameType;

	AnimationHead() = default;
	AnimationHead( const AnimationHead& ) = default;
	AnimationHead( AnimationHead&& ) = default;
	AnimationHead( const RAnimation& a_Animation )
		: m_Animation( &a_Animation )
	{
		m_TrackHeads.reserve( m_Animation->GetChannelCount() );

		for ( auto& Channel : m_Animation->GetChannels() )
		{
			m_TrackHeads.emplace_back( Channel, m_Animation->GetDuration() );
		}
	}
	AnimationHead& operator=( const AnimationHead& ) = default;
	AnimationHead& operator=( AnimationHead&& ) = default;

	// Get the owning animation.
	const RAnimation* GetAnimation() const { return m_Animation; }

	// Get current frame of an animation track at an index.
	FrameType GetCurrentFrame( const size_t a_Index ) const { return m_TrackHeads[ a_Index ].GetCurrentFrame(); }

	// Get all current frames.
	vector< FrameType > GetCurrentFrames() const;
	
	// Get current transform of an animation track at an index.
	mat4 GetCurrentTransform( const size_t a_Index ) const { return m_TrackHeads[ a_Index ].GetCurrentTransform(); }

	// Get all current transforms.
	vector< mat4 > GetCurrentTransforms() const;
	
	// Is the head at the end of the clip?
	bool IsFinished() const { return m_Animation ? m_TrackHeads[ 0u ].IsFinished() : true; }
	
	// Seek the head to the start of the clip.
	void SeekStart() { for( auto& TrackHead : m_TrackHeads ) TrackHead.SeekStart(); }

	// Seek the head to the end of the clip such that IsFinished will be true.
	void SeekEnd() { for( auto& TrackHead : m_TrackHeads ) TrackHead.SeekEnd(); }
	
	// Seek from the current position.
	void Seek( const TimeType a_Time ) { for( auto& TrackHead : m_TrackHeads ) TrackHead.Seek( a_Time ); }

	// Seek from the current position.
	AnimationHead& operator+=( const TimeType a_Offset ) { Seek( a_Offset ); return *this; }
	
	// Seek from the current position.
	AnimationHead operator+( const TimeType a_Offset ) const { AnimationHead Temp = *this; Temp += a_Offset; return Temp; }

	// Seek from the current position.
	AnimationHead& operator-=( const TimeType a_Offset ) { Seek( -a_Offset ); return *this; }
	
	// Seek from the current position.
	AnimationHead operator-( const TimeType a_Offset ) const { AnimationHead Temp = *this; Temp -= a_Offset; return Temp; }

	// Get the current frame of the animation.
	vector< FrameType > operator*() const { return GetCurrentFrames(); }

private:

	const RAnimation*				m_Animation;
	vector< AnimationTrackHead >	m_TrackHeads;
};