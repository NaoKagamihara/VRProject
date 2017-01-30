//////////////////////////////
//�����o���̓����蔻��̃T�[�`��`

#include		"CDrawerCollSearch.h"

void CDrawerCollSearch::onTrigger(PmObject* obj)
{
	//�E�肩����Ȃ�
	if (obj->checkObjectTag(EPmObjectTag::OBJ_LEFTHAND) || obj->checkObjectTag(EPmObjectTag::OBJ_RIGHTHAND))
	{
		//�����Ȃ��Ƃ�
		if (m_pHand == NULL)
		{
			//�肪�������ċ����Ԃɂ���
			m_pHand = obj;
			//�������Ă����Ԃɂ���Ƃ��ɉ��������Ă��Ȃ��Ȃ�͂߂�悤�ɂ���
			if (!m_pHand->checkObjectTag(EPmObjectTag::OBJ_HANDGRASP))
			{
				//������Ԃɂ���
				m_Pos = m_pHand->getGlobalPos();
				m_Poss = true;
			}

		}
		//�����Ȃ���Ԃɂ���
		else if (m_pHand == obj)
		{
			m_pHand = NULL;
			m_Poss = false;
			m_On = false;
		}

	}
}

void CDrawerCollSearch::update()
{
	//�肪�������ċ��鎞��
	if (m_pHand != NULL)
	{
		if (m_Poss == true)
		{
			m_dPos = m_Pos;
			m_Pos = m_pHand->getGlobalPos();
			
			//����t���O�Ȃ爬��
			if (m_pHand->checkObjectTag(EPmObjectTag::OBJ_HANDGRASP))
			{
				m_On = true;
			}		
			else m_On = false;
		}

	}
}