/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE, INRIA 2001-2002
 *    Djalel ABDEMOUCHE, INRIA 2002-2004
 *    Fabrice Leray,     INRIA 2004-xxxx
 *    Comment:
 *    This file contains all functions used to CLONE an object, it means make
 *    a copy of an object under the same subwin.
 --------------------------------------------------------------------------*/

#include "ObjectStructure.h"
#include "HandleManagement.h"

#ifndef __SCI_CLONE__
#define __SCI_CLONE__

extern double *sciCloneColormap (sciPointObj * pobj); /* CLONE */
extern sciPointObj *CloneText (sciPointObj * pthis); /* CLONE */
extern sciPointObj *sciCloneObj (sciPointObj * pobj); /* CLONE */

extern sciPointObj *CloneRectangle (sciPointObj * pthis);
extern sciPointObj *ClonePolyline (sciPointObj * pthis);
extern sciPointObj *CloneArc (sciPointObj * pthis);


extern sciPointObj *sciCopyObj (sciPointObj * pobj, sciPointObj * psubwinparenttarget ); /* CLONE */

#endif /* __SCI_CLONE__ */
