/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library 
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

extern int Objmove(sciPointObj * pobj, double d[], int m, BOOL opt); /* INTERACTION */
extern BOOL sciIsAreaZoom(integer *box, integer *box1 ,integer *section); /* INTERACTION */






#endif /* __SCI_INTERACTION__ */
