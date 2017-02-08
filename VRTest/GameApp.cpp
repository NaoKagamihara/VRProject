/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h" 
#include	"TestScene.h"
#include	"VR/VRManager.h"
#include	"VR/VRCamera.h"

PmGameScene*	g_pScene;
CVRCamera*		g_pCamera;


/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//リソース配置ディレクトリの設定
	CUtilities::SetCurrentDirectory("Resource");

	//ＶＲ初期化
	CVRManager::Initialize();
	g_pCamera = new CVRCamera();

	g_pScene = new TestScene(g_pCamera);

	g_pScene->Preparation();
	g_pScene->Create();
	g_pScene->SceneInitialize();
	g_pScene->Initialize();
	
	//しぇーだーしゅとく
	//CGraphicsUtilities::Get3DPrimitiveShader();

	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//キーの更新
	g_pInput->RefreshKey();

	//escapeキーでゲーム終了
	if (g_pInput->IsKeyPush(MOFKEY_ESCAPE))
	{

	}

	//VR更新
	CVRManager::Update();

	//シーン更新
	g_pScene->SceneUpdate();
	g_pScene->Update();
	g_pScene->SceneRearUpdate();

	return TRUE;
}

/*************************************************************************//*!
		@brief			アプリケーションの描画
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//描画処理
	g_pGraphics->RenderStart();
	//深度バッファ有効化
	g_pGraphics->SetDepthEnable(TRUE);

	/////////////////////////////////////////////////////
	//VR用描画

	//-------------------------------------------------------------------------
	//右目描画開始
	CVRManager::Begin(CVRManager::Eye::Right);
	//画面のクリア
	g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);
	//カメラ設定
	g_pCamera->SetNowEye(CVRManager::Eye::Right);
	//g_pGraphics->SetBlending(BLEND_NONE);
	//シーン画面描画
	g_pScene->Render();

	//右目描画終了
	CVRManager::End();
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//左目描画開始
	CVRManager::Begin(CVRManager::Eye::Left);
	
	//画面のクリア
	g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);

	//カメラ設定
	g_pCamera->SetNowEye(CVRManager::Eye::Left);

	//シーン画面描画
	g_pScene->Render();

	//左目描画終了
	CVRManager::End();
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//PCディスプレイへの表示（必要があれば）

	//画面のクリア
	g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);

	//カメラ設定
	g_pCamera->SetNowEye(CVRManager::Eye::Default);

	//シーン画面描画
	g_pScene->Render();

	//FPS
	CGraphicsUtilities::RenderString(10, 30, "%d", CUtilities::GetFPS());

	////描画先を戻す
	//g_pGraphics->SetRenderTarget(prt, g_pGraphics->GetDepthTarget());

	//g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);
	//CMatrix44 mat;
	//static float ry = 0;
	//ry += 0.01f;
	//mat.RotationX(ry);
	//mat.SetTranslation(0,0,10);
	//g_pTarget->Render(mat);
	//-------------------------------------------------------------------------
	
	//描画の終了
	g_pGraphics->RenderEnd();

	//VR画面へのテクスチャ転送
	CVRManager::Present();

	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの解放
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void){

	//VR解放
	CVRManager::Release();
	//シーン解放
	g_pScene->SceneRelease();
	g_pScene->Release();
	delete g_pCamera;
	delete g_pScene;

	return TRUE;
}