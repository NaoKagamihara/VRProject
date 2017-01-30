/////////////////
//CVRPlayerCamera

#pragma once

#include	"VR/VRManager.h"
#include	"VR/VRCamera.h"

class CVRPlayerCamera
{
private:
	CVRCamera*	m_pCamera;
	CVector3	m_Angle;

public:

	CVRPlayerCamera(CVRCamera*	camera);
	~CVRPlayerCamera();

	void initialize();
	void update(CVector3 wPos);
};