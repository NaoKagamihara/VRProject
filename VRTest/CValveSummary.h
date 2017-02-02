//////////////////////////////////
//�o���u

#pragma once

#include	"CBreakGimmick.h"
#include	"PmRender.h"
#include	"CHand.h"
#include	"math.h"
#include	"CommonDefine.h"
#include	"CVRPlayerCamerah.h"

#define		COLLISIONRADIUS			3.0f		//�o���u�̓����蔻��̑傫��

enum EHANDSTATE
{
	RIGHTHAND = 0,
	LEFTHAND,
	HANDNUM,
};

//�o���u�̓�����T�[�`
class CValveSearch : public PmCollisionSearch
{
private:
	PmObject*	m_pHandObj[2];
public:
	CValveSearch(){ m_pHandObj[RIGHTHAND] = NULL; m_pHandObj[LEFTHAND] = NULL; };
	virtual ~CValveSearch(){};

	void onTrigger(PmObject* obj);

	//�肪�o���u�ɓ������ċ���΃I�u�W�F�������Ă���i����ȊO��NULL�j
	PmObject* getHandObj(int handNum){ return m_pHandObj[handNum]; };
};

//�o���u�Ǘ��N���X
class CValveSummary : public CBreakGimmick
{
private:
	CMeshContainer		m_ValuveMesh;	//�o���u�`��p���b�V��
	PmObjectDynamic*	m_pCollision;	//�o���u�p�����蔻��
	CValveSearch*		m_pValveSearch;	//�o���u�̏Փ˃T�[�`

	float				m_HisAng[HANDNUM];		//�ŏ��Ɉ����Ă�����̊p�x
	
	CVector3			m_Rotation;		//�o���u�̉�]�l
	bool				m_bprevflag;	//�o���u���������Ƃ��̃t���O

	//�e�X�g�p�̃p�C�v���b�V��
	CMeshContainer		m_PipeMesh;		//�p�C�v���b�V��
	PmObject*			m_pPipeObj;

public:
	CValveSummary(){};
	~CValveSummary(){};

	void Create();
	void Update();
	void RearUpdate();
	void Render();
	void DebugRender();
	void Release();

	void gimmickBreak();
};