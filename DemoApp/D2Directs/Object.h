#pragma once
#include <array> // Include for std::array
using namespace std;

// �÷��̾�, ���� ���� ��ǥ, ������, �����̼� ���� ���� ���� 
// �Ǵ�, ��Ʈ���� �����ų ������Ʈ�� ���� 

// �ʿ��� ���� : RECT, Rotatioon, Scale(float), dx,dy 

// ��� ���� ������ �迭�� �����ؼ�, �� �迭�� ��� ������ �ҷ��ͼ� 
// transform  �Ǵ� ��ȯ ��Ŀ� ����� �� ������ ���� ������? 
// size ���� ���� ������ �ʿ��ϴ�. 

struct Objtransform
{
	// �̵��� ����� int Ÿ��
	D2D1_VECTOR_2F Pos; 

	// �������� ����� float Ÿ�� 
	D2D1_VECTOR_2F Scale; 

	// ȸ���� ����� float Ÿ�� 
	float rotateX;
};

class Object
{
private:
	// ������Ʈ�� �׸� ���� 
	ComPtr<ID2D1SolidColorBrush> FillBrush; 


public:
	D2D1_RECT_F Objrect;
	// ������Ʈ�� ��ǥ 	// ������Ʈ�� ũ�� 	// ������Ʈ�� ȸ�� ũ�� 
	Objtransform transforms;
	D2D1_MATRIX_3X2_F transform;
	ComPtr<ID2D1RenderTarget> ObjTarget;

	Object(D2D1_POINT_2F Pos, D2D1_POINT_2F Scale, float Rotate) {
		Objrect = { 0,0,0,0 };
		transforms = { Pos.x, Pos.y, Scale.x, Scale.y, Rotate};
	}

	~Object() {	
		UnInitialize();
	}

	// ������Ʈ�� �׸��� �Լ� 
	void BeginDrawandClear(); 
	void Initialize(); 
	void DrawObject();
	void UpdateTransformMatrix();
	void EndDraw(); 
	void ObjectRender();
	void UnInitialize(); 

	void SetRenderTarget(ComPtr<ID2D1RenderTarget> target); 
	void SetObjPosition(float x, float y);
	void SetObjRotation(float rx); 
	void SetObjScale(float sx, float sy); 

};
