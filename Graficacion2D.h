// Graficacion2D.h : main header file for the Graficacion2D application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGraficacion2DApp:
// See Graficacion2D.cpp for the implementation of this class
//

class CGraficacion2DApp : public CWinApp
{
public:
	CGraficacion2DApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGraficacion2DApp theApp;