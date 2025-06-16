#pragma once
#include "pch.h"
#include "GameObject.h"

class Camera : public GameObject
{
private : 
	D2D1::Matrix3x2F m_CameraMatrix; 
	
	void Update(); 
	void ProgressInput();

public : 
	D2D1::Matrix3x2F GetMatrix();
};

