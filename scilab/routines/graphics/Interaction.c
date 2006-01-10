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

#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "Interaction.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "DrawObjects.h"

#if WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

/**sciAddCallback
 * @memo exec the callback associated with pthis
 */
int
sciAddCallback (sciPointObj * pthis,char *code, int len, int mevent )
{
  /* Destruction de l'ancienne callback si elle existait */
  sciDelCallback (pthis);

  switch (sciGetEntityType (pthis))
    {
    case SCI_SUBWIN:
      if ((pSUBWIN_FEATURE (pthis)->callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  sciprint("No more Memory allocation !\n");
	  return -1;
	}
      else 
	{
	  strncpy(pSUBWIN_FEATURE (pthis)->callback, code, len);
	  pSUBWIN_FEATURE (pthis)->callbacklen = len; 
	  pSUBWIN_FEATURE (pthis)->callbackevent = mevent;
	}
      break;
    case SCI_ARC:
      if ((pARC_FEATURE (pthis)->callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  sciprint("No more Memory allocation !\n");
	  return -1;
	}
      else 
	{
	  strncpy(pARC_FEATURE (pthis)->callback, code, len);
	  pARC_FEATURE (pthis)->callbacklen = len;
	  pARC_FEATURE (pthis)->callbackevent = mevent;
                     
	}
      break;
    case SCI_RECTANGLE:
      if ((pRECTANGLE_FEATURE (pthis)->callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  sciprint("No more Memory allocation !\n");
	  return -1;
	}
      else 
	{
	  strncpy(pRECTANGLE_FEATURE (pthis)->callback, code, len);
	  pRECTANGLE_FEATURE (pthis)->callbacklen = len;
	  pRECTANGLE_FEATURE (pthis)->callbackevent = mevent;
                       
	}
      break;
    case SCI_SEGS:  
      if ((pSEGS_FEATURE (pthis)->callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  sciprint("No more Memory allocation !\n");
	  return -1;
	}
      else 
	{
	  strncpy(pSEGS_FEATURE (pthis)->callback, code, len);
	  pPOLYLINE_FEATURE (pthis)->callbacklen = len;
	  pPOLYLINE_FEATURE (pthis)->callbackevent = mevent;
                       
	}
      break;
    case SCI_FEC:  
      if ((pFEC_FEATURE (pthis)->callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  sciprint("No more Memory allocation !\n");
	  return -1;
	}
      else 
	{
	  strncpy(pFEC_FEATURE (pthis)->callback, code, len);
	  pPOLYLINE_FEATURE (pthis)->callbacklen = len;
	  pPOLYLINE_FEATURE (pthis)->callbackevent = mevent;
                       
	}
      break;
    case SCI_GRAYPLOT: 
      if ((pGRAYPLOT_FEATURE (pthis)->callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  sciprint("No more Memory allocation !\n");
	  return -1;
	}
      else 
	{
	  strncpy(pGRAYPLOT_FEATURE (pthis)->callback, code, len);
	  pPOLYLINE_FEATURE (pthis)->callbacklen = len;
	  pPOLYLINE_FEATURE (pthis)->callbackevent = mevent;
                       
	}
      break;
    case SCI_POLYLINE:
      if ((pPOLYLINE_FEATURE (pthis)->callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  sciprint("No more Memory allocation !\n");
	  return -1;
	}
      else 
	{
	  strncpy(pPOLYLINE_FEATURE (pthis)->callback, code, len);
	  pPOLYLINE_FEATURE (pthis)->callbacklen = len;
	  pPOLYLINE_FEATURE (pthis)->callbackevent = mevent;
                        
	}
      break;
    case SCI_UIMENU:
      if ((pUIMENU_FEATURE (pthis)->label.callback = CALLOC (len+1, sizeof (char))) == NULL)
	{
	  sciprint("No more Memory allocation !\n");
	  return -1;
	}
      else 
	{
	  strncpy(pUIMENU_FEATURE (pthis)->label.callback, code, len);
	  pUIMENU_FEATURE (pthis)->label.callbacklen = len;
	}
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_FIGURE:
    case SCI_SBV:
    case SCI_SBH:
    case SCI_LIGHT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
	
    default:
      sciprint ("\r\n No Callback is associated with this Entity");
      return -1;
      break;
    }
  return -1;
}




/**sciGetCallback
 * @memo returns the scilab callback code
 */
char *sciGetCallback(sciPointObj * pthis)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_SUBWIN:
      return (char *)(pSUBWIN_FEATURE(pthis)->callback);
      break;
    case SCI_ARC:
      return (char *)(pARC_FEATURE(pthis)->callback);
      break;
    case SCI_SEGS: 
      return (char *)(pSEGS_FEATURE(pthis)->callback);
      break; 
    case SCI_FEC: 
      return (char *)(pFEC_FEATURE(pthis)->callback);
      break; 
    case SCI_GRAYPLOT: 
      return (char *)(pGRAYPLOT_FEATURE(pthis)->callback);
      break;
    case SCI_POLYLINE:
      return (char *)(pPOLYLINE_FEATURE(pthis)->callback);
      break;
    case SCI_RECTANGLE:
      return (char *)(pRECTANGLE_FEATURE(pthis)->callback);
      break;
    case SCI_TEXT:
      return (char *)(pTEXT_FEATURE(pthis)->callback);
      break;
    case SCI_SURFACE:
      return (char *)(pSURFACE_FEATURE(pthis)->callback);
      break;
    case SCI_AXES:	
      return (char *)(pAXES_FEATURE(pthis)->callback);
      break;
    case SCI_UIMENU:
      return (char *)(pUIMENU_FEATURE(pthis)->label.callback);
      break;
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_FIGURE:
    case SCI_SBV:
    case SCI_SBH:
    case SCI_LIGHT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("\r\nNo Callback is associetad with this Entity");
      return (char *)NULL;
      break;
    }
  return (char *)NULL;
}

/* 07/11/2002*/
/**sciGetCallbackMouseEvent
 * @author Djalel Abdemouche
 */
int sciGetCallbackMouseEvent(sciPointObj * pthis)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE(pthis)->callbackevent;
      break;
    case SCI_ARC:
      return pARC_FEATURE(pthis)->callbackevent;
      break;
    case SCI_SEGS: 
      return pSEGS_FEATURE(pthis)->callbackevent;
      break; 
    case SCI_FEC: 
      return pFEC_FEATURE(pthis)->callbackevent;
      break; 
    case SCI_GRAYPLOT: 
      return pGRAYPLOT_FEATURE(pthis)->callbackevent;
      break;
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE(pthis)->callbackevent;
      break;
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE(pthis)->callbackevent;
      break;
    case SCI_TEXT:
      return pTEXT_FEATURE(pthis)->callbackevent;
      break;
    case SCI_SURFACE:
      return pSURFACE_FEATURE(pthis)->callbackevent;
      break;
    case SCI_AXES:	
      return pAXES_FEATURE(pthis)->callbackevent;
      break;
    case SCI_UIMENU:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_FIGURE:
    case SCI_SBV:
    case SCI_SBH:
    case SCI_LIGHT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("\r\nNo Callback is associated with this Entity");
      return 100;
      break;
    }
  return 100;
}/**sciSetCallbackMouseEvent
  * @author Djalel Abdemouche
  */
int sciSetCallbackMouseEvent(sciPointObj * pthis, int mevent)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_SUBWIN:
      pSUBWIN_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_ARC:
      pARC_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_SEGS: 
      pSEGS_FEATURE(pthis)->callbackevent = mevent;
      break; 
    case SCI_FEC: 
      pFEC_FEATURE(pthis)->callbackevent = mevent;
      break; 
    case SCI_GRAYPLOT: 
      pGRAYPLOT_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_POLYLINE:
      pPOLYLINE_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_RECTANGLE:
      pRECTANGLE_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_TEXT:
      pTEXT_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_SURFACE:
      pSURFACE_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_AXES:	
      pAXES_FEATURE(pthis)->callbackevent = mevent;
      break;
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_FIGURE:
    case SCI_SBV:
    case SCI_SBH:
    case SCI_LIGHT:
    case SCI_AGREG:
    case SCI_UIMENU:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("\r\nNo Callback is associated with this Entity");
      return 100;
      break;
    }
  return 100;
}




/**sciGetCallbackLen
 * @memo returns the length of the scilab callback code
 */
int
sciGetCallbackLen (sciPointObj * pthis)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE (pthis)->callbacklen;
      break;
    case SCI_ARC:
      return pARC_FEATURE (pthis)->callbacklen;
      break;
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE (pthis)->callbacklen;
      break;
    case SCI_SEGS: 
      return pSEGS_FEATURE (pthis)->callbacklen;
      break; 
    case SCI_FEC:  
      return pFEC_FEATURE (pthis)->callbacklen;
      break;  
    case SCI_GRAYPLOT: 
      return pGRAYPLOT_FEATURE (pthis)->callbacklen;
      break;     
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE (pthis)->callbacklen;
      break;    
    case SCI_UIMENU:
      return pUIMENU_FEATURE(pthis)->label.callbacklen;
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_FIGURE:
    case SCI_SBV:
    case SCI_SBH:
    case SCI_LIGHT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("\r\nNo Callback is associated with this Entity");
      return -1;
      break;
    }
  return -1;
}



