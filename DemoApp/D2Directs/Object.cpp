#include "pch.h"
#include "Object.h"

void Object::BeginDrawandClear()
{
	ObjTarget->BeginDraw();
	ObjTarget->Clear();
}

void Object::Initialize()
{
	if (!ObjTarget) {
		MessageBox(nullptr, L"ObjTarget가 설정되지 않았습니다.", L"오류", MB_OK);
		return;
	}

	ObjTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::SkyBlue),
		&FillBrush); 

	//Objrect = { transforms.Pos.x, transforms.Pos.y,  transforms.Pos.x + transforms.Scale.x,  transforms.Pos.y + transforms.Scale.y };
}

void Object::DrawObject()
{
	ObjTarget->DrawRectangle(Objrect, FillBrush.Get(), 2.0f);
}

void Object::UpdateTransformMatrix()
{
	D2D1_POINT_2F center = {
	   transforms.Pos.x + transforms.Scale.x / 2.0f,
	   transforms.Pos.y + transforms.Scale.y / 2.0f
	};

	// 행렬 계산 순서: 스케일 → 회전 → 이동
	transform = D2D1::Matrix3x2F::Scale(transforms.Scale.x, transforms.Scale.y, center) *
				D2D1::Matrix3x2F::Rotation(transforms.rotateX, center) *
				D2D1::Matrix3x2F::Translation(transforms.Pos.x, transforms.Pos.y);
}

void Object::EndDraw()
{
	ObjTarget->EndDraw(); 
}

void Object::ObjectRender()
{
	BeginDrawandClear();
	DrawObject(); 
	EndDraw(); 
}

void Object::UnInitialize()
{
	ObjTarget = nullptr;
	FillBrush = nullptr;
}

void Object::SetRenderTarget(ComPtr<ID2D1RenderTarget> target)
{
	ObjTarget = target; 
}

void Object::SetObjPosition(float x, float y)
{
	transforms.Pos.x = x;
	transforms.Pos.y = y;
}

void Object::SetObjRotation(float rx)
{
	transforms.rotateX = rx;
	
}

void Object::SetObjScale(float sx, float sy)
{
	transforms.Scale.x = sx;
	transforms.Scale.y = sy;
}
