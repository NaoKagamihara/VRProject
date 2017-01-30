//////////////////////////////
//引き出しの当たり判定のサーチ定義

#include		"CDrawerCollSearch.h"

void CDrawerCollSearch::onTrigger(PmObject* obj)
{
	//右手か左手なら
	if (obj->checkObjectTag(EPmObjectTag::OBJ_LEFTHAND) || obj->checkObjectTag(EPmObjectTag::OBJ_RIGHTHAND))
	{
		//何もないとき
		if (m_pHand == NULL)
		{
			//手が当たって居る状態にする
			m_pHand = obj;
			//当たっている状態にするときに何も握っていないなら掴めるようにする
			if (!m_pHand->checkObjectTag(EPmObjectTag::OBJ_HANDGRASP))
			{
				//握れる状態にする
				m_Pos = m_pHand->getGlobalPos();
				m_Poss = true;
			}

		}
		//何もない状態にする
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
	//手が当たって居る時に
	if (m_pHand != NULL)
	{
		if (m_Poss == true)
		{
			m_dPos = m_Pos;
			m_Pos = m_pHand->getGlobalPos();
			
			//握るフラグなら握る
			if (m_pHand->checkObjectTag(EPmObjectTag::OBJ_HANDGRASP))
			{
				m_On = true;
			}		
			else m_On = false;
		}

	}
}