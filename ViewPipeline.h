#pragma once
#include "atltypes.h"
#include "Poligono.h"

class CViewPipeline
{
public:
	CViewPipeline(void);
	~CViewPipeline(void);
	
protected:
	CRect m_pdv;
	CRect m_ventana;
public:
	void SetWindow(CRect nueva_ventana);
	void SetViewport(CRect nuevo_pdv);
	CRect GetWindow(){return m_ventana;};
	CRect GetViewport(){return m_pdv;};
	CPoint World2Device(CPoint p);
protected:
	float sx,sy;
	void CalcularEscalamiento(void);
public:
	CPoligono World2Device(CPoligono poli);
	CPoint Device2World(CPoint p);
	int Encode(CPoint point);
	bool ClipLine(CPoint & p1, CPoint & p2);
};
