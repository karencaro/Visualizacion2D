#pragma once

class ViewPipeline
{
	CRect w; //ventana (el area que se quiere desplegar)
	CRect vp;//Puerto de vista (viewport)
	         //(En que parte del dispositivo se va a 
	         //desplegar)
	float sx,sy; //factores de escalamiento ventana/viewport

public:
	ViewPipeline(void);
	~ViewPipeline(void);
	void SetViewport(CRect nvp);
	// Transforma p en coordenadas mundiales a coordenadas de dispositivo
	CPoint WorldToDevice(CPoint p);
	CPoint DeviceToWorld(CPoint p);
public:
	// modifica la ventana del viewing pipeline
	void SetWindow(CRect nw);
	CRect GetWindow(){return w;};
	CRect GetViewport(){return vp;};
	bool ClipLine(CPoint & p1, CPoint & p2);
	char Codificacion(CPoint &p);
};
