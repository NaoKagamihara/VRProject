//////////////////////////////
//アイテムクラス定義

#include	"CItem.h"

//持たれていない状態にする
void CItem::changeModeFREE()
{
	this->setIsDisGravity(false);
	this->setKinematic(false);
	m_eItemState = EItemState::ITEM_FREE;
}

//持たれている状態にする
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