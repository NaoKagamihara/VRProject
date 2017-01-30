/*************************************************************************//*!
					
					@file	VRManager.cpp
					@brief	VR関連管理。

															@author	CDW
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"VRManager.h"

namespace Mof {
	
vr::IVRSystem*						CVRManager::m_pHMD					=			NULL;			//VRデバイス
vr::IVRRenderModels*				CVRManager::m_pRenderModels			=			NULL;			//VR描画モデル
vr::TrackedDevicePose_t				CVRManager::m_TrackedDevicePose[vr::k_unMaxTrackedDeviceCount];	//VR姿勢
vr::VRControllerState_t				CVRManager::m_LeftControllerState;								//VRコントローラー状態
vr::VRControllerState_t				CVRManager::m_LeftControllerPrevState;							//VRコントローラー状態
vr::VRControllerState_t				CVRManager::m_RightControllerState;								//VRコントローラー状態
vr::VRControllerState_t				CVRManager::m_RightControllerPrevState;							//VRコントローラー状態
MofU32								CVRManager::m_Width					=			1512;			//VR幅
MofU32								CVRManager::m_Height				=			1680;			//VR高さ
CMatrix44							CVRManager::m_DevicePose[vr::k_unMaxTrackedDeviceCount];		//VR姿勢行列
CMatrix44							CVRManager::m_HMDPose;											//VRHMD行列
MofBool								CVRManager::m_bLeftController		=			FALSE;			//コントローラー認識
CMatrix44							CVRManager::m_LeftControllerPose;								//VRHMD行列
CVector3							CVRManager::m_LeftControllerVector;								//VRHMD行列
MofBool								CVRManager::m_bRightController		=			FALSE;			//コントローラー認識
CMatrix44							CVRManager::m_RightControllerPose;								//VRHMD行列
CVector3							CVRManager::m_RightControllerVector;							//VRHMD行列
LPTexture							CVRManager::m_pRenderTexture[Count]	=			{NULL};			//VR描画テクスチャ
LPDepthTarget						CVRManager::m_pDepthTarget			=			NULL;			//VR深度
LPRenderTarget						CVRManager::m_pOrigRenderTarget		=			NULL;			//描画テクスチャ退避
LPDepthTarget						CVRManager::m_pOrigDepthTarget		=			NULL;			//描画深度退避
ViewPort							CVRManager::m_OrigViewPort;										//描画ビューポート退避
MofFloat							CVRManager::m_VRScale				=			10.0f;			//VRスケール


LPGeometry*							CVRManager::m_Models = NULL;
int									CVRManager::m_ModelsCount = 0;

/*************************************************************************//*!
		@brief			初期化
		@param			None

		@return			TRUE			正常終了<br>
						FALSE			設定失敗
*//**************************************************************************/
MofBool CVRManager::Initialize(void) {
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Scene);
	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		MOF_PRINTLOG("Unable to init VR runtime: %s\n", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		return FALSE;
	}

	uint32_t w = 0;
	uint32_t h = 0;
	m_pHMD->GetRecommendedRenderTargetSize(&w, &h);
	m_Width = w;
	m_Height = h;

	m_pRenderModels = (vr::IVRRenderModels *)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError);
	if (!m_pRenderModels)
	{
		m_pHMD = NULL;
		vr::VR_Shutdown();
		MOF_PRINTLOG("Unable to get render model interface: %s\n", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		return FALSE;
	}

	if (!vr::VRCompositor())
	{
		MOF_PRINTLOG("Compositor initialization failed. See log file for details\n");
		return FALSE;
	}

	for (MofU32 i = 0; i < Eye::Count; i++)
	{
		m_pRenderTexture[i] = new CTexture();
		if (!m_pRenderTexture[i]->CreateTarget(m_Width, m_Height, PIXELFORMAT_R32G32B32A32_FLOAT, BUFFERACCESS_GPUREADWRITE))
		{
			return FALSE;
		}
	}
	m_pDepthTarget = new CDepthTarget();
	if (!m_pDepthTarget->Create(CGraphicsUtilities::GetGraphics(), m_Width, m_Height, PIXELFORMAT_D24S8_UNORM))
	{
		return FALSE;
	}
	return TRUE;
}

