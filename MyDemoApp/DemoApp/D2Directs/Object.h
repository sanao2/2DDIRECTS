#pragma once
#include "pch.h"
#include "transform.h"
#include "RenderManager.h"
#include "Image.h"
using namespace std;

class Object
{
protected : 
	Transform* transform; 
public:
	D2D1::Matrix3x2F* m_transform;

	Object(ImageManager* image, RenderManager* renderManager) : m_image(image), m_render(renderManager) 
	{
	}
	virtual ~Object() {
		delete transform;
		delete m_transform; 
		delete m_image;
		delete m_render;
	}

	void Initialize();
	void Update();
	void Render();

	D2D1::Matrix3x2F* Gettransform() { return m_transform; }
	Transform* GetTransformPointer() { return transform; }

private:
	ImageManager* m_image;
	RenderManager* m_render;
	Camera* m_camera;

};
