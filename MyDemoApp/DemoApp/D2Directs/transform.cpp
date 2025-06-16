#include "pch.h"
#include "transform.h"

D2D1::Matrix3x2F Transform::GetMatrix() const
{
	if (m_Parrent != nullptr )
	{
		return D2D1::Matrix3x2F::Scale(m_Scale.x, m_Scale.y) *
			   D2D1::Matrix3x2F::Rotation(Angle) *
			   D2D1::Matrix3x2F::Translation(m_pos.x, m_pos.y);
	}

	return D2D1::Matrix3x2F::Scale(m_Scale.x, m_Scale.y) *
		D2D1::Matrix3x2F::Rotation(Angle) *
		D2D1::Matrix3x2F::Translation(m_pos.x, m_pos.y);
}

	// ---------------- Setting Method ----------------

void Transform::SetPosition(D2D1_VECTOR_2F movement)
{
	m_pos.x = movement.x;
	m_pos.y = movement.y;
}

void Transform::SetScale(D2D1_VECTOR_2F rotate)
{
	m_Scale.x = rotate.x; 
	m_Scale.y = rotate.y; 
}

void Transform::SetAngle(float angle)
{
	Angle = angle; 
}

void Transform::SetParrent(Transform* parrant)
{
	m_Parrent = parrant; 
}

	// ------------------ Get Method ------------------

D2D1_VECTOR_2F Transform::GetPosition() const
{
	return m_pos; 
}

D2D1_VECTOR_2F Transform::GetScale() const
{
	return m_Scale;
}

float Transform::GetAngle() const
{
	return Angle;
}

// ------------------ Get Parrent ------------------

Transform* Transform::GetParrent() const
{
	return m_Parrent;
}

