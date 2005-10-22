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

#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "HandleManagement.h"
#include "GetProperty.h"
#include "bcg.h"


#if WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

sciHandleTab * PENDOFHANDLETAB;
sciPointObj * PCURRENTPOBJ;


extern sciPointObj *pfiguremdl;
extern sciPointObj *paxesmdl;


/*********************************** Handle ******************************************/

/**sciSetHandle
 * @memo Sets the handle to this object used only by sciAddNewHandle !!!
 */
void
sciSetHandle (sciPointObj * pobj, sciHandleTab * pvalue)
{
  if ( (pobj != pfiguremdl) && (pobj != paxesmdl))
    switch (sciGetEntityType (pobj))
      {
      case SCI_FIGURE:
      case SCI_SUBWIN:
      case SCI_TEXT:
      case SCI_TITLE:
      case SCI_LEGEND:
      case SCI_ARC:
      case SCI_SEGS: 
      case SCI_FEC: 
      case SCI_GRAYPLOT: 
      case SCI_POLYLINE:
      case SCI_RECTANGLE:
      case SCI_SURFACE:
      case SCI_LIGHT:
      case SCI_AXES:
      case SCI_PANNER:
      case SCI_SBH:
      case SCI_SBV:
      case SCI_MENU:
      case SCI_MENUCONTEXT:
      case SCI_STATUSB:
      case SCI_AGREG:
      case SCI_MERGE:
      case SCI_LABEL: /* F.Leray 28.05.04 */
      case SCI_UIMENU:
	(sciGetRelationship (pobj))->phandle = pvalue;		/** put the new index handle */
	break;
      default:
	break;
      }
}


sciHandleTab *
sciGetpendofhandletab()
{
  return PENDOFHANDLETAB;
}

/**sciAddNewHandle
 * @memo Returns a generated handle for this object, and put the handle and the object in the handle table
 */
int
sciAddNewHandle (sciPointObj * pobj)
{
  sciHandleTab *newhd;
  
  if ((newhd = MALLOC ((sizeof (sciHandleTab)))) == NULL)
    return -1;
  newhd->pprev = PENDOFHANDLETAB;/* We have to use directly PENDOFHANDLETAB and not sciGetHandleTabPointer */
  newhd->pnext = (sciHandleTab *) NULL;
  newhd->index = (long)pobj;/* pour l'instant je prend la valeur du pointeur comme handle !!! */
  
  newhd->pointobj = pobj;
  if (PENDOFHANDLETAB != (sciHandleTab *) NULL)
    PENDOFHANDLETAB->pnext = newhd;
  else
    newhd->pprev = (sciHandleTab *) NULL;
  PENDOFHANDLETAB = newhd;
  
  sciSetHandle (pobj, PENDOFHANDLETAB);
  return 0;
}


/**sciGetHandleTabPointer
 * @memo Returns the handle's pointer address structure from this object
 */
sciHandleTab *
sciGetHandleTabPointer (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_MERGE: 
    case SCI_LABEL:
    case SCI_UIMENU:
      return (sciHandleTab *) ((sciGetRelationship (pobj))->phandle);
    default:
      return (sciHandleTab *) NULL;
      break;
    }
  return (sciHandleTab *) NULL;
}




/**sciDelHandle
 * @memo Removes this pointed handle from the handle table
 */
extern int sciDelHandle
(sciPointObj * pobj)
{
  int tmp = 0;
  sciHandleTab *phandletabtodel;	/* point to a handle structure (prev, value, next) */

  /* We get the handle pointer */
  tmp = 0;
  phandletabtodel = (sciHandleTab *) sciGetHandleTabPointer (pobj);
  if (phandletabtodel == (sciHandleTab *) NULL)
    tmp = 1;
  else
    {
      if (phandletabtodel->pprev == (sciHandleTab *) NULL)
	tmp += 2;
      if (phandletabtodel->pnext == (sciHandleTab *) NULL)
	tmp += 4;
    }
  switch (tmp)
    {
    case 0: /* le phandletabtodel->pprev != NULL et le phandletabtodel->pnext != NULL */
      /*(phandletabtodel->pnext)->pprev = (phandletabtodel->pprev)->pnext; ERREUR */
      (phandletabtodel->pnext)->pprev = phandletabtodel->pprev;
      (phandletabtodel->pprev)->pnext = phandletabtodel->pnext;
      FREE (phandletabtodel);
      break;
    case 2:/* le phandletabtodel->pprev == NULL et le phandletabtodel->pnext !NULL */
      (phandletabtodel->pnext)->pprev = (sciHandleTab *) NULL;
      FREE (phandletabtodel);
      break;
    case 4:/* le phandletabtodel->pprev != NULL et le phandletabtodel->pnext == NULL */
      (phandletabtodel->pprev)->pnext = (sciHandleTab *) NULL;
      PENDOFHANDLETAB = phandletabtodel->pprev;
      FREE (phandletabtodel);    
      break;
    case 6:/* le phandletabtodel->pprev == NULL et le phandletabtodel->pnext == NULL */
      FREE (phandletabtodel);
      PENDOFHANDLETAB = (sciHandleTab *) NULL;
      break;
    case 1:/* the handeltab is empty */
    case 3:/* in prevision */
    case 5:/* in prevision */
    case 7:/* in prevision */
    default:
      sciprint ("no handle to del\n");
      return -1;
      break;
    }
  return 0;
}


