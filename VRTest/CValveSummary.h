//////////////////////////////////
//�o���u

#pragma once

#include	"CBreakGimmick.h"
#include	"PmRender.h"
#include	"math.h"
#include	"CommonDefine.h"
#include	"CVRPlayerCamerah.h"
#include	"CHandCollSearch.h"

#define		COLLISIONRADIUS			3.0f		//�o���u�̓����蔻��̑傫��


//�o���u�Ǘ��N���X
class CValveSummary : public CBreakGimmick
{
private:
	CMeshContainer		m_ValuveMesh;	//�o���u�`��p���b�V��
	PmObjectDynamic*	m_pCollision;	//�o���u�p�����蔻��
	CHandCollSearch*	m_pValveSearch;	//�o���u�̏Փ˃T�[�`

	float				m_HisAng[HANDNUM];		//�ŏ��Ɉ����Ă�����̊p�x
	
	CVector3			m_Rotation;		//�o���u�̉�]�l
	bool				m_bprevflag;	//�o���u���������Ƃ��̃t���O

	//�e�X�g�p�̃p�C�v���b�V��
	CMeshContainer		m_PipeMesh;		//�p�C�v���b�V��
	PmObject*			m_pPipeObj;

public:
	CValveSummary(CInundation* inun) : CBreakGimmick(inun){};
	~CValveSummary(){};

	void Create();
	void Update();
	void RearUpdate();
	void Render();
	void DebugRender();
	void Release();

	void gimmickBreak();
};