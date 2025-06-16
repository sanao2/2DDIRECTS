#include "pch.h"
#include "Camera.h"

void Camera::Update()
{
	ProgressInput(); 
}

void Camera::ProgressInput()
{
	D2D1_VECTOR_2F input = {};

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		input.x -= 2.0f;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		input.x += 2.0f;
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		input.y += 2.0f;
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		input.y -= 2.0f;
	}

	m_CameraMatrix = m_transform->GetMatrix();
	m_CameraMatrix.Invert();
}

D2D1::Matrix3x2F Camera::GetMatrix()
{
	return m_CameraMatrix;
}
