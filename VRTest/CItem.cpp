//////////////////////////////
//�A�C�e���N���X��`

#include	"CItem.h"

//������Ă��Ȃ���Ԃɂ���
void CItem::changeModeFREE()
{
	this->setIsDisGravity(false);
	this->setKinematic(false);
	m_eItemState = EItemState::ITEM_FREE;
}

//������Ă����Ԃɂ���
void CItem::changeModeHave()
{
	this->setIsDisGravity(true);
	this->setKinematic(true);
	m_eItemState = EItemState::ITEM_HAVE;
}

//
void CItem::changeModeUse()
{

}

//
void CItem::changeModeNot()
{
	m_eItemState = EItemState::ITEM_NOT;
}