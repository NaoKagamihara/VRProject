
//////////////////////
//壊れるギミック基底クラス

#pragma once

#include	"PmGameSummary.h"

class CBreakGimmick : public PmGameSummary
{
protected:
	bool	m_bBreakFlag;
public:
	CBreakGimmick(){ m_bBreakFlag = false; };
	virtual ~CBreakGimmick(){};

	virtual void gimmickBreak() = 0;

	bool getBreakFlag(){ return m_bBreakFlag; };
};