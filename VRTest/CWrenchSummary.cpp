
//�����`�Ǘ��N���X��`

#include	"CWrenchSummary.h"

void CWrenchSummary::Create()
{

	//���b�V���ǂݍ���
	CMeshContainer mesh;
	if (!mesh.Load("renti.mom"))
	{
		ERMS("WrenchMeshLoadError");
	}

	PmPhysxCreateTool* tool = this->createPhysxCreateTool();
	//�}�e���A��
	PxMaterial* mate;
	mate = tool->createMaterial(CVector3(0.5f, 0.5f, 0.1f));

	////////////////////////////////////////////////////
	//�o���u�쐬
	//�A�C�e���C���X�^���X�쐬
	m_pItem = new CWrench(mesh);
	createCItemObject(m_pItem);

	//**�����蔻��̎l�p���쐬
	//�����蔻��̑傫���擾
	CVector3 collScale(SCALEX, SCALEY, SCALEZ);
	//�����蔻���S�̂̑傫���ɍ��킹��

	//�����蔻��쐬
	PxShape* shape = tool->createBox(collScale, mate);
	m_pItem->addShape(*shape);

	//���W�w��
	m_pItem->setGlobalPos(CVector3(-2, 1, 0));
	//�g���K�[���[�h�ɕύX
	m_pItem->setTrigger(1, true);

	//**�Փ˔���̓ʃ��b�V�����쐬
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