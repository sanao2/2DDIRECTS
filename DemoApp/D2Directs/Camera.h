#pragma once
#include "pch.h"

// ī�޶��� ��ġ, ũ��, ȸ�� ������ transform �� ��Ƽ� ī�޶��� ũ���� �߽����� ���߰� �Ѵ�. 

struct CameraInfo
{
	float cameraAngle; 
	D2D1_POINT_2F CameraScale;
};

class Camera
{
private : 
	// ī�޶��� ��ġ 
	D2D1_POINT_2F CameraPos; 
	// ī�޶� ��� ��ȯ 
	D2D1_MATRIX_3X2_F transform;
	D2D1_MATRIX_3X2_F CameraMatrix;
	// ī�޶� ���� 
	CameraInfo info; 

public : 
	Camera(D2D1_POINT_2F camerapos, D2D1_POINT_2F camerascale, float cameraAngle)
		: transform(),
		  CameraPos{ camerapos },
		  info{ cameraAngle, camerascale }
	{ }

	void Initialize(); 
	D2D1_MATRIX_3X2_F GetCameraMatrix() { return CameraMatrix;  }
};

