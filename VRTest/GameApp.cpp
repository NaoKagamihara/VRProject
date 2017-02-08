/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
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
		@brief			�A�v���P�[�V�����̏�����
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//���\�[�X�z�u�f�B���N�g���̐ݒ�
	CUtilities::SetCurrentDirectory("Resource");

	//�u�q������
	CVRManager::Initialize();
	g_pCamera = new CVRCamera();

	g_pScene = new TestScene(g_pCamera);

	g_pScene->Preparation();
	g_pScene->Create();
	g_pScene->SceneInitialize();
	g_pScene->Initialize();
	
	//�����[���[����Ƃ�
	//CGraphicsUtilities::Get3DPrimitiveShader();

	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̍X�V
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//�L�[�̍X�V
	g_pInput->RefreshKey();

	//escape�L�[�ŃQ�[���I��
	if (g_pInput->IsKeyPush(MOFKEY_ESCAPE))
	{

	}

	//VR�X�V
	CVRManager::Update();

	//�V�[���X�V
	g_pScene->SceneUpdate();
	g_pScene->Update();
	g_pScene->SceneRearUpdate();

	return TRUE;
}

/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̕`��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//�`�揈��
	g_pGraphics->RenderStart();
	//�[�x�o�b�t�@�L����
	g_pGraphics->SetDepthEnable(TRUE);

	/////////////////////////////////////////////////////
	//VR�p�`��

	//-------------------------------------------------------------------------
	//�E�ڕ`��J�n
	CVRManager::Begin(CVRManager::Eye::Right);
	//��ʂ̃N���A
	g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);
	//�J�����ݒ�
	g_pCamera->SetNowEye(CVRManager::Eye::Right);
	//g_pGraphics->SetBlending(BLEND_NONE);
	//�V�[����ʕ`��
	g_pScene->Render();

	//�E�ڕ`��I��
	CVRManager::End();
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//���ڕ`��J�n
	CVRManager::Begin(CVRManager::Eye::Left);
	
	//��ʂ̃N���A
	g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);

	//�J�����ݒ�
	g_pCamera->SetNowEye(CVRManager::Eye::Left);

	//�V�[����ʕ`��
	g_pScene->Render();

	//���ڕ`��I��
	CVRManager::End();
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//PC�f�B�X�v���C�ւ̕\���i�K�v������΁j

	//��ʂ̃N���A
	g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);

	//�J�����ݒ�
	g_pCamera->SetNowEye(CVRManager::Eye::Default);

	//�V�[����ʕ`��
	g_pScene->Render();

	//FPS
	CGraphicsUtilities::RenderString(10, 30, "%d", CUtilities::GetFPS());

	////�`����߂�
	//g_pGraphics->SetRenderTarget(prt, g_pGraphics->GetDepthTarget());

	//g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);
	//CMatrix44 mat;
	//static float ry = 0;
	//ry += 0.01f;
	//mat.RotationX(ry);
	//mat.SetTranslation(0,0,10);
	//g_pTarget->Render(mat);
	//-------------------------------------------------------------------------
	
	//�`��̏I��
	g_pGraphics->RenderEnd();

	//VR��ʂւ̃e�N�X�`���]��
	CVRManager::Present();

	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̉��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Release(void){

	//VR���
	CVRManager::Release();
	//�V�[�����
	g_pScene->SceneRelease();
	g_pScene->Release();
	delete g_pCamera;
	delete g_pScene;

	return TRUE;
}