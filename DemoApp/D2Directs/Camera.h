#pragma once
#include "pch.h"

// 카메라의 위치, 크기, 회전 정보를 transform 에 담아서 카메라의 크기의 중심점을 비추게 한다. 

struct CameraInfo
{
	float cameraAngle; 
	D2D1_POINT_2F CameraScale;
};

class Camera
{
private : 
	// 카메라의 위치 
	D2D1_POINT_2F CameraPos; 
	// 카메라 행렬 변환 
	D2D1_MATRIX_3X2_F transform;
	D2D1_MATRIX_3X2_F CameraMatrix;
	// 카메라 정보 
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

