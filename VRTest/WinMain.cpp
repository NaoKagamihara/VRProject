#include	"GameApp.h"
#include	"Mof.h"


#ifdef		UNICODE
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#endif
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Mof::LPFramework pFrame = new Mof::CDX11GameFramework();
	Mof::WINDOWSGAMEFRAMEWORKINITIALIZEINFO Info;
	Info.pApplication = new CGameApp();
	Info.WindowCreateInfo.Width = 1512;
	Info.WindowCreateInfo.Height = 1680;
	//Info.GraphicsCreateInfo.RefreshRate = 90;
	Info.FPS = 0;
	Info.GraphicsCreateInfo.bMSAA = FALSE;
	pFrame->Initialize(&Info);
	pFrame->Run();
	MOF_SAFE_DELETE(pFrame);
	return 0;
}