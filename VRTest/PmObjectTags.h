////////////////////////////////////////
//PmObject�Ɏ�������Tag

#pragma once

///////////////////////////
//�����蔻��̃t�B���^�p�̃^�O
enum EFilterTag
{
	FILTER_DEFAULT			= (1 << 0),
	FILTER_TEST = (1 << 1),
	FILTER_TEST2 = (1 << 2)
};

////////////////////////////////
//�I�u�W�F�N�g�𔻕ʂ��邽�߂̃^�O
enum EPmObjectTag
{
	OBJ_RIGHTHAND = (1 << 0),
	OBJ_LEFTHAND = (1 << 1),
	OBJ_HANDGRASP = (1 << 2),
	OBJ_ITEM = (1 << 3),
	OBJ_FIGURE = (1 << 4),
	OBJ_FUSE = (1 << 5),
	OBJ_DHAVE = (1 << 6),
};