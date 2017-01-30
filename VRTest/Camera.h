///////////////////////////////////////////////
//ÉJÉÅÉâ

#pragma once

#include			"Mof.h"

#include	"VR/VRManager.h"
#include	"VR/VRCamera.h"

class TestCamera
{
private:
	CVRCamera		m_Camera;
	CVector3		m_Angle;
	CVector3		m_Pos;

public:

	TestCamera();
	~TestCamera();

	void Initialize();
	void Update();
	void Render();

	CVector3 getCameraPos(){ return m_Pos; };
	CVector3 getAngle(){ return m_Angle; };
};