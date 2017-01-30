//////////////////////////////////////
//�Q�[���𓮂������C���̃I�u�W�F�N�g
//

#include		"PmGameScene.h"


////////////////////////////////////////////////////
//�V�[���ɃI�u�W�F�N�g��ǉ����邽�߂̃��\�b�h
void PmGameScene::addSummary(CGameSummary* sum)
{
	mv_pObject.push_back(sum);
}

////////////////////////////////////////////////////
//�V�[����PxRigidBody�����I�u�W�F�N�g��ǉ����邽�߂̃��\�b�h
void PmGameScene::addPmSummary(PmGameSummary* sum)
{
	sum->setObjcetCreate(m_Manager.getObjectCreate());
	mv_pObject.push_back(sum);
}

PmGameScene::PmGameScene() : m_Manager()
{
	debugMode = false;
}

void PmGameScene::Preparation()
{
	m_Manager.Initialize();	//Physx���g�p���邽�߂̏���
}

void PmGameScene::Initialize()
{
	for (vector<CGameSummary*>::iterator itr = mv_pObject.begin(); itr != mv_pObject.end(); ++itr)
	{
		(*itr)->Create();
	}

	for (vector<CGameSummary*>::iterator itr = mv_pObject.begin(); itr != mv_pObject.end(); ++itr)
	{
		(*itr)->Initialize();
	}
}

void PmGameScene::Risetto()
{
	for (vector<CGameSummary*>::iterator itr = mv_pObject.begin(); itr != mv_pObject.end(); ++itr)
	{
		(*itr)->Initialize();
	}
}

void PmGameScene::Update()
{
	//�f�o�b�O���[�h�ؑ�
	if (g_pInput->IsKeyPush(MOFKEY_F1))
		debugMode = debugMode ? false : true;
	for (vector<CGameSummary*>::iterator itr = mv_pObject.begin(); itr != mv_pObject.end(); ++itr)
	{
		(*itr)->Update();
	}

	//�V�[���̕������Z�X�V
	m_Manager.Update();		

	for (vector<CGameSummary*>::iterator itr = mv_pObject.begin(); itr != mv_pObject.end(); ++itr)
	{
		(*itr)->RearUpdate();
	}
}

void PmGameScene::Render()
{
	for (vector<CGameSummary*>::iterator itr = mv_pObject.begin(); itr != mv_pObject.end(); ++itr)
	{
		(*itr)->Render();
	}

	for (vector<CGameSummary*>::iterator itr = mv_pObject.begin(); itr != mv_pObject.end(); ++itr)
	{
		(*itr)->RearRender();
	}

	if (debugMode == true)
	{
		for (vector<CGameSummary*>::iterator itr = mv_pObject.begin(); itr != mv_pObject.end(); ++itr)
		{
			(*itr)->DebugRender();
		}
	}
}

void PmGameScene::Release()
{
	for (vector<CGameSummary*>::iterator itr = mv_pObject.begin(); itr != mv_pObject.end(); ++itr)
	{
		(*itr)->Release();
		MOF_SAFE_DELETE((*itr));
	}

	m_Manager.Release();
}