/**sciDelCallback
 * @memo returns the length of the scilab callback code
 */
int
sciDelCallback (sciPointObj * pthis)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_SUBWIN:
      pSUBWIN_FEATURE (pthis)->callbacklen = 0;
      pSUBWIN_FEATURE (pthis)->callbackevent = 100;
      FREE(pSUBWIN_FEATURE (pthis)->callback);
      pSUBWIN_FEATURE (pthis)->callback = NULL;
      break;
    case SCI_ARC:
      pARC_FEATURE (pthis)->callbacklen = 0;
      pARC_FEATURE (pthis)->callbackevent = 100;
      FREE(pARC_FEATURE (pthis)->callback);
      pARC_FEATURE (pthis)->callback = NULL;
      break;
    case SCI_RECTANGLE:
      pRECTANGLE_FEATURE (pthis)->callbacklen = 0;
      pRECTANGLE_FEATURE (pthis)->callbackevent = 100;
      FREE(pRECTANGLE_FEATURE (pthis)->callback);
      pRECTANGLE_FEATURE (pthis)->callback = NULL;
      break;
    case SCI_SEGS:  
      pSEGS_FEATURE (pthis)->callbacklen = 0;
      pSEGS_FEATURE (pthis)->callbackevent = 100;
      FREE(pSEGS_FEATURE (pthis)->callback);
      pSEGS_FEATURE (pthis)->callback = NULL;
      break;
    case SCI_FEC:
      pFEC_FEATURE (pthis)->callbacklen = 0;
      pFEC_FEATURE (pthis)->callbackevent = 100;
      FREE(pFEC_FEATURE (pthis)->callback);
      pFEC_FEATURE (pthis)->callback = NULL;
      break;  
    case SCI_GRAYPLOT:
      pGRAYPLOT_FEATURE (pthis)->callbacklen = 0; 
      pGRAYPLOT_FEATURE (pthis)->callbackevent = 100;
      FREE(pGRAYPLOT_FEATURE (pthis)->callback);
      pGRAYPLOT_FEATURE (pthis)->callback = NULL;
      break; 
    case SCI_POLYLINE:
      pPOLYLINE_FEATURE (pthis)->callbacklen = 0;
      pPOLYLINE_FEATURE (pthis)->callbackevent = 100;
      FREE(pPOLYLINE_FEATURE (pthis)->callback);
      pPOLYLINE_FEATURE (pthis)->callback = NULL;
      break;
    case SCI_UIMENU:
      pUIMENU_FEATURE(pthis)->label.callbacklen=0;
      FREE(pUIMENU_FEATURE(pthis)->label.callback);
      pUIMENU_FEATURE(pthis)->label.callback=NULL;
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_FIGURE:
    case SCI_SBV:
    case SCI_SBH:
    case SCI_LIGHT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("\r\nNo Callback is associated with this Entity");
      return -1;
      break;
    }
  return 0;
}


