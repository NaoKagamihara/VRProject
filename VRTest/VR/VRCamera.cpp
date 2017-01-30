/*************************************************************************//*!
					
					@file	VRCamera.cpp
					@brief	VR用カメラクラス。

															@author	CDW
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"VRCamera.h"

namespace Mof {

/*************************************************************************//*!
		@brief			コンストラクタ
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
		@brief			コンストラクタ
		@param[in]		pName		カメラ名

		@return			None
*//**************************************************************************/
CVRCamera::CVRCamera(LPCMofChar pName) :
CCamera(pName) ,
m_NowEye(CVRManager::Eye::Default) {
	m_ViewPort.w = CVRManager::GetWidth();
	m_ViewPort.h = CVRManager::GetHeight();
}
/*************************************************************************//*!
		@brief			コピーコンストラクタ
		@param[in]		pObj		コピーカメラ

		@return			None
*//**************************************************************************/
CVRCamera::CVRCamera(const CVRCamera& pObj) :
CCamera(pObj) ,
m_NowEye(pObj.m_NowEye) {
}
/*************************************************************************//*!
		@brief			デストラクタ
		@param			None

		@return			None
*//**************************************************************************/
CVRCamera::~CVRCamera() {
	Release();
}


/*************************************************************************//*!
		@brief			カメラの更新<br>
						設定されている情報からカメラに必要な各種行列を計算しなおす
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CVRCamera::Update(void) {
	//基本カメラのアップデート
	CCamera::Update();
	//VRシステムがない
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
	

	CMatrix44 matHMD = CVRManager::GetHMDMatrix();	//HMDのルームスケール内絶対位置回転等のマトリクス
	CMatrix44 matEyePos;		//左右目位置の行列
	CMatrix44 matHmdView;		//HMD位置回転×左右目行列
	CMatrix44 matProj;			//プロジェクション行列
	CMatrix44 matVPos;			//カメラ位置行列（方向固定）
	CVector3  vVrFront(0,0,1);	//VR空間内の正面方向。メンバで持っておいて、外から持ってくるといいかも
	
	matVPos.LookAtLH(m_Position, m_Position + vVrFront, Vector3(0, 1, 0));
	
	//カメラ位置＋HMD移動後の位置を計算
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

	//左右目から見たときのビュー・プロジェクション・ビルボードマトリクスを計算
	for (int i = 0; i < CVRManager::Eye::Count; i++)
	{
		//左右目位置の行列を取得
		CVRManager::ConvertMatrix44(pHMD->GetEyeToHeadTransform((vr::EVREye)i), matEyePos);
		matEyePos._41 *= CVRManager::GetVRScale();
		matEyePos._42 *= CVRManager::GetVRScale();
		matEyePos._43 *= CVRManager::GetVRScale();
		matEyePos.Inverse();
		matHmdView = matHMD * matEyePos;
		//GL系のビュー行列を、DX系のビュー行列に変換	// http://cv4mar.blogspot.jp/2009/03/transformation-matrices-between-opengl.html
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
		
		//左右目位置のポジションマトリクス取得
		//matHmdView *= matEyePos;
		matHmdView.Inverse();
		matHmdView.GetTranslation(m_VRPosition[i]);

		//視線方向ベクトル
		CVector3 vf(0, 0, 1);
		vf.TransformNormal(matHmdView);
		m_VRFront[i] = vf;

		//プロジェクション行列の生成（右手座標系に注意）
		CVRManager::ConvertMatrix44(pHMD->GetProjectionMatrix((vr::EVREye)i, m_Projection.Near, m_Projection.Far), matProj);
		//GL系のプロジェクション行列を、DX系のプロジェクション行列に変換
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

		//ビュー×プロジェクション行列
		m_VRViewProjectionMatrix[i] = m_VRViewMatrix[i] * m_VRProjectionMatrix[i];

		//ビルボードのマトリクス生成???
		CMatrix33 vml33(
			m_VRViewMatrix[i]._11, m_VRViewMatrix[i]._12, m_VRViewMatrix[i]._13,
			m_VRViewMatrix[i]._21, m_VRViewMatrix[i]._22, m_VRViewMatrix[i]._23,
			m_VRViewMatrix[i]._31, m_VRViewMatrix[i]._32, m_VRViewMatrix[i]._33);
		vml33.Inverse(m_VRBillBoardMatrix[i]);

	}


	//自分が設定カメラの場合更新を行う
	if (CGraphicsUtilities::GetCamera() == this)
	{
		CGraphicsUtilities::SetCamera(this);
	}
	return TRUE;
}
/*************************************************************************//*!
		@brief			ビューポート設定<br>
						画面サイズから自動的に設定を行う。
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CVRCamera::SetViewPort(void){
	m_ViewPort.x = 0.0f;	m_ViewPort.y = 0.0f;
	m_ViewPort.w = CVRManager::GetWidth();
	m_ViewPort.h = CVRManager::GetHeight();
	m_ViewPort.n = 0.0f;	m_ViewPort.f = 1.0f;
	return TRUE;
}

}
