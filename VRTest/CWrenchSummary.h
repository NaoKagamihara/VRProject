////////////////////////////////
//�����`���Ǘ�����N���X

#include	"CWrench.h"
#include	"PmGameSummary.h"

//�����蔻��̑傫��
#define		SCALEX		1.0f
#define		SCALEY		1.0f
#define		SCALEZ		3.0f

class CWrenchSummary : public PmGameSummary
{
private:
	CItem*				m_pItem;

public:
	CWrenchSummary(){};
	~CWrenchSummary(){};

	virtual void Create();
	virtual void Render();
	virtual void DebugRender();
};