/**sciGetHandle
 * @memo Returns the handle 
 */
long sciGetHandle (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_MERGE:  
    case SCI_LABEL: /* F.Leray 27.05.04 */
    case SCI_UIMENU:
      return (sciGetRelationship (pobj))->phandle->index;
      break;
    default:
      sciprint("no handle for this object !\n");
      return -1;
      break;
    }
  return -1;
}



/**sciGetPointFromHandle
 * @memo Returns the object pointer form the handle argument
 */
sciPointObj *
sciGetPointerFromHandle (long handle)
{

  sciHandleTab *phandletab;
  if ( handle != sciGetHandle(pfiguremdl) && handle != sciGetHandle(paxesmdl))
    {
      phandletab = PENDOFHANDLETAB;
      while ((phandletab != NULL) && (phandletab->index != handle))
	phandletab = phandletab->pprev;
      
      if (phandletab == NULL)
	{
	  /* sciprint ("this is not or no more a valid handle !!\n");  F.Leray Adding 'or no more' */
	  return (sciPointObj *) NULL;
	}  
      return (sciPointObj *) phandletab->pointobj;
    }
  else if ( handle == sciGetHandle(pfiguremdl))
    return (sciPointObj *) pfiguremdl;
  else if ( handle == sciGetHandle(paxesmdl))
    return (sciPointObj *) paxesmdl;
  else
    {
      /* sciprint ("this is not or no more a valid handle !!\n");  F.Leray Adding 'or no more' */
      return (sciPointObj *) NULL;
    }
    
}

/************************************************ End Handle *************************************************/




/**sciGetRelationship
 * @memo Returns the structure of the relationship. Do not use this in the Consturctor Functions !
 */
sciRelationShip *
sciGetRelationship (sciPointObj * pobj)
{
  sciRelationShip *tmp=NULL;
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return &(pFIGURE_FEATURE (pobj)->relationship);
      break;
    case SCI_SUBWIN:
      return  &(pSUBWIN_FEATURE (pobj)->relationship);
      break;
    case SCI_TEXT:
      return  &(pTEXT_FEATURE (pobj)->relationship);
      break;
    case SCI_TITLE:
      return  &(pTITLE_FEATURE (pobj)->text.relationship);
      break;
    case SCI_LEGEND:
      return  &(pLEGEND_FEATURE (pobj)->text.relationship);
      break;
    case SCI_ARC:
      return  &(pARC_FEATURE (pobj)->relationship);
      break;
    case SCI_SEGS: 
      return  &(pSEGS_FEATURE (pobj)->relationship);
      break; 
    case SCI_FEC:  
      return  &(pFEC_FEATURE (pobj)->relationship);
      break;
    case SCI_GRAYPLOT:
      return  &(pGRAYPLOT_FEATURE (pobj)->relationship);
      break;
  
    case SCI_POLYLINE:
      return  &(pPOLYLINE_FEATURE (pobj)->relationship);
      break;
    case SCI_RECTANGLE:
      return  &(pRECTANGLE_FEATURE (pobj)->relationship);
      break;
    case SCI_SURFACE:
      return  &(pSURFACE_FEATURE (pobj)->relationship);
      break;
    case SCI_LIGHT:
      return  &(pLIGHT_FEATURE (pobj)->relationship);
      break;
    case SCI_AXES:
      return  &(pAXES_FEATURE (pobj)->relationship);
      break;
    case SCI_PANNER:
      return  &(pPANNER_FEATURE (pobj)->relationship);
      break;
    case SCI_SBH:
      return  &(pSBH_FEATURE (pobj)->relationship);
      break;
    case SCI_SBV:
      return  &(pSBV_FEATURE (pobj)->relationship);
      break;
    case SCI_MENU:
      return  &(pMENU_FEATURE (pobj)->relationship);
      break;
    case SCI_MENUCONTEXT:
      return  &(pMENUCONTEXT_FEATURE (pobj)->relationship);
      break;
    case SCI_STATUSB:
      return  &(pSTATUSB_FEATURE (pobj)->relationship);
      break;
    case SCI_AGREG:
      return  &(pAGREG_FEATURE (pobj)->relationship);
      break; 
    case SCI_MERGE:
      return  &(pMERGE_FEATURE (pobj)->relationship);
      break;
    case SCI_LABEL: /* F.Leray 27.05.04 */
      return  &(pLABEL_FEATURE (pobj)->text.relationship);
      break;
    case SCI_UIMENU: 
      tmp=&(pUIMENU_FEATURE (pobj)->label.relationship);
      return  &(pUIMENU_FEATURE (pobj)->label.relationship);
      break;
    default:
      return (sciRelationShip *) NULL;
      break;
    }
  return (sciRelationShip *) NULL;
}


