
////////////////////////////////////////
//フィギュア管理クラス

#include		"CFigureSummary.h"

void CFigureSummary::Create()
{
	CMeshContainer mesh;
	if (!mesh.Load("t302/t302.mom"))
		ERMS("t303LoadERROE");

	//フィギュアを作成
	m_pFigure = new CFigure(mesh);
	createCItemObject(m_pFigure);
	
	PmPhysxCreateTool* tool = this->createPhysxCreateTool();
	PxMaterial* mate = tool->createMaterial(CVector3(0.5f, 0.5f, 0.1f));
	//当たり判定作成
	PxShape* shape = tool->createSphere(SCALE,mate);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	m_pFigure->addShape(*shape);

	PxShape* conShape = tool->createMesh(mesh, mate);
	m_pFigure->addShape(*conShape);

	//座標指定
	m_pFigure->setGlobalPos(CVector3(10, 20, 0));
	//トリガーモードに変更

	delete tool;

}

void CFigureSummary::Render()
{
	m_pFigure->render();
}

void CFigureSummary::DebugRender()
{
	m_pFigure->debugRender();
}