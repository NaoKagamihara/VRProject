//////////////////////////////////////
//ゲームを動かすメインのオブジェクト
//

#include		"PmGameScene.h"


////////////////////////////////////////////////////
//シーンにオブジェクトを追加するためのメソッド
void PmGameScene::addSummary(CGameSummary* sum)
{
	mv_pObject.push_back(sum);
}

////////////////////////////////////////////////////
//シーンにPxRigidBodyを持つオブジェクトを追加するためのメソッド
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
	m_Manager.Initialize();	//Physxを使用するための準備
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
	//デバッグモード切替
	if (g_pInput->IsKeyPush(MOFKEY_F1))
		debugMode = debugMode ? false : true;
	for (vector<CGameSummary*>::iterator itr = mv_pObject.begin(); itr != mv_pObject.end(); ++itr)
	{
		(*itr)->Update();
	}

	//シーンの物理演算更新
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