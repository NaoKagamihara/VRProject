/*************************************************************************//*!
					
					@file	VRCamera.inl
					@brief	VR�p�J�����N���X�B

															@author	CDW
															@date	2014.05.14
*//**************************************************************************/

/*************************************************************************//*!
		@brief			�L�����_�؂�ւ�
		@param[in]		eye		���_
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
FORCE_INLINE MofBool CVRCamera::SetNowEye(CVRManager::Eye eye) {
	m_NowEye = eye;
	if (CGraphicsUtilities::GetCamera() == this)
	{
		CGraphicsUtilities::SetCamera(this);
	}
	return TRUE;
}
/*************************************************************************//*!
		@brief			�J�����ʒu�擾
		@param			None

		@return			�J�����ʒu
*//**************************************************************************/
FORCE_INLINE CVector3& CVRCamera::GetViewPosition(void) {
	switch (m_NowEye)
	{
		case CVRManager::Left:
		case CVRManager::Right:
		case CVRManager::Default:
			return m_VRPosition[m_NowEye];
	}
	return m_Position;
}
/*************************************************************************//*!
		@brief			�J�����ʒu�擾
		@param			None

		@return			�J�����ʒu
*//**************************************************************************/
FORCE_INLINE CVector3& CVRCamera::GetCameraPosition(void) {
	return m_Position;
}
/*************************************************************************//*!
		@brief			�r���[�s��擾
		@param			None

		@return			�r���[�s��
*//**************************************************************************/
FORCE_INLINE CMatrix44& CVRCamera::GetViewMatrix(void) {
	switch (m_NowEye)
	{
	case CVRManager::Left:
	case CVRManager::Right:
		return m_VRViewMatrix[m_NowEye];
	}
	return m_ViewMatrix;
}
/*************************************************************************//*!
		@brief			�r���{�[�h�s��擾
		@param			None

		@return			�r���{�[�h�s��
*//**************************************************************************/
FORCE_INLINE CMatrix33& CVRCamera::GetBillBoardMatrix(void) {
	switch (m_NowEye)
	{
	case CVRManager::Left:
	case CVRManager::Right:
		return m_VRBillBoardMatrix[m_NowEye];
	}
	return m_BillBoardMatrix;
}
/*************************************************************************//*!
		@brief			�v���W�F�N�V�����s��擾
		@param			None

		@return			�v���W�F�N�V�����s��
*//**************************************************************************/
FORCE_INLINE CMatrix44& CVRCamera::GetProjectionMatrix(void) {
	switch (m_NowEye)
	{
	case CVRManager::Left:
	case CVRManager::Right:
		return m_VRProjectionMatrix[m_NowEye];
	}
	return m_ProjectionMatrix;
}
/*************************************************************************//*!
		@brief			�r���[�v���W�F�N�V�����s��擾
		@param			None

		@return			�r���[�v���W�F�N�V�����s��
*//**************************************************************************/
FORCE_INLINE CMatrix44& CVRCamera::GetViewProjectionMatrix(void) {
	switch (m_NowEye)
	{
	case CVRManager::Left:
	case CVRManager::Right:
		return m_VRViewProjectionMatrix[m_NowEye];
	}
	return m_ViewProjectionMatrix;
}

/*************************************************************************//*!
		@brief			�����x�N�g���擾
		@param			None

		@return			�����x�N�g��
*//**************************************************************************/
FORCE_INLINE CVector3& CVRCamera::GetViewFront(void)
{
	switch (m_NowEye)
	{
	case CVRManager::Left:
	case CVRManager::Right:
		return m_VRFront[m_NowEye];
	}
	return m_ViewFront;
}


//[EOF]