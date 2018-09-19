// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Graficacion2D.h"
#include "ChildView.h"
#include "MainFrm.h"
#define _USE_MATH_DEFINES
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
: m_tool(1)
, m_tool_state(0)
, punto_inicial(0)
, wpoint(0)
, ccenter(0)
, angulo_r(0)
, poligono_seleccionado(-1)
, escalamiento_x(0)
{
	lt_gray_pen.CreatePen(PS_SOLID,1,RGB(128,128,128));
	selection_pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	selection_color=RGB(255,0,0);
	arrow_struct.nWidth=10;		
	arrow_struct.fTheta=0.350;	
	arrow_struct.iMode=2;
	m_dibujo=new CDibujo();

}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_POLIGONO_SELECCIONAR, &CChildView::OnPoligonoSeleccionar)
	ON_UPDATE_COMMAND_UI(ID_POLIGONO_SELECCIONAR, &CChildView::OnUpdatePoligonoSeleccionar)
	ON_COMMAND(ID_POLIGONO_AGREGAR, &CChildView::OnPoligonoAgregar)
	ON_UPDATE_COMMAND_UI(ID_POLIGONO_AGREGAR, &CChildView::OnUpdatePoligonoAgregar)
	ON_COMMAND(ID_POLIGONO_COLOR, &CChildView::OnPoligonoColor)
	ON_COMMAND(ID_POLIGONO_ROTAR, &CChildView::OnPoligonoRotar)
	ON_UPDATE_COMMAND_UI(ID_POLIGONO_ROTAR, &CChildView::OnUpdatePoligonoRotar)
	ON_COMMAND(ID_POLIGONO_ESCALAR, &CChildView::OnPoligonoEscalar)
	ON_UPDATE_COMMAND_UI(ID_POLIGONO_ESCALAR, &CChildView::OnUpdatePoligonoEscalar)
	ON_COMMAND(ID_POLIGONO_TRASLADAR, &CChildView::OnPoligonoTrasladar)
	ON_UPDATE_COMMAND_UI(ID_POLIGONO_TRASLADAR, &CChildView::OnUpdatePoligonoTrasladar)
	ON_COMMAND(ID_DESPLIEGUE_ZOOMIN, &CChildView::OnDespliegueZoomin)
	ON_COMMAND(ID_DESPLIEGUE_ZOOMOUT, &CChildView::OnDespliegueZoomout)
	ON_COMMAND(ID_DESPLIEGUE_1, &CChildView::OnDespliegue1)
	ON_UPDATE_COMMAND_UI(ID_DESPLIEGUE_ZOOMIN, &CChildView::OnUpdateDespliegueZoomin)
	ON_UPDATE_COMMAND_UI(ID_DESPLIEGUE_ZOOMOUT, &CChildView::OnUpdateDespliegueZoomout)
	ON_COMMAND(ID_FILE_SAVE, &CChildView::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CChildView::OnFileOpen)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);


	CMainFrame *frm=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	status_bar=&(frm->m_wndStatusBar);
	zoom_cursor=AfxGetApp()->LoadCursor(IDC_ZOOM_CURSOR);
	zoom_out_cursor=AfxGetApp()->LoadCursor(IDC_ZOOMOUT_CURSOR);
	ch_cursor=AfxGetApp()->LoadStandardCursor(IDC_CROSS);

	return TRUE;
}
void CChildView::OnPoligonoSeleccionar()
{
	// TODO: Add your command handler code here
	m_tool=0;
}

