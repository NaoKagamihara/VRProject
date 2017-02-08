////////////////////////
//ƒŒƒ“ƒ`

#pragma once

#include	"CItem.h"
#include	"PmRender.h"
#include	"CommonDefine.h"

class CWrench : public CItem
{
private:
	CMeshContainer m_Mesh;
public:
	CWrench(CMeshContainer& mesh);
	virtual ~CWrench();

	virtual void itemHave(const CVector3& pos, const CQuaternion& rot);

	virtual void itemLost(const CVector3& vec, const CQuaternion& rot);

	virtual void render();
	virtual void debugRender();
};