
//////////////////////////////////////
//ひきだし管理クラス定義

#include		"CDrawerSummary.h"

CDrawerSummary::CDrawerSummary(CVector3& pos, CVector3& rot)
{
	//引き出しの位置と姿勢を決定
	m_Pos = pos;
	m_Rot = rot;
}

void CDrawerSummary::Create()
{
	PmPhysxCreateTool* tool = this->createPhysxCreateTool();
	PxMaterial* mate = tool->createMaterial(CVector3(0.5f, 0.5f, 0.5f));

	//ひきだし本体のモデル読み込み
	if (!m_DrawerBody.Load("DrawerMom/drawer_Body.mom"))
		ERMS("DrawerBodyLoadError");
	m_pDrawerBodyObj = createTriangleObject(m_Pos, mate, m_DrawerBody, CVector3(OBJALLSCALE, OBJALLSCALE, OBJALLSCALE));
	m_pDrawerBodyObj->setGlobalRot(m_Rot);

	//ひきだし部分のモデル読み込み
	if (!m_Drawer.Load("DrawerMom/drawer.mom"))
		ERMS("DrawerLoadError");

	//引き出しのハンドルモデル読み込み
	if (!m_DrawerHandle.Load("DrawerMom/drawer_handle.mom"))
		ERMS("DrawerHandleLoadError");

	////////////////////////////////////////////////////////
	//引き出しのボーンからハンドルをセットする座標を取得する
	LPBoneArray handleBoneArray = m_Drawer.GetBone();
	LPBone handleBone = handleBoneArray->GetData(DRAWERHANDLEBONE);
	CMatrix44 bonePos = handleBone->GetMatrix();
	bonePos.GetTranslation(m_HandlePos);
	m_HandlePos *= OBJALLSCALE;
	m_HandlePos.z += 1 * OBJALLSCALE;			//微調整
	m_HandlePos.RotationY(m_Rot.y);				//本体の回転を加える

	//ボーン情報から引き出しを設置する座標を取得
	LPBoneArray boneArray = m_DrawerBody.GetBone();
	int boneNum = DRAWERBONESTART;
	for (int i = 0; i < DRAWERNUM; i++)
	{
		CVector3 pos;
		LPBone bone = boneArray->GetData(boneNum);	//ボーン取得
		CMatrix44 mat = bone->GetMatrix();			//行列取得
		mat.GetTranslation(pos);					//ベクトルに変換
		pos.z += 18;								//最奥にずらして設置
		pos.RotationY(m_Rot.y);						//本体の回転に合わせる
		pos *= OBJALLSCALE;							//大きさを合わせる
		pos += m_Pos;								//本体の座標に設置
		//本体の逆行列をかけた初期位置を取得
		CMatrix44 iniMat,iniMatInv;
		iniMat = m_pDrawerBodyObj->getActorMatrix44();
		iniMat.Inverse(iniMatInv);
		m_DrawerIniPos[i] = pos * iniMatInv;
		//設置する座標にオブジェクトを作成
		m_pDrawerObj[i] = (PmObjectDynamic*)createEmpObject(pos, mate, true);	//空のオブジェクトを作成
		m_pDrawerObj[i]->setGlobalRot(m_Rot);

		///////////////////////////////////
		//底壁を作成
		m_pDrawerObj[i]->setKinematic(true);		//固定オブジェに変更
		CVector3 size(DRAWERLENX, DRAWERLENY, DRAWERLENZ);
		size *= OBJALLSCALE;
		PxShape* boxShape = tool->createBox(size, mate);
		///////////////////////////////////////////////////////////////////////////////////////////
		PxVec3 boxPos(0.5f, -5.0f, 0);											//ボーン変更前
		boxPos *= OBJALLSCALE;
		boxShape->setLocalPose(PxTransform(boxPos));						
		///////////////////////////////////////////////////////////////////////////////////////////
		m_pDrawerObj[i]->addShape(*boxShape);

		//////////////////////////////////////////
		//左右の壁作成
		PxShape* shape;
		for (int tow = 0; tow < 2; tow++)
		{
			//横の壁作成
			shape = tool->createBox(size, mate);
			PxTransform tra;
			CMatrix44 rot;
			//回転
			if (tow % 2 == 0) rot.RotationZ(MOF_ToRadian(60));
			else rot.RotationZ(MOF_ToRadian(-60));
			CQuaternion cq;
			rot.GetRotation(cq);
			PxQuat pq(cq.x, cq.y, cq.z, cq.w);
			tra.q = pq;

			//座標
			///////////////////////////////////////////////////////////////////////////////////////////
			if (tow % 2 == 0) tra.p = PxVec3(-4.5f, -2.1f, 0);							//ボーン変更前
			else tra.p = PxVec3(4.5f, -2.1f, 0);
			tra.p *= OBJALLSCALE;
			///////////////////////////////////////////////////////////////////////////////////////////

			//アクターに追加
			shape->setLocalPose(tra);
			m_pDrawerObj[i]->addShape(*shape);

			///////////////////////////////
			//手前の壁作成
			PxShape* sideShape = tool->createBox(CVector3(DRAWERSIDEX, DRAWERSIDEY, DRAWERSIDEZ), mate);
			PxTransform sideTra = sideShape->getLocalPose();
			if (tow % 2 == 0) sideTra.p = PxVec3(0, 0, -17.5f);							//ボーン変更前
			else sideTra.p = PxVec3(0,0,17);
			sideTra.p *= OBJALLSCALE;

			//アクターに追加
			sideShape->setLocalPose(sideTra);
			m_pDrawerObj[i]->addShape(*sideShape);
		}

		//トリガー用のオブジェクト作成
		float sphereSize = 5 * OBJALLSCALE;
		m_pDrawerHandleObj[i] = (PmObjectDynamic*)createSphereObject(CVector3(0, 0, 0), mate, sphereSize, true);
		m_pDrawerHandleObj[i]->setKinematic(true);
		m_pDrawerHandleObj[i]->setTrigger(1, true);
		//ボーンの位置に移動させる
		CVector3 handleSetPos = m_HandlePos + m_pDrawerObj[i]->getGlobalPos();
		m_pDrawerHandleObj[i]->setGlobalPos(handleSetPos);
		m_pDrawerHandleObj[i]->setGlobalRot(m_Rot);
		//衝突検知クラスを作成して持たせる
		m_pDrawerSearch[i] = new CDrawerCollSearch;
		delete m_pDrawerHandleObj[i]->getCollisionSearch();
		m_pDrawerHandleObj[i]->setCollisionSearch(m_pDrawerSearch[i]);

		boneNum += 2;	//次のボーンの配列に進める
	}

	

	delete tool;
}

