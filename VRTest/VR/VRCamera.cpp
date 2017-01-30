/*************************************************************************//*!
					
					@file	VRCamera.cpp
					@brief	VR�p�J�����N���X�B

															@author	CDW
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"VRCamera.h"

namespace Mof {

/*************************************************************************//*!
		@brief			�R���X�g���N�^
		@param			None

		@return			None
*//**************************************************************************/
CVRCamera::CVRCamera() :
CCamera() ,
m_NowEye(CVRManager::Eye::Default) {
	m_ViewPort.w = CVRManager::GetWidth();
	m_ViewPort.h = CVRManager::GetHeight();
}
/*************************************************************************//*!
		@brief			�R���X�g���N�^
		@param[in]		pName		�J������

		@return			None
*//**************************************************************************/
CVRCamera::CVRCamera(LPCMofChar pName) :
CCamera(pName) ,
m_NowEye(CVRManager::Eye::Default) {
	m_ViewPort.w = CVRManager::GetWidth();
	m_ViewPort.h = CVRManager::GetHeight();
}
/*************************************************************************//*!
		@brief			�R�s�[�R���X�g���N�^
		@param[in]		pObj		�R�s�[�J����

		@return			None
*//**************************************************************************/
CVRCamera::CVRCamera(const CVRCamera& pObj) :
CCamera(pObj) ,
m_NowEye(pObj.m_NowEye) {
}
/*************************************************************************//*!
		@brief			�f�X�g���N�^
		@param			None

		@return			None
*//**************************************************************************/
CVRCamera::~CVRCamera() {
	Release();
}


