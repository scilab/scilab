/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE, INRIA 2001-2002
 *    Djalel ABDEMOUCHE, INRIA 2002-2004
 *    Fabrice Leray,     INRIA 2004-xxxx
 *    Comment:
 *    This file contains all functions used in graphics handle management.
 --------------------------------------------------------------------------*/

#include "ObjectStructure.h"

#ifndef __SCI_HANDLE_MANAGEMENT__
#define __SCI_HANDLE_MANAGEMENT__

/* Handle functions */
extern void sciSetHandle (sciPointObj * pobj, sciHandleTab * pvalue);
extern sciHandleTab * sciGetpendofhandletab(); /* HANDLE MANAGEMENT */
extern int sciAddNewHandle (sciPointObj * pobj);
extern int sciDelHandle (sciPointObj * pobj);
extern long sciGetHandle (sciPointObj * pobj);
extern sciHandleTab *sciGetHandleTabPointer (sciPointObj * pobj);
extern sciPointObj *sciGetPointerFromHandle (long handle);

extern sciRelationShip *sciGetRelationship (sciPointObj * pobj); /* HANDLE MANAGEMENT */
extern int sciSetParent (sciPointObj * pson, sciPointObj * pparent); /* HANDLE MANAGEMENT */
extern sciPointObj *sciGetParent (sciPointObj * pobj); /* HANDLE MANAGEMENT */
extern void sciSetCurrentSon (sciPointObj * pparent, sciPointObj * pson); /* HANDLE MANAGEMENT */
extern sciPointObj *sciGetCurrentSon (sciPointObj * pobj); /* HANDLE MANAGEMENT */
extern BOOL sciAddThisToItsParent (sciPointObj * pthis,  sciPointObj * pparent); /* HANDLE MANAGEMENT */
extern BOOL sciDelThisToItsParent (sciPointObj * pthis, sciPointObj * pparent); /* HANDLE MANAGEMENT */
extern sciSons *sciGetSons (sciPointObj * pobj); /* HANDLE MANAGEMENT */
extern sciSons *sciGetLastSons (sciPointObj * pobj); /* HANDLE MANAGEMENT */
extern long sciGetCurrentHandle (); /* HANDLE MANAGEMENT */
extern sciPointObj *sciGetCurrentObj ();  /* HANDLE MANAGEMENT */
extern int sciSetCurrentObj (sciPointObj * pobj); /* HANDLE MANAGEMENT */



#endif /* __SCI_HANDLE_MANAGEMENT__ */
