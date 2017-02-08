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
#include	"CDrawerSummary.h"
#include	"CDirectionSet.h"
#include	"CFuseSummary.h"
#include	"CMonitor.h"

//壊れるギミック
#include	"CValveSummary.h"


#include	"FluidTest.h"

class TestScene : public PmGameScene
{
private:
	CVRCamera*		m_pCamera;		//プレイヤーに渡すＶＲ用カメラ
	CInundation*	m_pInun;		//浸水管理クラス

	LPTexture		m_pMonitorTexture;		//モニターに描画するテクスチャ
	LPTexture		m_pTex;
public:
	TestScene(CVRCamera* camera){ m_pCamera = camera; };
	~TestScene(){};

	void Create()
	{
		addPmSummary(new PmTest);				//テスト用クラス
		addSummary(new CameraPmTest);			//テスト用カメラクラス

		//モニター描画用のテクスチャ生成
		m_pMonitorTexture = new CTexture;
		m_pMonitorTexture->CreateTarget(g_pGraphics->GetTargetWidth(), g_pGraphics->GetTargetHeight(), PixelFormat::PIXELFORMAT_R8G8B8A8_UNORM, BufferAccess::BUFFERACCESS_GPUREADWRITE);
		//m_pMonitorTexture->CreateTarget(MONITORSIZEWIDTH, MONITORSIZEHEIGHT, PixelFormat::PIXELFORMAT_R8G8B8A8_UNORM, BufferAccess::BUFFERACCESS_GPUREADWRITE);

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
		addPmSummary(new CFuseSummary(CVector3(0, 0, 0), CVector3(0, 0, 0)));

		//モニター追加
		addPmSummary(new CMonitor(m_pMonitorTexture));

		//最後に水を追加
		//addPmSummary(m_pInun);
		
		addPmSummary(new FluidTest(this->getFluid()));

		m_pTex = new CTexture;
		m_pTex->Load("back.jpg");
	}

	void SceneRearUpdate()
	{
		//描画先を一時的に保存し、描画先をセット
		LPRenderTarget prt = g_pGraphics->GetRenderTarget();
		g_pGraphics->SetRenderTarget(m_pMonitorTexture->GetRenderTarget(),g_pGraphics->GetDepthTarget());

		//画面のクリア
		g_pGraphics->ClearTarget(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0);

		//モニターに描画
		CGraphicsUtilities::RenderString(10, 10, "TETETETETESTaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
		CRectangle drec(0, 0, m_pMonitorTexture->GetWidth(), m_pMonitorTexture->GetHeight());
		CGraphicsUtilities::RenderTexture(drec, m_pTex);

		//描画先を戻す
		g_pGraphics->SetRenderTarget(prt, g_pGraphics->GetDepthTarget());
	}

	void SceneRelease()
	{
		delete m_pMonitorTexture;
		delete m_pTex;
		delete m_pInun;
	}
};