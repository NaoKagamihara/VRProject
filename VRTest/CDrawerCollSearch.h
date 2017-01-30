
/////////////////////////////////
//�����o���̓����蔻��T�[�`

#pragma once

#include		"PmCollisionSearch.h"
#include		"PmObject.h"

class CDrawerCollSearch : public PmCollisionSearch
{
private:
	bool		m_Poss;	//�͂߂邩�ǂ���
	bool		m_On;	//�͂߂��ԃt���O
	PmObject*	m_pHand;//�������ċ����
	CVector3	m_dPos;	//�O�̍��W
	CVector3	m_Pos;	//���W
public:
	CDrawerCollSearch(){ m_Poss = false; m_On = false; };
	~CDrawerCollSearch(){};

	void onTrigger(PmObject* obj);
	void update();

	bool getOnGrasp(){ return m_On; };
	CVector3 getdPos(){ return m_dPos; };
	CVector3 getPos(){ return m_Pos; };
};