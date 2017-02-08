/////////////////////////////////////
//��

#pragma once

#include	"PmObject.h"
#include	"PmRender.h"
#include	"CItem.h"

//���f���`�掞�̑傫��
#define		MODELSCALE			0.1f

//////////////////////////////////////////////
//��̏��
enum EHandState
{
	HAND_FREE = (1 << 0),
	HAND_HAVE = (1 << 1),
	HAND_USE = (1 << 2),
	HAND_GRASP = (1 << 3),
};

////////////////////////////////////////////
//��̏�Ԃ̍X�V�ɕK�v�ȃf�[�^
struct HandData
{
	CVector3	pos;				//���W
	CQuaternion	handRot;			//��̊p�x
	bool		grasp;				//����t���O
	bool		button;				//�g���t���O

	//CVector3	poseVec;			//��̎p��
};

///////////////////////////////////////////////////
//��̓����蔻��T�[�`
class CHandSearch : public PmCollisionSearch
{
private:
	//�͈͓��ɂ���A�C�e���̃|�C���^
	//�����Ȃ��ꍇ��NULL
	CItem*		m_pItem;
public:

	CHandSearch() : m_pItem(NULL) {};
	~CHandSearch(){};

	//�͈͓���Item�̃|�C���^��ۑ�����
	void onTrigger(PmObject* obj);

	//�ۑ�����Item���擾����
	CItem* getItem(){ return m_pItem; };

	//Item��NULL�ɂ���
	void emp(){ m_pItem = NULL; };
};

///////////////////////////////////////////////////
//��̏�Ԃ��Ǘ�����N���X
class CHand
{
private:
	CMeshContainer	m_Mesh;		//��̃��f��
	PmObject*		m_pHandColl;	//��̓����蔻��
	EHandState		m_eState;		//��̏��
	CItem*			m_pItem;		//�����Ă���A�C�e��
	CHandSearch*	m_pSearch;		//�g���K�[
	CVector3		m_HysPos;		//�P�t���[���O�̍��W
	CQuaternion		m_HysRot;		//�P�t���[���O�̌���
	CVector3		m_PoseVec;		//�R���g���[���̎p��

public:
	CHand(PmObject* obj,EPmObjectTag tag);
	~CHand();

	//�ǂݍ���
	void load();

	//�X�V
	void update(HandData& data);

	//�`��
	void render();

	//�f�o�b�O�p�̕`��
	void debugRender();

	PmObject* getHandColl(){ return m_pHandColl; };

private:

	//��̏�Ԃɉ���������
	void freeUpdate(HandData& data);
	void haveUpdate(HandData& data);
	void useUpdate(HandData& data);
	void graspUpdate(HandData& data);

};