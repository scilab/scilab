#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>


#include "bcg.h"
#include "PloEch.h" 

extern void GPopupResize();
struct BCG *tmpScilabXgc;
int cmptclip=0;
extern int versionflag;
extern void newfec __PARAMS((integer *xm,integer *ym,double *triangles,double *func,integer *Nnode,
		      integer *Ntr,double *zminmax,integer *colminmax));
extern void GraySquare1(integer *x, integer *y, double *z, integer n1, integer n2);
extern void GraySquare(integer *x, integer *y, double *z, integer n1, integer n2);
extern void Plo2d1RealToPixel(integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf);
extern void Plo2d2RealToPixel __PARAMS((integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf));
extern void Plo2d3RealToPixel __PARAMS((integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf));
extern void Plo2d4RealToPixel __PARAMS((integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf));
extern void Champ2DRealToPixel __PARAMS((integer *xm,integer *ym,integer *zm,integer *na,
					 integer *arsize,integer *colored,double *x,double *y,
                                         double  *fx,double *fy,integer *n1,integer *n2,double *arfact));
int DestroyAgregation (sciPointObj * pthis);

/*************************************************************************************************/


/** Functions for entity oriented graphic */


/**
 * This variable is a prev/next structure
 * of the handle, pointing to the table 
 * with te current handle 
 */
static sciHandleTab *pendofhandletab = (sciHandleTab *) NULL;

/**
 * This variable is memory of the current entity selected 
 */
static sciPointObj *pcurrentpobj = (sciPointObj *) NULL;

/**
 * This table is memory of the clipping associated to the objects  
 */
extern sciClipTab ptabclip[15];

/**sciGetPointerToFeature
 * @memo Returns the pointer to features structure from this object Used only for functions FREE or to use void pointer
 */
void *
sciGetPointerToFeature (sciPointObj * pobj)
{

  /* perhaps is it necessary to cast the returns value with void* type ! */
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciFigure *) pFIGURE_FEATURE (pobj);
      break;
    case SCI_SUBWIN:
      return (sciSubWindow *) pSUBWIN_FEATURE (pobj);
      break;
    case SCI_TEXT:
      return (sciText *) pTEXT_FEATURE (pobj);
      break;
    case SCI_TITLE:
      return (sciTitle *) pTITLE_FEATURE (pobj);
      break;
    case SCI_LEGEND:
      return (sciLegend *) pLEGEND_FEATURE (pobj);
      break;
    case SCI_ARC:
      return (sciArc *) pARC_FEATURE (pobj);
      break;
    case SCI_POLYLINE:
      return (sciPolyline *) pPOLYLINE_FEATURE (pobj);
      break;
    case SCI_SEGS:  
      return (sciSegs *) pSEGS_FEATURE (pobj); 
      break;
    case SCI_FEC:
      return (sciFec *) pFEC_FEATURE (pobj); 
      break;
    case SCI_GRAYPLOT:
      return (sciGrayplot *) pGRAYPLOT_FEATURE (pobj);
      break;
    case SCI_PATCH:
      return (sciPatch *) pPATCH_FEATURE (pobj);
      break;
    case SCI_RECTANGLE:
      return (sciRectangle *) pRECTANGLE_FEATURE (pobj);
      break;
    case SCI_SURFACE:
      return (sciSurface *) pSURFACE_FEATURE (pobj);
      break;
    case SCI_LIGHT:
      return (sciLightSource *) pLIGHT_FEATURE (pobj);
      break;
    case SCI_AXIS:
      return (sciAxis *) pAXIS_FEATURE (pobj);
      break;
    case SCI_AXES:
      return (sciAxes *) pAXES_FEATURE (pobj);
      break;
    case SCI_PANNER:
      return (sciPanner *) pPANNER_FEATURE (pobj);
      break;
    case SCI_SBH:
      return (sciScrollBarHorz *) pSBH_FEATURE (pobj);
      break;
    case SCI_SBV:
      return (sciScrollBarVert *) pSBV_FEATURE (pobj);
      break;
    case SCI_MENU:
      return (sciMenu *) pMENU_FEATURE (pobj);
      break;
    case SCI_MENUCONTEXT:
      return (sciMenuContext *) pMENUCONTEXT_FEATURE (pobj);
      break;
    case SCI_STATUSB:
      return (sciStatusBar *) pSTATUSB_FEATURE (pobj);
      break;
    case SCI_AGREG:
      return (sciAgreg *) pAGREG_FEATURE (pobj);
      break;
    default:
      return (void *) NULL;
      break;
    }
  return (void *) NULL;
}


/**sciSetEntityType
 * @memo Sets the type of this Scilab graphic object (pobj)
 */
void
sciSetEntityType (sciPointObj * pobj, sciEntityType value)
{
  if (pobj != (sciPointObj *) NULL)
    pobj->entitytype = value;
}


/**sciGetEntityType
 * @memo Gets the type of this Scilab graphic object return -1 if bad param !
 */
sciEntityType
sciGetEntityType (sciPointObj * pobj)
{
  if (pobj != (sciPointObj *) NULL)
    return pobj->entitytype;
  return -1;
}


/**sciGetCharEntityType
 * @memo Gets the type of this Scilab graphic object in char format return -1 if bad param !
 */
char *
sciGetCharEntityType (sciPointObj * pobj)
{
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return "Figure";
      break;
    case SCI_SUBWIN:
      return "Axes";
      break;
    case SCI_TEXT:
      return "Text";
      break;
    case SCI_TITLE:
      return "Title";
      break;
    case SCI_LEGEND:
      return "Legend";
      break;
    case SCI_ARC:
      return "Arc";
      break; 
    case SCI_POLYLINE:
      return "Polyline";
      break;
    case SCI_SEGS:
      return (pSEGS_FEATURE (pobj)->ptype == 0) ?  "Segs": "champ";
      break; 
    case SCI_FEC: 
      return "Fec";
      break;
    case SCI_GRAYPLOT:
      return (pGRAYPLOT_FEATURE (pobj)->type ==0) ? "Grayplot":"Matplot";
      break;
    case SCI_PATCH:
      return "Patch";
      break;
    case SCI_RECTANGLE:
      return "Rectangle";
      break;
    case SCI_SURFACE:
      switch (pSURFACE_FEATURE (pobj)->typeof3d)
	{
	case SCI_FAC3D:
	  return "Fac3d";
	  break;
	case SCI_PLOT3D:
	  return "Plot3d";
	  break;
	case SCI_PARAM3D:
	case SCI_PARAM3D1:
	  return "Param3d";
	  break;
	case SCI_CONTOUR:

	  break;
	}
      break;
    case SCI_LIGHT:
      return "Light";
      break;
    case SCI_AXIS:
      return "Error";
      break;
    case SCI_AXES:
      return "Axis";
      break;
    case SCI_PANNER:
      return "Panner";
      break;
    case SCI_SBH:
      return "Scollbar_hor    ";
      break;
    case SCI_SBV:
      return "Scollbar_ver";
      break;
    case SCI_MENU:
      return "Menu";
      break;
    case SCI_MENUCONTEXT:
      return "Contexte";
      break;
    case SCI_STATUSB:
      return "Statut_bar";
      break;
    case SCI_AGREG:
      return "Agregation";
      break;
    default:
      return (char *)NULL;
      break;
    }
  return (char *)NULL;
}

/*********************************** Handle ******************************************/

/**sciSetHandle
 * @memo Sets the handle to this object used only by sciAddNewHandle !!!
 */
void
sciSetHandle (sciPointObj * pobj, sciHandleTab * pvalue)
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
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:  
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
      (sciGetRelationship (pobj))->phandle = pvalue;		/** put the new index handle */
      break;
    default:
      break;
    }
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
  newhd->pprev = pendofhandletab;/* We have to use directly pendofhandletab and not sciGetHandleTabPointer */
  newhd->pnext = (sciHandleTab *) NULL;
  newhd->index = (long)pobj;/* pour l'instant je prend la valeur du pointeur comme handle !!! */
  
  newhd->pointobj = pobj;
  if (pendofhandletab != (sciHandleTab *) NULL)
    pendofhandletab->pnext = newhd;
  else
    newhd->pprev = (sciHandleTab *) NULL;
  pendofhandletab = newhd;
  
  sciSetHandle (pobj, pendofhandletab);
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
    case SCI_PATCH:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:  
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
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
extern int
sciDelHandle (sciPointObj * pobj)
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
      pendofhandletab = phandletabtodel->pprev;
	  FREE (phandletabtodel);    
      break;
    case 6:/* le phandletabtodel->pprev == NULL et le phandletabtodel->pnext == NULL */
	  FREE (phandletabtodel);
      pendofhandletab = (sciHandleTab *) NULL;
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
long
sciGetHandle (sciPointObj * pobj)
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
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:  
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
      return (sciGetRelationship (pobj))->phandle->index;
      break;
    default:
		sciprint("no handle for this object!\n");
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

  phandletab = pendofhandletab;
  while ((phandletab != NULL) && (phandletab->index != handle))
    phandletab = phandletab->pprev;

  if (phandletab == NULL)
    {
      sciprint ("this is not a valid handle !!\n");
      return (sciPointObj *) NULL;
    }

  return (sciPointObj *) phandletab->pointobj;
}

/************************************************ End Handle *************************************************/



/****************************** 
 * Graphic Context Functions  *
 ******************************/

 
 
/**sciGetGraphicContext
 * @memo Returns the structure of the Graphic Context. Do not use this in the Consturctor Functions !
 */
sciGraphicContext *
sciGetGraphicContext (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return &(pFIGURE_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_SUBWIN:
      return  &(pSUBWIN_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_ARC:
      return  &(pARC_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_PATCH:
      return  &(pPATCH_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_SEGS: 
      return  &(pSEGS_FEATURE (pobj)->graphiccontext);
      break; 
    case SCI_FEC:
      return  &(pFEC_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_GRAYPLOT: 
      return  &(pGRAYPLOT_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_POLYLINE:
      return  &(pPOLYLINE_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_RECTANGLE:
      return  &(pRECTANGLE_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_SURFACE:
      return  &(pSURFACE_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_LIGHT:
      return  &(pLIGHT_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_AXIS:
      return  &(pAXIS_FEATURE (pobj)->graphiccontext);
      break; 
    case SCI_AXES:
      return  &(pAXES_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_MENU:
      return  &(pMENU_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_MENUCONTEXT:
      return  &(pMENUCONTEXT_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_STATUSB:
      return  &(pSTATUSB_FEATURE (pobj)->graphiccontext);
      break;
    case SCI_TEXT:
    case SCI_AGREG:
    case SCI_TITLE:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_LEGEND:
    default:
      return (sciGraphicContext *) NULL;
      break;
    }
  return (sciGraphicContext *) NULL;
}


/**sciInitGraphicContext
 * @memo Inits the graphic context of this object with the default value. the new graphic object inherits parent's features by sciGetParent()
 */
int
sciInitGraphicContext (sciPointObj * pobj)
{
  /* 
   * initialisation du contexte graphique par defaut 
   * que l'on peut recuperer sur les structure de base de scilab 
   * la colormap des fils est heritee du parent
   */
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicContext(pobj))->backgroundcolor = 33;
      (sciGetGraphicContext(pobj))->foregroundcolor = 32;
      (sciGetGraphicContext(pobj))->fillstyle = HS_HORIZONTAL;
      (sciGetGraphicContext(pobj))->fillcolor = (sciGetGraphicContext(pobj))->backgroundcolor;
      (sciGetGraphicContext(pobj))->linewidth = 1;
      (sciGetGraphicContext(pobj))->linestyle = PS_SOLID;
      (sciGetGraphicContext(pobj))->ismark    = FALSE;
      (sciGetGraphicContext(pobj))->markstyle = 0;
      return 0;
      break;
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:    
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
      (sciGetGraphicContext(pobj))->backgroundcolor =	sciGetBackground (sciGetParent (pobj)) - 1;
      (sciGetGraphicContext(pobj))->foregroundcolor =	sciGetForeground (sciGetParent (pobj)) - 1;
      (sciGetGraphicContext(pobj))->fillstyle =	sciGetFillStyle (sciGetParent (pobj));
      (sciGetGraphicContext(pobj))->fillcolor = (sciGetGraphicContext(pobj))->backgroundcolor;
      (sciGetGraphicContext(pobj))->linewidth =	sciGetLineWidth (sciGetParent (pobj));
      (sciGetGraphicContext(pobj))->linestyle =	sciGetLineStyle (sciGetParent (pobj));
      (sciGetGraphicContext(pobj))->ismark    =	sciGetIsMark (sciGetParent (pobj));
      (sciGetGraphicContext(pobj))->markstyle =	sciGetMarkStyle (sciGetParent (pobj));
      return 0;
      break;
    case SCI_AGREG:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    default:
      return -1;
      break;
    }
  return 0;
}

 
/**sciGetGraphicMode
 * @memo Returns the structure of the Graphic Context. Do not use this in the Consturctor Functions !
 */
scigMode *sciGetGraphicMode (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return &(pFIGURE_FEATURE (pobj)->gmode);
      break;
    case SCI_SUBWIN:
      return &(pSUBWIN_FEATURE (pobj)->gmode);
      break;
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS: 
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_LEGEND:
    default:
      return (scigMode *) NULL;
      break;
    }
  return (scigMode *) NULL;
}


/**sciInitGraphicMode
 * @memo Inits the graphic mode of this object with the default value
 */
int
sciInitGraphicMode (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicMode (pobj))->wresize = TRUE;
      (sciGetGraphicMode (pobj))->addplot = TRUE;
      (sciGetGraphicMode (pobj))->autoscaling = TRUE;
      (sciGetGraphicMode (pobj))->zooming = FALSE;
      (sciGetGraphicMode (pobj))->oldstyle = FALSE;
      (sciGetGraphicMode (pobj))->xormode = 3;
      break;
    case SCI_SUBWIN:
      (sciGetGraphicMode (pobj))->wresize =sciGetResize (sciGetParent (pobj));
      (sciGetGraphicMode (pobj))->addplot =sciGetAddPlot (sciGetParent (pobj));
      (sciGetGraphicMode (pobj))->autoscaling =sciGetAutoScale (sciGetParent (pobj));
      (sciGetGraphicMode (pobj))->zooming =sciGetZooming (sciGetParent (pobj));
      (sciGetGraphicMode (pobj))->oldstyle =sciGetGraphicsStyle (sciGetParent (pobj)); 
      (sciGetGraphicMode (pobj))->xormode =sciGetXorMode (sciGetParent (pobj));
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("This object haven't any graphic mode\n");
      return -1;
      break;
    }
  return 0;
}


/**sciSetColormap
 * This function sets a colormap to the figure. It's the same for all sons. 
 * Setting the colormap a must be a m x 3 double RGB matrix:  
 * a[i] = RED, a[i+m] = GREEN, a[i+2*m] = BLUE 
 */
int
sciSetColormap (sciPointObj * pobj, double *rgbmat, integer m, integer n)
{
  double *pc;
  int k;
  C2F(dr)("xset","colormap",&m,&n,PI0,PI0,PI0,PI0,rgbmat,PD0,PD0,PD0,0L,0L);
  
  pc=pFIGURE_FEATURE( (sciPointObj *) pobj)->pcolormap;

  FREE(pFIGURE_FEATURE( (sciPointObj *) pobj)->pcolormap);
  if((pFIGURE_FEATURE(pobj)->pcolormap = (double *) MALLOC (m * n * sizeof (double))) == (double *) NULL)
    {
      sciprint ("Not enough memory available for colormap\n");
      pFIGURE_FEATURE( (sciPointObj *) pobj)->pcolormap=pc;
      return -1;
    }  
  
  for (k=0;k<m*n;k++) 
    pFIGURE_FEATURE( (sciPointObj *) pobj)->pcolormap[k] = rgbmat[k];
  sciSetBackground ((sciPointObj *) pobj, sciGetNumColors (pobj));
  sciSetForeground ((sciPointObj *) pobj, sciGetNumColors (pobj)+1); 
  return 0;

}

/**sciGetColormap
 * This function gets a colormap from the figure. It's the same for all sons
 * @memo Gets the colormap rgbmat must be a m x 3 double RGB matrix:  
 * a[i] = RED, a[i+m] = GREEN, a[i+2*m] = BLUE 
 */
int
sciGetColormap (sciPointObj * pobj, double *rgbmat)
{
  int i;
  int m = sciGetNumColors (pobj);	/* the number of the color*/
  if (sizeof (rgbmat) != m * 3)
    return -1;
  for (i = 0; i < m; i++)
    {
      rgbmat[i] = sciGetScilabXgc (pobj)->Red[i];
      rgbmat[i + m] = sciGetScilabXgc (pobj)->Green[i];
      rgbmat[i + 2 * m] = sciGetScilabXgc (pobj)->Blue[i];
    }
  return 0;
}

/**sciSetNumColors
 * This function sets the number of the color defined in colormap
 * param sciPointObj * pobj: the pointer to the entity
 * param int numcolor: the number of the indexed color
 * return 0 if ok, -1 if not
 */
int
sciSetNumColors (sciPointObj * pobj, int numcolors)
{
  sciGetScilabXgc (pobj)->Numcolors = numcolors;
  return 0;
}

/**sciGetNumColors
 * @memo This function gets the number of the color defined in colormap
 */
int
sciGetNumColors (sciPointObj * pobj)
{
  return sciGetScilabXgc (pobj)->Numcolors;
}

/**sciCloneColormap
 * This function clone a colormap from the figure. It must be FREE a lesat. 
 * It's the same for all sons
 * @memo Setting the colormap rgbmat must be a m x 3 double RGB matrix:  
 * a[i] = RED, a[i+m] = GREEN, a[i+2*m] = BLUE 
 */
double *
sciCloneColormap (sciPointObj * pobj)
{
  double *rgbmat;
  int m = sciGetNumColors (pobj);
  int i;

  if ((rgbmat = malloc (m * 3 * sizeof (double))) == NULL)
    return (double *) NULL;

  for (i = 0; i < m; i++)
    {
      rgbmat[i] = sciGetScilabXgc (pobj)->Red[i];
      rgbmat[i + m] = sciGetScilabXgc (pobj)->Green[i];
      rgbmat[i + 2 * m] = sciGetScilabXgc (pobj)->Blue[i];
    }
  return (double *) rgbmat;
}

/**sciSetBackground
 * @memo Sets the number of the Background
 */
int
sciSetBackground (sciPointObj * pobj, int colorindex)
{
  /* code taken in void C2F(setbackground)(num, v2, v3, v4) from JPC */
  if (sciGetScilabXgc (pobj)->CurColorStatus == 1)
    {
      /* COLORREF px;                           COLORREF ? "periWin-bgc"*/
      sciGetScilabXgc (pobj)->NumBackground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      C2F(dr)("xset","alufunction",&(sciGetScilabXgc (pobj)->CurDrawFunction),
	      PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicContext(pobj))->backgroundcolor = Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SUBWIN:
      (sciGetGraphicContext(pobj))->backgroundcolor = Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      sciSetBackground (sciGetParentFigure (pobj), colorindex);	/* ajout mat necessaire pour l'instant */
      break;
    case SCI_TEXT:
      (sciGetFontContext(pobj))->backgroundcolor = Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_TITLE:
      (sciGetFontContext (pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LEGEND:
      (sciGetFontContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_ARC:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_POLYLINE:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_PATCH:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_RECTANGLE:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SURFACE:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LIGHT:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_AXIS:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break; 
    case SCI_AXES:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENU:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENUCONTEXT:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_STATUSB:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;   
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    default:
      break;
    }
  return 0;
}

/**sciGetBackground
 * @memo Gets the color number of the Background. Be carreful the return of the subwindow 
 * is the feature of its parent figure.
 */
int
sciGetBackground (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /** the colormap is 32x3 by default
	  +1 is due to the 
	  sciGetBackground that returns the "stored color + 1" 
          that is equal to the color set by the user
      */
      return (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_TEXT:
      return (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_TITLE:
      return (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_LEGEND:
      return (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_PATCH:
      return (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_LIGHT:
      return (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_AXIS:
      return (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_PANNER:		/* pas de context graphics */
      break;
    case SCI_SBH:		/* pas de context graphics */
      break;
    case SCI_SBV:		/* pas de context graphics */
      break;
    case SCI_MENU:
      return (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_MENUCONTEXT:
      return (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_STATUSB:
      return (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;  
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_AGREG:
    default:
      break;
    }
  return 0;
}

/**sciSetForeground
 * @memo sets the number of the Background
 */
int
sciSetForeground (sciPointObj * pobj, int colorindex)
{
  /*pour le moment les couleur pris en compte sont les memes pour tout le monde */
  if (sciGetScilabXgc (pobj)->CurColorStatus == 1)
    {

      sciGetScilabXgc (pobj)->NumForeground =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      C2F(dr)("xset","alufunction",&(sciGetScilabXgc (pobj)->CurDrawFunction),
	      PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,11L);
    }
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SUBWIN:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      sciSetForeground (sciGetParentFigure (pobj), colorindex);
      break;
    case SCI_TEXT:
      (sciGetFontContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_TITLE:
      (sciGetFontContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LEGEND:
      (sciGetFontContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_ARC:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_POLYLINE:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_PATCH:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_RECTANGLE:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SURFACE:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LIGHT:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_AXIS:
      (sciGetGraphicContext(pobj))->foregroundcolor = Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_AXES:
      (sciGetGraphicContext(pobj))->foregroundcolor = Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENU:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENUCONTEXT:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_STATUSB:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SEGS:
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_AGREG:  
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    default:
      break;
    }
  return 0;
}


int
sciSetFillFlag (sciPointObj * pobj, int fillflag)
{
	
  switch (sciGetEntityType (pobj))
    { 
    case SCI_RECTANGLE:
      pRECTANGLE_FEATURE (pobj)->fillflag = fillflag;
      break;
    case SCI_ARC:
      pARC_FEATURE (pobj)->fill = fillflag;
      break;
    case SCI_FIGURE:           /* pas de remplissage */
    case SCI_SUBWIN:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS: 
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:  
    case SCI_PANNER:		
    case SCI_SBH:		
    case SCI_SBV:		
    default:
      break;
    }
  return 0;
}

int
sciGetFillFlag (sciPointObj * pobj)
{
	
  switch (sciGetEntityType (pobj))
    { 
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE (pobj)->fillflag;
      break;
    case SCI_ARC:
      return pARC_FEATURE (pobj)->fill;
      break;
    case SCI_FIGURE:           /* pas de remplissage */
    case SCI_SUBWIN:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:  
    case SCI_PANNER:		
    case SCI_SBH:		
    case SCI_SBV:		
    default:
      break;
    }
  return 0;
}




/**sciGetForeground
 * @memo Gets the color number of the Foreground
 */
int
sciGetForeground (sciPointObj * pobj)
{
  /** the foreground is a solid line style in b&w */
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /** the colormap is 32x3 by default */
      return (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_TEXT:
      return (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_TITLE:
      return (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_LEGEND:
      return (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_PATCH:
      return (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_LIGHT:
      return (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_AXIS:
      return (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break; 
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_MENU:
      return (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_MENUCONTEXT:
      return (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_STATUSB:
      return (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_SEGS:
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_AGREG:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    default:
      break;
    }
  return 0;
}

/**sciGetLineWidth
 * @memo Gets the line width
 */
int
sciGetLineWidth (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_TEXT:
      return 0;
      break;
    case SCI_TITLE:
      return 0;
      break;
    case SCI_LEGEND:
      return 0;
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_PATCH:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_LIGHT:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_AXIS:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_MENU:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_MENUCONTEXT:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_STATUSB:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_SEGS: 
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    default:
      sciprint ("This object has no Line Width\n");
      return -1;
      break;
    }
  return -1;
}

/**sciSetLineWidth
 * @memo Sets the line width
 */
int
sciSetLineWidth (sciPointObj * pobj, int linewidth)
{

  if (linewidth < 0)
    {
      sciprint ("the line width must be greater than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_SUBWIN:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  sciSetLineWidth (sciGetParentFigure (pobj), linewidth);
	  return 0;
	  break;
	case SCI_ARC:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
        case SCI_POLYLINE:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_PATCH:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_LIGHT:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_AXIS:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_AXES:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_MENU:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_MENUCONTEXT:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  return 0;
	  break;
	case SCI_STATUSB:
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  break;
        case SCI_SEGS: 
	  (sciGetGraphicContext(pobj))->linewidth = linewidth;
	  break;
	case SCI_AGREG:
	case SCI_FEC: 
	case SCI_GRAYPLOT:
	case SCI_PANNER:
	case SCI_SBH:
	case SCI_SBV:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	default:
	  /* pas de context graphics */
	  sciprint ("This object has no  line width \n");
	  return -1;
	  break;
	}
    }
  return -1;
}


/**sciGetLineStyle
 * @memo Gets the line style
 */
int
sciGetLineStyle (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_PATCH:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_LIGHT:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_AXIS:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_MENU:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_MENUCONTEXT:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_STATUSB:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_SEGS: 
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    default:
      sciprint ("This object has no Line Width\n");
      return -1;
      break;
    }
  return 0;
}

/**sciSetLineStyle
 * @memo Sets the line style
 */
int
sciSetLineStyle (sciPointObj * pobj, int linestyle)
{

  if (linestyle < 0)
    {
      sciprint ("the line width must be greater than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_SUBWIN:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  sciSetLineStyle (sciGetParentFigure (pobj), linestyle);
	  return 0;
	  break;
	case SCI_ARC:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
        case SCI_POLYLINE:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_PATCH:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_LIGHT:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_AXIS:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_AXES:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_MENU:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_MENUCONTEXT:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  return 0;
	  break;
	case SCI_STATUSB:
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  break;
	case SCI_SEGS: 
	  (sciGetGraphicContext(pobj))->linestyle = linestyle;
	  break;
	case SCI_FEC: 
	case SCI_GRAYPLOT:
	case SCI_AGREG:
	case SCI_PANNER:
	case SCI_SBH:
	case SCI_SBV:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	default:
	  /* pas de context graphics */
	  sciprint ("This object have no  line width \n");
	  return -1;
	  break;
	}
    }
  return 0;
}

/**sciGetIsMark
 * @memo Gets the line style
 */
BOOL
sciGetIsMark (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_PATCH:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_AXIS:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_FEC: 
      return (sciGetGraphicContext(pobj))->ismark;
      break; 
    case SCI_SEGS:
      return (sciGetGraphicContext(pobj))->ismark;
      break; 
    case SCI_GRAYPLOT:
    case SCI_LIGHT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_PANNER:	/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    default:
      sciprint ("This object has no ismark\n");
      return -1;
      break;
    }
  return 0;
}

/**sciSetIsMark
 * @memo Sets the line style
 */
int
sciSetIsMark (sciPointObj * pobj, BOOL ismark)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_SUBWIN:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      sciSetIsMark (sciGetParentFigure (pobj), ismark);
      return 0;
      break;
    case SCI_ARC:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_POLYLINE:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_PATCH:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_RECTANGLE:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_SURFACE:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_AXIS:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_AXES:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_FEC:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break; 
    case SCI_SEGS: 
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break; 
    case SCI_GRAYPLOT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_LIGHT:
    case SCI_AGREG:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    default:
      /* pas de context graphics */
      sciprint ("This object have no  ismark \n");
      return -1;
      break;
    }
  return 0;
}


/**sciGetMarkStyle
 * @memo Gets the mark style
 */
int
sciGetMarkStyle (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_SUBWIN:
      return sciGetMarkStyle (sciGetParent (pobj));
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_PATCH:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_AXIS:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_FEC:
      return (sciGetGraphicContext(pobj))->markstyle;
      break; 
    case SCI_SEGS:
      return (sciGetGraphicContext(pobj))->markstyle;
      break; 
    case SCI_GRAYPLOT:
    case SCI_LIGHT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    default:
      return -1;
      break;
    }
  return 0;
}

/**sciSetMarkStyle
 * @memo Sets the line style
 */
int
sciSetMarkStyle (sciPointObj * pobj, int markstyle)
{

  if (markstyle < 0)
    {
      sciprint ("the mark style must be greater or equal than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
	case SCI_SUBWIN:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  sciSetMarkStyle (sciGetParentFigure (pobj), markstyle);
	  return 0;
	  break;
	case SCI_ARC:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
        case SCI_POLYLINE:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
	case SCI_PATCH:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
	case SCI_AXIS:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
        case SCI_AXES:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
        case SCI_FEC: 
          (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break; 
	case SCI_SEGS: 
          (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break; 
	case SCI_GRAYPLOT:
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
 	case SCI_LIGHT:
	case SCI_AGREG:
	case SCI_PANNER:
	case SCI_SBH:
	case SCI_SBV:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	default:
	  /* pas de context graphics */
	  sciprint ("This object have no mark \n");
	  return -1;
	  break;
	}
    }
  return 0;
}


/**sciGetFillStyle
 * @memo Gets the fill style
 */
int
sciGetFillStyle (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_SUBWIN:
      return sciGetFillStyle (sciGetParent (pobj));
      break;
    case SCI_TEXT:
      return -1;
      break;
    case SCI_TITLE:
      return -1;
      break;
    case SCI_LEGEND:
      return -1;
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_PATCH:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_AXIS:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_LIGHT:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    default:
      sciprint ("This object has no Line Width\n");
      return -1;
      break;
    }
  return 0;
}

/**sciSetFillStyle
 * @memo Sets the fillstyle
 */
int
sciSetFillStyle (sciPointObj * pobj, int fillstyle)
{

  if (fillstyle < 0)
    {
      sciprint ("the fill width must be equal or greater than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
	case SCI_SUBWIN:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  sciSetFillStyle (sciGetParentFigure (pobj), fillstyle);
	  return 0;
	  break;
	case SCI_ARC:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
        case SCI_POLYLINE:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
	case SCI_PATCH:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
	case SCI_AXIS:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
        case SCI_AXES:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
	case SCI_SEGS: 
        case SCI_FEC: 
	case SCI_GRAYPLOT:
	case SCI_LIGHT:
	case SCI_PANNER:
	case SCI_SBH:
	case SCI_SBV:
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
	case SCI_AGREG:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	default:
	  sciprint ("This object have no  line style \n");
	  return -1;
	  break;
	}
    }
  return 0;
}

/**sciGetFillColor
 * @memo Gets the fill color
 */
int
sciGetFillColor (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicContext(pobj))->fillcolor;
      break;
    case SCI_SUBWIN:
      return sciGetFillColor (sciGetParent (pobj));
      break;
    case SCI_TEXT:
      return -1;
      break;
    case SCI_TITLE:
      return -1;
      break;
    case SCI_LEGEND:
      return -1;
      break;
    case SCI_ARC:
      return (sciGetGraphicContext(pobj))->fillcolor;
      break;
    case SCI_POLYLINE:
      return (sciGetGraphicContext(pobj))->fillcolor;
      break;
    case SCI_PATCH:
      return (sciGetGraphicContext(pobj))->fillcolor;
      break;
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->fillcolor;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->fillcolor;
      break;
    case SCI_AXIS:
      return (sciGetGraphicContext(pobj))->fillcolor;
      break;
    case SCI_AXES:
      return (sciGetGraphicContext(pobj))->fillcolor;
      break;
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_LIGHT:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    default:
      sciprint ("This object has no Line Width\n");
      return -1;
      break;
    }
  return 0;
}



/**sciSetFillColor
 * @memo Sets the fillcolor
 */
int
sciSetFillColor (sciPointObj * pobj, int fillcolor)
{

  if (fillcolor < 0)
    {
      sciprint ("the fill width must be equal or greater than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
	case SCI_SUBWIN:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  sciSetFillStyle (sciGetParentFigure (pobj), fillcolor);
	  return 0;
	  break;
	case SCI_ARC:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
        case SCI_POLYLINE:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
	case SCI_PATCH:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
	case SCI_AXIS:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
        case SCI_AXES:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
	case SCI_SEGS: 
        case SCI_FEC: 
	case SCI_GRAYPLOT:
	case SCI_LIGHT:
	case SCI_PANNER:
	case SCI_SBH:
	case SCI_SBV:
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
	case SCI_AGREG:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	default:
	  sciprint ("This object have no  line width \n");
	  return -1;
	  break;
	}
    }
  return 0;
}


/**sciGetRelationship
 * @memo Returns the structure of the relationship. Do not use this in the Consturctor Functions !
 */
sciRelationShip *
sciGetRelationship (sciPointObj * pobj)
{
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
    case SCI_PATCH:
      return  &(pPATCH_FEATURE (pobj)->relationship);
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
    case SCI_AXIS:
      return  &(pAXIS_FEATURE (pobj)->relationship);
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
    case SCI_PATCH:
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
    case SCI_AXIS:
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
  sciPointObj *tmp;
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_SUBWIN:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_TEXT:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_TITLE:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_LEGEND:
      tmp = (pLEGEND_FEATURE (pobj)->text.relationship).pparent;
      return (sciPointObj *) tmp;
      break;
    case SCI_ARC:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_SEGS:  
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break; 
    case SCI_FEC: 
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break; 
    case SCI_GRAYPLOT:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break; 
    case SCI_POLYLINE:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_PATCH:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_RECTANGLE:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_SURFACE:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_LIGHT:
      return (sciPointObj *)(sciGetRelationship (pobj))->pparent;
      break;
    case SCI_AXIS:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_AXES:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_PANNER:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_SBH:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_SBV:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_MENU:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_MENUCONTEXT:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_STATUSB:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_AGREG:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    default:
      break;
    }
  return (sciPointObj *) NULL;
}



/**sciGetParentFigure
 * @memo Returns the the Figure parent
 */
sciPointObj *
sciGetParentFigure (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_PATCH:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS: 
    case SCI_AXES:
    case SCI_SUBWIN:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
      return sciGetScilabXgc ((sciPointObj *) pobj)->mafigure;  
      break;                                                     
    default:  
      return (sciPointObj *) NULL;
      break;
    }
  return (sciPointObj *) NULL;
}
/** 19/09/2002 ***/
/**sciGetParentSubwin
 * @memo Returns the the Subwin parent
 */
sciPointObj *
sciGetParentSubwin (sciPointObj * pobj)
{
  sciPointObj *subwin;

  subwin = pobj;
  switch (sciGetEntityType (pobj))
    { 
    case SCI_FIGURE:
      return (sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure ());
      break;
    case SCI_SUBWIN:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_PATCH:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS: 
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
      while (sciGetEntityType(subwin) != SCI_SUBWIN)
	subwin=sciGetParent(subwin);      
      return (sciPointObj *) subwin;  
      break;                                                     
    default:  
      return (sciPointObj *) NULL;
      break;
    }
  return (sciPointObj *) NULL;
}

/**sciGetNumfigure
 * @memo Returns the the Number of parent figure
 */
int 
sciGetNumFigure (sciPointObj * pobj)
{
  sciPointObj *figure;

  figure = pobj;
  switch (sciGetEntityType (pobj))
    { 
    case SCI_FIGURE:
      return  sciGetNum(sciGetCurrentFigure()) ;
      break;
    case SCI_SUBWIN:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_PATCH:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS: 
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
      while (sciGetEntityType(figure) != SCI_FIGURE)
	figure=sciGetParent(figure);      
      return sciGetNum(figure); 
      break;                                                     
    default:  
      return -1;
      break;
    }
  return -1;
}

/**sciGetScilabXgc
 * @memo Returns the ScilabXgc of the root recursively...
 */
struct BCG *
sciGetScilabXgc (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return pFIGURE_FEATURE (pobj)->pScilabXgc;
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_PATCH:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:  
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_SUBWIN:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
      /* on recherche la root par recursivite 
	 puisque scilabxgc n'est connu que par le parent */
      return (struct BCG *) sciGetScilabXgc (sciGetParent (pobj));	
      break;
    default:
      return (struct BCG *) NULL;
      break;
    }
  return (struct BCG *) NULL;
}



/**sciGetDC
 * @memo Returns the HDC of the figure window (not the parent)
 */
#ifdef WIN32
HDC        /* BCG Type priWin !! decommente par SS ???? */ 
sciGetDC (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
    case SCI_SUBWIN:
      return GetDC (sciGetScilabXgc (pobj)->CWindow);
      break;
    default:
      return sciGetDC (sciGetParent (pobj));
      break;
    }
  return (HDC ) NULL;        /* Type HDC ! "periWin-bgc"*/
}
#endif

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
    case SCI_PATCH:
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
    case SCI_AXIS:
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
    case SCI_PATCH:
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
    case SCI_AXIS:
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
    case SCI_PATCH:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_ARC:
    case SCI_AGREG:
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
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_AGREG:
      /* recherche de l'objet a effacer*/
      OneSon = (sciGetRelationship (pparent)->psons);
      OneSonprev = OneSon;
      tmp = 0;
      while ( (OneSon != NULL) &&  (OneSon->pointobj != pthis) )
	{
	  OneSonprev = OneSon;/* on garde une trace du precedant */
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
    case SCI_PATCH:
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
    case SCI_AXIS:
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
    case SCI_PATCH:
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
    case SCI_AXIS:
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
    default:
      return (sciSons *) NULL;
      break;
    }
  return (sciSons *) NULL;
}



/************************************ END SONS **************************************/
/**sciSetIsClipping*/
void
sciSetIsClipping (sciPointObj * pobj, int value)
{  
  
  
  switch (sciGetEntityType (pobj))
    {
   
    case SCI_SUBWIN:
      pSUBWIN_FEATURE (pobj)->isclip = value;
    case SCI_ARC:
      pARC_FEATURE (pobj)->isclip = value;
      break;
    case SCI_POLYLINE:
      pPOLYLINE_FEATURE (pobj)->isclip = value;
      break;
    case SCI_PATCH:
      pPATCH_FEATURE (pobj)->isclip = value;
      break;
    case SCI_RECTANGLE:
      pRECTANGLE_FEATURE (pobj)->isclip = value;
      break;   
    case SCI_SEGS: 
      pSEGS_FEATURE (pobj)->isclip = value;
      break;       
    case SCI_TEXT: 
      pTEXT_FEATURE (pobj)->isclip = value;
      break;   
    case SCI_LIGHT: 
      pLIGHT_FEATURE (pobj)->isclip = value;
      break;   
    case SCI_AXES: 
      pAXES_FEATURE (pobj)->isclip = value;
      break; 
    case SCI_AXIS: 
    case SCI_SURFACE:  
    case SCI_LEGEND: 
    case SCI_TITLE:
    case SCI_FEC: 
    case SCI_GRAYPLOT:    
    case SCI_AGREG: 
    case SCI_FIGURE:
    case SCI_SBH:   
    case SCI_PANNER:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    default:
      break;
    }
}

/**sciGetIsClipping*/
int
sciGetIsClipping (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
   
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE (pobj)->isclip;
      break;
    case SCI_ARC:
      return pARC_FEATURE (pobj)->isclip;
      break;
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE (pobj)->isclip;
      break;
    case SCI_PATCH:
      return pPATCH_FEATURE (pobj)->isclip;
      break;
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE (pobj)->isclip;
      break;   
    case SCI_SEGS: 
      return pSEGS_FEATURE (pobj)->isclip;
      break;      
    case SCI_TEXT: 
      return pTEXT_FEATURE (pobj)->isclip;
      break;   
    case SCI_LIGHT: 
      return pLIGHT_FEATURE (pobj)->isclip;
      break;   
    case SCI_AXES: 
      return pAXES_FEATURE (pobj)->isclip;
      break; 
    case SCI_SURFACE:
    case SCI_LEGEND: 
    case SCI_AXIS: 
    case SCI_TITLE:    
    case SCI_AGREG: 
    case SCI_FIGURE: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_SBH:   
    case SCI_PANNER:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    default:
      return -2;
      break;
    } 
  return -2;
}




/**sciSetClipping
 * @memo Sets the clipping area
 * @author Djalel ABDEMOUCHE
 * 29/11/2002
 */
void
sciSetClipping (sciPointObj * pobj, double *pclip)
{
  int i;
  ++cmptclip;
  sciSetIsClipping (pobj, cmptclip);
  sciSetIsClipping (sciGetParentSubwin(pobj), cmptclip);
  ptabclip[cmptclip].index=cmptclip;
  for (i=0 ; i<4 ; i++)
    ptabclip[cmptclip].clip[i]= pclip[i];
  
}


/**sciGetClipping
 * @memo Gets the clipping area
 * 29/11/2002
 */
double *
sciGetClipping (sciPointObj * pobj)
{
  int index;

  index = sciGetIsClipping (pobj);
  return ptabclip[index].clip;

}

/**sciClip
 * @memo Set the clipping rectangle 
 * @author Djalel ABDEMOUCHE
 * 29/11/2002
 */
void
sciClip (int value)
{
  int x,y,w,h; 
  if (value == 0)
    frame_clip_on();
  else if (value > 0)
    { 
      x = XDouble2Pixel( ptabclip[value].clip[0]); 
      y = YDouble2Pixel( ptabclip[value].clip[1]);     
      w = WDouble2Pixel( ptabclip[value].clip[0],ptabclip[value].clip[2]); 
      h = HDouble2Pixel( ptabclip[value].clip[1],ptabclip[value].clip[3]);
      C2F(dr)("xset","clipping",&x, &y, &w, &h,PI0,PI0,PD0,PD0,PD0,PD0,4L,8L);
    }
}

/**sciClip
 * @memo Clear the clipping rectangle 
 * @author Djalel ABDEMOUCHE
 * 29/11/2002
 */
void
sciUnClip (int value)
{
  if (value > -1 )
    C2F(dr)("xset","clipoff",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,7L);
}


/**sciSetHighLight
 * @memo Tunrs ON or OFF the highlighting of the objects when there are selected
 */

void
sciSetHighLight (sciPointObj * pobj, BOOL value)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /* (sciGetGraphicMode (pobj))->highlight = value; unused */
      break;
    case SCI_SUBWIN:
      /* la valeur est herite du parent...pour le moment */

      /* (sciGetGraphicMode (pobj))->highlight = value;
      sciSetHighLight (sciGetParentFigure (pobj), value);unused */
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_SEGS: 
    case SCI_FEC: 
   case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
      sciprint ("We cannot highlight this object\n");
      break;
    default:
      sciprint ("We cannot highlight this object\n");
      break;
    }
}

/**sciGetHighLight
 * @memo Returns the highlight mode where the object are.
 */

BOOL
sciGetHighLight (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /*return (sciGetGraphicMode (pobj))->highlight; */
      break;
    case SCI_SUBWIN:
      /* the value is inhirated by the parent */
      /*return sciGetHighLight (sciGetParentFigure (pobj));*/
      break;
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
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_PATCH:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    default:
      sciprint ("This object cannot be highlighted\n");
      return FALSE;
      break;
    }
  return FALSE;
}


/**sciSetAddPlot
 * @memo Tunrs ON or OFF the possibility to draw multi plots in one graphic. 
 * If FLASE, plot is cleared before new drawing
 */
void
sciSetAddPlot (sciPointObj * pobj, BOOL value)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicMode (pobj))->addplot = value;
      break;
    case SCI_SUBWIN:
      (sciGetGraphicMode (pobj))->addplot = value;
      (sciGetGraphicMode (sciGetParentFigure(pobj)))->addplot = value;
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    default:
      sciprint ("This object cannot be addploted\n");
      break;
    }
}


/**sciGetAddPlot
 * @memo Returns the mode of the adding plot
 * @param sciPointObj * pobj: the pointer to the entity
 * @return TRUE if yes, FLASE if no 
 * @author Djalel ABDEMOUCHE
 * 25/10/2002
 */
BOOL
sciGetAddPlot (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicMode (pobj))->addplot;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicMode (pobj))->addplot; 
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    default:
      sciprint ("This object cannot be addploted\n");
      return FALSE;
      break;
    } 
  return FALSE;
}


/**sciSetAutoScale
 * @memo Sets the mode if the plot is in zoom fit mode, or fixed by the user
 */
void
sciSetAutoScale (sciPointObj * pobj, BOOL value)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      sciSetAutoScale(sciGetSelectedSubWin (pobj),value);
      break;
    case SCI_SUBWIN:
      (sciGetGraphicMode (pobj))->autoscaling = value;
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
      sciprint ("This object cannot be autoscaled 1 sciSetAutoScale\n");
      break;
      break;
    default:
      sciprint ("This object cannot be autoscaled 2 sciSetAutoScale\n");
      break;
    }
}


/**sciGetAutoScale
 * @memo Returns the mode of the autoscaling
 */
BOOL
sciGetAutoScale (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicMode (pobj))->autoscaling;
      break;
    case SCI_SUBWIN:
      return (sciGetGraphicMode (pobj))->autoscaling;
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
      sciprint ("This object cannot be autoscaled 1 sciGetAutoScale\n");
      return FALSE;
      break;
    default:
      sciprint ("This object cannot be autoscaled 2 sciGetAutoScale\n");
      return FALSE;
      break;
    }
  return FALSE;
}


/**sciSetZooming
 * @memo Sets the zooming state of the object. Is it or not zommed right now
 */
void
sciSetZooming (sciPointObj * pobj, BOOL value)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicMode (pobj))->zooming = value;
      break;
    case SCI_SUBWIN:
      (sciGetGraphicMode (pobj))->zooming = value;
      /* the value is inhirated by the parent */
      sciSetZooming (sciGetParentFigure (pobj), value);
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
      sciprint ("\r\nThis object cannot be zoomed\r\n");
      break;
    default:
      sciprint ("\r\nThis object cannot be zoomed\r\n");
      break;
    }
}


/**sciGetZooming
 * @memo Returns the zooming state of the object is it or not zoomed.
 */
BOOL
sciGetZooming (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicMode (pobj))->zooming;
      break;
    case SCI_SUBWIN:
      /* the value is inherited by the parent */
      return (sciGetGraphicMode (pobj))->zooming;
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:  
    case SCI_AXES:  
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
      sciprint ("\r\nThis object cannot be zoomed \r\n");
      return FALSE;
      break;
    default:
      sciprint ("\r\nThis object cannot be zoomed \r\n");
      return FALSE;
      break;
    }
  return FALSE;
}

/**sciSetDefaultValue
 * @memo Sets the default values of figure properties
 * (graphics context, graphics mode, and graphics font)
 * @author Djalel ABDEMOUCHE
 */
void
sciSetDefaultValues ()
{
  if ((sciInitGraphicContext (sciGetCurrentFigure()) == -1) ||
      (sciInitGraphicMode (sciGetCurrentFigure()) == -1))
    sciprint("\r\n default values cant not be loaded !");
  else
    C2F(dr)("xset","default",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,7L); 
}

/**sciSetGraphicsStyle
 * @memo Sets the graphics style
 */
void
sciSetGraphicsStyle (sciPointObj * pobj, BOOL value)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicMode (pobj))->oldstyle = value;
      break;
    case SCI_SUBWIN: 
      /* the value is inhirated by the parent */
      sciSetGraphicsStyle (sciGetParentFigure (pobj), value);
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
      sciprint ("\r\nNothing to do\n");
      break;
    default:
      sciprint ("\r\nNothing to do\n");
      break;
    }
}
/**sciGetGraphicsStyle
 * @memo Returns the graphics style
 */
BOOL
sciGetGraphicsStyle (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicMode (pobj))->oldstyle;
      break;
    case SCI_SUBWIN: 
      /* the value is inhirated by the parent */
      return sciGetGraphicsStyle (sciGetParentFigure (pobj));
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:  
    case SCI_AXES:  
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
      sciprint ("\r\nNothing to do\n");
      return FALSE;
      break;
    default:
      sciprint ("\r\nNothing to do\n");
      return FALSE;
      break;
    }
  return FALSE;
}
/**sciSetXorMode
 * @memo Sets the xor mode
 * @param sciPointObj * pobj: the pointer to the entity
 * @param int value: the value of the xor mode
 * @return 
 * @author Djalel ABDEMOUCHE
 * @see sciGetXorMode
 */
void
sciSetXorMode (sciPointObj * pobj, int value)
{ 
  C2F(dr)("xset","alufunction",&(value),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,11L);
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicMode (pobj))->xormode = value;
      break;
    case SCI_SUBWIN: 
      /* the value is inhirated by the parent */
      sciSetXorMode (sciGetParentFigure (pobj), value);
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
      sciprint ("\r\nNothing to do\n");
      break;
    default:
      sciprint ("\r\nNothing to do\n");
      break;
    }
}
/**sciGetXorMode
 * @memo Returns the drawing Xor mode
 * @param sciPointObj * pobj: the pointer to the entity
 * @return 0,1,... 15
 * @author Djalel ABDEMOUCHE
 * @see sciSetXorMode
 */
int
sciGetXorMode (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicMode (pobj))->xormode;
      break;
    case SCI_SUBWIN: 
      /* the value is inhirated by the parent */
      return sciGetXorMode (sciGetParentFigure (pobj));
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:  
    case SCI_AXES:  
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
      sciprint ("\r\nNothing to do\n");
      return FALSE;
      break;
    default:
      sciprint ("\r\nNothing to do\n");
      return FALSE;
      break;
    }
  return FALSE;
}




/**sciSetResize
 * @memo Tunrs ON or OFF the autoresizing mode (when the window is resized by user)
 * @param sciPointObj * pobj: the pointer to the entity
 * @param BOOL value: the value of the switch
 */
void
sciSetResize (sciPointObj * pobj, BOOL value)
{
  integer xtmp = 0;
  integer x[2];
  integer num1 = (value ? 1 : 0);

  /* this code will coms from
   *  C2F(setwresize)((i = value, &i), PI0,PI0,PI0);
   * je changerais ce morceau de code quand tout csera OK
   */
  if (sciGetScilabXgc (pobj)->CurResizeStatus != num1)
    {
      sciGetScilabXgc (pobj)->CurResizeStatus = num1;	/* a faire avant setwindowdim */
      
      C2F(dr)("xget","wpdim",&xtmp,x,&xtmp,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,5L); 
      C2F(dr)("xset","wpdim",&(x[0]),&(x[1]),PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,5L);
#ifdef WIN32
      /* Win function sciGetScilabXgc (pobj)->horzsi.nPos !!? BCG.horzsi.nPos*/
      /* SetViewportOrgEx (GetDC (sciGetScilabXgc (pobj)->CWindow),  
      			-sciGetScilabXgc (pobj)->horzsi.nPos,
      			-sciGetScilabXgc (pobj)->vertsi.nPos, NULL);*/
       /*if (sciGetPixmapStatus () == 0)                                          
	InvalidateRect (sciGetScilabXgc (pobj)->CWindow, NULL, TRUE); 
      return ScilabXgc->CurPixmapStatus;
      UpdateWindow (sciGetScilabXgc (pobj)->CWindow); */ /* dependent function MacWinOther.c"*/

#endif
    }
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicMode (pobj))->wresize = value;
      break;
    case SCI_SUBWIN:
      (sciGetGraphicMode (pobj))->wresize = value;
      /* the value is inhirated by the parent */
      sciSetResize (sciGetParentFigure (pobj), value);
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:  
    case SCI_AXES: 
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    default:
      sciprint ("This object cannot be resized\n");
      return;
      break;
    }
}


/**sciGetResize (replace sciGetwresize()in the next version)
 * @memo Returns if this object is in autoresizing mode (when the window is resized by user)
 * @param sciPointObj * pobj: the pointer to the entity
 * @return TRUE if yes, FLASE if no
 */
BOOL
sciGetResize (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return (sciGetGraphicMode (pobj))->wresize;
      break;
    case SCI_SUBWIN:
      /* the value is inhirated by the parent */
      return sciGetResize (sciGetParent (pobj));
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:  
    case SCI_AXES: 
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
      sciprint ("This object cannot be resized\n");
      return FALSE;
      break;
    default:
      sciprint ("This object cannot be resized\n");
      return FALSE;
      break;
    }
  return FALSE;
}


/**sciSetName 
 * @memo Sets the name of the Figure
 * @param sciPointObj * pobj: the pointer to the entity
 * @param char pvalue: a pointer to the string contening name
 * @param int length: the length of the string
 * @return 
 */
void
sciSetName (sciPointObj * pobj, char *pvalue, int length)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      strncpy (pFIGURE_FEATURE (pobj)->name, pvalue,
	       Min (sizeof ("ScilabGraphic") + 4, length));
      pFIGURE_FEATURE (pobj)->namelen =
	Min (sizeof ("ScilabGraphic") + 4, length); 
      C2F(dr)("xname",pvalue,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,(long) length);
      break;
    case SCI_SUBWIN:
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can be named\n");
      break;
    }
}


/**sciGetName
 * @memo Returns the name of the Figure or SubWindow
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the pointer to the char name
 */
char *
sciGetName (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return pFIGURE_FEATURE (pobj)->name;
      break;
    case SCI_SUBWIN:
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can be named\n");
      return NULL;
      break;
    }
}


/**sciGetNameLength
 * @memo Returns the length of the name of the Figure or SubWindow
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the length of the Name
 */
int
sciGetNameLength (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return pFIGURE_FEATURE (pobj)->namelen;
      break;
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE (pobj)->namelen;
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure or SubWindow can be named\n");
      return 0;
      break;
    }
}


/**sciSetNum
 * @memo Sets the number of the Figure.
 * @param sciPointObj * pobj: the pointer to the entity
 * @param int value: the value of the number of the windows
 */
void
sciSetNum (sciPointObj * pobj, int *value)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      pFIGURE_FEATURE (pobj)->number = *value;
      break;
    case SCI_SUBWIN:
      pSUBWIN_FEATURE (pobj)->number = *value;
      sciSetNum (sciGetParentFigure (pobj), value);
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure be numerated\n");
      break;
    }
}


/**sciGetNum
 * @memo Returns the number of the Figure.
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the number of the windows contening this entity
 */
int
sciGetNum (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return pFIGURE_FEATURE (pobj)->number;
      break;
    case SCI_SUBWIN:
      return sciGetNum (sciGetParent(pobj));
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can be numerated\n");
      return -1;
      break;
    }
}


/**sciSetDim
 * @memo Sets the dimension of the Figure or Subwin
 * @param sciPointObj * pobj: the pointer to the entity
 * @param int *pwidth: the width of the window dimension
 * @param int *pheight: the height of the window dimension
 * @return 
 */
void
sciSetDim (sciPointObj * pobj, int *pwidth, int *pheight)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /* selectionner le xgc correspondant puis */
      pFIGURE_FEATURE (pobj)->figuredimwidth = *pwidth;
      pFIGURE_FEATURE (pobj)->figuredimheight = *pheight;

      /* GPopupResize (sciGetScilabXgc (pobj), pwidth, pheight); commented out SS 03/01/03 */

      break;
    case SCI_SUBWIN:
      pSUBWIN_FEATURE (pobj)->windimwidth = *pwidth;
      pSUBWIN_FEATURE (pobj)->windimheight = *pheight;
      C2F(dr)("xset","wdim",pwidth, pheight,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,4L);
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure or Subwin can be sized\n");
      break;
    }
}


/**sciGetWidth
 * @memo Returns the width in pixel of the figure or subwin
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the width of the dimension of the window or figure 
 * (the visibility dimension) in pixel dimension
 */
double /* ????? */
sciGetWidth (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
    case SCI_SUBWIN:
      return sciGetPosWidth(pobj);
      break;
    default:
      sciprint ("Only Figure is physical dimensioned\n");
      return -1;
      break;
    }
}


/**sciGetHeight
 * @memo Returns the height in pixel of the figure or subwin
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the height of the dimension of the window or figure (the visibility dimension) in pixel dimension
 */
double /* ????? */
sciGetHeight (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
    case SCI_SUBWIN:
      return sciGetPosHeight(pobj);
      break;
    default:
      sciprint ("Only Figure is physical dimensioned\n");
      return -1;
      break;
    }
}


/**sciSetFigurePos
 * @memo Sets the position of the FIGURE (the window) in root.
 */
int
sciSetFigurePos (sciPointObj * pobj, int pposx, int pposy)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      C2F(dr)("xset","wpos",&pposx,&pposy,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,4L);
      pFIGURE_FEATURE (pobj)->inrootposx = pposx;
      pFIGURE_FEATURE (pobj)->inrootposy = pposy;
      return 0;
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can breturn position\n");
      return -1;
      break;
    }
  return -1;
}


/**sciGetFigurePosX
 * @memo Returns the horizontal position of  the FIGURE (the window) in root, in pixels
 * @param sciPointObj * pobj
 * @return the position in pixel dimension
 */
int
sciGetFigurePosX (sciPointObj * pobj)
{
  integer x[2],y=0;
  double d=0;
  
  switch (sciGetEntityType (pobj))
    { 
    case SCI_FIGURE:
      /* modifiee par D.ABDEMOUCHE **/ 
      C2F(dr)("xget","wpos",&y,x,&y,PI0,PI0,PI0,&d,PD0,PD0,PD0,4L,4L);
      sciSetFigurePos (pobj, x[0], x[1]);
      return pFIGURE_FEATURE (pobj)->inrootposx;
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can return position\n");
      return -1;
      break;
    }
}


/**sciGetFigurePosY
 * @memo Returns the vertical position of the window in pixels
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the position in pixel dimension
 */
int
sciGetFigurePosY (sciPointObj * pobj)
{  
  integer x[2],y=0;
  double d=0;
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /* modifiee par D.ABDEMOUCHE **/ 
      C2F(dr)("xget","wpos",&y,x,&y,PI0,PI0,PI0,&d,PD0,PD0,PD0,4L,4L);
      sciSetFigurePos (pobj, x[0], x[1]);
      return pFIGURE_FEATURE (pobj)->inrootposy;
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can return position\n");
      return -1;
      break;
    }
}


/**sciSetFigureIconified
 * @memo Minimizes or Restores the window if TRUE or FALSE (useful to get the Window on front)
 * @param sciPointObj * pobj: the pointer to the entity
 * @param BOOL value: TRUE the window will be iconify, FALSE the window will be raise
 * @return 
 */
void
sciSetFigureIconify (sciPointObj * pobj, BOOL value)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      pFIGURE_FEATURE (pobj)->isiconified = value;
      /* hWndParent de Type HWND (BCG) "WIN" */
      /*     if (value)
	ShowWindow ((sciGetScilabXgc (pobj))->hWndParent, SW_MINIMIZE);
      else
	{
	  ShowWindow ((sciGetScilabXgc (pobj))->hWndParent, SW_SHOWNORMAL); 
	  BringWindowToTop ((sciGetScilabXgc (pobj))->hWndParent);
	  } */
      break;
    default:
      sciSetFigureIconify(sciGetCurrentFigure(), value);
      break;
    }
}


/**sciGetIsFigureIconified
 * @memo Determines whether the specified Figure is minimized (iconic). 
 * @param sciPointObj * pobj: the pointer to the entity
 * @return TRUE if yes, FALSE if no
 */
BOOL
sciGetIsFigureIconified (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /* hWndParent de Type HWND (BCG) "WIN" */
      /*return (pFIGURE_FEATURE (pobj)->isiconified =
	IsIconic ((sciGetScilabXgc (pobj))->hWndParent)); */
      return FALSE;
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can return iconic status\n");
      return FALSE;
      break;
    }
}


/**sciSetSubwindowPos
 * Sets subwindow position 
 * @param sciPointObj * pobj: the pointer to the entity
 * @param int pposx: the pointer to the x position
 * @param int pposy: the pointer to the y position
 * @return  0 if OK , else -1
 * @version 0.1.
 **/
int
sciSetSubWindowPos (sciPointObj * pobj, int *pposx, int *pposy)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_SUBWIN:
      /* selectionner le xgc correspondant puis */
      pSUBWIN_FEATURE (pobj)->infigureposx = *pposx;
      pSUBWIN_FEATURE (pobj)->infigureposy = *pposy;
      return 0;
      /* C2F(setwindowpos)( width, height, PI0, PI0); */
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only subwin can be return position\n");
      return -1;
      break;
    }
  return -1;
}


/**sciGetSubwindowPosX
 * Gets scrollbar position; use unreserved function sciGetPosX for all. 
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int vertical position in pixel
 **/
int
sciGetSubwindowPosX (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_SUBWIN:
      /* selectionner le xgc correspondant puis */
      return pSUBWIN_FEATURE (pobj)->infigureposx;
      /* C2F(getwindowpos)( width, height, PI0, PI0); */
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can return position\n");
      return -1;
      break;
    }
}


/**sciGetSubwindowPosY
 * @memo Gets subwindow position; use unreserved function sciGetPosX for all.
 * @param sciPointObj * pobj: the pointer to the entity in pixel
 * @return  int vertical position 
 **/
int
sciGetSubwindowPosY (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_SUBWIN:
      /* selectionner le xgc correspondant puis */
      return pSUBWIN_FEATURE (pobj)->infigureposy;
      /* C2F(getwindowpos)( width, height, PI0, PI0); */
      break;
    case SCI_AGREG:
    default:
      sciprint ("Only Figure can return position\n");
      return -1;
      break;
    }
}



/**sciGetIsSelected
 * @memo Determines wether this object is selected or not.
 * @param sciPointObj * pobj: the pointer to the entity
 * @return TRUE if yes, FALSE if not
 */
BOOL
sciGetIsSelected (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return pFIGURE_FEATURE (pobj)->isselected;
      break;
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE (pobj)->isselected;
      break;
    case SCI_TITLE:
      return pTITLE_FEATURE (pobj)->isselected;
      break;
    case SCI_LEGEND:
      return pLEGEND_FEATURE (pobj)->isselected;
      break;
    case SCI_ARC:
      return pARC_FEATURE (pobj)->isselected;
      break;
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE (pobj)->isselected;
      break;
    case SCI_PATCH:
      return pPATCH_FEATURE (pobj)->isselected;
      break;
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE (pobj)->isselected;
      break;
    case SCI_SURFACE:
      return pSURFACE_FEATURE (pobj)->isselected;
      break;    
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_TEXT:
    case SCI_LIGHT:
    case SCI_AXIS:  
    case SCI_AXES: 
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      return FALSE;
      break;
    }
}

/**sciSetVisibility*/
void
sciSetVisibility (sciPointObj * pobj, BOOL value)
{  
  sciSons *psonstmp;

  psonstmp = sciGetSons ((sciPointObj *) pobj);
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      pFIGURE_FEATURE (pobj)->visible = value;
      while ((psonstmp != (sciSons *) NULL) && (psonstmp->pointobj != (sciPointObj *)NULL))
	{
	  sciSetVisibility ((sciPointObj *)psonstmp->pointobj,value); 
	  psonstmp = psonstmp->pnext;
	}
      break;
    case SCI_SUBWIN:
      pSUBWIN_FEATURE (pobj)->visible = value;
      while ((psonstmp != (sciSons *) NULL) && (psonstmp->pointobj != (sciPointObj *)NULL))
	{
	  sciSetVisibility ((sciPointObj *)psonstmp->pointobj,value); 
	  psonstmp = psonstmp->pnext;
	}
      break;
    case SCI_TITLE:
      pTITLE_FEATURE (pobj)->visible = value;
      break;
    case SCI_LEGEND:
      pLEGEND_FEATURE (pobj)->visible = value;
      break;
    case SCI_ARC:
      pARC_FEATURE (pobj)->visible = value;
      break;
    case SCI_POLYLINE:
      pPOLYLINE_FEATURE (pobj)->visible = value;
      break;
    case SCI_PATCH:
      pPATCH_FEATURE (pobj)->visible = value;
      break;
    case SCI_RECTANGLE:
      pRECTANGLE_FEATURE (pobj)->visible = value;
      break;
    case SCI_SURFACE:
      pSURFACE_FEATURE (pobj)->visible = value;
      break;    
    case SCI_SEGS: 
      pSEGS_FEATURE (pobj)->visible = value;
      break;    
    case SCI_FEC: 
      pFEC_FEATURE (pobj)->visible = value;
      break;    
    case SCI_GRAYPLOT: 
      pGRAYPLOT_FEATURE (pobj)->visible = value;
      break;    
    case SCI_TEXT: 
      pTEXT_FEATURE (pobj)->visible = value;
      break;   
    case SCI_LIGHT: 
      pLIGHT_FEATURE (pobj)->visible = value;
      break;   
    case SCI_AXIS: 
      pAXIS_FEATURE (pobj)->visible = value;
      break;     
    case SCI_AXES: 
      pAXES_FEATURE (pobj)->visible = value;
      break;    
    case SCI_AGREG: 
      pAGREG_FEATURE (pobj)->visible = value; 
      while ((psonstmp != (sciSons *) NULL) && (psonstmp->pointobj != (sciPointObj *)NULL))
	{
	  sciSetVisibility ((sciPointObj *)psonstmp->pointobj,value); 
	  psonstmp = psonstmp->pnext;
	}  
      break;
    case SCI_SBH:   
    case SCI_PANNER:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    default:
      break;
    }
}

/**sciGetVisibility*/
BOOL
sciGetVisibility (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return pFIGURE_FEATURE (pobj)->visible;
      break;
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE (pobj)->visible;
      break;
    case SCI_TITLE:
      return pTITLE_FEATURE (pobj)->visible;
      break;
    case SCI_LEGEND:
      return pLEGEND_FEATURE (pobj)->visible;
      break;
    case SCI_ARC:
      return pARC_FEATURE (pobj)->visible;
      break;
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE (pobj)->visible;
      break;
    case SCI_PATCH:
      return pPATCH_FEATURE (pobj)->visible;
      break;
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE (pobj)->visible;
      break;
    case SCI_SURFACE:
      return pSURFACE_FEATURE (pobj)->visible;
      break;    
    case SCI_SEGS: 
      return pSEGS_FEATURE (pobj)->visible;
      break;    
    case SCI_FEC: 
      return pFEC_FEATURE (pobj)->visible;
      break;    
    case SCI_GRAYPLOT: 
      return pGRAYPLOT_FEATURE (pobj)->visible;
      break;    
    case SCI_TEXT: 
      return pTEXT_FEATURE (pobj)->visible;
      break;   
    case SCI_LIGHT: 
      return pLIGHT_FEATURE (pobj)->visible;
      break;   
    case SCI_AXIS: 
      return pAXIS_FEATURE (pobj)->visible;
      break;     
    case SCI_AXES: 
      return pAXES_FEATURE (pobj)->visible;
      break;    
    case SCI_AGREG: 
      return pAGREG_FEATURE (pobj)->visible;
      break;
    case SCI_SBH:   
    case SCI_PANNER:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    default:
      return TRUE;
      break;
    }
}



/**sciGetSelectedSon
 * @memo Determines wether this object is selected or not. 
 * WARNING TO BE DEFINED. No sciSetSelected is coded
 * @param sciPointObj * pparent: the pointer to the entity
 * @return the pointer sciPointObj *poinson of the current selected son
 */
sciPointObj *
sciGetSelectedSon (sciPointObj * pparent)
{
  sciSons *psonstmp;

  psonstmp = sciGetSons (pparent);
  /* init */
  if (psonstmp != (sciSons *) NULL)
    {
      /* on peut commencer sur le next */
      /* tant que le fils  */
      while ((psonstmp->pnext != (sciSons *) NULL)
	     && (sciGetIsSelected (psonstmp->pointobj) != TRUE))
	psonstmp = psonstmp->pnext;
      if (sciGetIsSelected (psonstmp->pointobj) == FALSE)
	return (sciPointObj *) NULL;
      else
	return (sciPointObj *) psonstmp->pointobj;
    }
  else
    return (sciPointObj *) NULL;

}



/**sciGetSelectedSubWin
 * @memo Determines wich SubWin is selected or not. WARNING TO BE DEFINED.
 * @param sciPointObj * pparent: the pointer to the entity
 * @return the pointer sciPointObj *poinson of the current selected son
 */
sciPointObj *
sciGetSelectedSubWin (sciPointObj * pparent)
{
  sciSons *psonstmp;
  if (sciGetEntityType (pparent) != SCI_FIGURE)
    {	  
      sciprint("This Handle is not a Figure\n");
      return (sciPointObj *)NULL;
    }
  psonstmp = sciGetSons (pparent);
  /* init */
  if (psonstmp != (sciSons *) NULL)
    {
      /* on peut commencer sur le next */
      /* tant que le fils  */
      while ((psonstmp->pnext != (sciSons *) NULL)
	     && ((sciGetIsSelected (psonstmp->pointobj) != TRUE) 
		 || (sciGetEntityType (psonstmp->pointobj) != SCI_SUBWIN)))
	psonstmp = psonstmp->pnext;
      if (sciGetIsSelected (psonstmp->pointobj) == FALSE)
	return (sciPointObj *) NULL;
      else 
	if (sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
	  return (sciPointObj *) psonstmp->pointobj;
	else return (sciPointObj *) NULL;
    }
  else
    return (sciPointObj *) NULL;
}


/**sciSetSelectedSubWin
 * @memo Determines wich SubWin is selected or not. WARNING TO BE DEFINED.
 * @param sciPointObj * psubwinobj: the pointer to the entity sub window
 * @return 1 if OK or -1 if NOT OK
 */
int
sciSetSelectedSubWin (sciPointObj * psubwinobj)
{
  sciPointObj *pselectedsuwin;

  /* on verifie que l'entite passee en argument est bien une sous fenetre */
  if (sciGetEntityType (psubwinobj) != SCI_SUBWIN)
    {
      sciprint("This Handle is not a SubWindow\n");
      return -1;
    }
  /* on verifie que la sous fenetre donnee n'est pas deja selectionnee */
  if (sciGetIsSelected (psubwinobj))
    return 1;

  /* sinon on recherche la sous fenetre deja selectionnee */
  /* dans l'ensemble des sous fenetres du parent SCI_FIGURE */
  if ((pselectedsuwin = (sciPointObj *)sciGetSelectedSubWin(sciGetParent(psubwinobj))) != (sciPointObj *)NULL)
    /* puis on la deselectionne */
    pSUBWIN_FEATURE (pselectedsuwin)->isselected = FALSE;

  /* puis on selectionne la sous fenetre passee en argument */
  pSUBWIN_FEATURE (psubwinobj)->isselected = TRUE;

  set_scale ("tttfff", pSUBWIN_FEATURE (psubwinobj)->WRect,
	     pSUBWIN_FEATURE (psubwinobj)->FRect, NULL,
	     pSUBWIN_FEATURE(psubwinobj)->logflags, NULL); 
  return 1;
}

/**sciIsExistingSubWin
 * @memo Determines if this SubWindow is an existing one in the current SCI_FIGURE 
 * in association with the wrect and frect....
 * @param WRect: Window rectangle dimension in double, 
 * @param FRect: window user's scale dimension in double, 
 * @param logscale: flag for logarithmic window
 * @return the pointer to the existing SubWindow, or Null if no one is existing
 */
sciPointObj *
sciIsExistingSubWin (WRect)
     double WRect[4];
{
  sciPointObj *pparentfigure;
  sciSons *psonstmp;

  double WRectTmp[4];
  int stop = 0;

  pparentfigure = (sciPointObj *)sciGetCurrentFigure();
  if (sciGetEntityType (pparentfigure) != SCI_FIGURE)
    return (sciPointObj *)NULL;

  /**  15/03/2002 **/
  if ((WRect[0] == 0.)&&(WRect[1] == 0.)&&(WRect[2] == 1.)&&(WRect[3] == 1.))
    return (sciPointObj *) sciGetLastSons (pparentfigure)->pointobj;

  psonstmp = sciGetSons (pparentfigure);
  /* init */
  if (psonstmp != (sciSons *) NULL)
    {
      /* on peut commencer sur le next */
      /* tant que le fils  */
      /* j'utilise l'ordre d'evaluation normalise C pour */
      /* verifier d'abord qu'il s'agit d'une sous fenetre */
      /* puis si les WRect et FRect sont bons */
      stop = 0;
      while ((psonstmp->pnext != (sciSons *) NULL) && (stop == 0))
	{
	  WRectTmp[0] = pSUBWIN_FEATURE (psonstmp->pointobj)->WRect[0];
	  WRectTmp[1] = pSUBWIN_FEATURE (psonstmp->pointobj)->WRect[1];
	  WRectTmp[2] = pSUBWIN_FEATURE (psonstmp->pointobj)->WRect[2];
	  WRectTmp[3] = pSUBWIN_FEATURE (psonstmp->pointobj)->WRect[3];

	  if ((sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
	      && (Abs(WRectTmp[0] - WRect[0]) < 1e-8)
	      && (Abs(WRectTmp[1] - WRect[1]) < 1e-8)
	      && (Abs(WRectTmp[2] - WRect[2]) < 1e-8)
	      && (Abs(WRectTmp[3] - WRect[3]) < 1e-8))
	    {
	      stop = 1;
	    }
	  else 
	    psonstmp = psonstmp->pnext;
	}
		
      if ((sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
	  && (Abs(WRectTmp[0] - WRect[0]) < 1e-8)
	  && (Abs(WRectTmp[1] - WRect[1]) < 1e-8)
	  && (Abs(WRectTmp[2] - WRect[2]) < 1e-8)
	  && (Abs(WRectTmp[3] - WRect[3]) < 1e-8))

	return (sciPointObj *) psonstmp->pointobj;
      else return (sciPointObj *) NULL;
    }

  return (sciPointObj *)NULL;
}



/**sciSetOriginalSubWin
 * @memo DO NOT USE !!!
 * @param sciPointObj * pfigure, the parent figure, 
 * @param sciPointObj * psubwin: the pointer to the entity subwindow
 * @return 0 if OK -1 if not
 */
int
sciSetOriginalSubWin (sciPointObj * pfigure, sciPointObj * psubwin)
{     
  pFIGURE_FEATURE(pfigure)->originalsubwin0011 = psubwin; 
  return 0;   
}



/**sciGetOriginalSubWin
 * @memo PRIVATE.
 * @param sciPointObj * pfigure: the pointer to the parent figure
 * @return the pointer of the original subwindow
 */
sciPointObj *
sciGetOriginalSubWin (sciPointObj * pfigure)
{
  return (sciPointObj *)pFIGURE_FEATURE(pfigure)->originalsubwin0011; /* ????*/
}




/****************************************** TEXT ******************************
 **sciGetFontDeciWidth
 * @memo Gets the Font Size
 */
int
sciGetFontDeciWidth (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      return -1;
      break;
    case SCI_SUBWIN:
      return -1;
      break;
    case SCI_TEXT:
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_TITLE:
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_LEGEND:
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_ARC:
      return -1;
      break;
    case SCI_POLYLINE:
      return -1;
      break;
    case SCI_PATCH:
      return -1;
      break;
    case SCI_RECTANGLE:
      return -1;
      break;
    case SCI_SURFACE:
      return -1;
      break;
    case SCI_LIGHT:
      return -1;
      break;
    case SCI_AXIS:  
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_AXES:  
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_MENU:
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_MENUCONTEXT:
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_STATUSB:
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    default:
      sciprint ("This object has no fontdeciwidth\n");
      return -1;
      break;
    }
  return 0;
}



/**sciSetFontDeciWidth
 * @memo Sets the font size
 */
int
sciSetFontDeciWidth (sciPointObj * pobj, int fontdeciwidth)
{

  if (fontdeciwidth < 0)
    {
      sciprint ("The font size must be greater than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	  /* pas de context graphics */
	  sciprint ("This object have no  Font width \n");
	  return -1;
	  break;
	case SCI_SUBWIN:
	  /* pas de context graphics */
	  sciprint ("This object have no  Font width \n");
	  return -1;
	  break;
	case SCI_TEXT:
	  (sciGetFontContext(pobj))->fontdeciwidth = fontdeciwidth;
	  return 0;
	  break;
	case SCI_TITLE:
	  (sciGetFontContext(pobj))->fontdeciwidth =
	    fontdeciwidth;
	  return 0;
	  break;
	case SCI_LEGEND:
	  (sciGetFontContext(pobj))->fontdeciwidth =
	    fontdeciwidth;
	  return 0;
	  break;
	case SCI_ARC:
	  /* pas de context graphics */
	  sciprint ("This object have no  Font width \n");
	  return -1;
	  break;
	case SCI_POLYLINE:
	  /* pas de context graphics */
	  sciprint ("This object have no  Font width \n");
	  return -1;
	  break;
	case SCI_PATCH:
	  /* pas de context graphics */
	  sciprint ("This object have no  Font width \n");
	  return -1;
	  break;
	case SCI_RECTANGLE:
	  /* pas de context graphics */
	  sciprint ("This object have no  Font width \n");
	  return -1;
	  break;
	case SCI_SURFACE:
	  /* pas de context graphics */
	  sciprint ("This object have no  font width \n");
	  return -1;
	  break;
	case SCI_LIGHT:
	  /* pas de context graphics */
	  sciprint ("This object have no  font width \n");
	  return -1;
	  break;
	case SCI_AXIS:
	  (sciGetFontContext(pobj))->fontdeciwidth =
	    fontdeciwidth;
	  return 0;
	  break;
        case SCI_AXES:
	  (sciGetFontContext(pobj))->fontdeciwidth =
	    fontdeciwidth;
	  return 0;
	  break;
	case SCI_PANNER:
	case SCI_SBH:
	case SCI_SBV:
	  /* pas de context graphics */
	  sciprint ("This object have no  font width \n");
	  return -1;
	  break;
	case SCI_MENU:
	  (sciGetFontContext(pobj))->fontdeciwidth =
	    fontdeciwidth;
	  return 0;
	  break;
	case SCI_MENUCONTEXT:
	  (sciGetFontContext(pobj))->fontdeciwidth =
	    fontdeciwidth;
	  return 0;
	  break;
	case SCI_STATUSB:
	  (sciGetFontContext(pobj))->fontdeciwidth =
	    fontdeciwidth;
	  break;
        case SCI_SEGS: 
	case SCI_FEC: 
	case SCI_GRAYPLOT:
        case SCI_AGREG:
	default:
	  break;
	}
    }
  return 0;
}




/*sciGetFontOrientation
 * gets the Font Orientation
 */
int
sciGetFontOrientation (sciPointObj * pobj)
{
  int tmp;
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      tmp = (sciGetFontContext(pobj))->textorientation;
      return tmp;
      break;
    case SCI_TITLE:
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_LEGEND:
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_AXIS:
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_AXES:
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_MENU:
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_MENUCONTEXT:
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_STATUSB:
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_AGREG:
    default:
      sciprint ("This object has no textorientation\n");
      return 0;
      break;
    }
  return 0;
}



/**sciSetFontOrientation
 * @memo Sets the font Orientation
 */
int
sciSetFontOrientation (sciPointObj * pobj, int textorientation)
{
  if (textorientation < 0)
    {
      sciprint ("The font size must be greater than 0\n");
      return -1;
    }
  else
    {
      switch (sciGetEntityType (pobj))
	{
	case SCI_TEXT:
	  (sciGetFontContext(pobj))->textorientation = textorientation;
	  return 0;
	  break;
	case SCI_TITLE:
	  (sciGetFontContext(pobj))->textorientation =	    textorientation;
	  return 0;
	  break;
	case SCI_LEGEND:
	  (sciGetFontContext(pobj))->textorientation =	    textorientation;
	  return 0;
	  break;
	case SCI_AXIS:
	  (sciGetFontContext(pobj))->textorientation =	    textorientation;
	  return 0;
	  break;
	case SCI_AXES:
	  (sciGetFontContext(pobj))->textorientation =	    textorientation;
	  return 0;
	  break;
	case SCI_MENU:
	  (sciGetFontContext(pobj))->textorientation =	    textorientation;
	  return 0;
	  break;
	case SCI_MENUCONTEXT:
	  (sciGetFontContext(pobj))->textorientation =	    textorientation;
	  return 0;
	  break;
	case SCI_STATUSB:
	  (sciGetFontContext(pobj))->textorientation =	    textorientation;
	  break;
	case SCI_ARC:
	case SCI_SEGS: 
	case SCI_FEC: 
	case SCI_GRAYPLOT: 
	case SCI_POLYLINE:
	case SCI_PATCH:
	case SCI_RECTANGLE:
	case SCI_SURFACE:
	case SCI_LIGHT:
	case SCI_FIGURE:
	case SCI_SUBWIN:
	case SCI_PANNER:
	case SCI_SBH:
	case SCI_SBV:
	case SCI_AGREG:
	default:
	  /* pas de context graphics */
	  sciprint ("This object have no  font width \n");
	  return -1;
	  break;

	}
    }
  return 0;
}



/**sciSetText
 * @memo Sets the Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @param  char *text : the text which has to be put
 * @param  int n
 * @return  0 if OK, -1 if not
 */
int
sciSetText (sciPointObj * pobj, char text[], int n)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      FREE(pTEXT_FEATURE (pobj)->ptextstring);
      if ((pTEXT_FEATURE (pobj)->ptextstring = calloc (n+1, sizeof (char))) == NULL)
	return -1;
      strncpy (pTEXT_FEATURE (pobj)->ptextstring, text, n);
      pTEXT_FEATURE (pobj)->textlen = n;
      break;
    case SCI_TITLE:
      FREE(pTITLE_FEATURE (pobj)->text.ptextstring);
      if ((pTITLE_FEATURE (pobj)->text.ptextstring = calloc (n+1, sizeof (char))) == NULL)
	return -1;
      strncpy (pTITLE_FEATURE (pobj)->text.ptextstring, text, n);
      pTITLE_FEATURE (pobj)->text.textlen = n;
      break;
    case SCI_LEGEND:
      FREE(pLEGEND_FEATURE (pobj)->text.ptextstring);
      if ((pLEGEND_FEATURE (pobj)->text.ptextstring = calloc (n+1, sizeof (char))) == NULL)
	return -1;
      strncpy (pLEGEND_FEATURE (pobj)->text.ptextstring, text, n);
      pLEGEND_FEATURE (pobj)->text.textlen = n;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC:
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      return -1;
      break;
    }
  return 0;
}



/**sciGetText
 * @memo Gets the Text in TEXT, TITLE or LEGEND. Be Carreful, 
 * @memo this return a unterminated string (without \0)
 * @memo To obtain the length of the text, use sciGetTextLength@param  sciPointObj *pobj
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  point to char[] if OK or NULL if not 
 */
char *
sciGetText (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return pTEXT_FEATURE (pobj)->ptextstring;
      break;
    case SCI_TITLE:
      return pTITLE_FEATURE (pobj)->text.ptextstring;
      break;
    case SCI_LEGEND:
      return pLEGEND_FEATURE (pobj)->text.ptextstring;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      return (char *) NULL;
      break;
    }
}



/**sciGetTextLength
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if not OK, over 0 if ok
 */
unsigned int
sciGetTextLength (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return pTEXT_FEATURE (pobj)->textlen;
      break;
    case SCI_TITLE:
      return pTITLE_FEATURE (pobj)->text.textlen;
      break;
    case SCI_LEGEND:
      return pLEGEND_FEATURE (pobj)->text.textlen;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
  return -1;
}


/**sciSetTextPosX
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if not OK, over 0 if ok
 */
int
sciSetTextPosX (sciPointObj * pobj, double x)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      pTEXT_FEATURE (pobj)->x = x;
      return 0;
      break;
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
  return -1;
}


/**sciGetTextPosX
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if not OK, over 0 if ok
 */
double
sciGetTextPosX (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return pTEXT_FEATURE (pobj)->x;
      break;
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
  return -1;
}



/**sciGetTextPosWidth
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if not OK, over 0 if ok
 */
double
sciGetTextPosWidth (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return 0;
      break;
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
  return -1;
}



/**sciGetTextPosHeight
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if not OK, over 0 if ok
 */
double
sciGetTextPosHeight (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return 0;
      break;
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
  return -1;
}




/**sciSetTextPosY
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if not OK, over 0 if ok
 */
int
sciSetTextPosY (sciPointObj * pobj, double y)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      pTEXT_FEATURE (pobj)->y = y;
      return 0;
      break;
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
  return -1;
}



/**sciGetTextPosY
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if not OK, over 0 if ok
 */
double
sciGetTextPosY (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return pTEXT_FEATURE (pobj)->y;
      break;
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE: 
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
  return -1;
}



/**sciGetFontBackground
 * @memo Gets the background color
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int color if OK, -1 if not 
 */
int
sciGetFontBackground (sciPointObj * pobj)
{


  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_STATUSB:
      return (sciGetFontContext(pobj))->backgroundcolor;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_AGREG:
    default:
      return -1;
      break;
    }
}





/**sciSetFontBackground
 * @memo Sets the background color
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, -1 if not
 */
int
sciSetFontBackground (sciPointObj * pobj, int colorindex)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));;
      break;
    case SCI_TITLE:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LEGEND:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_AXIS:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_AXES:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENU:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENUCONTEXT:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_STATUSB:
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_AGREG:
    default:
      return -1;
      break;
    }
  return 0;
}



/**sciGetFontForeground
 * @memo Gets the foreground color
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int foregroundcolor if OK, -1 if not
 */
int
sciGetFontForeground (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_AXIS:  
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
      return (sciGetFontContext(pobj))->foregroundcolor+ 1 ;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_AGREG:
    default:
      return -1;
      break;
    }
}



/**sciSetFontForeground
 * @memo Sets the foreground color
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, if not
 */
int
sciSetFontForeground (sciPointObj * pobj, int colorindex)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_TITLE:
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LEGEND:
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_AXIS:
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break; 
    case SCI_AXES:
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENU:
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_MENUCONTEXT:
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_STATUSB:
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_AGREG:
    default:
      return -1;
      break;
    }
  return 0;
}



/**sciGetFontStyle
 * @memo Gets the font style (a logical mask with 
 * @memo SCI_ATTR_BOLD|SCI_ATTR_ITALIC|SCI_ATTR_UNDERLINE|SCI_ATTR_STRIKEOUT)
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 OK, -1 if not
 */
int
sciGetFontStyle (sciPointObj * pobj)
{
  int iAttributes = SCI_DONT_CARE;

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      iAttributes =
	(sciGetFontContext(pobj))->fontbold ? (iAttributes | SCI_ATTR_BOLD) : iAttributes;
      iAttributes =
	(sciGetFontContext(pobj))->fontitalic ? (iAttributes | SCI_ATTR_ITALIC) : iAttributes;
      iAttributes =
	(sciGetFontContext(pobj))->fontunderline ? (iAttributes | SCI_ATTR_UNDERLINE) : iAttributes;
      iAttributes =
	(sciGetFontContext(pobj))->fontstrikeout ? (iAttributes | SCI_ATTR_STRIKEOUT) : iAttributes;
      return iAttributes;
      break;
    case SCI_TITLE:
      iAttributes =
	(sciGetFontContext(pobj))->fontbold ? (iAttributes | SCI_ATTR_BOLD) : iAttributes;
      iAttributes =
	(sciGetFontContext(pobj))->fontitalic ? (iAttributes | SCI_ATTR_ITALIC) : iAttributes;
      iAttributes =
	(sciGetFontContext(pobj))->fontunderline ? (iAttributes | SCI_ATTR_UNDERLINE) : iAttributes;
      iAttributes =
	(sciGetFontContext(pobj))->fontstrikeout ? (iAttributes | SCI_ATTR_STRIKEOUT) : iAttributes;
      return iAttributes;
      break;
    case SCI_LEGEND:
      (sciGetFontContext(pobj))->fontbold ? (iAttributes | SCI_ATTR_BOLD) : iAttributes;
      iAttributes =
	(sciGetFontContext(pobj))->fontitalic ? (iAttributes | SCI_ATTR_ITALIC) : iAttributes;
      iAttributes =
	(sciGetFontContext(pobj))->fontunderline ? (iAttributes | SCI_ATTR_UNDERLINE) : iAttributes;
      iAttributes =
	(sciGetFontContext(pobj))->fontstrikeout ? (iAttributes | SCI_ATTR_STRIKEOUT) : iAttributes;
      return iAttributes;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC:
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      return -1;
      break;
    }
}



/**sciSetFontStyle
 * @memo Sets the font style
 * @param sciPointObj * pobj: the pointer to the entity
 * @param  int iAttributes (a logical mask with SCI_ATTR_BOLD|SCI_ATTR_ITALIC|SCI_ATTR_UNDERLINE|SCI_ATTR_STRIKEOUT)
 * @return  int 0 if OK, -1 if not
 */
int
sciSetFontStyle (sciPointObj * pobj, int iAttributes)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      (sciGetFontContext(pobj))->fontbold =
	iAttributes & SCI_ATTR_BOLD ? 700 : 0;
      (sciGetFontContext(pobj))->fontitalic =
	iAttributes & SCI_ATTR_ITALIC ? 1 : 0;
      (sciGetFontContext(pobj))->fontunderline =
	iAttributes & SCI_ATTR_UNDERLINE ? 1 : 0;
      (sciGetFontContext(pobj))->fontstrikeout =
	iAttributes & SCI_ATTR_STRIKEOUT ? 1 : 0;
      break;
    case SCI_TITLE:
      (sciGetFontContext(pobj))->fontbold =
	iAttributes & SCI_ATTR_BOLD ? 700 : 0;
      (sciGetFontContext(pobj))->fontitalic =
	iAttributes & SCI_ATTR_ITALIC ? 1 : 0;
      (sciGetFontContext(pobj))->fontunderline =
	iAttributes & SCI_ATTR_UNDERLINE ? 1 : 0;
      (sciGetFontContext(pobj))->fontstrikeout =
	iAttributes & SCI_ATTR_STRIKEOUT ? 1 : 0;
      break;
    case SCI_LEGEND:
      (sciGetFontContext(pobj))->fontbold =
	iAttributes & SCI_ATTR_BOLD ? 700 : 0;
      (sciGetFontContext(pobj))->fontitalic =
	iAttributes & SCI_ATTR_ITALIC ? 1 : 0;
      (sciGetFontContext(pobj))->fontunderline =
	iAttributes & SCI_ATTR_UNDERLINE ? 1 : 0;
      (sciGetFontContext(pobj))->fontstrikeout =
	iAttributes & SCI_ATTR_STRIKEOUT ? 1 : 0;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      return -1;
      break;
    }
  return 0;
}



/**sciSetFontName
 * @memo Sets the font name 
 * @param sciPointObj * pobj: the pointer to the entity
 * @param  char *fontname
 * @param  int n
 * @return  int 0 if OK, -1 if not
 */
int
sciSetFontName (sciPointObj * pobj, char pfontname[], int n)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      /*      if (realloc
	  (	(sciGetFontContext(pobj))->pfontname,
		n * sizeof (char)) == NULL)
	return -1;
      strncpy (	(sciGetFontContext(pobj))->pfontname, pfontname, n);
      (sciGetFontContext(pobj))->fontnamelen = n;*/
      break;
    case SCI_TITLE:
       /*if (realloc
	  (	(sciGetFontContext(pobj))->pfontname,
		n * sizeof (char)) == NULL)
	return -1;
      strncpy (	(sciGetFontContext(pobj))->pfontname, pfontname,
		n);
      (sciGetFontContext(pobj))->fontnamelen = n;*/
      break;
    case SCI_LEGEND:
      /*if (realloc
	  (	(sciGetFontContext(pobj))->pfontname,
		n * sizeof (char)) == NULL)
	return -1;
      strncpy (	(sciGetFontContext(pobj))->pfontname, pfontname,
		n);
		(sciGetFontContext(pobj))->fontnamelen = n;*/
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      return -1;
      break;
    }
  return 0;
}



/**sciGetFontName
 * @memo Gets the font name
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  char array of string font name if ok, NULL if not
 */
char *
sciGetFontName (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return 	(sciGetFontContext(pobj))->pfontname;
      break;
    case SCI_TITLE:
      return 	(sciGetFontContext(pobj))->pfontname;
      break;
    case SCI_LEGEND:
      return 	(sciGetFontContext(pobj))->pfontname;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      return (char *) NULL;
      break;
    }
}



/**sciGetFontNameLength
 * @memo Sets size of Text in TEXT, TITLE or LEGEND
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int > 0 if OK, 0 if not
 */
unsigned int
sciGetFontNameLength (sciPointObj * pobj)
{

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return 	(sciGetFontContext(pobj))->fontnamelen;
      break;
    case SCI_TITLE:
      return 	(sciGetFontContext(pobj))->fontnamelen;
      break;
    case SCI_LEGEND:
      return 	(sciGetFontContext(pobj))->fontnamelen;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("This object has no text !\n");
      return 0;
      break;
    }
}



/**sciSetLegendPos
 * @memo Sets the Legend Position in the window
 * @param sciPointObj * pobj: the pointer to the entity
 * @return 0 if ok, -1 if not
 */
int
sciSetLegendPos (sciPointObj * pobj, int x, int y)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_LEGEND:
      pLEGEND_FEATURE (pobj)->pos.x = x;
      pLEGEND_FEATURE (pobj)->pos.y = y;
      break;
    case SCI_TITLE:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_TEXT:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("Your are not using a legend object !\n");
      return -1;
      break;
    }
  return 0;
}



/**sciGetLegendPos
 * @memo Returns the Title Position
 */
POINT2D
sciGetLegendPos (sciPointObj * pobj)
{
  POINT2D tmppoint;
  if (sciGetEntityType (pobj) == SCI_TITLE)
    return pLEGEND_FEATURE (pobj)->pos;
  else
    {
      tmppoint.x = -1;
      tmppoint.y = -1;
      sciprint ("Your are not using a legend object !\n");
      return tmppoint;
    }
}



/**sciSetLegendPlace
 * @memo Sets the Title Place  with SCI_TITLE_IN_TOP or SCI_TITLE_IN_BOTTOM and calculate the real position in the window
 * @param sciPointObj * pobj: the pointer to the entity
 * @return 0 if ok, -1 if not
 */
int
sciSetLegendPlace (sciPointObj * pobj, sciLegendPlace place)
{
  int x = 0;
  int y = 0;

  if (sciGetEntityType (pobj) == SCI_TITLE)
    {
      pLEGEND_FEATURE (pobj)->place = place;
      /* calcul de l emplacement relatif du titre 
       * en fonction de la taille de la police 
       * de la fenetre...
       */
      sciSetLegendPos (pobj, x, y);
      return 0;
    }
  else
    sciprint ("Your are not using a legend object !\n");
  return -1;
}



/**sciGetLegendPlace
 * @memo Returns the Title place with SCI_TITLE_IN_TOP or SCI_TITLE_IN_BOTTOM and calculate the real position in the window
 * @param sciPointObj * pobj: the pointer to the entity
 * @return sciTitlePlace
 */
sciTitlePlace
sciGetLegendPlace (sciPointObj * pobj)
{
  return pTITLE_FEATURE (pobj)->titleplace;
}


/**sciSetTitlePos
 * @memo Sets the Title Position in the graphique window. This function is actualy private
 * @param sciPointObj * pobj: the pointer to the entity
 * @return 0 if ok, -1 if not
 */
int
sciSetTitlePos (sciPointObj * pobj, int x, int y)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TITLE:
      pTITLE_FEATURE (pobj)->pos.x = x;
      pTITLE_FEATURE (pobj)->pos.y = y;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_TEXT:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:   
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("Your are not using a title object !\n");
      return -1;
      break;
    }
  return 0;
}



/**sciGetTitlePos
 * @memo Returns the Title Position in the graphique window. This function is actualy reserved for internal use, not for scilab users
 */
POINT2D
sciGetTitlePos (sciPointObj * pobj)
{
  POINT2D tmppoint;
  if (sciGetEntityType (pobj) == SCI_TITLE)
    return pTITLE_FEATURE (pobj)->pos;
  else
    {
      tmppoint.x = -1;
      tmppoint.y = -1;
      sciprint ("Your are not using a title object !\n");
      return tmppoint;
    }
}



/**sciSetTitlePlace
 * @memo Sets the Title Place  with SCI_TITLE_IN_TOP or SCI_TITLE_IN_BOTTOM and calculate the real position in the window
 * @param sciPointObj * pobj: the pointer to the entity
 * @return 0 if ok, -1 if not
 */
int
sciSetTitlePlace (sciPointObj * pobj, sciTitlePlace place)
{
  int x = 0;
  int y = 0;

  if (sciGetEntityType (pobj) == SCI_TITLE)
    {
      pTITLE_FEATURE (pobj)->titleplace = place;
      /* calcul de l emplacement relatif du titre 
       * en fonction de la taille de la police 
       * de la fenetre...
       */
      sciSetTitlePos (pobj, x, y);
      return 0;
    }
  else
    sciprint ("Your are not using a title object !\n");
  return -1;
}



/**sciGetTitlePlace
 * @memo Returns the Title Position
 * @param sciPointObj * pobj: the pointer to the entity
 * @return sciTitlePlace if ok, -1 if not
 */
sciTitlePlace
sciGetTitlePlace (sciPointObj * pobj)
{
  if (sciGetEntityType (pobj) == SCI_TITLE)
    return pTITLE_FEATURE (pobj)->titleplace;
  else
    sciprint ("Your are not using a title object !\n");
  return -1;
}



/**sciGetFontContext
 * @memo Returns the structure of the Font Context. Do not use this in the Consturctor Functions !
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the pointer to the font context structure if ok, NULL if not
 */
sciFont *
sciGetFontContext (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return  &(pTEXT_FEATURE (pobj)->fontcontext);
      break;
    case SCI_MENU:
      return  &(pMENU_FEATURE (pobj)->text.fontcontext);
      break;
    case SCI_MENUCONTEXT:
      return  &(pMENUCONTEXT_FEATURE (pobj)->text.fontcontext);
      break;
    case SCI_STATUSB:
      return  &(pSTATUSB_FEATURE (pobj)->text.fontcontext);
      break;
    case SCI_TITLE:
      return  &(pTITLE_FEATURE (pobj)->text.fontcontext);
      break;
    case SCI_LEGEND:
      return  &(pLEGEND_FEATURE (pobj)->text.fontcontext);
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AGREG:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    default:
      return (sciFont *)NULL;
      break;
    }
}


/**sciInitFontContext
 * @memo Inits the graphic context of this object with the default value
 * @param sciPointObj * pobj: the pointer to the entity
 */
int
sciInitFontContext (sciPointObj * pobj)
{
  /* 
   * initialisation du contexte font par defaut 
   * que l'on peut recuperer sur les structures de base de scilab 
   * la colormap des fils est heritee du parent
   * nous prenons le couleur de background et foreground
   * plutot que fontforeground pour l'initialisation
   */

  /* unknown function initfontname "Win-stand"!! */
  /* static TCHAR inifontname[] = TEXT ("Times New Roman");*/  

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      (sciGetFontContext(pobj))->backgroundcolor = sciGetBackground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->foregroundcolor = sciGetForeground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->fontbold        =  FALSE;	/* Normal font in windows */
      (sciGetFontContext(pobj))->fontdeciwidth   = 100;	/* je ne fais pas le mask */
      (sciGetFontContext(pobj))->fontitalic      = FALSE;	/* tout de suite pour */
      (sciGetFontContext(pobj))->fontunderline   = FALSE;	/* garder la facilite */
      (sciGetFontContext(pobj))->fontstrikeout   = FALSE;	/*d'utilisation sur les differents OS */
      (sciGetFontContext(pobj))->textorientation = 0;
      /**/
      (sciGetFontContext(pobj))->fontnamelen=1; /*fontname not used */
      if (((sciGetFontContext(pobj))->pfontname =
	   calloc ((sciGetFontContext(pobj))->fontnamelen + 1,
		   sizeof (char))) == NULL)
	{
	  sciprint ("No more Memory for fontname\n"); 
	  return 0;
	}
      /* unknown function initfontname "Win-stand"!!  */ 
      /* strncpy ((sciGetFontContext(pobj))->pfontname, inifontname,  
		 (sciGetFontContext(pobj))->fontnamelen);*/ 
      break;
    case SCI_TITLE:
      (sciGetFontContext(pobj))->backgroundcolor =	sciGetBackground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->foregroundcolor =	sciGetForeground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->fontbold = FALSE;
      (sciGetFontContext(pobj))->fontdeciwidth = 100;
      (sciGetFontContext(pobj))->fontitalic = FALSE;
      (sciGetFontContext(pobj))->fontunderline = FALSE;
      (sciGetFontContext(pobj))->fontstrikeout = FALSE;
      (sciGetFontContext(pobj))->textorientation = 0;
      (sciGetFontContext(pobj))->fontnamelen=1; /*fontname not used */
      /* Unknown Function "WIN" */
      /*     (sciGetFontContext(pobj))->fontnamelen =lstrlen (inifontname);*/    
      if (
	  ((sciGetFontContext(pobj))->pfontname = calloc ((sciGetFontContext(pobj))->fontnamelen + 1,
							  sizeof (char))) == NULL)
	{
	  sciprint ("No more Memory for fontname\n");
	  return 0;
	}
      /* Unknown Function "WIN" */
      /*     strncpy ((sciGetFontContext(pobj))->pfontname, inifontname, 
	     (sciGetFontContext(pobj))->fontnamelen);*/
      break;
    case SCI_LEGEND:
      (sciGetFontContext(pobj))->backgroundcolor = sciGetBackground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->foregroundcolor =	sciGetForeground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->fontbold = FALSE;
      (sciGetFontContext(pobj))->fontdeciwidth = 100;
      (sciGetFontContext(pobj))->fontitalic = FALSE;
      (sciGetFontContext(pobj))->fontunderline = FALSE;
      (sciGetFontContext(pobj))->fontstrikeout = FALSE;
      (sciGetFontContext(pobj))->textorientation = 0;
      (sciGetFontContext(pobj))->fontnamelen=1; /*fontname not used */
      /* Unknown Function "WIN" */
      /*     (sciGetFontContext(pobj))->fontnamelen = lstrlen (inifontname);     */
      if (
	  ((sciGetFontContext(pobj))->pfontname = calloc ((sciGetFontContext(pobj))->fontnamelen + 1,
							  sizeof (char))) == NULL)
	{
	  sciprint ("No more Memory for fontname\n");
	  return 0;
	}
      /* Unknown Function "WIN" */
      /*      strncpy ((sciGetFontContext(pobj))->pfontname,
      	       inifontname,                       
      	       (sciGetFontContext(pobj))->fontnamelen);*/
      break;
    case SCI_AXIS:
      (sciGetFontContext(pobj))->backgroundcolor = sciGetBackground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->foregroundcolor = sciGetForeground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->fontbold = FALSE;
      (sciGetFontContext(pobj))->fontdeciwidth = 100;
      (sciGetFontContext(pobj))->fontitalic = FALSE;
      (sciGetFontContext(pobj))->fontunderline = FALSE;
      (sciGetFontContext(pobj))->fontstrikeout = FALSE;
      (sciGetFontContext(pobj))->textorientation = 0;
      (sciGetFontContext(pobj))->fontnamelen=1; /*fontname not used */
      /* Unknown Function "WIN" */
      /*(sciGetFontContext(pobj))->fontnamelen = lstrlen (inifontname); */
      if (
	  ((sciGetFontContext(pobj))->pfontname = calloc ((sciGetFontContext(pobj))->fontnamelen + 1,
							  sizeof (char))) == NULL)
	{
	  sciprint ("No more Memory for fontname\n");
	  return 0;
	} 
    case SCI_AXES:
      (sciGetFontContext(pobj))->backgroundcolor = sciGetBackground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->foregroundcolor = sciGetForeground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->fontbold = FALSE;
      (sciGetFontContext(pobj))->fontdeciwidth = 100;
      (sciGetFontContext(pobj))->fontitalic = FALSE;
      (sciGetFontContext(pobj))->fontunderline = FALSE;
      (sciGetFontContext(pobj))->fontstrikeout = FALSE;
      (sciGetFontContext(pobj))->textorientation = 0;
      (sciGetFontContext(pobj))->fontnamelen=1; /*fontname not used */
      /* Unknown Function "WIN" */
      /*      (sciGetFontContext(pobj))->fontnamelen = lstrlen (inifontname); */
      if (
	  ((sciGetFontContext(pobj))->pfontname = calloc ((sciGetFontContext(pobj))->fontnamelen + 1,
							  sizeof (char))) == NULL)
	{
	  sciprint ("No more Memory for fontname\n");
	  return 0;
	}
      /* Unknown Function "WIN" */
      /*     strncpy ((sciGetFontContext(pobj))->pfontname, inifontname, 
	     (sciGetFontContext(pobj))->fontnamelen);*/
      break;
    case SCI_MENU:
      (sciGetFontContext(pobj))->backgroundcolor = sciGetBackground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->foregroundcolor = sciGetForeground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->fontbold = FALSE;
      (sciGetFontContext(pobj))->fontdeciwidth = 100;
      (sciGetFontContext(pobj))->fontitalic = FALSE;
      (sciGetFontContext(pobj))->fontunderline = FALSE;
      (sciGetFontContext(pobj))->fontstrikeout = FALSE;
      (sciGetFontContext(pobj))->textorientation = 0;
      (sciGetFontContext(pobj))->fontnamelen=1; /*fontname not used */
      /* Unknown Function "WIN" */
      /*      (sciGetFontContext(pobj))->fontnamelen = lstrlen (inifontname);  */
      if (
	  ((sciGetFontContext(pobj))->pfontname = calloc ((sciGetFontContext(pobj))->fontnamelen + 1,
							  sizeof (char))) == NULL)
	{
	  sciprint ("No more Memory for fontname\n");
	  return 0;
	}
      /* Unknown Function "WIN" */
      /*     strncpy ((sciGetFontContext(pobj))->pfontname, inifontname,  
      	       (sciGetFontContext(pobj))->fontnamelen); */
      break;
    case SCI_MENUCONTEXT:
      (sciGetFontContext(pobj))->backgroundcolor =	sciGetBackground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->foregroundcolor =	sciGetForeground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->fontbold = FALSE;
      (sciGetFontContext(pobj))->fontdeciwidth = 100;
      (sciGetFontContext(pobj))->fontitalic = FALSE;
      (sciGetFontContext(pobj))->fontunderline = FALSE;
      (sciGetFontContext(pobj))->fontstrikeout = FALSE;
      (sciGetFontContext(pobj))->textorientation = 0;
      (sciGetFontContext(pobj))->fontnamelen=1; /*fontname not used */
      /* Unknown Function "WIN" */
      /*(sciGetFontContext(pobj))->fontnamelen =	lstrlen (inifontname);   */
      if (
	  ((sciGetFontContext(pobj))->pfontname = calloc ((sciGetFontContext(pobj))->fontnamelen + 1,
							  sizeof (char))) == NULL)
	{
	  sciprint ("No more Memory for fontname\n");
	  return 0;
	}
      /* Unknown Function "WIN" */
      /*      strncpy ((sciGetFontContext(pobj))->pfontname,
      	       inifontname,                                 
      	       (sciGetFontContext(pobj))->fontnamelen);*/
      break;
    case SCI_STATUSB:
      (sciGetFontContext(pobj))->backgroundcolor =	sciGetBackground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->foregroundcolor =	sciGetForeground (sciGetParent (pobj)) - 1;
      (sciGetFontContext(pobj))->fontbold = FALSE;
      (sciGetFontContext(pobj))->fontdeciwidth = 100;
      (sciGetFontContext(pobj))->fontitalic = FALSE;
      (sciGetFontContext(pobj))->fontunderline = FALSE;
      (sciGetFontContext(pobj))->fontstrikeout = FALSE;
      (sciGetFontContext(pobj))->textorientation = 0;
      (sciGetFontContext(pobj))->fontnamelen=1; /*fontname not used */
      /* Unknown Function "WIN" */
      /*     (sciGetFontContext(pobj))->fontnamelen =	lstrlen (inifontname);*/
      if (
	  ((sciGetFontContext(pobj))->pfontname = calloc ((sciGetFontContext(pobj))->fontnamelen + 1,
							  sizeof (char))) == NULL)
	{
	  sciprint ("No more Memory for fontname\n");
	  return 0;
	}
      /* Unknown Function "WIN" */
      /*strncpy ((sciGetFontContext(pobj))->pfontname,inifontname, 15);*/
      break;   
    case SCI_FIGURE:     
    case SCI_SUBWIN: 
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AGREG:
    default:
      return -1;
      break;
    }
  return 0;
}


/************************************************** END TEXT **********************************/

/************************************************** MENU ***************************************/
/*sciGethPopMenu
 * This function returns the handle of the menu associated with this object and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
HMENU
sciGethPopMenu (sciPointObj * pthis)
{
  /* structure BCG periWIN ! winuser.h "Window"*/
  /*  HWND hwnd_window_pointed; */

  /* On cherche le handle de la fenetre selectionnnee */
  /* hwnd_window_pointed = WindowFromPoint(Point);    */
  switch(sciGetEntityType(pthis)) {
  case SCI_SUBWIN:
    return sciGethPopMenu(pSUBWIN_FEATURE (pthis)->pPopMenu);
    break;
  case SCI_MENUCONTEXT:
    return pMENUCONTEXT_FEATURE(pthis)->hPopMenu;
    break;
  case SCI_FIGURE:
  case SCI_TEXT:
  case SCI_TITLE:
  case SCI_LEGEND:
  case SCI_ARC:
  case SCI_SEGS: 
  case SCI_FEC: 
  case SCI_GRAYPLOT: 
  case SCI_POLYLINE:
  case SCI_PATCH:
  case SCI_RECTANGLE:
  case SCI_SURFACE: 
  case SCI_LIGHT:
  case SCI_AXIS:  
  case SCI_AXES:
  case SCI_PANNER:
  case SCI_SBH:
  case SCI_SBV:
  case SCI_MENU:
  case SCI_STATUSB:
  case SCI_AGREG:
  default: 
    return (HMENU) NULL ;
    break;
  }
  return (HMENU) NULL;
}






/**sciAddLabelMenu
 * @memo This function add one label to this menu
 * @param sciPointObj * pthis: the pointer to the entity
 * @return 0 if ok, -1 if not
 */
int
sciAddLabelMenu (sciPointObj * pthis, char plabel[], int n)
{
  sciLabelMenu *pscilabelmenutmp;

  switch (sciGetEntityType (pthis))
    {
    case SCI_MENU:
      pscilabelmenutmp = (sciLabelMenu *) (pMENU_FEATURE (pthis)->plabelmenu);
      break;
    case SCI_MENUCONTEXT:
      pscilabelmenutmp =
	(sciLabelMenu *) (pMENUCONTEXT_FEATURE (pthis)->plabelmenu);
      break;
    case SCI_AGREG:
    default:
      return -1;
      break;
    }
  while (pscilabelmenutmp != (sciLabelMenu *) NULL)
    pscilabelmenutmp = (sciLabelMenu *) pscilabelmenutmp->pnextlabelmenu;
  if ((pscilabelmenutmp = MALLOC (sizeof (sciLabelMenu))) == NULL)
    return -1;
  if ((pscilabelmenutmp->plabel = calloc (n+1, sizeof (char))) == NULL)
    {
      sciprint("\nNo more place to allocates text string, try a shorter string\n");
      return -1;
    }
  strncpy (pscilabelmenutmp->plabel, plabel, n);
  pscilabelmenutmp->pnextlabelmenu = (sciLabelMenu *) NULL;
  return 0;
}


/**del sciDelLabelsMenu
 * @memo This function destroies ALL LABELS menu and the elementaries structures
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
sciDelLabelsMenu (sciPointObj * pthis)
{
  sciLabelMenu *pscilabelmenutmp;
  sciLabelMenu *pscilabelmenutofree;

  switch (sciGetEntityType (pthis))
    {
    case SCI_MENU:
      pscilabelmenutmp = (sciLabelMenu *) (pMENU_FEATURE (pthis)->plabelmenu);
      break;
    case SCI_MENUCONTEXT:
      pscilabelmenutmp =
	(sciLabelMenu *) (pMENUCONTEXT_FEATURE (pthis)->plabelmenu);
      break;
    default:
    case SCI_AGREG:
      return -1;
      break;
    }
  while (pscilabelmenutmp != (sciLabelMenu *) NULL)
    {
      /* Il faut retirer tous les menus lies par la liste aussi */
      pscilabelmenutofree = pscilabelmenutmp;
      pscilabelmenutmp = pscilabelmenutmp->pnextlabelmenu;
      FREE (pscilabelmenutofree->plabel);
      FREE (pscilabelmenutofree);
    }
  return 0;
}



/*sciAttachPopMenu
 * This function sets the handle of the menu associated with this object
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
sciAttachPopMenu (sciPointObj *pthis, sciPointObj *pPopMenu)
{
  /*  HWND hwnd_window_pointed;*/     /* structure BCG periWIN ! winuser.h "Window */
   
  if (sciGetEntityType(pPopMenu) == SCI_MENUCONTEXT) /* verification of the validate type */
    {
      /* On cherche le handle de la fenetre selectionnnee */
      /* hwnd_window_pointed = WindowFromPoint(Point);    */
      switch(sciGetEntityType(pthis)) {
      case SCI_SUBWIN:
	pSUBWIN_FEATURE (pthis)->pPopMenu = pPopMenu;
	return 0;
	break;
      case SCI_MENUCONTEXT:
      case SCI_FIGURE:
      case SCI_TEXT:
      case SCI_TITLE:
      case SCI_LEGEND:
      case SCI_ARC:
      case SCI_SEGS: 
      case SCI_FEC:
      case SCI_GRAYPLOT: 
      case SCI_POLYLINE:
      case SCI_PATCH:
      case SCI_RECTANGLE:
      case SCI_SURFACE: 
      case SCI_LIGHT:
      case SCI_AXIS:  
      case SCI_AXES:
      case SCI_PANNER:
      case SCI_SBH:
      case SCI_SBV:
      case SCI_MENU:
      case SCI_STATUSB:
      case SCI_AGREG:
      default: 
	return -1;
	break;
      }
    }
  sciprint("Your second entity is not a popup menu !\n");
  return -1;
}

/*********************************************** END MENU ***************************************/

/**sciGetScrollPosV
 * @memo Returns the vertical scrollbar position
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the scroll bar position
 */
int
sciGetScrollPosV (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_SBV:
    default:
      /*   return GetScrollPos ((sciGetScilabXgc (pobj))->CWindow, SB_VERT);*/ /* GetScrollPos fct stand dans WIN */
      return -1;
      break;
    }
}



/**sciGetScrollPosH
 * @memo Returns the vertical scrollbar position
 * @param sciPointObj * pobj: the pointer to the entity
 * @return the scroll bar position
 */
int
sciGetScrollPosH (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_SBH:
    default:
      /* return GetScrollPos ((sciGetScilabXgc (pobj))->CWindow, SB_HORZ);*/ /* GetScrollPos fct stand dans WIN */
      return -1;
      break;
    }
}



/********************* modifier le 01/02/2002 ************************
 * On detruit pas la sous fenetre, elle est initialiser avec la figure
 * pour cette version, on considere qu'il y'a 1 seule sous fenetre et 
 * elle suit la fenetre principale (voir xbasc() ), la fenetre n'est pas 
 * consideree comme un des fils.  
 **DestroyAllGraphicsSons
 * This function destroies childs and pthis
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyAllGraphicsSons (sciPointObj * pthis)
{
  /* to destroy only the sons put the while into the switch !*/
  sciSons *toto;
 
  toto = sciGetSons (pthis);
  while ((toto != (sciSons *) NULL) &&
	 (toto->pointobj != (sciPointObj *) NULL))
    {
      DestroyAllGraphicsSons (toto->pointobj);
      toto = sciGetSons (pthis);
    }
  switch (sciGetEntityType (pthis))
    {

    case SCI_FIGURE:
      return 0;
      break;
    case SCI_SUBWIN:
      DestroySubWin (pthis);
      return 0;
      break;
    case SCI_TEXT:
      DestroyText (pthis);
      return 0;
      break;
    case SCI_TITLE:
      DestroyTitle (pthis);
      return 0;
      break;
    case SCI_LEGEND:
      DestroyLegend (pthis);
      return 0;
      break;
    case SCI_ARC:
      DestroyArc (pthis);
      return 0; 
      break;
    case SCI_SEGS:
      DestroySegs (pthis);
      return 0;
      break;  
    case SCI_FEC:  
      DestroyFec (pthis);
      return 0;
      break; 
    case SCI_GRAYPLOT: 
      DestroyGrayplot (pthis);
      return 0;
      break; 
    case SCI_POLYLINE:
      DestroyPolyline (pthis);
      return 0;
      break;
    case SCI_PATCH:
      DestroyPatch (pthis);
      return 0;
      break;
    case SCI_RECTANGLE:
      DestroyRectangle (pthis);
      return 0;
      break;
    case SCI_SURFACE:
      DestroySurface (pthis);
      return 0;
      break;
    case SCI_LIGHT:
      break;
    case SCI_AXIS:
      DestroyAxis (pthis);
      return 0;
      break;
    case SCI_AXES:
      DestroyAxes (pthis);
      return 0;
      break;
    case SCI_PANNER:
      break;
    case SCI_SBH:
      DestroyScrollH (pthis);
      return 0;
      break;
    case SCI_SBV:
      DestroyScrollV (pthis);
      return 0;
      break;
    case SCI_MENU:
      DestroySciMenu (pthis);
      return 0;
      break;
    case SCI_MENUCONTEXT:
      DestroyMenuContext (pthis);
      return 0;
      break;
    case SCI_STATUSB:
      DestroyStatusBar (pthis);
      return 0;
      break;
    case SCI_AGREG:
      DestroyAgregation (pthis);
      return 0;
      break;
    default:
      sciprint ("This object cannot be destroyall\n");
      return -1;
      break;
    }

  return 0;
}



/**sciDelGraphicObj
 * This function delete only users graphics object and its dependency limited to SCI_TITLE SCI_LEGEND SCI_ARC case SCI_POLYLINE SCI_RECTANGLE SCI_PATCH SCI_SURFACE SCI_AXIS SCI_MENU SCI_MENUCONTEXT SCI_SUBWIN
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
sciDelGraphicObj (sciPointObj * pthis)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_PATCH:
    case SCI_SURFACE:
    case SCI_AXIS:    
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_TEXT:
      DestroyAllGraphicsSons (pthis);
      return 0;
      break;
    case SCI_SUBWIN:
      if (sciGetAxes (sciGetParent(pthis),pthis) == (sciPointObj *) NULL)
      	sciXbasc();
      else
        DestroyAllGraphicsSons (pthis);
      return 0;
      break;         
    case SCI_FIGURE:
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_STATUSB:
    default:
      sciprint ("This object cannot be deleted\n");
      return -1;
      break;
    }

  return 0;
}

/**************************************** CONSTRUCTORS ********************************************/


/**ConstructFigure
 * @memo This function creates the parents window (manager) and the elementaries structures
 */
/************ 18/01/2002 ***********/
sciPointObj *
ConstructFigure (XGC)
     struct BCG *XGC;
{
 
  sciPointObj *pobj = (sciPointObj *) NULL;
  /* data for scilab function getwindowdim getpopupdim ... */
  integer verbose = 0;
  integer x[2];
  integer narg = 0, dummy=0, m; 


  /* memory allocation for the new Figure   affectation du type allocation de la structure */

  if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
    {
      return NULL;	  
    }
  sciSetEntityType (pobj, SCI_FIGURE);
  if ((pobj->pfeatures = MALLOC ((sizeof (sciFigure)))) == NULL)
    {
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
  /* add a number in the HandelTable  reallocation de la table + 1 puis affectation de l'indice    */
  
  if (sciAddNewHandle (pobj) == -1)
    {
      FREE(pobj->pfeatures);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
 
  /*  le seul parent est la root (l'ecran, le bureau quoi !) Pour l'instant il n'y a pas de fils selectionne */
  /*sciSetParent (pobj, (sciPointObj *) NULL); */
  /* il n'y a pas de parents !!!*/
  
  if (!(sciAddThisToItsParent(pobj, (sciPointObj *)NULL))) 
    {
      sciDelHandle (pobj);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
  
  sciSetCurrentSon (pobj, (sciPointObj *) NULL);

  pFIGURE_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
  pFIGURE_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;
  pFIGURE_FEATURE (pobj)->pScilabXgc = XGC;
  
  /** the colormap is mx3 matrix */
  m = sciGetNumColors (pobj);
  if((pFIGURE_FEATURE(pobj)->pcolormap = (double *) MALLOC (m * 3 * sizeof (double))) == (double *) NULL)
    {
      sciDelHandle (pobj);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }  
    
  C2F(dr)("xget", "colormap",&verbose,&narg,&dummy,PI0,PI0,PI0,pFIGURE_FEATURE(pobj)->pcolormap,PD0,PD0,PD0,4L,8L);
  /* initialisation de context et mode graphique par defaut */
  if (sciInitGraphicContext (pobj) == -1)
    {
      sciDelHandle (pobj);
      FREE(pFIGURE_FEATURE(pobj)->pcolormap);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
  if (sciInitGraphicMode (pobj) == -1)
    {
      sciDelHandle (pobj);    
      FREE(pFIGURE_FEATURE(pobj)->pcolormap);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }   

  strncpy (pFIGURE_FEATURE (pobj)->name, "Scilab Graphic", sizeof ("Scilab Graphic") + 4);
  pFIGURE_FEATURE (pobj)->namelen = Min (sizeof ("Scilab Graphic") + 4, 14); 
  sciSetNum (pobj, &(XGC->CurWindow));
  pFIGURE_FEATURE (pobj)->figuredimwidth = XGC->CWindowWidth;
  pFIGURE_FEATURE (pobj)->figuredimheight = XGC->CWindowHeight;
  C2F(dr)("xget","wpos",&verbose,x,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,4L);
  sciSetFigurePos (pobj, x[0], x[1]);
  pFIGURE_FEATURE (pobj)->isiconified = FALSE;
  pFIGURE_FEATURE (pobj)->isselected = TRUE;
  pFIGURE_FEATURE (pobj)->rotstyle = 0;
  pFIGURE_FEATURE (pobj)->visible = TRUE;
  pFIGURE_FEATURE (pobj)->numsubwinselected = 0; 
  return pobj;
}


/**DestroyFigure
 * @memo This function destroies the parents window (manager) and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyFigure (sciPointObj * pthis)
{
  /* there is no needed for 
   * sciDelThisToItsParent(pthis, );
   */
  if (sciDelHandle (pthis) == -1)
    return -1;
  /* This code has to be validated on all systems
   * because sciGetPointerToFeature returns a void
   */
  FREE(pFIGURE_FEATURE(pthis)->pcolormap);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* delete windows() */
  return 0;
}



/**ConstructStatusBar
 * @memo this function creates the StatusBar of Figure and the elementaries structures
 */
sciPointObj *
ConstructStatusBar (sciPointObj * pparentfigure)
{
  sciPointObj *pobj = (sciPointObj *) NULL;

  if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return NULL;
      sciSetEntityType (pobj, SCI_STATUSB);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciStatusBar)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentfigure);*/
      if (!(sciAddThisToItsParent (pobj, pparentfigure)))
	{
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pSTATUSB_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pSTATUSB_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;
      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciInitFontContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      return (sciPointObj *)pobj;
    }
  else
    {
      sciprint ("The parent has to be a FIGURE \n");
      return (sciPointObj *) NULL;
    }
}



/**DestroyStatusBar
 * @memo This function destroies the StatusBar of Figure and the elementaries structures and returns pthis with NULL pointer and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyStatusBar (sciPointObj * pthis)
{
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer la figure parente */
  return 0;
}



/**ConstructSubWin
 * @memo This function creates the Subwindow (the Axe) and the elementaries structures
 */
sciPointObj *
ConstructSubWin (sciPointObj * pparentfigure, int pwinnum)
{

  char dir;
  int i;

  if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
    {
      sciPointObj *pobj = (sciPointObj *) NULL;
      /*if (sciInitChildWin (pparentfigure, pwinname) == -1)
	return NULL;*/
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return NULL;
      sciSetEntityType (pobj, SCI_SUBWIN);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciSubWindow)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentfigure);*/
      if (!(sciAddThisToItsParent (pobj, pparentfigure)))
	{
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pSUBWIN_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pSUBWIN_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;
      pSUBWIN_FEATURE (pobj)->callback = (char *)NULL;
      pSUBWIN_FEATURE (pobj)->callbacklen = 0;
      pSUBWIN_FEATURE (pobj)->callbackevent = 100;

      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);          
	  return (sciPointObj *) NULL;
	}   
      if (sciInitGraphicMode (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	} 

      pSUBWIN_FEATURE (pobj)->logflags[0] = 'n';
      pSUBWIN_FEATURE (pobj)->logflags[1] = 'n';

  
     /* axes labelling values*/
      pSUBWIN_FEATURE (pobj)->axes.ticscolor  = -1;
      pSUBWIN_FEATURE (pobj)->axes.textcolor  = -1;
      pSUBWIN_FEATURE (pobj)->axes.fontsize  = -1;  
      pSUBWIN_FEATURE (pobj)->axes.subint[0]  = 1;   
      pSUBWIN_FEATURE (pobj)->axes.subint[1]  = 1; 
      pSUBWIN_FEATURE (pobj)->axes.xdir='d'; /*SS 02/01/03 */
      pSUBWIN_FEATURE (pobj)->axes.ydir='l'; /*SS 02/01/03 */


      pSUBWIN_FEATURE (pobj)->axes.rect  = 1;
      pSUBWIN_FEATURE (pobj)->axes.limits[0]  = 0;
      pSUBWIN_FEATURE (pobj)->grid  = -1;
      pSUBWIN_FEATURE (pobj)->isaxes  = FALSE;
     
      dir= 'd'; pSUBWIN_FEATURE (pobj)->axes.xdir=dir;
      dir= 'l'; pSUBWIN_FEATURE (pobj)->axes.ydir=dir;      
      for (i=0 ; i<4 ; i++)
        {  pSUBWIN_FEATURE (pobj)->axes.xlim[i]= Cscale.xtics[i]; 
	pSUBWIN_FEATURE (pobj)->axes.ylim[i]= Cscale.ytics[i]; }

      pSUBWIN_FEATURE (pobj)->WRect[0]   = 0;
      pSUBWIN_FEATURE (pobj)->WRect[1]   = 0;
      pSUBWIN_FEATURE (pobj)->WRect[2]   = 1;
      pSUBWIN_FEATURE (pobj)->WRect[3]   = 1;

      pSUBWIN_FEATURE (pobj)->FRect[0]   = 0;
      pSUBWIN_FEATURE (pobj)->FRect[1]   = 0;
      pSUBWIN_FEATURE (pobj)->FRect[2]   = 1;
      pSUBWIN_FEATURE (pobj)->FRect[3]   = 1;
         
      /* on set cette fenetre comme selectionnee par defaut a son parent */
      pSUBWIN_FEATURE (pobj)->isselected = FALSE; /* on place la valeur par defaut pour le bon fonctionnement*/ 
      /** 25/11/2002 **/
      pSUBWIN_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj)); 
      pSUBWIN_FEATURE (pobj)->isclip = -1;
            
      if (sciSetSelectedSubWin(pobj) != 1)
	    
	return (sciPointObj *)NULL;
	    
      /* recupere dans les structures Scilab wdim
         C2F(getwindowdim)(&verbose, x, &narg,&dummy);
         sciSetSubWinDim(pobj, x, x+1); */

      pSUBWIN_FEATURE (pobj)->pPopMenu = (sciPointObj *)NULL;/* initialisation of popup menu*/

      /* contruit le menu contextuel  pour tester */
      /*sprintf(strtmp, "%d",sciGetNum(pobj));
	sciAttachPopMenu(pobj, ConstructMenuContext(pparentfigure, strtmp, strlen(strtmp)));*/
      /*14/03/2002*/ sciSetCurrentObj (pobj);
      return (sciPointObj *)pobj;
    }
  else
    {
      sciprint ("The parent has to be a FIGURE \n");
      return (sciPointObj *) NULL;
    }
  return (sciPointObj *) NULL;
}



/**DestroySubWin
 * @memo This function destroies the Subwindow (the Axe) and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroySubWin (sciPointObj * pthis)
{ 
 
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  if ( sciGetCallback(pthis) != (char *)NULL)
    FREE(sciGetCallback(pthis));
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis); 
  return 0;
}



/**ConstructScrollV
 * @memo This function creates the scroll bar erticall
 */
sciPointObj *
ConstructScrollV (sciPointObj * pparentfigure)
{
  sciPointObj *pobjsbv = (sciPointObj *) NULL;

  if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
    {
      /* definition of scrollbars with original code */
      /* definition of the vertical scroll bar in the new struct */
      if ((pobjsbv = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return NULL;
      sciSetEntityType (pobjsbv, SCI_SBV);
      if ((pobjsbv->pfeatures = MALLOC ((sizeof (sciScrollBarVert)))) == NULL)
	{
	  FREE(pobjsbv);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobjsbv) == -1)
	{
	  FREE(pobjsbv->pfeatures);
	  FREE(pobjsbv);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobjsbv, pparentfigure);*/
      if (!(sciAddThisToItsParent (pobjsbv, pparentfigure)))
	{
	  sciDelHandle (pobjsbv);
	  FREE(pobjsbv->pfeatures);
	  FREE(pobjsbv);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobjsbv, (sciPointObj *) NULL);
      pSBH_FEATURE (pobjsbv)->relationship.psons = (sciSons *) NULL;
      pSBH_FEATURE (pobjsbv)->relationship.plastsons = (sciSons *) NULL;
      return pobjsbv;
    }
  else
    {
      sciprint ("The parent has to be a FIGURE \n");
      return (sciPointObj *) NULL;
    }
}



/**ConstructScrollH
 * @memo This function creates horizontal scroll bar 
 */
sciPointObj *
ConstructScrollH (sciPointObj * pparentfigure)
{
  sciPointObj *pobjsbh = (sciPointObj *) NULL;

  if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
    {
      /* definition of scrollbars with original code */
      /* definition of the horizontal scroll bar in the new struct */
      if ((pobjsbh = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return NULL;
      sciSetEntityType (pobjsbh, SCI_SBH);
      if ((pobjsbh->pfeatures = MALLOC ((sizeof (sciScrollBarHorz)))) == NULL)
	{
	  FREE(pobjsbh);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobjsbh) == -1)
	{
	  FREE(pobjsbh->pfeatures);
	  FREE(pobjsbh);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobjsbh, pparentfigure);*/
      if (!(sciAddThisToItsParent (pobjsbh, pparentfigure)))
	{
	  sciDelHandle (pobjsbh);
	  FREE(pobjsbh->pfeatures);
	  FREE(pobjsbh);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobjsbh, (sciPointObj *) NULL);
      pSBH_FEATURE (pobjsbh)->relationship.psons = (sciSons *) NULL;
      pSBH_FEATURE (pobjsbh)->relationship.plastsons = (sciSons *) NULL;
      return pobjsbh;
    }
  else
    {
      sciprint ("The parent has to be a FIGURE \n");
      return (sciPointObj *) NULL;
    }
}



/**DestroyScrollV
 * @memo This function destroies the Subwindow (the Axe) and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyScrollV (sciPointObj * pthis)
{
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer la figure parente */
  return 0;
}



/**DestroyScrollH
 * @memo This function destroies the Subwindow (the Axe) and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyScrollH (sciPointObj * pthis)
{
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer la figure parente */
  return 0;
}



/**ConstructText
 * @memo This function creates the parents window (manager) and the elementaries structures
 * @param  sciPointObj *pparentsubwin :
 * @param  char text[] : intial text string.
 * @param  int n : the number of element in text
 * @return  : pointer sciPointObj if ok , NULL if not
 */
sciPointObj *
ConstructText (sciPointObj * pparentsubwin, char text[], int n, double x,
	       double y, double wy)
{
  sciPointObj *pobj = (sciPointObj *) NULL;

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN) 
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_TEXT);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciText)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pTEXT_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pTEXT_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pTEXT_FEATURE (pobj)->callback = (char *)NULL;
      pTEXT_FEATURE (pobj)->callbacklen = 0;
      pTEXT_FEATURE (pobj)->callbackevent = 100;
      pTEXT_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure (pobj));  
      pTEXT_FEATURE (pobj)->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); 

      if ((pTEXT_FEATURE (pobj)->ptextstring = calloc (n+1, sizeof (char))) ==
	  NULL)
	{
	  sciprint("No more place to allocates text string, try a shorter string");
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);	  
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      strncpy (pTEXT_FEATURE (pobj)->ptextstring, text, n);
      pTEXT_FEATURE (pobj)->textlen = n;
      pTEXT_FEATURE (pobj)->x = x;
      pTEXT_FEATURE (pobj)->y = y;
      pTEXT_FEATURE (pobj)->wy = wy;
      if (sciInitFontContext (pobj) == -1)
	{
	  FREE(pTEXT_FEATURE (pobj)->ptextstring);	  
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);	  
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}


/**CloneText
 * @memo 
 * @param sciPointObj * pthis: the pointer to the entity
 */
sciPointObj *
CloneText (sciPointObj * pthis)
{
  sciPointObj *pobj, *subwinparent;
  subwinparent = pthis;

  while ((sciGetEntityType(subwinparent = sciGetParent(subwinparent)) != SCI_SUBWIN)
	 && (sciGetEntityType(subwinparent) != -1));
  if (sciGetEntityType(subwinparent) == -1)
    return (sciPointObj *)NULL;
  if (!(pobj = ConstructText (subwinparent, sciGetText(pthis), sciGetTextLength(pthis), 
			      sciGetTextPosX(pthis), sciGetTextPosY(pthis),0)))
    return (sciPointObj *)NULL;
  if (sciSetBackground(pobj, sciGetBackground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetForeground(pobj, sciGetForeground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetFontDeciWidth(pobj, sciGetFontDeciWidth (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetFontOrientation(pobj, sciGetFontOrientation (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetFontStyle(pobj,sciGetFontStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetFontName(pobj, sciGetFontName (pthis), sciGetFontNameLength(pthis)) == -1)
    return (sciPointObj *)NULL;
	
  return (sciPointObj *)pobj;
}


/**DestroyText
 * @memo This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyText (sciPointObj * pthis)
{
  FREE (pTEXT_FEATURE (pthis)->ptextstring);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}



/**ConstructTitle
 * @memo This function creates Title structure
 * @param  sciPointObj *pparentsubwin
 * @param  char text[] : intial text string.
 * @param  int n the number of element in text
 * @param  sciTitlePlace place : the title's place  (SCI_TITLE_IN_TOP, SCI_TITLE_IN_BOTTOM)
 * @return  : pointer sciPointObj if ok , NULL if not
 */
sciPointObj *
ConstructTitle (sciPointObj * pparentsubwin, char text[], int type)
{
  sciPointObj *pobj = (sciPointObj *) NULL;

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC (sizeof (sciPointObj))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_TITLE);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciTitle)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pTITLE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pTITLE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);

      pTITLE_FEATURE (pobj)->text.relationship.psons = (sciSons *) NULL;
      pTITLE_FEATURE (pobj)->text.relationship.plastsons = (sciSons *) NULL;

      pTITLE_FEATURE (pobj)->text.callback = (char *)NULL;
      pTITLE_FEATURE (pobj)->text.callbacklen = 0; 
      pTITLE_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj)); 
     

      if ((pTITLE_FEATURE (pobj)->text.ptextstring =calloc (strlen(text)+1, sizeof (char))) == NULL)
	{
	  sciprint("No more place to allocates text string, try a shorter string");
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pTITLE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /* on copie le texte du titre dans le champs specifique de l'objet */
      strncpy (pTITLE_FEATURE (pobj)->text.ptextstring, text, strlen(text));
      pTITLE_FEATURE (pobj)->text.textlen = strlen(text);
      pTITLE_FEATURE (pobj)->ptype = type;

      pTITLE_FEATURE (pobj)->text.fontcontext.textorientation = 0;

      pTITLE_FEATURE (pobj)->titleplace = SCI_TITLE_IN_TOP;
      pTITLE_FEATURE (pobj)->isselected = TRUE;
      if (sciInitFontContext (pobj) == -1)
	{
	  FREE(pTITLE_FEATURE(pobj)->text.ptextstring);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pTITLE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      return (sciPointObj *) pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}


/**DestroyTitle
 * @memo This function destroies the Subwindow (the Axe) and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyTitle (sciPointObj * pthis)
{
  FREE (pTITLE_FEATURE (pthis)->text.ptextstring);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}



/**constructLegend
 * @memo This function creates  Legend structure
 */
sciPointObj *
ConstructLegend (sciPointObj * pparentsubwin, char text[], int n, int nblegends, int *pstyle
		 , sciPointObj **pptabofpointobj)
{
  sciPointObj *pobj = (sciPointObj *) NULL;

  /*
   * verifier qu'il n'y a pas d'objet existant !!!! 
   * si oui alors le detruire puis le reconstruire.
   * car il ne peut y avoir qu'une legende
   */
  sciSons *psonstmp;
  int i=0;

  psonstmp = sciGetSons (pparentsubwin);
  /* init */
  if (psonstmp != (sciSons *) NULL)	/* on peut commencer sur le next */
    /* tant que le fils n'est pas une legende */
    while ((psonstmp->pnext != (sciSons *) NULL)
	   && sciGetEntityType (psonstmp->pointobj) != SCI_LEGEND)
      psonstmp = psonstmp->pnext;

  if (sciGetEntityType (psonstmp->pointobj) == SCI_LEGEND)
    DestroyLegend (psonstmp->pointobj);

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_LEGEND);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciLegend)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pLEGEND_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pLEGEND_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pLEGEND_FEATURE (pobj)->text.relationship.psons = (sciSons *) NULL;
      pLEGEND_FEATURE (pobj)->text.relationship.plastsons = (sciSons *) NULL;
      pLEGEND_FEATURE (pobj)->text.callback = (char *)NULL;
      pLEGEND_FEATURE (pobj)->text.callbacklen = 0;
      pLEGEND_FEATURE (pobj)->text.callbackevent = 100; 
      pLEGEND_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj)); 
      

      /* Allocation de la structure sciText */
      if ((pLEGEND_FEATURE (pobj)->text.ptextstring = calloc (n+1, sizeof (char))) == NULL)
	{
	  sciprint("\nNo more place to allocates text string, try a shorter string\n");
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pLEGEND_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /* on copie le texte du titre dans le champs specifique de l'objet */
      strncpy (pLEGEND_FEATURE (pobj)->text.ptextstring, text, n);
      pLEGEND_FEATURE (pobj)->nblegends = nblegends;

      if ((pLEGEND_FEATURE (pobj)->pptabofpointobj = 
	   MALLOC(nblegends*sizeof(sciPointObj*))) == NULL)
	{
	  sciprint("No more memory for legend\n");
	  FREE(pLEGEND_FEATURE (pobj)->text.ptextstring);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pLEGEND_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      if ((pLEGEND_FEATURE (pobj)->pstyle = MALLOC(nblegends*sizeof(int))) != NULL)
	{
	  for (i=0; i < nblegends; i++)
	    {
	      pLEGEND_FEATURE (pobj)->pstyle[i] = pstyle[i];
	      pLEGEND_FEATURE (pobj)->pptabofpointobj[i] = pptabofpointobj[i];
	    }
	}
      else
	{		  
	  sciprint("\nNo more place to allocates style\n");
	  FREE(pLEGEND_FEATURE (pobj)->pptabofpointobj);
	  FREE(pLEGEND_FEATURE (pobj)->text.ptextstring);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pLEGEND_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}



      pLEGEND_FEATURE (pobj)->text.textlen = n;
      pLEGEND_FEATURE (pobj)->text.fontcontext.textorientation = 0;
      pLEGEND_FEATURE (pobj)->pos.x = 0;
      pLEGEND_FEATURE (pobj)->pos.y = 0;
      pLEGEND_FEATURE (pobj)->width = 0;
      pLEGEND_FEATURE (pobj)->height = 0;
      pLEGEND_FEATURE (pobj)->place = SCI_LEGEND_IN_INSIDE;
      pLEGEND_FEATURE (pobj)->isselected = TRUE;
      pLEGEND_FEATURE (pobj)->issurround = FALSE;

      if (sciInitFontContext (pobj) == -1)
	{
	  sciprint("Problem with sciInitFontContext\n");
	  FREE(pLEGEND_FEATURE (pobj)->pptabofpointobj);
	  FREE(pLEGEND_FEATURE (pobj)->text.ptextstring);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pLEGEND_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}


/**DestroyLegend
 * @memo This function destroies legend and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyLegend (sciPointObj * pthis)
{
  FREE(pLEGEND_FEATURE (pthis)->pptabofpointobj);
  FREE (pLEGEND_FEATURE (pthis)->text.ptextstring);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}



/**ConstructPolyline
 * @memo This function creates  Polyline 2d structure
 */
sciPointObj *
ConstructPolyline (sciPointObj * pparentsubwin, double *pvecx, double *pvecy,
		   int closed, int n1, int n2,int plot)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciPolyline *ppoly = (sciPolyline *) NULL;
  double xmax, ymax;

  int i = 0;
  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_POLYLINE);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciPolyline)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pPOLYLINE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pPOLYLINE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pPOLYLINE_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pPOLYLINE_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pPOLYLINE_FEATURE (pobj)->callback = (char *)NULL;
      pPOLYLINE_FEATURE (pobj)->callbacklen = 0; 
      pPOLYLINE_FEATURE (pobj)->callbackevent = 100; 
      pPOLYLINE_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj)); 
      pPOLYLINE_FEATURE (pobj)->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); 

      pPOLYLINE_FEATURE (pobj)->isselected = TRUE;
      ppoly = pPOLYLINE_FEATURE (pobj);
      if ((ppoly->pvector = MALLOC (n1 * sizeof (POINT2D))) == NULL)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pPOLYLINE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /* pour le moment je garde les vecteurs separes, et non en POINT2D */
      if ((ppoly->pvx = MALLOC (n1 * sizeof (double))) == NULL)
	{
	  FREE(pPOLYLINE_FEATURE (pobj)->pvector);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pPOLYLINE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((ppoly->pvy = MALLOC (n1 * sizeof (double))) == NULL)
	{
	  FREE(pPOLYLINE_FEATURE (pobj)->pvx);
	  FREE(pPOLYLINE_FEATURE (pobj)->pvector);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pPOLYLINE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      ppoly->xmin   = pvecx[0];
      ppoly->ymin   = pvecy[0];
      xmax          = 0;
      ymax          = 0;
      ppoly->width  = 0;
      ppoly->height = 0;

      for (i = 0; i < n1; i++)
	{
	  ppoly->pvx[i] = 0;
	  ppoly->pvy[i] = 0; 
	}
      for (i = 0; i < n1; i++)
	{
	  ppoly->pvector[i].x = pvecx[i];
	  ppoly->pvector[i].y = pvecy[i];
	  ppoly->pvx[i]       = pvecx[i];
	  ppoly->pvy[i]       = pvecy[i];
  
	  ppoly->xmin   = Min(ppoly->pvx[i], ppoly->xmin);
	  xmax          = Max(ppoly->pvx[i] ,xmax);

	  ppoly->ymin   = Min(ppoly->pvy[i], ppoly->ymin);
	  ymax          = Max(ppoly->pvy[i], ymax);
	}
  
      ppoly->width  = fabs(xmax - ppoly->xmin);
      ppoly->height = fabs(ymax - ppoly->ymin); 
      ppoly->n1 = n1;		/* memorisation du nombre des courbes */
      ppoly->n2 = n2;		/* memorisation du nombre de points */
      ppoly->closed = closed;
      ppoly->plot = plot; 

      if (sciInitGraphicContext (pobj) == -1)
	{
	  FREE(pPOLYLINE_FEATURE (pobj)->pvy);
	  FREE(pPOLYLINE_FEATURE (pobj)->pvx);
	  FREE(pPOLYLINE_FEATURE (pobj)->pvector);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pPOLYLINE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}


/**ClonePolyline
 * @memo This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
sciPointObj *
ClonePolyline (sciPointObj * pthis)
{
  sciPointObj * pobj, *subwinparent;
  subwinparent = pthis;

  while ((sciGetEntityType(subwinparent = sciGetParent(subwinparent)) != SCI_SUBWIN)
	 && (sciGetEntityType(subwinparent) != -1));
  if (sciGetEntityType(subwinparent) == -1)
    return (sciPointObj *)NULL;
  if (!(pobj = ConstructPolyline (subwinparent, pPOLYLINE_FEATURE(pthis)->pvx, pPOLYLINE_FEATURE(pthis)->pvy,
				  pPOLYLINE_FEATURE(pthis)->closed, pPOLYLINE_FEATURE(pthis)->n1,pPOLYLINE_FEATURE(pthis)->n2,pPOLYLINE_FEATURE(pthis)->plot)))
    return (sciPointObj *)NULL;
  if (sciSetBackground(pobj, sciGetBackground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetForeground(pobj, sciGetForeground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineStyle(pobj, sciGetLineStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetFillStyle(pobj, sciGetFillStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineWidth(pobj, sciGetLineWidth (pthis)) == -1)
    return (sciPointObj *)NULL;

  return (sciPointObj *)pobj;
}


/**DestroyPolyline
 * @memo This function destroies Polyline and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyPolyline (sciPointObj * pthis)
{
  FREE (pPOLYLINE_FEATURE (pthis)->pvector);
  FREE (pPOLYLINE_FEATURE (pthis)->pvx);
  FREE (pPOLYLINE_FEATURE (pthis)->pvy);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}

/********************** 14/05/2002 *****
 **ConstructGayplot
 * @memo This function creates Grayplot
 */
sciPointObj *
ConstructGrayplot (sciPointObj * pparentsubwin, double *pvecx, double *pvecy,
		   double *pvecz, int n1, int n2, int type)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciGrayplot *pgray = (sciGrayplot *) NULL;
  int i = 0,cmpt;
  
  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_GRAYPLOT);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciGrayplot)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pGRAYPLOT_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pGRAYPLOT_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pGRAYPLOT_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pGRAYPLOT_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pGRAYPLOT_FEATURE (pobj)->callback = (char *)NULL;
      pGRAYPLOT_FEATURE (pobj)->callbacklen = 0; 
      pGRAYPLOT_FEATURE (pobj)->callbackevent = 100;

      pGRAYPLOT_FEATURE (pobj)->isselected = TRUE; 
      pGRAYPLOT_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj));
      pGRAYPLOT_FEATURE (pobj)->type = type;
   
      strncpy (pGRAYPLOT_FEATURE (pobj)->datamapping, "scaled", 6);
      pgray = pGRAYPLOT_FEATURE (pobj);
      
      if ((pgray->pvecx = MALLOC (n1 * sizeof (double))) == NULL)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pGRAYPLOT_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      cmpt = (type == 2)? 4:n2 ;
      if (type != 2)
	if ((pgray->pvecy = MALLOC (cmpt * sizeof (double))) == NULL)
	  {
	    FREE(pGRAYPLOT_FEATURE (pobj)->pvecx);
	    sciDelThisToItsParent (pobj, sciGetParent (pobj));
	    sciDelHandle (pobj);
	    FREE(pGRAYPLOT_FEATURE(pobj));
	    FREE(pobj);
	    return (sciPointObj *) NULL;
	}
      if ((pgray->pvecz = MALLOC ((n1*n2) * sizeof (double))) == NULL)
	{
	  FREE(pGRAYPLOT_FEATURE (pobj)->pvecx);
	  FREE(pGRAYPLOT_FEATURE (pobj)->pvecy);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pGRAYPLOT_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      for (i = 0; i < n1; i++)
	{
	  pgray->pvecx[i] = pvecx[i];
	}
      if (type != 2)
	for (i = 0; i < n2; i++)
	  {
	    pgray->pvecy[i] = pvecy[i];
	  }
      for (i = 0; i < (n1*n2); i++)
	{
	  pgray->pvecz[i] = pvecz[i];
	}
      pgray->nx = n1;	      
      pgray->ny = n2;		
      if (sciInitGraphicContext (pobj) == -1)
	{
	  FREE(pGRAYPLOT_FEATURE (pobj)->pvecx);
	  FREE(pGRAYPLOT_FEATURE (pobj)->pvecy); 
	  FREE(pGRAYPLOT_FEATURE (pobj)->pvecz);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pGRAYPLOT_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}

/**DestroyGrayplot
 * @memo This function destroies Grayplot and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 * Djalel ABDEMOUCHE
 */
int
DestroyGrayplot (sciPointObj * pthis)
{
  FREE (pGRAYPLOT_FEATURE (pthis)->pvecx);
  if (pGRAYPLOT_FEATURE (pthis)->type != 2)
    FREE (pGRAYPLOT_FEATURE (pthis)->pvecy);
  FREE (pGRAYPLOT_FEATURE (pthis)->pvecz);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}


/********************** 21/05/2002 *****
 **ConstructFec
 * @memo This function creates Grayplot
 * @author Djalel.ABDEMOUCHE
 * @see sciSetCurrentObj
 */
sciPointObj *
ConstructFec (sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pnoeud, 
	      double *pfun, int Nnode, int Ntr, double *zminmax, integer *colminmax)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciFec *pfec = (sciFec *) NULL;
  int i = 0;
  
  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_FEC);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciFec)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pFEC_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pFEC_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pFEC_FEATURE (pobj)->callback = (char *)NULL;
      pFEC_FEATURE (pobj)->callbacklen = 0;
      pFEC_FEATURE (pobj)->callbackevent = 100;

      pFEC_FEATURE (pobj)->isselected = TRUE; 
      pFEC_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj));
     
   
      pfec = pFEC_FEATURE (pobj);
      
      if ((pfec->pvecx = MALLOC (Nnode * sizeof (double))) == NULL)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((pfec->pvecy = MALLOC (Nnode * sizeof (double))) == NULL)
	{
	  FREE(pFEC_FEATURE (pobj)->pvecx);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((pfec->pnoeud = MALLOC ((5*Ntr) * sizeof (double))) == NULL)
	{
	  FREE(pFEC_FEATURE (pobj)->pvecx);
	  FREE(pFEC_FEATURE (pobj)->pvecy);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((pfec->pfun = MALLOC (Nnode * sizeof (double))) == NULL)
	{
	  FREE(pFEC_FEATURE (pobj)->pvecx);
	  FREE(pFEC_FEATURE (pobj)->pvecy); 
	  FREE(pFEC_FEATURE (pobj)->pnoeud);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((pfec->zminmax = MALLOC (2 * sizeof (double))) == NULL)
	{
	  FREE(pFEC_FEATURE (pobj)->pvecx);
	  FREE(pFEC_FEATURE (pobj)->pvecy);
	  FREE(pFEC_FEATURE (pobj)->pnoeud); 
	  FREE(pFEC_FEATURE (pobj)->pfun);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	} 
      if ((pfec->colminmax = MALLOC (2 * sizeof (integer))) == NULL)
	{
	  FREE(pFEC_FEATURE (pobj)->pvecx);
	  FREE(pFEC_FEATURE (pobj)->pvecy);
	  FREE(pFEC_FEATURE (pobj)->pnoeud); 
	  FREE(pFEC_FEATURE (pobj)->pfun); 
	  FREE(pFEC_FEATURE (pobj)->zminmax);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      for (i = 0; i < Nnode; i++)
	{
	  pfec->pvecx[i] = pvecx[i];
	  pfec->pvecy[i] = pvecy[i];
	  pfec->pfun[i] = pfun[i];
	}
      for (i = 0; i < (5*Ntr); i++)
	{
	  pfec->pnoeud[i] = pnoeud[i];
	}
      for (i = 0; i < 2; i++)
	{
	  pfec->zminmax[i] = zminmax[i];
	  pfec->colminmax[i] = colminmax[i];
	}
	   
      pfec->Nnode = Nnode;	      
      pfec->Ntr = Ntr;		
      if (sciInitGraphicContext (pobj) == -1)
	{
	  FREE(pFEC_FEATURE (pobj)->pvecx);
	  FREE(pFEC_FEATURE (pobj)->pvecy); 
	  FREE(pFEC_FEATURE (pobj)->pnoeud);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pFEC_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}
/**DestroyGrayplot
 * @memo This function destroies Grayplot and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 * @author Djalel ABDEMOUCHE
 * @version 0.1
 * @see 
 */
int
DestroyFec (sciPointObj * pthis)
{
  FREE (pFEC_FEATURE (pthis)->pvecx);
  FREE (pFEC_FEATURE (pthis)->pvecy);
  FREE (pFEC_FEATURE (pthis)->pnoeud); 
  FREE (pFEC_FEATURE (pthis)->pfun); 
  FREE (pFEC_FEATURE (pthis)->zminmax);  
  FREE (pFEC_FEATURE (pthis)->colminmax);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}

/**ConstructSegs
 * @memo This function creates Segments
 * @author Djalel.ABDEMOUCHE
 * @version 0.1
 * @see sciSetCurrentObj
 */
sciPointObj *
ConstructSegs (sciPointObj * pparentsubwin, integer type,double *vx, double *vy, 
               integer Nbr1,integer Nbr2, double *vfx, double *vfy, integer flag,
              integer *style, double arsize, integer colored, double arfact) 
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciSegs *psegs = (sciSegs *) NULL;
  integer i;

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_SEGS);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciSegs)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pSEGS_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pSEGS_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pSEGS_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pSEGS_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pSEGS_FEATURE (pobj)->callback = (char *)NULL;
      pSEGS_FEATURE (pobj)->callbacklen = 0;
      pSEGS_FEATURE (pobj)->callbackevent = 100;
       
      pSEGS_FEATURE (pobj)->isselected = TRUE;  
      pSEGS_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj)); 
      pSEGS_FEATURE (pobj)->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); 
     
   
      psegs = pSEGS_FEATURE (pobj); 
      psegs->ptype = type;
          
      if ((psegs->vx = MALLOC (Nbr1 * sizeof (double))) == NULL)
	{ 
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pSEGS_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((psegs->vy = MALLOC (Nbr2 * sizeof (double))) == NULL)
	{ 
	  FREE(pSEGS_FEATURE (pobj)->vx);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pSEGS_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}  
       for (i = 0; i < Nbr1; i++)
	{
	  psegs->vx[i] = vx[i];
	} 
       for (i = 0; i < Nbr2; i++)
	{
	  psegs->vy[i] = vy[i];
	}
      psegs->ptype = type;
      if (type == 0)
      {   
	psegs->arrowsize = arsize;     
	if ((psegs->pstyle = MALLOC (Nbr1 * sizeof (integer))) == NULL)
	  {
	    FREE(pSEGS_FEATURE (pobj)->vx); 
	    FREE(pSEGS_FEATURE (pobj)->vy); 
	    sciDelThisToItsParent (pobj, sciGetParent (pobj));
	    sciDelHandle (pobj);
	    FREE(pSEGS_FEATURE(pobj));
	    FREE(pobj);
	    return (sciPointObj *) NULL;
	  }
	if (flag == 1)
	  {
	    for (i = 0; i < Nbr1; i++)
	      psegs->pstyle[i] = style[i];
	  }
	else {
	  for (i = 0; i < Nbr1; i++)
	    psegs->pstyle[i] =  style[0];
	}
	
	psegs->iflag = flag; 
        psegs->Nbr1 = Nbr1;
      }	
      else
	{ 
        psegs->Nbr1 = Nbr1;   
        psegs->Nbr2 = Nbr2;	 
        psegs->pcolored = colored;
        psegs->parfact = arfact;
        if ((psegs->vfx = MALLOC ((Nbr1*Nbr2) * sizeof (double))) == NULL)
	      {
		FREE(pSEGS_FEATURE (pobj)->vx); 
		FREE(pSEGS_FEATURE (pobj)->vy); 
		sciDelThisToItsParent (pobj, sciGetParent (pobj));
		sciDelHandle (pobj);
		FREE(pSEGS_FEATURE(pobj));
		FREE(pobj);
		return (sciPointObj *) NULL;
	      }
	if ((psegs->vfy = MALLOC ((Nbr1*Nbr2) * sizeof (double))) == NULL)
	      {
		FREE(pSEGS_FEATURE (pobj)->vx); 
		FREE(pSEGS_FEATURE (pobj)->vy); 
		sciDelThisToItsParent (pobj, sciGetParent (pobj));
		sciDelHandle (pobj);
		FREE(pSEGS_FEATURE(pobj));
		FREE(pobj);
		return (sciPointObj *) NULL;
	      }  
	for (i = 0; i < (Nbr1*Nbr2); i++)
	{
	  psegs->vfx[i] = vfx[i];
	  psegs->vfy[i] = vfy[i];
	}
	}	
      if (sciInitGraphicContext (pobj) == -1)
	{
	  FREE(pSEGS_FEATURE (pobj)->vx);
	  FREE(pSEGS_FEATURE (pobj)->vy);     
          if (type ==0)
            {
	    FREE(pSEGS_FEATURE (pobj)->pstyle);
            }
          else
	    {
	      FREE(pSEGS_FEATURE (pobj)->vfx);  
               FREE(pSEGS_FEATURE (pobj)->vfy); 
            }         
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pSEGS_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}
/**DestroySegments
 * @memo This function destroies Grayplot and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 * @author Djalel ABDEMOUCHE
 * @version 0.1
 * @see 
 */
int
DestroySegs (sciPointObj * pthis)
{  
  
  FREE (pSEGS_FEATURE (pthis)->vx);
  FREE (pSEGS_FEATURE (pthis)->vy);  
  if (pSEGS_FEATURE (pthis)->ptype ==0) 
   {
  FREE(pSEGS_FEATURE (pthis)->pstyle);
   } 
  else 
    {
      FREE(pSEGS_FEATURE (pthis)->vfx);  
      FREE(pSEGS_FEATURE (pthis)->vfy); 
    } 
  sciDelThisToItsParent (pthis, sciGetParent (pthis)); 
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}


/**ConstructPatch
 * This function creates
 */
sciPointObj *
ConstructPatch (sciPointObj * pparentsubwin, double *pvecx, double *pvecy,
		int n)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciPolyline *ppoly = (sciPolyline *) NULL;
  double xmax, ymax;

  
  int i = 0;
  
  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_PATCH);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciPatch)))) == NULL)
	return (sciPointObj *) NULL;
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pPATCH_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pPATCH_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pPATCH_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pPATCH_FEATURE (pobj)->callback = (char *)NULL;
      pPATCH_FEATURE (pobj)->callbacklen = 0;
      pPATCH_FEATURE (pobj)->callbackevent = 100;


      pPATCH_FEATURE (pobj)->isselected = TRUE;    
      pPATCH_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj)); 
      pPATCH_FEATURE (pobj)->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); 
      ppoly = (sciPolyline *) pPATCH_FEATURE (pobj);
      if ((ppoly->pvector = MALLOC (n * sizeof (POINT2D))) == NULL)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pPATCH_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /* pour le moment je garde les vecteurs separes, et non en POINT2D */
      if ((ppoly->pvx = MALLOC (n * sizeof (double))) == NULL)
	{
	  FREE(pPATCH_FEATURE (pobj)->pvector);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pPATCH_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((ppoly->pvy = MALLOC (n * sizeof (double))) == NULL)
	{
	  FREE(pPATCH_FEATURE (pobj)->pvx);
	  FREE(pPATCH_FEATURE (pobj)->pvector);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pPATCH_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      ppoly->xmin   = pvecx[0];
      ppoly->ymin   = pvecy[0];
      xmax          = 0;
      ymax          = 0;
      ppoly->width  = 0;
      ppoly->height = 0;


      for (i = 0; i < n; i++)
	{
	  ppoly->pvector[i].x = pvecx[i];
	  ppoly->pvector[i].y = pvecy[i];
	  ppoly->pvx[i]       = pvecx[i];
	  ppoly->pvy[i]       = pvecy[i];

	  ppoly->xmin   = Min(ppoly->pvx[i], ppoly->xmin);
	  xmax          = Max(ppoly->pvx[i] ,xmax);

	  ppoly->ymin   = Min(ppoly->pvy[i], ppoly->ymin);
	  ymax          = Max(ppoly->pvy[i], ymax);
	}
      ppoly->width  = fabs(xmax - ppoly->xmin);
      ppoly->height = fabs(ymax - ppoly->ymin);

      ppoly->n1 = n;		/* memorisation du nombre de points */
      ppoly->closed = 1;
      if (sciInitGraphicContext (pobj) == -1)
	{
	  FREE(pPATCH_FEATURE (pobj)->pvy);
	  FREE(pPATCH_FEATURE (pobj)->pvx);
	  FREE(pPATCH_FEATURE (pobj)->pvector);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pPATCH_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}


/**ClonePatch
 * @memo This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
sciPointObj *
ClonePatch (sciPointObj * pthis)
{
  sciPointObj * pobj, *subwinparent;
  subwinparent = pthis;

  while ((sciGetEntityType(subwinparent = sciGetParent(subwinparent)) != SCI_SUBWIN)
	 && (sciGetEntityType(subwinparent) != -1));
  if (sciGetEntityType(subwinparent) == -1)
    return (sciPointObj *)NULL;
  if (!(pobj = ConstructPatch (subwinparent, pPATCH_FEATURE(pthis)->pvx, 
			       pPATCH_FEATURE(pthis)->pvy, pPATCH_FEATURE(pthis)->n)))
    return (sciPointObj *)NULL;
  if (sciSetBackground(pobj, sciGetBackground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetForeground(pobj, sciGetForeground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineStyle(pobj, sciGetLineStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetFillStyle(pobj, sciGetFillStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineWidth(pobj, sciGetLineWidth (pthis)) == -1)
    return (sciPointObj *)NULL;

  return (sciPointObj *)pobj;
}


/**DestroyPatch
 * @memo This function destroies Patch and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyPatch (sciPointObj * pthis)
{
  FREE (pPATCH_FEATURE (pthis)->pvector);
  FREE (pPATCH_FEATURE (pthis)->pvx);
  FREE (pPATCH_FEATURE (pthis)->pvy);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}



/**ConstructArc
 * @memo This function creates an Arc structure
 */
sciPointObj *
ConstructArc (sciPointObj * pparentsubwin, double x, double y,
	      double height, double width, double alphabegin, double alphaend, int color, int fill)
{
  sciPointObj *pobj = (sciPointObj *) NULL;

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_ARC);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciArc)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pARC_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pARC_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pARC_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pARC_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pARC_FEATURE (pobj)->callback = (char *)NULL;
      pARC_FEATURE (pobj)->callbacklen = 0;
      pARC_FEATURE (pobj)->callbackevent = 100;
    
      pARC_FEATURE (pobj)->x = x;
      pARC_FEATURE (pobj)->y = y;
      pARC_FEATURE (pobj)->height = height;
      pARC_FEATURE (pobj)->width = width;
      pARC_FEATURE (pobj)->alphabegin = alphabegin;
      pARC_FEATURE (pobj)->alphaend = alphaend;
      pARC_FEATURE (pobj)->isselected = TRUE; 
      pARC_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj)); 
      pARC_FEATURE (pobj)->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); 
      pARC_FEATURE (pobj)->fill = fill;

      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pARC_FEATURE (pobj));
	  FREE(pobj);
	  sciprint("pas de context");
	  return (sciPointObj *) NULL;
	}
      /** 07/03/2002 **/ 
      if (color!= -1 )
	sciSetForeground (pobj,color);

      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}


/**CloneArc
 * @memo This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
sciPointObj *
CloneArc (sciPointObj * pthis)
{
  sciPointObj * pobj, *subwinparent;
  subwinparent = pthis;

  while ((sciGetEntityType(subwinparent = sciGetParent(subwinparent)) != SCI_SUBWIN)
	 && (sciGetEntityType(subwinparent) != -1));
  if (sciGetEntityType(subwinparent) == -1)
    return (sciPointObj *)NULL;
  if (!(pobj = ConstructArc (subwinparent, pARC_FEATURE(pthis)->x, 
			     pARC_FEATURE(pthis)->y, pARC_FEATURE(pthis)->height,pARC_FEATURE(pthis)->width,
			     pARC_FEATURE(pthis)->alphabegin, pARC_FEATURE(pthis)->alphaend,-1,0)))
    return (sciPointObj *)NULL;
  if (sciSetBackground(pobj, sciGetBackground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetForeground(pobj, sciGetForeground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineStyle(pobj, sciGetLineStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetFillStyle(pobj, sciGetFillStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineWidth(pobj, sciGetLineWidth (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetFillFlag(pobj, sciGetFillFlag (pthis)) == -1)
    return (sciPointObj *)NULL;

  return (sciPointObj *)pobj;
}


/**DestroyArc
 * @memo This function destroies Pacthc and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyArc (sciPointObj * pthis)
{
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE(pARC_FEATURE (pthis)->callback);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}



/**ConstructRectangle
 * @memo This function creates Rectangle structure and only this to destroy all sons use DelGraphicsSon
 */
sciPointObj *
ConstructRectangle (sciPointObj * pparentsubwin, double x, double y,
		    double height, double width, double horzcurvature,
		    double vertcurvature, int fillflag, int fillcolor, int str)
{
  sciPointObj *pobj = (sciPointObj *) NULL; 
 
  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_RECTANGLE);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciRectangle)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pRECTANGLE_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pRECTANGLE_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pRECTANGLE_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pRECTANGLE_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pRECTANGLE_FEATURE (pobj)->callback = (char *)NULL;
      pRECTANGLE_FEATURE (pobj)->callbacklen = 0;
      pRECTANGLE_FEATURE (pobj)->callbackevent = 100;


      pRECTANGLE_FEATURE (pobj)->x = x;
      pRECTANGLE_FEATURE (pobj)->y = y;
      pRECTANGLE_FEATURE (pobj)->height = height;
      pRECTANGLE_FEATURE (pobj)->width = width;
      /** 15/02/2002 **/
      pRECTANGLE_FEATURE (pobj)->fillflag = fillflag; 
      pRECTANGLE_FEATURE (pobj)->fillcolor = fillcolor; 
      /** 06/02/2002 **/
      pRECTANGLE_FEATURE (pobj)->str = str;
      pRECTANGLE_FEATURE (pobj)->strheight = 0;
      pRECTANGLE_FEATURE (pobj)->strwidth = 0;
      /**************/
      pRECTANGLE_FEATURE (pobj)->horzcurvature = horzcurvature;
      pRECTANGLE_FEATURE (pobj)->vertcurvature = vertcurvature;
      pRECTANGLE_FEATURE (pobj)->isselected = TRUE;
      pRECTANGLE_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj)); 
      pRECTANGLE_FEATURE (pobj)->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); 
      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pRECTANGLE_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (pRECTANGLE_FEATURE (pobj)->fillcolor < 0)
	sciSetForeground (pobj,-(pRECTANGLE_FEATURE (pobj)->fillcolor));
      else	
	if (pRECTANGLE_FEATURE (pobj)->fillcolor > 0)
	  sciSetForeground (pobj,pRECTANGLE_FEATURE (pobj)->fillcolor);
	else 
	  sciSetForeground (pobj,sciGetForeground (sciGetParent (pobj)) );
      
      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}



/**CloneRectangle
 * @memo This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
sciPointObj *
CloneRectangle (sciPointObj * pthis)
{
  sciPointObj * pobj, *subwinparent;
  subwinparent = pthis;

  while ((sciGetEntityType(subwinparent = sciGetParent(subwinparent)) != SCI_SUBWIN)
	 && (sciGetEntityType(subwinparent) != -1));
  if (sciGetEntityType(subwinparent) == -1)
    return (sciPointObj *)NULL;
  if (!(pobj = ConstructRectangle (subwinparent, pRECTANGLE_FEATURE(pthis)->x, 
				   pRECTANGLE_FEATURE(pthis)->y, pRECTANGLE_FEATURE(pthis)->height,pRECTANGLE_FEATURE(pthis)->width, 
				   pRECTANGLE_FEATURE(pthis)->horzcurvature, pRECTANGLE_FEATURE(pthis)->vertcurvature,0,0,0)))
    return (sciPointObj *)NULL;
  if (sciSetBackground(pobj, sciGetBackground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetForeground(pobj, sciGetForeground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineStyle(pobj, sciGetLineStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetFillStyle(pobj, sciGetFillStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineWidth(pobj, sciGetLineWidth (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetFillFlag(pobj, sciGetFillFlag (pthis)) == -1)
    return (sciPointObj *)NULL;

  return (sciPointObj *)pobj;
}


/**DestroyRectangle
 * @memo This function destroies Rectangle and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyRectangle (sciPointObj * pthis)
{
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}




/**ConstructSurface
 * @memo This function creates Surface Structure
 */
sciPointObj *
ConstructSurface (sciPointObj * pparentsubwin, sciTypeOf3D typeof3d, 
		  double * pvecx, double * pvecy, double * pvecz,integer *zcol, 
		  integer izcol, integer dimzx, integer dimzy, double theta, double alpha, 
		  char *legend, integer *flag, double *ebox,integer flagcolor)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  int i=0, j=0;
  int nx,ny,nz,nc,izc=izcol;

  if (typeof3d == SCI_PLOT3D) {
    nx=dimzx;
    ny=dimzy;
    nz=dimzx*dimzy;
    if (flagcolor == 2)
      nc=nz; /* one color per facet */
    else if (flagcolor == 3)
      nc=nz*4; /*one color per edge */
    else 
      nc=0;
  }
  else if (typeof3d == SCI_PARAM3D1) {
    nx=dimzx*dimzy;
    ny=dimzx*dimzy;
    nz=dimzx*dimzy;
    nc=dimzy;
    izc=1;
  }
  else {
    nx=dimzx*dimzy;
    ny=dimzx*dimzy;
    nz=dimzx*dimzy;
    if (flagcolor == 2)
      nc=dimzy; /* one color per facet */
    else if (flagcolor == 3)
      nc=nz; /*one color per edge */
    else 
      nc=0;
  }

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_SURFACE);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciSurface)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pSURFACE_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pSURFACE_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pSURFACE_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pSURFACE_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pSURFACE_FEATURE (pobj)->callback = (char *)NULL;
      pSURFACE_FEATURE (pobj)->callbacklen = 0;
      pSURFACE_FEATURE (pobj)->callbackevent = 100; 
      pSURFACE_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj)); 
 

      if (((pSURFACE_FEATURE (pobj)->pvecx = MALLOC ((nx * sizeof (double)))) == NULL))
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pSURFACE_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      else
	{
	  for (i = 0;i < nx; i++)
	    pSURFACE_FEATURE (pobj)->pvecx[i] = pvecx[i];
	}
      if (((pSURFACE_FEATURE (pobj)->pvecy = MALLOC ((ny * sizeof (double)))) == NULL))
	{
	  FREE(pSURFACE_FEATURE (pobj)->pvecx);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pSURFACE_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      else
	{
	  for (j = 0;j < ny; j++)
	    pSURFACE_FEATURE (pobj)->pvecy[j] = pvecy[j];
	}

      if (((pSURFACE_FEATURE (pobj)->pvecz = MALLOC ((nz * sizeof (double)))) == NULL))
	{
	  FREE(pSURFACE_FEATURE (pobj)->pvecy);
	  FREE(pSURFACE_FEATURE (pobj)->pvecx);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pSURFACE_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      else
	{
	  for (j = 0;j < nz; j++)
	    pSURFACE_FEATURE (pobj)->pvecz[j] = pvecz[j];
	}

      if (izc !=0&&nc>0 ) {
	if (((pSURFACE_FEATURE (pobj)->zcol = MALLOC ((nc * sizeof (integer)))) == NULL))
	  {
	    FREE(pSURFACE_FEATURE (pobj)->pvecy);
	    FREE(pSURFACE_FEATURE (pobj)->pvecx); 
	    FREE(pSURFACE_FEATURE (pobj)->pvecz);
	    sciDelThisToItsParent (pobj, sciGetParent (pobj));
	    sciDelHandle (pobj);
	    FREE(pSURFACE_FEATURE (pobj));
	    FREE(pobj);
	    return (sciPointObj *) NULL;
	  }
	else
	  {
	    if (izcol !=0)
	      for (j = 0;j < nc; j++)  
		pSURFACE_FEATURE (pobj)->zcol[j]= zcol[j];
	    else /*param3d case */
	      for (j = 0;j < nc; j++)  
		pSURFACE_FEATURE (pobj)->zcol[j]= 1;
	  }
      }

      pSURFACE_FEATURE (pobj)->dimzx = dimzx;
      pSURFACE_FEATURE (pobj)->dimzy = dimzy; 
      pSURFACE_FEATURE (pobj)->izcol = izc;
      pSURFACE_FEATURE (pobj)->pproj = NULL;	/* Les projections ne sont pas encore calculees */
      pSURFACE_FEATURE (pobj)->isselected = TRUE;

      pSURFACE_FEATURE (pobj)->flag[0] = flag[0];
      pSURFACE_FEATURE (pobj)->flag[1] = flag[1];
      pSURFACE_FEATURE (pobj)->flag[2] = flag[2];

      pSURFACE_FEATURE (pobj)->alpha = alpha;
      pSURFACE_FEATURE (pobj)->theta = theta;
      if ((pSURFACE_FEATURE (pobj)->legend = calloc (strlen(legend), sizeof (char))) == NULL)
	{
	  sciprint("No more place to allocates text string, try a shorter string");
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);	  
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      strncpy (pSURFACE_FEATURE (pobj)->legend, legend, strlen(legend));

   
      pSURFACE_FEATURE (pobj)->ebox[0] = ebox[0];
      pSURFACE_FEATURE (pobj)->ebox[1] = ebox[1];
      pSURFACE_FEATURE (pobj)->ebox[2] = ebox[2];
      pSURFACE_FEATURE (pobj)->ebox[3] = ebox[3];
      pSURFACE_FEATURE (pobj)->ebox[4] = ebox[4];
      pSURFACE_FEATURE (pobj)->ebox[5] = ebox[5];
      pSURFACE_FEATURE (pobj)->flagcolor =flagcolor;
      pSURFACE_FEATURE (pobj)->typeof3d = typeof3d;

      if (sciInitGraphicContext (pobj) == -1)
	{
	  FREE(pSURFACE_FEATURE (pobj)->pvecz);
	  FREE(pSURFACE_FEATURE (pobj)->pvecy);
	  FREE(pSURFACE_FEATURE (pobj)->pvecx);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pSURFACE_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}


/**DestroySurface
 * @memo This function destroies surface and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroySurface (sciPointObj * pthis)
{
  FREE(pSURFACE_FEATURE (pthis)->pvecz);
  FREE(pSURFACE_FEATURE (pthis)->pvecy);
  FREE(pSURFACE_FEATURE (pthis)->pvecx);
  if (pSURFACE_FEATURE (pthis)->izcol != 0 ) 
    FREE(pSURFACE_FEATURE (pthis)->zcol);

  FREE(pSURFACE_FEATURE (pthis)->legend);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (pSURFACE_FEATURE (pthis)->pproj);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent, le vecteur 3d et 2d */
  return 0;
}

/**ConstructAxis
 * @memo This function creates Axis structure
 */
sciPointObj *
ConstructAxis (sciPointObj * pparentsubwin, char *strflag, int style, double minx,
	       double miny, double minz, double maxx, double maxy,
	       double maxz)
{
  sciPointObj *pobj = (sciPointObj *) NULL;

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_AXIS);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciAxis)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pAXIS_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pAXIS_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pAXIS_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pAXIS_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pAXIS_FEATURE (pobj)->callback = (char *)NULL;
      pAXIS_FEATURE (pobj)->callbacklen = 0;
      pAXIS_FEATURE (pobj)->callbackevent = 100;   
      pAXIS_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj)); 
      
      pAXIS_FEATURE (pobj)->minx = minx;
      pAXIS_FEATURE (pobj)->miny = miny;
      pAXIS_FEATURE (pobj)->minz = minz;
      pAXIS_FEATURE (pobj)->maxx = maxx;
      pAXIS_FEATURE (pobj)->maxy = maxy;
      pAXIS_FEATURE (pobj)->maxz = maxz;
      pAXIS_FEATURE (pobj)->styledimension = (style != 3) ? 2 : 3;
      pAXIS_FEATURE (pobj)->stylecrossing = 0;
      pAXIS_FEATURE (pobj)->plabelx = (char *) NULL;
      pAXIS_FEATURE (pobj)->plabely = (char *) NULL;
      pAXIS_FEATURE (pobj)->plabelz = (char *) NULL;
      pAXIS_FEATURE (pobj)->manualscale = FALSE;
      pAXIS_FEATURE (pobj)->grid = -1;
      pAXIS_FEATURE(pobj)->aaint[0] = 2;
      pAXIS_FEATURE(pobj)->aaint[1] = 10;
      pAXIS_FEATURE(pobj)->aaint[2] = 2;
      pAXIS_FEATURE(pobj)->aaint[3] = 10;


      pAXIS_FEATURE (pobj)->strflaglen = strlen (strflag);

      strncpy(pAXIS_FEATURE (pobj)->strflag, strflag, pAXIS_FEATURE (pobj)->strflaglen);



      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pAXIS_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}  

      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}



/**CloneAxis
 * @memo This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
sciPointObj *
CloneAxis (sciPointObj * pthis)
{
  sciPointObj * pobj, *subwinparent;
  subwinparent = pthis;

  while ((sciGetEntityType(subwinparent = sciGetParent(subwinparent)) != SCI_SUBWIN)
	 && (sciGetEntityType(subwinparent) != -1));
  if (sciGetEntityType(subwinparent) == -1)
    return (sciPointObj *)NULL;
  if (!(pobj = ConstructAxis (subwinparent,pAXIS_FEATURE(pthis)->strflag, 
			      pAXIS_FEATURE(pthis)->styledimension, 
			      pAXIS_FEATURE(pthis)->minx, pAXIS_FEATURE(pthis)->miny, pAXIS_FEATURE(pthis)->minz,
			      pAXIS_FEATURE(pthis)->maxx, pAXIS_FEATURE(pthis)->maxy, pAXIS_FEATURE(pthis)->maxz)))
    return (sciPointObj *)NULL;
  if (sciSetBackground(pobj, sciGetBackground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetForeground(pobj, sciGetForeground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineStyle(pobj, sciGetLineStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetFillStyle(pobj, sciGetFillStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineWidth(pobj, sciGetLineWidth (pthis)) == -1)
    return (sciPointObj *)NULL;

  return (sciPointObj *)pobj;
}


/**DestroyAxis
 * @memo This function destroies axis and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 */
int
DestroyAxis (sciPointObj * pthis)
{
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (pAXIS_FEATURE (pthis)->plabelx);
  FREE (pAXIS_FEATURE (pthis)->plabely);
  FREE (pAXIS_FEATURE (pthis)->plabelz);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent, le vecteur 3d et 2d */
  return 0;
}



/**ConstructAxes
 * @memo This function creates Axes structure
 * @author Djalel ABDEMOUCHE
 * @see sciSetCurrentObj
 *
 */
sciPointObj *
ConstructAxes (sciPointObj * pparentsubwin, char dir, char tics, double *vx, 
	       int nx, double *vy, int ny,char **str, int subint, char *format,
	       int fontsize, int textcolor, int ticscolor, char logscale, int seg)  
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciAxes *paxes = (sciAxes *) NULL;
  int i;

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_AXES);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciAxes)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pAXES_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pAXES_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pAXES_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pAXES_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pAXES_FEATURE (pobj)->callback = (char *)NULL;
      pAXES_FEATURE (pobj)->callbacklen = 0;
      pAXES_FEATURE (pobj)->callbackevent = 100;
      pAXES_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj));
      pAXES_FEATURE (pobj)->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); 
 
     
      pAXES_FEATURE (pobj)->dir =dir;
      pAXES_FEATURE (pobj)->tics =tics;
  
      paxes = pAXES_FEATURE (pobj);
      if ((paxes->vector = MALLOC (Max(nx,ny) * sizeof (POINT2D))) == NULL)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pPOLYLINE_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /* pour le moment je garde les vecteurs separes, et non en POINT2D */
      if ((paxes->vx = MALLOC (nx * sizeof (double))) == NULL)
	{
	  FREE(pAXES_FEATURE (pobj)->vector);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pAXES_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if ((paxes->vy = MALLOC (ny * sizeof (double))) == NULL)
	{
	  FREE(pAXES_FEATURE (pobj)->vector);
	  FREE(pAXES_FEATURE (pobj)->vx);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pAXES_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      for (i = 0; i < nx; i++)
	{
	  paxes->vx[i]       = vx[i];	 
	}
      for (i = 0; i < ny; i++)
	{
	  paxes->vy[i]       = vy[i];
	}
     
     
      
      pAXES_FEATURE (pobj)->nx =nx; 
      
     
      pAXES_FEATURE (pobj)->ny =ny;
     
      if ((pAXES_FEATURE(pobj)->str= malloc (Max(nx,ny) * sizeof (char*))) == NULL)
	return (sciPointObj *) NULL;

      pAXES_FEATURE(pobj)->str = str;
      
      pAXES_FEATURE (pobj)->subint = subint;
      pAXES_FEATURE (pobj)->fontsize =fontsize; 
      pAXES_FEATURE (pobj)->textcolor =textcolor;
      pAXES_FEATURE (pobj)->ticscolor =ticscolor;
      pAXES_FEATURE (pobj)->seg =seg;    
      pAXES_FEATURE (pobj)->format =format;
      pAXES_FEATURE (pobj)->logscale=logscale; 

     

      if (sciInitGraphicContext (pobj) == -1)
	{
    	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
    	  sciDelHandle (pobj);
    	  FREE(pAXES_FEATURE (pobj));
    	  FREE(pobj);
    	  return (sciPointObj *) NULL;
	}


      return pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}



/**CloneAxes
 * @memo This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */

/*
sciPointObj *
CloneAxes (sciPointObj * pthis)
{
  sciPointObj * pobj, *subwinparent;
  subwinparent = pthis;

  while ((sciGetEntityType(subwinparent = sciGetParent(subwinparent)) != SCI_SUBWIN)
	 && (sciGetEntityType(subwinparent) != -1));
  if (sciGetEntityType(subwinparent) == -1)
    return (sciPointObj *)NULL;
    if (!(pobj = ConstructAxes (subwinparent,pAXES_FEATURE(pthis)->strflag, pAXES_FEATURE(pthis)->styledimension,      			      pAXES_FEATURE(pthis)->minx, pAXES_FEATURE(pthis)->miny, pAXES_FEATURE(pthis)->minz,  
			      pAXES_FEATURE(pthis)->maxx, pAXES_FEATURE(pthis)->maxy, pAXES_FEATURE(pthis)->maxz)))
    return (sciPointObj *)NULL;
  if (sciSetBackground(pobj, sciGetBackground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetForeground(pobj, sciGetForeground (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineStyle(pobj, sciGetLineStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetFillStyle(pobj, sciGetFillStyle (pthis)) == -1)
    return (sciPointObj *)NULL;
  if (sciSetLineWidth(pobj, sciGetLineWidth (pthis)) == -1)
    return (sciPointObj *)NULL;

  return (sciPointObj *)pobj;
}
*/

/**DestroyAxes
 * @memo This function destroies axes and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 */
int
DestroyAxes (sciPointObj * pthis)
{
  int i;
  char **str;
  
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (pAXES_FEATURE(pthis)->vx);
  FREE (pAXES_FEATURE(pthis)->vy);
  str=pAXES_FEATURE(pthis)->str;
  for (i=Max(pAXES_FEATURE(pthis)->nx,pAXES_FEATURE(pthis)->ny)-1;i<0;i--) 
    FREE (pAXES_FEATURE(pthis)->str); 
  FREE (pAXES_FEATURE(pthis)->str);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent, le vecteur 3d et 2d */
  return 0;
}



/**ConstructMenuContext
 * @memo This function creates MenuContext structure only for tests on Widows system during developpement.
 * @memo ths function may be not incuded in the final version
 * pour obtenir un menu de contexe vide pour chacune des entites. L'utilisateur specifie avec un addlabel les label et callback necessaire ensuite.
 * @param  sciPointObj * pparentfigure: the parent entity
 */
sciPointObj *
ConstructMenuContext (sciPointObj * pparentfigure)
{
  sciPointObj *pobj = (sciPointObj *) NULL;

  if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_MENUCONTEXT);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciMenuContext)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pMENUCONTEXT_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentfigure);*/
      if (!(sciAddThisToItsParent (pobj, pparentfigure)))
	{
	  sciDelHandle (pobj);
	  FREE(pMENUCONTEXT_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      /*if (sciAddLabelMenu (pobj, plabel, n) == -1)
	return (sciPointObj *) NULL;*/
      pMENUCONTEXT_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pMENUCONTEXT_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pMENUCONTEXT_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciInitFontContext (pobj);

      /* windows dependant*/
      /*     if ((pMENUCONTEXT_FEATURE(pobj)->hPopMenu = CreatePopupMenu ()) == NULL) 
      {
      	  sciprint ("Scilab can't have more menu \n");
      	  FREE ((sciGetFontContext(pobj))->pfontname);
      	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
      	  sciDelHandle (pobj);
      	  FREE(pMENUCONTEXT_FEATURE (pobj));
      	  FREE(pobj);
      	  return (sciPointObj *) NULL;
       }
       AppendMenu (pMENUCONTEXT_FEATURE(pobj)->hPopMenu, MF_SEPARATOR , 0, NULL);*/
      /*AppendMenu (sciGethPopMenu (pobj), MF_STRING, 4401, plabel);*/

      return (sciPointObj *) pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}


/**DestroyMenuContext
 * @memo This function destroies menu context and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroyMenuContext (sciPointObj * pthis)
{
  sciDelLabelsMenu (pthis);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent, le vecteur 3d et 2d */
  return 0;
}



/**ConstructMenu
 * @memo This function creates Menu structure
 */
sciPointObj *
ConstructMenu (sciPointObj * pparentfigure, char plabel[], int n)
{
  sciPointObj *pobj = (sciPointObj *) NULL;

  if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_MENU);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciMenu)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pMENU_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentfigure);*/
      if (!(sciAddThisToItsParent (pobj, pparentfigure)))
	{
	  sciDelHandle (pobj);
	  FREE(pMENU_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pMENU_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pMENU_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      if (sciAddLabelMenu (pobj, plabel, n) == -1)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pMENU_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      if (sciInitGraphicContext (pobj) == -1)
	{
	  sciDelLabelsMenu (pobj);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pMENU_FEATURE (pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciInitFontContext (pobj);

      return (sciPointObj *) pobj;
    }
  else
    {
      sciprint ("The parent has to be a SUBWIN \n");
      return (sciPointObj *) NULL;
    }
}


/**DestroySciMenu
 * @memo This function destroies menu and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */
int
DestroySciMenu (sciPointObj * pthis)
{
  sciDelLabelsMenu (pthis);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent, le vecteur 3d et 2d */
  return 0;
}




/**sciCloneObj
 */
sciPointObj *
sciCloneObj (sciPointObj * pobj)
{
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
      return CloneText(pobj);
      break;
    case SCI_POLYLINE:
      return ClonePolyline(pobj);
      break;
    case SCI_PATCH:
      return ClonePatch(pobj);
      break;
    case SCI_ARC:
      return CloneArc(pobj);
      break;
    case SCI_RECTANGLE:
      return CloneRectangle(pobj);
      break;
    case SCI_AGREG:

    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_LEGEND:
    case SCI_TITLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXIS:  
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    default:
      sciprint ("This object cannot be cloned !\n");
      return (sciPointObj *)NULL;
      break;
    }
  return (sciPointObj *)NULL;
}


/**sciCopyObj
 */
sciPointObj *
sciCopyObj (sciPointObj * pobj, sciPointObj * psubwinparenttarget )
{
  sciPointObj *pcopyobj;

  pcopyobj = sciCloneObj ((sciPointObj *)pobj);
  sciDelThisToItsParent ((sciPointObj *)pcopyobj, (sciPointObj *)sciGetParent(pcopyobj));
  sciAddThisToItsParent ((sciPointObj *)pcopyobj, (sciPointObj *)psubwinparenttarget);
  return (sciPointObj *)pcopyobj;
}




/**************************************** END CONSTRUCTORS ********************************************/


/**sciDrawObj
 * @memo Draws Object (only the basic  graphicobject under subwindows) in its SubWindow or figure
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, -1 if not
 */
int
sciDrawObj (sciPointObj * pobj)
{
  char str[2] = "xv",locstr;
  integer n,n1,uc,verbose=0,narg,xz[10],na,arssize,sflag=0;
  integer *xm, *ym,*zm,n2 = 1, xtmp[4], ytmp[4],style[1],rect1[4];
  integer closeflag = 0;
  integer width, height;
  double anglestr,w2,h2;
  double xx[2],yy[2];   
  integer px1[2],py1[2],pn1=1,pn2=2;
  integer nn1,nn2, arsize,lstyle,iflag;
  double arsize1=5.0,arsize2=5.0,dv;
  integer angle1, angle2;
  integer x1, yy1, w1, h1, wstr,hstr,hh1;
  integer x[5], v;
  integer xold[5], vold = 0, flagx = 0;
  sciSons *psonstmp;
  integer itmp[5];		
  integer markidsizeold[2], markidsizenew[2];
  sciPointObj *psubwin, *currentsubwin;
  double locx,locy,loctit;
  char logflags[4];
#ifdef WIN32
  int flag;
#endif

  int i,j;
  /* variable pour le set_scale update_frame_bouns*/
  double subwin[4], framevalues[4];
  double mx[2], my[2];
  int mn1, mn2;

  subwin[0]    = 0;
  subwin[1]    = 0;
  subwin[2]   = 1;
  subwin[3]    = 1;
  framevalues[0] = 0;
  framevalues[1] = 0;
  framevalues[2] = 1;
  framevalues[3] = 1;

   currentsubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());

  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE: 
      x[1] = sciGetBackground (pobj);x[4] = 0;
      /** xclear will properly upgrade background if necessary **/
#ifdef WIN32
       flag=MaybeSetWinhdc();
#endif
      C2F (dr) ("xclear", "v", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0,0L, 0L);
      C2F (dr) ("xset", "background",x+1,x+1,x+4,x+4,x+4,&v,&dv,&dv,&dv,&dv,5L,4096);
#ifdef WIN32
      if ( flag == 1) ReleaseWinHdc();
#endif
      psonstmp = sciGetLastSons (pobj);
      while (psonstmp != (sciSons *) NULL)
	{
	  if (sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
	    /* We draw only the graphics object, not the widget */
	    sciDrawObj (psonstmp->pointobj);
	  psonstmp = psonstmp->pprev;
	}
      break;
    case SCI_SUBWIN: 
      if (!sciGetVisibility(pobj)) break;
      sciSetSelectedSubWin(pobj);
      set_scale ("tttfff", pSUBWIN_FEATURE (pobj)->WRect, 
		 pSUBWIN_FEATURE (pobj)->FRect,
		 NULL, pSUBWIN_FEATURE (pobj)->logflags, NULL);      
      if (pSUBWIN_FEATURE (pobj)->isaxes)
	{
	  sciSetCurrentObj (sciGetParent(pobj));
	  /* load the object foreground and dashes color */
	  x[0] = sciGetForeground (pobj);
	  x[2] = sciGetLineWidth (pobj);
	  x[3] = sciGetLineStyle (pobj);
	  markidsizenew[0] = sciGetMarkStyle(pobj);
	  markidsizenew[1] = sciGetLineWidth (pobj);x[4] = 0;v = 0;dv = 0;

#ifdef WIN32
	  flag=MaybeSetWinhdc();
#endif
	  C2F (dr) ("xset","dashes",x,x,x+4,x+4,x+4,&v,&dv,&dv,&dv,&dv,5L,4096);
	  C2F (dr) ("xset","foreground",x,x,x+4,x+4,x+4,&v,&dv,&dv,&dv,&dv,5L,4096);
	  C2F (dr) ("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F (dr) ("xset","mark",&markidsizenew[0],&markidsizenew[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    
          /* SS 01/01/03 BEURK*/
	  Cscale.logflag[0]=pSUBWIN_FEATURE (pobj)->logflags[0];
	  Cscale.logflag[1]=pSUBWIN_FEATURE (pobj)->logflags[1];

	  axis_draw (pSUBWIN_FEATURE (pobj)->strflag);
#ifdef WIN32
	  if ( flag == 1) ReleaseWinHdc();
#endif

	}                         
      /******************/

    case SCI_AGREG: 
      if (!sciGetVisibility(pobj)) break;
      /* scan the hierarchie and call sciDrawObj */
      psonstmp = sciGetLastSons (pobj);
      while (psonstmp != (sciSons *) NULL)
	{
	  sciDrawObj (psonstmp->pointobj);
	  psonstmp = psonstmp->pprev;
	}
      break;
      /************ 30/04/2001 **************************************************/

    case SCI_LEGEND: 
      if (!sciGetVisibility(pobj)) break;
      sciSetCurrentObj (pobj);	
      C2F (dr1) ("xget", "dashes", &flagx, &xold[0], &vold, &vold, &vold,
		 &vold, &dv, &dv, &dv, &dv, 5L, 4096);
      C2F (dr1) ("xget", "foreground", &flagx, &xold[1], &vold, &vold, &vold,
		 &vold, &dv, &dv, &dv, &dv, 5L, 4096);


      itmp[0] = 0;		/* verbose */
      itmp[1] = 0;		/* thickness value*/
      itmp[2] = 1;		/* narg*/
      C2F (dr) ("xget", "thickness", &itmp[0], &itmp[1], &itmp[2], PI0, PI0,
		PI0, PD0, PD0, PD0, PD0, 0L, 0L);
      C2F (dr) ("xget", "mark", &itmp[0], markidsizeold, &itmp[3], PI0, PI0, PI0,
		PD0, PD0, PD0, PD0, 0L, 0L);

      /* load the object foreground and dashes color */
      x[0] = sciGetForeground (pobj);	/*la dash est de la meme couleur que le foreground*/
      x[4] = 0;
      v = 0;
      dv = 0;
#ifdef WIN32 
      flag=MaybeSetWinhdc();
#endif
      C2F (dr1) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
		 &dv, &dv, &dv, 5L, 4096);
      C2F (dr1) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
		 &dv, &dv, &dv, &dv, 5L, 4096);

      /*permet la mise a jour des legendes correspondantes aux entites associees */
      for (i = 0; i < pLEGEND_FEATURE (pobj)->nblegends; i++)
	{
	  if (sciGetIsMark(pLEGEND_FEATURE (pobj)->pptabofpointobj[i]))
	    pLEGEND_FEATURE (pobj)->pstyle[i] = 
	      - sciGetMarkStyle (pLEGEND_FEATURE (pobj)->pptabofpointobj[i]);
	  else
	    pLEGEND_FEATURE (pobj)->pstyle[i] = 
	      sciGetForeground(pLEGEND_FEATURE (pobj)->pptabofpointobj[i]);
	}
      sciSetCurrentObj(pobj);
      Legends(pLEGEND_FEATURE (pobj)->pstyle, &(pLEGEND_FEATURE (pobj)->nblegends), sciGetText(pobj));
       
          
      /* restore the graphic context */

      C2F (dr1) ("xset", "dashes", &xold[0], &vold, &vold, &vold, &vold, &v,
		 &dv, &dv, &dv, &dv, 5L, 6L);
      C2F (dr1) ("xset", "foreground", &xold[1], &vold, &vold, &vold, &vold,
		 &v, &dv, &dv, &dv, &dv, 5L, 10L);
#ifdef WIN32 
      if ( flag == 1) ReleaseWinHdc ();
#endif
 
      break; 
      /******************************** 22/05/2002 ***************************/    
    case SCI_FEC:  
      if (!sciGetVisibility(pobj)) break;
      
      n1=1;      
      if ((xm = MALLOC ((pFEC_FEATURE (pobj)->Nnode)*sizeof (integer))) == NULL)	return -1;
      if ((ym = MALLOC ((pFEC_FEATURE (pobj)->Nnode)*sizeof (integer))) == NULL)	return -1;
      for ( i =0 ; i < pFEC_FEATURE (pobj)->Nnode ; i++) {
	xm[i]= XScale(pFEC_FEATURE (pobj)->pvecx[i]); 
	ym[i]= YScale(pFEC_FEATURE (pobj)->pvecy[i]);} 
#ifdef WIN32
       flag=MaybeSetWinhdc();
#endif   
      newfec(xm,ym,pFEC_FEATURE (pobj)->pnoeud,pFEC_FEATURE (pobj)->pfun,
	     &pFEC_FEATURE (pobj)->Nnode,&pFEC_FEATURE (pobj)->Ntr,
	     pFEC_FEATURE (pobj)->zminmax,pFEC_FEATURE (pobj)->colminmax);
#ifdef WIN32
      if ( flag == 1) ReleaseWinHdc();
#endif

      break;      
      /******************************** 22/05/2002 ***************************/    
     case SCI_SEGS:    
       if (!sciGetVisibility(pobj)) break;
      
       sciClip(sciGetIsClipping(pobj)); 

       /* load the object foreground and dashes color */
      x[2] = sciGetLineWidth (pobj);
      x[3] = sciGetLineStyle (pobj);
      x[4] = 0;
      markidsizenew[0] =  sciGetMarkStyle(pobj);;
      markidsizenew[1] =  sciGetLineWidth (pobj);;

#ifdef WIN32 
      flag=MaybeSetWinhdc();
#endif

      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,
		&dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);    
      C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);
      C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
		PD0, PD0, 0L, 0L);
#ifdef WIN32 
      if ( flag == 1) ReleaseWinHdc ();
#endif 
   
      if (pSEGS_FEATURE (pobj)->ptype == 0)
        { 
	  n=pSEGS_FEATURE (pobj)->Nbr1; 
	  arsize = pSEGS_FEATURE (pobj)->arrowsize; 
	  if ((xm = MALLOC (n*sizeof (integer))) == NULL)	return -1;
	  if ((ym = MALLOC (n*sizeof (integer))) == NULL)	return -1;
	  for ( i =0 ; i <n ; i++) {
	    xm[i]= XScale(pSEGS_FEATURE (pobj)->vx[i]); 
	    ym[i]= YScale(pSEGS_FEATURE (pobj)->vy[i]);} 
#ifdef WIN32 
	  flag=MaybeSetWinhdc();
#endif
	  if (pSEGS_FEATURE (pobj)->arrowsize == 0)
	    C2F(dr)("xsegs","v",xm,ym,&n,pSEGS_FEATURE (pobj)->pstyle,&pSEGS_FEATURE (pobj)->iflag,
		    PI0,PD0,PD0,PD0,PD0,0L,0L);
	  else 
	    C2F(dr1)("xarrow","v",pSEGS_FEATURE (pobj)->pstyle,&pSEGS_FEATURE (pobj)->iflag
		     ,&n,PI0,PI0,PI0,pSEGS_FEATURE (pobj)->vx,pSEGS_FEATURE (pobj)->vy,&pSEGS_FEATURE (pobj)->arrowsize,PD0,0L,0L);
#ifdef WIN32 
	  if ( flag == 1) ReleaseWinHdc ();
#endif 
	}
      else
        {
#ifdef WIN32 
	  flag=MaybeSetWinhdc();
#endif
        C2F(dr)("xget","use color",&verbose, &uc, &narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
        if (uc)
	  C2F(dr)("xget","color",&verbose,xz,&narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
	else
	  C2F(dr)("xget","line style",&verbose,xz,&narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
#ifdef WIN32 
	if ( flag == 1) ReleaseWinHdc ();
#endif 
	n=2*(pSEGS_FEATURE (pobj)->Nbr1)*(pSEGS_FEATURE (pobj)->Nbr2); 
	xm = graphic_alloc(0,n,sizeof(int));
	ym = graphic_alloc(1,n,sizeof(int));
	if ( xm == 0 || ym == 0) 
	  {
	    sciprint("Running out of memory \n");
	    return -1;
	  }      
	if ( pSEGS_FEATURE (pobj)->pcolored != 0) {
	  zm = graphic_alloc(2,n/2,sizeof(int));
	  if (  zm == 0 ) 
	    {
	      sciprint("Running out of memory \n");
	      return -1;
	    }      
	}
	Champ2DRealToPixel(xm,ym,zm,&na,&arssize,&(pSEGS_FEATURE (pobj)->pcolored),
           pSEGS_FEATURE (pobj)->vx,pSEGS_FEATURE (pobj)->vy,pSEGS_FEATURE (pobj)->vfx,
           pSEGS_FEATURE (pobj)->vfy,&(pSEGS_FEATURE (pobj)->Nbr1),
           &(pSEGS_FEATURE (pobj)->Nbr2),&(pSEGS_FEATURE (pobj)->parfact));
#ifdef WIN32 
	flag=MaybeSetWinhdc();
#endif
	if ( pSEGS_FEATURE (pobj)->pcolored ==0) 
          C2F(dr)("xarrow","v",xm,ym,&na,&arssize,xz,&sflag,&dv,&dv,&dv,&dv,0L,0L); 
	else
	  C2F(dr)("xarrow","v",xm,ym,&na,&arssize,zm,(sflag=1,&sflag),&dv,&dv,&dv,&dv,0L,0L);   
#ifdef WIN32 
      if ( flag == 1) ReleaseWinHdc ();
#endif 
        }  

      sciUnClip(sciGetIsClipping(pobj));
      break;
    case SCI_GRAYPLOT:  
      if (!sciGetVisibility(pobj)) break;
      n1 = pGRAYPLOT_FEATURE (pobj)->nx;
      n2 = pGRAYPLOT_FEATURE (pobj)->ny;    
  
      switch (pGRAYPLOT_FEATURE (pobj)->type )
	{
	case 0:
	  if ((xm = MALLOC (n1*sizeof (integer))) == NULL)	return -1;
	  if ((ym = MALLOC (n2*sizeof (integer))) == NULL){
	    FREE(xm);return -1;
	  }
 
	  for ( i =0 ; i < n1 ; i++)  
	    xm[i]= XScale(pGRAYPLOT_FEATURE (pobj)->pvecx[i]); 
	  for ( i =0 ; i < n2 ; i++)  
	    ym[i]= YScale(pGRAYPLOT_FEATURE (pobj)->pvecy[i]);   
#ifdef WIN32
       flag=MaybeSetWinhdc();
#endif
	  frame_clip_on(); 
  
	  if (strncmp(pGRAYPLOT_FEATURE (pobj)->datamapping,"scaled", 6) == 0)
	    GraySquare(xm,ym,pGRAYPLOT_FEATURE (pobj)->pvecz,n1,n2); /* SS 03/01/03 */
	  else  
	    GraySquare1(xm,ym,pGRAYPLOT_FEATURE (pobj)->pvecz,n1,n2); 

	  frame_clip_off();  
	  C2F(dr)("xrect","v",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],
		  &Cscale.WIRect1[3],PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
#ifdef WIN32
      if ( flag == 1) ReleaseWinHdc();
#endif

	  FREE(xm);FREE(ym); /* SS 03/01/03 */
	  break;
	case 1:
	  if ((xm = MALLOC (n2*sizeof (integer))) == NULL) 
	    return -1;
	  if ((ym = MALLOC (n1*sizeof (integer))) == NULL){
	    FREE(xm);return -1;
	  }

          for ( j =0 ; j < n2 ; j++) xm[j]= XScale(j+0.5);
          for ( j =0 ; j < n1 ; j++) ym[j]= YScale(((n1-1)-j+0.5));
#ifdef WIN32
       flag=MaybeSetWinhdc();
#endif
          frame_clip_on(); 
          GraySquare1(xm,ym,pGRAYPLOT_FEATURE (pobj)->pvecz,n1,n2);  
          frame_clip_off();  
          C2F(dr)("xrect","v",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],
		  &Cscale.WIRect1[3],PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
#ifdef WIN32
      if ( flag == 1) ReleaseWinHdc();
#endif

	  FREE(xm);FREE(ym); /* SS 03/01/03 */
          break;
	case 2:
	  if ((xm = MALLOC (n2*sizeof (integer))) == NULL) 
	    return -1;
	  if ((ym = MALLOC (n1*sizeof (integer))) == NULL){
	    FREE(xm);return -1;
	  }

	  xx[0]=pGRAYPLOT_FEATURE (pobj)->pvecx[0];
	  xx[1]=pGRAYPLOT_FEATURE (pobj)->pvecx[2];
	  yy[0]=pGRAYPLOT_FEATURE (pobj)->pvecx[1];
	  yy[1]=pGRAYPLOT_FEATURE (pobj)->pvecx[3];
	  /** Boundaries of the frame **/
	  C2F(echelle2d)(xx,yy,px1,py1,&pn1,&pn2,"f2i",3L); 
          for ( j =0 ; j < n2 ; j++)	 
	    xm[j]= (int) (( px1[1]*j + px1[0]*((n2-1)-j) )/(n2-1));
  
	  for ( j =0 ; j < n1 ; j++)	 
	    ym[j]= (int) (( py1[0]*j + py1[1]*((n1-1)-j) )/ (n1-1)); 
#ifdef WIN32
	  flag=MaybeSetWinhdc();
#endif

	  frame_clip_on(); 
	  GraySquare1(xm,ym,pGRAYPLOT_FEATURE (pobj)->pvecz,n1,n2); 
	  frame_clip_off();
#ifdef WIN32
      if ( flag == 1) ReleaseWinHdc();
#endif
	  FREE(xm);FREE(ym); /* SS 03/01/03 */
	  break;
	default:
	  break;
	}
      break; 
    case SCI_POLYLINE: 
      if (!sciGetVisibility(pobj)) break;

      sciSetCurrentObj (pobj);	  
      
      itmp[0] = 0;		/* verbose*/
      itmp[1] = 0;		/* thickness value*/
      itmp[2] = 1;		/* narg*/

      /* load the object foreground and dashes color */
      x[0] = sciGetForeground (pobj);	
      x[2] = sciGetLineWidth (pobj);
      x[3] = sciGetLineStyle (pobj);
      markidsizenew[0] = sciGetMarkStyle(pobj);
      markidsizenew[1] = sciGetLineWidth (pobj);
      x[4] = 0;
      v = 0;
      dv = 0;
      logflags[0]='g';
      logflags[1]= pSUBWIN_FEATURE(sciGetParentSubwin(pobj))->logflags[0];
      logflags[2]= pSUBWIN_FEATURE(sciGetParentSubwin(pobj))->logflags[1];


#ifdef WIN32 
      flag=MaybeSetWinhdc();
#endif

      C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
		&dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
		&dv, &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);
      C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
		PD0, PD0, 0L, 0L);   
      C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);
#ifdef WIN32 
      if ( flag == 1) ReleaseWinHdc ();
#endif  
      n1 = pPOLYLINE_FEATURE (pobj)->n1;
      n2 = pPOLYLINE_FEATURE (pobj)->n2;
      closeflag = pPOLYLINE_FEATURE (pobj)->closed;    
      
      if ((xm = MALLOC ((2*n1*n2)*sizeof (integer))) == NULL)	return -1;
      if ((ym = MALLOC ((2*n1*n2)*sizeof (integer))) == NULL)	return -1;
      sciClip(sciGetIsClipping(pobj));
#ifdef WIN32 
      flag=MaybeSetWinhdc ();
#endif

      switch (pPOLYLINE_FEATURE (pobj)->plot)
        {
        case 0:
	  C2F (echelle2d) (pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy, xm, ym, &n1, &n2, "f2i",3L); 
	  if (! sciGetIsMark(pobj))
	    C2F (dr) ("xlines", "xv", &n1, xm, ym, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,0L,0L);
	  else
	    C2F (dr) ("xmarks", "xv", &n1, xm, ym, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 6L,2L);
	  break; 
        case 1:
	  Plo2d1RealToPixel(&n2,&n1,pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy,xm,ym,logflags);  
	  break;
        case 2:
	  Plo2d2RealToPixel(&n2,&n1,pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy,xm,ym,logflags);
          n1=n1*2;
          break;
        case 3:  
          Plo2d3RealToPixel(&n2,&n1,pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy,xm,ym,logflags); 
          for ( j = 0 ; j < n2 ; j++)
	    {
	      lstyle=x[0];
	      iflag=0; nn1= n1*2;
	      C2F(dr)("xsegs","v",&xm[2*n1*j],&ym[2*n1*j],&nn1,&lstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    }
	  break;
        case 4: 
          Plo2d4RealToPixel(&n2,&n1,pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy,xm,ym,logflags);  
          nn2=2*(n1)-1;
          arsize1= Cscale.WIRect1[2]/70.0;arsize2= Cscale.WIRect1[3]/70.0;
          arsize=  (arsize1 < arsize2) ? inint(10*arsize1) : inint(10*arsize2) ;
          for ( j = 0 ; j < n2 ; j++)
	    {
	      integer lstyle=sciGetMarkStyle(pobj) ,iflag=0;
	      C2F(dr)("xarrow","v",&xm[2*n1*j],&ym[2*n1*j],&nn2,&arsize,&lstyle,&iflag,PD0,PD0,PD0,PD0,0L,0L); 
	    } 
          break;
        default:
	  sciprint ("This Polyline cannot be drawn !\n");
#ifdef WIN32 
	  if ( flag == 1) ReleaseWinHdc ();
#endif  
	  break;     
	}
      if (! sciGetIsMark(pobj))
	C2F (dr) ("xlines", "xv", &n1, xm, ym, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
      else
	C2F (dr) ("xmarks", "xv", &n1, xm, ym, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 8L, 2L);
#ifdef WIN32 
      if ( flag == 1) ReleaseWinHdc ();
#endif  
      sciUnClip(sciGetIsClipping(pobj));
     

      FREE (xm);
      FREE (ym);
     
      break;
    case SCI_PATCH: 
      if (!sciGetVisibility(pobj)) break;
      sciSetCurrentObj (pobj);	     

      itmp[0] = 0;		/* verbose*/
      itmp[1] = 0;		/* thickness value*/
      itmp[2] = 1;	       /* narg*/
      C2F (dr) ("xget", "thickness", &itmp[0], &itmp[1], &itmp[2], PI0, PI0,
		PI0, PD0, PD0, PD0, PD0, 0L, 0L);

      /* load the object foreground and dashes color */
      x[0] = sciGetForeground (pobj);	
      x[2] = sciGetLineWidth (pobj);
      x[3] = sciGetLineStyle (pobj);
      x[4] = 0;
      v = 0;
      dv = 0;
#ifdef WIN32 
      flag=MaybeSetWinhdc ();
#endif
      C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
		&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
		&dv, &dv, &dv, &dv, 5L, 10L);
      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 4L, 9L);
#ifdef WIN32 
      if ( flag == 1) ReleaseWinHdc ();
#endif

      n = pPATCH_FEATURE (pobj)->n;
      closeflag = pPATCH_FEATURE (pobj)->closed;

      if ((xm = MALLOC (n * sizeof (int))) == NULL)
      	return -1;
      if ((ym = MALLOC (n * sizeof (int))) == NULL)
      	return -1;
      C2F (echelle2d) (pPATCH_FEATURE (pobj)->pvx,
		       pPATCH_FEATURE (pobj)->pvy, xm, ym, &n, &n2, "f2i",
		       3L);
#ifdef WIN32 
       flag=MaybeSetWinhdc ();
#endif
      sciClip(sciGetIsClipping(pobj));
      C2F (dr) ("xarea", str, &n, xm, ym, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
#ifdef WIN32 
      if ( flag == 1) ReleaseWinHdc ();
#endif
      sciUnClip(sciGetIsClipping(pobj));

      FREE (xm);
      FREE (ym);
      break;
    case SCI_ARC: 
      if (!sciGetVisibility(pobj)) break;
      sciSetCurrentObj (pobj);	
      n = 1;
      
     
      itmp[0] = 0;		/* verbose*/
      itmp[1] = 0;		/* thickness value*/
      itmp[2] = 1;		/* narg*/
      C2F (dr) ("xget", "thickness", itmp, itmp+1, itmp+2, PI0, PI0,
		PI0, PD0, PD0, PD0, PD0, 4L, 9L);

      /* load the object foreground and dashes color */
      
      x[0] = sciGetForeground (pobj);	
      x[2] = sciGetLineWidth (pobj);
      x[3] = sciGetLineStyle (pobj);
      x[4] = 0;
      v = 0;
      dv = 0;
#ifdef WIN32 
      flag=MaybeSetWinhdc ();
#endif
    
      
      
      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, 
		&dv, &dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3,&v, 
		&dv, &dv, &dv, &dv, 5L, 10L );
      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, 
		PD0, PD0, PD0, PD0, 4L, 9L);   
      C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, 
		PD0, PD0, PD0, PD0, 4L, 10L);
      
#ifdef WIN32 
      if ( flag == 1) ReleaseWinHdc ();
#endif
      x1 = XDouble2Pixel (pARC_FEATURE (pobj)->x);
      yy1 = YDouble2Pixel (pARC_FEATURE (pobj)->y);
      w2 = pARC_FEATURE (pobj)->width;
      h2 = pARC_FEATURE (pobj)->height; 
      angle1 = pARC_FEATURE (pobj)->alphabegin;
      angle2 = pARC_FEATURE (pobj)->alphaend;   
     
     

      /* Nouvelles fonctions de changement d'echelle pour les longueurs --> voir PloEch.h */
      w1 = WScale(w2);
      h1 = HScale(h2);
      
      
#ifdef WIN32 
      flag=MaybeSetWinhdc ();
#endif
      sciClip(sciGetIsClipping(pobj));
      if (pARC_FEATURE (pobj)->fill  <= 0)
	C2F (dr) ("xarc", str, &x1, &yy1, &w1, &h1, &angle1, &angle2, PD0, PD0, PD0,PD0, 5L, 0L);
      else
	C2F (dr) ("xfarc", str, &x1, &yy1, &w1, &h1, &angle1, &angle2, PD0, PD0, PD0,PD0, 5L, 0L);
      sciUnClip(sciGetIsClipping(pobj));
#ifdef WIN32 
     if ( flag == 1)  ReleaseWinHdc ();
#endif
      break;
    case SCI_RECTANGLE:  
      if (!sciGetVisibility(pobj)) break;

      sciSetCurrentObj (pobj);
      n = 1;
      if (sciGetFillStyle(pobj) != 0)
	{       x[0] = 64;	/*la dash est de la meme couleur que le foreground*/
	x[1] = 1;
	x[2] = 0;
	x[3] = 0;
	x[4] = 0;
	x[5] = sciGetFillColor(pobj);
#ifdef WIN32 
	flag=MaybeSetWinhdc ();
#endif
		 
	C2F (dr1) ("xset", "pattern", &x[5], x+3, x, x+1, x+3, &v, &dv,
		   &dv, &dv, &dv, 5L, 4096);
#ifdef WIN32 
	if ( flag == 1) ReleaseWinHdc ();
#endif
	}
      /* load the object foreground and dashes color */
      x[0] = sciGetForeground (pobj);
      x[2] = sciGetLineWidth (pobj);
      x[3] = sciGetLineStyle (pobj);
      x[4] = 0;
      markidsizenew[0] =  sciGetMarkStyle(pobj);
      markidsizenew[1] =  sciGetLineWidth (pobj);
     
      v = 0;
      dv = 0; 
#ifdef WIN32 
      flag=MaybeSetWinhdc ();
#endif

      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,
		&dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,
		&dv, &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);    
      C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);
      C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
		PD0, PD0, 0L, 0L);
#ifdef WIN32 
      if ( flag == 1) ReleaseWinHdc ();
#endif 
      x1 = XDouble2Pixel(pRECTANGLE_FEATURE (pobj)->x); 
      yy1 = YDouble2Pixel(pRECTANGLE_FEATURE (pobj)->y);
        
      /* Nouvelles fonctions de changement d'echelle pour les longueurs --> voir PloEch.h */
      width = WDouble2Pixel(pRECTANGLE_FEATURE (pobj)->x,pRECTANGLE_FEATURE (pobj)->width); 
      height = HDouble2Pixel(pRECTANGLE_FEATURE (pobj)->y,pRECTANGLE_FEATURE (pobj)->height);
          
      if (pRECTANGLE_FEATURE (pobj)->strwidth==0)
	{
	  pRECTANGLE_FEATURE (pobj)->strwidth=width;
	  pRECTANGLE_FEATURE (pobj)->strheight=height;
             
	}
      wstr=pRECTANGLE_FEATURE (pobj)->strwidth;
      hstr=pRECTANGLE_FEATURE (pobj)->strheight;
               
             
#ifdef WIN32 
      flag=MaybeSetWinhdc ();
#endif
      sciClip(sciGetIsClipping(pobj));
      if (! sciGetIsMark(pobj))
	if (pRECTANGLE_FEATURE (pobj)->str == 1)
	  {
	    yy1 -= hstr;
	    C2F(dr)("xrect",str,&x1,&yy1,&wstr,&hstr,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  }
	else
	  if (pRECTANGLE_FEATURE (pobj)->fillflag == 0)
	    C2F(dr)("xrect",str,&x1,&yy1,&width,&height,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  else
	    if (pRECTANGLE_FEATURE (pobj)->fillflag == 1)
	      C2F(dr)("xfrect",str,&x1,&yy1,&width,&height,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    else
	      sciprint("  The value must be 1  or 0\r\n");
      else {
	n = 4;
	xtmp[0] = x1;
	xtmp[1] = x1+width;
	xtmp[2] = x1+width;
	xtmp[3] = x1;
	ytmp[0] = yy1;
	ytmp[1] = yy1;
	ytmp[2] = yy1+height;
	ytmp[3] = yy1+height;
	C2F (dr) ("xmarks", str, &n, xtmp, ytmp, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 6L, 0L);
      }
      sciUnClip(sciGetIsClipping(pobj));
#ifdef WIN32 
     if ( flag == 1)  ReleaseWinHdc ();
#endif
      break;
    case SCI_TEXT: 
      if (!sciGetVisibility(pobj)) break;
      sciSetCurrentObj (pobj);	
      n = 1;
      /* load the object foreground and dashes color */
      x[0] = sciGetFontForeground (pobj);/*la dash est de la meme couleur que le foreground*/
      x[2] = sciGetFontDeciWidth (pobj)/100;
      x[3] = 0;
      x[4] = sciGetFontStyle(pobj);
      v = 0;
      dv = 0;
#ifdef WIN32 
      flag=MaybeSetWinhdc ();
#endif
      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
      C2F(dr)("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);
#ifdef WIN32 
      if ( flag == 1) ReleaseWinHdc ();
#endif

      x1  = XDouble2Pixel (pTEXT_FEATURE (pobj)->x);
      yy1 = YDouble2Pixel (pTEXT_FEATURE (pobj)->y);

      hh1 = inint (318.0 * pTEXT_FEATURE (pobj)->wy) ;  
      if (hh1 != 0)
	yy1 -=hh1;
        
      anglestr = (sciGetFontOrientation (pobj)/10); 	/* *10 parce que l'angle est concerve en 1/10eme de degre*/

      flagx = 0;
#ifdef WIN32 
      flag=MaybeSetWinhdc ();
#endif
      sciClip(sciGetIsClipping(pobj));

      C2F (displaystring) (sciGetText (pobj), &x1, &yy1, PI0, &flagx, PI0,
      			   PI0, &anglestr, PD0, PD0, PD0);
#ifdef WIN32 
      if ( flag == 1) ReleaseWinHdc ();
#endif
      sciUnClip(sciGetIsClipping(pobj));
      break;
    case SCI_AXIS: 
      if (!sciGetVisibility(pobj)) break;
      sciSetCurrentObj (pobj);	
      itmp[0] = 0;		/* verbose */
      itmp[1] = 0;		/* thickness value */
      itmp[2] = 1;		/* narg */
      C2F (dr) ("xget", "thickness", &itmp[0], &itmp[1], &itmp[2], PI0, PI0,
		PI0, PD0, PD0, PD0, PD0, 0L, 0L);
      C2F (dr) ("xget", "mark", &itmp[0], markidsizeold, &itmp[3], PI0, PI0, PI0,
		PD0, PD0, PD0, PD0, 0L, 0L);

      /* load the object foreground and dashes color */
      x[0] = sciGetForeground (pobj);
      x[2] = sciGetLineWidth (pobj);
      x[3] = sciGetLineStyle (pobj);
      markidsizenew[0] = sciGetMarkStyle(pobj);
      markidsizenew[1] = sciGetLineWidth (pobj);
      x[4] = 0;
      v = 0;
      dv = 0;

#ifdef WIN32 
      flag=MaybeSetWinhdc ();
#endif

      C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
		&dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
		&dv, &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);
      C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
		PD0, PD0, 0L, 0L);
     
#ifdef WIN32 
      if ( flag == 1) ReleaseWinHdc ();
#endif

      mx[0] = pSUBWIN_FEATURE (sciGetParent(pobj))->FRect[0];
      my[0] = pSUBWIN_FEATURE (sciGetParent(pobj))->FRect[1];
      mx[1] = pSUBWIN_FEATURE (sciGetParent(pobj))->FRect[2];
      my[1] = pSUBWIN_FEATURE (sciGetParent(pobj))->FRect[3];
      mn1 = 1;
      mn2 = 2;

#ifdef WIN32 
      flag=MaybeSetWinhdc (); 
#endif
      axis_draw (pAXIS_FEATURE (pobj)->strflag);
#ifdef WIN32 
      if ( flag == 1) ReleaseWinHdc ();       
#endif

	
      /** 23/05/2002 **/
      if (pAXIS_FEATURE (pobj)->grid != -1)
	{
	  style[0] = pAXIS_FEATURE (pobj)->grid;
	  C2F(xgrid)(style);  
	}
      break;
    case SCI_TITLE:
      if (!sciGetVisibility(pobj)) break;
      sciSetCurrentObj (pobj);       
      /* load the object foreground and dashes color */
      x[0] = sciGetFontForeground (pobj);
      x[2] = sciGetFontDeciWidth (pobj)/100;
      x[3] = 0;
      x[4] = sciGetFontStyle(pobj);
      v = 0;
      dv = 0;
#ifdef WIN32
      flag=MaybeSetWinhdc ();
#endif
      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
      C2F(dr)("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);
#ifdef WIN32
      if ( flag == 1) ReleaseWinHdc ();
#endif
      sciClip(sciGetIsClipping(pobj));
      flagx = 0;
      anglestr = 0;
      psubwin = sciGetParentSubwin(pobj);
      if (sciGetEntityType(psubwin) == SCI_SUBWIN)
	{
	  locstr=pSUBWIN_FEATURE(psubwin)->axes.xdir;
	  switch (locstr)
	    {
	    case 'd':
	      locy=pSUBWIN_FEATURE (psubwin)->FRect[1];
	      loctit=pSUBWIN_FEATURE (psubwin)->FRect[3];
	      break;
	    case 'c':
	      locy=(pSUBWIN_FEATURE (psubwin)->FRect[1]>0.0)?pSUBWIN_FEATURE (psubwin)->FRect[1]: 0.0;
	      locy=(pSUBWIN_FEATURE (psubwin)->FRect[3]<0.0)?pSUBWIN_FEATURE (psubwin)->FRect[1]: locy;
	      loctit=pSUBWIN_FEATURE (psubwin)->FRect[3];
	      break;
	    case 'u':
	      locy=pSUBWIN_FEATURE (psubwin)->FRect[3];
	      loctit=pSUBWIN_FEATURE (psubwin)->FRect[1];
	      break;
	    default:
	      locy=pSUBWIN_FEATURE (psubwin)->FRect[1];
	      loctit=pSUBWIN_FEATURE (psubwin)->FRect[3];
	      break;
	    }
	  locstr=pSUBWIN_FEATURE(psubwin)->axes.ydir;
	  switch (locstr)
	    {
	    case 'l':
	      locx=pSUBWIN_FEATURE (psubwin)->FRect[0];
	      break;
	    case 'c':
	      locx=(pSUBWIN_FEATURE (psubwin)->FRect[0]>0.0)?pSUBWIN_FEATURE (psubwin)->FRect[0]: 0.0;
	      locx=(pSUBWIN_FEATURE (psubwin)->FRect[2]<0.0)?pSUBWIN_FEATURE (psubwin)->FRect[0]: locx;
	      loctit=pSUBWIN_FEATURE (psubwin)->FRect[1];
	      break;
	    case 'r':
	      locx=pSUBWIN_FEATURE (psubwin)->FRect[2];
	      break;
	    default:
	      locx=pSUBWIN_FEATURE (psubwin)->FRect[0];
	      break;
	    }
	}
      switch (pTITLE_FEATURE (pobj)->ptype)
	{
	case 1:
	  rect1[0]= XScale(pSUBWIN_FEATURE (psubwin)->FRect[0]);
	  rect1[1]= (loctit==pSUBWIN_FEATURE (psubwin)->FRect[1])?(YScale(loctit)+Cscale.WIRect1[3]/6):YScale(loctit);

	  rect1[2]= Cscale.WIRect1[2];
	  rect1[3]= Cscale.WIRect1[3]/6;
	  break;
	case 2:
	  rect1[0]= Cscale.WIRect1[0]+Cscale.WIRect1[2];
	  rect1[1]= YScale(locy-(pSUBWIN_FEATURE (psubwin)->FRect[3]-pSUBWIN_FEATURE (psubwin)->FRect[1])/12);
	  rect1[2]= Cscale.WIRect1[2]/6;
	  rect1[3]= Cscale.WIRect1[3]/6;
	  break;
	case 3:
	  rect1[0]= XScale(locx-(pSUBWIN_FEATURE (psubwin)->FRect[2]-pSUBWIN_FEATURE (psubwin)->FRect[0])/12);
	  rect1[1]= Cscale.WIRect1[1]-Cscale.WIRect1[3]/24;
	  rect1[2]= Cscale.WIRect1[2]/6;
	  rect1[3]= Cscale.WIRect1[3]/12;
	  break;
	}
#ifdef WIN32
      flag=MaybeSetWinhdc ();
#endif
      C2F(dr1)("xstringtt",sciGetText (pobj),&rect1[0],&rect1[1],&rect1[2],&rect1[3],&v,&v,&dv,&dv,&dv,&dv,10L,0L);
#ifdef WIN32
      if ( flag == 1) ReleaseWinHdc ();
#endif
      sciUnClip(sciGetIsClipping(pobj));
      break;

    case SCI_AXES:
      if (!sciGetVisibility(pobj)) break;
      sciSetCurrentObj (pobj);	
    
      /* load the object foreground and dashes color */
      
      x[0] = sciGetForeground (pobj);
      x[2] = sciGetLineWidth (pobj);
      x[3] = 0;
      x[4] = 0;
      v = 0;
      dv = 0;
#ifdef WIN32 
      flag=MaybeSetWinhdc ();
#endif
          C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,
		&dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,
		&dv, &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);
      sciClip(sciGetIsClipping(pobj));
     
      Sci_Axis(pAXES_FEATURE (pobj)->dir,pAXES_FEATURE (pobj)->tics,pAXES_FEATURE(pobj)->vx,
               &pAXES_FEATURE (pobj)->nx,pAXES_FEATURE(pobj)->vy,&pAXES_FEATURE (pobj)->ny,
               pAXES_FEATURE(pobj)->str,pAXES_FEATURE (pobj)->subint,pAXES_FEATURE (pobj)->format,
               pAXES_FEATURE (pobj)->fontsize,pAXES_FEATURE (pobj)->textcolor,
               pAXES_FEATURE (pobj)->ticscolor,(char)(pAXES_FEATURE (pobj)->logscale),pAXES_FEATURE (pobj)->seg); 
#ifdef WIN32 
      if ( flag == 1) ReleaseWinHdc ();
#endif
      sciUnClip(sciGetIsClipping(pobj));   
			  
      break;

      /****************************************************************************************/
    case SCI_SURFACE: 
      if (!sciGetVisibility(pobj)) break;
      sciSetCurrentObj (pobj);	
      itmp[0] = 0;		/* verbose*/
      itmp[1] = 0;		/* thickness value*/
      itmp[2] = 1;		/* narg*/
      C2F (dr) ("xget", "thickness", &itmp[0], &itmp[1], &itmp[2], PI0, PI0,PI0, PD0, PD0, PD0, PD0, 4L, 9L);
      C2F (dr) ("xget", "mark", &itmp[0], markidsizeold, &itmp[3], PI0, PI0, PI0,PD0, PD0, PD0, PD0, 4L, 4L);

      /* load the object foreground and dashes color */
      x[0] = sciGetForeground (pobj);	
      x[2] = sciGetLineWidth (pobj);
      x[3] = sciGetLineStyle (pobj);
      markidsizenew[0] = sciGetMarkStyle(pobj);
      markidsizenew[1] = sciGetLineWidth (pobj);
      x[4] = 0;v = 0;dv = 0; 
#ifdef WIN32
       flag=MaybeSetWinhdc();
#endif
      C2F (dr) ("xset", "dashes",     x,   x,   x+4, x+4, x+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x,   x,   x+4, x+4, x+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
      C2F (dr) ("xset", "thickness",  x+2, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 9L);
      C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 4L, 4L);
#ifdef WIN32
      if ( flag == 1) ReleaseWinHdc ();
#endif
		

      n=1;               
#ifdef WIN32
	  flag=MaybeSetWinhdc();
#endif     
      switch(pSURFACE_FEATURE (pobj)->typeof3d)
	{
	case SCI_FAC3D:
	  switch(pSURFACE_FEATURE (pobj)->flagcolor) {
	  case 1 :
	    C2F(fac3d1)(pSURFACE_FEATURE (pobj)->pvecx,pSURFACE_FEATURE (pobj)->pvecy,
			pSURFACE_FEATURE (pobj)->pvecz,
			pSURFACE_FEATURE (pobj)->zcol,&pSURFACE_FEATURE (pobj)->dimzx,
			&pSURFACE_FEATURE (pobj)->dimzy,
			&pSURFACE_FEATURE (pobj)->theta,&pSURFACE_FEATURE (pobj)->alpha,
			pSURFACE_FEATURE (pobj)->legend,pSURFACE_FEATURE (pobj)->flag,
			pSURFACE_FEATURE (pobj)->ebox,n);
	    break;
	  case 0:
	    C2F(fac3d)(pSURFACE_FEATURE (pobj)->pvecx,pSURFACE_FEATURE (pobj)->pvecy,
		       pSURFACE_FEATURE (pobj)->pvecz,
		       pSURFACE_FEATURE (pobj)->zcol,&pSURFACE_FEATURE (pobj)->dimzx,
		       &pSURFACE_FEATURE (pobj)->dimzy,
		       &pSURFACE_FEATURE (pobj)->theta,&pSURFACE_FEATURE (pobj)->alpha,
		       pSURFACE_FEATURE (pobj)->legend,pSURFACE_FEATURE (pobj)->flag,
		       pSURFACE_FEATURE (pobj)->ebox,n);
	    break;
	  case 3:
	    C2F(fac3d3)(pSURFACE_FEATURE (pobj)->pvecx,pSURFACE_FEATURE (pobj)->pvecy,
			pSURFACE_FEATURE (pobj)->pvecz,
			pSURFACE_FEATURE (pobj)->zcol,&pSURFACE_FEATURE (pobj)->dimzx,
			&pSURFACE_FEATURE (pobj)->dimzy,
			&pSURFACE_FEATURE (pobj)->theta,&pSURFACE_FEATURE (pobj)->alpha,
			pSURFACE_FEATURE (pobj)->legend,pSURFACE_FEATURE (pobj)->flag,
			pSURFACE_FEATURE (pobj)->ebox,n);
	    break; 
	  case 2:
	    C2F(fac3d2)(pSURFACE_FEATURE (pobj)->pvecx,pSURFACE_FEATURE (pobj)->pvecy,
			pSURFACE_FEATURE (pobj)->pvecz,
			pSURFACE_FEATURE (pobj)->zcol,&pSURFACE_FEATURE (pobj)->dimzx,
			&pSURFACE_FEATURE (pobj)->dimzy,
			&pSURFACE_FEATURE (pobj)->theta,&pSURFACE_FEATURE (pobj)->alpha,
			pSURFACE_FEATURE (pobj)->legend,pSURFACE_FEATURE (pobj)->flag,
			pSURFACE_FEATURE (pobj)->ebox,n);
	    break; 
	  }
	  break;
	case SCI_PLOT3D:
	  switch(pSURFACE_FEATURE (pobj)->flagcolor) {
	  case 1 :
	    C2F(plot3d1)(pSURFACE_FEATURE (pobj)->pvecx,pSURFACE_FEATURE (pobj)->pvecy,
			 pSURFACE_FEATURE (pobj)->pvecz,
			 &pSURFACE_FEATURE (pobj)->dimzx,&pSURFACE_FEATURE (pobj)->dimzy,
			 &pSURFACE_FEATURE (pobj)->theta,&pSURFACE_FEATURE (pobj)->alpha,
			 pSURFACE_FEATURE (pobj)->legend,pSURFACE_FEATURE (pobj)->flag,
			 pSURFACE_FEATURE (pobj)->ebox,n);
	    break;
	  case 0:
	    C2F(plot3d)(pSURFACE_FEATURE (pobj)->pvecx,pSURFACE_FEATURE (pobj)->pvecy,
			pSURFACE_FEATURE (pobj)->pvecz,
			&pSURFACE_FEATURE (pobj)->dimzx,&pSURFACE_FEATURE (pobj)->dimzy,
			&pSURFACE_FEATURE (pobj)->theta,&pSURFACE_FEATURE (pobj)->alpha,
			pSURFACE_FEATURE (pobj)->legend,pSURFACE_FEATURE (pobj)->flag,
			pSURFACE_FEATURE (pobj)->ebox,n);
	    break;
	  }
	  break;
	case SCI_PARAM3D1:
	  C2F(param3d1)(pSURFACE_FEATURE (pobj)->pvecx,pSURFACE_FEATURE (pobj)->pvecy,
			pSURFACE_FEATURE (pobj)->pvecz,
			&pSURFACE_FEATURE (pobj)->dimzx,&pSURFACE_FEATURE (pobj)->dimzy,
			&pSURFACE_FEATURE (pobj)->izcol,pSURFACE_FEATURE (pobj)->zcol,
			&pSURFACE_FEATURE (pobj)->theta,&pSURFACE_FEATURE (pobj)->alpha,
			pSURFACE_FEATURE (pobj)->legend,pSURFACE_FEATURE (pobj)->flag,
			pSURFACE_FEATURE (pobj)->ebox,n);
	  break;
	default:
	  break;
	}
#ifdef WIN32
      if ( flag == 1) ReleaseWinHdc ();
#endif
      break;
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    default:
      return -1;
      break;
    }
  sciSetSelectedSubWin (currentsubwin);
  return -1;
}




/**sciCreateFont
 * @memo This funcion creates logical font
 * @param  HDC hdc: The Handle Context...(hdc = BeginPaint(hwnd))
 * @param  TCHAR * szFaceName: the name of the font (for ex: Times New Roman)
 * @param  int iDeciPtHeight:
 * @param  int iDeciPtWidth:
 * @param  int iAttributes: the logical bitwise between SCI_ATTR_
 * @param  BOOL fLogRes:
 * @return  HFONT structure
 */
#ifdef WIN32 
/*
HFONT
sciCreateFont (HDC hdc, char *szFaceName, int iDeciPtHeight,
	       int iDeciPtWidth, int iAttributes, BOOL fLogRes)
{
  FLOAT cxDpi, cyDpi;
  HFONT hFont;
  LOGFONT lf;
  POINT2D pt;
  TEXTMETRIC tm;

  SaveDC (hdc);

  SetGraphicsMode (hdc, GM_ADVANCED);
  ModifyWorldTransform (hdc, NULL, MWT_IDENTITY);
  SetViewportOrgEx (hdc, 0, 0, NULL);
  SetWindowOrgEx (hdc, 0, 0, NULL);

  if (fLogRes)
    {
      cxDpi = (FLOAT) GetDeviceCaps (hdc, LOGPIXELSX);
      cyDpi = (FLOAT) GetDeviceCaps (hdc, LOGPIXELSY);
    }
  else
    {
      cxDpi = (FLOAT) (25.4 * GetDeviceCaps (hdc, HORZRES) /
		       GetDeviceCaps (hdc, HORZSIZE));

      cyDpi = (FLOAT) (25.4 * GetDeviceCaps (hdc, VERTRES) /
		       GetDeviceCaps (hdc, VERTSIZE));
    }

  pt.x = (int) (iDeciPtWidth * cxDpi / 72);
  pt.y = (int) (iDeciPtHeight * cyDpi / 72);

  DPtoLP (hdc, &pt, 1);

  lf.lfHeight = -(int) (fabs (pt.y) / 10.0 + 0.5);
  lf.lfWidth = 0;
  lf.lfEscapement = 0;
  lf.lfOrientation = 0;
  lf.lfWeight = iAttributes & SCI_ATTR_BOLD ? 700 : 0;
  lf.lfItalic = iAttributes & SCI_ATTR_ITALIC ? 1 : 0;
  lf.lfUnderline = iAttributes & SCI_ATTR_UNDERLINE ? 1 : 0;
  lf.lfStrikeOut = iAttributes & SCI_ATTR_STRIKEOUT ? 1 : 0;
  lf.lfCharSet = DEFAULT_CHARSET;
  lf.lfOutPrecision = 0;
  lf.lfClipPrecision = 0;
  lf.lfQuality = 0;
  lf.lfPitchAndFamily = 0;

  lstrcpy (lf.lfFaceName, szFaceName);

  hFont = CreateFontIndirect (&lf);

  if (iDeciPtWidth != 0)
    {
      hFont = (HFONT) SelectObject (hdc, hFont);

      GetTextMetrics (hdc, &tm);

      DeleteObject (SelectObject (hdc, hFont));

      lf.lfWidth = (int) (tm.tmAveCharWidth *
			  fabs (pt.x) / fabs (pt.y) + 0.5);

      hFont = CreateFontIndirect (&lf);
    }

  RestoreDC (hdc, -1);
  return hFont;
}
*/
#endif



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
  return (sciPointObj *) pcurrentpobj;
}



/**sciSetCurrentObj()
 * @memo sets the pointer to the current selected object (actualy figure object). Used after contructor function.
 */
int
sciSetCurrentObj (sciPointObj * pobj)
{
  /* pcurrentobj is a static variable */
  pcurrentpobj = pobj;
  return 0;
}



/**sciGetCurrentFigure
 * @memo Returns the pointer to the current selected figure. 
 */
sciPointObj *
sciGetCurrentFigure ()
{
  return (sciPointObj *) sciGetCurrentScilabXgc ()->mafigure  ; 
}                                                               



static BOOL modereplay = FALSE;
/**sciSetReplay
 * @memo sets the variable "we are in replay mode"
 */
int
sciSetReplay (BOOL value)
{
  modereplay = value;
  return 0;

}


/**sciGetReplay
 * @memo returns the variable "we are in replay mode"
 */
BOOL
sciGetReplay ()
{
  return modereplay;

}



/**sciSetPosX
 * @memo returns coordonee X in REAL (probably obsolete in few futur, replace with sciSetpoint)
 */
int
sciSetPosX (sciPointObj * pthis, double x)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_TEXT:
      sciSetTextPosX (pthis, x);
      return 0;
      break;  
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
      return -1;
      break; 
    case SCI_POLYLINE:  
    case SCI_LEGEND: 
    case SCI_AXIS:
    case SCI_TITLE:
      return -1;
      break;  
    case SCI_SEGS:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:    
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("This object has no place X\n");
      return -1;
      break;
    }
  return -1;
}



/**sciSetPosY
 * @memo returns coordonee Y in REAL (probably obolete in few futur, replace with sciSetpoint)
 */
int
sciSetPosY (sciPointObj * pthis, double y)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_TEXT:
      sciSetTextPosY (pthis, y);
      return 0;
      break;      
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
      return -1;
      break; 
    case SCI_POLYLINE:
    case SCI_LEGEND:
    case SCI_AXIS:
    case SCI_TITLE:
      return -1;
      break; 
    case SCI_SEGS:
    case SCI_SBV:
    case SCI_SBH:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_PATCH:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("This object has no place Y\n");
      return -1;
      break;
    }
  return -1;
}




/**sciGetPosX
 * @memo returns coordonee X in pixel for figure and subwindow and in REAL for other entity (probably obolete in few futur, replace with sciGetpoint)
 */
double
sciGetPosX (sciPointObj * pthis)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_FIGURE:
      return (double) sciGetFigurePosX (pthis);
      break;
    case SCI_SUBWIN:
      return (double) sciGetSubwindowPosX (pthis);
      break;
    case SCI_TEXT:
      return sciGetTextPosX (pthis);
      break;
    case SCI_SBH:
      return (double) sciGetScrollPosH (pthis);
      break;
    case SCI_SBV:
      return 0;			/* les coordonnees sont (0,y)  il n'y a pas de X */
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE (pthis)->x;
      break;
    case SCI_ARC:		
      return pARC_FEATURE (pthis)->x;
      break;
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE (pthis)->xmin;
      break;
    case SCI_PATCH:
      return pPATCH_FEATURE (pthis)->xmin;
      break;
    case SCI_AGREG:
      return pAGREG_FEATURE (pthis)->xmin;
      break; 
    case SCI_LEGEND:
    case SCI_AXIS:
    case SCI_TITLE:
      return -1;
      break;
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
      return -1;
      break; 
    case SCI_SEGS:
    case SCI_SURFACE:
    case SCI_LIGHT:  
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    default:
      sciprint ("This object has no place X\n");
      return -1;
      break;
    }
  return -1;
}


/**sciGetPosWidth
 * @memo returns width in in pixel for figure and subwindow and in REAL dimension for the other (probably obolete in few futur, replace with sciGetpoint)
 */
double
sciGetPosWidth (sciPointObj * pthis)
{
  int x[2],itmp=0;
  double xtmp1, xtmp2;
  switch (sciGetEntityType (pthis))
    {
    case SCI_FIGURE:
      C2F(dr)("xget","wpdim",&itmp,x,&itmp,PI0,PI0,PI0,&xtmp2,PD0,PD0,PD0,0L,0L);
      pFIGURE_FEATURE (pthis)->figuredimwidth=x[0];
      return pFIGURE_FEATURE (pthis)->figuredimwidth;
      break;
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE (pthis)->windimwidth;
      break;
    case SCI_TEXT:
      return sciGetTextPosWidth (pthis);
      break;
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE (pthis)->width;
      break;
    case SCI_ARC:		
      return pARC_FEATURE (pthis)->width;
      break;
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE (pthis)->width;
      break;
    case SCI_PATCH:
      return pPATCH_FEATURE (pthis)->width;
      break;
    case SCI_AGREG:
      xtmp1 = pAGREG_FEATURE(pthis)->xmax;
      xtmp2 = pAGREG_FEATURE(pthis)->xmin;
      return fabs(pAGREG_FEATURE(pthis)->xmax - pAGREG_FEATURE(pthis)->xmin);
      break;
      break; 
    case SCI_LEGEND:   
    case SCI_AXIS:
    case SCI_TITLE:
      return -1;
      break;
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
      return -1;
      break; 
    case SCI_SEGS:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_SURFACE:
    case SCI_LIGHT:    
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    default:
      sciprint ("This object has no place X\n");
      return -1;
      break;
    }
  return -1;
}


/**sciGetPosY
 * @memo returns coordonee Y  in REAL dimension (probably obolete in few futur, replace with sciGetpoint)
 */
double
sciGetPosY (sciPointObj * pthis)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_FIGURE:
      return (double) sciGetFigurePosY (pthis);
      break;
    case SCI_SUBWIN:
      return (double) sciGetSubwindowPosY (pthis);
      break;
    case SCI_TEXT:
      return sciGetTextPosY (pthis);
      break;
    case SCI_SBV:
      return (double) sciGetScrollPosV (pthis);
      break;
    case SCI_SBH:
      return 0;			/* les coordonnees sont (x,0)  il n'y a pas de y */
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE (pthis)->y;
      break;
    case SCI_ARC:		
      return pARC_FEATURE (pthis)->y;
      break;
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE (pthis)->ymin;
      break;
    case SCI_PATCH:
      return pPATCH_FEATURE (pthis)->ymin;
      break;
    case SCI_AGREG:
      return pAGREG_FEATURE (pthis)->ymin;
      break;   
    case SCI_LEGEND:
    case SCI_AXIS:
    case SCI_TITLE:
      return -1;
      break;
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
      return -1;
      break;
    case SCI_SURFACE:
    case SCI_LIGHT:    
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    default:
      sciprint ("This object has no place Y\n");
      return -1;
      break;
    }
  return -1;
}




/**sciGetPosHeight
 * @memo returns width in REAL (probably obolete in few futur, replace with sciGetpoint)
 */
double
sciGetPosHeight (sciPointObj * pthis)
{ 
  int x[2],itmp=0;
  double xtmp2;
	
  switch (sciGetEntityType (pthis))
    {
    case SCI_FIGURE:  
      C2F(dr)("xget","wpdim",&itmp,x,&itmp,PI0,PI0,PI0,&xtmp2,PD0,PD0,PD0,0L,0L);
      pFIGURE_FEATURE (pthis)->figuredimheight=x[1];
      return pFIGURE_FEATURE (pthis)->figuredimheight;
      break;
    case SCI_SUBWIN:
      return pSUBWIN_FEATURE (pthis)->windimheight;
      break;
    case SCI_TEXT:
      return sciGetTextPosHeight (pthis);
      break;
    case SCI_RECTANGLE:
      return pRECTANGLE_FEATURE (pthis)->height;
      break;
    case SCI_ARC:		
      return pARC_FEATURE (pthis)->height;
      break;
    case SCI_POLYLINE:
      return pPOLYLINE_FEATURE (pthis)->height;
      break;
    case SCI_PATCH:
      return pPATCH_FEATURE (pthis)->height;
      break;
    case SCI_AGREG:
      return fabs(pAGREG_FEATURE (pthis)->ymax - pAGREG_FEATURE (pthis)->ymin);
      break;  
    case SCI_LEGEND: 
    case SCI_AXIS:
    case SCI_TITLE:
      return -1;
      break;
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
      return -1;
      break;
    case SCI_SBH:
    case SCI_SBV:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    default:
      sciprint ("This object has no place X\n");
      return -1;
      break;
    }
  return -1;
}



/**sciGetPoint
 * @memo returns pointer to the points of the entity, and a pointer to the number of points. This function allocates memory for the tab of point, so after using the tab don't forget to free it
 */
double *sciGetPoint(sciPointObj * pthis, int *numrow, int *numcol)
{
  double *tab;
  int i,k;
  switch (sciGetEntityType (pthis))
    {
    case SCI_FIGURE:
      *numrow = 2;
      *numcol = 2;
      if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] = sciGetPosX (pthis);
      tab[1] = sciGetPosY (pthis);
      tab[2] = (double)sciGetWidth (pthis);
      tab[3] = (double)sciGetHeight (pthis);
      return (double*)tab;
      break;
    case SCI_SUBWIN:
      *numrow = 3;
      *numcol = 2;
      if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] = sciGetPosX (pthis);
      tab[1] = sciGetPosY (pthis);
      tab[2] = (double)sciGetWidth (pthis);
      tab[3] = (double)sciGetHeight (pthis);
      tab[4] = (double)sciGetScilabXgc(pthis)->CWindowWidthView; 
      tab[5] = (double)sciGetScilabXgc(pthis)->CWindowHeightView; 
      return (double*)tab;
      break;
    case SCI_PATCH:
      *numrow = pPATCH_FEATURE (pthis)->n;
      *numcol = 2;
      if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      for (i=0;i < *numrow;i++)
	{
	  tab[i] = pPATCH_FEATURE (pthis)->pvx[i];	
	  tab[*numrow+i]= pPATCH_FEATURE (pthis)->pvy[i];  
	}
      return (double*)tab;
      break;
    case SCI_POLYLINE:
      *numrow = pPOLYLINE_FEATURE (pthis)->n1;
      *numcol = 2;
      if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      for (i=0;i < *numrow;i++)
	{
	  tab[i] = pPOLYLINE_FEATURE (pthis)->pvx[i];	
	  tab[*numrow+i]= pPOLYLINE_FEATURE (pthis)->pvy[i];  
	}
      return (double*)tab;
      break;
    case SCI_RECTANGLE:
      *numrow = 2;
      *numcol = 2;
      if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] = pRECTANGLE_FEATURE (pthis)->x;
      tab[2] = pRECTANGLE_FEATURE (pthis)->y;
      tab[2] = pRECTANGLE_FEATURE (pthis)->width;
      tab[3] = pRECTANGLE_FEATURE (pthis)->height;
      return (double*)tab;
      break;
    case SCI_ARC:
      *numrow = 3;
      *numcol = 2;
      if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] = sciGetPosX (pthis);
      tab[1] = sciGetPosY (pthis);
      tab[2] = pARC_FEATURE (pthis)->width;
      tab[3] = pARC_FEATURE (pthis)->height;
      tab[4] = pARC_FEATURE (pthis)->alphabegin;
      tab[5] = pARC_FEATURE (pthis)->alphaend;
      return (double*)tab;
      break;
    case SCI_AGREG:
      *numrow = 2;
      *numcol = 2;
      if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] = sciGetPosX (pthis);
      tab[1] = sciGetPosY (pthis);
      tab[2] = sciGetPosWidth(pthis);
      tab[3] = sciGetPosHeight(pthis);
      return (double*)tab;
      break;
    case SCI_TEXT:
      *numrow = 1;
      *numcol = 2;
      if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] = sciGetPosX (pthis);
      tab[1] = sciGetPosY (pthis);
      return (double*)tab;
      break;
    case SCI_SBV:
      return (double*)NULL;
      break;
    case SCI_SBH:
      return (double*)NULL;			/* les coordonnees sont (x,0) */
    case SCI_TITLE:
    case SCI_SEGS: 
      *numrow = pSEGS_FEATURE (pthis)->Nbr1;
      if (pSEGS_FEATURE (pthis)->ptype == 0) {
	*numcol = 2;
	if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	  return (double*)NULL;
	for (i=0;i < *numrow;i++)
	  {
	    tab[i] = pSEGS_FEATURE (pthis)->vx[i];	
	    tab[*numrow+i]= pSEGS_FEATURE (pthis)->vy[i];  
	  }
      }
      else {
	*numcol = 2 + *numrow*2;
	if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	  return (double*)NULL;
	for (i=0;i < *numrow;i++)
	  {
	    tab[i] = pSEGS_FEATURE (pthis)->vx[i];	
	    tab[*numrow+i]= pSEGS_FEATURE (pthis)->vy[i];  
	  }
	k=*numrow*2;
	for (i=0;i < *numrow * *numrow;i++)
	  {
	    tab[k+i] = pSEGS_FEATURE (pthis)->vfx[i];	
	  }
	k=k+*numrow * *numrow;
	for (i=0;i < *numrow * *numrow;i++)
	  {
	    tab[k+i] = pSEGS_FEATURE (pthis)->vfy[i];	
	  }
      }
      return (double*)tab;
      break;
    case SCI_SURFACE:
      if (pSURFACE_FEATURE (pthis)->typeof3d == SCI_PARAM3D1)  {
	int N;
	*numrow = pSURFACE_FEATURE (pthis)->dimzx;
	*numcol = 3*pSURFACE_FEATURE (pthis)->dimzy;
	N = *numrow * pSURFACE_FEATURE (pthis)->dimzy;
	if ((tab = calloc(3*N,sizeof(double))) == NULL)
	  return (double*)NULL;
	for (i=0;i < N;i++) 
	  tab[i] = pSURFACE_FEATURE (pthis)->pvecx[i];	
	k=N;
	for (i=0;i < N;i++) 
	  tab[k+i] = pSURFACE_FEATURE (pthis)->pvecy[i];
	k=k+ N;
	for (i=0;i < N;i++) 
	  tab[k+i] = pSURFACE_FEATURE (pthis)->pvecz[i];
      }
      else {
	sciprint ("Un handled data field\n");
	return (double*)NULL;
      }
      return (double*)tab;
      break;
    case SCI_GRAYPLOT:
      if (pGRAYPLOT_FEATURE (pthis)->type == 0) { /* gray plot */
	int ny=pGRAYPLOT_FEATURE (pthis)->ny,nx=pGRAYPLOT_FEATURE (pthis)->nx;
	*numrow = nx+1;
	*numcol = ny+1;
	if ((tab = calloc(*numrow * *numcol,sizeof(double))) == NULL)
	  return (double*)NULL;
	tab[0]=0;
	for (i=0;i < nx;i++) 
	  tab[i+1] = pGRAYPLOT_FEATURE (pthis)->pvecx[i];
	for (i=0;i < ny;i++) 
	  tab[*numrow*(i+1)] = pGRAYPLOT_FEATURE (pthis)->pvecy[i];

	for (i=0;i < ny;i++) 
	  for (k=0;k < nx;k++) 
	    tab[*numrow*(i+1)+k+1] = pGRAYPLOT_FEATURE (pthis)->pvecz[nx*i+k];
      }
      else  {/* Matplot */
	int ny=pGRAYPLOT_FEATURE (pthis)->ny-1,nx=pGRAYPLOT_FEATURE (pthis)->nx-1;
	*numrow = nx;	*numcol = ny;
	if ((tab = calloc(nx*ny,sizeof(double))) == NULL)
	  return (double*)NULL;
	for (i=0;i < nx*ny;i++) 
	    tab[i] = pGRAYPLOT_FEATURE (pthis)->pvecz[i];
      }
      return (double*)tab;
      break;
    case SCI_FEC: 
      *numcol = 3;
      *numrow = pFEC_FEATURE (pthis)->Nnode;
      if ((tab = calloc(*numrow * 3,sizeof(double))) == NULL)
	return (double*)NULL;

      for (i=0;i < *numrow;i++) {
	tab[i] = pFEC_FEATURE (pthis)->pvecx[i];
	tab[*numrow+i] = pFEC_FEATURE (pthis)->pvecy[i];
	tab[*numrow*2+i] = pFEC_FEATURE (pthis)->pfun[i];
      }
      return (double*)tab;
      break;
    case SCI_LEGEND:
    case SCI_LIGHT:
    case SCI_AXIS:    
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    default:
      sciprint ("This object has no points Y\n");
      return (double*)NULL;
      break;
    }
  return (double*)NULL;
}


/**sciSetPoint
 * @memo sets points of the entity, and a pointer to the number of points
 */
int
sciSetPoint(sciPointObj * pthis, double *tab, int *numrow, int *numcol)
{
  int i,n1,k,k1;
  double *pvx,*pvy, *pvfx,*pvfy;
  int *pstyle;
  POINT2D *pvector;

  switch (sciGetEntityType (pthis))
    {
    case SCI_POLYLINE:
      n1=pPOLYLINE_FEATURE (pthis)->n1;
      if (*numcol != 2)
	{
	  sciprint("The number of columns must be %d\n",2);
	  return -1;
	}
      if (*numrow != n1) /* SS 30/1/02 */
	{
	  n1=*numrow;
	  if ((pvx = MALLOC (n1 * sizeof (double))) == NULL) return -1;
	  if ((pvy = MALLOC (n1 * sizeof (double))) == NULL) {
	    FREE(pvx);
	    return -1;
	  }
	  if ((pvector = MALLOC (n1 * sizeof (POINT2D))) == NULL) {
	    FREE(pvx);FREE(pvy);
	    return -1;
	  }
	  FREE(pPOLYLINE_FEATURE (pthis)->pvx);
	  FREE(pPOLYLINE_FEATURE (pthis)->pvy);
	  FREE(pPOLYLINE_FEATURE (pthis)->pvector);
	  for (i=0;i < *numrow;i++)
	    {
	      pvx[i] = tab[i];
	      pvy[i] = tab[i+ (*numrow)];
	      pvector[i].x = tab[i];
	      pvector[i].y = tab[i+ (*numrow)];
	    }
	  pPOLYLINE_FEATURE (pthis)->pvx=pvx;
	  pPOLYLINE_FEATURE (pthis)->pvy=pvy;
	  pPOLYLINE_FEATURE (pthis)->pvector=pvector;
	  pPOLYLINE_FEATURE (pthis)->n1=n1;
	}
      else
	for (i=0;i < *numrow;i++)
	  {
	    pPOLYLINE_FEATURE (pthis)->pvx[i] = tab[i];
	    pPOLYLINE_FEATURE (pthis)->pvy[i] = tab[i+ (*numrow)];
	  }
      return 0;
      break;
    case SCI_PATCH:
      n1=pPATCH_FEATURE (pthis)->n;
      if (*numcol != 2)
	{
	  sciprint("The number of columns must be %d\n",2);
	  return -1;
	}
      if (*numrow != n1) /* SS 30/1/02 */
	{
	  n1=*numrow;
	  if ((pvx = MALLOC (n1 * sizeof (double))) == NULL) return -1;
	  if ((pvy = MALLOC (n1 * sizeof (double))) == NULL) {
	    FREE(pvx);
	    return -1;
	  }
	  if ((pvector = MALLOC (n1 * sizeof (POINT2D))) == NULL) {
	    FREE(pvx);FREE(pvy);
	    return -1;
	  }
	  FREE(pPATCH_FEATURE (pthis)->pvx);
	  FREE(pPATCH_FEATURE (pthis)->pvy);
	  FREE(pPATCH_FEATURE (pthis)->pvector);
	  for (i=0;i < *numrow;i++)
	    {
	      pvx[i] = tab[i];
	      pvy[i] = tab[i+ (*numrow)];
	      pvector[i].x = tab[i];
	      pvector[i].y = tab[i+ (*numrow)];
	    }
	  pPATCH_FEATURE (pthis)->pvx=pvx;
	  pPATCH_FEATURE (pthis)->pvy=pvy;
	  pPATCH_FEATURE (pthis)->pvector=pvector;
	  pPATCH_FEATURE (pthis)->n=n1;
	}
      else 
	for (i=0;i < *numrow;i++)
	  {
	    pPATCH_FEATURE (pthis)->pvx[i] = tab[i];
	    pPATCH_FEATURE (pthis)->pvy[i] = tab[i+ (*numrow)];
	  }
      return 0;
      break;
    case SCI_RECTANGLE:
      if (*numrow * *numcol != 4)
	{
	  sciprint("The number of element must be %d\n",4);
	  return -1;
	}

      pRECTANGLE_FEATURE (pthis)->x          = tab[0];
      pRECTANGLE_FEATURE (pthis)->y          = tab[1];
      pRECTANGLE_FEATURE (pthis)->width      = tab[2];
      pRECTANGLE_FEATURE (pthis)->height     = tab[3];
      return 0;
      break;
    case SCI_ARC:
      if (*numrow * *numcol != 6)
	{
	  sciprint("The number of elements must be %d\n",6);
	  return -1;
	}
      
      pARC_FEATURE (pthis)->x          = tab[0];
      pARC_FEATURE (pthis)->width      = tab[2];
      pARC_FEATURE (pthis)->alphabegin = tab[4];
      pARC_FEATURE (pthis)->y          = tab[1];
      pARC_FEATURE (pthis)->height     = tab[3];
      pARC_FEATURE (pthis)->alphaend   = tab[5];
      return 0;
      break;
    case SCI_TEXT:
      if (*numrow * *numcol != 2)
	{
	  sciprint("The number of elements must be %d\n",2);
	  return -1;
	}
      pTEXT_FEATURE (pthis)->x = tab[0];
      pTEXT_FEATURE (pthis)->y = tab[1];
      return 0;
      break;
    case SCI_SEGS: 
      if (pSEGS_FEATURE (pthis)->ptype == 0) {
	if (*numcol != 2)
	  {
	    sciprint("The number of columns must be %d\n",2);
	    return -1;
	  }
	n1=pSEGS_FEATURE (pthis)->Nbr1;
	if (*numrow != n1) /* SS 30/1/02 */
	  {
	    n1=*numrow;
	    if ((pvx = MALLOC (n1 * sizeof (double))) == NULL) return -1;
	    if ((pvy = MALLOC (n1 * sizeof (double))) == NULL) {
	      FREE(pvx);
	      return -1;
	    }
	    if ((pstyle = MALLOC (n1 * sizeof (int))) == NULL) {
	      FREE(pvx);FREE(pvy);
	      return -1;
	    }
	    FREE(pSEGS_FEATURE (pthis)->vx);
	    FREE(pSEGS_FEATURE (pthis)->vy);
	    FREE(pSEGS_FEATURE (pthis)->pstyle);
	    for (i=0;i < *numrow;i++)
	      {
		pvx[i] = tab[i];
		pvy[i] = tab[i+ (*numrow)];
		pstyle[i] = 0;
	      }
	    pSEGS_FEATURE (pthis)->vx=pvx;
	    pSEGS_FEATURE (pthis)->vy=pvy;
	    pSEGS_FEATURE (pthis)->Nbr1=n1;
	    pSEGS_FEATURE (pthis)->pstyle=pstyle;
	  }
	else
	  for (i=0;i < *numrow;i++)
	    {
	      pSEGS_FEATURE (pthis)->vx[i] = tab[i];
	      pSEGS_FEATURE (pthis)->vy[i] = tab[i+ (*numrow)];
	    }
      }
      else {
	if (*numcol != 2 +2*(*numrow * *numrow))
	  {
	    sciprint("The number of columns must be %d\n",2 +2*(*numrow * *numrow));
	    return -1;
	  }
	n1=pSEGS_FEATURE (pthis)->Nbr1;
	if (*numrow != n1) /* SS 30/1/02 */
	  {
	    n1=*numrow;
	    if ((pvx = MALLOC (n1 * sizeof (double))) == NULL) return -1;
	    if ((pvy = MALLOC (n1 * sizeof (double))) == NULL) {
	      FREE(pvx);
	      return -1;
	    }
	    if ((pstyle = MALLOC (n1 * sizeof (int))) == NULL) {
	      FREE(pvx);FREE(pvy);
	      return -1;
	    }
	    if ((pvfx = MALLOC ((n1*n1) * sizeof (double))) == NULL) return -1;
	    if ((pvfy = MALLOC ((n1*n1) * sizeof (double))) == NULL) {
	      FREE(pvx);FREE(pvy);FREE(pvfx);
	      return -1;
	    }
	    FREE(pSEGS_FEATURE (pthis)->vx);
	    FREE(pSEGS_FEATURE (pthis)->vy);
	    FREE(pSEGS_FEATURE (pthis)->vfx);
	    FREE(pSEGS_FEATURE (pthis)->vfy);
	    for (i=0;i < n1;i++)
	      {
		pvx[i] = tab[i];
		pvy[i] = tab[i+ (*numrow)];

	      }
	    k=2*n1;
	    for (i=0;i < n1*n1;i++)
	      {
		pvfx[i] = tab[k+i];
		pvfy[i] = tab[k+n1*n1+i];

	      }
	    pSEGS_FEATURE (pthis)->vx=pvx;
	    pSEGS_FEATURE (pthis)->vy=pvy;
	    pSEGS_FEATURE (pthis)->vx=pvfx;
	    pSEGS_FEATURE (pthis)->vy=pvfy;
	    pSEGS_FEATURE (pthis)->Nbr1=n1;

	  }
	else {
	  for (i=0;i < *numrow;i++)   {
	    pSEGS_FEATURE (pthis)->vx[i] = tab[i];
	    pSEGS_FEATURE (pthis)->vy[i] = tab[i+ (*numrow)];
	  }
	  k=2* (*numrow);
	  k1=k+ (*numrow * *numrow);
	  for (i=0;i < *numrow * *numrow ;i++)   {
	    pSEGS_FEATURE (pthis)->vfx[i] = tab[k+i];
	    pSEGS_FEATURE (pthis)->vfy[i] = tab[k1+i];
	  }
	}
      }
      return 0;
      break;


    case SCI_SURFACE:
      if (pSURFACE_FEATURE (pthis)->typeof3d == SCI_PARAM3D1)  {
	int ncurv,N,*zcol;
	double *pvecx, *pvecy, *pvecz;
	ncurv=*numcol/3;
	if (3*ncurv != *numcol) {
	  sciprint("The number of columns must be a multiple of 3\n");
	  return -1;
	}
	N = *numrow * ncurv;
	if ((*numrow != pSURFACE_FEATURE (pthis)->dimzx)||(*numcol != pSURFACE_FEATURE (pthis)->dimzy)){
	  if ((pvecx = MALLOC (N * sizeof (double))) == NULL) return -1;
	  if ((pvecy = MALLOC (N * sizeof (double))) == NULL) {
	    FREE(pvecx);
	    return -1;
	  }
	  if ((pvecz = MALLOC (N * sizeof (double))) == NULL) {
	    FREE(pvecx);FREE(pvecy);
	    return -1;
	  }
	  if ((zcol = MALLOC (ncurv * sizeof (int))) == NULL) {
	    FREE(pvecx);FREE(pvecy);FREE(pvecz);
	    return -1;
	  }

	  FREE(pSURFACE_FEATURE (pthis)->pvecx);pSURFACE_FEATURE (pthis)->pvecx=pvecx;
	  FREE(pSURFACE_FEATURE (pthis)->pvecy);pSURFACE_FEATURE (pthis)->pvecy=pvecy;
	  FREE(pSURFACE_FEATURE (pthis)->pvecz);pSURFACE_FEATURE (pthis)->pvecz=pvecz;

	  /* adjust colors */
	  if (ncurv <= pSURFACE_FEATURE (pthis)->dimzy) {
	    for (i=0;i <ncurv ;i++) 
	      zcol[i] = pSURFACE_FEATURE (pthis)->zcol[i];
	  }
	  else {
	    for (i=0;i <pSURFACE_FEATURE (pthis)->dimzy ;i++) 
	      zcol[i] = pSURFACE_FEATURE (pthis)->zcol[i];
	    for (i=pSURFACE_FEATURE (pthis)->dimzy;i < ncurv ;i++) 
	      zcol[i] = 1;
	  }
	  FREE(pSURFACE_FEATURE (pthis)->zcol);pSURFACE_FEATURE (pthis)->zcol=zcol;
	  pSURFACE_FEATURE (pthis)->dimzx=*numrow;
	  pSURFACE_FEATURE (pthis)->dimzy= ncurv;
	}
	for (i=0;i < N;i++) 
	  pSURFACE_FEATURE (pthis)->pvecx[i] = tab[i];	
	k=N;
	for (i=0;i < N;i++) 
	  pSURFACE_FEATURE (pthis)->pvecy[i] = tab[k+i];
	k=k+ N;
	for (i=0;i < N;i++) 
	  pSURFACE_FEATURE (pthis)->pvecz[i] = tab[k+i];
      }
      else {
	sciprint ("Un handled data field\n");
	return -1;
      }
      return 0;
      break;
    case SCI_GRAYPLOT:
      if (pGRAYPLOT_FEATURE (pthis)->type == 0) { /* gray plot */
	double *pvecx,*pvecy,*pvecz;
	int nx,ny;
	nx=*numrow-1;
	ny=*numcol-1;
	if (pGRAYPLOT_FEATURE (pthis)->ny!=ny || pGRAYPLOT_FEATURE (pthis)->nx!=nx) {
	  if ((pvecx = calloc(nx,sizeof(double))) == NULL) {
	    sciprint ("Not enough memory\n");
	    return -1;}
	  if ((pvecy = calloc(ny,sizeof(double))) == NULL) {
	    FREE(pvecx);
	    sciprint ("Not enough memory\n");
	    return -1;}
	  if ((pvecz = calloc(nx*ny,sizeof(double))) == NULL) {
	    FREE(pvecx);FREE(pvecy);
	    sciprint ("Not enough memory\n");
	    return -1;}
	  FREE(pGRAYPLOT_FEATURE (pthis)->pvecx);pGRAYPLOT_FEATURE (pthis)->pvecx=pvecx;
	  FREE(pGRAYPLOT_FEATURE (pthis)->pvecy);pGRAYPLOT_FEATURE (pthis)->pvecy=pvecy;
	  FREE(pGRAYPLOT_FEATURE (pthis)->pvecz);pGRAYPLOT_FEATURE (pthis)->pvecz=pvecz;
	}
	for (i=0;i < nx;i++) 
	  pGRAYPLOT_FEATURE (pthis)->pvecx[i] = tab[i+1];

	for (i=0;i < ny;i++) 
	  pGRAYPLOT_FEATURE (pthis)->pvecy[i] = tab[*numrow*(i+1)];
	for (i=0;i < ny;i++) 
	  for (k=0;k < nx;k++) 
	    pGRAYPLOT_FEATURE (pthis)->pvecz[nx*i+k]=tab[*numrow*(i+1)+k+1];
	pGRAYPLOT_FEATURE (pthis)->ny=ny;
	pGRAYPLOT_FEATURE (pthis)->nx=nx;
      }
      else  {/* Matplot */
	double *pvecz;
	int nx,ny;
	nx=*numrow;
	ny=*numcol;
	if (pGRAYPLOT_FEATURE (pthis)->ny!=ny+1 || pGRAYPLOT_FEATURE (pthis)->nx!=nx+1) {
	  if ((pvecz = calloc(nx*ny,sizeof(double))) == NULL) {
	    sciprint ("Not enough memory\n");
	    return -1;}
	  FREE(pGRAYPLOT_FEATURE (pthis)->pvecz);pGRAYPLOT_FEATURE (pthis)->pvecz=pvecz;
	}
	for (i=0;i < nx*ny;i++) 
	  pGRAYPLOT_FEATURE (pthis)->pvecz[i]=tab[i];
	pGRAYPLOT_FEATURE (pthis)->ny=ny+1;
	pGRAYPLOT_FEATURE (pthis)->nx=nx+1;
      }
      break;
    case SCI_FEC: 
      {
      double *pvecx,*pvecy,*pfun;
      int Nnode;
      if (*numcol != 3) {
	sciprint ("The data must have 3 columns\n");
	return -1;}
      
      Nnode = *numrow;
      if (pFEC_FEATURE (pthis)->Nnode!=Nnode) {
	if ((pvecx = calloc(Nnode,sizeof(double))) == NULL) {
	  sciprint ("Not enough memory\n");
	  return -1;}
	if ((pvecy = calloc(Nnode,sizeof(double))) == NULL) {
	  sciprint ("Not enough memory\n");
	  FREE(pvecx);
	  return -1;}
	if ((pfun = calloc(Nnode,sizeof(double))) == NULL) {
	  sciprint ("Not enough memory\n");
	  FREE(pvecx);FREE(pvecy);
	  return -1;}
	FREE( pFEC_FEATURE (pthis)->pvecx); pFEC_FEATURE (pthis)->pvecx=pvecx;
	FREE( pFEC_FEATURE (pthis)->pvecy); pFEC_FEATURE (pthis)->pvecy=pvecy;
	FREE( pFEC_FEATURE (pthis)->pfun); pFEC_FEATURE (pthis)->pfun=pfun;
      }
      for (i=0;i < Nnode;i++) {
	pFEC_FEATURE (pthis)->pvecx[i]=tab[i];
	pFEC_FEATURE (pthis)->pvecy[i]=tab[Nnode+i];
	pFEC_FEATURE (pthis)->pfun[i]=tab[2*Nnode+i];
      }
      }
      break;
    case SCI_SBV:
    case SCI_SBH:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_LIGHT:    
    case SCI_AXIS:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    default:
      sciprint ("This object has no possibility to set points points Y\n");
      return -1;
      break;
    }
  return -1;
}


/*********************************************** CLICK FUNCTIONS ***********************************/

/**sciGetObjClicked
 * @memo returns the sons' entity wich is cliked on x,y coordinates. If none, then returns this entity
 */
sciPointObj 
 *sciGetObjClicked (sciPointObj *pthis,int x, int y)
{
  sciSons *sons;
  sciPointObj *result;

  /* 06/11/2002 */
  if (sciGetEntityType(pthis) != SCI_AGREG)/* We stop the recursivity at agregation */
    {
      sons = sciGetSons (pthis);
      while ((sons != (sciSons *) NULL) && (sons->pointobj != (sciPointObj *) NULL))
	{
	  result = sciGetObjClicked (sons->pointobj, x, y);

	  if (result == (sciPointObj *)NULL)
	    sons = sons->pnext;
	  else return result;
	}
    }
  if (sciIsClicked(pthis, x, y))
    return (sciPointObj *)pthis;
  else return (sciPointObj *)NULL;
  return (sciPointObj *)NULL;
}



/**sciIsClicked
 * @memo returns true if this entity wich is cliked on x,y coordinates. Here is the algorithm for click detection
 */
/* 06/11/2002 */
BOOL 
sciIsClicked(sciPointObj *pthis,int x, int y)
{
  double xtmp, ytmp, xtmp2, ytmp2;
  double DELTAX, DELTAY, vect[4];
  int n, xm[4];
  double *tab;
  int numrow, numcol, i;

  switch (sciGetEntityType (pthis))
    {
    case SCI_SUBWIN:
      xtmp = (sciGetWidth (pthis)*(pSUBWIN_FEATURE (pthis)->WRect[0]));
      xtmp2 = xtmp + (sciGetWidth (pthis)*(pSUBWIN_FEATURE (pthis)->WRect[2]));
      ytmp = (sciGetHeight(pthis)*(pSUBWIN_FEATURE (pthis)->WRect[1]));
      ytmp2 = ytmp + (sciGetHeight(pthis)*(pSUBWIN_FEATURE (pthis)->WRect[3]));
      if (
	  (x >= xtmp)
	  && (y >= ytmp)
	  && (x <= xtmp2)
	  && (y <= ytmp2)
	  )
	{
	  set_scale ("tttfff", pSUBWIN_FEATURE (pthis)->WRect, 
		     pSUBWIN_FEATURE (pthis)->FRect, NULL, "nn", NULL);
	  return TRUE;
	}
      else
	return FALSE;
      break;
    case SCI_ARC:
      /* on recupere la dimension de la sous fenetre parente */
      set_scale ("tttfff", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
		 pSUBWIN_FEATURE (sciGetParent(pthis))->FRect, NULL, "nn", NULL);
      DELTAX = fabs(0.01 * sciGetWidth (sciGetParent(pthis)));/* dimension in pixel */
      DELTAY = fabs(0.01 * sciGetHeight (sciGetParent(pthis)));/* dimension in pixel */
      n = 4;
      vect[0] = pARC_FEATURE (pthis)->x;
      vect[1] = pARC_FEATURE (pthis)->y;
      vect[2] = pARC_FEATURE (pthis)->width;
      vect[3] = pARC_FEATURE (pthis)->height;
      C2F(rect2d)(vect, xm, &n,"f2i");
      if (
	  ((abs(x - xm[0]) < DELTAX) && ((y - xm[1]) < DELTAY)) ||
	  ((abs(x - xm[0] - xm[2]) < DELTAX) && (abs(y - xm[1]) < DELTAY)) ||
	  ((abs(x - xm[0]) < DELTAX) && (abs(y - xm[1] - xm[3]) < DELTAY)) ||
	  ((abs(x - xm[0] - xm[2]) < DELTAX) && (abs(y - xm[1] - xm[3]) < DELTAY))
	  )
	{
	  return TRUE;
	}
      return FALSE;
      break;
    case SCI_AGREG:
      /* on recupere la dimension de la sous fenetre parente */
      set_scale ("tttfff", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
		 pSUBWIN_FEATURE (sciGetParent(pthis))->FRect, NULL, "nn", NULL);
      DELTAX = fabs(0.01 * sciGetWidth (sciGetParent(pthis)));/* dimension in pixel */
      DELTAY = fabs(0.01 * sciGetHeight (sciGetParent(pthis)));/* dimension in pixel */
      n=4;
      vect[0] = pAGREG_FEATURE (pthis)->xmin;
      vect[1] = pAGREG_FEATURE (pthis)->ymin;
      vect[2] = sciGetPosWidth(pthis);
      vect[3] = sciGetPosHeight(pthis);
      C2F(rect2d)(vect, xm, &n,"f2i");
      if (
	  ((abs(x - xm[0]) < DELTAX) && ((y - xm[1]) < DELTAY)) ||
	  ((abs(x - xm[0] - xm[2]) < DELTAX) && (abs(y - xm[1]) < DELTAY)) ||
	  ((abs(x - xm[0]) < DELTAX) && (abs(y - xm[1] - xm[3]) < DELTAY)) ||
	  ((abs(x - xm[0] - xm[2]) < DELTAX) && (abs(y - xm[1] - xm[3]) < DELTAY))
	  )
	{
	  return TRUE;
	}
      return FALSE;
      break;
    case SCI_RECTANGLE:
      /* on recupere la dimension de la sous fenetre parente */
      set_scale ("tttfff", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
		 pSUBWIN_FEATURE (sciGetParent(pthis))->FRect, NULL, "nn", NULL);
      DELTAX = fabs(0.01 * sciGetWidth (sciGetParent(pthis)));/* dimension in pixel */
      DELTAY = fabs(0.01 * sciGetHeight (sciGetParent(pthis)));/* dimension in pixel */
      n=4;
      vect[0] = pRECTANGLE_FEATURE (pthis)->x;
      vect[1] = pRECTANGLE_FEATURE (pthis)->y;
      vect[2] = pRECTANGLE_FEATURE (pthis)->width;
      vect[3] = pRECTANGLE_FEATURE (pthis)->height;
      C2F(rect2d)(vect, xm, &n,"f2i");
      if (
	  ((abs(x - xm[0]) < DELTAX) && ((y - xm[1]) < DELTAY)) ||
	  ((abs(x - xm[0] - xm[2]) < DELTAX) && (abs(y - xm[1]) < DELTAY)) ||
	  ((abs(x - xm[0]) < DELTAX) && (abs(y - xm[1] - xm[3]) < DELTAY)) ||
	  ((abs(x - xm[0] - xm[2]) < DELTAX) && (abs(y - xm[1] - xm[3]) < DELTAY))
	  )
	{
	  return TRUE;
	}
      return FALSE;
      break;
    case SCI_POLYLINE:
      set_scale ("tttfff", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
		 pSUBWIN_FEATURE (sciGetParent(pthis))->FRect, NULL, "nn", NULL);
      DELTAX = fabs(0.01 * sciGetWidth (sciGetParent(pthis)));/* dimension in pixel */
      DELTAY = fabs(0.01 * sciGetHeight (sciGetParent(pthis)));/* dimension in pixel */
      tab = sciGetPoint(pthis, &numrow, &numcol);
      i   = 0;
      while((i < numrow) && 
	    ((abs(XDouble2Pixel(tab[2*i]) - x) > DELTAX) || 
	     (abs(YDouble2Pixel(tab[2*i+1]) - y) > DELTAY)))
	{
	  i++;
	}
      if ((i < numrow)   && (abs(XDouble2Pixel(tab[2*i]) - x) < DELTAX) && 
	  (abs(YDouble2Pixel(tab[2*i+1]) - y) < DELTAY))
	{
	  FREE(tab);
	  return TRUE;
	}
      FREE(tab);
      return FALSE;
      break;
    case SCI_PATCH:
      set_scale ("tttfff", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
		 pSUBWIN_FEATURE (sciGetParent(pthis))->FRect, NULL, "nn", NULL);
      DELTAX = fabs(0.01 * sciGetWidth (sciGetParent(pthis)));/* dimension in pixel */
      DELTAY = fabs(0.01 * sciGetHeight (sciGetParent(pthis)));/* dimension in pixel */
      tab = sciGetPoint(pthis, &numrow, &numcol);
      i   = 0;
      while((i < numrow) && 
	    ((abs(XDouble2Pixel(tab[2*i]) - x) > DELTAX) || 
	     (abs(YDouble2Pixel(tab[2*i+1]) - y) > DELTAY)))
	i++;
      if ((i < numrow)   && (abs(XDouble2Pixel(tab[2*i]) - x) < DELTAX) && 
	  (abs(YDouble2Pixel(tab[2*i+1]) - y) < DELTAY))
	{
	  FREE(tab);
	  return TRUE;
	}
      FREE(tab);
      return FALSE;
      break;
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SURFACE:
    case SCI_AXIS:    
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_FIGURE:
    case SCI_SBV:
    case SCI_SBH:
    case SCI_LIGHT:
    default:
      return FALSE;
      break;
    }
  return FALSE;
}


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
      if ((pSUBWIN_FEATURE (pthis)->callback = calloc (len+1, sizeof (char))) == NULL)
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
      if ((pARC_FEATURE (pthis)->callback = calloc (len+1, sizeof (char))) == NULL)
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
      if ((pRECTANGLE_FEATURE (pthis)->callback = calloc (len+1, sizeof (char))) == NULL)
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
      if ((pSEGS_FEATURE (pthis)->callback = calloc (len+1, sizeof (char))) == NULL)
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
      if ((pFEC_FEATURE (pthis)->callback = calloc (len+1, sizeof (char))) == NULL)
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
      if ((pGRAYPLOT_FEATURE (pthis)->callback = calloc (len+1, sizeof (char))) == NULL)
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
      if ((pPOLYLINE_FEATURE (pthis)->callback = calloc (len+1, sizeof (char))) == NULL)
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
    case SCI_PATCH:
      if ((pPATCH_FEATURE (pthis)->callback = calloc (len+1, sizeof (char))) == NULL)
	{
	  sciprint("No more Memory allocation !\n");
	  return -1;
	}
      else 
	{
	  strncpy(pPATCH_FEATURE (pthis)->callback, code, len);
	  pPATCH_FEATURE (pthis)->callbacklen = len;
	  pPATCH_FEATURE (pthis)->callbackevent = mevent;
                       
	}
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SURFACE:
    case SCI_AXIS:    
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
    default:
      sciprint ("\r\n No Callback is associeted with this Entity");
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
    case SCI_PATCH:
      return (char *)(pPATCH_FEATURE (pthis)->callback);
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
    case SCI_AXIS:	
      return (char *)(pAXIS_FEATURE(pthis)->callback);
      break;
    case SCI_AXES:	
      return (char *)(pAXES_FEATURE(pthis)->callback);
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
      sciprint ("\r\nNo Callback is associeted with this Entity");
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
    case SCI_PATCH:
      return pPATCH_FEATURE (pthis)->callbackevent;
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
    case SCI_AXIS:	
      return pAXIS_FEATURE(pthis)->callbackevent;
      break;
    case SCI_AXES:	
      return pAXES_FEATURE(pthis)->callbackevent;
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
      sciprint ("\r\nNo Callback is associeted with this Entity");
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
    case SCI_PATCH:
      pPATCH_FEATURE (pthis)->callbackevent = mevent;
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
    case SCI_AXIS:	
      pAXIS_FEATURE(pthis)->callbackevent = mevent;
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
      sciprint ("\r\nNo Callback is associeted with this Entity");
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
    case SCI_PATCH:
      return pPATCH_FEATURE (pthis)->callbacklen;
      break;    
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SURFACE:
    case SCI_AXIS:   
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
      sciprint ("\r\nNo Callback is associeted with this Entity");
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
    case SCI_PATCH:
      pPATCH_FEATURE (pthis)->callbacklen = 0;
      pPATCH_FEATURE (pthis)->callbackevent = 100;
      FREE(pPATCH_FEATURE (pthis)->callback);
      pPATCH_FEATURE (pthis)->callback = NULL;
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SURFACE:
    case SCI_AXIS:    
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
    default:
      sciprint ("\r\nNo Callback is associeted with this Entity");
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
    case SCI_PATCH:
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
      else sciprint ("\r\nNo Callback is associeted with this Entity");
      return 0;
      break;
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SURFACE:
    case SCI_AXIS:    
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
    default:
      sciprint ("\r\nNo Callback is associeted with this Entity");
      return -1;
      break;
    }
  return -1;
}

/************************************ End of callback Functions ************************************/


/************************************ Agregation *******************************************/


/**sciConstructAgregation
 * @memo constructes an agregation of entities
 * @memo do only a association with a parent and a handle reservation !
 */
sciPointObj *
ConstructAgregation (long *handelsvalue, int number) /* Conflicting types with definition */
{
  sciSons *sons, *sonsnext;
  sciPointObj *pobj, *prevpparent;
  int i;
  double xtmp,xtmp1;

  prevpparent = sciGetParent(sciGetPointerFromHandle((long) handelsvalue[0]));

  /* we verify if entities are all basic */
  for (i=0;i<number;i++)
    {
      xtmp =  handelsvalue[i];
       
      switch (sciGetEntityType(sciGetPointerFromHandle(xtmp)))
	{
	case SCI_ARC:
	case SCI_RECTANGLE:
	case SCI_SEGS: 
	case SCI_FEC: 
	case SCI_GRAYPLOT: 
	case SCI_POLYLINE:
	case SCI_PATCH:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	case SCI_SURFACE:
	case SCI_AXIS:    
	case SCI_AXES:
	case SCI_LIGHT:
	  /* We verify  if the basics entities are not packed, and are all associated to a subwindow */
	  if (
	      (sciGetEntityType(sciGetParent(sciGetPointerFromHandle(xtmp))) != SCI_SUBWIN)
	      ||
	      (sciGetParent(sciGetPointerFromHandle(xtmp)) != prevpparent)
	      )
	    {
	      sciprint("entity number %d have to be a basic graphic entity, already packed or have a different parents !\n",(i+1));
	      return (sciPointObj *) NULL;
	    }
	  break;
	case SCI_AGREG:
	  /* We verify  if the agregation is packed, associated to a subwindow or have the same parent */
	  if (
	      (
	       (sciGetEntityType(sciGetParent(sciGetPointerFromHandle(xtmp))) != SCI_SUBWIN)
	       &&
	       (sciGetEntityType(sciGetParent(sciGetPointerFromHandle(xtmp))) != SCI_AGREG)
	       )
	      ||
	      (sciGetParent(sciGetPointerFromHandle(xtmp)) != prevpparent)
	      )
	    {
	      sciprint("entity number %d have to be a basic graphic entity, already packed or have a different parents !\n",(i+1));
	      return (sciPointObj *) NULL;
	    }
	  break;
	  /* we verify if the entity is not */
	case SCI_SUBWIN:
	case SCI_PANNER:
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
	case SCI_FIGURE:
	case SCI_SBV:
	case SCI_SBH:
	default:
	  sciprint("entity number %d have to be a basic graphic entity\n",(i+1));
	  return (sciPointObj *) NULL;
	}
      prevpparent = sciGetParent(sciGetPointerFromHandle(xtmp));
    }
  if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
    return (sciPointObj *) NULL;

  sciSetEntityType (pobj, SCI_AGREG);
  if ((pobj->pfeatures = MALLOC ((sizeof (sciAgreg)))) == NULL)
    return (sciPointObj *) NULL;

  if (sciAddNewHandle (pobj) == -1)
    {
      sciprint("no handle to allocate\n");
      return (sciPointObj *) NULL;
    }

  /* the parent of the agregation will be the parent of the sons entities */
  /*sciSetParent (pobj, (sciPointObj *)sciGetParent(
    sciGetPointerFromHandle((long) (stk(*value))[0])));*/
  if (!(sciAddThisToItsParent (pobj, (sciPointObj *)sciGetParent(
								 sciGetPointerFromHandle((long) handelsvalue[0])))))
    return NULL;

  sciSetCurrentSon (pobj, (sciPointObj *) NULL);
  pAGREG_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;

  pAGREG_FEATURE (pobj)->callback = (char *)NULL;
  pAGREG_FEATURE (pobj)->callbacklen = 0;
  pAGREG_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj));

  sonsnext = (sciSons *) NULL;

  /* initialisation with the first son */
  xtmp = (long) handelsvalue[0];
  pAGREG_FEATURE(pobj)->xmin   = sciGetPosX((sciPointObj *)sciGetPointerFromHandle(xtmp));
  pAGREG_FEATURE(pobj)->ymin   = sciGetPosY((sciPointObj *)sciGetPointerFromHandle(xtmp));
  pAGREG_FEATURE(pobj)->xmax   = sciGetPosX((sciPointObj *)sciGetPointerFromHandle(xtmp))
    + sciGetPosWidth((sciPointObj *)sciGetPointerFromHandle(xtmp));
  pAGREG_FEATURE(pobj)->ymax   = sciGetPosY((sciPointObj *)sciGetPointerFromHandle(xtmp))
    + sciGetPosHeight((sciPointObj *)sciGetPointerFromHandle(xtmp));

  for (i=0;i<number;i++)
    {
      if ((sons = MALLOC ((sizeof (sciSons)))) == NULL)
	return (sciPointObj *)NULL;
      if (i == 0)
	pAGREG_FEATURE(pobj)->relationship.plastsons = (sciSons *)sons;

      xtmp = handelsvalue[i];
      sons->pointobj      = sciGetPointerFromHandle(xtmp);

      /* Nous changeons le parent de l'entite qui devient alors l'agregation */
      if (sons->pointobj != NULL)
	{
	  sciDelThisToItsParent (sons->pointobj, sciGetParent(sons->pointobj));
	  sciAddThisToItsParent (sons->pointobj, pobj);
	}

      pAGREG_FEATURE(pobj)->xmax  = Max(pAGREG_FEATURE(pobj)->xmax,
					sciGetPosX(sons->pointobj) + sciGetPosWidth(sons->pointobj));
      pAGREG_FEATURE(pobj)->ymax =  Max(pAGREG_FEATURE(pobj)->ymax,
					sciGetPosY(sons->pointobj) + sciGetPosHeight(sons->pointobj));
      xtmp1 = Min(pAGREG_FEATURE(pobj)->xmin, sciGetPosX(sons->pointobj));
      pAGREG_FEATURE(pobj)->xmin = xtmp1;
      pAGREG_FEATURE(pobj)->ymin = Min(pAGREG_FEATURE(pobj)->ymin, sciGetPosY(sons->pointobj));

      sons->pprev         = (sciSons *)NULL;
      sons->pnext         = sonsnext;
      if (sonsnext)
	sonsnext->pprev = sons;
      sonsnext            = sons;
    }

  pAGREG_FEATURE(pobj)->relationship.psons = sons;

  /*  rect of the dimension of the agregation */
  pAGREG_FEATURE(pobj)->isselected = TRUE;

  return (sciPointObj *)pobj;
}



/**DestroyAgregation
 * @memo This function destroies the agregation and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 */
int
DestroyAgregation (sciPointObj * pthis)
{
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1) return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  return 0;
}


/**sciUnAgregation
 * @memo This function destroies the agregation and  and unpackes the elementaries structures to associates them to its parent
 */
int
sciUnAgregation (sciPointObj * pobj)
{
  sciPointObj *pparent, *pobjson;
  sciSons *psons = (sciSons *)NULL;
	
  if (sciGetEntityType(pobj) != SCI_AGREG)
    return -1;

  psons = sciGetSons(pobj);
  pobjson = psons->pointobj;
  pparent = sciGetParent(pobj);
  while ((psons != (sciSons *) NULL) && (pobjson != (sciPointObj *) NULL))
    {
      /* we delete this son to this */ 
      pobjson = psons->pointobj;
      psons = psons->pnext;
      sciDelThisToItsParent (pobjson, pobj);
      /* and link to its old parent */
      sciAddThisToItsParent (pobjson, pparent);
    }
  DestroyAgregation(pobj);
  return 0;
}

/************************************ End of Agregation *******************************************/


void sciInitObj(pointXGC)
     double *pointXGC; 
{ 
  struct BCG *InitScilabXgc; 
  static sciPointObj *mafigure;
  static sciPointObj *masousfen;

  InitScilabXgc=(struct BCG *)pointXGC;
  /*  si le ScilabXgc a été initialisé et s'il est different de celui pocedé par la figure courante
   *  construire une nouvelle fenetre et une sous fenetre 
   * une autre facon de comparer c'est de voir est ce que InitScilabXgc->mafigure n'est pas vide (!=NULL */
  if (tmpScilabXgc != InitScilabXgc)
    if ((mafigure = ConstructFigure (InitScilabXgc)) != NULL)
      {
	sciprint("\r\n une nouvelle figure");
	InitScilabXgc->mafigure = mafigure;
        InitScilabXgc->graphicsversion = 1;
	tmpScilabXgc = InitScilabXgc; 
	if ((masousfen = ConstructSubWin (mafigure, InitScilabXgc->CurWindow)) != NULL)
	  sciSetOriginalSubWin (mafigure, masousfen);
      }
}

sciPointObj *sciIsExistingFigure(value)
     int *value;
{
  sciHandleTab *hdl;
  sciPointObj  *pobj, *figure;

  figure= (sciPointObj *) NULL;
  hdl=pendofhandletab;
  while (hdl != NULL)
    {
      pobj=(sciPointObj *) sciGetPointerFromHandle (hdl->index);
      if (sciGetEntityType (pobj)== SCI_FIGURE) 
	{
          pFIGURE_FEATURE (pobj)->isselected = FALSE;
	  if ( sciGetNum ( pobj) == *value) 
            {  
              figure=pobj;
              pFIGURE_FEATURE (figure)->isselected = TRUE;
            }
	}
      hdl=hdl->pprev;
    }
  if (figure != (sciPointObj *) NULL)
    return figure;
  return  (sciPointObj *) NULL;    
}

void sciSwitchWindow(winnum)
     int *winnum; 
{ 
  struct BCG *CurXGC; 
  static sciPointObj *mafigure;
  static sciPointObj *masousfen;  
  integer v;
  double dv; 
  
  /* find if exist figure winnum */
  /* une autre methode c est de tester CurXGC->mafigure = NULL */
  if ( (sciPointObj *) sciIsExistingFigure(winnum) == (sciPointObj *) NULL) 
    {
      /** Figure winnum don't exist **/
      /** Create Figure **/ 
      C2F(dr)("xget","gc",&v,&v,&v,&v,&v,&v,(double *)&CurXGC,&dv,&dv,&dv,5L,10L);/* ????? SS*/
      if ((mafigure = ConstructFigure (CurXGC)) != NULL)
	{
	  CurXGC->mafigure = mafigure;
          CurXGC->graphicsversion = 1;
	  if ((masousfen = ConstructSubWin (mafigure, CurXGC->CurWindow)) != NULL)
	    sciSetOriginalSubWin (mafigure, masousfen);
	}
       
    } 
}


void sciRedrawFigure()
{
  sciSetReplay (TRUE);
  sciDrawObj (sciGetCurrentFigure ());
  sciSetReplay (FALSE);
   
}

void sciRedrawF(value)
     int *value;   
{ 
  sciPointObj *figure; 
  integer cur,na,verb=0;
 
  figure= (sciPointObj *) sciIsExistingFigure(value); 
  C2F (dr) ("xget", "window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,6L);
  C2F (dr) ("xset", "window",value,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,6L);
  sciSetCurrentObj(figure);
  sciSetReplay (TRUE);
  sciDrawObj (figure);
  sciSetReplay (FALSE);
  C2F (dr) ("xset", "window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,6L);
   
}

void sciXbasc()
{  
  static sciPointObj *mafigure;
  static sciPointObj *masousfen;  
 
  mafigure= (sciPointObj *) sciGetCurrentFigure(); 
  DestroyAllGraphicsSons((sciPointObj *)mafigure);
  
   
  if ((masousfen = ConstructSubWin (mafigure, 'o')) != NULL)
    sciSetOriginalSubWin (mafigure, masousfen);
  sciDrawObj(sciGetCurrentFigure ());       
} 	

void sciXclear()
{
  static sciPointObj *masousfen, *tmpsousfen;
  sciSons *psonstmp; 
     
  tmpsousfen= (sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure());
  psonstmp = sciGetSons (sciGetCurrentFigure());
  
  while (psonstmp != (sciSons *) NULL)	
    {  
      if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
	{
	  masousfen= (sciPointObj *)psonstmp->pointobj;
	  sciSetSelectedSubWin (masousfen);
	  sciSetdrawmode (FALSE); 
	}
      psonstmp = psonstmp->pnext;
    }
  sciSetSelectedSubWin (tmpsousfen);
  sciDrawObj(sciGetCurrentFigure ());      
}

void sciXdraw()
{
  static sciPointObj *masousfen, *tmpsousfen;
  sciSons *psonstmp; 
   
  tmpsousfen= (sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure ());
  psonstmp = sciGetSons (sciGetCurrentFigure ());
  
  
  while (psonstmp != (sciSons *) NULL)	
    {  
      if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
	{
          masousfen= (sciPointObj *)psonstmp->pointobj;
          sciSetSelectedSubWin (masousfen);
          sciSetdrawmode (TRUE); 
	}
      psonstmp = psonstmp->pnext;
    }
  sciSetSelectedSubWin (tmpsousfen); 
  sciDrawObj(sciGetCurrentFigure ());
           
}

BOOL
sciGetdrawmode (sciPointObj *pobj)
{
  static sciPointObj *subwin;

  subwin= (sciPointObj *) sciGetParentSubwin (pobj);
  if  (subwin != (sciPointObj *) NULL) 
    return pSUBWIN_FEATURE(subwin)->visible ;
  return FALSE;
}

int
sciSetdrawmode (BOOL mode)
{ 
  static sciPointObj *pobj;

  pobj= (sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure ());
  pSUBWIN_FEATURE(pobj)->visible = mode ;	
  sciDrawObj(sciGetCurrentFigure ());
  return 0;
}

int Objmove (hdl,x,y,opt)
     long *hdl;
     double x,y;
     BOOL opt;
{   
  long tmphdl;
  sciPointObj *pobj;  
  sciSons *psonstmp;
  int i;

  pobj = (sciPointObj *)sciGetPointerFromHandle(*hdl);
  sciSetCurrentObj (pobj);
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
      break;
    case SCI_RECTANGLE: 
      pRECTANGLE_FEATURE(pobj)->x += x;  
      pRECTANGLE_FEATURE(pobj)->y += y; 
      break; 
    case SCI_AGREG: 
      psonstmp = sciGetSons((sciPointObj *) pobj);
      while ((psonstmp != (sciSons *)NULL) && (psonstmp->pointobj != (sciPointObj *)NULL))
	{
	  tmphdl =sciGetHandle((sciPointObj *)psonstmp->pointobj);
	  Objmove (&tmphdl,x,y,opt);
	  psonstmp = psonstmp->pnext;
	}
      break;
    case SCI_TEXT:  
      pTEXT_FEATURE(pobj)->x += x; 
      pTEXT_FEATURE(pobj)->y += y;
      break;
    case SCI_SEGS:   
      for (i=0;i<pSEGS_FEATURE(pobj)->Nbr1;i++) {
	pSEGS_FEATURE(pobj)->vx[i] += x; 
	pSEGS_FEATURE(pobj)->vy[i] += y;
      }
      break;
    case SCI_POLYLINE: 
      for (i=0;i<pPOLYLINE_FEATURE(pobj)->n1;i++) {
	pPOLYLINE_FEATURE(pobj)->pvx[i] += x; 
	pPOLYLINE_FEATURE(pobj)->pvy[i] += y;
      }
      break;
    case SCI_PATCH:
      for (i=0;i<pPATCH_FEATURE(pobj)->n;i++) {
	pPATCH_FEATURE(pobj)->pvx[i] += x; 
	pPATCH_FEATURE(pobj)->pvy[i] += y;
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
    case SCI_FIGURE:
    case SCI_AXIS:        
    case SCI_SURFACE:    
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
    default:
      sciprint ("This object can not be moved\r\n");
      return -1;
      break;
    }    
  if (opt)
    sciDrawObj(pobj);
  else
    sciDrawObj(sciGetParentFigure(sciGetCurrentObj ()));
  return 0;
}

int sciType (marker)
     char *marker;
{ 
  if      (strncmp(marker,"background", 10) == 0) { return 1;}	
  else if (strncmp(marker,"foreground", 10) == 0) {return 1;}	
  else if (strncmp(marker,"fill_mode", 9) == 0)   {return 10;}		
  else if (strncmp(marker,"thickness", 9) == 0)   {return 1;}
  else if (strncmp(marker,"line_style", 10) == 0) {return 1;}		
  else if (strncmp(marker,"mark_style", 10) == 0) {return 1;}	
  else if (strcmp(marker,"mark_size") == 0) {return 1;}	
  else if (strncmp(marker,"mark_mode", 9) == 0)   {return 10;}	
  else if (strncmp(marker,"figure_position", 15) == 0) {return 1;}	 
  else if (strncmp(marker,"axes_size", 9) == 0)   {return 1;}
  else if (strncmp(marker,"axes_visible", 12) == 0)   {return 10;}	
  else if (strncmp(marker,"figure_size", 11) == 0){return 1;}	
  else if (strncmp(marker,"figure_id", 9) == 0)   {return 1;}	
  else if (strncmp(marker,"figure_name", 11) == 0){return 10;}   
  else if (strncmp(marker,"figures_id", 10) == 0)   {return 1;}
  else if (strncmp(marker,"polyline_style", 14) == 0){return 1;} 
  else if (strncmp(marker,"font_size", 9) == 0)   {return 1;}	
  else if (strncmp(marker,"font_angle", 10) == 0) {return 1;}		
  else if (strncmp(marker,"font_foreground", 15) == 0){return 1;}	
  else if (strncmp(marker,"font_style", 10) == 0) {return 1;}	      
  else if (strncmp(marker,"font_name", 9) == 0)   {return 10;}
  else if (strncmp(marker,"textcolor", 9) == 0)   {return 1;}
  else if (strcmp(marker,"labels_font_size") == 0)   {return 1;}
  else if (strcmp(marker,"labels_font_color") == 0)   {return 1;}
  else if (strncmp(marker,"text", 4) == 0)        {return 10;}	 
  else if (strncmp(marker,"old_style", 9) == 0)   {return 10;}
  else if (strncmp(marker,"figure_style", 12) == 0)   {return 10;}        
  else if (strncmp(marker,"visible", 7) == 0)     {return 10;} 
  else if (strncmp(marker,"auto_resize", 10) == 0){return 10;}
  else if (strncmp(marker,"pixel_drawing_mode", 18) == 0)    {return 10;}    
  else if (strncmp(marker,"default_values", 14) == 0) {return 10;} 
  else if (strncmp(marker,"color_map", 9) == 0)   {return 1;}    
  else if (strncmp(marker,"x_location", 10) == 0) {return 10;} 
  else if (strncmp(marker,"y_location", 10) == 0) {return 10;}   
  else if (strncmp(marker,"tics_direction", 14) == 0)  {return 10;}   
  else if (strncmp(marker,"tight_limits", 12) == 0){return 10;} 
  else if (strncmp(marker,"box", 3) == 0)         {return 10;}
  else if (strncmp(marker,"tics_color", 10) == 0) {return 1;}	 
  else if (strncmp(marker,"tics_textcolor", 14) == 0) {return 1;}	  
  else if (strncmp(marker,"tics_textsize", 13) == 0)  {return 1;}
  else if (strncmp(marker,"xtics_coord", 11) == 0){return 1;}	
  else if (strncmp(marker,"ytics_coord", 11) == 0){return 1;}	 
  else if (strncmp(marker,"grid", 4) == 0)        {return 1;}   
  else if (strncmp(marker,"tics_segment", 12) == 0) {return 10;} 
  else if (strncmp(marker,"tics_style", 10) == 0) {return 10;} 
  else if (strncmp(marker,"format_n", 9) == 0)    {return 10;}    
  else if (strncmp(marker,"tics_labels", 11) == 0){return 10;}  
  else if (strncmp(marker,"sub_tics", 8) == 0)    {return 1;}	
  else if (strncmp(marker,"zoom_box", 8) == 0)    {return 1;}	
  else if (strncmp(marker,"zoom_state", 9) == 0)  {return 10;}  
  else if (strncmp(marker,"clip_box", 8) == 0)    {return 1;}	
  else if (strncmp(marker,"clip_state", 9) == 0)  {return 10;} 
  else if (strncmp(marker,"auto_clear", 10) == 0) {return 10;}		
  else if (strncmp(marker,"auto_scale", 10) == 0) {return 10;}		  	 
  else if (strncmp(marker,"arrow_size", 10) == 0) {return 1;}
  else if (strncmp(marker,"segs_color", 10) == 0) {return 1;}
  else if (strncmp(marker,"colored", 7) == 0) {return 10;}
  else if (strcmp(marker,"data") == 0)            {return 1;}	
  else if (strncmp(marker,"hdl", 3) == 0)         {return 1;}		
  else if (strncmp(marker,"callbackmevent", 14) == 0) {return 1;}
  else if (strncmp(marker,"callback", 8) == 0)    {return 10;} 	
  else if (strncmp(marker,"log_flags", 9) == 0)   {return 10;}
  else if (strcmp(marker,"data_mapping") == 0)    {return 10;}
  else if (strcmp(marker,"surface_color") == 0)    {return 1;}
  else if (strcmp(marker,"rotation_angles") == 0)    {return 1;}
  else if (strcmp(marker,"flag") == 0)    {return 1;}
  else if (strcmp(marker,"color_flag") == 0)    {return 1;}
  else if (strcmp(marker,"axes_bounds") == 0)    {return 1;}
  else if (strcmp(marker,"data_bounds") == 0)    {return 1;}
  else if (strcmp(marker,"surface_color") == 0)    {return 1;}
  else if (strcmp(marker,"rotation_style") == 0)    {return 10;}
  else if (strcmp(marker,"triangles") == 0)    {return 1;}
  else if (strcmp(marker,"z_bounds") == 0)    {return 1;}
  else if (strcmp(marker,"current_axes") == 0)    {return 9;}
  else if (strcmp(marker,"children") == 0)    {return 9;}

  else { sciprint("\r\n Unknown property \r");return 0;}
}
/**sciGetAxes
 * @memo Gets SUBWIN of this Scilab graphic figure
 * @author Djalel ABDEMOUCHE
 */
sciPointObj *
sciGetAxes (pparentfigure,psubwin)
     sciPointObj *pparentfigure;
     sciPointObj *psubwin;
{
  sciSons *psonstmp; 
  
  psonstmp = sciGetSons (pparentfigure);
  
  
  if (psonstmp != (sciSons *) NULL)	
    {  
      /* tant que le fils ne corespond pas a l'entite */
      while ((psonstmp->pnext  != (sciSons *) NULL)
	     && (sciGetEntityType (psonstmp->pointobj) != SCI_SUBWIN))
	psonstmp = psonstmp->pnext;
      
      if  (psonstmp->pnext  == (sciSons *) NULL)
	return (sciPointObj *) NULL;
      else 
	if (sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
	  return (sciPointObj *)psonstmp->pointobj;
	else
	  return (sciPointObj *) NULL; /* added SS ????*/
    }
  else
    return (sciPointObj *) NULL;
} 

void initsubwin()
{
  sciPointObj *psubwin;
  char dir;

  Cscale2default(); 
  psubwin=sciGetSelectedSubWin (sciGetCurrentFigure ());
  
  pSUBWIN_FEATURE (psubwin)->WRect[0]   = 0;
  pSUBWIN_FEATURE (psubwin)->WRect[1]   = 0;
  pSUBWIN_FEATURE (psubwin)->WRect[2]   = 1;
  pSUBWIN_FEATURE (psubwin)->WRect[3]   = 1;

  pSUBWIN_FEATURE (psubwin)->FRect[0]   = 0;
  pSUBWIN_FEATURE (psubwin)->FRect[1]   = 0;
  pSUBWIN_FEATURE (psubwin)->FRect[2]   = 1;
  pSUBWIN_FEATURE (psubwin)->FRect[3]   = 1; 
            
  dir= 'd'; pSUBWIN_FEATURE (psubwin)->axes.xdir=dir;
  dir= 'l'; pSUBWIN_FEATURE (psubwin)->axes.ydir=dir;
  pSUBWIN_FEATURE (psubwin)->axes.rect = 1;  
  pSUBWIN_FEATURE (psubwin)->axes.ticscolor = -1;
  pSUBWIN_FEATURE (psubwin)->axes.textcolor = -1;
  pSUBWIN_FEATURE (psubwin)->axes.fontsize  = -1;
  pSUBWIN_FEATURE (psubwin)->axes.subint[0] =  1;
  pSUBWIN_FEATURE (psubwin)->axes.subint[1] =  1;
  pSUBWIN_FEATURE (psubwin)->axes.limits[0]  = 0;  
  pSUBWIN_FEATURE (psubwin)->visible = TRUE;   
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

void DeleteObjs(integer win_num)
{
  sciPointObj *figure;
  struct BCG *Xgc;

  figure = (sciPointObj *)  sciIsExistingFigure(&win_num);
  if (  figure != (sciPointObj *) NULL )
    {
      Xgc = (struct BCG *) pFIGURE_FEATURE(figure)->pScilabXgc;
      DestroyAllGraphicsSons (figure);
      DestroyFigure (figure);
      Xgc->mafigure = (sciPointObj *) NULL;
    }
}

void scizoom(bbox)
     double bbox[4];
{
  sciPointObj *psousfen;
  double fmin,fmax,lmin,lmax;
  integer min,max,puiss,deux=2,dix=10;
 
  psousfen= (sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure());


  if (!(sciGetZooming(psousfen)))
    {
      sciSetZooming(psousfen, 1);
      pSUBWIN_FEATURE (psousfen)->FRect_kp[0]   = pSUBWIN_FEATURE (psousfen)->FRect[0];
      pSUBWIN_FEATURE (psousfen)->FRect_kp[1]   = pSUBWIN_FEATURE (psousfen)->FRect[1];
      pSUBWIN_FEATURE (psousfen)->FRect_kp[2]   = pSUBWIN_FEATURE (psousfen)->FRect[2];
      pSUBWIN_FEATURE (psousfen)->FRect_kp[3]   = pSUBWIN_FEATURE (psousfen)->FRect[3];
    }
  /** regraduation de l'axe des axes ***/
  fmin=  bbox[0];
  fmax=  bbox[2];
  C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
  pSUBWIN_FEATURE(psousfen)->axes.xlim[2]=puiss;
  pSUBWIN_FEATURE (psousfen)->FRect[0]=lmin;
  pSUBWIN_FEATURE (psousfen)->FRect[2]=lmax;


  fmin= bbox[1]; fmax= bbox[3];
  C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
  pSUBWIN_FEATURE(psousfen)->axes.ylim[2]=puiss;
  pSUBWIN_FEATURE (psousfen)->FRect[1]=lmin;
  pSUBWIN_FEATURE (psousfen)->FRect[3]=lmax;
  /*****/

}

void Zoom_Subwin(bbox)
     double bbox[4];
{   
  sciPointObj *psousfen;
  sciSons *psonstmp;  
  double fmin,fmax,lmin,lmax;  
  integer min,max,puiss,deux=2,dix=10;
  integer box[4],box1[4],section[4];

  box[0]= Min(XDouble2Pixel(bbox[0]),XDouble2Pixel(bbox[2])); 
  box[2]= Max(XDouble2Pixel(bbox[0]),XDouble2Pixel(bbox[2]));
  box[1]= Min(YDouble2Pixel(bbox[1]),YDouble2Pixel(bbox[3])); 
  box[3]= Max(YDouble2Pixel(bbox[1]),YDouble2Pixel(bbox[3]));
        
  psonstmp = sciGetSons (sciGetCurrentFigure());
  while (psonstmp != (sciSons *) NULL) {  
    if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN) {
      psousfen= (sciPointObj *)psonstmp->pointobj;
      sciSetSelectedSubWin(psousfen);
      box1[0]= Cscale.WIRect1[0]; 
      box1[2]= Cscale.WIRect1[2]+Cscale.WIRect1[0];
      box1[1]= Cscale.WIRect1[1]; 
      box1[3]= Cscale.WIRect1[3]+Cscale.WIRect1[1];
                 
      if (sciIsAreaZoom(box,box1,section)) { 
        bbox[0]= Min(XPixel2Double(section[0]),XPixel2Double(section[2])); 
        bbox[2]= Max(XPixel2Double(section[0]),XPixel2Double(section[2]));
        bbox[1]= Min(YPixel2Double(section[1]),YPixel2Double(section[3])); 
        bbox[3]= Max(YPixel2Double(section[1]),YPixel2Double(section[3])); 
        if (!(sciGetZooming(psousfen))) {
          sciSetZooming(psousfen, 1);
          pSUBWIN_FEATURE (psousfen)->FRect_kp[0]   = pSUBWIN_FEATURE
	    (psousfen)->FRect[0];
          pSUBWIN_FEATURE (psousfen)->FRect_kp[1]   = pSUBWIN_FEATURE
	    (psousfen)->FRect[1];
          pSUBWIN_FEATURE (psousfen)->FRect_kp[2]   = pSUBWIN_FEATURE
	    (psousfen)->FRect[2];
          pSUBWIN_FEATURE (psousfen)->FRect_kp[3]   = pSUBWIN_FEATURE
	    (psousfen)->FRect[3];
        }
                      
        /** regraduation de l'axe ***/
        fmin=  bbox[0]; 
        fmax=  bbox[2];
        C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
        pSUBWIN_FEATURE(psousfen)->axes.xlim[2]=puiss; 
        pSUBWIN_FEATURE (psousfen)->FRect[0]=lmin;  
        pSUBWIN_FEATURE (psousfen)->FRect[2]=lmax; 
        fmin= bbox[1]; fmax= bbox[3];   
        C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
        pSUBWIN_FEATURE(psousfen)->axes.ylim[2]=puiss;
        pSUBWIN_FEATURE (psousfen)->FRect[1]=lmin;  
        pSUBWIN_FEATURE (psousfen)->FRect[3]=lmax; 
      }
    }     
    psonstmp = psonstmp->pnext;
  }

  sciSetSelectedSubWin((sciPointObj *) sciGetSelectedSubWin
		       (sciGetCurrentFigure()));
  sciSetReplay(1);
  sciDrawObj(sciGetCurrentFigure());
  sciSetReplay(0);
}

void Unzoom_Subwin()
{
  double fmin,fmax,lmin,lmax;  
  integer min,max,puiss,deux=2,dix=10;
  sciPointObj *psousfen;
  sciSons *psonstmp;  

  psonstmp = sciGetSons (sciGetCurrentFigure());
  while (psonstmp != (sciSons *) NULL) {  
    if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN) {
      psousfen= (sciPointObj *)psonstmp->pointobj;
      if (sciGetZooming(psousfen)) {
        sciSetZooming(psousfen, 0);
        pSUBWIN_FEATURE (psousfen)->FRect[0]   = pSUBWIN_FEATURE
	  (psousfen)->FRect_kp[0];
        pSUBWIN_FEATURE (psousfen)->FRect[1]   = pSUBWIN_FEATURE
	  (psousfen)->FRect_kp[1];
        pSUBWIN_FEATURE (psousfen)->FRect[2]   = pSUBWIN_FEATURE
	  (psousfen)->FRect_kp[2];
        pSUBWIN_FEATURE (psousfen)->FRect[3]   = pSUBWIN_FEATURE
	  (psousfen)->FRect_kp[3];
      }
          
      /**axes  regraduation  ***/
      fmin= pSUBWIN_FEATURE (psousfen)->FRect[0];
      fmax= pSUBWIN_FEATURE (psousfen)->FRect[2]; 
      C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
      pSUBWIN_FEATURE(psousfen)->axes.xlim[2]=puiss; 
          
      fmin= pSUBWIN_FEATURE (psousfen)->FRect[1];
      fmax= pSUBWIN_FEATURE (psousfen)->FRect[3]; 
      C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
      pSUBWIN_FEATURE(psousfen)->axes.ylim[2]=puiss;
    }     
    psonstmp = psonstmp->pnext;
  } 
        
  sciSetReplay(1);
  sciDrawObj(sciGetCurrentFigure());
  sciSetReplay(0);
}

/**sciGetIdFigure
 */
void
sciGetIdFigure (int *vect, int *id)
{
  sciHandleTab *hdl;
  sciPointObj  *pobj;

  hdl = pendofhandletab;
  while (hdl != NULL)
    { 
      pobj=(sciPointObj *) sciGetPointerFromHandle (hdl->index);
      if (sciGetEntityType(pobj) == SCI_FIGURE)
	{
	  vect[*id] = sciGetNum(pobj);
	  (*id)++;
	} 
        
      hdl = hdl->pprev;
    }
}
int version_flag() 
{ 
  double *XGC,dv=0;
  struct BCG *CurrentScilabXgc; 
  int v=0;

  C2F(dr)("xget","gc",&v,&v,&v,&v,&v,&v,(double *)&XGC,&dv,&dv,&dv,5L,10L); /* ajout cast ???*/
  CurrentScilabXgc=(struct BCG *)XGC;
  if (CurrentScilabXgc==(struct BCG *)NULL) return 1;

  return (CurrentScilabXgc->graphicsversion == 0) ? 1 : 0; 
}

struct BCG *
sciGetCurrentScilabXgc ()
{ 
  integer v=0;
  double *XGC, dv=0;
  struct BCG *CurrentScilabXgc; 
 
  C2F(dr)("xget","gc",&v,&v,&v,&v,&v,&v,(double *)&XGC,&dv,&dv,&dv,5L,10L); /* ajout cast ???*/
  CurrentScilabXgc=(struct BCG *)XGC;
  return (struct BCG *) CurrentScilabXgc;
}

extern void Obj_RedrawNewAngle(sciPointObj *psubwin,double theta,double alpha)
{
  sciPointObj *pobj;
  
  pobj= (sciPointObj *) sciGetSurface(psubwin);
  if(pobj != (sciPointObj *) NULL)	
    {   
      pSURFACE_FEATURE (pobj)->theta   = theta;
      pSURFACE_FEATURE (pobj)->alpha   = alpha;
    }
}
extern BOOL Check3DObjs()
{  
  sciPointObj *pobj;
  sciSons *psonstmp;

  psonstmp = sciGetSons (sciGetCurrentFigure()); 
  while (psonstmp != (sciSons *) NULL)	
    {  
      if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN) 
	{
	  if ((pobj= sciGetSurface(psonstmp->pointobj)) != (sciPointObj *) NULL)
	    return TRUE;
	}
      psonstmp = psonstmp->pnext;
    } 
  return FALSE;
}
extern sciPointObj *CheckClickedSubwin(integer x, integer y)
{ 
  integer box[4]; 
  sciSons *psonstmp;
  sciPointObj *psubwin; 
 
  psonstmp = sciGetSons (sciGetCurrentFigure());  
  while (psonstmp != (sciSons *) NULL)	
    {  
      if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN) 
	{
	  psubwin= (sciPointObj *) psonstmp->pointobj;
	  sciSetSelectedSubWin(psubwin); 
	  box[0]= Cscale.WIRect1[0]; 
	  box[2]= Cscale.WIRect1[2]+Cscale.WIRect1[0];
	  box[1]= Cscale.WIRect1[1]; 
	  box[3]= Cscale.WIRect1[3]+Cscale.WIRect1[1];
	  if ((x >= box[0]) && (x <= box[2]) && (y >= box[1]) && (y <= box[3])) 
	    return (sciPointObj *) psubwin;                
	}
      psonstmp = psonstmp->pnext;
    } 
  return (sciPointObj *) NULL;                
}

 
extern sciPointObj *sciGetSurface(sciPointObj *psubwin)
{
  sciSons *psonstmp;
  
  psonstmp = sciGetSons (psubwin);
  while (psonstmp != (sciSons *) NULL)	
    {   
      if(sciGetEntityType (psonstmp->pointobj) == SCI_SURFACE) 
	return (sciPointObj *) psonstmp->pointobj;
      psonstmp = psonstmp->pnext;
    }
  return (sciPointObj *) NULL;
}