/**sciExecCallback
 * @memo exec the callback associated with pthis
 */
int
sciExecCallback (sciPointObj * pthis)
{
  int mlhs = 0, mrhs = 1, ibegin = 1, l1, m1, n1 = 1;
  char name[] = "execstr" ;
  m1 = sciGetCallbackLen(pthis);
  switch (sciGetEntityType (pthis))
    {
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_RECTANGLE:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
      if (sciGetCallback(pthis))
	{
	  CreateVar(1, "c", &m1, &n1, &l1);
	  strncpy(cstk(l1), sciGetCallback(pthis), sciGetCallbackLen(pthis));
	  /* back conversion to Scilab coding */
	  Convert2Sci(1);
	  SciString(&ibegin,name,&mlhs,&mrhs);
	  /* check if an error has occured while running a_function */
	  LhsVar(1) = 0; 
	  return 0;
	}
      else sciprint ("\r\nNo Callback is associated with this Entity");
      return 0;
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_FIGURE:
    case SCI_SBV:
    case SCI_SBH:
    case SCI_LIGHT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      sciprint ("\r\nNo Callback is associated with this Entity");
      return -1;
      break;
    }
  return -1;
}

/************************************ End of callback Functions ************************************/


/*-------------------------------------------------------------------------------------------------*/
/* Objmove                                                                                         */
/* move a handle in the graphic window                                                             */
/*-------------------------------------------------------------------------------------------------*/

