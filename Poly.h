#pragma once
#include<stdio.h>
#include<vector>
#include"ViewPipeline.h"
using std::vector;

class Poly
{
	COLORREF color;
	int is_closed; //esta cerrado o abierto
	vector<CPoint> vertices;

public:
	Poly(void);
	~Poly(void);
	void Draw(CDC * pDC);
	void AddVertex(CPoint v);
	bool Load(FILE * input);
	void Draw(CDC * pDC, ViewPipeline vpl);
};
