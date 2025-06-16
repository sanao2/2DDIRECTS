#pragma once
#include <vector> 

class Transform
{
private:
	// �θ� �� transform 
	Transform* m_Parrent;

	// �ڽİ� �θ��� ��ġ�� ũ��, �׸��� ȸ�� 
	D2D1_VECTOR_2F m_pos;
	D2D1_VECTOR_2F m_Scale;
	float Angle;

public:
	Transform() : m_Parrent{ nullptr },	m_pos{ 0.0f, 0.0f }, 
				  m_Scale{ 1.0f, 1.0f }, Angle(0.0f)
	{
		
	}

	D2D1::Matrix3x2F GetMatrix() const;

	// ---------------- Setting Method
	void SetPosition(D2D1_VECTOR_2F movement);
	void SetScale(D2D1_VECTOR_2F rotate);
	void SetAngle(float angle);

	// ------------------ Get Method
	D2D1_VECTOR_2F GetPosition() const;
	D2D1_VECTOR_2F GetScale() const;
	float GetAngle() const;

	// ---------------- Settiong Parrent 
	void SetParrent(Transform* parrant);

	// ------------------ Get Parrent 
	Transform* GetParrent() const;


};