int Objmove (hdl,d,m,opt)
     long *hdl;
     double* d;
     int m;
     BOOL opt;
{   
  long tmphdl;
  double x,y,z;
  sciPointObj *pobj;  
  sciSons *psonstmp;
  int i,n;
  x=d[0];y=d[1];
  if (m==3) z=d[2]; else z=0.0;

  pobj = (sciPointObj *)sciGetPointerFromHandle(*hdl);
  /*sciSetCurrentObj (pobj);*/  /* Useless*/

  switch (sciGetEntityType (pobj))
    {    
    case SCI_SUBWIN:
      pSUBWIN_FEATURE(pobj)->FRect[0] +=x;
      pSUBWIN_FEATURE(pobj)->FRect[2] +=x;
      pSUBWIN_FEATURE(pobj)->FRect[1] +=y;
      pSUBWIN_FEATURE(pobj)->FRect[3] +=y;
      break;
    case SCI_ARC:
      pARC_FEATURE(pobj)->x +=x;
      pARC_FEATURE(pobj)->y += y; 
      if (m == 3) pRECTANGLE_FEATURE(pobj)->z += z;
      break;
    case SCI_RECTANGLE: 
      pRECTANGLE_FEATURE(pobj)->x += x;  
      pRECTANGLE_FEATURE(pobj)->y += y; 
      if (m == 3) pRECTANGLE_FEATURE(pobj)->z += z;
      break; 
    case SCI_AGREG: 
      psonstmp = sciGetSons((sciPointObj *) pobj);
      while ((psonstmp != (sciSons *)NULL) && (psonstmp->pointobj != (sciPointObj *)NULL))
	{
	  tmphdl =sciGetHandle((sciPointObj *)psonstmp->pointobj);
	  Objmove (&tmphdl,d,m,opt);
	  psonstmp = psonstmp->pnext;
	}
      break;
    case SCI_TEXT:  
      pTEXT_FEATURE(pobj)->x += x; 
      pTEXT_FEATURE(pobj)->y += y;
      if (m == 3) pTEXT_FEATURE(pobj)->z += z;
      break;
    case SCI_SEGS:
      n=pSEGS_FEATURE(pobj)->Nbr1;   
      for (i=0;i<n;i++) {
	pSEGS_FEATURE(pobj)->vx[i] += x; 
	pSEGS_FEATURE(pobj)->vy[i] += y;
      }
      if (m == 3) {
	if  (pSEGS_FEATURE(pobj)->vz == (double *)NULL) {
	  if ((pSEGS_FEATURE(pobj)->vz = MALLOC (n * sizeof (double)))==NULL) return -1;
	    for (i=0;i<n;i++) 
	      pSEGS_FEATURE(pobj)->vz[i] = z; 
	}
	else
	  for (i=0;i<n;i++) 
	    pSEGS_FEATURE(pobj)->vz[i] += z; 
      }
      break;
    case SCI_POLYLINE: 
      n=pPOLYLINE_FEATURE(pobj)->n1;
      for (i=0;i<n;i++) {
	pPOLYLINE_FEATURE(pobj)->pvx[i] += x; 
	pPOLYLINE_FEATURE(pobj)->pvy[i] += y;
      }
      if (m == 3) {
	if  (pPOLYLINE_FEATURE(pobj)->pvz == (double *)NULL) {
	  if ((pPOLYLINE_FEATURE(pobj)->pvz = MALLOC (n * sizeof (double)))==NULL) return -1;
	    for (i=0;i<n;i++) 
	      pPOLYLINE_FEATURE(pobj)->pvz[i] = z; 
	}
	else
	  for (i=0;i<n;i++) 
	    pPOLYLINE_FEATURE(pobj)->pvz[i] += z; 
      }
      break;
    case SCI_FEC: 
      for (i=0;i<pFEC_FEATURE(pobj)->Nnode;i++) {
	pFEC_FEATURE(pobj)->pvecx[i] += x; 
	pFEC_FEATURE(pobj)->pvecy[i] += y;
      }
      break;
    case SCI_GRAYPLOT:   
      for (i=0;i<pGRAYPLOT_FEATURE(pobj)->nx;i++)
	pGRAYPLOT_FEATURE(pobj)->pvecx[i] += x; 
      for (i=0;i<pGRAYPLOT_FEATURE(pobj)->ny;i++)
	pGRAYPLOT_FEATURE(pobj)->pvecy[i] += y;
      break;
    case SCI_SURFACE: 
      switch(pSURFACE_FEATURE (pobj)->typeof3d)
	{
	case SCI_FAC3D: 
	  n= pSURFACE_FEATURE (pobj)->dimzx* pSURFACE_FEATURE (pobj)->dimzy;
	  for (i=0;i<n;i++) {
	    pSURFACE_FEATURE(pobj)->pvecx[i] += x; 
	    pSURFACE_FEATURE(pobj)->pvecy[i] += y;
	  }
	  if (m == 3) {
	    if  (pSURFACE_FEATURE(pobj)->pvecz == (double *)NULL) {
	      if ((pSURFACE_FEATURE(pobj)->pvecz = MALLOC (n * sizeof (double)))==NULL) return -1;
	      for (i=0;i<n;i++) 
		pSURFACE_FEATURE(pobj)->pvecz[i] = z; 
	    }
	    else
	      for (i=0;i<n;i++) 
		pSURFACE_FEATURE(pobj)->pvecz[i] += z; 
	  }
	  break;
	case SCI_PLOT3D:
	  for (i=0;i<pSURFACE_FEATURE (pobj)->dimzx;i++) 
	    pSURFACE_FEATURE(pobj)->pvecx[i] += x; 
	  for (i=0;i<pSURFACE_FEATURE (pobj)->dimzy;i++) 
	    pGRAYPLOT_FEATURE(pobj)->pvecy[i] += y;
	  if (m == 3) {
	    n=pSURFACE_FEATURE (pobj)->dimzx*pSURFACE_FEATURE (pobj)->dimzy;
	    if  (pSURFACE_FEATURE(pobj)->pvecz == (double *)NULL) {
	      if ((pSURFACE_FEATURE(pobj)->pvecz = MALLOC (n * sizeof (double)))==NULL) return -1;
	      for (i=0;i<n;i++) 
		pSURFACE_FEATURE(pobj)->pvecz[i] = z; 
	    }
	    else
	      for (i=0;i<n;i++) 
		pSURFACE_FEATURE(pobj)->pvecz[i] += z; 
	  }
	  break;
	case SCI_CONTOUR:
	case SCI_PARAM3D:
	case SCI_PARAM3D1: /* Nothing to be done */
	  break;
	}
      break;
    case SCI_LABEL:
      pLABEL_FEATURE(pobj)->position[0] += x; 
      pLABEL_FEATURE(pobj)->position[1] += y;
      pLABEL_FEATURE(pobj)->auto_position = FALSE;
      break;
    case SCI_FIGURE:
    case SCI_AXES:
    case SCI_LIGHT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_PANNER:	
    case SCI_SBH:		
    case SCI_SBV:	      
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_UIMENU:
    default:
      sciprint ("This object can not be moved\r\n");
      return -1;
      break;
    }    
  if (opt)
  {
    /*sciDrawObjIfRequired(pobj);*/
    sciRefreshObj( pobj ) ;
  }
  else
  {
    /*sciDrawObj(sciGetParentFigure(pobj));*/
    sciRefreshObj( sciGetParentFigure( pobj ) ) ;
  }
  /* sciDrawObj(sciGetParentFigure(sciGetCurrentObj ()));*/
    
  return 0;
}


