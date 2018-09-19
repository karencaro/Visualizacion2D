#include "StdAfx.h"
#include "Poly.h"

Poly::Poly(void)
{
}

Poly::~Poly(void)
{
}

void Poly::Draw(CDC * pDC)
{
	if(vertices.size()<2)return;
	//dibujar el poligono
	//configuramos el color
	pDC->SelectStockObject(DC_PEN);
	pDC->SetDCPenColor(color);
    //unimos los vertices con lineas
	pDC->MoveTo(vertices[0]);
	for(int i=1;i<vertices.size();i++)
		pDC->LineTo(vertices[i]);
	if(is_closed)//poly cerrado, dibujamos linea para cerrarlo
		pDC->LineTo(vertices[0]);
}

void Poly::AddVertex(CPoint v)
{
	vertices.push_back(v);
}

bool Poly::Load(FILE * input)
{
	//lo primero en la especificacion de un poly es 
	//el numero de vertices
    int nv;
	fscanf(input,"%d",&nv);
	fscanf(input,"%d",&color);
	fscanf(input,"%d",&is_closed);
	//cargamos cada uno de los vertices
	for(int i=0;i<nv;i++){
		CPoint v;
		fscanf(input,"%d,%d",&v.x,&v.y);
		vertices.push_back(v);
	}
	return true;
}
/*
void Poly::Draw(CDC * pDC, ViewPipeline vpl)
{
	if(vertices.size()<2)return;
	//dibujar el poligono
	//configuramos el color
	pDC->SelectStockObject(DC_PEN);
	pDC->SetDCPenColor(color);
    //unimos los vertices con lineas
	//enviamos el resultado de la conversion del
	//vertice de coordenadas mundiales a dispositivo
	pDC->MoveTo(vpl.WorldToDevice(vertices[0]));
	for(int i=1;i<vertices.size();i++)
		pDC->LineTo(vpl.WorldToDevice(vertices[i]));
	if(is_closed)//poly cerrado, dibujamos linea para cerrarlo
		pDC->LineTo(vpl.WorldToDevice(vertices[0]));
}*/
void Poly::Draw(CDC * pDC, ViewPipeline vpl)
{
	if(vertices.size()<2)return;
	//dibujar el poligono
	//configuramos el color
	pDC->SelectStockObject(DC_PEN);
	pDC->SetDCPenColor(color);
    //unimos los vertices con lineas
	//enviamos el resultado de la conversion del
	//vertice de coordenadas mundiales a dispositivo
	//hacemos clip de las lineas que se dibujan
	CPoint pi,pf;
	int i;
	for(i=0;i<vertices.size()-1;i++){
		pi=vertices[i];
		pf=vertices[i+1];
		if(vpl.ClipLine(pi,pf)){
			pDC->MoveTo(vpl.WorldToDevice(pi));
			pDC->LineTo(vpl.WorldToDevice(pf));
		}
	}
		 
	if(is_closed){//poly cerrado, dibujamos linea para cerrarlo
		pi=vertices[i];
		pf=vertices[0];
		if(vpl.ClipLine(pi,pf)){
			pDC->MoveTo(vpl.WorldToDevice(pi));
			pDC->LineTo(vpl.WorldToDevice(pf));
		}
	}
}