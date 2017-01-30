/*************************************************************************//*!
					
					@file	VRManager.inl
					@brief	VR関連管理。

															@author	CDW
															@date	2014.05.14
*//**************************************************************************/

/*************************************************************************//*!
		@brief			VR左コントローラーの有無を判定する
		@param			None
				
		@return			TRUE		あり<br>
						FALSE		なし
*//**************************************************************************/
FORCE_INLINE MofBool CVRManager::IsLeftController(void) {
	return m_bLeftController;
}
/*************************************************************************//*!
		@brief			VR左コントローラーのタッチを判定する
		@param			None
				
		@return			TRUE		タッチ<br>
						FALSE		タッチなし
*//**************************************************************************/
FORCE_INLINE MofBool CVRManager::IsLeftTouch(void) {
	return (m_LeftControllerState.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) ? TRUE : FALSE;
}
/*************************************************************************//*!
		@brief			VR左コントローラーのボタン押下を判定する
		@param[in]		id			ボタンID
				
		@return			TRUE		タッチ<br>
						FALSE		タッチなし
*//**************************************************************************/
FORCE_INLINE MofBool CVRManager::IsLeftHold(MofU32 id) {
	return (m_LeftControllerState.ulButtonPressed & vr::ButtonMaskFromId((vr::EVRButtonId)id)) ? TRUE : FALSE;
}
/*************************************************************************//*!
		@brief			VR右コントローラーの有無を判定する
		@param			None
				
		@return			TRUE		あり<br>
						FALSE		なし
*//**************************************************************************/
FORCE_INLINE MofBool CVRManager::IsRightController(void) {
	return m_bRightController;
}
/*************************************************************************//*!
		@brief			VR右コントローラーのタッチを判定する
		@param			None
				
		@return			TRUE		タッチ<br>
						FALSE		タッチなし
*//**************************************************************************/
FORCE_INLINE MofBool CVRManager::IsRightTouch(void) {
	return (m_RightControllerState.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) ? TRUE : FALSE;
}
/*************************************************************************//*!
		@brief			VR右コントローラーのボタン押下を判定する
		@param[in]		id			ボタンID
				
		@return			TRUE		タッチ<br>
						FALSE		タッチなし
*//**************************************************************************/
FORCE_INLINE MofBool CVRManager::IsRightHold(MofU32 id) {
	return (m_RightControllerState.ulButtonPressed & vr::ButtonMaskFromId((vr::EVRButtonId)id)) ? TRUE : FALSE;
}
/*************************************************************************//*!
		@brief			VRスケールを取得する
		@param[in]		s			VRスケール
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
FORCE_INLINE MofBool CVRManager::SetVRScale(MofFloat s){
	m_VRScale = s;
	return TRUE;
}
/*************************************************************************//*!
		@brief			VRシステムを取得する
		@param			None
				
		@return			VRシステム
*//**************************************************************************/
FORCE_INLINE vr::IVRSystem* CVRManager::GetDevice(void) {
	return m_pHMD;
}
/*************************************************************************//*!
		@brief			VR描画モデルを取得する
		@param			None
				
		@return			VR描画モデル
*//**************************************************************************/
FORCE_INLINE vr::IVRRenderModels* CVRManager::GetRenderModel(void) {
	return m_pRenderModels;
}
/*************************************************************************//*!
		@brief			VR高さを取得する
		@param			None
				
		@return			VR高さ
*//**************************************************************************/
FORCE_INLINE MofU32 CVRManager::GetWidth(void) {
	return m_Width;
}
/*************************************************************************//*!
		@brief			VR高さを取得する
		@param			None
				
		@return			VR高さ
*//**************************************************************************/
FORCE_INLINE MofU32 CVRManager::GetHeight(void) {
	return m_Height;
}
/*************************************************************************//*!
		@brief			VR用の描画テクスチャを取得
		@param[in]		eye			VR視点
				
		@return			VR用の描画テクスチャ
*//**************************************************************************/
FORCE_INLINE LPTexture CVRManager::GetTexture(CVRManager::Eye eye) {
	return m_pRenderTexture[eye];
}
/*************************************************************************//*!
		@brief			VRHMD行列を取得する
		@param			None
				
		@return			VRHMD行列
*//**************************************************************************/
FORCE_INLINE CMatrix44& CVRManager::GetHMDMatrix(void) {
	return m_HMDPose;
}
/*************************************************************************//*!
		@brief			VR左コントローラー行列を取得する
		@param			None
				
		@return			VR左コントローラー行列
*//**************************************************************************/
FORCE_INLINE CMatrix44& CVRManager::GetLeftControllerMatrix(void) {
	return m_LeftControllerPose;
}
/*************************************************************************//*!
		@brief			VR左コントローラー姿勢ベクトルを取得する
		@param			None
				
		@return			VR左コントローラー姿勢ベクトル
*//**************************************************************************/
FORCE_INLINE CVector3& CVRManager::GetLeftControllerPoseVector(void) {
	return m_LeftControllerVector;
}
/*************************************************************************//*!
		@brief			VR右コントローラー行列を取得する
		@param			None
				
		@return			VR右コントローラー行列
*//**************************************************************************/
FORCE_INLINE CMatrix44& CVRManager::GetRightControllerMatrix(void) {
	return m_RightControllerPose;
}
/*************************************************************************//*!
		@brief			VR右コントローラー姿勢ベクトルを取得する
		@param			None
				
		@return			VR右コントローラー姿勢ベクトル
*//**************************************************************************/
FORCE_INLINE CVector3& CVRManager::GetRightControllerPoseVector(void) {
	return m_RightControllerVector;
}

/*************************************************************************//*!
		@brief			VR左コントローラーＸタッチを取得する
		@param			None
				
		@return			VR左コントローラーＸタッチ
*//**************************************************************************/
FORCE_INLINE MofFloat CVRManager::GetLeftTouchHorizontal(void) {
	return m_LeftControllerState.rAxis[0].x;
}
/*************************************************************************//*!
		@brief			VR左コントローラーＹタッチを取得する
		@param			None
				
		@return			VR左コントローラーＹタッチ
*//**************************************************************************/
FORCE_INLINE MofFloat CVRManager::GetLeftTouchVertical(void) {
	return m_LeftControllerState.rAxis[0].y;
}
/*************************************************************************//*!
		@brief			VR右コントローラーＸタッチを取得する
		@param			None
				
		@return			VR右コントローラーＸタッチ
*//**************************************************************************/
FORCE_INLINE MofFloat CVRManager::GetRightTouchHorizontal(void) {
	return m_RightControllerState.rAxis[0].x;
}
/*************************************************************************//*!
		@brief			VR右コントローラーＹタッチを取得する
		@param			None
				
		@return			VR右コントローラーＹタッチ
*//**************************************************************************/
FORCE_INLINE MofFloat CVRManager::GetRightTouchVertical(void) {
	return m_RightControllerState.rAxis[0].y;
}
/*************************************************************************//*!
		@brief			VRスケールを取得する
		@param			None
				
		@return			VRスケール
*//**************************************************************************/
FORCE_INLINE MofFloat CVRManager::GetVRScale(void){
	return m_VRScale;
}

/*************************************************************************//*!
		@brief			VRコントローラータッチを取得する
		@param			None
				
		@return			VRコントローラータッチ
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
		@brief			VR左コントローラートリガーを取得する
		@param			None
				
		@return			VR左コントローラートリガー
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