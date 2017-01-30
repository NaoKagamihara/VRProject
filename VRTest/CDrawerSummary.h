
//////////////////////////////////////
//ひきだし管理クラス

//INCLUDE
#include	"PmGameSummary.h"
#include	"PmRender.h"
#include	"PmObjectTags.h"
#include	"CommonDefine.h"
#include	"CDrawerCollSearch.h"

//DEFINE	
#define		DRAWERBONESTART		3			//ボーン開始番号
#define		DRAWERNUM			5			//ひきだしの数
#define		DRAWERLENX			7			//引き出しの壁の大きさx
#define		DRAWERLENY			1			//引き出しの壁の大きさy
#define		DRAWERLENZ			35			//引き出しの壁の大きさz
#define		DRAWERSIDEX			4			//ひきだしの手前の大きさx
#define		DRAWERSIDEY			4			//ひきだしの手前の大きさy
#define		DRAWERSIDEZ			0.3f		//ひきだしの手前の大きさz
#define		DRAWERHANDLEBONE	3			//引き出しのハンドルのボーン番号
#define		DRAWERLEN			9.5f		//ひきだしの長さ

//ひきだし管理クラス
class CDrawerSummary : public PmGameSummary
{
private:
	CVector3 m_Pos;		//ひきだし本体の座標
	CVector3 m_Rot;		//ひきだし本体の向き

	CMeshContainer		m_DrawerBody;		//ひきだし本体のモデル
	PmObject*			m_pDrawerBodyObj;	//ひきだし本体

	CMeshContainer		m_Drawer;						//ひきだしのモデル
	PmObjectDynamic*	m_pDrawerObj[DRAWERNUM];		//ひきだし

	CMeshContainer		m_DrawerHandle;					//引き出しのハンドルのモデル
	PmObjectDynamic*	m_pDrawerHandleObj[DRAWERNUM];	//引き出しのハンドルの当たり判定
	CDrawerCollSearch*  m_pDrawerSearch[DRAWERNUM];		//引き出しの衝突サーチ
	CVector3			m_DrawerIniPos[DRAWERNUM];		//引き出しの初期座標
	CVector3			m_HandlePos;					//引き出しのボーンから取得したハンドルの距離

	PmObjectDynamic*	m_Test;
	int nb = 0;

public:
	CDrawerSummary(CVector3& pos, CVector3& rot);
	virtual ~CDrawerSummary(){};

	void Create();
	void Update();
	void RearUpdate();
	void Render();
	void DebugRender();
	void Release();
};