#pragma once

#include <vector>
using namespace std;
class CViewPipeline;
class CPoligono
{
 protected:	
	vector<CPoint> m_vertices;
	vector<CPoint> m_vertices_originales;
	int m_closed;
	COLORREF m_color;
	CPoint min,max; //valor minimo y maximo de x y y en el polihono
    CPoint center; //el centro del poligono
public:
	CPoligono(int closed=1);
	~CPoligono(void);
	void AddVertex(CPoint point);
	CPoint &operator[](unsigned int index);
	CPoint &GetVertex(unsigned int index);
	int IsClosed(){return m_closed;}
	void SetColor(COLORREF color){m_color=color;};
	void SetClosed(int closed){m_closed=closed;};
	int Size(){return (int)m_vertices.size();}
	int Save(FILE * file);
	int Load(FILE * file);
	void CalculateMinMax(void);
	CPoint GetMin(){return min;} 
	CPoint GetMax(){return max;}
	CPoint GetCenter(){return center;}
	COLORREF GetColor(){return m_color;};
	int PuntoDentroDeArea(CPoint p);
	float MatrizTransformacion[3][3];
	void AplicarTransformacion();

	
	void Draw(CDC * pDC);
	void Draw(CDC * pDC, CViewPipeline * v_pipeline);

};
