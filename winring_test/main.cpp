#include <windows.h>

//#include <iostream>

#include "..\winring\OlsDef.h"

#ifdef RUN_TIME_DYNAMIC_LINKING
#include "..\winring\OlsApiInit.h"
#else // for Load-Time Dynamic Linking
#include "..\winring\OlsApi.h"
#ifdef _M_X64
#pragma comment(lib, "WinRing0x64.lib")
#else
#pragma comment(lib, "WinRing0.lib")
#endif
#endif


#ifdef RUN_TIME_DYNAMIC_LINKING
#include "..\winring\OlsApiInitExt.h"
#else // for Load-Time Dynamic Linking
#include "..\winring\OlsApi.h"
#endif

#ifdef RUN_TIME_DYNAMIC_LINKING
HMODULE m_hOpenLibSys;
#endif

void GetWinRing0dllStates()
{
	switch (GetDllStatus())
	{
	case OLS_DLL_NO_ERROR:
		break;
	case OLS_DLL_UNSUPPORTED_PLATFORM:
		MessageBox(NULL, L"DLL Status Error!! UNSUPPORTED_PLATFORM", NULL, NULL);
		return;
		break;
	case OLS_DLL_DRIVER_NOT_LOADED:
		MessageBox(NULL, L"DLL Status Error!! DRIVER_NOT_LOADED", NULL, NULL);
		return;
		break;
	case OLS_DLL_DRIVER_NOT_FOUND:
		MessageBox(NULL, L"DLL Status Error!! DRIVER_NOT_FOUND", NULL, NULL);
		return;
		break;
	case OLS_DLL_DRIVER_UNLOADED:
		MessageBox(NULL, L"DLL Status Error!! DRIVER_UNLOADED", NULL, NULL);
		return;
		break;
	case OLS_DLL_DRIVER_NOT_LOADED_ON_NETWORK:
		MessageBox(NULL, L"DLL Status Error!! DRIVER_NOT_LOADED_ON_NETWORK", NULL, NULL);
		return;
		break;
	case OLS_DLL_UNKNOWN_ERROR:
	default:
		MessageBox(NULL, L"DLL Status Error!! UNKNOWN_ERROR", NULL, NULL);
		return;
		break;
	}


}

int main()
{
	//std::cout << "find start! \n";

#ifdef RUN_TIME_DYNAMIC_LINKING
	m_hOpenLibSys = NULL;
	if (!InitOpenLibSys(&m_hOpenLibSys))
	{
		MessageBox(NULL, L"DLL Load Error!!", NULL, NULL);
		return FALSE;
	}
#else
	if (!InitializeOls())
	{
		MessageBox(NULL, L"Error InitializeOls()!!", NULL, NULL);
		return FALSE;
	}
#endif

	GetWinRing0dllStates();

	/*

	bool isFind;
	HWND pFindWnd;
	while (1)
	{
	pFindWnd = ::FindWindow(NULL, L"explorer.exe - ϵͳޯ٦");
	if (pFindWnd != NULL)
	{
	isFind = true;
	if (WriteIoPortByteEx(0x80, 0x55))
	{
	std::cout << "I/O write successfully! By error! \n";
	}
	break;
	}
	if (GetAsyncKeyState(VK_SHIFT))
	{
	isFind = false;
	if (WriteIoPortByteEx(0x80, 0x55))
	{
	std::cout << "I/O write successfully! By Mannual! \n";
	}
	break;
	}
	}
	if (isFind)
	{
	std::cout << "find yes ! \n";
	}
	else
	{
	std::cout << "find no ! \n";
	}
	std::cout << "find end! \n";
	*/
	/*
	unsigned char data[16];
	unsigned int size = 0;
	ReadPhysicalMemory(
		0x40000000,	// Physical Memory Address
		data,		// Buffer
		16,		// Count
		1		// Unit Size (BYTE, WORD, DWORD)
	);
	*/
	//WriteIoPortByteEx(0x80, 0x55);
#ifdef RUN_TIME_DYNAMIC_LINKING
	DeinitOpenLibSys(&m_hOpenLibSys);
#else
	DeinitializeOls();
#endif

	system("pause");
	return 0;
}