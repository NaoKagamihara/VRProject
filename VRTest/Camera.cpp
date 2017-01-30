 //////////////////////////////
//カメラ定義

#include			"Camera.h"

#define				CAMERA_DISTANCE	(1.0f)

TestCamera::TestCamera()
{

}

TestCamera::~TestCamera()
{

}

void TestCamera::Initialize()
{
	m_Pos.x = 0;
	m_Pos.y = 3;
	m_Pos.z = 3;

	//カメラ初期化
	m_Camera.SetViewPort();
	m_Camera.LookAt(Vector3(0.0f, 0.0f, -2.0f), Vector3(0, 0, 0), Vector3(0, 1, 0));
	m_Camera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
	m_Camera.Update();
	CGraphicsUtilities::SetCamera(&m_Camera);
}

void TestCamera::Update()
{

	CVector3 vRot;
	if (g_pInput->IsMouseKeyHold(MOFMOUSE_RBUTTON))
	{
		g_pInput->GetMouseMove(vRot);
		vRot *= 0.005f;
	}

	m_Angle += Vector3(vRot.y, vRot.x, 0);

	CMatrix33 matRot;
	matRot.RotationZXY(m_Angle);
	CVector3 vFront(0, 0, 1);
	vFront.TransformCoord(matRot, vFront);

	//キーによるオブジェクト移動
	float speed = 1.0f;
	CVector3 vMove;

	if (g_pInput->IsKeyHold(MOFKEY_SPACE))
	{
		vMove.y += speed;
	}
	if (g_pInput->IsKeyHold(MOFKEY_LSHIFT))
	{
		vMove.y -= speed;
	}


	if (g_pInput->IsKeyHold(MOFKEY_A))
	{
		vMove.x -= speed;
	}
	if (g_pInput->IsKeyHold(MOFKEY_D))
	{
		vMove.x += speed;
	}
	if (g_pInput->IsKeyHold(MOFKEY_S))
	{
		vMove.z -= speed;
	}
	if (g_pInput->IsKeyHold(MOFKEY_W))
	{
		vMove.z += speed;
	}
	vMove.RotationY(m_Angle.y);
	m_Pos += vMove;

	//カメラ更新
	m_Camera.LookAt(m_Pos, m_Pos + vFront * CAMERA_DISTANCE, Vector3(0, 1, 0));
	m_Camera.Update();
}