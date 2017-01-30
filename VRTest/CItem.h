
#pragma once

#include	"PmPreparation.h"
#include	"PmObjectDynamic.h"

////////////////////////////////////////
//Item�̏��
enum EItemState
{
	ITEM_FREE = (1 << 0),
	ITEM_HAVE = (1 << 1),
	ITEM_USE = (1 << 2),
};

////////////////////////////////////////
//Item�N���X
class CItem : public PmObjectDynamic
{
protected:

	EItemState		m_eItemState;

	//������Ă��Ȃ���Ԃɂ���
	void changeModeFREE();

	//������Ă����Ԃɂ���
	void changeModeHave();

public:
	CItem(){};
	virtual ~CItem(){};

	//��Ԃ�Ԃ�
	EItemState getState(){ return m_eItemState; };

	//�A�C�e��������
	virtual void itemHave(const CVector3& pos, const CVector3& rot){};
	virtual void itemHave(const CVector3& pos, const CQuaternion& rot){};

	//�A�C�e���𗣂�
	virtual void itemLost(const CVector3& vec){};

	//�A�C�e�����g�p����
	virtual void itemUse(){};

	//�`��
	virtual void render() = 0;
	virtual void debugRender() = 0;
};