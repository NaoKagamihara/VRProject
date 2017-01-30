/*************************************************************************//*!
					
					@file	VRManager.inl
					@brief	VR�֘A�Ǘ��B

															@author	CDW
															@date	2014.05.14
*//**************************************************************************/

/*************************************************************************//*!
		@brief			VR���R���g���[���[�̗L���𔻒肷��
		@param			None
				
		@return			TRUE		����<br>
						FALSE		�Ȃ�
*//**************************************************************************/
FORCE_INLINE MofBool CVRManager::IsLeftController(void) {
	return m_bLeftController;
}
/*************************************************************************//*!
		@brief			VR���R���g���[���[�̃^�b�`�𔻒肷��
		@param			None
				
		@return			TRUE		�^�b�`<br>
						FALSE		�^�b�`�Ȃ�
*//**************************************************************************/
FORCE_INLINE MofBool CVRManager::IsLeftTouch(void) {
	return (m_LeftControllerState.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) ? TRUE : FALSE;
}
/*************************************************************************//*!
		@brief			VR���R���g���[���[�̃{�^�������𔻒肷��
		@param[in]		id			�{�^��ID
				
		@return			TRUE		�^�b�`<br>
						FALSE		�^�b�`�Ȃ�
*//**************************************************************************/
FORCE_INLINE MofBool CVRManager::IsLeftHold(MofU32 id) {
	return (m_LeftControllerState.ulButtonPressed & vr::ButtonMaskFromId((vr::EVRButtonId)id)) ? TRUE : FALSE;
}
/*************************************************************************//*!
		@brief			VR�E�R���g���[���[�̗L���𔻒肷��
		@param			None
				
		@return			TRUE		����<br>
						FALSE		�Ȃ�
*//**************************************************************************/
FORCE_INLINE MofBool CVRManager::IsRightController(void) {
	return m_bRightController;
}
/*************************************************************************//*!
		@brief			VR�E�R���g���[���[�̃^�b�`�𔻒肷��
		@param			None
				
		@return			TRUE		�^�b�`<br>
						FALSE		�^�b�`�Ȃ�
*//**************************************************************************/
FORCE_INLINE MofBool CVRManager::IsRightTouch(void) {
	return (m_RightControllerState.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) ? TRUE : FALSE;
}
/*************************************************************************//*!
		@brief			VR�E�R���g���[���[�̃{�^�������𔻒肷��
		@param[in]		id			�{�^��ID
				
		@return			TRUE		�^�b�`<br>
						FALSE		�^�b�`�Ȃ�
*//**************************************************************************/
FORCE_INLINE MofBool CVRManager::IsRightHold(MofU32 id) {
	return (m_RightControllerState.ulButtonPressed & vr::ButtonMaskFromId((vr::EVRButtonId)id)) ? TRUE : FALSE;
}
/*************************************************************************//*!
		@brief			VR�X�P�[�����擾����
		@param[in]		s			VR�X�P�[��
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
FORCE_INLINE MofBool CVRManager::SetVRScale(MofFloat s){
	m_VRScale = s;
	return TRUE;
}
/*************************************************************************//*!
		@brief			VR�V�X�e�����擾����
		@param			None
				
		@return			VR�V�X�e��
*//**************************************************************************/
FORCE_INLINE vr::IVRSystem* CVRManager::GetDevice(void) {
	return m_pHMD;
}
/*************************************************************************//*!
		@brief			VR�`�惂�f�����擾����
		@param			None
				
		@return			VR�`�惂�f��
*//**************************************************************************/
FORCE_INLINE vr::IVRRenderModels* CVRManager::GetRenderModel(void) {
	return m_pRenderModels;
}
/*************************************************************************//*!
		@brief			VR�������擾����
		@param			None
				
		@return			VR����
*//**************************************************************************/
FORCE_INLINE MofU32 CVRManager::GetWidth(void) {
	return m_Width;
}
/*************************************************************************//*!
		@brief			VR�������擾����
		@param			None
				
		@return			VR����
*//**************************************************************************/
FORCE_INLINE MofU32 CVRManager::GetHeight(void) {
	return m_Height;
}
/*************************************************************************//*!
		@brief			VR�p�̕`��e�N�X�`�����擾
		@param[in]		eye			VR���_
				
		@return			VR�p�̕`��e�N�X�`��
*//**************************************************************************/
FORCE_INLINE LPTexture CVRManager::GetTexture(CVRManager::Eye eye) {
	return m_pRenderTexture[eye];
}
/*************************************************************************//*!
		@brief			VRHMD�s����擾����
		@param			None
				
		@return			VRHMD�s��
*//**************************************************************************/
FORCE_INLINE CMatrix44& CVRManager::GetHMDMatrix(void) {
	return m_HMDPose;
}
/*************************************************************************//*!
		@brief			VR���R���g���[���[�s����擾����
		@param			None
				
		@return			VR���R���g���[���[�s��
*//**************************************************************************/
FORCE_INLINE CMatrix44& CVRManager::GetLeftControllerMatrix(void) {
	return m_LeftControllerPose;
}
/*************************************************************************//*!
		@brief			VR���R���g���[���[�p���x�N�g�����擾����
		@param			None
				
		@return			VR���R���g���[���[�p���x�N�g��
*//**************************************************************************/
FORCE_INLINE CVector3& CVRManager::GetLeftControllerPoseVector(void) {
	return m_LeftControllerVector;
}
/*************************************************************************//*!
		@brief			VR�E�R���g���[���[�s����擾����
		@param			None
				
		@return			VR�E�R���g���[���[�s��
*//**************************************************************************/
FORCE_INLINE CMatrix44& CVRManager::GetRightControllerMatrix(void) {
	return m_RightControllerPose;
}
/*************************************************************************//*!
		@brief			VR�E�R���g���[���[�p���x�N�g�����擾����
		@param			None
				
		@return			VR�E�R���g���[���[�p���x�N�g��
*//**************************************************************************/
FORCE_INLINE CVector3& CVRManager::GetRightControllerPoseVector(void) {
	return m_RightControllerVector;
}

