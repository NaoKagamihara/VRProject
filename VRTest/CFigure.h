
////////////////////////////////////
//�t�B�M���A�N���X

#pragma once

#include		"CItem.h"
#include		"PmRender.h"
#include		"CommonDefine.h"

class CFigure : public CItem
{
private:
	CMeshContainer	m_Mesh;
	CQuaternion		m_HaveRot;			//�����ꂽ�u�Ԃ̉�]
	bool			m_HaveFlag;			//���
public:
	CFigure(CMeshContainer& mesh);
	virtual ~CFigure();

	virtual void itemHave(const CVector3& pos, const CQuaternion& rot);

	virtual void itemLost(const CVector3& vec, const CQuaternion& rot);

	virtual void render();
	virtual void debugRender();
};