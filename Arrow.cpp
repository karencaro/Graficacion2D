#include "stdafx.h"
#include "Arrow.h"
#include <math.h>

// ArrowTo()
//
void ArrowTo(CDC *pDC, int x, int y, ARROWSTRUCT *pA) {

	CPoint ptTo(x, y);

	ArrowTo(pDC, ptTo, pA);
}

// ArrowTo()
//
void ArrowTo(CDC *pDC, CPoint lpTo, ARROWSTRUCT *pA) {

	CPoint pFrom;
	CPoint pBase;
	CPoint aptPoly[3];
	float vecLine[2];
	float vecLeft[2];
	float fLength;
	float th;
	float ta;

	// get from point
	pFrom=pDC->MoveTo(0, 0);

	// set to point
	aptPoly[0].x = lpTo.x;
	aptPoly[0].y = lpTo.y;

	// build the line vector
	vecLine[0] = (float) aptPoly[0].x - pFrom.x;
	vecLine[1] = (float) aptPoly[0].y - pFrom.y;

	// build the arrow base vector - normal to the line
	vecLeft[0] = -vecLine[1];
	vecLeft[1] = vecLine[0];

	// setup length parameters
	fLength = (float) sqrt(vecLine[0] * vecLine[0] + vecLine[1] * vecLine[1]);
	th = pA->nWidth / (2.0f * fLength);
	ta = pA->nWidth / (2.0f * (tanf(pA->fTheta) / 2.0f) * fLength);

	// find the base of the arrow
	pBase.x = (int) (aptPoly[0].x + -ta * vecLine[0]);
	pBase.y = (int) (aptPoly[0].y + -ta * vecLine[1]);

	// build the points on the sides of the arrow
	aptPoly[1].x = (int) (pBase.x + th * vecLeft[0]);
	aptPoly[1].y = (int) (pBase.y + th * vecLeft[1]);
	aptPoly[2].x = (int) (pBase.x + -th * vecLeft[0]);
	aptPoly[2].y = (int) (pBase.y + -th * vecLeft[1]);

	pDC->MoveTo(pFrom);

	// draw we're fillin'...
	if(pA->iMode==0){
		pDC->LineTo(pBase);
		pDC->LineTo(aptPoly[1]);
		pDC->LineTo(aptPoly[0]);
		pDC->LineTo(aptPoly[2]);
		pDC->LineTo(pBase);
		pDC->MoveTo(aptPoly[0]);
		return;
	}
	if(pA->iMode==1) {
		pDC->LineTo(aptPoly[0]);
		pDC->Polygon(aptPoly,3);
		return;
	}

	if(pA->iMode==2){
		pDC->LineTo(pBase);
		pDC->LineTo(aptPoly[0]);
		pDC->MoveTo(aptPoly[1]);
		pDC->LineTo(aptPoly[0]);
		pDC->MoveTo(aptPoly[2]);
		//pDC->LineTo(pBase);
		pDC->LineTo(aptPoly[0]);
	}
}