BOOL sciIsAreaZoom(box,box1,section)
     integer box[4];
     integer box1[4];
     integer section[4];
{
  if ( (box[0]<=box1[0]) && (box[2]>box1[0]) && (box[2]<=box1[2]) 
       && (box[1]<box1[1]) && (box[3]>box1[1]) && (box[3]<=box1[3]) )
    {section[0]=box1[0];  section[2]=box[2];  section[1]=box1[1];  section[3]=box[3];return TRUE;}
  
  if ( (box[0]<=box1[0]) && (box[2]>box1[0]) && (box[2]<=box1[2]) 
       && (box[1]>=box1[1]) && (box[3]<=box1[3]) )
    {section[0]=box1[0];  section[2]=box[2];  section[1]=box[1];  section[3]=box[3];return TRUE;}
  
  if ( (box[0]<=box1[0]) && (box[2]>box1[0]) && (box[2]<=box1[2]) 
       && (box[1]>=box1[1]) && (box[1]<=box1[3]) && (box[3]>=box1[3]) )
    {section[0]=box1[0];  section[2]=box[2];  section[1]=box[1];  section[3]=box1[3];return TRUE;}

  if ( (box[0]<=box1[0]) && (box[2]>box1[0]) && (box[2]<=box1[2]) 
       && (box[1]<box1[1]) &&  (box[3]>box1[3]) )
    {section[0]=box1[0];  section[2]=box[2];  section[1]=box1[1];  section[3]=box1[3];return TRUE;}
  /*********************/
 
  if ( (box[0]>box1[0])  && (box[2]<=box1[2]) 
       && (box[1]<box1[1]) && (box[3]>box1[1]) && (box[3]<=box1[3]) )
    {section[0]=box[0];  section[2]=box[2];  section[1]=box1[1];  section[3]=box[3]; return TRUE;}
  
  if ( (box[0]>box1[0])  && (box[2]<=box1[2]) 
       && (box[1]>=box1[1]) && (box[3]<=box1[3]) )
    {section[0]=box[0];  section[2]=box[2];  section[1]=box[1];  section[3]=box[3]; return TRUE;}
  
  if ( (box[0]>box1[0])  && (box[2]<=box1[2]) 
       && (box[1]>=box1[1]) && (box[1]<=box1[3]) && (box[3]>=box1[3]) )
    {section[0]=box[0];  section[2]=box[2];  section[1]=box[1];  section[3]=box1[3]; return TRUE;}

  if ( (box[0]>box1[0])  && (box[2]<=box1[2]) 
       && (box[1]<box1[1]) &&  (box[3]>box1[3]) )
    {section[0]=box[0];  section[2]=box[2];  section[1]=box1[1];  section[3]=box1[3];return TRUE;}
  /*********************/ 
  if ( (box[0]>box1[0]) && (box[0]<box1[2]) && (box[2]>=box1[2]) 
       && (box[1]<box1[1]) && (box[3]>box1[1]) && (box[3]<=box1[3]) )
    {section[0]=box[0];  section[2]=box1[2];  section[1]=box1[1];  section[3]=box[3]; return TRUE;}
  
  if ( (box[0]>box1[0]) && (box[0]<box1[2]) && (box[2]>=box1[2]) 
       && (box[1]>=box1[1]) && (box[3]<=box1[3]) )
    {section[0]=box[0];  section[2]=box1[2];  section[1]=box[1];  section[3]=box[3];return TRUE;}
  
  if ( (box[0]>box1[0]) && (box[0]<box1[2]) && (box[2]>=box1[2]) 
       && (box[1]>=box1[1]) && (box[1]<=box1[3]) && (box[3]>=box1[3]) )
    {section[0]=box[0];  section[2]=box1[2];  section[1]=box[1];  section[3]=box1[3];return TRUE;}

  if ( (box[0]>box1[0]) && (box[0]<box1[2]) && (box[2]>=box1[2]) 
       && (box[1]<box1[1]) &&  (box[3]>box1[3]) )
    {section[0]=box[0];  section[2]=box1[2];  section[1]=box1[1];  section[3]=box1[3]; return TRUE;}
  /*********************/  
  if ( (box[0]<box1[0])  && (box[2]>box1[2]) 
       && (box[1]<box1[1]) && (box[3]>box1[1]) && (box[3]<=box1[3]) )
    {section[0]=box1[0];  section[2]=box1[2];  section[1]=box1[1];  section[3]=box[3];return TRUE;}
  
  if ( (box[0]<box1[0])  && (box[2]>box1[2])
       && (box[1]>=box1[1]) && (box[3]<=box1[3]) )
    {section[0]=box1[0];  section[2]=box1[2];  section[1]=box[1];  section[3]=box[3];return TRUE;}
  
  if ( (box[0]<box1[0])  && (box[2]>box1[2])
       && (box[1]>=box1[1]) && (box[1]<=box1[3]) && (box[3]>=box1[3]) )
    {section[0]=box1[0];  section[2]=box1[2];  section[1]=box[1];  section[3]=box1[3];return TRUE;}
  return FALSE;

}


