/*************************************************************************//*!
					
					@file	VRCamera.inl
					@brief	VR用カメラクラス。

															@author	CDW
															@date	2014.05.14
*//**************************************************************************/

/*************************************************************************//*!
		@brief			有効視点切り替え
		@param[in]		eye		視点
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
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
		@brief			カメラ位置取得
		@param			None

		@return			カメラ位置
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
		@brief			カメラ位置取得
		@param			None

		@return			カメラ位置
*//**************************************************************************/
FORCE_INLINE CVector3& CVRCamera::GetCameraPosition(void) {
	return m_Position;
}
/*************************************************************************//*!
		@brief			ビュー行列取得
		@param			None

		@return			ビュー行列
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
		@brief			ビルボード行列取得
		@param			None

		@return			ビルボード行列
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
		@brief			プロジェクション行列取得
		@param			None

		@return			プロジェクション行列
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
		@brief			ビュープロジェクション行列取得
		@param			None

		@return			ビュープロジェクション行列
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
		@brief			視線ベクトル取得
		@param			None

		@return			視線ベクトル
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