
///////////////
//���̐ݒ�

#pragma once

#include		"PmGameSummary.h"

//���ݒ�
class CDirectionSetSummary : public PmGameSummary
{
private:
	CDirectionalLight m_Light;
public:
	CDirectionSetSummary(){};
	~CDirectionSetSummary(){};

	void Create();
};