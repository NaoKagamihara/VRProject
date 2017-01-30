
//////////////////////////////////
//Playerクラス

#include	"CPlayer.h"

CPlayer::CPlayer(CVRCamera* camera) : m_pRightHand(NULL), m_pLeftHand(NULL),
m_pCamera(NULL)
{
	m_pCamera = new CVRPlayerCamera(camera);
	//m_rHandData.pos = CVector3(0, 5, 0);
}

CPlayer::~CPlayer()
{

}

void CPlayer::Create()
{
	//左手と右手を作成
	PmPhysxCreateTool* tool = createPhysxCreateTool();
	PxMaterial* mate = tool->createMaterial(CVector3(1, 1, 1));

	//右手作成
	m_pRightHand = new CHand(createSphereObject(CVector3(0, 10, 0), mate, 0.5, false), EPmObjectTag::OBJ_RIGHTHAND);
	m_pRightHand->load();
	//m_pRightHand = new CHand(createBoxObject(CVector3(0.0f, 0.0f, 0.0f), mate, 0.2, 0.5f, 0.5f, false));

	//左手作成
	m_pLeftHand = new CHand(createSphereObject(CVector3(0, 10, 0), mate, 0.5, false), EPmObjectTag::OBJ_LEFTHAND);
	m_pLeftHand->load();

	delete tool;
}

void CPlayer::Initialize()
{
	//VR使用時カメラ
	//m_pCamera->initialize();
	//VRコントローラのモデル読み込み
	//CVRManager::LoadRenderModels();
}

void CPlayer::Update()
{
	//VR用の手の更新
	//handUpdate();

	//キーボードから手の更新
	debugUpdate();

	//VRカメラの更新
	//m_pCamera->update(m_Pos);

	//中心座標の移動
	//手の姿勢線描画
	CVector3 rot, v(0, 0, 1);
	rot = m_pRightHand->getHandColl()->getGlobalVecRot();
	v.RotationZ(rot.z);
	v.RotationY(rot.y);
	v.RotationX(rot.x);

	//白い箱
	CVector3 vec = m_pRightHand->getHandColl()->getGlobalPos() + m_Pos;
	vec.y /= v.y;
	CVector3 test = v * abs(vec.y);

	test += m_pRightHand->getHandColl()->getGlobalPos();
	if (test.y == 0)
	{
		if (!m_pRightHand->getHandColl()->checkObjectTag(EPmObjectTag::OBJ_HANDGRASP))
			if (CVRManager::IsRightHold(vr::EVRButtonId::k_EButton_ApplicationMenu) && m_MoveCount >= 100)
			{
				m_MoveCount = 0;
				m_Pos = test;
			}
	}

	m_MoveCount++;
}
static int num = 0;
void CPlayer::RearUpdate()
{
	
	if (g_pInput->IsKeyPush(MOFKEY_0))
	{
		num += 100;
	}
	if (g_pInput->IsKeyPush(MOFKEY_9))
	{
		num -= 100;
	}
}

void CPlayer::Render()
{
	m_pLeftHand->render();
	m_pRightHand->render();
}

void CPlayer::RearRender()
{

}

void CPlayer::DebugRender()
{
	m_pLeftHand->debugRender();
	m_pRightHand->debugRender();

	//手の姿勢線描画
	CVector3 rot, v(0, 0, 1);
	rot = m_pRightHand->getHandColl()->getGlobalVecRot();
	v.RotationZ(rot.z);
	v.RotationY(rot.y);
	v.RotationX(rot.x);
	CGraphicsUtilities::RenderLine(m_pRightHand->getHandColl()->getGlobalPos(), m_pRightHand->getHandColl()->getGlobalPos() + v * 2, MOF_XRGB(0, 1, 0));

	//ＶＲの指している場所に白い箱と雑な振動テスト
	////白い箱
	//CVector3 vec = m_pRightHand->getHandColl()->getGlobalPos();
	//vec.y /= v.y;
	//CVector3 test = v * abs(vec.y);

	//test += m_pRightHand->getHandColl()->getGlobalPos();
	//if (test.y == 0)
	//{
	//	if (!m_pRightHand->getHandColl()->checkObjectTag(EPmObjectTag::OBJ_HANDGRASP))
	//		if (CVRManager::IsRightHold(vr::EVRButtonId::k_EButton_ApplicationMenu) && m_MoveCount >= 100)
	//		{
	//			m_MoveCount = 0;
	//			//m_Pos = test;
	//			CVRManager::GetDevice()->TriggerHapticPulse(0, 1, 1000);
	//			CVRManager::GetDevice()->TriggerHapticPulse(1, 1, 1000);
	//		}
	//	CMatrix44 mat;
	//	mat.SetTranslation(test);
	//	CGraphicsUtilities::RenderBox(mat);
	//}

	//vr::IVRSystem* tes = CVRManager::GetDevice();
	//tes->TriggerHapticPulse(4, 0, num);

	//CGraphicsUtilities::RenderString(10, 100, "%d", num);
}

