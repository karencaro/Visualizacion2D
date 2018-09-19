#pragma once
#include"Poly.h"
#include<vector>
using std::vector;

class Dibujo
{
	vector<Poly> polis;
public:
	Dibujo(void);
	~Dibujo(void);
	void Draw(CDC * pDC);
	// Lee un archivo y carga el dibujo en el
	bool Load(char * filename);
	void Draw(CDC * pDC, ViewPipeline vpl);
};
