
#ifndef _ARROW_H_
#define _ARROW_H_

#include <windows.h>

// ARROWSTRUCT
//
// Defines the attributes of an arrow.
typedef struct tARROWSTRUCT {
	int nWidth;		// width (in pixels) of the full base of the arrowhead
	float fTheta;	// angle (in radians) at the arrow tip between the two
					//  sides of the arrowhead
	int iMode;		// Indicates the mode of the arrow tip (0.- triangle not filled, 1.- filled triangle, 2.-no triangle
//  filled
} ARROWSTRUCT;

// ArrowTo()
//
// Draws an arrow, using the current pen and brush, from the current position
//  to the passed point using the attributes defined in the ARROWSTRUCT.
void ArrowTo(CDC *pDC, int x, int y, ARROWSTRUCT *pArrow);
void ArrowTo(CDC *pDC, CPoint lpTo, ARROWSTRUCT *pArrow);

#endif
