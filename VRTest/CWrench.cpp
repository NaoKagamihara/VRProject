///////////////////////
//�����`

#include	"CWrench.h"

CWrench::CWrench(CMeshContainer& mesh)
{
	m_Mesh = mesh;
}

CWrench::~CWrench()
{
	m_Mesh.Release();
}

void CWrench::itemHave(const CVector3& pos, const CQuaternion& rot)
{
	changeModeHave();
	setGlobalRot(rot);
	setGlobalPos(pos);
}

void CWrench::itemLost(const CVector3& vec, const CQuaternion& rot)
{	
	changeModeFREE();
	addForce(vec * 50, PxForceMode::eVELOCITY_CHANGE);
}
void CWrench::render()
{

	CMatrix44 sca;
	CMatrix44 rot;
	CMatrix44 pos;

	//�傫��
	sca.Scaling(OBJALLSCALE - 0.2f);
	//��]
	this->getGlobalQuoRot().ConvertMatrixTranspose(rot);
	//���W
	pos.SetTranslation(this->getGlobalPos());
	//�`��
	m_Mesh.Render(sca * rot * pos);

}

void CWrench::debugRender()
{
	PmRenderShape::RenderShape(this, Color(1, 0, 0, 0.7f));
}