/*************************************************************************//*!
		@brief			�J�����̍X�V<br>
						�ݒ肳��Ă����񂩂�J�����ɕK�v�Ȋe��s����v�Z���Ȃ���
		@param			None

		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CVRCamera::Update(void) {
	//��{�J�����̃A�b�v�f�[�g
	CCamera::Update();
	//VR�V�X�e�����Ȃ�
	vr::IVRSystem* pHMD = CVRManager::GetDevice();
	if (!pHMD)
	{
		for (MofU32 i = 0; i < CVRManager::Eye::Count; i++)
		{
			m_VRPosition[i] = m_Position;
			m_VRViewMatrix[i] = m_ViewMatrix;
			m_VRBillBoardMatrix[i] = m_BillBoardMatrix;
			m_VRProjectionMatrix[i] = m_ProjectionMatrix;
			m_VRViewProjectionMatrix[i] = m_ViewProjectionMatrix;
		}
		return TRUE;
	}
	

	CMatrix44 matHMD = CVRManager::GetHMDMatrix();	//HMD�̃��[���X�P�[������Έʒu��]���̃}�g���N�X
	CMatrix44 matEyePos;		//���E�ڈʒu�̍s��
	CMatrix44 matHmdView;		//HMD�ʒu��]�~���E�ڍs��
	CMatrix44 matProj;			//�v���W�F�N�V�����s��
	CMatrix44 matVPos;			//�J�����ʒu�s��i�����Œ�j
	CVector3  vVrFront(0,0,1);	//VR��ԓ��̐��ʕ����B�����o�Ŏ����Ă����āA�O���玝���Ă���Ƃ�������
	
	matVPos.LookAtLH(m_Position, m_Position + vVrFront, Vector3(0, 1, 0));
	
	//�J�����ʒu�{HMD�ړ���̈ʒu���v�Z
	matHmdView = matHMD;
	{
		matHmdView._13 *= -1;
		matHmdView._23 *= -1;
		matHmdView._31 *= -1;
		matHmdView._32 *= -1;
		matHmdView._43 *= -1;
	}
	matHmdView = matVPos * matHmdView;
	matHmdView.Inverse();
	matHmdView.GetTranslation(m_VRPosition[CVRManager::Eye::Count]);

	{
		CVector3 vpos, vat(0,0,1);
		vpos *= matHmdView;
		vat *= matHmdView;
		m_ViewMatrix.LookAtLH(vpos, vat, Vector3(0, 1, 0));
	}

	//���E�ڂ��猩���Ƃ��̃r���[�E�v���W�F�N�V�����E�r���{�[�h�}�g���N�X���v�Z
	for (int i = 0; i < CVRManager::Eye::Count; i++)
	{
		//���E�ڈʒu�̍s����擾
		CVRManager::ConvertMatrix44(pHMD->GetEyeToHeadTransform((vr::EVREye)i), matEyePos);
		matEyePos._41 *= CVRManager::GetVRScale();
		matEyePos._42 *= CVRManager::GetVRScale();
		matEyePos._43 *= CVRManager::GetVRScale();
		matEyePos.Inverse();
		matHmdView = matHMD * matEyePos;
		//GL�n�̃r���[�s����ADX�n�̃r���[�s��ɕϊ�	// http://cv4mar.blogspot.jp/2009/03/transformation-matrices-between-opengl.html
		{
			matHmdView._13 *= -1;
			matHmdView._23 *= -1;
			matHmdView._31 *= -1;
			matHmdView._32 *= -1;
			matHmdView._43 *= -1;
			//matHmdView._41 *= -1;

		}
		matHmdView = matVPos * matHmdView;
		m_VRViewMatrix[i] = matHmdView;
		
		//���E�ڈʒu�̃|�W�V�����}�g���N�X�擾
		//matHmdView *= matEyePos;
		matHmdView.Inverse();
		matHmdView.GetTranslation(m_VRPosition[i]);

		//���������x�N�g��
		CVector3 vf(0, 0, 1);
		vf.TransformNormal(matHmdView);
		m_VRFront[i] = vf;

		//�v���W�F�N�V�����s��̐����i�E����W�n�ɒ��Ӂj
		CVRManager::ConvertMatrix44(pHMD->GetProjectionMatrix((vr::EVREye)i, m_Projection.Near, m_Projection.Far), matProj);
		//GL�n�̃v���W�F�N�V�����s����ADX�n�̃v���W�F�N�V�����s��ɕϊ�
		{
			matProj = matProj;
			matProj._31 *= -1;
			matProj._32 *= -1;
			matProj._33 *= -1;
			matProj._34 *= -1;
			/*matProj._13 = (matProj._13 + matProj._14) * 0.5f;
			matProj._23 = (matProj._23 + matProj._24) * 0.5f;
			matProj._33 = (matProj._33 + matProj._34) * 0.5f;
			matProj._43 = (matProj._43 + matProj._44) * 0.5f;//*/
		}
		m_VRProjectionMatrix[i] = matProj;

		//�r���[�~�v���W�F�N�V�����s��
		m_VRViewProjectionMatrix[i] = m_VRViewMatrix[i] * m_VRProjectionMatrix[i];

		//�r���{�[�h�̃}�g���N�X����???
		CMatrix33 vml33(
			m_VRViewMatrix[i]._11, m_VRViewMatrix[i]._12, m_VRViewMatrix[i]._13,
			m_VRViewMatrix[i]._21, m_VRViewMatrix[i]._22, m_VRViewMatrix[i]._23,
			m_VRViewMatrix[i]._31, m_VRViewMatrix[i]._32, m_VRViewMatrix[i]._33);
		vml33.Inverse(m_VRBillBoardMatrix[i]);

	}


	//�������ݒ�J�����̏ꍇ�X�V���s��
	if (CGraphicsUtilities::GetCamera() == this)
	{
		CGraphicsUtilities::SetCamera(this);
	}
	return TRUE;
}
/*************************************************************************//*!
		@brief			�r���[�|�[�g�ݒ�<br>
						��ʃT�C�Y���玩���I�ɐݒ���s���B
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CVRCamera::SetViewPort(void){
	m_ViewPort.x = 0.0f;	m_ViewPort.y = 0.0f;
	m_ViewPort.w = CVRManager::GetWidth();
	m_ViewPort.h = CVRManager::GetHeight();
	m_ViewPort.n = 0.0f;	m_ViewPort.f = 1.0f;
	return TRUE;
}

}
