
#include	"CHand.h"

//////////////////////////////////////////////////
//��̓����蔻��T�[�`
void CHandSearch::onTrigger(PmObject* obj)
{
	PmObject* test = obj;

	if (obj->checkObjectTag(EPmObjectTag::OBJ_ITEM))
	{
		if (m_pItem == NULL)
		{
			m_pItem = (CItem*)obj;
		}
		else if (m_pItem == obj)
		{
			m_pItem = NULL;
		}
	}
}

///////////////////////////////////////////////////
//����Ǘ�����N���X

//�R���X�g���N�^
CHand::CHand(PmObject* obj, EPmObjectTag tag) : m_pHandColl(NULL),
m_eState(),
m_pItem(NULL),
m_pSearch(NULL),
m_PoseVec()
{
	m_pHandColl = obj;

	m_pHandColl->setTrigger(1, true);
	m_pHandColl->releaseCollisionSearch();
	m_pSearch = new CHandSearch();
	m_pHandColl->setCollisionSearch(m_pSearch);

	m_eState = EHandState::HAND_FREE;

	m_pHandColl->setObjectTag(tag);
}
//�f�X�g���N�^
CHand::~CHand()
{

}

//�ǂݍ���
void CHand::load()
{
	if (m_pHandColl->checkObjectTag(EPmObjectTag::OBJ_LEFTHAND))
		if (!m_Mesh.Load("HandMom/hand_left.mom"))
			ERMS("LefthandMeshLoadError");
	if (m_pHandColl->checkObjectTag(EPmObjectTag::OBJ_RIGHTHAND))
		if (!m_Mesh.Load("HandMom/hand_right.mom"))
			ERMS("RighthandMeshLoadError");
}

//�X�V
void CHand::update(HandData& data)
{
	m_pHandColl->setGlobalPos(data.pos);	//���͂���X�V
	m_pHandColl->setGlobalRot(data.handRot);
	//m_PoseVec = data.poseVec;

	CQuaternion q = m_pHandColl->getGlobalQuoRot();

	//��̏�Ԃɉ���������
	switch (m_eState)
	{
	case EHandState::HAND_FREE:
		freeUpdate(data);
		break;
	case EHandState::HAND_HAVE:
		haveUpdate(data);
		break;
	case EHandState::HAND_USE:
		useUpdate(data);
		break;
	case EHandState::HAND_GRASP:
		graspUpdate(data);
		break;

	default:
		break;
	}

	//�A�C�e���������Ă�Ƃ�Item�̍��W�X�V
	if (m_pItem != NULL)
	{
		m_pItem->itemHave(m_pHandColl->getGlobalPos(), m_pHandColl->getGlobalQuoRot());
	}
	//��������Ă��Ȃ���΃t���[�ɂ���
	if (data.grasp == false)
	{
		m_eState = EHandState::HAND_FREE;
		m_pHandColl->changeObjectTag(EPmObjectTag::OBJ_HANDGRASP, false);
	}
	m_HysPos = data.pos;
	m_HysRot = data.handRot;
}

//�`��
void CHand::render()
{
	CMatrix44 mat;
	mat.SetTranslation(m_pHandColl->getGlobalPos());
	
	CMatrix44 rot;
	m_pHandColl->getGlobalQuoRot().ConvertMatrixTranspose(rot);
	//rot.RotationY(MOF_ToRadian(-90));

	CMatrix44 sca;
	sca.Scaling(MODELSCALE);

	//���f���`��
	m_Mesh.Render(sca * rot * mat);

}

//�f�o�b�O�p�`��
void CHand::debugRender()
{
	switch (m_eState)
	{
	case EHandState::HAND_FREE:
		PmRenderShape::RenderShape(m_pHandColl, Mof::Color(1, 0, 0, 0.5f));
		CGraphicsUtilities::RenderString(10, 10, "%s", "FREE");
		break;
	case EHandState::HAND_HAVE:
	case EHandState::HAND_GRASP:
		PmRenderShape::RenderShape(m_pHandColl, Mof::Color(0, 1, 0, 0.5f));
		if (m_eState == EHandState::HAND_HAVE) CGraphicsUtilities::RenderString(10, 10, "%s", "HAVE");
		else CGraphicsUtilities::RenderString(10, 10, "%s", "GRASP");
		break;
	case EHandState::HAND_USE:
		PmRenderShape::RenderShape(m_pHandColl, Mof::Color(0, 0, 1, 0.5f));
		CGraphicsUtilities::RenderString(10, 10, "%s", "USE");
		break;

	default:
		break;
	}

	////��̎p�����`��
	//CVector3 rot,v(0,0,1);
	//rot = m_pHandColl->getGlobalVecRot();
	//v.RotationZ(rot.z);
	//v.RotationY(rot.y);
	//v.RotationX(rot.x);
	//CGraphicsUtilities::RenderLine(m_pHandColl->getGlobalPos(), m_pHandColl->getGlobalPos() + v * 2, MOF_XRGB(0, 1, 0));
	//
	////������
	//CVector3 vec = m_pHandColl->getGlobalPos();
	//vec.y /= v.y;
	//CVector3 test = v * abs(vec.y);

	//test += m_pHandColl->getGlobalPos();
	//CMatrix44 mat;
	//mat.SetTranslation(test);
	//CGraphicsUtilities::RenderBox(mat);
}

///////////////////////////////////////
//��̏�Ԃɉ���������

void CHand::freeUpdate(HandData& data)
{
	//����
	if (data.grasp == true)
	{
		//���������ɃA�C�e��������Ύ���
		if (m_pSearch->getItem() != NULL)
		{
			m_pItem = m_pSearch->getItem();
			m_eState = EHandState::HAND_HAVE;
		}
		//������邾��
		else
		{
			m_eState = EHandState::HAND_GRASP;
			m_pHandColl->changeObjectTag(EPmObjectTag::OBJ_HANDGRASP, true);
		}
	}
}

void CHand::haveUpdate(HandData& data)
{
	if (data.grasp == false)
	{
		m_pItem->itemLost(data.pos - m_HysPos,data.handRot - m_HysRot);
		m_pItem = NULL;
	}
}

void CHand::useUpdate(HandData& data)
{

}

void CHand::graspUpdate(HandData& data)
{

}