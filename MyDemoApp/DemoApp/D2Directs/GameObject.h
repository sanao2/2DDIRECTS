#pragma once
#include "pch.h"
#include "transform.h"

class GameObject
{
protected : 
	Transform* m_transform; 

public : 
	virtual ~GameObject() { delete m_transform; }
	Transform* GetTransform() const; 

};

