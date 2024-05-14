#include <Resource/Animation.hpp>

AnimationTrackHead::PositionType AnimationTrackHead::GetCurrentPosition() const
{
	const PositionKeyFrameType& BeforeKeyFrame = m_Channel->Positions.KeyFrames[ m_CurrentPositionIndex ];
	const PositionKeyFrameType& AfterKeyFrame = m_Channel->Positions.KeyFrames[ m_CurrentPositionIndex + 1u ];

	return PositionKeyFrameType::Interpolate( BeforeKeyFrame, AfterKeyFrame, m_CurrentTime );
}

AnimationTrackHead::RotationType AnimationTrackHead::GetCurrentRotation() const
{
	const RotationKeyFrameType& BeforeKeyFrame = m_Channel->Rotations.KeyFrames[ m_CurrentRotationIndex ];
	const RotationKeyFrameType& AfterKeyFrame = m_Channel->Rotations.KeyFrames[ m_CurrentRotationIndex + 1u ];

	return RotationKeyFrameType::Interpolate( BeforeKeyFrame, AfterKeyFrame, m_CurrentTime );
}

AnimationTrackHead::ScaleType AnimationTrackHead::GetCurrentScale() const
{
	const ScaleKeyFrameType& BeforeKeyFrame = m_Channel->Scales.KeyFrames[ m_CurrentRotationIndex ];
	const ScaleKeyFrameType& AfterKeyFrame = m_Channel->Scales.KeyFrames[ m_CurrentRotationIndex + 1u ];

	return ScaleKeyFrameType::Interpolate( BeforeKeyFrame, AfterKeyFrame, m_CurrentTime );
}

bool AnimationTrackHead::IsFinished() const
{
	return m_CurrentTime == m_Duration;
}

void AnimationTrackHead::SeekStart()
{
	m_CurrentTime = 0.0f;
	m_CurrentPositionIndex = m_Channel->Positions.Size() > 0u ? 0u : -1;
	m_CurrentRotationIndex = m_Channel->Rotations.Size() > 0u ? 0u : -1;
	m_CurrentScaleIndex = m_Channel->Scales.Size() > 0u ? 0u : -1;
}

void AnimationTrackHead::SeekEnd()
{
	m_CurrentTime = m_Duration;
	m_CurrentPositionIndex = m_Channel->Positions.Size() - 1u;
	m_CurrentRotationIndex = m_Channel->Rotations.Size() - 1u;
	m_CurrentScaleIndex = m_Channel->Scales.Size() - 1u;
}

void AnimationTrackHead::Seek( const TimeType a_Offset )
{
	if ( a_Offset == 0.0f ) return;

	m_CurrentTime += a_Offset;

	// Clamp current time to valid values.
	m_CurrentTime = glm::clamp( m_CurrentTime, 0.0f, m_Duration );

	if ( a_Offset > 0.0f )
	{
		const PositionKeyFrameType& PositionKeyFrame = m_Channel->Positions.FindAfter( m_CurrentTime, m_CurrentPositionIndex );
		const RotationKeyFrameType& RotationKeyFrame = m_Channel->Rotations.FindAfter( m_CurrentTime, m_CurrentRotationIndex );
		const ScaleKeyFrameType& ScaleKeyFrame = m_Channel->Scales.FindAfter( m_CurrentTime, m_CurrentScaleIndex );

		m_CurrentPositionIndex = &PositionKeyFrame - m_Channel->Positions.KeyFrames.data();
		m_CurrentRotationIndex = &RotationKeyFrame - m_Channel->Rotations.KeyFrames.data();
		m_CurrentScaleIndex = &ScaleKeyFrame - m_Channel->Scales.KeyFrames.data();
	}
	else
	{
		const PositionKeyFrameType& PositionKeyFrame = m_Channel->Positions.FindBefore( m_CurrentTime, m_CurrentPositionIndex );
		const RotationKeyFrameType& RotationKeyFrame = m_Channel->Rotations.FindBefore( m_CurrentTime, m_CurrentRotationIndex );
		const ScaleKeyFrameType& ScaleKeyFrame = m_Channel->Scales.FindBefore( m_CurrentTime, m_CurrentScaleIndex );

		m_CurrentPositionIndex = &PositionKeyFrame - m_Channel->Positions.KeyFrames.data();
		m_CurrentRotationIndex = &RotationKeyFrame - m_Channel->Rotations.KeyFrames.data();
		m_CurrentScaleIndex = &ScaleKeyFrame - m_Channel->Scales.KeyFrames.data();
	}
}

vector< AnimationHead::FrameType > AnimationHead::GetCurrentFrames() const
{
	vector< FrameType > Frames;
	Frames.resize( m_TrackHeads.size() );

	for ( size_t i = 0; i < m_TrackHeads.size(); ++i )
	{
		Frames[ i ] = m_TrackHeads[ i ].GetCurrentFrame();
	}

	return Frames;
} 

vector< mat4 > AnimationHead::GetCurrentTransforms() const
{
	vector< mat4 > Transforms;
	Transforms.resize( m_TrackHeads.size() );

	for ( size_t i = 0; i < m_TrackHeads.size(); ++i )
	{
		Transforms[ i ] = m_TrackHeads[ i ].GetCurrentTransform();
	}

	return Transforms;
}