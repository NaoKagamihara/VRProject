/*************************************************************************//*!
	@file	PmGameObject.h
	@brief	PmGameSceneに追加する基底クラス

														@author	鏡原
														@date	2016/10/20 手直しとコメント
														@date	2016/10/24 createCharacterCapsuleObject引数変更
*//**************************************************************************/

//ONCE
#pragma once

//INCLUDE
#include	"CGameSummary.h"
#include	"PmManager.h"


/*******************************//*!
	@brief	PmObjectを管理する基底クラス

			PmManagerに作成PmObjectを管理、追加する基底クラス

*//********************************/
class PmGameSummary : public CGameSummary
{
private:
	PmObjectCreate* m_pCreate;			//!<PmObjectを追加する

public:

	/*************************************************************************//*!
		@brief			コンストラクタ
		@param			None

		@return			None
	*//**************************************************************************/
	PmGameSummary(){};

	/*************************************************************************//*!
		@brief			コンストラクタ
		@param[in]		mng				このクラスから追加される先のPmMnager			

		@return			None
	*//**************************************************************************/
	PmGameSummary(PmObjectCreate& create){ m_pCreate = &create; };

	/*************************************************************************//*!
		@brief			デストラクタ
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual ~PmGameSummary(){};

	/*************************************************************************//*!
		@brief			使用するObjectCreateをセットする
		@param[in]		create				このクラスから追加される先のPmObjectCreate			

		@return			None
	*//**************************************************************************/
	void setObjcetCreate(PmObjectCreate* create){ m_pCreate = create; };

	/*************************************************************************//*!
		@brief			作成

						PmGameSceneで一度だけ呼び出される<br>
						Loadや動的なメモリの確保に使用する
						
		@param			None

		@return			None
	*//**************************************************************************/
	virtual void Create(){};

	/*************************************************************************//*!
		@brief			初期化
						
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void Initialize(){};

	/*************************************************************************//*!
		@brief			更新

						物理エンジンを更新する前に呼ばれる
						
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void Update(){};

	/*************************************************************************//*!
		@brief			Updateの後に更新

						Updateの計算の計算を行い、物理エンジンの計算を行った後に<br>
						RearUpdateが呼ばれる
						
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void RearUpdate(){};

	/*************************************************************************//*!
		@brief			描画

						Update処理がすべて行われた後に呼ばれる
						
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void Render(){};

	/*************************************************************************//*!
		@brief			Renderの後に描画

						Renderがすべて呼ばれた後に呼ばれる
						
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void RearRender(){};

	/*************************************************************************//*!
		@brief			デバッグ用の描画
						
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void DebugRender(){};

	/*************************************************************************//*!
		@brief			メモリ解放を行う

						PmGameSceneに追加している場合自動で呼ばれる
						
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void Release(){};

	/*************************************************************************//*!
		@brief			PmPhysxCreateToolを作成する

						materialの作成
						PmManagerに追加されない状態でオブジェクトを<br>
						作成したい場合に使用する
						
		@param			None		

		@return			このクラスを管理しているPhysxで使用するための<br>
						作成ツールを返す
	*//**************************************************************************/
	PmPhysxCreateTool* createPhysxCreateTool();

	/******************************************************************************
	**PmObjectの作成(作成時にシーンに追加される)
	******************************************************************************/

	//追加
	void createPlane(const CVector3& vec, PxMaterial* material);

	/*************************************************************************//*!
		@brief			Geometryが空のPmObjectを作成する

						シーンに追加済みの形を持っていないActorを作成する
						
		@param[in]		pos			座標
		@param[in]		material	マテリアル
		@param[in]		dynamic		Actorの種類 : true 動的, false 静的

		@return			シーンに追加済みのオブジェクト
	*//**************************************************************************/
	PmObject* createEmpObject(const CVector3& pos, PxMaterial* material,const bool dynamic);