void CChildView::OnUpdatePoligonoSeleccionar(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_dibujo->Size()<1){
		pCmdUI->Enable(FALSE);
		poligono_seleccionado=-1;
	}
	else{
		/*if(poligono_seleccionado<0||poligono_seleccionado>=m_dibujo->Size())
			poligono_seleccionado=0;*/
		pCmdUI->Enable();
	}

	if(m_tool==0){
		pCmdUI->SetCheck();
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CChildView::OnPoligonoAgregar()
{
	// TODO: Add your command handler code here
	m_tool=1;
	status_bar->SetPaneText(0,"Click izquierdo para indicar el primer vertice de un nuevo poligono");
}

void CChildView::OnUpdatePoligonoAgregar(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_tool==1){
		pCmdUI->SetCheck();
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CChildView::OnPoligonoColor()
{
	// TODO: Add your command handler code here
	//Cambiar el color default del poligono
	CColorDialog dlg;
	if(dlg.DoModal()!=IDOK)return;
	if(poligono_seleccionado<0){
		m_default_color=dlg.GetColor();
	}
	else{
		(*(m_dibujo))[poligono_seleccionado].SetColor(dlg.GetColor());
	}
}

void CChildView::OnPoligonoRotar()
{
	// TODO: Add your command handler code here
	m_tool=3;
	m_tool_state=0;
	status_bar->SetPaneText(0,"Presione el boton izquierdo para comenzar a indicar el angulo de rotacion");
}

void CChildView::OnUpdatePoligonoRotar(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_dibujo->Size()<1){
		pCmdUI->Enable(FALSE);
	}
	else{
		pCmdUI->Enable();
	}
	if(m_tool==3){
		pCmdUI->SetCheck();
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CChildView::OnPoligonoEscalar()
{
	// TODO: Add your command handler code here
	m_tool=4;
	m_tool_state=0;
	status_bar->SetPaneText(0,"Presione el boton izquierdo para comenzar a indicar el factor de escalamiento");
}

void CChildView::OnUpdatePoligonoEscalar(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_dibujo->Size()<1){
		pCmdUI->Enable(FALSE);
	}
	else{
		pCmdUI->Enable();
	}
	if(m_tool==4){
		pCmdUI->SetCheck();
	}else{
		pCmdUI->SetCheck(0);
	}

}

void CChildView::OnPoligonoTrasladar()
{
	// TODO: Add your command handler code here
	m_tool=5;
	m_tool_state=0;
	status_bar->SetPaneText(0,"Presione el boton izquierdo para indicar el punto inicial del vector de traslamiento");
}

void CChildView::OnUpdatePoligonoTrasladar(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_dibujo->Size()<1){
		pCmdUI->Enable(FALSE);
	}
	else{
		pCmdUI->Enable();
	}
	if(m_tool==5){
		pCmdUI->SetCheck();
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CChildView::OnDespliegueZoomin()
{
	// TODO: Add your command handler code here
	m_tool=10;
	m_tool_state=0;
	status_bar->SetPaneText(0,"Presione el boton izquierdo para comenzar a capturar el area de zoom");
}

void CChildView::OnDespliegueZoomout()
{
	// TODO: Add your command handler code here
	m_tool=11;
	m_tool_state=0;
	status_bar->SetPaneText(0,"Click izquierdo para volver a la vista anterior o aumentar el area desplegada");
}

void CChildView::OnDespliegue1()
{
	// TODO: Add your command handler code here
	m_v_pipeline.SetWindow(CRect(-10000,10000,10000,-10000));
	vistas_previas.clear();
	RedrawWindow();
}

void CChildView::OnUpdateDespliegueZoomin(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_tool==10){
		pCmdUI->SetCheck();
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CChildView::OnUpdateDespliegueZoomout(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_tool==11){
		pCmdUI->SetCheck();
	}else{
		pCmdUI->SetCheck(0);
	}
}
void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	int rc;
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
	dc.SelectObject(&lt_gray_pen);
	CPoint origin=m_v_pipeline.World2Device(CPoint(0,0));
	dc.Rectangle(m_v_pipeline.GetViewport());
	dc.MoveTo(origin.x, origin.y-50);
	dc.LineTo(origin.x, origin.y+50);
	dc.MoveTo(origin.x-50, origin.y);
	dc.LineTo(origin.x+50, origin.y);
	
	CDibujo &dib=*(m_dibujo);
	dc.SelectStockObject(DC_PEN);
	for(int nf=0;nf<m_dibujo->Size();nf++){
		if(nf==poligono_seleccionado)
			rc=dc.SetDCPenColor(selection_color);
		else{
			rc=dc.SetDCPenColor(dib[nf].GetColor());
		}
		dib[nf].Draw(&dc,&m_v_pipeline);
	}
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

		if(m_tool==0){
		SeleccionarFigura(point);
	}
	if(m_tool==1){
		// TODO: Add your message handler code here and/or call default
		if(m_tool_state==0){//No se ha comenzado captura de un poligono y click indica la captura de un nuevo poligono
			                //El primer vertice sera fijado y el segundo comenzara en la misma posicion.
			punto_inicial=point;   //Se capturaran en coordenadas de dispositivo. Cuando se fije un punto, se convertira a mundiales
			punto_final=point;     //y se insertara al poligono
			punto_auxiliar=point;
			poligono_capturado=CPoligono();
			poligono_capturado.AddVertex(m_v_pipeline.Device2World(point));
			m_tool_state=1;  //se capturara el siguiente punto
			CDC *pDC=GetDC();
			pDC->SelectObject(lt_gray_pen);
			pDC->SetROP2(R2_XORPEN);
			pDC->MoveTo(punto_auxiliar);
			pDC->LineTo(punto_final);
			ReleaseDC(pDC);
			status_bar->SetPaneText(0,"Click izquierdo para indicar la posicion del segundo vertice e insertar vertice nuevo, doble click para terminar");
		}
		else{
			poligono_capturado.AddVertex(m_v_pipeline.Device2World(point));
			CDC *pDC=GetDC();
			pDC->SetROP2(R2_XORPEN);   //Borra la version anterior de los vertices
			pDC->SelectObject(lt_gray_pen);
			pDC->MoveTo(punto_auxiliar);
			pDC->LineTo(punto_final);
			if(m_tool_state>1)
				pDC->LineTo(punto_inicial);
			punto_final=point;
			pDC->SetROP2(R2_COPYPEN);   //Dibuja la ultima linea para hacerla definitiva
			pDC->SelectStockObject(BLACK_PEN);
			pDC->MoveTo(punto_auxiliar);
			pDC->LineTo(punto_final);
			punto_auxiliar=punto_final;
			m_tool_state=2;//se ha capturado mas de un punto. Este estado indicara que se tiene que
			               //dibujar una linea para cerrar el poligono
			pDC->SetROP2(R2_XORPEN);   //Dibuja la parte del poligono que involugra los ultimos vertices
			pDC->SelectObject(lt_gray_pen);
			pDC->MoveTo(punto_auxiliar);
			pDC->LineTo(punto_final);
			if(m_tool_state>1)
				pDC->LineTo(punto_inicial);
			ReleaseDC(pDC);
			status_bar->SetPaneText(0,"Click izquierdo para indicar la posicion del vertice e insertar vertice nuevo, doble click para terminar");
		
		}
		return;
	}
	if(m_tool==3){// Rotacion
		punto_inicial=point;
		punto_final=point;
		ccenter=m_v_pipeline.World2Device((*m_dibujo)[poligono_seleccionado].GetCenter());
		d2c=abs(punto_inicial.x-ccenter.x);
		if(abs(punto_inicial.y-ccenter.y)>d2c)
			d2c=abs(punto_inicial.y-ccenter.y);
		angulo_r=0;
		CalcularAngulo();
		m_tool_state=1;
		status_bar->SetPaneText(0,"Arrastre para establecer el angulo de rotacion");
		return;
	}
	if(m_tool==4){ //Escalamiento
 		punto_inicial=point;
		punto_final=point;
		ccenter=m_v_pipeline.World2Device((*m_dibujo)[poligono_seleccionado].GetCenter());
		CalcularEscalamiento();
		CPoint dmin=m_v_pipeline.World2Device((*m_dibujo)[poligono_seleccionado].GetMin());
		CPoint dmax=m_v_pipeline.World2Device((*m_dibujo)[poligono_seleccionado].GetMax());
		sizex=dmax.x-dmin.x;//el tamaño del rectangulo que envuelve la figura seleccionada
		sizey=dmin.y-dmax.y;
		CRect bounding_box;
		bounding_box.left=ccenter.x-((long)((double)sizex*escalamiento_x+.5)/2);
		bounding_box.right=ccenter.x+((long)((double)sizex*escalamiento_x+.5)/2);
		bounding_box.top=ccenter.y-((long)((double)sizey*escalamiento_y+.5)/2);
		bounding_box.bottom=ccenter.y+((long)((double)sizey*escalamiento_y+.5)/2);
        CDC *pDC=GetDC();
		pDC->SelectObject(lt_gray_pen);
        pDC->SelectStockObject(HOLLOW_BRUSH);
		pDC->SetROP2(R2_XORPEN);
		pDC->Rectangle(bounding_box);
		ReleaseDC(pDC);
		m_tool_state=1;
		status_bar->SetPaneText(0,"Arrastre el cursor para definir el factor de escalamiento. Para aumentar, alejarse del centro de la figura");
		return;
	}

	if(m_tool==5){ //Traslamiento
 		punto_inicial=point;
		punto_final=point;
		CDC *pDC=GetDC();
		pDC->SelectObject(lt_gray_pen);
		pDC->SetROP2(R2_XORPEN);
		pDC->MoveTo(punto_inicial);
		ArrowTo(pDC,punto_final,&arrow_struct);
		ReleaseDC(pDC);
		m_tool_state=1;
		status_bar->SetPaneText(0,"Arrastre el cursor en la magnitud y direccion del traslamiento");
		return;
	}
	if(m_tool==10){
		punto_inicial=point;
		punto_final=point;
		CDC *pDC=GetDC();
		//pDC->SelectStockObject(WHITE_PEN);
		pDC->SelectObject(lt_gray_pen);
		CPen grp;
		pDC->SelectStockObject(HOLLOW_BRUSH);
		pDC->SetROP2(R2_XORPEN);
		pDC->Rectangle(punto_inicial.x,punto_inicial.y,punto_final.x,punto_final.y);
		ReleaseDC(pDC);
		m_tool_state=1;
		status_bar->SetPaneText(0,"Arrastre el cursor para definir el area de zoom");
		return;
	}
	if(m_tool==11){
		if(vistas_previas.size()>0){ //regresa a la vista previa
			CRect vista_previa=vistas_previas.back();
			vistas_previas.pop_back();
			m_v_pipeline.SetWindow(vista_previa);
			RedrawWindow();
			return;
		}
		//zoom out 20%
		CRect cv=m_v_pipeline.GetWindow();
		long wsx,wsy;
		wsx=cv.right-cv.left;
		wsy=cv.top-cv.bottom;
		cv.left-=wsx/10; //recorre 10% borde izquierdo de la ventana
		cv.right+=wsx/10;
		cv.top+=wsy/10;
		cv.bottom-=wsy/10;
		m_v_pipeline.SetWindow(cv);
		RedrawWindow();
	}
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(m_tool==3&&m_tool_state==1){
		if(angulo_r!=0){
			CDC *pDC=GetDC();
		    pDC->SelectObject(lt_gray_pen);
	    	pDC->SetROP2(R2_XORPEN);
		    pDC->MoveTo(ccenter);
		    pDC->LineTo(punto_inicial);
		    pDC->MoveTo(ccenter);
		    pDC->LineTo(punto_auxiliar);
			if(angulo_r<0)
               pDC->Arc(ccenter.x-d2c,ccenter.y-d2c,ccenter.x+d2c,ccenter.y+d2c,punto_inicial.x,punto_inicial.y,punto_final.x,punto_final.y);
			else
				pDC->Arc(ccenter.x-d2c,ccenter.y-d2c,ccenter.x+d2c,ccenter.y+d2c,punto_final.x,punto_final.y,punto_inicial.x,punto_inicial.y);
            
		    ReleaseDC(pDC);
		}
		punto_final=m_v_pipeline.Device2World(point);
		punto_inicial=m_v_pipeline.Device2World(punto_inicial);
		ccenter=(*m_dibujo)[poligono_seleccionado].GetCenter();
		CalcularAngulo();
		m_tool_state=0;
		sprintf(status_message,"Angulo de rotacion: %lf. De click en el punto inicial para indicar un nuevo angulo de rotacion",angulo_r*(180/M_PI));
		status_bar->SetPaneText(0,status_message);
		AplicarRotacion();
		return;
	}
	if(m_tool==4&&m_tool_state==1){ //Escalamiento
 		
		CRect bounding_box;
		bounding_box.left=ccenter.x-((long)((double)sizex*escalamiento_x+.5)/2);
		bounding_box.right=ccenter.x+((long)((double)sizex*escalamiento_x+.5)/2);
		bounding_box.top=ccenter.y-((long)((double)sizey*escalamiento_y+.5)/2);
		bounding_box.bottom=ccenter.y+((long)((double)sizey*escalamiento_y+.5)/2);
        CDC *pDC=GetDC();
		pDC->SelectObject(lt_gray_pen);
		pDC->SelectStockObject(HOLLOW_BRUSH);
		pDC->SetROP2(R2_XORPEN);
		pDC->Rectangle(bounding_box);
		ReleaseDC(pDC);
		punto_final=point;
		CalcularEscalamiento();
		m_tool_state=0;
		status_bar->SetPaneText(0,"Presione el boton izquierdo para comenzar a indicar el factor de escalamiento");
		AplicarEscalamiento();
		return;
	}

	if(m_tool==5&&m_tool_state==1){
		CDC *pDC=GetDC();
		pDC->SelectObject(lt_gray_pen);
		pDC->SelectStockObject(WHITE_BRUSH);
		pDC->SetROP2(R2_XORPEN);
		pDC->MoveTo(punto_inicial);
		ArrowTo(pDC,punto_final,&arrow_struct);
		punto_final=point;
		ReleaseDC(pDC);
		m_tool_state=0;
		status_bar->SetPaneText(0,"De click en el punto inicial del vector de traslacion");
		//Convierte punto inicial y final a coordenadas mundiales
		punto_inicial=m_v_pipeline.Device2World(punto_inicial);
		punto_final=m_v_pipeline.Device2World(punto_final);
		//Calcular vector de traslamiemto y hacer el escalamiento
		traslamiento_x=punto_final.x-punto_inicial.x;
		traslamiento_y=punto_final.y-punto_inicial.y;
		AplicarTraslamiento();
		
		return;


	}
	if(m_tool==10&&m_tool_state==1){ //zoom
		CDC *pDC=GetDC();
		//pDC->SelectObject(lt_gray_pen);
		pDC->SelectObject(lt_gray_pen);
		pDC->SelectStockObject(HOLLOW_BRUSH);
		pDC->SetROP2(R2_XORPEN);
		pDC->Rectangle(punto_inicial.x,punto_inicial.y,punto_final.x,punto_final.y);
		punto_final=point;
		ReleaseDC(pDC);
		CPoint pi=m_v_pipeline.Device2World(punto_inicial);
        CPoint pf=m_v_pipeline.Device2World(punto_final);
		long tmp;
		if(pf.x<pi.x){
           tmp=pf.x;pf.x=pi.x;pi.x=tmp;
		}
		if(pf.y<pi.y){
           tmp=pf.y;pf.y=pi.y;pi.y=tmp;
		}
		long zax,zay;
		zax=pf.x-pi.x;
        zay=pf.y-pi.y;
		if(zax<zay){ // hacer el area cuadrada para que no exista distorsion
			long mpx=pi.x+(zax/2);
			pi.x=mpx-(zay/2);
			pf.x=mpx+(zay/2);
		}
		else{
			long mpy=pi.y+(zay/2);
			pi.y=mpy-(zax/2);
			pf.y=mpy+(zax/2);
		}
		CRect nueva_ventana(pi.x,pf.y,pf.x,pi.y);
		vistas_previas.push_back(m_v_pipeline.GetWindow());
		m_v_pipeline.SetWindow(nueva_ventana);
		m_tool_state=0;
		status_bar->SetPaneText(0,"De click en el punto inicial del area a ampliar");
		RedrawWindow();
	}
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(m_tool==1&&m_tool>0){
		poligono_capturado.AddVertex(m_v_pipeline.Device2World(point));
		CDC *pDC=GetDC();
		pDC->SetROP2(R2_XORPEN);   //Borra la version anterior de los vertices
		pDC->SelectObject(lt_gray_pen);
		pDC->MoveTo(punto_auxiliar);
		pDC->LineTo(punto_final);
		if(m_tool_state>1)
			pDC->LineTo(punto_inicial);
		pDC->SetROP2(R2_COPYPEN);   //Dibuja la ultima linea para hacerla definitiva
		pDC->SelectStockObject(BLACK_PEN);
		pDC->MoveTo(punto_auxiliar);
		pDC->LineTo(punto_final);
		if(m_tool_state>1)
			pDC->LineTo(punto_inicial);
		ReleaseDC(pDC);
		m_dibujo->AddPolygon(poligono_capturado);
		m_tool_state=0;//se ha capturado mas de un punto. Este estado indicara que se tiene que
			               //dibujar una linea para cerrar el poligono
		status_bar->SetPaneText(0,"Click izquierdo para indicar el primer vertice de un nuevo poligono");
		poligono_seleccionado=m_dibujo->Size()-1;
		RedrawWindow();
	}
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
  
	wpoint=m_v_pipeline.Device2World(point);
	sprintf(xy_message,"(%ld, %ld)", wpoint.x,wpoint.y);
	status_bar->SetPaneText(1,xy_message);
	
	if(m_tool==1&&m_tool_state>0){
		CDC *pDC=GetDC();
		pDC->SetROP2(R2_XORPEN);   //Borra la version anterior de los vertices
		pDC->SelectObject(lt_gray_pen);
		pDC->MoveTo(punto_auxiliar);
		pDC->LineTo(punto_final);
		if(m_tool_state>1)
			pDC->LineTo(punto_inicial);
		punto_final=point;  //actualiza el ultimo vertice y dibuja la version nueva
		pDC->MoveTo(punto_auxiliar);
		pDC->LineTo(punto_final);
		if(m_tool_state>1)
			pDC->LineTo(punto_inicial);
		ReleaseDC(pDC);

	}
	if(m_tool==3&&m_tool_state==1){
		CDC *pDC=GetDC();
		pDC->SelectObject(lt_gray_pen);
		pDC->SetROP2(R2_XORPEN);
		if(angulo_r!=0){
			pDC->MoveTo(ccenter);
		    pDC->LineTo(punto_inicial);
		    pDC->MoveTo(ccenter);
		    pDC->LineTo(punto_auxiliar);
			if(angulo_r<0)
               pDC->Arc(ccenter.x-d2c,ccenter.y-d2c,ccenter.x+d2c,ccenter.y+d2c,punto_inicial.x,punto_inicial.y,punto_final.x,punto_final.y);
			else
				pDC->Arc(ccenter.x-d2c,ccenter.y-d2c,ccenter.x+d2c,ccenter.y+d2c,punto_final.x,punto_final.y,punto_inicial.x,punto_inicial.y);
		}
		punto_final=point;
		punto_auxiliar.x=ccenter.x+(d2c*cos(alfa2));
		punto_auxiliar.y=ccenter.y+(d2c*sin(alfa2));
	    CalcularAngulo();
		status_bar->SetPaneText(0,status_message);
		pDC->MoveTo(ccenter);
		pDC->LineTo(punto_inicial);
		pDC->MoveTo(ccenter);
		pDC->LineTo(punto_auxiliar);
        if(angulo_r<0)
               pDC->Arc(ccenter.x-d2c,ccenter.y-d2c,ccenter.x+d2c,ccenter.y+d2c,punto_inicial.x,punto_inicial.y,punto_final.x,punto_final.y);
			else
				pDC->Arc(ccenter.x-d2c,ccenter.y-d2c,ccenter.x+d2c,ccenter.y+d2c,punto_final.x,punto_final.y,punto_inicial.x,punto_inicial.y);
		ReleaseDC(pDC);
		return;
		
	}
	if(m_tool==4&&m_tool_state==1){ //Escalamiento
 		
		CRect bounding_box;
		bounding_box.left=ccenter.x-((long)((double)sizex*escalamiento_x+.5)/2);
		bounding_box.right=ccenter.x+((long)((double)sizex*escalamiento_x+.5)/2);
		bounding_box.top=ccenter.y-((long)((double)sizey*escalamiento_y+.5)/2);
		bounding_box.bottom=ccenter.y+((long)((double)sizey*escalamiento_y+.5)/2);
        CDC *pDC=GetDC();
		pDC->SelectObject(lt_gray_pen);
		pDC->SelectStockObject(HOLLOW_BRUSH);
		pDC->SetROP2(R2_XORPEN);
		pDC->Rectangle(bounding_box);
		punto_final=point;
		CalcularEscalamiento();
		bounding_box.left=ccenter.x-((long)((double)sizex*escalamiento_x+.5)/2);
		bounding_box.right=ccenter.x+((long)((double)sizex*escalamiento_x+.5)/2);
		bounding_box.top=ccenter.y-((long)((double)sizey*escalamiento_y+.5)/2);
		bounding_box.bottom=ccenter.y+((long)((double)sizey*escalamiento_y+.5)/2);
		pDC->Rectangle(bounding_box);
		ReleaseDC(pDC);
		m_tool_state=1;
		status_bar->SetPaneText(0,"Arrastre el cursor para definir el factor de escalamiento. Para aumentar, alejarse del centro de la figura");
		return;
	}
	if(m_tool==5&&m_tool_state==1){  //Traslamiento
		CDC *pDC=GetDC();
		pDC->SelectObject(lt_gray_pen);
		pDC->SelectStockObject(WHITE_BRUSH);
		pDC->SetROP2(R2_XORPEN);
		pDC->MoveTo(punto_inicial);
		ArrowTo(pDC,punto_final,&arrow_struct);
		punto_final=point;
		pDC->MoveTo(punto_inicial);
		ArrowTo(pDC,punto_final,&arrow_struct);
		ReleaseDC(pDC);

	}
	if(m_tool==10&&m_tool_state==1){ //zoom
		CDC *pDC=GetDC();
		pDC->SelectObject(lt_gray_pen);
		//pDC->SelectObject(lt_gray_pen);
		pDC->SelectStockObject(HOLLOW_BRUSH);
		pDC->SetROP2(R2_XORPEN);
		pDC->Rectangle(punto_inicial.x,punto_inicial.y,punto_final.x,punto_final.y);
		punto_final=point;
		pDC->Rectangle(punto_inicial.x,punto_inicial.y,punto_final.x,punto_final.y);
		ReleaseDC(pDC);
	}
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if(nChar==9){ //Tab selecciona la siguiente figura en el documento
		if(m_dibujo->Size()<2)return;
		if(poligono_seleccionado<0)return;
		DrawSelected(1);
		poligono_seleccionado=(poligono_seleccionado+1)%(m_dibujo->Size());
		DrawSelected();
	}
}

BOOL CChildView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	if(m_tool==1){
		SetCursor(ch_cursor);
		return true;
	}
	if(m_tool==10){
		SetCursor(zoom_cursor);
		return true;
	}
	if(m_tool==11){
		SetCursor(zoom_out_cursor);
		return true;
	}
    return CWnd::OnSetCursor(pWnd, nHitTest, message);
	//return true;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	long vpsize;
	if(cx<cy){// ajusta para que el view port siempre tenga el mismo aspecto
		vpsize=cx;
		m_v_pipeline.SetViewport(CRect(0,(cy/2)-(vpsize/2),cx-1,(cy/2)+(vpsize/2)));
	}
	else{
		vpsize=cy;
		m_v_pipeline.SetViewport(CRect(cx/2-(vpsize/2),0,cx/2+(vpsize/2),cy-1));
	}
}
int CChildView::DrawSelected(int restore)
{
	if(poligono_seleccionado<0)return 0;
	CDC *pDC=GetDC();
	pDC->SelectStockObject(DC_PEN);
    if(restore){
		pDC->SetDCPenColor((*(m_dibujo))[poligono_seleccionado].GetColor());
	}
	else{
		pDC->SetDCPenColor(selection_color);
	}
	
	(*(m_dibujo))[poligono_seleccionado].Draw(pDC,&m_v_pipeline);
	return 0;
}

void CChildView::CalcularEscalamiento(void)
{
	escalamiento_x=(double)(punto_final.x-ccenter.x)/(double)(punto_inicial.x-ccenter.x);
	escalamiento_y=(double)(punto_final.y-ccenter.y)/(double)(punto_inicial.y-ccenter.y);

}

void CChildView::AplicarRotacion(void)
{
	//Insertar aqui la rotacion
	//El angulo de rotacion esta almacenado en la variable angulo_r. y el indice en el documento es figura_seleccionada
	//Rotar con respecto al centro de la figura. Para obtener el centro hay que invocar GetCenter() de la figura seleccionada
	//emeuno
//	float m1[3][3]={{1,0,0},
//					  {0,1,0},
//					  {0,0,1}};
//	(*m_dibujo)[poligono_seleccionado].AgregarTransformacion(m1);
}

void CChildView::AplicarEscalamiento(void)
{
	//Insertar aqui el escalamiento.
		//Los factores de escalamiento estan almacenados en las variables escalamiento_x y escalamiento_y
		//Escalar con respecto al centro de la figura. Para obtener el centro hay que invocar GetCenter() de la figura seleccionada
//float m1[3][3]={{1,0,0},
//					  {0,1,0},
//					  {0,0,1}};
//	(*m_dibujo)[poligono_seleccionado].AgregarEscalamiento(m1);
}

void CChildView::AplicarTraslamiento(void)
{
	//Insertar aqui el traslamiento. traslamiento_x y traslamiento_y son los componentes del vector de traslamiento
	//float m1[3][3]={{1,0,0},
	//				  {0,1,0},
	//				  {0,0,1}};
	//(*m_dibujo)[poligono_seleccionado].AgregarTraslamiento(m1);
}

void CChildView::SeleccionarFigura(CPoint point)
{
	//Insertar el codigo para seleccionar la figura. Dado el punto point, debe verificar si el punto
	//se encuentra dentro del area de alguna figura. El indice de la figura en la cual se encuentre el punto
	//debe ser almacenado en poligono_seleccionado;
     
	
	CPoint tpoint=m_v_pipeline.Device2World(point);
    CDibujo &dib=*(m_dibujo);
	int new_sel=-1;
	for(int i=0;i<dib.Size();i++){
		if(dib[i].PuntoDentroDeArea(tpoint)){
			if(i!=poligono_seleccionado){
				new_sel=i;
				break;
			}
			else{
				new_sel=-1;
			}
		}
	}
	poligono_seleccionado=new_sel;
    //Una vez marcada la figura a la que se dio click, hay que dibujar nuevamente
	RedrawWindow();

}
void CChildView::CalcularAngulo(void)
{

	//double a1,a2;
	//angulo de la linea indicada por el punto inicial hacia el centro, con respecto al horizonte
	alfa1=atan2((double)(punto_inicial.y-ccenter.y),(double)(punto_inicial.x-ccenter.x)); 
    //angulo de la linea indicada por el punto final hacia el centro, con respecto al horizonte
	alfa2=atan2((double)(punto_final.y-ccenter.y),(double)(punto_final.x-ccenter.x));
	angulo_r=alfa2-alfa1;

}
void CChildView::OnFileSave()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(FALSE,"grf",0,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"Archivos de dibujo (*.grf)|*.grf|Todos los archivos (*.*)|*.*||",this);
	if(dlg.DoModal()!=IDOK)return;
	if(!m_dibujo->Save(dlg.GetPathName())){
		MessageBox("No se pudo grabar el dibujo!", "Error grabando archivo",MB_OK);
	}

}

void CChildView::OnFileOpen()
{
	// TODO: Add your command handler code here
	
	CFileDialog dlg(TRUE,"grf",0,OFN_PATHMUSTEXIST,"Archivos de dibujo (*.grf)|*.grf|Todos los archivos (*.*)|*.*||",this);
	if(dlg.DoModal()!=IDOK)return;
	CDibujo *new_dibujo=new CDibujo();
	if(!new_dibujo->Load(dlg.GetPathName())){
		delete new_dibujo;
		MessageBox("No se pudo abrir el dibujo!", "Error abriendo archivo",MB_OK);
		return;
	}
	delete m_dibujo;
	m_dibujo=new_dibujo;
	poligono_seleccionado=-1;
	RedrawWindow();

}

void CChildView::AgregarTransformacion(float * matriz)
{
}

void CChildView::AgregarEscalamiento(float * matriz)
{
}

void CChildView::AgregarRotacion(float * matriz)
{
}
