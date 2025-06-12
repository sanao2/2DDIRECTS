#pragma once
#include <array> // Include for std::array
using namespace std;

// 플레이어, 몬스터 등의 좌표, 스케일, 로테이션 값을 가질 파일 
// 또는, 비트맵을 적용시킬 오브젝트를 생성 

// 필요한 변수 : RECT, Rotatioon, Scale(float), dx,dy 

// 행렬 값을 저장할 배열을 선언해서, 그 배열에 담긴 값들을 불러와서 
// transform  또는 변환 행렬에 사용할 수 있으면 되지 않을까? 
// size 값은 따로 선언이 필요하다. 

struct Objtransform
{
	// 이동을 담당할 int 타입
	D2D1_VECTOR_2F Pos; 

	// 스케일을 담당할 float 타입 
	D2D1_VECTOR_2F Scale; 

	// 회전을 담당할 float 타입 
	float rotateX;
};

class Object
{
private:
	// 오브젝트를 그릴 도형 
	ComPtr<ID2D1SolidColorBrush> FillBrush; 


public:
	D2D1_RECT_F Objrect;
	// 오브젝트의 좌표 	// 오브젝트의 크기 	// 오브젝트의 회전 크기 
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

	// 오브젝트를 그리는 함수 
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
