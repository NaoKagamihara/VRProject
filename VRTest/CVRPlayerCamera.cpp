/////////////////
//CVRPlayerCamera

#include	"CVRPlayerCamerah.h"

CVRPlayerCamera::CVRPlayerCamera(CVRCamera*	camera) : m_pCamera(NULL)
{
	m_pCamera = camera;
}

CVRPlayerCamera::~CVRPlayerCamera()
{

}

void CVRPlayerCamera::initialize()
{
	//ƒJƒƒ‰‰Šú‰»
	m_pCamera->SetViewPort();
	m_pCamera->LookAt(Vector3(-2.0f, 2.0f, -2.0f), Vector3(0, 0, 0), Vector3(0, 1, 0));
	m_pCamera->PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
	m_pCamera->Update();
	CGraphicsUtilities::SetCamera(m_pCamera);
}

void CVRPlayerCamera::update(CVector3 wPos)
{
	CMatrix44 mat = CVRManager::GetHMDMatrix();
	
	mat.Inverse();
	CMatrix33 mirror;
	mirror._33 = -1;

	CMatrix33 rot;
	mat.GetRotation(rot);
	rot *= mirror;

	CVector3 vFront(0, 0, -1);
	vFront.TransformCoord(rot, vFront);

	CVector3 pos(wPos.x, wPos.y, wPos.z);
	//pos.TransformCoord(mat, pos);
	//pos *= mirror;

	m_pCamera->LookAt(pos, pos + vFront, Vector3(0, 1, 0));
	m_pCamera->Update();
}