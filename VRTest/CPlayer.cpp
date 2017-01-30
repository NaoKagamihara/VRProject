
//////////////////////////////////
//Player�N���X

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
	//����ƉE����쐬
	PmPhysxCreateTool* tool = createPhysxCreateTool();
	PxMaterial* mate = tool->createMaterial(CVector3(1, 1, 1));

	//�E��쐬
	m_pRightHand = new CHand(createSphereObject(CVector3(0, 10, 0), mate, 0.5, false), EPmObjectTag::OBJ_RIGHTHAND);
	m_pRightHand->load();
	//m_pRightHand = new CHand(createBoxObject(CVector3(0.0f, 0.0f, 0.0f), mate, 0.2, 0.5f, 0.5f, false));

	//����쐬
	m_pLeftHand = new CHand(createSphereObject(CVector3(0, 10, 0), mate, 0.5, false), EPmObjectTag::OBJ_LEFTHAND);
	m_pLeftHand->load();

	delete tool;
}

void CPlayer::Initialize()
{
	//VR�g�p���J����
	//m_pCamera->initialize();
	//VR�R���g���[���̃��f���ǂݍ���
	//CVRManager::LoadRenderModels();
}

void CPlayer::Update()
{
	//VR�p�̎�̍X�V
	//handUpdate();

	//�L�[�{�[�h�����̍X�V
	debugUpdate();

	//VR�J�����̍X�V
	//m_pCamera->update(m_Pos);

	//���S���W�̈ړ�
	//��̎p�����`��
	CVector3 rot, v(0, 0, 1);
	rot = m_pRightHand->getHandColl()->getGlobalVecRot();
	v.RotationZ(rot.z);
	v.RotationY(rot.y);
	v.RotationX(rot.x);

	//������
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

	//��̎p�����`��
	CVector3 rot, v(0, 0, 1);
	rot = m_pRightHand->getHandColl()->getGlobalVecRot();
	v.RotationZ(rot.z);
	v.RotationY(rot.y);
	v.RotationX(rot.x);
	CGraphicsUtilities::RenderLine(m_pRightHand->getHandColl()->getGlobalPos(), m_pRightHand->getHandColl()->getGlobalPos() + v * 2, MOF_XRGB(0, 1, 0));

	//�u�q�̎w���Ă���ꏊ�ɔ������ƎG�ȐU���e�X�g
	////������
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
	//VR�̃R���g���[����������擾

	//�E��
	//���W
	CMatrix44 rMat = CVRManager::GetTrackingMatrix(CVRManager::TD_RIGHT);
	rMat.GetTranslation(m_rHandData.pos);
	m_rHandData.pos += m_Pos;

	//��]
	rMat.Transpose();
	rMat.GetRotation(m_rHandData.handRot);

	//�g���K�[
	if (CVRManager::IsRightHold(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) m_rHandData.grasp = true;
	else  m_rHandData.grasp = false;
	//�{�^��
	if (CVRManager::IsRightHold(vr::EVRButtonId::k_EButton_SteamVR_Touchpad)) m_rHandData.button = true;
	else  m_rHandData.button = false;

	//�R���g���[���[�̎p��
	//CVector3 rPose = CVRManager::GetRightControllerPoseVector();
	//rMat.GetRotation(m_rHandData.poseVec);
	//m_rHandData.poseVec = rPose;

	//�X�V
	m_pRightHand->update(m_rHandData);

	//����
	//���W
	CMatrix44 lMat = CVRManager::GetTrackingMatrix(CVRManager::TD_LEFT);
	lMat.GetTranslation(m_lHandData.pos);
	m_lHandData.pos += m_Pos;
	//��]
	lMat.Transpose();
	lMat.GetRotation(m_lHandData.handRot);

	//�g���K�[
	if (CVRManager::IsLeftHold(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) m_lHandData.grasp = true;
	else  m_lHandData.grasp = false;
	//�{�^��
	if (CVRManager::IsLeftHold(vr::EVRButtonId::k_EButton_SteamVR_Touchpad)) m_lHandData.button = true;
	else  m_lHandData.button = false;

	////�R���g���[���[�̎p��
	//CVector3 lPose = CVRManager::GetLeftControllerPoseVector();
	//m_lHandData.poseVec = lPose;

	//�X�V
	m_pLeftHand->update(m_lHandData);
}

void CPlayer::debugUpdate()
{
	////////////////////////////////////////////
	//�L�[�{�[�h���͂ōX�V
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

	//�g���K�[�E
	if (g_pInput->IsKeyHold(MOFKEY_X)) m_rHandData.grasp = true;
	else  m_rHandData.grasp = false;

	//�g���K�[��
	if (g_pInput->IsKeyHold(MOFKEY_Z)) m_lHandData.grasp = true;
	else  m_lHandData.grasp = false;

	m_pRightHand->update(m_rHandData);
	m_pLeftHand->update(m_lHandData);
}