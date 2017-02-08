
//バルブクラス定義

#include	"CValveSummary.h"


//////////////////////////////////////
//バルブ管理クラス
void CValveSummary::Create()
{
	PmPhysxCreateTool* tool = createPhysxCreateTool();

	//まてりある
	PxMaterial* mate;
	mate = tool->createMaterial(CVector3(0.5f, 0.5f, 0.5f));

	CVector3 mainPos(0, 10, 20);

	//当たり判定作成
	m_pCollision = (PmObjectDynamic*)createSphereObject(mainPos, mate, COLLISIONRADIUS, true);
	delete tool;

	//サーチを持たせる
	m_pValveSearch = new CHandCollSearch();
	m_pCollision->releaseCollisionSearch();
	m_pCollision->setCollisionSearch(m_pValveSearch);
	m_pCollision->setKinematic(true);
	m_pCollision->setTrigger(1, true);

	//モデル読み込み
	if (!m_ValuveMesh.Load("valve.mom"))
	{
		ERMS("valveLoad ERROR");
	}

	//////////////////////////////////////////////////////////
	//テスト用にここで定義

	//当たり判定の座標を変更
	PxShape* shape = m_pCollision->getShape(1);
	PxTransform tra = shape->getLocalPose();
	CVector3 vec;
	tra.p.x = 2.0f;
	shape->setLocalPose(tra);

	//向きを変更
	m_Rotation = CVector3(0, MOF_ToRadian(90), 0);
	m_pCollision->setGlobalRot(m_Rotation);


	if (!m_PipeMesh.Load("pipe/pipe.mom"))
	{
		ERMS("pipeLoad ERROR");
	}
	m_pPipeObj = createTriangleObject(CVector3(mainPos.x,mainPos.y - 1,mainPos.z), mate, m_PipeMesh, CVector3(OBJALLSCALE, OBJALLSCALE, OBJALLSCALE));
	m_pPipeObj->setGlobalRot(CVector3(0, MOF_ToRadian(90), 0));
	m_bprevflag = false;
}

void CValveSummary::Update()
{
	if (m_pValveSearch->getHandObj(RIGHTHAND) != NULL)
	{	
		//m_Rotation.x += MOF_ToRadian(1);
		//m_pCollision->setGlobalRot(m_Rotation);	
	}

	//壊れている間は水を追加
	if (m_bBreakFlag == true)
	{
		m_pInundation->addWater(DEFADDWATER);
	}
}

void CValveSummary::RearUpdate()
{
	//両手で回転させられるかテスト用
	bool flagGrasp = false;

	//左右の手で握っているときだけ回す
	PmObject* hand[HANDNUM] = { NULL };
	for (int i = 0; i < HANDNUM; i++)
	{
		//左右の手がバルブに当たっているか || 握っているか
		hand[i] = m_pValveSearch->getHandObj(i);
		if (hand[i] == NULL || !hand[i]->checkObjectTag(EPmObjectTag::OBJ_HANDGRASP))
		{
			flagGrasp = false;
			break;
		}
		flagGrasp = true;
	}

	//両手で握っていれば
	if (flagGrasp == true)
	{
		//バルブのposeを取得する
		CMatrix44 mat;

		//座標と姿勢を取得する
		PxTransform tra = m_pCollision->getActor()->getGlobalPose();

		///////////////////////////////////////
		//回転
		//グローバル回転取得
		//CQuaternion pq(tra.q.x, tra.q.y, tra.q.z, tra.q.w);
		//pq.ConvertMatrixTranspose(mat);
		mat.RotationZXY(CVector3(0, MOF_ToRadian(90), 0));

		//shapeのposeを取り出す
		PxShape* shape;
		m_pCollision->getActor()->getShapes(&shape, 1);
		//ローカル座標取得
		CVector3 lpv(shape->getLocalPose().p.x, shape->getLocalPose().p.y, shape->getLocalPose().p.z);

		//グローバルに合わせてローカルを回転させる
		lpv *= mat;

		///////////////////////////////////////
		//座標
		//globalを考慮したshapeの座標
		CVector3 pv(tra.p.x + lpv.x, tra.p.y + lpv.y, tra.p.z + lpv.z);

		//行列に変換
		mat.SetTranslation(pv);

		//逆行列を求める
		CMatrix44 matInv;
		mat.Inverse(matInv);

		//////////////////////////////////
		//両手の角度を求める
		CVector3 handVec[HANDNUM];
		for (int i = 0; i < HANDNUM; i++)
		{
			//各手に逆行列をかける
			handVec[i] = hand[i]->getGlobalPos();
			handVec[i] *= matInv;
		}
		//角度を計算
		//角度を求める
		float r = atan2f(handVec[RIGHTHAND].y - handVec[LEFTHAND].y, (handVec[RIGHTHAND].z - handVec[LEFTHAND].z));

		//手の動きに合わせてバルブを回転させる
		if (m_HisAng[LEFTHAND] != 1000.0f && m_bprevflag)
		{
			m_Rotation.x += m_HisAng[LEFTHAND] - r;
			m_pCollision->setGlobalRot(m_Rotation);
		}
		//座標を保存
		m_HisAng[LEFTHAND] = r;

		//回しきったら修理完了
		if (MOF_ToDegree(m_Rotation.x) >= 360)
		{
			m_bBreakFlag = false;
			m_Rotation.x = 0.0f;
		}

		//握っている場所に手を固定する
		for (int i = 0; i < HANDNUM; i++)
		{
			//手に逆行列をかける
			CVector3 vec = hand[i]->getGlobalPos();
			vec *= matInv;

			//角度を求める
			float r = atan2f(vec.y, vec.z);
			CVector3 npos(0, sinf(r) * COLLISIONRADIUS, cosf(r) * COLLISIONRADIUS);
			npos *= mat;

			//手をバルブに固定する
			hand[i]->setGlobalPos(CVector3(npos.x, npos.y, npos.z));
		}
	}
	m_bprevflag = flagGrasp;
}

void CValveSummary::Render()
{
	//
	CMatrix44 mat;
	mat.SetTranslation(m_pCollision->getGlobalPos());

	CMatrix44 sca;
	sca.Scaling(0.4f);

	CMatrix44 rot;
	m_pCollision->getGlobalQuoRot().ConvertMatrixTranspose(rot);
	
	//テスト
	CMatrix44 test;
	CMatrix44 test2;
	test2.SetTranslation(CVector3(2.0f, 0, 0));
	test *= test2;

	m_ValuveMesh.Render(sca * rot * mat);
	CMatrix44 pMat = m_pPipeObj->getActorMatrix44(CVector3(OBJALLSCALE, OBJALLSCALE, OBJALLSCALE));
	m_PipeMesh.Render(pMat);
}

void CValveSummary::DebugRender()
{
	PmRenderShape::RenderShape(m_pCollision, Mof::Color(1, 0, 0, 0.8f));

	CGraphicsUtilities::RenderString(10, 50, "%f",MOF_ToDegree(m_Rotation.x));
}

void CValveSummary::Release()
{
	m_ValuveMesh.Release();
}

void CValveSummary::gimmickBreak()
{
	m_bBreakFlag = true;
}