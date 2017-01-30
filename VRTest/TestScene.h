//////////////////////////////
//テスト用シーン作成

#pragma once

#include		"PmGameScene.h"
#include		"TestObject.h"
#include		"CPlayer.h"
#include		"VR/VRCamera.h"

//アイテム
#include	"CWrenchSummary.h"
#include	"CFigureSummary.h"

//ギミック
#include	"CValveSummary.h"
#include	"CDrawerSummary.h"
#include	"CDirectionSet.h"

#include	"FluidTest.h"

class TestScene : public PmGameScene
{
private:
	CVRCamera*	m_pCamera;
public:
	TestScene(CVRCamera* camera){ m_pCamera = camera; };
	~TestScene(){};

	void Create()
	{
		addPmSummary(new PmTest);
		addSummary(new CameraPmTest);
		addPmSummary(new CPlayer(m_pCamera));

		////////////////////////////////////////////////////////
		//アイテム追加
		addPmSummary(new CWrenchSummary);
		//addPmSummary(new CFigureSummary);

		////////////////////////////////////////////////////////
		//ギミック追加
		//バルブ追加
		CBreakGimmick* valve = new CValveSummary();
		addPmSummary(valve);
		//ギミック破壊
		valve->gimmickBreak();

		//光の設定
		addPmSummary(new CDirectionSetSummary);

		//ひきだし追加
		addPmSummary(new CDrawerSummary(CVector3(0, 10, 25), CVector3(0, MOF_ToRadian(0), 0)));
	}
};