void scizoom(bbox,pobj)
     double bbox[4];
     sciPointObj * pobj;
{
  sciPointObj *psousfen;
  double fmin,fmax,lmin,lmax;
  integer min,max,puiss,deux=2,dix=10;
  psousfen= pobj; /* ??? */

  if ( !( sciGetZooming(pobj) ) )
  {
    sciSetZooming(psousfen, 1);
    /*    pSUBWIN_FEATURE (psousfen)->ZRect_kp[0]   = pSUBWIN_FEATURE (psousfen)->ZRect[0]; */
    /*       pSUBWIN_FEATURE (psousfen)->ZRect_kp[1]   = pSUBWIN_FEATURE (psousfen)->ZRect[1]; */
    /*       pSUBWIN_FEATURE (psousfen)->ZRect_kp[2]   = pSUBWIN_FEATURE (psousfen)->ZRect[2]; */
    /*       pSUBWIN_FEATURE (psousfen)->ZRect_kp[3]   = pSUBWIN_FEATURE (psousfen)->ZRect[3]; */
  }
  /** regraduation de l'axe des axes ***/
  fmin=  bbox[0];
  fmax=  bbox[2];
  if( pSUBWIN_FEATURE (psousfen)->logflags[0] == 'n' )
  {
    C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
    pSUBWIN_FEATURE(psousfen)->axes.xlim[2]=puiss;
    pSUBWIN_FEATURE (psousfen)->ZRect[0]=lmin;
    pSUBWIN_FEATURE (psousfen)->ZRect[2]=lmax;
  }
  else
  {
    pSUBWIN_FEATURE(psousfen)->axes.xlim[2]=0;
    pSUBWIN_FEATURE (psousfen)->ZRect[0]=fmin;
    pSUBWIN_FEATURE (psousfen)->ZRect[2]=fmax;
  }
  
  fmin= bbox[1]; 
  fmax= bbox[3];
  if ( pSUBWIN_FEATURE (psousfen)->logflags[1] == 'n' )
  {
    C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
    pSUBWIN_FEATURE(psousfen)->axes.ylim[2]=puiss;
    pSUBWIN_FEATURE (psousfen)->ZRect[1]=lmin;
    pSUBWIN_FEATURE (psousfen)->ZRect[3]=lmax;
  }
  else
  {
    pSUBWIN_FEATURE(psousfen)->axes.ylim[2]=0;
    pSUBWIN_FEATURE (psousfen)->ZRect[1]=fmin;
    pSUBWIN_FEATURE (psousfen)->ZRect[3]=fmax;
  }

  /* default values when zooming in 3d */
  /* and scizoom takes only xmin ymin xmax ymax AND not zmin zmax (for now at least) */
  /* F.Leray 29.09.05 */
  pSUBWIN_FEATURE (psousfen)->ZRect[4] = pSUBWIN_FEATURE (psousfen)->SRect[4];
  pSUBWIN_FEATURE (psousfen)->ZRect[5] = pSUBWIN_FEATURE (psousfen)->SRect[5];
  
}