/**sciSetParent
 * @memo Sets the parent to this object (that have to be the son). 
 * The parent's FIGURE has to be NULL
 * pson est l'objet courant et *pparent est le parent a lui associer
 */
int
sciSetParent (sciPointObj * pson, sciPointObj * pparent)
{	
  switch (sciGetEntityType (pson))
    {
    case SCI_FIGURE:
      /* the figure has no parent */
      if (pparent != (sciPointObj *)NULL)
	return -1;
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_SUBWIN:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_TEXT:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_TITLE:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_LEGEND:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_ARC:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_SEGS:  
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_FEC: 
      (sciGetRelationship (pson))->pparent = pparent;
      break; 
    case SCI_GRAYPLOT:
      (sciGetRelationship (pson))->pparent = pparent;
      break; 
    case SCI_POLYLINE:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_RECTANGLE:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_SURFACE:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_LIGHT:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_AXES:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_PANNER:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_SBH:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_SBV:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_MENU:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_MENUCONTEXT:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_STATUSB:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_AGREG:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_MERGE:
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_LABEL: /* F.Leray 27.05.04 */
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    case SCI_UIMENU: 
      (sciGetRelationship (pson))->pparent = pparent;
      break;
    default:
      return -1;
      break;
    }
  return 0;
}


/**sciGetParent
 * @memo Returns the pointer to the parent object
 */
sciPointObj *
sciGetParent (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_LEGEND:
      return (sciPointObj *) (pLEGEND_FEATURE (pobj)->text.relationship).pparent;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_ARC:
    case SCI_SEGS:  
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_MERGE:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break; 
	
    default:
      break;
    }
  return (sciPointObj *) NULL;
}


/****************************************** SONS ******************************************/

/**sciSetCurrentSon
 * @memo Sets the selected son to this object (that have to be the parent)
 */
void
sciSetCurrentSon (sciPointObj * pparent, sciPointObj * pson)
{
  switch (sciGetEntityType (pparent))
    {
    case SCI_FIGURE:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_SUBWIN:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_TEXT:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_TITLE:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_LEGEND:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_ARC:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_SEGS: 
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;   
    case SCI_FEC:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;  
    case SCI_GRAYPLOT:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break; 
    case SCI_POLYLINE:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_RECTANGLE:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_SURFACE:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_LIGHT:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_AXES:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_PANNER:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_SBH:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_SBV:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_MENU:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_MENUCONTEXT:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_STATUSB:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_AGREG:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_MERGE:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;  
    case SCI_LABEL: /* F.Leray 28.05.04 */
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break; 
    case SCI_UIMENU:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break; 
    default:
      break;
    }
}


/**sciGetCurrentSon
 * @memo Returns the point to the selected son object
 */
sciPointObj *
sciGetCurrentSon (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_SUBWIN:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_TEXT:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_TITLE:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_LEGEND:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_ARC:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_SEGS:  
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_FEC: 
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_GRAYPLOT: 
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_POLYLINE:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_RECTANGLE:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_SURFACE:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_LIGHT:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_AXES:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_PANNER:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_SBH:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_SBV:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_MENU:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_MENUCONTEXT:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_STATUSB:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_AGREG:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_MERGE:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 : normally useless... */
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    case SCI_UIMENU:
      return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
      break;
    default:
      return (sciPointObj *) NULL;
      break;
    }
  return (sciPointObj *) NULL;
}


/**sciAddThisToItsParent
 * @memo Sets this object to its parent. 
 * The list is pointed from the newer to the older.
 */
