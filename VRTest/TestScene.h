//////////////////////////////
//テスト用シーン作成

#pragma once

#include		"PmGameScene.h"
#include		"TestObject.h"
#include		"CPlayer.h"
#include		"VR/VRCamera.h"

//浸水クラス
#include	"CInundation.h"
#include	"FluidTest.h"

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
	CVRCamera*		m_pCamera;		//プレイヤーに渡すＶＲ用カメラ
	CInundation*	m_pInun;		//浸水管理クラス
public:
	TestScene(CVRCamera* camera){ m_pCamera = camera; };
	~TestScene(){};

	void Create()
	{
		addPmSummary(new PmTest);				//テスト用クラス
		addSummary(new CameraPmTest);			//テスト用カメラクラス

		addPmSummary(new CPlayer(m_pCamera));	//プレイヤー
		m_pInun = new CInundation();			//浸水管理クラス
		//光の設定
		addPmSummary(new CDirectionSetSummary);

		////////////////////////////////////////////////////////
		//アイテム追加
		addPmSummary(new CWrenchSummary);
		//addPmSummary(new CFigureSummary);

		////////////////////////////////////////////////////////
		//壊れるギミック追加
		//バルブ追加
		CBreakGimmick* valve = new CValveSummary(m_pInun);
		addPmSummary(valve);
		//ギミック破壊
		valve->gimmickBreak();

		////////////////////////////////////////////////////////
		//ギミック追加
		//ひきだし追加
		addPmSummary(new CDrawerSummary(CVector3(23.5f, 5, -32), CVector3(0, MOF_ToRadian(180), 0)));

		//最後に水を追加
		addPmSummary(m_pInun);
		
		addPmSummary(new FluidTest(this->getFluid()));
	}
};