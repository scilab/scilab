/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE  , INRIA 2001-2002
 *    Djalel ABDEMOUCHE  , INRIA 2002-2004
 *    Fabrice Leray      , INRIA 2004-2006
 *    Jean-Baptiste Silvy, INRIA 2005-xxxx
 *    Comment:
 *    This file contains all functions used in graphics handle management.
 --------------------------------------------------------------------------*/

#ifndef __SCI_HANDLE_MANAGEMENT__
#define __SCI_HANDLE_MANAGEMENT__

#include "ObjectStructure.h"

/* Handle functions */
extern void sciSetHandle (sciPointObj * pobj, sciHandleTab * pvalue);
extern sciHandleTab * sciGetpendofhandletab(void); /* HANDLE MANAGEMENT */
extern int sciAddNewHandle (sciPointObj * pobj);
extern int sciDelHandle (sciPointObj * pobj);
extern long sciGetHandle (sciPointObj * pobj);
extern sciHandleTab *sciGetHandleTabPointer (sciPointObj * pobj);
extern sciPointObj *sciGetPointerFromHandle (long handle);

extern sciRelationShip *sciGetRelationship (sciPointObj * pobj); /* HANDLE MANAGEMENT */
extern int sciSetParent (sciPointObj * pson, sciPointObj * pparent); /* HANDLE MANAGEMENT */
extern sciPointObj *sciGetParent (sciPointObj * pobj); /* HANDLE MANAGEMENT */
extern BOOL sciAddThisToItsParent (sciPointObj * pthis,  sciPointObj * pparent); /* HANDLE MANAGEMENT */
extern BOOL sciDelThisToItsParent (sciPointObj * pthis, sciPointObj * pparent); /* HANDLE MANAGEMENT */
extern sciSons *sciGetSons (sciPointObj * pobj); /* HANDLE MANAGEMENT */
extern sciSons *sciGetLastSons (sciPointObj * pobj); /* HANDLE MANAGEMENT */
extern sciSons * sciGetNextAccessibleSon( sciSons * son ) ; /* HANDLE MANAGEMENT */
extern sciSons * sciGetFirstAccessibleSon( sciPointObj * pObj ) ; /* HANDLE MANAGEMENT */
extern sciSons * sciFindSon( sciPointObj * searchedObj, sciPointObj * parentObj ) ;
extern int sciRelocateHandles( unsigned long handles[], int nbHandles, unsigned long newParentHandle ) ;
extern BOOL sciCanBeSonOf( sciPointObj * son, sciPointObj * parent ) ;
extern int swapHandles( unsigned long firstHdl, unsigned long secondHdl ) ;

#endif /* __SCI_HANDLE_MANAGEMENT__ */
