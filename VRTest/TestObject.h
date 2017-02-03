////////////////////////
//test

#pragma once

#include "PmGameSummary.h"
#include "Camera.h"
#include "PmRender.h"

#include	"PmObjectTags.h"
#include	"CWrench.h"

class capsuleSearch : public PmCollisionSearch
{
public:
	int count = 0;

	capsuleSearch(){};
	~capsuleSearch(){};

	void onTrigger(PmObject* obj)
	{
		count++;
	}

	void onExit(PmObject* obj)
	{
		//count++;
	};
};

class testSearch : public PmCollisionSearch
{
public:
	int count = 0;

	testSearch(){};
	~testSearch(){};

	void onTrigger(PmObject* obj)
	{
		count++;
	}

	void onContact(PmObject* obj)
	{
		count++;
	}
};

class CameraPmTest : public PmGameSummary
{
private:
	TestCamera m_Camera;
public:
	CameraPmTest(){};

	virtual ~CameraPmTest(){};

	virtual void Create()
	{

	}

	virtual void Initialize()
	{
		m_Camera.Initialize();
	}

	virtual void Update()
	{
		m_Camera.Update();
	};

	virtual void RearUpdate(){};

	virtual void Render()
	{
	};

	virtual void RearRender(){};

	virtual void DebugRender(){};

	virtual void Release(){};
};

class PmTest : public PmGameSummary
{ 
private:
	PmObjectDynamic*	obj;
	PmObject*			floor;
	PmObject*			hasira[4];
	PmObject*			w;

	CMeshContainer		mesh;
	CMeshContainer		mesh2;

	CVector3 pos;

public:
 	PmTest(){};
	~PmTest(){};

	virtual void Create()
	{


		PmPhysxCreateTool* tool = this->createPhysxCreateTool();

		////マテリアル
		PxMaterial* mate;
		PxMaterial* boxMat = tool->createMaterial(CVector3(0.8f, 0.8f, 0.8f));
		//マテリアル作成
		mate = tool->createMaterial(CVector3(0.5f, 0.5f, 0.1f)); 
		//床作成
		floor = createBoxObject(CVector3(0, -2, 0), mate, 100, 1, 100, false);
		floor->setCollision(1, false);
		//floor = createTriangleObject(CVector3(0, 0, 0), mate, mesh, CVector3(OBJALLSCALE, OBJALLSCALE, OBJALLSCALE));

		//createBoxObject(CVector3(0, -1, 0), mate, 100, 1, 100, false);
		//w->setCollision(1, true);

		//hasira[0] = createBoxObject(CVector3(20, 0, 0), mate, 3, 10, 3, false);
		//hasira[1] = createBoxObject(CVector3(-20, 0, 0), mate, 3, 10, 3, false);
		//hasira[2] = createBoxObject(CVector3(0, 0, 30), mate, 3, 10, 3, false);
		//hasira[3] = createBoxObject(CVector3(0, 0, -30), mate, 3, 10, 3, false);	
		
		createPlane(CVector3(0, 0, 0),mate);

		if (!mesh.Load("submarine_in.mom"))
		{
			ERMS("MeshLoadError");
		}
		if (!mesh2.Load("monitorroom.mom"))
		{
			ERMS("MeshLoadError");
		}

		delete tool;

		pos = CVector3(0, 0, 0);

	}
	 
	virtual void Initialize()
	{



	}

	virtual void Update()
	{
		if (g_pInput->IsKeyHold(MOFKEY_SPACE))
		{
			if (g_pInput->IsKeyHold(MOFKEY_UP))
			{
				pos.z += 0.1f;
			}
			if (g_pInput->IsKeyHold(MOFKEY_DOWN))
			{
				pos.z -= 0.1f;
			}
			if (g_pInput->IsKeyHold(MOFKEY_LEFT))
			{
				pos.x -= 0.1f;
			}
			if (g_pInput->IsKeyHold(MOFKEY_RIGHT))
			{
				pos.x += 0.1f;
			}
		}

		if (g_pInput->IsKeyHold(MOFKEY_B))
		{
			CVector3 pos = floor->getGlobalPos();
			pos.y += 0.01f;
			floor->setGlobalPos(pos);
		}
	}

	virtual void RearUpdate()
	{

	}

	virtual void Render()
	{

		//方向が分かる用の箱４つ描画
		//PmRenderShape::RenderShape(hasira[0], Color(1, 0, 0, 1));
		//PmRenderShape::RenderShape(hasira[1], Color(0, 1, 0, 1));
		//PmRenderShape::RenderShape(w, Color(0, 0, 1, 1));
		//PmRenderShape::RenderShape(hasira[3], Color(1, 0, 1, 1));

		CMatrix44 mat;
		CMatrix44 sca;
		MofFloat scl = 0.3f;
		sca.Scaling(CVector3(scl, scl, scl));
		mat.SetTranslation(CVector3(0, 0, 0));
		mesh.Render(sca * mat);	
		CMatrix44 rot;
		rot.RotationY(MOF_ToRadian(180));
		mat.SetTranslation(CVector3(10, 0, 0));
		//mesh2.Render(rot * sca * mat);
		
		//床描画
		PmRenderShape::RenderShape(floor, Mof::Color(0, 0, 0.8f, 0.3f));
	}

	virtual void RearRender()
	{

	}

	virtual void DebugRender()
	{

	}

	virtual void Release()
	{
		mesh.Release();
	}
};