
//////////////////////////////////////
//フィギュアクラス定義

#include		"CFigure.h"

CFigure::CFigure(CMeshContainer& mesh)
{
	m_Mesh = mesh;
	this->changeObjectTag(EPmObjectTag::OBJ_FIGURE, true);
	m_HaveFlag = false;
}

CFigure::~CFigure()
{
	m_Mesh.Release();
}


void CFigure::itemHave(const CVector3& pos, const CQuaternion& rot)
{
	if (m_HaveFlag == false)
	{
		m_HaveRot = this->getGlobalQuoRot();
		m_HaveFlag = true;
	}
	changeModeHave();
	setGlobalRot(rot * m_HaveRot);
	setGlobalPos(pos);
}

void CFigure::itemLost(const CVector3& vec, const CQuaternion& rot)
{
	changeModeFREE();
	addForce(vec * 50, PxForceMode::eVELOCITY_CHANGE);
}
void CFigure::render()
{
	//フィギュア描画
	CMatrix44 mat = this->getActorMatrix44();
	m_Mesh.Render(mat);

	CVector3 vec;
	mat.GetTranslation(vec);
	CSphere s(vec, 5);
	CGraphicsUtilities::RenderSphere(mat);
}

void CFigure::debugRender()
{
	PmRenderShape::RenderShape(this, Color(1, 0, 0, 0.7f));
}