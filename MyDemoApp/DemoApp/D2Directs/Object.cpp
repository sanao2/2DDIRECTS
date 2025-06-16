#include "pch.h"
#include "Object.h"



void Object::Initialize()
{
	if (m_image == nullptr || m_render == nullptr)
	{
		cout << "초기화 되지 않았습니다." << endl; 
		return; 
	}

	m_image = new ImageManager();
	m_render = new RenderManager();
	m_camera = new Camera();
}


void Object::Update()
{

}

void Object::Render()
{
	D2D1_SIZE_F size = m_image->GetBitmap()->GetSize();

	// 이미지의 중심점을 중심으로 위치 설정 
	D2D1::Matrix3x2F renderMatrix = D2D1::Matrix3x2F::Scale(1.0f, 1.0f) *
		D2D1::Matrix3x2F::Translation(size.width / 2, size.height / 2); 

	D2D1::Matrix3x2F finalMatrix = renderMatrix * transform->GetMatrix() * m_camera->GetMatrix() *m_render->GetMatrix();
	ComPtr<ID2D1Bitmap1> bitmap = m_image->GetBitmap(); 

	m_render->DrawBitmapTransform(bitmap, finalMatrix);
}



