#include "StdAfx.h"
#include "ViewPipeline.h"
#include<algorithm>
#define DERECHA 0x01
#define IZQUIERDA 0x02
#define ARRIBA 0x04
#define ABAJO 0x08
using std::swap;
ViewPipeline::ViewPipeline(void)
{
	//ventana default
	w.left=-10000;
	w.top=10000;
	w.right=10000;
	w.bottom=-10000;
	//puerto de vista default
	vp.left=0;
	vp.top=0;
	vp.right=640;
	vp.bottom=480;
	//calculamos factores de escalamiento
	sx=(float)(vp.right-vp.left)/(float)(w.right-w.left);
	sy=(float)(vp.top-vp.bottom)/(float)(w.top-w.bottom);

}

ViewPipeline::~ViewPipeline(void)
{
}

void ViewPipeline::SetViewport(CRect nvp)
{
	vp=nvp;
	//al cambiar el viewport o la ventana, la proporcion
	//entre ambos cambia, es necesario recalcular factores
	//de escalamiento
	sx=(float)(vp.right-vp.left)/(float)(w.right-w.left);
	sy=(float)(vp.top-vp.bottom)/(float)(w.top-w.bottom);
}

// Transforma p en coordenadas mundiales a coordenadas de dispositivo
CPoint ViewPipeline::WorldToDevice(CPoint p)
{
	CPoint pp;
	pp.x= (float)(p.x-w.left)*sx+vp.left;
	pp.y= (float)(p.y-w.bottom)*sy+vp.bottom;
	
	return pp;
}
// Transforma p en coordenadas dispositivo a coordenadas mundiales
CPoint ViewPipeline::DeviceToWorld(CPoint p)
{
	CPoint pp;
	pp.x= (float)(p.x-vp.left)/sx+w.left;
	pp.y= (float)(p.y-vp.bottom)/sy+w.bottom;
	
	return pp;
}

// modifica la ventana del viewing pipeline
void ViewPipeline::SetWindow(CRect nw)
{
	w=nw;
	//al cambiar el viewport o la ventana, la proporcion
	//entre ambos cambia, es necesario recalcular factores
	//de escalamiento
	sx=(float)(vp.right-vp.left)/(float)(w.right-w.left);
	sy=(float)(vp.top-vp.bottom)/(float)(w.top-w.bottom);
}


bool ViewPipeline::ClipLine(CPoint & p1, CPoint & p2)
{
	int terminar=0;
	char codigo1=0;
	char codigo2=0;
	while(terminar!=1)
	{
		codigo1=Codificacion(p1);
		codigo2=Codificacion(p2);
		if(codigo1|codigo2==0)
		{
			return true;
		}
		if(codigo1&codigo2!=0)
		{
			return false;
		}
		if (codigo1==0)
		{
			//swap x1<->x2
			//swap y1<->y2
			//swap codigo1<->codigo2
			swap<long>(p1.x,p2.x);
			swap<long>(p1.y,p2.y);
			swap<char>(codigo1,codigo2);
		}
		//verificar codigo1 para saber que reecorte hacer y recortar 
		if (codigo1&codigo2)
		{
			return false;
		}
		if(codigo1&IZQUIERDA)
		{
			float m =(float)(p1.y-p1.y)/(p2.x-p1.x);
			p1.y=(w.left-p1.x)*m+p1.y;
			p1.x=w.left;
			
		}
		if(codigo1&DERECHA)
		{
			float m =(float)(p2.y-p1.y)/(p2.x-p1.x);
			p1.y=(w.bottom-p1.y)*m+p1.x;
			p1.x=w.bottom;
			
			
		}

		if(codigo1&ARRIBA)
		{
			float m =(float)(p2.y-p1.y)/(p2.x-p1.x);
			p1.x=p1.x+(w.top-p1.y)/m;
			p1.y=w.top;
		}
		if(codigo1&ABAJO)
		{
			float m =(float)(p2.y-p1.y)/(p2.x-p1.x);
			p1.x=p1.x+(w.right-p1.y)/m;
			p1.y=w.right;
		}
		//encontrar nueva x1 y y1
	}
	//Implementar el algoritmo de recorte de linea.
	//p1 y p2 son referencias, por lo que cualquier modificacion que se
	//haga implica que se esta modificano el objeto enviado como parametro,
	//es decir, no se pierde la modificacion por que no son copia
	return true;//devuelve verdadero si la linea es visible.
}
char ViewPipeline::Codificacion(CPoint & p)
{
	//DERECHA 0x01
	//IZQUIERDA 0x02
	// ARRIBA 0x04
	// ABAJO 0x08
	char codigo=0;
	if (p.x<w.left)
	{
		codigo=codigo|IZQUIERDA;
	}
	if(p.x>w.right)
	{
		codigo=codigo|DERECHA;
	}
	if(p.y>w.top)
	{
		codigo=codigo|ARRIBA;
	}
	if(p.y<w.bottom)
	{
		codigo=codigo|ABAJO;
	}
	return codigo;
}