/*************************************************************************//*!
		@brief			VR���R���g���[���[�w�^�b�`���擾����
		@param			None
				
		@return			VR���R���g���[���[�w�^�b�`
*//**************************************************************************/
FORCE_INLINE MofFloat CVRManager::GetLeftTouchHorizontal(void) {
	return m_LeftControllerState.rAxis[0].x;
}
/*************************************************************************//*!
		@brief			VR���R���g���[���[�x�^�b�`���擾����
		@param			None
				
		@return			VR���R���g���[���[�x�^�b�`
*//**************************************************************************/
FORCE_INLINE MofFloat CVRManager::GetLeftTouchVertical(void) {
	return m_LeftControllerState.rAxis[0].y;
}
/*************************************************************************//*!
		@brief			VR�E�R���g���[���[�w�^�b�`���擾����
		@param			None
				
		@return			VR�E�R���g���[���[�w�^�b�`
*//**************************************************************************/
FORCE_INLINE MofFloat CVRManager::GetRightTouchHorizontal(void) {
	return m_RightControllerState.rAxis[0].x;
}
/*************************************************************************//*!
		@brief			VR�E�R���g���[���[�x�^�b�`���擾����
		@param			None
				
		@return			VR�E�R���g���[���[�x�^�b�`
*//**************************************************************************/
FORCE_INLINE MofFloat CVRManager::GetRightTouchVertical(void) {
	return m_RightControllerState.rAxis[0].y;
}
/*************************************************************************//*!
		@brief			VR�X�P�[�����擾����
		@param			None
				
		@return			VR�X�P�[��
*//**************************************************************************/
FORCE_INLINE MofFloat CVRManager::GetVRScale(void){
	return m_VRScale;
}

/*************************************************************************//*!
		@brief			VR�R���g���[���[�^�b�`���擾����
		@param			None
				
		@return			VR�R���g���[���[�^�b�`
*//**************************************************************************/
FORCE_INLINE Vector2 CVRManager::GetTouch(TrackingDevice no)
{
	Vector2 v;
	switch (no)
	{
	case TD_LEFT:	v = *(Vector2*)(&m_LeftControllerState.rAxis[0].x);		break;
	case TD_RIGHT:	v = *(Vector2*)(&m_RightControllerState.rAxis[0].x);	break;
	}
	return v;
}


FORCE_INLINE LPGeometry CVRManager::GetVRModels(TrackingDevice device)
{
	if (!m_Models) return NULL;
	switch (device)
	{
	case TD_HMD:	//HMD
	case TD_LEFT:	//LeftController
	case TD_RIGHT:	//RightController
		return m_Models[device];
	}
	return NULL;
}





		
/*************************************************************************//*!
		@brief			VR���R���g���[���[�g���K�[���擾����
		@param			None
				
		@return			VR���R���g���[���[�g���K�[
*//**************************************************************************/
FORCE_INLINE MofFloat CVRManager::GetTrigger(TrackingDevice no)
{
	switch (no)
	{
	case TD_LEFT:		//LEFT
		return m_LeftControllerState.rAxis[1].x;

	case TD_RIGHT:		//Right
		return m_RightControllerState.rAxis[1].x;
	}
	return 0;
}

//[EOF]