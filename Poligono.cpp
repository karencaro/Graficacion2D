#include "StdAfx.h"
#include "Poligono.h"
#include "ViewPipeline.h"

CPoligono::CPoligono(int isclosed)
{
	m_closed=isclosed;
	m_color=RGB(0,0,0);
	min=CPoint(100000000,100000000);
	max=CPoint(-100000000,-100000000);
}

CPoligono::~CPoligono(void)
{

}

void CPoligono::AddVertex(CPoint point)
{
	m_vertices_originales.push_back(point);
	m_vertices.push_back(point);
	if(point.x>max.x)max.x=point.x;
	if(point.x<min.x)min.x=point.x;
	if(point.y>max.y)max.y=point.y;
	if(point.y<min.y)min.y=point.y;
	center=CPoint(min.x+((max.x-min.x)/2),min.y+((max.y-min.y)/2));
}
CPoint &CPoligono::operator[](unsigned int index){
		if(index>=m_vertices.size())
			return m_vertices[0];
		return m_vertices[index];
}
CPoint &CPoligono::GetVertex(unsigned int index){
		if(index>=m_vertices.size())
			return m_vertices[0];
		return m_vertices[index];
}
int CPoligono::Save(FILE * file)
{
	fprintf(file,"%d\n%d\n%d\n",m_vertices.size(),m_color,m_closed);  //Graba el numero de vertices, el color y si es cerrado o no
	for(int i=0;i<m_vertices.size();i++)
		fprintf(file,"%ld,%ld\n",m_vertices[i].x,m_vertices[i].y);   //Graba cada uno de los vertices
	return 1;
}

int CPoligono::Load(FILE * file)
{   
	int num_vertices;
	fscanf(file,"%d\n",&num_vertices);  //Leer el numero de vertices
	fscanf(file,"%d\n",&m_color);       //Leer el color
	fscanf(file,"%d\n",&m_closed);      //Leer si es cerrado o no
	for(int i=0;i<num_vertices;i++){
		CPoint p; 
		fscanf(file,"%ld,%ld\n",&p.x,&p.y);  //Leer cada uno de los vertices
		m_vertices.push_back(p);
	}
	CalculateMinMax();
	return 1;
}


void CPoligono::CalculateMinMax(void)
{
	min=CPoint(100000000,100000000);
	max=CPoint(-100000000,-100000000);
	CPoint point;
	for(int i=0;i<m_vertices.size();i++){
		point=m_vertices[i];
    	if(point.x>max.x)max.x=point.x;
	    if(point.x<min.x)min.x=point.x;
	    if(point.y>max.y)max.y=point.y;
	    if(point.y<min.y)min.y=point.y;
	}
	center=CPoint(min.x+((max.x-min.x)/2),min.y+((max.y-min.y)/2));

}


int CPoligono::PuntoDentroDeArea(CPoint tp)
{
  if(tp.x<min.x||tp.x>max.x||tp.y<min.y||tp.y>max.y)
	  return 0;

	int i, j, c = 0;
  for (i = 0, j = m_vertices.size()-1; i < m_vertices.size(); j = i++) {
    if ( ((m_vertices[i].y>tp.y) != (m_vertices[j].y>tp.y)) &&
	 (tp.x < (m_vertices[j].x-m_vertices[i].x) * (tp.y-m_vertices[i].y) / (m_vertices[j].y-m_vertices[i].y) + m_vertices[i].x) )
       c++ ;
  }
  return c%2;
 
}


void CPoligono::Draw(CDC * pDC)
{
	//pDC->SetDCPenColor(this->m_color);
	
		pDC->MoveTo(m_vertices[0].x,m_vertices[0].y);
	    for(int i=1;i<m_vertices.size();i++){
		   pDC->LineTo(m_vertices[i].x,m_vertices[i].y);
	    }
		if(m_closed&&m_vertices.size()>2){
	       pDC->LineTo(m_vertices[0].x,m_vertices[0].y);
		}
	
}

void CPoligono::Draw(CDC * pDC, CViewPipeline * v_pipeline)
{
	CPoint vertice1,vertice2;
	if(m_vertices.size()<2)
		return;
	int i;
	for(i=0;i<m_vertices.size()-1;i++){
		vertice1=m_vertices[i];
		vertice2=m_vertices[i+1];
		if(v_pipeline->ClipLine(vertice1,vertice2)){
			pDC->MoveTo(v_pipeline->World2Device(vertice1));
			pDC->LineTo(v_pipeline->World2Device(vertice2));
		}
	}
	if(m_closed){
		vertice1=m_vertices[i];
		vertice2=m_vertices[0];
		if(v_pipeline->ClipLine(vertice1,vertice2)){
			pDC->MoveTo(v_pipeline->World2Device(vertice1));
			pDC->LineTo(v_pipeline->World2Device(vertice2));
		}
	}
}
void CPoligono::AgregarTransformacion(float matriz[3][3])
{
	float mat[3][3]={0};
	for(int i=0; i<3;i++)
	{
		for(int k=0;k<3;k++)
		{
			for(int j=0;j<3;j++)
			{
				mat[i][k]+= this->MatrizTransformacion[i][j] * matriz[j][k];
			}
		}
	}
	for(int i=0; i<3;i++)
	{
		for(int j=0; j<3; j++)
		{
			MatrizTransformacion[i][j]=mat[i][j];
		}
	}
}
void CPoligono::AplicarTransformacion(){	
	m_vertices.clear();
	for(int i=0;i<this->m_vertices_originales.size();i++){
			CPoint p;
			p.x = 0;
			p.y = 0;
			p.x += m_vertices_originales[i].x * MatrizTransformacion[0][0] + m_vertices_originales[i].y * MatrizTransformacion[1][0] + MatrizTransformacion[2][0];
			p.y += m_vertices_originales[i].x * MatrizTransformacion[0][1] + m_vertices_originales[i].y * MatrizTransformacion[1][1] + MatrizTransformacion[2][1];;
			m_vertices.push_back(p);
	}
	CalculateMinMax();
}
