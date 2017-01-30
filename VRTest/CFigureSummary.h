
/////////////////////////////////////
//フィギュア管理クラス

#pragma once

#define			SCALE			5

#include		"PmGameSummary.h"
#include		"CFigure.h"

class CFigureSummary : public PmGameSummary
{
private:
	CFigure* m_pFigure;
public:
	CFigureSummary(){};
	~CFigureSummary(){};

	void Create();

	void Render();
	void DebugRender();
};