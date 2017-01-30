
///////////////
//Œõ‚Ìİ’è

#pragma once

#include		"PmGameSummary.h"

//Œõİ’è
class CDirectionSetSummary : public PmGameSummary
{
private:
	CDirectionalLight m_Light;
public:
	CDirectionSetSummary(){};
	~CDirectionSetSummary(){};

	void Create();
};