BOOL
sciAddThisToItsParent (sciPointObj * pthis, sciPointObj * pparent)
{
  sciSons *OneSon = (sciSons *) NULL;
  
  if (sciSetParent(pthis, pparent) == -1)
    return FALSE;

  switch (sciGetEntityType (pthis))
    {
    case SCI_FIGURE:		
      /* on ne fait rien puisqu'il ne peut y avoir un parent dans ce cas */
      return TRUE;
    case SCI_SUBWIN:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_TITLE:
    case SCI_TEXT:
    case SCI_LEGEND:
    case SCI_RECTANGLE:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_ARC:
    case SCI_AGREG:
    case SCI_MERGE: 
    case SCI_LABEL: /* F.Leray 27.05.04 */
    case SCI_UIMENU:
      /* Si c'est null alors il n'y a pas encore de fils d'affecte */
      if ((sciSons *) (sciGetRelationship (pparent)->psons) != NULL)
	{			
	  /* Il existe au moins un fils d'affecte */
	  /* on cree la nouvelle variable */
	  if ((OneSon = MALLOC ((sizeof (sciSons)))) == NULL)
	    return FALSE;
	  OneSon->pnext = (sciSons *)(sciGetRelationship (pparent)->psons);
	  OneSon->pprev = (sciSons *)NULL;
	  (sciGetRelationship (pparent)->psons)->pprev = (sciSons *)OneSon;
	}
      else
	{			/* C'est tout neuf alors on cree la variable */
	  if ((OneSon = MALLOC ((sizeof (sciSons)))) == NULL)
	    return FALSE;
	  OneSon->pnext = (sciSons *)NULL;
	  OneSon->pprev = (sciSons *)NULL;
	  sciGetRelationship (pparent)->plastsons = (sciSons *)OneSon;
	}
      OneSon->pointobj = pthis;
      sciGetRelationship (pparent)->psons = (sciSons *)OneSon;
      return TRUE;
      break;
    default:
      return FALSE;
      break;
    }
  return FALSE;
}


/**sciDelThisToItsParent
 * @memo deletes this son object to its parent, 
 * free the son structure, but not the son object structure 
 * (for which its parent is set to null) , 
 * that have to be free manually or packed to another parent
 */
BOOL
sciDelThisToItsParent (sciPointObj * pthis, sciPointObj * pparent)
{
  int tmp = 0;
  sciSons *OneSon = (sciSons *) NULL;
  sciSons *OneSonprev = (sciSons *) NULL;

  switch (sciGetEntityType (pthis))
    {

    case SCI_FIGURE:
      /* on ne fait rien puisqu'il ne peut y avoir un parent dans ce cas */
      return TRUE;
    case SCI_POLYLINE:
      /*      printf("je vais detruire le lien entre polyline et subwin\n"); fflush(NULL); */
    case SCI_AGREG:
    case SCI_SUBWIN:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_TITLE:
    case SCI_TEXT:
    case SCI_LEGEND:
    case SCI_RECTANGLE:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 

    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_ARC:
    case SCI_MERGE:
    case SCI_LABEL:
    case SCI_UIMENU:
      /* recherche de l'objet a effacer*/
      OneSon = (sciGetRelationship (pparent)->psons);
      OneSonprev = OneSon;
      tmp = 0;
      while ( (OneSon != NULL) &&  (OneSon->pointobj != pthis) )
	{
	  OneSonprev = OneSon;/* on garde une trace du precedent*/
	  OneSon = (sciSons *) OneSon->pnext;
	}/* fin du while */
      /* dans quel cas de figure somme nous ? */
      if (OneSon == (sciSons *)NULL)
	tmp += 1;
      else 
	{
	  if (OneSon->pprev == (sciSons *)NULL)
	    tmp += 2;
	  if (OneSon->pnext == (sciSons *)NULL)
	    tmp += 4;
	}
      switch(tmp)
	{
	case 0:/* ok<-OneSon->ok     */
	  (OneSon->pnext)->pprev = (OneSon->pprev);
	  (OneSon->pprev)->pnext = (OneSon->pnext);
	  FREE(OneSon);
	  return TRUE;
	  break;
	case 2:/* ok<-OneSon->NULL   */
	  (sciGetRelationship (pparent)->psons) = OneSon->pnext;
	  (sciGetRelationship (pparent)->psons)->pprev = (sciSons *)NULL;
	  FREE(OneSon);
	  return TRUE;
	  break;
	case 4:/* NULL<-OneSon->ok   */
	  sciGetRelationship (pparent)->plastsons = OneSon->pprev;
	  (sciGetRelationship (pparent)->plastsons)->pnext = (sciSons *)NULL;
	  FREE(OneSon);
	  return TRUE;
	  break;
	case 6:/* NULL<-OneSon->NULL */
	  sciGetRelationship (pparent)->plastsons = NULL;
	  sciGetRelationship (pparent)->psons = NULL;
	  FREE(OneSon);
	  return TRUE;
	  break;
	case 1:/* OneSon == NULL     */
	case 3:
	case 5:
	case 7:
	default :
	  sciprint ("There is no Son in this Parent!!!!\n");
	  return FALSE;
	  break;
	}
      break;
    default:
      return FALSE;
      break;
    }
  return FALSE;
}


