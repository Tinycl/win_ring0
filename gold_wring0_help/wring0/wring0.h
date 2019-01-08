
// wring0.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cwring0App:
// See wring0.cpp for the implementation of this class
//

class Cwring0App : public CWinApp
{
public:
	Cwring0App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cwring0App theApp;