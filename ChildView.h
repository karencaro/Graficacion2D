// ChildView.h : interface of the CChildView class
//


#pragma once
#include "atltypes.h"
#include "viewpipeline.h"
//#include "MainFrm.h"
#include "Arrow.h"
#include "Dibujo.h"
#include <vector>
#include "afxwin.h"

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:

	CDibujo *m_dibujo; //dibujo
	CPoint punto_inicial,punto_inicial_w;
	CPoint punto_final,punto_final_w;
	CPoint punto_auxiliar,punto_auxiliar_w;
	CPoligono poligono_capturado;
	//herramienta seleccionada. 
	//0.-seleccion, 1.-Dibujar/agregar poligono,
	//3.-Rotar, 4.-Escalar, 5.-Trasladar
	//10.- Zoom in, 11.- Zoom out
	int m_tool; 
	int m_tool_state; //Estado de captura en la herramienta seleccionada

	COLORREF m_default_color;
	CStatusBar *status_bar;
	char xy_message[25];
	char status_message[150];
	ARROWSTRUCT arrow_struct;
	HCURSOR zoom_cursor,ch_cursor,zoom_out_cursor;
	vector<CRect> vistas_previas;
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPoligonoSeleccionar();
	afx_msg void OnUpdatePoligonoSeleccionar(CCmdUI *pCmdUI);
	afx_msg void OnPoligonoAgregar();
	afx_msg void OnUpdatePoligonoAgregar(CCmdUI *pCmdUI);
	afx_msg void OnPoligonoColor();
	afx_msg void OnPoligonoRotar();
	afx_msg void OnUpdatePoligonoRotar(CCmdUI *pCmdUI);
	afx_msg void OnPoligonoEscalar();
	afx_msg void OnUpdatePoligonoEscalar(CCmdUI *pCmdUI);
	afx_msg void OnPoligonoTrasladar();
	afx_msg void OnUpdatePoligonoTrasladar(CCmdUI *pCmdUI);
	afx_msg void OnDespliegueZoomin();
	afx_msg void OnDespliegueZoomout();
	afx_msg void OnDespliegue1();
	afx_msg void OnUpdateDespliegueZoomin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDespliegueZoomout(CCmdUI *pCmdUI);
	CPoint wpoint;
	CViewPipeline m_v_pipeline;
	CPen lt_gray_pen;
	long traslamiento_x,traslamiento_y;
	CPoint ccenter;
	long d2c;
	long sizex,sizey;
	void CalcularAngulo(void);
	double angulo_r,alfa1,alfa2;
	double mpf;
	int DrawSelected(int restore=0);
	int poligono_seleccionado;
	CPen selection_pen;
	COLORREF selection_color;
	double escalamiento_x,escalamiento_y;
	void CalcularEscalamiento(void);
	void AplicarRotacion(void);
	void AplicarEscalamiento(void);
	void AplicarTraslamiento(void);
	void SeleccionarFigura(CPoint point);
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	void AgregarTransformacion(float * matriz);
	void AgregarEscalamiento(float * matriz);
	void AgregarRotacion(float * matriz);
};

