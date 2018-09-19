#pragma once
#include "Poligono.h"
#include <vector>
using namespace std;
class CDibujo
{
protected:
	vector<CPoligono> m_figuras;

public:
	

	CDibujo(void);
	~CDibujo(void);
	void AddPolygon(CPoligono poli);
	CPoligono & operator[](unsigned int index);
	int Save(CString filename);
	int Load(CString filename);
	int Size(){return m_figuras.size();};
};