/*************************************************************************//*!
		@brief			VR姿勢行列の更新
		@param			None
		
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CVRManager::Update(void) {
	if (!m_pHMD)
	{
		return FALSE;
	}
	vr::VRCompositor()->WaitGetPoses(m_TrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
	
	//コントローラー認識用
	int invcnt = 0;
	vr::TrackedDeviceIndex_t iln = MOF_U32_MAX;
	vr::TrackedDeviceIndex_t irn = MOF_U32_MAX;
	vr::TrackedDeviceIndex_t iconn[2] = { MOF_U32_MAX,MOF_U32_MAX };

	for (vr::TrackedDeviceIndex_t nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
	{
		if (m_TrackedDevicePose[nDevice].bPoseIsValid)
		{
			m_DevicePose[nDevice] = ConvertMatrix44(m_TrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
			m_DevicePose[nDevice]._41 *= m_VRScale;
			m_DevicePose[nDevice]._42 *= m_VRScale;
			m_DevicePose[nDevice]._43 *= m_VRScale;
			vr::ETrackedDeviceClass vc = m_pHMD->GetTrackedDeviceClass(nDevice);
			if (vc == vr::TrackedDeviceClass_HMD)
			{
				m_DevicePose[vr::k_unTrackedDeviceIndex_Hmd].Inverse(m_HMDPose);
			}
			else if (vc == vr::TrackedDeviceClass_Controller)
			{
				vr::ETrackedControllerRole roll = m_pHMD->GetControllerRoleForTrackedDeviceIndex(nDevice);
				if (roll == vr::TrackedControllerRole_LeftHand)
				{
					iln = nDevice;
				}
				else if (roll == vr::TrackedControllerRole_RightHand)
				{
					irn = nDevice;
				}
				else if(invcnt < 2)
				{
					iconn[invcnt++] = nDevice;
				}
			}
		}
	}

	if (invcnt == 2)
	{
		iln = iconn[0];
		irn = iconn[1];
	}
	else if (iln == MOF_U32_MAX)
	{
		iln = iconn[0];
	}
	else if (irn == MOF_U32_MAX)
	{
		irn = iconn[0];
	}

	//コントローラー状態の取得
	if (iln != MOF_U32_MAX)
	{
		m_bLeftController = TRUE;
		m_LeftControllerPrevState = m_LeftControllerState;
		if (m_pHMD->GetControllerState(iln, &m_LeftControllerState,sizeof(m_LeftControllerState)))
		{
			m_LeftControllerPose = m_DevicePose[iln];
			CVector3 vf(0, 0, 1);
			CVector3Utilities::TransformNormal(vf, m_LeftControllerPose, m_LeftControllerVector);			
		}
	}
	else
	{
		m_bLeftController = FALSE;
	}
	if (irn != MOF_U32_MAX)
	{
		m_bRightController = TRUE;
		m_RightControllerPrevState = m_RightControllerState;
		if (m_pHMD->GetControllerState(irn, &m_RightControllerState, sizeof(m_RightControllerState)))
		{
			m_RightControllerPose = m_DevicePose[irn];
			CVector3 vf(0, 0, 1);
			CVector3Utilities::TransformNormal(vf, m_RightControllerPose, m_RightControllerVector);
		}
	}
	else
	{
		m_bRightController = FALSE;
	}
	return TRUE;
}
/*************************************************************************//*!
		@brief			開始
		@param[in]		eye			視点
		
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CVRManager::Begin(CVRManager::Eye eye){
	if (!m_pHMD)
	{
		return FALSE;
	}
	LPGraphics pGraphics = CGraphicsUtilities::GetGraphics();
	LPRenderCommandProxy pProxy = CGraphicsUtilities::GetCommandProxy();
	// 描画ターゲットの退避と設定
	if (pProxy)
	{
		m_pOrigRenderTarget = pProxy->GetRenderTarget();
		m_pOrigDepthTarget = pProxy->GetDepthTarget();
		m_OrigViewPort = *CGraphicsUtilities::GetCamera()->GetViewPort();
		//ビューポートをターゲットに合わせる
		ViewPort vp = m_OrigViewPort;
		vp.x = vp.y = 0;
		vp.w = m_Width;
		vp.h = m_Height;
		pProxy->SetRenderTarget(m_pRenderTexture[eye]->GetRenderTarget(), m_pDepthTarget);
		pProxy->SetViewPort(vp);
	}
	else
	{
		m_pOrigRenderTarget = pGraphics->GetRenderTarget();
		m_pOrigDepthTarget = pGraphics->GetDepthTarget();
		m_OrigViewPort = *CGraphicsUtilities::GetCamera()->GetViewPort();
		//ビューポートをターゲットに合わせる
		ViewPort vp = m_OrigViewPort;
		vp.x = vp.y = 0;
		vp.w = m_Width;
		vp.h = m_Height;
		pGraphics->SetRenderTarget(m_pRenderTexture[eye]->GetRenderTarget(), m_pDepthTarget);
		pGraphics->SetViewPort(vp);
	}

	return TRUE;
}
/*************************************************************************//*!
		@brief			終了
		@param			None
		
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CVRManager::End(){
	if (!m_pHMD)
	{
		return FALSE;
	}
	LPGraphics pGraphics = CGraphicsUtilities::GetGraphics();
	LPRenderCommandProxy pProxy = CGraphicsUtilities::GetCommandProxy();
	// 描画ターゲットを戻す
	if (pProxy)
	{
		pProxy->SetRenderTarget(m_pOrigRenderTarget, m_pOrigDepthTarget);
		pProxy->SetViewPort(m_OrigViewPort);
	}
	else
	{
		pGraphics->SetRenderTarget(m_pOrigRenderTarget, m_pOrigDepthTarget);
		pGraphics->SetViewPort(m_OrigViewPort);
	}
	m_pOrigRenderTarget = NULL;
	m_pOrigDepthTarget = NULL;
	return TRUE;
}

/*************************************************************************//*!
		@brief			テクスチャをVR用の描画テクスチャに転送する
		@param[in]		pTex		転送するシーンテクスチャ
		@param[in]		eye			転送するVR視点
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CVRManager::RenderTexture(LPTexture pTex, CVRManager::Eye eye) {
	if (!m_pHMD)
	{
		return FALSE;
	}
	LPGraphics pGraphics = CGraphicsUtilities::GetGraphics();
	Blending OrigBlend = pGraphics->GetBlending();
	m_pOrigRenderTarget = pGraphics->GetRenderTargetAt(0);
	m_pOrigDepthTarget = pGraphics->GetDepthTarget();
	m_OrigViewPort = *CGraphicsUtilities::GetCamera()->GetViewPort();

	//ビューポートをターゲットに合わせる
	ViewPort vp = m_OrigViewPort;
	vp.x = vp.y = 0;
	vp.w = m_Width;
	vp.h = m_Height;
	pGraphics->SetViewPort(vp);

	pGraphics->SetRenderTarget(m_pRenderTexture[eye]->GetRenderTarget(), m_pDepthTarget);
	pGraphics->SetBlending(BLEND_NONE);
	pGraphics->ClearTarget();
	Mof::Rectangle TargetRect(0, 0, m_Width, m_Height);
	CGraphicsUtilities::RenderTexture(TargetRect, pTex);

	pGraphics->SetBlending(OrigBlend);
	pGraphics->SetRenderTarget(m_pOrigRenderTarget, m_pOrigDepthTarget);
	pGraphics->SetViewPort(m_OrigViewPort);
	m_pOrigRenderTarget = NULL;
	m_pOrigDepthTarget = NULL;
	return TRUE;
}	
/*************************************************************************//*!
		@brief			VR描画テクスチャ転送
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CVRManager::Present(void) {
	if (!m_pHMD)
	{
		return FALSE;
	}
	m_pRenderTexture[Eye::Left]->GetTexture();
	m_pRenderTexture[Eye::Right]->GetTexture();
	vr::Texture_t leftEyeTexture = { m_pRenderTexture[Eye::Left]->GetTextureHandle(), vr::TextureType_DirectX, vr::ColorSpace_Auto };
	vr::EVRCompositorError errorl = vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
	vr::Texture_t rightEyeTexture = { m_pRenderTexture[Eye::Right]->GetTextureHandle(), vr::TextureType_DirectX, vr::ColorSpace_Auto };
	vr::EVRCompositorError errorr = vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
	return ((errorl != vr::VRCompositorError_None || errorr != vr::VRCompositorError_None) ? FALSE : TRUE);
}
	
/*************************************************************************//*!
		@brief			解放
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CVRManager::Release(void) {
	if (m_pHMD)
	{
		vr::VR_Shutdown();
		m_pHMD = NULL;
	}
	for (MofU32 i = 0; i < Eye::Count; i++)
	{
		MOF_SAFE_DELETE(m_pRenderTexture[i]);
	}
	MOF_SAFE_DELETE(m_pDepthTarget);


	for (MofU32 i = 0; i < m_ModelsCount; i++)
	{
		MOF_SAFE_DELETE(m_Models[i]);
	}
	MOF_SAFE_DELETE_ARRAY(m_Models);
	m_ModelsCount = 0;

	return TRUE;
}
/*************************************************************************//*!
		@brief			VRマトリクスをライブラリの行列形式に変換する
		@param[in]		matPose		VRマトリクス
				
		@return			変換結果行列
*//**************************************************************************/
CMatrix44 CVRManager::ConvertMatrix44(const vr::HmdMatrix34_t& matPose){
	return CMatrix44(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
		);
}
/*************************************************************************//*!
		@brief			VRマトリクスをライブラリの行列形式に変換する
		@param[in]		matPose		VRマトリクス
		@param[out]		out			出力行列
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CVRManager::ConvertMatrix44(const vr::HmdMatrix34_t& matPose, CMatrix44& out){
	out.SetValue(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
		);
	return TRUE;
}
/*************************************************************************//*!
		@brief			VRマトリクスをライブラリの行列形式に変換する
		@param[in]		matPose		VRマトリクス
				
		@return			変換結果行列
*//**************************************************************************/
CMatrix44 CVRManager::ConvertMatrix44(const vr::HmdMatrix44_t& matPose) {
	return CMatrix44(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], matPose.m[3][0],
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], matPose.m[3][1],
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], matPose.m[3][2],
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], matPose.m[3][3]
		);
}
/*************************************************************************//*!
		@brief			VRマトリクスをライブラリの行列形式に変換する
		@param[in]		matPose		VRマトリクス
		@param[out]		out			出力行列
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CVRManager::ConvertMatrix44(const vr::HmdMatrix44_t& matPose, CMatrix44& out) {
	out.SetValue(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], matPose.m[3][0],
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], matPose.m[3][1],
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], matPose.m[3][2],
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], matPose.m[3][3]
		);
	return TRUE;
}




bool CVRManager::LoadRenderModels()
{
	if (!m_pHMD) return false;

	m_ModelsCount = 3;// vr::k_unMaxTrackedDeviceCount;
	m_Models = new LPGeometry[m_ModelsCount];
	for (int i = 0; i < m_ModelsCount; i++){ m_Models[i] = 0; }

	for (uint32_t unTrackedDevice = vr::k_unTrackedDeviceIndex_Hmd; unTrackedDevice < vr::k_unMaxTrackedDeviceCount; unTrackedDevice++)
	{
		if (!m_pHMD->IsTrackedDeviceConnected(unTrackedDevice))
			continue;
		vr::RenderModel_ControllerMode_State_t;
		vr::ETrackedDeviceClass vc = m_pHMD->GetTrackedDeviceClass(unTrackedDevice);
		if (vc == vr::TrackedDeviceClass_HMD || vc == vr::ETrackedDeviceClass::TrackedDeviceClass_Controller)
		{
			//----------------------------------------------------------------
			//unTrackedDevice番目のデバイスの名前を取得
			vr::ETrackedDeviceProperty prop = vr::Prop_RenderModelName_String;
			
			//バッファ長計算
			uint32_t len = m_pHMD->GetStringTrackedDeviceProperty(unTrackedDevice, prop, NULL, 0, 0);
			if (len == 0){ continue; }

			//バッファを確保し、名前を取得
			char *pName = new char[len];
			m_pHMD->GetStringTrackedDeviceProperty(unTrackedDevice, prop, pName, len, 0);
			
			//読み込んでジオメトリに
			LPGeometry pGeometry = LoadRenderModels(pName);

			//後始末
			delete[] pName;
			if (!pGeometry) continue;

			//----------------------------------------------------------------
			//リスト追加
			int no = -1;
			if (vc == vr::TrackedDeviceClass_HMD) { no = 0; }
			else
			{
				vr::ETrackedControllerRole roll = m_pHMD->GetControllerRoleForTrackedDeviceIndex(unTrackedDevice);
				if (roll == vr::TrackedControllerRole_LeftHand) { no = 1; }
				if (roll == vr::TrackedControllerRole_RightHand) { no = 2; }
			}
			if (no == -1)
			{
				delete pGeometry;
				continue;
			}
			if (m_Models[no] != NULL)
			{
				m_Models[no]->Release();
				delete m_Models[no];
			}
			m_Models[no] = pGeometry;

			//break;
		}
	}
	return TRUE;
}

LPGeometry CVRManager::LoadRenderModels(const char* name)
{
	//----------------------------------------------------------------

	MOF_PRINTLOG("Load RenderModel:%s\n", name);
	vr::RenderModel_t *pVrModel = NULL;
	vr::RenderModel_TextureMap_t *pVrTexture = NULL;
	vr::EVRRenderModelError error;

	//----------------------------------------------------------------
	//モデルデータを読み込む
	while (1)
	{
		error = vr::VRRenderModels()->LoadRenderModel_Async(name, &pVrModel);
		if (error != vr::VRRenderModelError_Loading) break;
		Sleep(1);
	}

	//モデルデータが得られなかった
	if (!pVrModel) { return NULL; }

	//----------------------------------------------------------------
	//GL用のモデルが返ってくるので、DX用に色々直す

	VERTEX_NORMAL_TEXTURE1_COLORU1* vert = new VERTEX_NORMAL_TEXTURE1_COLORU1[pVrModel->unVertexCount];
	for (int i = 0; i < pVrModel->unVertexCount; i++)
	{
		const vr::RenderModel_Vertex_t* p = &pVrModel->rVertexData[i];
		vert[i].px = +p->vPosition.v[0];
		vert[i].py = +p->vPosition.v[1];
		vert[i].pz = -p->vPosition.v[2];
		vert[i].nx = +p->vNormal.v[0];
		vert[i].ny = +p->vNormal.v[1];
		vert[i].nz = -p->vNormal.v[2];
		vert[i].tu = +p->rfTextureCoord[0];
		vert[i].tv = +p->rfTextureCoord[1];
		vert[i].c = MOF_COLOR_WHITE;
	}
	LPVertexBuffer pVBuffer = new CVertexBuffer();
	BOOL ret = pVBuffer->CreateBuffer(VERTEXFLAG_POSITION | VERTEXFLAG_NORMAL | VERTEXFLAG_TEXTURECOORDS1 | VERTEXFLAG_COLOR1, pVrModel->unVertexCount, vert, BUFFERACCESS_GPUREADWRITE);
	delete[] vert;
	if (!ret)
	{
		delete pVBuffer;
		return NULL;
	}



	//頂点データをｚを逆にして格納
	/*struct Vert
	{
		Vector3 pos;
		Vector3 normal;
		Vector2 uv;
	}*pVert = new Vert[pVrModel->unVertexCount];
	for (int i = 0; i < pVrModel->unVertexCount; i++)
	{
		pVert[i] = *(Vert*)(&pVrModel->rVertexData[i]);
		pVert[i].pos.z *= -1;
		pVert[i].normal.z *= -1;
	}
	*/

	//頂点インデックスを逆順にして格納
	uint16_t* arIndex = new uint16_t[pVrModel->unTriangleCount * 3];
	for (int i = 0; i < pVrModel->unTriangleCount; i++)
	{
		arIndex[i * 3 + 0] = pVrModel->rIndexData[i * 3 + 0];
		arIndex[i * 3 + 1] = pVrModel->rIndexData[i * 3 + 2];
		arIndex[i * 3 + 2] = pVrModel->rIndexData[i * 3 + 1];
	}
	LPIndexBuffer pIBuffer = new CIndexBuffer();
	ret = pIBuffer->CreateBuffer(sizeof(uint16_t), pVrModel->unTriangleCount * 3, arIndex, BUFFERACCESS_GPUREADWRITE);
	delete[] arIndex;
	if(!ret)
	{
		delete pVBuffer;
		delete pIBuffer;
		return NULL;
	}


	//----------------------------------------------------------------
	//モデルのGeometryを生成
	/*LPGeometry pGeometry = CGraphicsUtilities::CreateGeometry(
		(LPCMofVoid)pVert, VERTEXFLAG_POSITION | VERTEXFLAG_NORMAL | VERTEXFLAG_TEXTURECOORDS1, pVrModel->unVertexCount,
		arIndex, pVrModel->unTriangleCount * 3);
	pGeometry->SetTopology(PrimitiveTopology::PRIMITIVETOPOLOGY_TRIANGLELIST);

	//不要分解放
	delete[] pVert;
	delete[] arIndex;
	*/

	LPMaterial pMaterial = new CMaterial();
	LPGeometry pGeometry = new CGeometry(name, PRIMITIVETOPOLOGY_TRIANGLELIST, pVBuffer, pIBuffer, pMaterial);

	if (!pGeometry)
	{ 
		delete pMaterial;
		delete pVBuffer;
		delete pIBuffer;
		delete[] arIndex;
		return NULL;
	}

	//----------------------------------------------------------------
	//テクスチャ
	while (1)
	{
		error = vr::VRRenderModels()->LoadTexture_Async(pVrModel->diffuseTextureId, &pVrTexture);
		if (error != vr::VRRenderModelError_Loading) break;
		Sleep(1);
	}
	if (pVrTexture)
	{
		//vr::RenderModel_TextureMap_tからCTextureに変換
		CTexture* pTexture = new CTexture();
		bool ret = pTexture->Create(pVrTexture->unWidth, pVrTexture->unHeight, PIXELFORMAT_R8G8B8A8_UNORM, BufferAccess::BUFFERACCESS_GPUREAD_CPUWRITE);
		if (!ret)
		{
			delete pTexture;
		}
		else
		{
			byte* pb = (byte*)pTexture->Map(BUFFERMAP_WRITE_DISCARD);
			memcpy(pb, pVrTexture->rubTextureMapData, pVrTexture->unWidth * pVrTexture->unHeight * 4);
			pTexture->UnMap();
			vr::VRRenderModels()->FreeTexture(pVrTexture);

			//マテリアルに追加
			LPMaterial pMaterial = pGeometry->GetMaterial();
			pMaterial->GetTextureArray()->Add(pTexture);
		}
	}

	//----------------------------------------------------------------
	//モデルデータ解放
	vr::VRRenderModels()->FreeRenderModel(pVrModel);

	return pGeometry;
}


Matrix44 CVRManager::GetTrackingMatrix(TrackingDevice device)
{
	//各マトリクスを取得
	CMatrix44 matCon;

	switch (device)
	{
	case TD_HMD:	//HMD
		//matCon = CVRManager::GetHMDMatrix();
		//matCon.Inverse();
		matCon = m_DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
		break;

	case TD_LEFT: //Left
		matCon = CVRManager::GetLeftControllerMatrix();
		break;

	case TD_RIGHT: //Right
		matCon = CVRManager::GetRightControllerMatrix();
		break;

	default:
		return Matrix44();
	}


	//GL系の行列を、DX系の行列に変換
	matCon._13 *= -1;
	matCon._23 *= -1;
	matCon._31 *= -1;
	matCon._32 *= -1;
	matCon._43 *= -1;
	
	//VR Scale倍
	CMatrix44 matScale;
	matScale.Scaling(CVRManager::GetVRScale());

	//最終的なMatrix
	return matScale * matCon;	
}

} //namespace
//[EOF]