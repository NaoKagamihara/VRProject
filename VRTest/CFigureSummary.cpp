
////////////////////////////////////////
//�t�B�M���A�Ǘ��N���X

#include		"CFigureSummary.h"

void CFigureSummary::Create()
{
	CMeshContainer mesh;
	if (!mesh.Load("t302/t302.mom"))
		ERMS("t303LoadERROE");

	//�t�B�M���A���쐬
	m_pFigure = new CFigure(mesh);
	createCItemObject(m_pFigure);
	
	PmPhysxCreateTool* tool = this->createPhysxCreateTool();
	PxMaterial* mate = tool->createMaterial(CVector3(0.5f, 0.5f, 0.1f));
	//�����蔻��쐬
	PxShape* shape = tool->createSphere(SCALE,mate);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	m_pFigure->addShape(*shape);

	PxShape* conShape = tool->createMesh(mesh, mate);
	m_pFigure->addShape(*conShape);

	//���W�w��
	m_pFigure->setGlobalPos(CVector3(10, 20, 0));
	//�g���K�[���[�h�ɕύX

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