	/*************************************************************************//*!
		@brief			BoxGeometryを持つPmObjectの作成
						
		@param[in]		pos			座標
		@param[in]		material	マテリアル
		@param[in]		x			boxの中心からx方向の長さ
		@param[in]		y			boxの中心からy方向の長さ
		@param[in]		z			boxの中心からz方向の長さ
		@param[in]		dynamic		Actorの種類 : true 動的, false 静的

		@return			シーンに追加済みのオブジェクト
	*//**************************************************************************/
	PmObject* createBoxObject(const CVector3& pos, PxMaterial* material,const MofFloat x,const MofFloat y,const MofFloat z,const bool dynamic);

	/*************************************************************************//*!
		@brief			SphereGeometryを持つPmObjectの作成
						
		@param[in]		pos			座標
		@param[in]		material	マテリアル
		@param[in]		r			半径
		@param[in]		dynamic		Actorの種類 : true 動的, false 静的

		@return			シーンに追加済みのオブジェクト
	*//**************************************************************************/
	PmObject* createSphereObject(const CVector3& pos, PxMaterial* material,const MofFloat r,const bool dynamic);

	/*************************************************************************//*!
		@brief			CapsuleGeometryを持つPmObjectの作成
						
		@param[in]		pos			座標
		@param[in]		material	マテリアル
		@param[in]		r			半径
		@param[in]		h			高さ(中心から半径を除いた+-の高さ)
		@param[in]		dynamic		Actorの種類 : true 動的, false 静的

		@return			シーンに追加済みのオブジェクト
	*//**************************************************************************/
	PmObject* createCapsuleObject(const CVector3& pos, PxMaterial* material,const MofFloat r,const MofFloat h,const bool dynamic);

	/*************************************************************************//*!
		@brief			ConvexGeometryを持つPmObjectの作成

						メッシュからConvexMeshを作成しシーンに追加する
						
		@param[in]		pos			座標
		@param[in]		material	マテリアル
		@param[in]		mesh		メッシュ
		@param[in]		dynamic		Actorの種類 :true 動的, false 静的
		@param[in]		scale		拡大縮小	:デフォルト = CVector3(1.0f,1.0f,1.0f)

		@return			シーンに追加済みのオブジェクト
	*//**************************************************************************/
	PmObject* createConvexObject(const CVector3& pos, PxMaterial* material, CMeshContainer mesh,const bool dynamic,const CVector3& scale = CVector3(1.0f,1.0f,1.0f));


	/*************************************************************************//*!
	@brief			TriangleGeometryを持つPmObjectの作成

					メッシュからTriangleGeometryを作成しシーンに追加する
						
	@param[in]		pos			座標
	@param[in]		material	マテリアル
	@param[in]		mesh		メッシュ
	@param[in]		dynamic		Actorの種類 :true 動的, false 静的
	@param[in]		scale		拡大縮小	:デフォルト = CVector3(1.0f,1.0f,1.0f)

	@return			シーンに追加済みのオブジェクト
	*//**************************************************************************/
	PmObject* createTriangleObject(const CVector3& pos, PxMaterial* material, CMeshContainer mesh, const CVector3& scale = CVector3(1.0f, 1.0f, 1.0f));


	//PmObjectCharacterを作成する
	PmObjectCharacter* createCharacterCapsuleObject(CVector3 pos, float height, float radius, PxMaterial* material,
		float slopeLimit = MOF_ToRadian(45.0f),
		float contactOffset = 0.1f,
		float stepOffset = 0.02f);

	/*************************************************************************//*!
		@brief			PmObjectをシーンから外す
						
		@param[in]		obj		シーンから外すPmObject

		@return			None
	*//**************************************************************************/
	void detachObject(PmObject* obj);

	/*************************************************************************//*!
	@brief			CItemをシーンに追加する
						
	@param[in]		obj		シーンに追加するobj

	@return			None
	*//**************************************************************************/
	void createCItemObject(PmObjectDynamic* obj);
};