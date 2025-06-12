#include "pch.h"
#include "Camera.h"

void Camera::Initialize()
{
	
	// ī�޶���� ����� ���� 
	D2D1_MATRIX_3X2_F CamTransform =
		D2D1::Matrix3x2F::Translation(-CameraPos.x, -CameraPos.y) * 
		D2D1::Matrix3x2F::Rotation(info.cameraAngle) *
		D2D1::Matrix3x2F::Scale(info.CameraScale.x, info.CameraScale.y) *
		D2D1::Matrix3x2F::Translation(CameraPos.x, CameraPos.y); 
	
	D2D1_MATRIX_3X2_F viewMatrix = CamTransform;
	if (!D2D1InvertMatrix(&viewMatrix)) {
		MessageBox(nullptr, L"ī�޶� ����� ����", L"Error", MB_OK);
	}

	CameraMatrix = viewMatrix;
}