/**sciGetSons
 * @memo Returns the pointer to the table of all sons objects. 
 * There is no SetSons, because a new Son calls sciAddThisToItsParent() it self
 */
sciSons *
sciGetSons (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_SUBWIN:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_TEXT:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_TITLE:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_LEGEND:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_ARC:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_SEGS: 
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;  
    case SCI_FEC:  
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break; 
    case SCI_GRAYPLOT: 
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break; 
    case SCI_POLYLINE:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_RECTANGLE:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_SURFACE:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_LIGHT:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_AXES:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_PANNER:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_SBH:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_SBV:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_MENU:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_MENUCONTEXT:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_STATUSB:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_AGREG:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_MERGE:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_LABEL: /* F.Leray 28.05.04  */
      
      /*printf("(sciGetRelationship (pobj)->psons = %d\n", (sciGetRelationship (pobj)->psons));*/
      
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;
    case SCI_UIMENU:
      return (sciSons *) (sciGetRelationship (pobj)->psons);
      break;

    default:
      return (sciSons *) NULL;
      break;
    }
  return (sciSons *) NULL;
}


/**sciGetLastSons
 * @memo Returns the pointer to the last son (in fact the first created and drawn). 
 * There is no SetSons, because a new Son calls sciAddThisToItsParent() it self
 */
sciSons *
sciGetLastSons (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_SUBWIN:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_TEXT:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_TITLE:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_LEGEND:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_ARC:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_SEGS: 
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_FEC: 
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_GRAYPLOT:  
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_POLYLINE:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_RECTANGLE:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_SURFACE:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_LIGHT:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_AXES:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_PANNER:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_SBH:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_SBV:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_MENU:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_MENUCONTEXT:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_STATUSB:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_AGREG:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_MERGE:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 : normally useless... */
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    case SCI_UIMENU:
      return (sciSons *)sciGetRelationship (pobj)->plastsons;
      break;
    default:
      return (sciSons *) NULL;
      break;
    }
  return (sciSons *) NULL;
}



/************************************ END SONS **************************************/


/**sciGetCurrentHandle()
 * @memo returns the handle of the current object
 */
long
sciGetCurrentHandle ()
{
  return sciGetHandle (sciGetCurrentObj ());
}




/**sciGetCurrentObj()
 * @memo Returns the pointer to the current selected object (actualy figure object). Duplicated in the peri*.c files.
 */
sciPointObj *
sciGetCurrentObj ()
{
  return (sciPointObj *) PCURRENTPOBJ;
}



/**sciSetCurrentObj()
 * @memo sets the pointer to the current selected object (actualy figure object). Used after contructor function.
 */
int
sciSetCurrentObj (sciPointObj * pobj)
{
  /* pcurrentobj is a static variable */
  /* if(PCURRENTPOBJ != NULL){
     sciprint (" \nIN sciSetCurrentObj, BEF setting, *PCURRENTPOBJ = %d\r\n",&(*PCURRENTPOBJ));
     sciprint (" IN sciSetCurrentObj, BEF setting, *PCURRENTPOBJ->EntityType = %s\r\n\n",sciGetCharEntityType(PCURRENTPOBJ));}
     else
     sciprint ("\nPCURRENTPOBJ is NULL (Only at the beginning normally)\n");*/
  
  PCURRENTPOBJ = pobj;
  /*
    sciprint (" IN sciSetCurrentObj, AFTER setting, *PCURRENTPOBJ = %d\r\n",&(*PCURRENTPOBJ));
    sciprint (" IN sciSetCurrentObj, AFTER setting, *PCURRENTPOBJ->EntityType = %s\r\n",sciGetCharEntityType(PCURRENTPOBJ));
    sciprint ("------------------------------------------------------------------------\n");*/
  return 0;
}

