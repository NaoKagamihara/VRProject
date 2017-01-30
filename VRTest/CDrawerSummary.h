
//////////////////////////////////////
//�Ђ������Ǘ��N���X

//INCLUDE
#include	"PmGameSummary.h"
#include	"PmRender.h"
#include	"PmObjectTags.h"
#include	"CommonDefine.h"
#include	"CDrawerCollSearch.h"

//DEFINE	
#define		DRAWERBONESTART		3			//�{�[���J�n�ԍ�
#define		DRAWERNUM			5			//�Ђ������̐�
#define		DRAWERLENX			7			//�����o���̕ǂ̑傫��x
#define		DRAWERLENY			1			//�����o���̕ǂ̑傫��y
#define		DRAWERLENZ			35			//�����o���̕ǂ̑傫��z
#define		DRAWERSIDEX			4			//�Ђ������̎�O�̑傫��x
#define		DRAWERSIDEY			4			//�Ђ������̎�O�̑傫��y
#define		DRAWERSIDEZ			0.3f		//�Ђ������̎�O�̑傫��z
#define		DRAWERHANDLEBONE	3			//�����o���̃n���h���̃{�[���ԍ�
#define		DRAWERLEN			9.5f		//�Ђ������̒���

//�Ђ������Ǘ��N���X
class CDrawerSummary : public PmGameSummary
{
private:
	CVector3 m_Pos;		//�Ђ������{�̂̍��W
	CVector3 m_Rot;		//�Ђ������{�̂̌���

	CMeshContainer		m_DrawerBody;		//�Ђ������{�̂̃��f��
	PmObject*			m_pDrawerBodyObj;	//�Ђ������{��

	CMeshContainer		m_Drawer;						//�Ђ������̃��f��
	PmObjectDynamic*	m_pDrawerObj[DRAWERNUM];		//�Ђ�����

	CMeshContainer		m_DrawerHandle;					//�����o���̃n���h���̃��f��
	PmObjectDynamic*	m_pDrawerHandleObj[DRAWERNUM];	//�����o���̃n���h���̓����蔻��
	CDrawerCollSearch*  m_pDrawerSearch[DRAWERNUM];		//�����o���̏Փ˃T�[�`
	CVector3			m_DrawerIniPos[DRAWERNUM];		//�����o���̏������W
	CVector3			m_HandlePos;					//�����o���̃{�[������擾�����n���h���̋���

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