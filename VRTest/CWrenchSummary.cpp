
//レンチ管理クラス定義

#include	"CWrenchSummary.h"

void CWrenchSummary::Create()
{

	//メッシュ読み込み
	CMeshContainer mesh;
	if (!mesh.Load("renti.mom"))
	{
		ERMS("WrenchMeshLoadError");
	}

	PmPhysxCreateTool* tool = this->createPhysxCreateTool();
	//マテリアル
	PxMaterial* mate;
	mate = tool->createMaterial(CVector3(0.5f, 0.5f, 0.1f));

	////////////////////////////////////////////////////
	//バルブ作成
	//アイテムインスタンス作成
	m_pItem = new CWrench(mesh);
	createCItemObject(m_pItem);

	//**当たり判定の四角を作成
	//当たり判定の大きさ取得
	CVector3 collScale(SCALEX, SCALEY, SCALEZ);
	//当たり判定を全体の大きさに合わせる

	//当たり判定作成
	PxShape* shape = tool->createBox(collScale, mate);
	m_pItem->addShape(*shape);

	//座標指定
	m_pItem->setGlobalPos(CVector3(-2, 1, 0));
	//トリガーモードに変更
	m_pItem->setTrigger(1, true);

	//**衝突判定の凸メッシュを作成
	//PxShape* meshShape = tool->createMesh(mesh, mate);
	//m_pItem->addShape(*meshShape);

	delete tool;
}

void CWrenchSummary::Render()
{
	m_pItem->render();
}

void CWrenchSummary::DebugRender()
{
	m_pItem->debugRender();
}