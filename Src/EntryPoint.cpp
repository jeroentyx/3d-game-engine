#include "Engine\Engine\CoreManager.h"
#include "Src\Engine\Systems\Graphics\EosRenderer\ForceGraphicCard.h"
#include "Src\Engine\Systems\Graphics\EosRenderer\FileWatcher.h"

//#include <Windows.h>		// For no console mode
EOS_GPU

// To change back to no console
/*
Go to linker settings and find the subsystem settings
Change: Console -> Windows for both debug and release mode
*/

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main()
{
	//	
	//	Entry point for the editor
	//
	//UNREFERENCED_PARAMETER(hInstance);
	//UNREFERENCED_PARAMETER(hPrevInstance);
	//UNREFERENCED_PARAMETER(pCmdLine);
	//UNREFERENCED_PARAMETER(nCmdShow);

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//
	//	Doing test file here
	//

	

	CoreManager core;
	core.Init();
	core.Update();
	core.Exit();
	return 0;
}