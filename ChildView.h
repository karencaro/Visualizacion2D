// ChildView.h : interface of the CChildView class
//


#pragma once
#include "Dibujo.h"

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
   Dibujo *dib;
   ViewPipeline viewpipeline;
   ViewPipeline vpzoom; //viewpipeline para zoom
   bool zoomw; //ventana de zoom activa
   //Declaramos variables para captura de area de zoom
   CPoint p1,p2;
   float zoom_d;//tamano de ventana para zoom
// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void SetZoomArea(CPoint p);
};

