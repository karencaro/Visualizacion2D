// Visualizacion2D.h : main header file for the Visualizacion2D application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CVisualizacion2DApp:
// See Visualizacion2D.cpp for the implementation of this class
//

class CVisualizacion2DApp : public CWinApp
{
public:
	CVisualizacion2DApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CVisualizacion2DApp theApp;