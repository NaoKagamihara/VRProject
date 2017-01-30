////////////////////////////////
//ƒŒƒ“ƒ`‚ğŠÇ—‚·‚éƒNƒ‰ƒX

#include	"CWrench.h"
#include	"PmGameSummary.h"

//“–‚½‚è”»’è‚Ì‘å‚«‚³
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