void CDrawerSummary::Update()
{
	LPBoneArray boneArray = m_Drawer.GetBone();
	//ボーン情報から引き出しを設置する座標を取得
	if (g_pInput->IsKeyPush(MOFKEY_N))
		if (nb != 0) nb--;
	if (g_pInput->IsKeyPush(MOFKEY_M))
		if (boneArray->GetArrayCount() > nb) nb++;

	for (int i = 0; i < DRAWERNUM; i++)
	{
		m_pDrawerSearch[i]->update();

		if (m_pDrawerSearch[i]->getOnGrasp())
		{
			//バルブのposeを取得する
			CMatrix44 mat = m_pDrawerBodyObj->getActorMatrix44();

			//逆行列を求める
			CMatrix44 matInv;
			mat.Inverse(matInv);

			//逆行列をかける
			CVector3 dPos = m_pDrawerSearch[i]->getdPos() * matInv;
			CVector3 pos = m_pDrawerSearch[i]->getPos() * matInv;
			CVector3 draPos = m_pDrawerObj[i]->getGlobalPos() * matInv;
			
			//移動量を求める
			draPos.z += pos.z - dPos.z;
			//移動制限
			if (m_DrawerIniPos[i].z <= draPos.z)
				draPos.z = m_DrawerIniPos[i].z;
			if (m_DrawerIniPos[i].z - DRAWERLEN >= draPos.z)
				draPos.z = m_DrawerIniPos[i].z - DRAWERLEN;

			//行列をもとに戻して動かす
			draPos *= mat;
			m_pDrawerObj[i]->setMovePos(draPos);
			
		}
	}
}

void CDrawerSummary::RearUpdate()
{
	for (int i = 0; i < DRAWERNUM; i++)
	{
		//ひきだしを動かせるときだけ
		if (m_pDrawerSearch[i]->getOnGrasp())
		{
			//ハンドルをひきだしの位置に合わせる
			CVector3 handlePos = m_pDrawerObj[i]->getGlobalPos() + m_HandlePos;
			m_pDrawerHandleObj[i]->setGlobalPos(handlePos);
		}
	}
}

void CDrawerSummary::Render()
{
	//ひきだし本体描画
	CMatrix44 matDrawerBody = m_pDrawerBodyObj->getMatrix44(1, CVector3(OBJALLSCALE, OBJALLSCALE, OBJALLSCALE));
	m_DrawerBody.Render(matDrawerBody);

	//ひきだし描画 && ハンドル描画
	CMatrix44 matDrawer;
	for (int i = 0; i < DRAWERNUM; i++)
	{
		matDrawer = m_pDrawerObj[i]->getActorMatrix44(CVector3(OBJALLSCALE, OBJALLSCALE, OBJALLSCALE));
		m_Drawer.Render(matDrawer);
		matDrawer = m_pDrawerHandleObj[i]->getActorMatrix44(CVector3(OBJALLSCALE, OBJALLSCALE, OBJALLSCALE));
		m_DrawerHandle.Render(matDrawer);
	}
}

void CDrawerSummary::DebugRender()
{

	for (int i = 0; i < DRAWERNUM; i++)
	{
		PmRenderShape::RenderShape(m_pDrawerObj[i], CVector4(1, 0, 0, 0.3f));
		PmRenderShape::RenderShape(m_pDrawerHandleObj[i], CVector4(0, 1, 0, 0.3f));
	}

	////ボーン情報から引き出しを設置する座標を取得
	//LPBoneArray boneArray = m_Drawer.GetBone();
	//LPBone bone = boneArray->GetData(nb);
	//CMatrix44 matPos = bone->GetMatrix();
	//CVector3 pos;
	//matPos.GetTranslation(pos);

	//CSphere sp(pos, 0.5f);
	//CGraphicsUtilities::RenderSphere(sp, CVector4(0, 1, 0, 1));
	//CGraphicsUtilities::RenderString(10,70,"%d",nb);
	//CGraphicsUtilities::RenderString(10, 90, "%s", bone->GetName()->GetString());
	//CMatrix44 mat;
	//CMatrix44 handleMat;
	//pos.z -= 1;
	//handleMat.SetTranslation(pos);
	//m_DrawerHandle.Render(handleMat);
	//m_Drawer.Render(mat);

	//CMatrix44 mat;
	//CSphere shape(pos,1);
	//m_Drawer.Render(mat);
	//CGraphicsUtilities::RenderSphere(mat);
}

void CDrawerSummary::Release()
{
	m_DrawerBody.Release();
}
