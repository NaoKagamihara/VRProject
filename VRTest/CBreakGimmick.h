
//////////////////////
//壊れるギミック基底クラス

#pragma once

#include	"PmGameSummary.h"
#include	"CInundation.h"

class CBreakGimmick : public PmGameSummary
{
protected:
	bool			m_bBreakFlag;
	CInundation*	m_pInundation;
public:
	CBreakGimmick(CInundation* inun){ m_bBreakFlag = false; m_pInundation = inun; };
	virtual ~CBreakGimmick(){};

	virtual void gimmickBreak() = 0;

	bool getBreakFlag(){ return m_bBreakFlag; };
};