void CPlayer::Release()
{
	MOF_SAFE_DELETE(m_pRightHand);
	MOF_SAFE_DELETE(m_pLeftHand);
	MOF_SAFE_DELETE(m_pCamera);
}

void CPlayer::handUpdate()
{

	////////////////////////////////////////////////
	//VRのコントローラから情報を取得

	//右手
	//座標
	CMatrix44 rMat = CVRManager::GetTrackingMatrix(CVRManager::TD_RIGHT);
	rMat.GetTranslation(m_rHandData.pos);
	m_rHandData.pos += m_Pos;

	//回転
	rMat.Transpose();
	rMat.GetRotation(m_rHandData.handRot);

	//トリガー
	if (CVRManager::IsRightHold(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) m_rHandData.grasp = true;
	else  m_rHandData.grasp = false;
	//ボタン
	if (CVRManager::IsRightHold(vr::EVRButtonId::k_EButton_SteamVR_Touchpad)) m_rHandData.button = true;
	else  m_rHandData.button = false;

	//コントローラーの姿勢
	//CVector3 rPose = CVRManager::GetRightControllerPoseVector();
	//rMat.GetRotation(m_rHandData.poseVec);
	//m_rHandData.poseVec = rPose;

	//更新
	m_pRightHand->update(m_rHandData);

	//左手
	//座標
	CMatrix44 lMat = CVRManager::GetTrackingMatrix(CVRManager::TD_LEFT);
	lMat.GetTranslation(m_lHandData.pos);
	m_lHandData.pos += m_Pos;
	//回転
	lMat.Transpose();
	lMat.GetRotation(m_lHandData.handRot);

	//トリガー
	if (CVRManager::IsLeftHold(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) m_lHandData.grasp = true;
	else  m_lHandData.grasp = false;
	//ボタン
	if (CVRManager::IsLeftHold(vr::EVRButtonId::k_EButton_SteamVR_Touchpad)) m_lHandData.button = true;
	else  m_lHandData.button = false;

	////コントローラーの姿勢
	//CVector3 lPose = CVRManager::GetLeftControllerPoseVector();
	//m_lHandData.poseVec = lPose;

	//更新
	m_pLeftHand->update(m_lHandData);
}

void CPlayer::debugUpdate()
{
	////////////////////////////////////////////
	//キーボード入力で更新
	if (g_pInput->IsKeyHold(MOFKEY_UP))
	{
		m_rHandData.pos.y += 0.1f;
	}
	if (g_pInput->IsKeyHold(MOFKEY_DOWN))
	{
		m_rHandData.pos.y -= 0.1f;
	}
	if (g_pInput->IsKeyHold(MOFKEY_LEFT))
	{
		m_rHandData.pos.x -= 0.1f;
	}
	if (g_pInput->IsKeyHold(MOFKEY_RIGHT))
	{
		m_rHandData.pos.x += 0.1f;
	}
	if (g_pInput->IsKeyHold(MOFKEY_O))
	{
		m_rHandData.pos.z += 0.1f;
	}
	if (g_pInput->IsKeyHold(MOFKEY_L))
	{
		m_rHandData.pos.z -= 0.1f;
	}	if (g_pInput->IsKeyHold(MOFKEY_T))
	{
		m_rHandData.pos.y += 0.1f;
	}
	if (g_pInput->IsKeyHold(MOFKEY_G))
	{
		m_rHandData.pos.y -= 0.1f;
	}
	if (g_pInput->IsKeyHold(MOFKEY_F))
	{
		m_rHandData.pos.x -= 0.1f;
	}
	if (g_pInput->IsKeyHold(MOFKEY_H))
	{
		m_rHandData.pos.x += 0.1f;
	}
	if (g_pInput->IsKeyHold(MOFKEY_Y))
	{
		m_rHandData.pos.z += 0.1f;
	}
	if (g_pInput->IsKeyHold(MOFKEY_U))
	{
		m_rHandData.pos.z -= 0.1f;
	}

	//トリガー右
	if (g_pInput->IsKeyHold(MOFKEY_X)) m_rHandData.grasp = true;
	else  m_rHandData.grasp = false;

	//トリガー左
	if (g_pInput->IsKeyHold(MOFKEY_Z)) m_lHandData.grasp = true;
	else  m_lHandData.grasp = false;

	m_pRightHand->update(m_rHandData);
	m_pLeftHand->update(m_lHandData);
}