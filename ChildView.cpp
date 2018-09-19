// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Visualizacion2D.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	dib=new Dibujo;
	dib->Load("grf.grf");
	zoomw=false;
	zoom_d=500;
}

CChildView::~CChildView()
{
	delete dib;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
	dib->Draw(&dc,viewpipeline);
	if(zoomw){
	   //dibujamos un rectangulo en la posicion del puerto
		//de vista del zoom;
		dc.SelectStockObject(BLACK_PEN);
		dc.SelectStockObject(WHITE_BRUSH);
		dc.Rectangle(vpzoom.GetViewport());
		//dibujamos el dibujo nuevamente con el viewpipeline del zoom
        dib->Draw(&dc,vpzoom);
		dc.SelectStockObject(DC_PEN);
		dc.SetDCPenColor(RGB(128,128,128));
		dc.SelectStockObject(HOLLOW_BRUSH);
		CRect zr=vpzoom.GetWindow();
		CPoint tl=viewpipeline.WorldToDevice(zr.TopLeft());
		CPoint br=viewpipeline.WorldToDevice(zr.BottomRight());
		dc.Rectangle(tl.x,tl.y,br.x,br.y);
		//
	}
}


void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect nvp(0,0,cx,cy);
	if(cx>cy){
		nvp.left=(cx-cy)/2;
		nvp.right=cx-nvp.left;
	}
	if(cy>cx){
		nvp.top=(cy-cx)/2;
		nvp.right=cy-nvp.top;
	}

	viewpipeline.SetViewport(nvp);
	if(zoomw){
        CRect pv; //para el puerto de vista del zoom
	    pv.left=cx-300;
	    pv.right=pv.left+250;
	    pv.top=50;
		pv.bottom=300;
		vpzoom.SetViewport(pv);
	}
	RedrawWindow();
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//el click sirve para marcar una esquina del area de zoom
	p1=point;
	//la segunda esquina comienza en ese mismo punto
	//y se modifica conforme se arrastra el mouse.
	//Termina de modificarse al soltar el boton del mouse
	p2=point;
	//dibujamos el rectangulo del area de zoom inicial en modo XOR
	//color blanco (lo blanco se hara negto, y lo negro blanco)
	CDC *dc=GetDC();
	dc->SelectStockObject(HOLLOW_BRUSH);
	dc->SelectStockObject(DC_PEN);
	dc->SetDCPenColor(RGB(255,255,255));
	//modificamos modo de dibujo a XOR
	dc->SetROP2(R2_XORPEN);
	dc->Rectangle(p1.x,p1.y,p2.x,p2.y);
	ReleaseDC(dc);

}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
    //verificamos si se esta arrastrando el raton.
	//(si se esta esta presionando el boton izquierdo)
	
	if(nFlags&MK_LBUTTON){
		//antes de actualizar la posicion final, borramos el rectangulo
		//que se habia dibujado. Se dibuja el rectangulo blanco en
		//modo XOR
        CDC *dc=GetDC();
		dc->SelectStockObject(HOLLOW_BRUSH);
		dc->SelectStockObject(DC_PEN);
	    dc->SetDCPenColor(RGB(255,255,255));
	    //modificamos modo de dibujo a XOR
	    dc->SetROP2(R2_XORPEN);
		dc->Rectangle(p1.x,p1.y,p2.x,p2.y);
		//actualizamos la posicion final del area de zoom
		p2=point;
		//dibujamos un rectangulo
		dc->Rectangle(p1.x,p1.y,p2.x,p2.y);
     	
		ReleaseDC(dc);
		return;
	}
	if(zoomw){
		this->SetZoomArea(point);
		RedrawWindow();
		
	}
    p2=point;

	
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Ad your message handler code here and/or call default
    //actualizamos posicion final de zoom, y ajustamos la vetnada
	//para hacer el zoom
	if(p1==p2)
		return;
	CPoint p1w=viewpipeline.DeviceToWorld(p1);
	CPoint p2w=viewpipeline.DeviceToWorld(p2);
	CRect newwindow(p1w.x,p1w.y,p2w.x,p2w.y);
    //veriificamos ventana
	if(p1w.x>p2w.x){
		newwindow.left=p2w.x;
		newwindow.right=p1w.x;
	}
	if(p2w.y>p1w.y){
		newwindow.top=p2w.y;
		newwindow.bottom=p1w.y;
	}
	//verificamos las proporciones de la ventana.
	int dx=newwindow.right-newwindow.left;
	int dy=newwindow.top-newwindow.bottom;
	if(dx>dy){
		//ajustamos la dimension de la nueva ventana en y
		int pm=newwindow.bottom+dy/2;
		newwindow.bottom=pm-dx/2;
		newwindow.top=pm+dx/2;
	}
	if(dy>dx){
		int pm=newwindow.left+dx/2;
        newwindow.left=pm-dy/2;
		newwindow.right=pm+dy/2;
	}	
	//actualizamos la ventana
	viewpipeline.SetWindow(newwindow);
	RedrawWindow();
	
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	//CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	bool adj_w=false;
//
	switch(nChar){
		case 'z':
		case 'Z':
			zoomw=!zoomw;
	        if(zoomw){
                //definimos el puerto de vista para zoom
		        CRect av;
		        GetWindowRect(av); //obtiene el area de la ventana
                CRect pv; //para el puerto de vista del zoom
		        pv.left=av.Width()-300;
		        pv.right=pv.left+250;
		        pv.top=50;
		        pv.bottom=300;
		        vpzoom.SetViewport(pv);
				adj_w=true;
			}
			RedrawWindow();
		    break;
		case VK_OEM_PLUS:
		
			zoom_d/=2.0f;RedrawWindow();adj_w=true;break;
		case VK_OEM_MINUS:
            zoom_d*=2.0f;RedrawWindow();adj_w=true;break;

	}
	if(adj_w){
		CURSORINFO ci;
		ci.cbSize=sizeof(CURSORINFO);
		
		GetCursorInfo(&ci);
		//CPoint(ci.ptScreenPos)//
		SetZoomArea(p2);
	}
	RedrawWindow();
}

void CChildView::SetZoomArea(CPoint p)
{
	//convertimos la posicion del mouse a coordenadas mundiales
		CPoint mw=viewpipeline.DeviceToWorld(p);
		//defino la ventana para el area de zoom
		CRect zw(mw.x-zoom_d,mw.y+zoom_d,mw.x+zoom_d,mw.y-zoom_d);
		vpzoom.SetWindow(zw);
	    //RedrawWindow();
}
