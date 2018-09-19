#include "StdAfx.h"
#include "ViewPipeline.h"
//#include <algorithm>
#define DERECHA 0x01
#define IZQUIERDA 0x02
#define ARRIBA 0x04
#define ABAJO 0x08
//using std::swap;
template<class T>
void MiSwap(T &x,T &y){
	T t;
	t=x;
	x=y;
	y=t;
}

CViewPipeline::CViewPipeline(void)
{
	m_ventana=CRect(-10000,10000,10000,-10000);
	m_pdv=CRect(0,0,640,480);
	CalcularEscalamiento();
}

CViewPipeline::~CViewPipeline(void)
{
}

void CViewPipeline::SetWindow(CRect nueva_ventana)
{
	m_ventana=nueva_ventana;
	CalcularEscalamiento();
}

void CViewPipeline::SetViewport(CRect nuevo_pdv)
{
	m_pdv=nuevo_pdv;
	CalcularEscalamiento();
}

CPoint CViewPipeline::World2Device(CPoint p)
{
	CPoint pt;
	pt.x=(long)((float)(p.x-m_ventana.left)*sx+0.5)+m_pdv.left;
	pt.y=(long)((float)(p.y-m_ventana.bottom)*sy+0.5)+m_pdv.bottom;
	return pt;
}

void CViewPipeline::CalcularEscalamiento(void)
{
	sx=(float)(m_pdv.right-m_pdv.left)/(float)(m_ventana.right-m_ventana.left);
	sy=(float)(m_pdv.top-m_pdv.bottom)/(float)(m_ventana.top-m_ventana.bottom);
}

CPoligono CViewPipeline::World2Device(CPoligono poli)
{
	CPoligono transformado;
	for(int i=0;i<poli.Size();i++){
		CPoint pt=World2Device(poli[i]);
		transformado.AddVertex(pt);
	}
	return transformado;
}

CPoint CViewPipeline::Device2World(CPoint p)
{
	CPoint pt;
	pt.x=(long)(((float)(p.x-m_pdv.left)/sx)+0.5)+m_ventana.left;
	pt.y=(long)(((float)(p.y-m_pdv.bottom)/sy)+0.5)+m_ventana.bottom;
	return pt;
}

int CViewPipeline::Encode(CPoint point)
{
	char codigo=0;
	if (point.x<m_ventana.left)
	{
		codigo=codigo|IZQUIERDA;
	}
	if(point.x>m_ventana.right)
	{
		codigo=codigo|DERECHA;
	}
	if(point.y>m_ventana.top)
	{
		codigo=codigo|ARRIBA;
	}
	if(point.y<m_ventana.bottom)
	{
		codigo=codigo|ABAJO;
	}
	return codigo;
}

bool CViewPipeline::ClipLine(CPoint & p1, CPoint & p2)
{
	int terminar=0;
	char codigo1=0;
	char codigo2=0;
	while(terminar!=1)
	{
		codigo1=Encode(p1);
		codigo2=Encode(p2);
		if((codigo1|codigo2)==0)
		{
			return true;
		}
		if((codigo1&codigo2)!=0)
		{
			return false;
		}
		if (codigo1==0)
		{
			//swap x1<->x2
			//swap y1<->y2
			//swap codigo1<->codigo2
			MiSwap<long>(p1.x,p2.x);
			MiSwap<long>(p1.y,p2.y);
			MiSwap<char>(codigo1,codigo2);
		}
		//verificar codigo1 para saber que reecorte hacer y recortar 
		if (codigo1&codigo2)
		{
			return false;
		}
		if(codigo1&IZQUIERDA)
		{
			float m =(float)(p2.y-p1.y)/(p2.x-p1.x);
			p1.y=(m_ventana.left-p1.x)*m+p1.y;
			p1.x=m_ventana.left;
			
		}
		if(codigo1&DERECHA)
		{
			float m =(float)(p2.y-p1.y)/(p2.x-p1.x);
			p1.y=(m_ventana.right-p1.x)*m+p1.y;
			p1.x=m_ventana.right;
			
			
		}

		if(codigo1&ARRIBA)
		{
			float m =(float)(p2.y-p1.y)/(p2.x-p1.x);
			p1.x=p1.x+(m_ventana.top-p1.y)/m;
			p1.y=m_ventana.top;
		}
		if(codigo1&ABAJO)
		{
			float m =(float)(p2.y-p1.y)/(p2.x-p1.x);
			p1.x=p1.x+(m_ventana.bottom-p1.y)/m;
			p1.y=m_ventana.bottom;
		}
	}
	return true;//devuelve verdadero si la linea es visible.
}
