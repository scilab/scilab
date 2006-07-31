/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE, INRIA 2001-2002
 *    Djalel ABDEMOUCHE, INRIA 2002-2004
 *    Fabrice Leray,     INRIA 2004-xxxx
 *    Comment:
 *    This file contains all functions used to INTERACT with the graphic
 *    window (zoom by pressing button, callbacks...)
 --------------------------------------------------------------------------*/

#include "ObjectStructure.h"
#include "HandleManagement.h"

#ifndef __SCI_INTERACTION__
#define __SCI_INTERACTION__


extern int sciAddCallback (sciPointObj * pthis, char *code, int len, int mevent); /* INTERACTION */
extern int sciExecCallback (sciPointObj * pthis); /* INTERACTION */
extern int sciGetCallbackMouseEvent (sciPointObj * pthis); /* INTERACTION */
extern int sciSetCallbackMouseEvent (sciPointObj * pthis, int mevent); /* INTERACTION */
extern int sciGetCallbackLen (sciPointObj * pthis); /* INTERACTION */
extern char *sciGetCallback (sciPointObj * pthis); /* INTERACTION */
extern int sciDelCallback (sciPointObj * pthis); /* INTERACTION */

extern int Objmove (long *hdl, double *d, int m, BOOL opt); /* INTERACTION */
extern BOOL sciIsAreaZoom(integer *box, integer *box1 ,integer *section); /* INTERACTION */
extern void scizoom(double bbox[], sciPointObj * pobj); /* INTERACTION */






#endif /* __SCI_INTERACTION__ */
