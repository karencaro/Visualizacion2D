#include "StdAfx.h"
#include "Dibujo.h"
#include <stdio.h>

Dibujo::Dibujo(void)
{
}

Dibujo::~Dibujo(void)
{
}

void Dibujo::Draw(CDC * pDC)
{
	for(int i=0;i<polis.size();i++)
		polis[i].Draw(pDC);

}

// Lee un archivo y carga el dibujo en el
bool Dibujo::Load(char * filename)
{
	FILE *input;
	input=fopen(filename,"r");
	if(!input)
		return false;
	//el primer renglon del archivo tiene el numero
	//de poligonos
	int np;
	fscanf(input,"%d",&np);
	//leemos cada uno de los poligonos
	for(int i=0;i<np;i++){
		Poly poly;
		poly.Load(input);
		polis.push_back(poly);
	}
    fclose(input);
	return true;
}

void Dibujo::Draw(CDC * pDC, ViewPipeline vpl)
{
	for(int i=0;i<polis.size();i++)
		polis[i].Draw(pDC,vpl);
}
