/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library fonctions
 *    Matthieu PHILIPPE, INRIA 1999-2001
 *    Djalel ABDEMOUCHE, INRIA 2001-2004
 --------------------------------------------------------------------------*/

#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>


#define spINSIDE_SPARSE /* F.Leray to have LARGEST_REAL*/
#if defined(THINK_C) || defined (__MWERKS__)
#include "::sparse:spConfig.h" 
#else
#include "../sparse/spConfig.h"
#endif

#include "bcg.h"
#include "PloEch.h" 

/* DJ.A 2003 */
#ifdef __STDC__
void wininfo(char *format,...);
#else
/*VARARGS0*/
void wininfo();
#endif

/*F.Leray : Format pour imprimer un nombre de la forme k10^a ; cf. Axes.c*/
extern void NumberFormat __PARAMS((char *str,integer k,integer a));
extern void C2F(plot3dn)(sciPointObj *pobj, 
			 double *x, double *y, double *z, 
			 integer *p, integer *q);

extern void C2F(fac3dn)(sciPointObj *pobj, 
			double *x, double *y, double *z, 
			integer *cvect, integer *p, integer *q);
/**DJ.Abdemouche 2003**/
#define TX3D(x1,y1,z1) inint(xz1= Cscale.Wscx1*(TRX(x1,y1,z1)-Cscale.frect[0]) +Cscale.Wxofset1);
#define TY3D(x1,y1,z1) inint(yz1= Cscale.Wscy1*(-TRY(x1,y1,z1)+Cscale.frect[3])+Cscale.Wyofset1);
extern void GPopupResize();
struct BCG *tmpScilabXgc;
int cmptclip=0;
sciPointObj *pfiguremdl = (sciPointObj *) NULL;/* DJ.A 08/01/04 */
sciPointObj *paxesmdl = (sciPointObj *) NULL;/* DJ.A 08/01/04 */
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
extern int cf_type; /* used by gcf to determine if current figure is a graphic (1) or a tksci (0) one */
/*************************************************************************************************/
/* DJ.A 08/01/04 */
static double xz1,yz1;
unsigned short defcolors[] = {
  0,   0,   0, /* Black: DEFAULTBLACK */
  0,   0, 255, /* Blue */
  0, 255,   0, /* Green */
  0, 255, 255, /* Cyan */
  255,   0,   0, /* Red */
  255,   0, 255, /* Magenta */
  255,   255,   0, /* Yellow */
  255, 255, 255, /* White: DEFAULTWHITE */
  0,   0, 144, /* Blue4 */
  0,   0, 176, /* Blue3 */
  0,   0, 208, /* Blue2 */
  135, 206, 255, /* LtBlue */
  0, 144,   0, /* Green4 */
  0, 176,   0, /* Green3 */
  0, 208,   0, /* Green2 */
  0, 144, 144, /* Cyan4 */
  0, 176, 176, /* Cyan3 */
  0, 208, 208, /* Cyan2 */
  144,   0,   0, /* Red4 */
  176,   0,   0, /* Red3 */
  208,   0,   0, /* Red2 */
  144,   0, 144, /* Magenta4 */
  176,   0, 176, /* Magenta3 */
  208,   0, 208, /* Magenta2 */
  128,  48,   0, /* Brown4 */
  160,  64,   0, /* Brown3 */
  192,  96,   0, /* Brown2 */
  255, 128, 128, /* Pink4 */
  255, 160, 160, /* Pink3 */
  255, 192, 192, /* Pink2 */
  255, 224, 224, /* Pink */
  255, 215,   0  /* Gold */
};

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
static sciPointObj *pcurrentpobj = (sciPointObj *) NULL; /* test extern instead of static*/

/**
 * This table is memory of the clipping associated to the objects  
 */
extern sciClipTab ptabclip[15];
extern double C2F(dsort)();/*DJ.A merge*/ 
static char error_message[70]; /* DJ.A 08/01/04 */

#ifdef WIN32
static int flag_DO; /* F.Leray 16.02.04 flag global pour la fonction récursive DrawObj*/
#endif
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
    case SCI_RECTANGLE:
      return (sciRectangle *) pRECTANGLE_FEATURE (pobj);
      break;
    case SCI_SURFACE:
      return (sciSurface *) pSURFACE_FEATURE (pobj);
      break;
    case SCI_LIGHT:
      return (sciLightSource *) pLIGHT_FEATURE (pobj);
      break;
      /*   case SCI_AXIS
	   return (sciAxis *) pAXIS_FEATURE (pobj);
	   break;*/
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
    case SCI_MERGE:
      return (sciMerge *) pMERGE_FEATURE (pobj);
      break;
    case SCI_LABEL:
      return (sciLabel *) pLABEL_FEATURE (pobj);
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
      return (pSEGS_FEATURE (pobj)->ptype == 0) ?  "Segs": "Champ";
      break; 
    case SCI_FEC: 
      return "Fec";
      break;
    case SCI_GRAYPLOT:
      return (pGRAYPLOT_FEATURE (pobj)->type ==0) ? "Grayplot":"Matplot";
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
	  break;/* DJ.A 2003 */
	case SCI_CONTOUR:
	default:
	  break;
	}
      break;
    case SCI_LIGHT:
      return "Light";
      break;
      /*   case SCI_AXIS
	   return "Error";
	   break;*/
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
    case SCI_MERGE:
      return "Merge";
      break;
    case SCI_LABEL: /* F.Leray 27.05.04 */
      return "Label";
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
	/*   case SCI_AXIS  */
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
	(sciGetRelationship (pobj))->phandle = pvalue;		/** put the new index handle */
	break;
      default:
	break;
      }
}


sciHandleTab *
sciGetpendofhandletab()
{
  return pendofhandletab;
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
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS  */
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
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS  */
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
      phandletab = pendofhandletab;
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
      /*   case SCI_AXIS
	   return  &(pAXIS_FEATURE (pobj)->graphiccontext);
	   break; */
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
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

  /* F.Leray debug 30.03.04*/
  /*
  sciGraphicContext * psciGC_pobj = sciGetGraphicContext(pobj); 
  sciGraphicContext * psciGC_paxe = sciGetGraphicContext(paxesmdl);
  sciGraphicContext * psciGC_pfig = sciGetGraphicContext(pfiguremdl);
  */
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      if (pobj == pfiguremdl)
	{
	  (sciGetGraphicContext(pobj))->backgroundcolor = -3; /*33;*/  /* F.Leray 29.03.04: Wrong index here: 32+1 (old method) must be changed to -1 new method*/
	  (sciGetGraphicContext(pobj))->foregroundcolor = -2; /*32;*/  /* F.Leray 29.03.04: Wrong index here: 32+2 (old method) must be changed to -2 new method*/
	  (sciGetGraphicContext(pobj))->fillstyle = HS_HORIZONTAL;
	  (sciGetGraphicContext(pobj))->fillcolor = (sciGetGraphicContext(pobj))->backgroundcolor;
	  (sciGetGraphicContext(pobj))->linewidth = 1;
	  (sciGetGraphicContext(pobj))->linestyle = PS_SOLID;
	  (sciGetGraphicContext(pobj))->ismark    = FALSE;
	  (sciGetGraphicContext(pobj))->markstyle = 0;
	  
	}
      else
	{
	  (sciGetGraphicContext(pobj))->backgroundcolor = (sciGetGraphicContext(pfiguremdl))->backgroundcolor;
	  (sciGetGraphicContext(pobj))->foregroundcolor = (sciGetGraphicContext(pfiguremdl))->foregroundcolor;
	  (sciGetGraphicContext(pobj))->fillstyle = (sciGetGraphicContext(pfiguremdl))->fillstyle;
	  (sciGetGraphicContext(pobj))->fillcolor = (sciGetGraphicContext(pfiguremdl))->fillcolor;
	  (sciGetGraphicContext(pobj))->linewidth = (sciGetGraphicContext(pfiguremdl))->linewidth;
	  (sciGetGraphicContext(pobj))->linestyle = (sciGetGraphicContext(pfiguremdl))->linestyle;
	  (sciGetGraphicContext(pobj))->ismark    = (sciGetGraphicContext(pfiguremdl))->ismark ;
	  (sciGetGraphicContext(pobj))->markstyle = (sciGetGraphicContext(pfiguremdl))->markstyle;
	}  
      return 0;
      break;
    case SCI_SUBWIN:
      if (pobj == paxesmdl)
	{
	  (sciGetGraphicContext(pobj))->backgroundcolor =	-3;  /* F.Leray 09.04.04 POSSIBLE DISCUSSION HERE : may it be (sciGetGraphicContext(sciGetParent(pobj)))->backgroundcolor ??*/
	  (sciGetGraphicContext(pobj))->foregroundcolor =	-2;  /* HERE too : may it be  (sciGetGraphicContext(sciGetParent(pobj)))->foregroundcolor ??*/
	  (sciGetGraphicContext(pobj))->fillstyle =	sciGetFillStyle (sciGetParent (pobj));
	  (sciGetGraphicContext(pobj))->fillcolor = (sciGetGraphicContext(pobj))->backgroundcolor;
	  (sciGetGraphicContext(pobj))->linewidth =	sciGetLineWidth (sciGetParent (pobj));
	  (sciGetGraphicContext(pobj))->linestyle =	sciGetLineStyle (sciGetParent (pobj));
	  (sciGetGraphicContext(pobj))->ismark    =	sciGetIsMark (sciGetParent (pobj));
	  (sciGetGraphicContext(pobj))->markstyle =	sciGetMarkStyle (sciGetParent (pobj));
	}
      else
	{
	  (sciGetGraphicContext(pobj))->backgroundcolor = (sciGetGraphicContext(paxesmdl))->backgroundcolor;
	  (sciGetGraphicContext(pobj))->foregroundcolor = (sciGetGraphicContext(paxesmdl))->foregroundcolor;
	  (sciGetGraphicContext(pobj))->fillstyle = (sciGetGraphicContext(paxesmdl))->fillstyle;
	  (sciGetGraphicContext(pobj))->fillcolor = (sciGetGraphicContext(paxesmdl))->fillcolor;
	  (sciGetGraphicContext(pobj))->linewidth = (sciGetGraphicContext(paxesmdl))->linewidth;
	  (sciGetGraphicContext(pobj))->linestyle = (sciGetGraphicContext(paxesmdl))->linestyle;
	  (sciGetGraphicContext(pobj))->ismark    = (sciGetGraphicContext(paxesmdl))->ismark;
	  (sciGetGraphicContext(pobj))->markstyle = (sciGetGraphicContext(paxesmdl))->markstyle;
	}
      return 0;
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS    */
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS */
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
      if (pobj == pfiguremdl)
	{
	  (sciGetGraphicMode (pobj))->wresize = TRUE;
	  (sciGetGraphicMode (pobj))->addplot = TRUE;
	  (sciGetGraphicMode (pobj))->autoscaling = TRUE;
	  (sciGetGraphicMode (pobj))->zooming = FALSE;
	  (sciGetGraphicMode (pobj))->oldstyle = FALSE;
	  (sciGetGraphicMode (pobj))->xormode = 3;
	}
      else
	{
	  (sciGetGraphicMode (pobj))->wresize = (sciGetGraphicMode (pfiguremdl))->wresize;
	  (sciGetGraphicMode (pobj))->addplot = (sciGetGraphicMode (pfiguremdl))->addplot;
	  (sciGetGraphicMode (pobj))->autoscaling = (sciGetGraphicMode (pfiguremdl))->autoscaling;
	  (sciGetGraphicMode (pobj))->zooming = (sciGetGraphicMode (pfiguremdl))->zooming;
	  (sciGetGraphicMode (pobj))->oldstyle = (sciGetGraphicMode (pfiguremdl))->oldstyle;
	  (sciGetGraphicMode (pobj))->xormode = (sciGetGraphicMode (pfiguremdl))->xormode;
	}
      break;
    case SCI_SUBWIN:
      if (pobj == paxesmdl)
	{
	  (sciGetGraphicMode (pobj))->wresize =sciGetResize (sciGetParent (pobj));
	  (sciGetGraphicMode (pobj))->addplot =sciGetAddPlot (sciGetParent (pobj));
	  (sciGetGraphicMode (pobj))->autoscaling =sciGetAutoScale (sciGetParent (pobj));
	  (sciGetGraphicMode (pobj))->zooming =sciGetZooming (sciGetParent (pobj));
	  (sciGetGraphicMode (pobj))->oldstyle =sciGetGraphicsStyle (sciGetParent (pobj)); 
	  (sciGetGraphicMode (pobj))->xormode =sciGetXorMode (sciGetParent (pobj));
      	}
      else
	{
	  (sciGetGraphicMode (pobj))->addplot =(sciGetGraphicMode (paxesmdl))->addplot;
	  (sciGetGraphicMode (pobj))->autoscaling = (sciGetGraphicMode (paxesmdl))->autoscaling;
	  (sciGetGraphicMode (pobj))->zooming =(sciGetGraphicMode (paxesmdl))->zooming;
	  (sciGetGraphicMode (pobj))->oldstyle =(sciGetGraphicMode (paxesmdl))->oldstyle;
	  (sciGetGraphicMode (pobj))->xormode =(sciGetGraphicMode (paxesmdl))->xormode;
      	}
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
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
  /*  double *pc;*/
  int k,old_m;
  
  old_m = sciGetNumColors(pobj); /* F.Leray*/


  if(n != 3){
    sciprint("Impossible size for colormap : number of colums must be 3\n");
    return 0;
  }
  
  if (pobj != pfiguremdl)
    C2F(dr)("xset","colormap",&m,&n,PI0,PI0,PI0,PI0,rgbmat,PD0,PD0,PD0,0L,0L);


  if(SCI_FIGURE != sciGetEntityType(pobj)){
    sciprint("sciSetColormap Error: Object must be a SCI_FIGURE\n");
    return 0;
  }

  FREE(pFIGURE_FEATURE( (sciPointObj *) pobj)->pcolormap);
  if((pFIGURE_FEATURE(pobj)->pcolormap = (double *) MALLOC (m * n * sizeof (double))) == (double *) NULL)
    {
      sciprint ("Not enough memory available for colormap\n");
      return -1;
    }  
  for (k=0;k<m*n;k++) 
    pFIGURE_FEATURE( (sciPointObj *) pobj)->pcolormap[k] = rgbmat[k];

  
/*   sciSetNumColors (pobj,old_m); /\* F.Leray *\/ */

  sciRecursiveUpdateBaW(pobj,old_m, m);

/*   sciSetNumColors (pobj,m); */

  return 0;
  
}


void sciRecursiveUpdateBaW(sciPointObj *pobj, int old_m, int m)
{
  sciSons *psonstmp;
  
  sciSetNumColors (pobj,old_m);
  
  if(old_m +1 == sciGetForeground(pobj)){         /* 0 => deals with Foreground */
    sciSetNumColors (pobj,m);
    sciUpdateBaW (pobj,0,-1); /* Black */
    sciSetNumColors (pobj,old_m);
  }
  else  if(old_m +2 == sciGetForeground(pobj)) {
    sciSetNumColors (pobj,m);
    sciUpdateBaW (pobj,0,-2); /* White */
    sciSetNumColors (pobj,old_m);
  }


  if(old_m +1 == sciGetBackground(pobj)) {   /* 1 => deals with Background */
    sciSetNumColors (pobj,m);
    sciUpdateBaW (pobj,1,-1);
    sciSetNumColors (pobj,old_m);
  }
  else if(old_m +2 == sciGetBackground(pobj)) {
    sciSetNumColors (pobj,m);
    sciUpdateBaW (pobj,1,-2);
    sciSetNumColors (pobj,old_m);
  }
      
  if((sciGetEntityType(pobj) == SCI_TEXT)        ||
     (sciGetEntityType(pobj) == SCI_TITLE)       ||
     (sciGetEntityType(pobj) == SCI_LEGEND)      ||
     (sciGetEntityType(pobj) == SCI_AXES)        ||
     (sciGetEntityType(pobj) == SCI_MENU)        ||
     (sciGetEntityType(pobj) == SCI_MENUCONTEXT) ||
     (sciGetEntityType(pobj) == SCI_STATUSB)     ||
     (sciGetEntityType(pobj) == SCI_SUBWIN)      ||
     (sciGetEntityType(pobj) == SCI_FIGURE)      ||
     (sciGetEntityType(pobj) == SCI_LABEL))
    {
    
    if(old_m +1 == sciGetFontForeground(pobj))   {      /* 2 => deals with FontForeground */
      sciSetNumColors (pobj,m);
      sciUpdateBaW (pobj,2,-1);   
      sciSetNumColors (pobj,old_m);
    }
    else  if(old_m +2 == sciGetFontForeground(pobj)) {
      sciSetNumColors (pobj,m);
      sciUpdateBaW (pobj,2,-2);
      sciSetNumColors (pobj,old_m);
    }

    if(old_m +1 == sciGetFontBackground(pobj))  {  /* 3 => deals with FontBackground */
      sciSetNumColors (pobj,m);
      sciUpdateBaW (pobj,3,-1);
      sciSetNumColors (pobj,old_m);
    }
    else if(old_m +2 == sciGetFontBackground(pobj)) {
      sciSetNumColors (pobj,m);
      sciUpdateBaW (pobj,3,-2);
      sciSetNumColors (pobj,old_m);
    }
  }

  sciSetNumColors (pobj,m); /* Add F.Leray 25.06.04 */
  
  psonstmp = sciGetLastSons (pobj);
  while (psonstmp != (sciSons *) NULL)
    {
      sciRecursiveUpdateBaW(psonstmp->pointobj, old_m, m);
      psonstmp = psonstmp->pprev;
    }
  
}


int  
sciUpdateBaW (sciPointObj * pobj, int flag, int value)
{
  
  /* F.Leray debug 30.03.04*/
  
  switch (flag)
    {
    case 0: /* Foreground*/
      switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	case SCI_SUBWIN: 
	case SCI_ARC:
	case SCI_SEGS: 
	case SCI_FEC: 
	case SCI_GRAYPLOT: 
	case SCI_POLYLINE:
	case SCI_RECTANGLE:
	case SCI_SURFACE:
	case SCI_LIGHT:
	case SCI_AXES:
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	  sciSetForeground(pobj,value);
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
      break;
    case 1: /* Background*/
       switch (sciGetEntityType (pobj))
	{
	case SCI_FIGURE:
	case SCI_SUBWIN:
	case SCI_ARC:
	case SCI_SEGS: 
	case SCI_FEC: 
	case SCI_GRAYPLOT: 
	case SCI_POLYLINE:
	case SCI_RECTANGLE:
	case SCI_SURFACE:
	case SCI_LIGHT:
	case SCI_AXES:
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	  sciSetBackground(pobj,value);
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
      break;
    case 2: /* FontForeground*/
      switch (sciGetEntityType (pobj))
	{
	case SCI_AXES:
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	case SCI_FIGURE:
	case SCI_SUBWIN:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	  sciSetFontForeground(pobj,value);
	  break;
	default:
	  return -1;
	  break;
	}
      break;
    case 3:
       switch (sciGetEntityType (pobj))
	{
	case SCI_AXES:
	case SCI_MENU:
	case SCI_MENUCONTEXT:
	case SCI_STATUSB:
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	case SCI_FIGURE:
	case SCI_SUBWIN:
	case SCI_LABEL: /* F.Leray 28.05.04 */
	  sciSetFontBackground(pobj,value);
	  break;
	default:
	  return -1;
	  break;
	}
       break;
       
    default:
      return -1;
      
    }
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
  if ( (pobj == pfiguremdl) || (pobj == paxesmdl)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_title)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_x_label)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_y_label)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_z_label) ) /* Addings F.Leray 10.06.04 */
    pFIGURE_FEATURE (pfiguremdl)->numcolors = numcolors;
  else
    sciGetScilabXgc (pobj)->Numcolors = numcolors;
  return 0;
}

/**sciGetNumColors
 * @memo This function gets the number of the color defined in colormap
 */
int
sciGetNumColors (sciPointObj * pobj)
{ 
  if ( (pobj == pfiguremdl) || (pobj == paxesmdl) 
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_title)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_x_label)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_y_label)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_z_label) ) /* Addings F.Leray 10.06.04 */
    return pFIGURE_FEATURE (pfiguremdl)->numcolors;
  else
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


/*** Adding F.Leray 31.03.04 for supporting -1 and -2 indexes.*/
int sciSetGoodIndex(sciPointObj * pobj, int colorindex) /* return colorindex or m (Default Black) or m+1 (Default White)*/
{
  int m = sciGetNumColors (pobj);	/* the number of the color*/

  if(colorindex == -1) /* Black */
    return m +1; /* modif. ici*/
  else if(colorindex == -2) /* White */
    return m+1 +1; /* modif. ici*/
  else
    return colorindex;
}

/* This function */
int sciGetGoodIndex(sciPointObj * pobj, int colorindex) /* return colorindex or m (Default Black) or m+1 (Default White)*/
{
  int m = sciGetNumColors (pobj);	/* the number of the color*/

  if(colorindex == -1) /* Black */
    return m+1;
  else if(colorindex == -2) /* White */
    return m+1 +1;
  else
    return colorindex;
}



/**sciSetBackground
 * @memo Sets the number of the Background
 */
int
sciSetBackground (sciPointObj * pobj, int colorindex)
{
  /*int zero = 0;*/
  
  int m = sciGetNumColors(pobj); 
  if(colorindex < -2 || colorindex > m+2) return 0;

  colorindex = sciSetGoodIndex(pobj,colorindex);

  if ( (pobj != pfiguremdl) && (pobj != paxesmdl)
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_title
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_x_label
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_y_label
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_z_label ) /* Addings F.Leray 10.06.04 */
    {
      /* code taken in void C2F(setbackground)(num, v2, v3, v4) from JPC */
      if (sciGetScilabXgc (pobj)->CurColorStatus == 1)
	{
	  /* COLORREF px;                           COLORREF ? "periWin-bgc"*/
	  sciGetScilabXgc (pobj)->NumBackground =
	    Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
	  /* F.Leray 02.04.04: WARNING: What follows is wrong because it forces the background for the all figure, not the object!! IT HAS TO BE REMOVE!!!!*/
	  /*	  C2F(dr) ("xset", "background",&colorindex,&colorindex,&zero,&zero,&zero,PI0,PD0,PD0,PD0,PD0,0L,0L); */ /* DJ.A 07/01/2004 */ 
	  C2F(dr)("xset","alufunction",&(sciGetScilabXgc (pobj)->CurDrawFunction),
		  PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
	}
    }
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicContext(pobj))->backgroundcolor = Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SUBWIN:
      (sciGetGraphicContext(pobj))->backgroundcolor = Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
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
    case SCI_RECTANGLE:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SURFACE:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LIGHT:
      (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
      /*   case SCI_AXIS
        (sciGetGraphicContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
	break; */
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
      (sciGetFontContext(pobj))->backgroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
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

  int colorindex = -999;
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /** the colormap is 32x3 by default
	  +1 is due to the 
	  sciGetBackground that returns the "stored color + 1" 
          that is equal to the color set by the user
      */
      colorindex = (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_SUBWIN:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_TEXT:
      colorindex =  (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_TITLE:
      colorindex =  (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_LEGEND:
      colorindex =  (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_ARC:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_POLYLINE:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_RECTANGLE:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_SURFACE:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_LIGHT:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
      /*   case SCI_AXIS
           colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
	   break;*/
    case SCI_AXES:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_PANNER:		/* pas de context graphics */
      break;
    case SCI_SBH:		/* pas de context graphics */
      break;
    case SCI_SBV:		/* pas de context graphics */
      break;
    case SCI_MENU:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_MENUCONTEXT:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_STATUSB:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;  
    case SCI_LABEL: /* F.Leray 28.05.04 */
      colorindex =  (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_AGREG:
   
    default:
      break;
    }
  
  colorindex = sciGetGoodIndex(pobj, colorindex);
  
  return colorindex;
}



/**sciSetForeground
 * @memo sets the number of the Background
 */
int
sciSetForeground (sciPointObj * pobj, int colorindex)
{

  int m = sciGetNumColors(pobj); 
  if(colorindex < -2 || colorindex > m+2) return 0;
  
  colorindex = sciSetGoodIndex(pobj,colorindex);

  /*pour le moment les couleur pris en compte sont les memes pour tout le monde */
  if ( (pobj != pfiguremdl) && (pobj != paxesmdl) 
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_title
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_x_label
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_y_label
       && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_z_label ) /* Addings F.Leray 10.06.04 */
    {
      if (sciGetScilabXgc (pobj)->CurColorStatus == 1)
	{
	  
	  sciGetScilabXgc (pobj)->NumForeground =
	    Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
	  C2F(dr)("xset","alufunction",&(sciGetScilabXgc (pobj)->CurDrawFunction),
		  PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,11L);
	}
    }
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SUBWIN:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      /*sciSetForeground (sciGetParentFigure (pobj), colorindex);*/
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
    case SCI_RECTANGLE:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_SURFACE:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LIGHT:
      (sciGetGraphicContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
      /*   case SCI_AXIS
	   (sciGetGraphicContext(pobj))->foregroundcolor = Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
	   break;*/
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
      (sciGetFontContext(pobj))->foregroundcolor =	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
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




/**sciGetForeground
 * @memo Gets the color number of the Foreground
 */
int
sciGetForeground (sciPointObj * pobj)
{

  int colorindex = -999;
  
  /** the foreground is a solid line style in b&w */
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /** the colormap is 32x3 by default */
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_SUBWIN:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_TEXT:
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_TITLE:
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_LEGEND:
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_ARC:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_POLYLINE:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_RECTANGLE:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_SURFACE:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_LIGHT:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
      /*   case SCI_AXIS
	   colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
	   break; */
    case SCI_AXES:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_MENU:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_MENUCONTEXT:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_STATUSB:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1;
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

  colorindex = sciGetGoodIndex(pobj, colorindex);
  
  return colorindex;
}



/* F.Leray 01.04.04*/
/* Adding sciGet____groundToDisplay to display the correct index*/
int
sciGetBackgroundToDisplay (sciPointObj * pobj)
{

  int colorindex = -999;
  int m = sciGetNumColors(pobj);
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /** the colormap is 32x3 by default
	  +1 is due to the 
	  sciGetBackground that returns the "stored color + 1" 
          that is equal to the color set by the user
      */
      colorindex = (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_SUBWIN:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_TEXT:
      colorindex =  (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_TITLE:
      colorindex =  (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_LEGEND:
      colorindex =  (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_ARC:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_POLYLINE:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_RECTANGLE:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_SURFACE:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_LIGHT:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
      /*   case SCI_AXIS
           colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
	   break;*/
    case SCI_AXES:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_MENU:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_MENUCONTEXT:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_STATUSB:
      colorindex =  (sciGetGraphicContext(pobj))->backgroundcolor + 1;
      break;  
    case SCI_LABEL: /* F.Leray 28.05.04 */
      colorindex =  (sciGetFontContext(pobj))->backgroundcolor + 1;
      break;
    case SCI_PANNER:	/* pas de context graphics */
    case SCI_SBH:	/* pas de context graphics */
    case SCI_SBV:	/* pas de context graphics */
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_AGREG:
    default:
      sciprint ("\r\nNo Background is associated with this Entity");
      return -1;
      break;
    }
  
  colorindex = sciGetGoodIndex(pobj, colorindex);

  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
  
  return colorindex;
}

int
sciGetForegroundToDisplay (sciPointObj * pobj)
{

  int colorindex = -999;
  int m = sciGetNumColors(pobj);

  /** the foreground is a solid line style in b&w */
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      /** the colormap is 32x3 by default */
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_SUBWIN:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_TEXT:
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_TITLE:
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_LEGEND:
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_ARC:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_POLYLINE:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_RECTANGLE:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_SURFACE:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_LIGHT:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
      /*   case SCI_AXIS
	   colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
	   break; */
    case SCI_AXES:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_MENU:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_MENUCONTEXT:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_STATUSB:
      colorindex =  (sciGetGraphicContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_LABEL:
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor + 1;
      break;
    case SCI_SEGS:
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_AGREG:
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    default:
      sciprint ("\r\nNo Foreground is associated with this Entity");
      return -1;
      break;
    }

  colorindex = sciGetGoodIndex(pobj, colorindex);
  
  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
    
  return colorindex;
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
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS */
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:  
    case SCI_PANNER:		
    case SCI_SBH:		
    case SCI_SBV:
    case SCI_LABEL: /* F.Leray 28.05.04 */	
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
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:  
    case SCI_PANNER:		
    case SCI_SBH:		
    case SCI_SBV:
    case SCI_LABEL: /* F.Leray 28.05.04 */	
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
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
    case SCI_LIGHT:
      return (sciGetGraphicContext(pobj))->linewidth;
      break;
      /*   case SCI_AXIS
         return (sciGetGraphicContext(pobj))->linewidth;
	 break;*/
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
	  /*sciSetLineWidth (sciGetParentFigure (pobj), linewidth);*/
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
	  /*   case SCI_AXIS
	       (sciGetGraphicContext(pobj))->linewidth = linewidth;
	       return 0;
	       break;*/
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
	case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
    case SCI_LIGHT:
      return (sciGetGraphicContext(pobj))->linestyle;
      break;
      /*   case SCI_AXIS
	   return (sciGetGraphicContext(pobj))->linestyle;
	   break;*/
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("This object has no Line Style\n");
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
      sciprint ("the line style must be greater than 0\n");
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
	  /*sciSetLineStyle (sciGetParentFigure (pobj), linestyle);*/
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
	  /*   case SCI_AXIS
	       (sciGetGraphicContext(pobj))->linestyle = linestyle;
	       return 0;
	       break;*/
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
	case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->ismark;
      break;
      /*   case SCI_AXIS
	   return (sciGetGraphicContext(pobj))->ismark;
	   break;*/
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
      /*sciSetIsMark (sciGetParentFigure (pobj), ismark);*/
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
    case SCI_RECTANGLE:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
    case SCI_SURFACE:
      (sciGetGraphicContext(pobj))->ismark = ismark;
      return 0;
      break;
      /*   case SCI_AXIS
	   (sciGetGraphicContext(pobj))->ismark = ismark;
	   return 0;
	   break;*/
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->markstyle;
      break;
      /*   case SCI_AXIS
	   return (sciGetGraphicContext(pobj))->markstyle;
	   break;*/
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
	  /*sciSetMarkStyle (sciGetParentFigure (pobj), markstyle);*/
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
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->markstyle = markstyle;
	  return 0;
	  break;
	  /*  case SCI_AXIS
	      (sciGetGraphicContext(pobj))->markstyle = markstyle;
	      return 0;
	      break;*/
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
	case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->fillstyle;
      break;
      /*   case SCI_AXIS
	   return (sciGetGraphicContext(pobj))->fillstyle;
	   break;*/
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("This object has no Fill Style\n");
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
	  /*sciSetFillStyle (sciGetParentFigure (pobj), fillstyle);*/
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
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	  return 0;
	  break;
	  /*   case SCI_AXIS
	       (sciGetGraphicContext(pobj))->fillstyle = fillstyle;
	       return 0;
	       break;*/
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
	case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_RECTANGLE:
      return (sciGetGraphicContext(pobj))->fillcolor;
      break;
    case SCI_SURFACE:
      return (sciGetGraphicContext(pobj))->fillcolor;
      break;
      /*   case SCI_AXIS
	   return (sciGetGraphicContext(pobj))->fillcolor;
	   break;*/
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("This object has no Fill Color\n");
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
	  /*sciSetFillStyle (sciGetParentFigure (pobj), fillcolor);*/
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
	case SCI_RECTANGLE:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
	case SCI_SURFACE:
	  (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	  return 0;
	  break;
	  /*   case SCI_AXIS
	       (sciGetGraphicContext(pobj))->fillcolor = fillcolor;
	       return 0;
	       break;*/
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
	case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_RECTANGLE:
      return  &(pRECTANGLE_FEATURE (pobj)->relationship);
      break;
    case SCI_SURFACE:
      return  &(pSURFACE_FEATURE (pobj)->relationship);
      break;
    case SCI_LIGHT:
      return  &(pLIGHT_FEATURE (pobj)->relationship);
      break;
      /*   case SCI_AXIS
	   return  &(pAXIS_FEATURE (pobj)->relationship);
	   break; */
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
      /*   case SCI_AXIS
	   (sciGetRelationship (pson))->pparent = pparent;
	   break;*/
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
    case SCI_RECTANGLE:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_SURFACE:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break;
    case SCI_LIGHT:
      return (sciPointObj *)(sciGetRelationship (pobj))->pparent;
      break;
      /*  case SCI_AXIS
	  return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
	  break;*/
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
    case SCI_MERGE:
      return (sciPointObj *) (sciGetRelationship (pobj))->pparent;
      break; 
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
  if ( (pobj == pfiguremdl) || (pobj == paxesmdl)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_title)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_x_label)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_y_label)
       || (pobj == pSUBWIN_FEATURE(paxesmdl)->mon_z_label) ) /* Addings F.Leray 10.06.04 */
    return (sciPointObj *) pfiguremdl;
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
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
      /*   case SCI_AXIS */
    case SCI_AXES:
    case SCI_SUBWIN:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_MERGE:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS */
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
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS */
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:  
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_SUBWIN:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_MERGE:  
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_RECTANGLE:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_SURFACE:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
    case SCI_LIGHT:
      (sciGetRelationship (pparent))->pcurrentson = pson;
      break;
      /*   case SCI_AXIS
	   (sciGetRelationship (pparent))->pcurrentson = pson;
	   break;*/
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
      /*  case SCI_AXIS
	  return (sciPointObj *) (sciGetRelationship (pobj))->pcurrentson;
	  break;*/
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
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_ARC:
    case SCI_AGREG:
    case SCI_MERGE: 
    case SCI_LABEL: /* F.Leray 27.05.04 */
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
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_ARC:
    case SCI_AGREG:
    case SCI_MERGE:
    case SCI_LABEL:
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
      /*   case SCI_AXIS
	   return (sciSons *) (sciGetRelationship (pobj)->psons);
	   break; */
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
      /*   case SCI_AXIS
	   return (sciSons *)sciGetRelationship (pobj)->plastsons;
	   break;*/
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
      /*   case SCI_AXIS */ /* F.Leray Adding here 10.03.04*/
      /*      pAXIS_FEATURE (pobj)->isclip = value;
	      break; */
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
      /*   case SCI_AXIS  
	   eturn pAXIS_FEATURE (pobj)->isclip;
	   break;*/
    case SCI_SURFACE:
    case SCI_LEGEND: 
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
      /*   case SCI_AXIS   */
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("This object cannot be highlighted\n");
      return FALSE;
      break;
    }
  return FALSE;
}


/**sciSetAddPlot
 * @memo Tunrs ON or OFF the possibility to draw multi plots in one graphic. 
 * If FALSE, plot is cleared before new drawing
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
      /*(sciGetGraphicMode (sciGetParentFigure(pobj)))->addplot = value;*/
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
      /*   case SCI_AXIS   */
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("This object cannot be addploted\n");
      break;
    }
}


/**sciGetAddPlot
 * @memo Returns the mode of the adding plot
 * @param sciPointObj * pobj: the pointer to the entity
 * @return TRUE if yes, FALSE if no 
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
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS   */
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
      if (pobj == pfiguremdl)
	(sciGetGraphicMode (pobj))->autoscaling = value;
      else
	sciSetAutoScale(sciGetSelectedSubWin (pobj),value);
      break;
    case SCI_SUBWIN:
      (sciGetGraphicMode (pobj))->autoscaling = value;
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
      /*   case SCI_AXIS   */
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS   */
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS */
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS  */
    case SCI_AXES:  
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
      (sciInitGraphicMode (sciGetCurrentFigure()) == -1) ||
      (sciInitFontContext (sciGetCurrentFigure()) == -1)) /* Adding F.Leray 13.04.04 to have the completed init.*/
    sciprint("\r\n default values cant not be loaded !");
  else
    {
      C2F(dr)("xset","default",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,7L); 
      sciGetScilabXgc (sciGetCurrentFigure())->graphicsversion = 1; /* To Re enable the NG -> graphicsversion = 1*/
    }
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
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS   */
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*/  case SCI_AXIS  */
    case SCI_AXES:  
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
  if ( (pobj != pfiguremdl) && (pobj != paxesmdl))
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
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS   */
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS  */
    case SCI_AXES:  
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
  integer num1 = (value ? 1 : 0);
  if ( (pobj != pfiguremdl) && (pobj != paxesmdl))
    { 
      /* this code will coms from
       *  C2F(setwresize)((i = value, &i), PI0,PI0,PI0);
       * je changerais ce morceau de code quand tout csera OK
       */
      if (sciGetScilabXgc (pobj)->CurResizeStatus != num1)
	{
	  C2F(dr)("xset","wresize",&(num1),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,5L);  
	  
	  /*  sciGetScilabXgc (pobj)->CurResizeStatus = num1;  
	      C2F(dr)("xget","wpdim",&xtmp,x,&xtmp,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,5L); 
	      C2F(dr)("xset","wpdim",&(x[0]),&(x[1]),PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,5L);*/
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
    }
  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
      (sciGetGraphicMode (pobj))->wresize = value;
      break;
    case SCI_SUBWIN:
      (sciGetGraphicMode (pobj))->wresize = value;
      /* the value is inhirated by the parent */
      /*sciSetResize (sciGetParentFigure (pobj), value);*/
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
      /*   case SCI_AXIS  */
    case SCI_AXES: 
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("This object cannot be resized\n");
      return;
      break;
    }
}


/**sciGetResize (replace sciGetwresize()in the next version)
 * @memo Returns if this object is in autoresizing mode (when the window is resized by user)
 * @param sciPointObj * pobj: the pointer to the entity
 * @return TRUE if yes, FALSE if no
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
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS  */
    case SCI_AXES: 
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
      if(strlen(pvalue) > 79){
	sciprint("Warning: Figure name is limlited to 80 characters\n");
	break;}
      else
	strcpy (pFIGURE_FEATURE (pobj)->name, pvalue);
      pFIGURE_FEATURE (pobj)->namelen = Min (79, length); 
      if (pobj != pfiguremdl) {
	char str[80];
	sprintf(str,pvalue,sciGetNum(pobj));
	C2F(dr)("xname",str,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,(long) length);
      }
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
      if (pobj != paxesmdl)
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
      if (pobj != pfiguremdl)
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
      /* synchronize figure position with its actual value */
      C2F(dr)("xget","wpos",&y,x,&y,PI0,PI0,PI0,&d,PD0,PD0,PD0,4L,4L);
      pFIGURE_FEATURE (pobj)->inrootposx=x[0];
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
      /* synchronize figure position with its actual value */
      C2F(dr)("xget","wpos",&y,x,&y,PI0,PI0,PI0,&d,PD0,PD0,PD0,4L,4L);
      pFIGURE_FEATURE (pobj)->inrootposy=x[1];
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
      /*   case SCI_AXIS  */
    case SCI_AXES: 
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
      /*   case SCI_AXIS 
	   pAXIS_FEATURE (pobj)->visible = value;
	   break;     */
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
      pLABEL_FEATURE (pobj)->visible = value;
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
      /*    case SCI_AXIS: 
	    return pAXIS_FEATURE (pobj)->visible;
	    break;     */
    case SCI_AXES: 
      return pAXES_FEATURE (pobj)->visible;
      break;    
    case SCI_AGREG: 
      return pAGREG_FEATURE (pobj)->visible;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return pLABEL_FEATURE (pobj)->visible;
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

  set_scale ("tttftf", pSUBWIN_FEATURE (psubwinobj)->WRect,
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
  int stop = 0,i;


  /* Initialisation de WRectTmp a 0*/
  for(i=0;i<4;i++)
    WRectTmp[i] = 0.;

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
      /* return -1*/
      /* Adding F.Leray 08.04.04 */
      return (sciGetFontContext(pobj))->fontdeciwidth;
      break;
    case SCI_SUBWIN:
      /* return -1;*/
      /* Adding F.Leray 08.04.04 */
      return (sciGetFontContext(pobj))->fontdeciwidth;
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
    case SCI_RECTANGLE:
      return -1;
      break;
    case SCI_SURFACE:
      return -1;
      break;
    case SCI_LIGHT:
      return -1;
      break;
      /*   case SCI_AXIS  
	   return (sciGetFontContext(pobj))->fontdeciwidth;
	   break;*/
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
    case SCI_LABEL:
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
	  case SCI_FIGURE: /* Adding F.Leray 08.04.04 */
	  /* pas de context graphics */
	  /*  sciprint ("This object have no  Font width \n");
	      return -1;*/
	  (sciGetFontContext(pobj))->fontdeciwidth = fontdeciwidth;
	  return 0;
	  break;
	case SCI_SUBWIN:
	  /* pas de context graphics */
	  /*sciprint ("This object have no  Font width \n");
	    return -1;*/
	  /* Adding F.Leray 08.04.04 */
	  (sciGetFontContext(pobj))->fontdeciwidth = fontdeciwidth;
	  return 0;
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
	  /*   case SCI_AXIS
	       (sciGetFontContext(pobj))->fontdeciwidth =
	       fontdeciwidth;
	       return 0;
	       break;*/
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
	case SCI_LABEL:
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
      /*   case SCI_AXIS
	   return (sciGetFontContext(pobj))->textorientation;
	   break;*/
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
    case SCI_SUBWIN:
      /* Adding F.Leray 08.04.04 */
      /* Is text orientation usefull with Axes ??*/
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_FIGURE:
      /* Adding F.Leray 08.04.04 */
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_LABEL:
      /* Adding F.Leray 28.05.04 */
      return (sciGetFontContext(pobj))->textorientation;
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
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
	  /*   case SCI_AXIS
	       (sciGetFontContext(pobj))->textorientation =	    textorientation;
	       return 0;
	       break;*/
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
	case SCI_SUBWIN: /* F.Leray 08.04.04*/
	  /* Is text orientation usefull with Axes ?? */
	  (sciGetFontContext(pobj))->textorientation = textorientation;
	  return 0;
	  break;
	case SCI_FIGURE: /* F.Leray 08.04.04*/
	  (sciGetFontContext(pobj))->textorientation = textorientation;
	  return 0;
	  break;
	case SCI_LABEL: /* F.Leray 28.05.04*/
	  (sciGetFontContext(pobj))->textorientation = textorientation;
	  return 0;
	  break;
	case SCI_ARC:
	case SCI_SEGS: 
	case SCI_FEC: 
	case SCI_GRAYPLOT: 
	case SCI_POLYLINE:
	case SCI_RECTANGLE:
	case SCI_SURFACE:
	case SCI_LIGHT:
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
      FREE(pLABEL_FEATURE (pobj)->text.ptextstring);
      if ((pLABEL_FEATURE (pobj)->text.ptextstring = calloc (n+1, sizeof (char))) == NULL)
	return -1;
      strncpy (pLABEL_FEATURE (pobj)->text.ptextstring, text, n);
      pLABEL_FEATURE (pobj)->text.textlen = n;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC:
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS*/
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return pLABEL_FEATURE (pobj)->text.ptextstring;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS*/
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return pLABEL_FEATURE (pobj)->text.textlen;
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS*/
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
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* None for the moment F.Leray 28.05.04 */
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
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* None for the moment F.Leray 28.05.04 */
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
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* None for the moment F.Leray 28.05.04 */
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
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* None for the moment F.Leray 28.05.04 */
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
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* None for the moment F.Leray 28.05.04 */
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
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* None for the moment F.Leray 28.05.04 */
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

  int colorindex = -999;
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_STATUSB:
    case SCI_SUBWIN: /* F.Leray 08.04.04 */
    case SCI_FIGURE: /* F.Leray 08.04.04 */
    case SCI_LABEL:  /* F.Leray 28.05.04 */
      colorindex = (sciGetFontContext(pobj))->backgroundcolor+1; /* +1 added by F.Leray 25.06.04 */
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
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
  
  colorindex = sciGetGoodIndex(pobj, colorindex); /* Adding F.Leray 31.03.04*/
  return colorindex; 
}


/**sciGetFontBackgroundToDisplay
 */
int
sciGetFontBackgroundToDisplay (sciPointObj * pobj)
{

  int colorindex = -999;
  int m = sciGetNumColors(pobj);
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_STATUSB:
    case SCI_SUBWIN: /* F.Leray 08.04.04 */
    case SCI_FIGURE: /* F.Leray 08.04.04 */
    case SCI_LABEL:  /* F.Leray 28.05.04 */
      colorindex = (sciGetFontContext(pobj))->backgroundcolor;
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_AGREG:
    default:
       sciprint ("\r\nNo FontBackground is associated with this Entity");
      return -1;
      break;
    }
  
  colorindex = sciGetGoodIndex(pobj, colorindex); /* Adding F.Leray 31.03.04*/
  
  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
  
  return colorindex; 
}




/**sciSetFontBackground
 * @memo Sets the background color
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, -1 if not
 */
int
sciSetFontBackground (sciPointObj * pobj, int colorindex)
{

 colorindex = sciSetGoodIndex(pobj,colorindex); /* Adding F.Leray 31.03.04*/
  
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
      /*   case SCI_AXIS
       (sciGetFontContext(pobj))->backgroundcolor =
       Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
       break;*/
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
    case SCI_SUBWIN: /* F.Leray 08.04.04 */
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_FIGURE: /* F.Leray 08.04.04 */
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      (sciGetFontContext(pobj))->backgroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
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

  int colorindex = -999;
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
      /*   case SCI_AXIS  */
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_SUBWIN:  /* F.Leray 08.04.04 */
    case SCI_FIGURE:  /* F.Leray 08.04.04 */
    case SCI_LABEL:   /* F.Leray 28.05.04 */
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor+ 1 ; /* Modif. F.Leray 31.03.04*/
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
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

  colorindex = sciGetGoodIndex(pobj, colorindex); /* Adding F.Leray 31.03.04*/
  return colorindex;
}


/**sciGetFontForegroundToDisplay
 */
int
sciGetFontForegroundToDisplay (sciPointObj * pobj)
{

  int colorindex = -999;
  int m = sciGetNumColors(pobj);
  
  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
      /*   case SCI_AXIS  */
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_SUBWIN:  /* F.Leray 08.04.04 */
    case SCI_FIGURE:  /* F.Leray 08.04.04 */
    case SCI_LABEL:   /* F.Leray 28.05.04 */
      colorindex =  (sciGetFontContext(pobj))->foregroundcolor+ 1 ; /* Modif. F.Leray 31.03.04*/
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_AGREG:
    default:
      sciprint ("\r\nNo FontForeground is associated with this Entity");
      return -1;
      break;
    }

  colorindex = sciGetGoodIndex(pobj, colorindex); /* Adding F.Leray 31.03.04*/
  
  if((m - colorindex == -1) || (m - colorindex == -2)) colorindex =  m - colorindex;
  
  return colorindex;
}




/**sciSetFontForeground
 * @memo Sets the foreground color
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, if not
 */
int
sciSetFontForeground (sciPointObj * pobj, int colorindex)
{
  
  colorindex = sciSetGoodIndex(pobj,colorindex); /* Adding F.Leray 31.03.04*/
  
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
      /*   case SCI_AXIS
	   (sciGetFontContext(pobj))->foregroundcolor =
	   Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
	   break; */
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
    case SCI_SUBWIN: /* F.Leray 08.04.04 */
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_FIGURE: /* F.Leray 08.04.04 */
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_LABEL: /* F.Leray 08.04.04 */
      (sciGetFontContext(pobj))->foregroundcolor =
	Max (0, Min (colorindex - 1, sciGetNumColors (pobj) + 1));
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
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
 * @memo Gets the font style 

 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 OK, -1 if not
 */
int
sciGetFontStyle (sciPointObj * pobj)
{


  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SUBWIN:
    case SCI_FIGURE:
    case SCI_LABEL: /* F.Leray 10.06.04 */
      return sciGetFontContext(pobj)->fonttype;
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC:
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS   */
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
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SUBWIN: 
    case SCI_FIGURE: 
    case SCI_LABEL: /* F.Leray 10.06.04 */
      (sciGetFontContext(pobj))->fonttype =iAttributes;
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS   */
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
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS   */
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* None for the moment F.Leray 28.05.04 */
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
    case SCI_SUBWIN: /* F.Leray 08.04.04 */
      return 	(sciGetFontContext(pobj))->pfontname;
      break;
    case SCI_FIGURE: /* F.Leray 08.04.04 */
      return 	(sciGetFontContext(pobj))->pfontname;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return 	(sciGetFontContext(pobj))->pfontname;
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS   */
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
    case SCI_SUBWIN: /* F.Leray 08.04.04 */
      return 	(sciGetFontContext(pobj))->fontnamelen;
      break;
    case SCI_FIGURE: /* F.Leray 08.04.04 */
      return 	(sciGetFontContext(pobj))->fontnamelen;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      return 	(sciGetFontContext(pobj))->fontnamelen;
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS   */
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
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS   */
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
      /*   case SCI_AXIS   */
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_SUBWIN: /* F.Leray 08.04.04 THE MOST IMPORTANT*/
      return &(pSUBWIN_FEATURE (pobj)->axes.fontcontext);
      break;
    case SCI_AXES:
      /*axes uses the parent subwindow font context S Steer 04.06.04*/
       return &(pSUBWIN_FEATURE (sciGetParentSubwin(pobj) )->axes.fontcontext);
       break;  
    case SCI_FIGURE: /* F.Leray 08.04.04 THE MOST IMPORTANT*/
      return &(pFIGURE_FEATURE (pobj)->fontcontext);
      break;
    case SCI_LABEL: /* F.Leray 27.05.04 */
      return &(pLABEL_FEATURE (pobj)->text.fontcontext);
      break;
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
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

  /*  sciPointObj * psubwin = NULL;*/
  

  switch (sciGetEntityType (pobj))
    {
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_LABEL: /* Re-init here must be better F.Leray 28.05.04 */
      (sciGetFontContext(pobj))->backgroundcolor = sciGetFontBackground (sciGetParent (pobj)) -1;
      (sciGetFontContext(pobj))->foregroundcolor = sciGetFontForeground (sciGetParent (pobj)) -1;
      (sciGetFontContext(pobj))->fonttype        = (sciGetFontContext(sciGetParent(pobj)))->fonttype; 
      (sciGetFontContext(pobj))->fontdeciwidth   = (sciGetFontContext(sciGetParent(pobj)))->fontdeciwidth;
      (sciGetFontContext(pobj))->textorientation = (sciGetFontContext(sciGetParent(pobj)))->textorientation;
      (sciGetFontContext(pobj))->fontnamelen     = (sciGetFontContext(sciGetParent(pobj)))->fontnamelen; 
      if (((sciGetFontContext(pobj))->pfontname =
	   calloc ((sciGetFontContext(pobj))->fontnamelen + 1,
		   sizeof (char))) == NULL)
	{
	  sciprint ("No more Memory for fontname\n"); 
	  return 0;
	}
      break;
    case SCI_SUBWIN: 
      if (pobj == paxesmdl)
	{
	  sciSetFontStyle (pobj, 6);/* set helvetica font */
	  (sciGetFontContext(pobj))->backgroundcolor = -3;
	  (sciGetFontContext(pobj))->foregroundcolor = -2; 
	  (sciGetFontContext(pobj))->fontdeciwidth = 100;
	  (sciGetFontContext(pobj))->textorientation = 0;
	  (sciGetFontContext(pobj))->fontnamelen=1; /*fontname not used */
	  
	  if (
	      ((sciGetFontContext(pobj))->pfontname = calloc ((sciGetFontContext(pobj))->fontnamelen + 1,
							      sizeof (char))) == NULL)
	    {
	      sciprint ("No more Memory for fontname\n");
	      return 0;
	    }
	}
      else
	{
	  (sciGetFontContext(pobj))->backgroundcolor = (sciGetFontContext(paxesmdl))->backgroundcolor;
	  (sciGetFontContext(pobj))->foregroundcolor = (sciGetFontContext(paxesmdl))->foregroundcolor;
	  (sciGetFontContext(pobj))->fonttype =        (sciGetFontContext(paxesmdl))->fonttype;
 	  (sciGetFontContext(pobj))->fontdeciwidth =   (sciGetFontContext(paxesmdl))->fontdeciwidth;
	  (sciGetFontContext(pobj))->textorientation = (sciGetFontContext(paxesmdl))->textorientation;
	  (sciGetFontContext(pobj))->fontnamelen =     (sciGetFontContext(paxesmdl))->fontnamelen; /*fontname not used */
	  
	  if (
	      ((sciGetFontContext(pobj))->pfontname = calloc ((sciGetFontContext(pobj))->fontnamelen + 1,
							      sizeof (char))) == NULL)
	    {
	      sciprint ("No more Memory for fontname\n");
	      return 0;
	    }
	  /* END ADDING F.Leray 08.04.04*/ 
	} 
      break;
    case SCI_FIGURE:
      if (pobj == pfiguremdl)
	{
	  sciSetFontStyle (pobj, 6);/* set helvetica font */
	  (sciGetFontContext(pobj))->backgroundcolor = -3;
	  (sciGetFontContext(pobj))->foregroundcolor = -2;
	  (sciGetFontContext(pobj))->fontdeciwidth = 100;
	  (sciGetFontContext(pobj))->textorientation = 0;
	  (sciGetFontContext(pobj))->fontnamelen=1; /*fontname not used */
	  
	  if (
	      ((sciGetFontContext(pobj))->pfontname = calloc ((sciGetFontContext(pobj))->fontnamelen + 1,
							      sizeof (char))) == NULL)
	    {
	      sciprint ("No more Memory for fontname\n");
	      return 0;
	    }
	  /* END ADDING F.Leray 08.04.04*/ 
	}
      else
	{
	  /* START ADDING F.Leray 08.04.04*/ 
	  /* F.Leray 08.04.04 : I create a  sciFont fontcontext; to act on Axes font*/
	  (sciGetFontContext(pobj))->backgroundcolor = (sciGetFontContext(pfiguremdl))->backgroundcolor;	
	  (sciGetFontContext(pobj))->foregroundcolor = (sciGetFontContext(pfiguremdl))->foregroundcolor;
	  (sciGetFontContext(pobj))->fonttype =        (sciGetFontContext(pfiguremdl))->fonttype;
  	  (sciGetFontContext(pobj))->fontdeciwidth =   (sciGetFontContext(pfiguremdl))->fontdeciwidth;
	  (sciGetFontContext(pobj))->textorientation = (sciGetFontContext(pfiguremdl))->textorientation;
	  (sciGetFontContext(pobj))->fontnamelen =     (sciGetFontContext(pfiguremdl))->fontnamelen; /*fontname not used */
	  
	  if (
	      ((sciGetFontContext(pobj))->pfontname = calloc ((sciGetFontContext(pobj))->fontnamelen + 1,
							      sizeof (char))) == NULL)
	    {
	      sciprint ("No more Memory for fontname\n");
	      return 0;
	    }
	  /* END ADDING F.Leray 08.04.04*/  
	}
      break;
    case SCI_PANNER:		/* pas de context graphics */
    case SCI_SBH:		/* pas de context graphics */
    case SCI_SBV:		/* pas de context graphics */
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
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
  case SCI_RECTANGLE:
  case SCI_SURFACE: 
  case SCI_LIGHT:
    /*   case SCI_AXIS  */
  case SCI_AXES:
  case SCI_PANNER:
  case SCI_SBH:
  case SCI_SBV:
  case SCI_MENU:
  case SCI_STATUSB:
  case SCI_AGREG:
  case SCI_LABEL: /* F.Leray 28.05.04 */
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
      case SCI_RECTANGLE:
      case SCI_SURFACE: 
      case SCI_LIGHT:
	/*   case SCI_AXIS  */
      case SCI_AXES:
      case SCI_PANNER:
      case SCI_SBH:
      case SCI_SBV:
      case SCI_MENU:
      case SCI_STATUSB:
      case SCI_AGREG:
      case SCI_LABEL: /* F.Leray 28.05.04 */
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
  sciSons *toto = NULL;
 
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
      /*   case SCI_AXIS
	   DestroyAxis (pthis);
	   return 0;
	   break;*/
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
    case SCI_MERGE:
      DestroyMerge (pthis);
      return 0;
      break;
    case SCI_LABEL: /* F.Leray 28.05.04 */
      DestroyLabel (pthis);
      return 0;
      break;
    default:
      sciprint ("Entity with type %d cannot be destroyed\n",sciGetEntityType (pthis));
      return -1;
      break;
    }

  return 0;
}



/**sciDelGraphicObj
 * This function delete only users graphics object and its dependency limited to 
 * SCI_TITLE SCI_LEGEND SCI_ARC SCI_POLYLINE SCI_RECTANGLE SCI_SURFACE 
 * SCI_AXIS SCI_MENU SCI_MENUCONTEXT SCI_SUBWIN
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
    case SCI_SURFACE:
      /*   case SCI_AXIS    */
    case SCI_AXES:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_AGREG:
    case SCI_TEXT:
    case SCI_MERGE: 
    case SCI_LABEL:
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
  integer i , m, n;
  integer x[2], verbose=0, narg=0; 


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
  n=3;
  m = sciGetNumColors (pfiguremdl);
  if((pFIGURE_FEATURE(pobj)->pcolormap = (double *) MALLOC (m * n * sizeof (double))) == (double *) NULL)
    {
      sciDelHandle (pobj);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }  

  /* F.Leray 30.03.04 */
  for (i=0; i <m*n ; i++)
    pFIGURE_FEATURE(pobj)->pcolormap[i] = pFIGURE_FEATURE(pfiguremdl)->pcolormap[i];
  C2F(dr)("xset","colormap",&m,&n,PI0,PI0,PI0,PI0,pFIGURE_FEATURE(pobj)->pcolormap,PD0,PD0,PD0,0L,0L);
  sciSetNumColors (pobj,m);
   
  /* initialisation de context et mode graphique par defaut (figure model)*/
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

  /* F.Leray 08.04.04 */
  if (sciInitFontContext (pobj) == -1)
    {
      sciDelHandle (pobj);
      FREE(pobj->pfeatures);	  
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
  sciSetNum (pobj, &(XGC->CurWindow));		   
  sciSetName(pobj, sciGetName(pfiguremdl), sciGetNameLength(pfiguremdl));
  sciSetResize((sciPointObj *) pobj,sciGetResize(pobj));
  pFIGURE_FEATURE(pobj)->windowdimwidth=pFIGURE_FEATURE(pfiguremdl)->windowdimwidth;  
  pFIGURE_FEATURE(pobj)->windowdimheight=pFIGURE_FEATURE(pfiguremdl)->windowdimheight;
  C2F(dr)("xset","wdim",&(pFIGURE_FEATURE(pobj)->windowdimwidth),
	  &(pFIGURE_FEATURE(pobj)->windowdimheight),PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
  pFIGURE_FEATURE (pobj)->figuredimwidth = pFIGURE_FEATURE (pfiguremdl)->figuredimwidth;
  pFIGURE_FEATURE (pobj)->figuredimheight = pFIGURE_FEATURE (pfiguremdl)->figuredimheight;
  C2F(dr)("xset","wpdim",&(pFIGURE_FEATURE(pobj)->figuredimwidth),
	  &(pFIGURE_FEATURE(pobj)->figuredimheight),PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","wpos",&verbose,x,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,4L);
  x[0]=(pFIGURE_FEATURE (pfiguremdl)->inrootposx <0)?x[0]:pFIGURE_FEATURE (pfiguremdl)->inrootposx;
  x[1]=(pFIGURE_FEATURE (pfiguremdl)->inrootposy <0)?x[1]:pFIGURE_FEATURE (pfiguremdl)->inrootposy;
  x[0]=(pFIGURE_FEATURE (pfiguremdl)->inrootposx <0)?x[0]:pFIGURE_FEATURE (pfiguremdl)->inrootposx;
  x[1]=(pFIGURE_FEATURE (pfiguremdl)->inrootposy <0)?x[1]:pFIGURE_FEATURE (pfiguremdl)->inrootposy;
  sciSetFigurePos (pobj,x[0],x[1]);
  pFIGURE_FEATURE (pobj)->isiconified = pFIGURE_FEATURE (pfiguremdl)->isiconified;
  pFIGURE_FEATURE (pobj)->isselected = pFIGURE_FEATURE (pfiguremdl)->isselected; 
  pFIGURE_FEATURE (pobj)->rotstyle = pFIGURE_FEATURE (pfiguremdl)->rotstyle;
  pFIGURE_FEATURE (pobj)->visible = pFIGURE_FEATURE (pfiguremdl)->visible;
  pFIGURE_FEATURE (pobj)->numsubwinselected = pFIGURE_FEATURE (pfiguremdl)->numsubwinselected;
  pFIGURE_FEATURE (pobj)->pixmap = pFIGURE_FEATURE (pfiguremdl)->pixmap ; 
  pFIGURE_FEATURE (pobj)->wshow = pFIGURE_FEATURE (pfiguremdl)->wshow ; 
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

  sciPointObj *pobj = (sciPointObj *) NULL;
  sciSubWindow * ppsubwin = NULL; 

  if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
    {
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
      
      ppsubwin =  pSUBWIN_FEATURE (pobj); /* debug */

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
      
      /* F.Leray 08.04.04 */
      if (sciInitFontContext (pobj) == -1)
	{
	  /* sciDelThisToItsParent (pobj, sciGetParent (pobj));*/
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);	  
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      dir= pSUBWIN_FEATURE (paxesmdl)->logflags[0];
      pSUBWIN_FEATURE (pobj)->logflags[0] = dir;
      dir= pSUBWIN_FEATURE (paxesmdl)->logflags[1]; 
      pSUBWIN_FEATURE (pobj)->logflags[1] = dir;

      pSUBWIN_FEATURE (pobj)->axes.ticscolor  = pSUBWIN_FEATURE (paxesmdl)->axes.ticscolor;
      pSUBWIN_FEATURE (pobj)->axes.subint[0]  = pSUBWIN_FEATURE (paxesmdl)->axes.subint[0];   
      pSUBWIN_FEATURE (pobj)->axes.subint[1]  = pSUBWIN_FEATURE (paxesmdl)->axes.subint[1]; 
      pSUBWIN_FEATURE (pobj)->axes.subint[2]  = pSUBWIN_FEATURE (paxesmdl)->axes.subint[2];

      dir= pSUBWIN_FEATURE (paxesmdl)->axes.xdir; 
      pSUBWIN_FEATURE (pobj)->axes.xdir = dir; 
      dir= pSUBWIN_FEATURE (paxesmdl)->axes.ydir; 
      pSUBWIN_FEATURE (pobj)->axes.ydir = dir;
 
      pSUBWIN_FEATURE (pobj)->axes.rect  = pSUBWIN_FEATURE (paxesmdl)->axes.rect;
      for (i=0 ; i<7 ; i++)
	pSUBWIN_FEATURE (pobj)->axes.limits[i]  = pSUBWIN_FEATURE (paxesmdl)->axes.limits[i] ;
 
      for (i=0 ; i<3 ; i++)
	pSUBWIN_FEATURE (pobj)->grid[i]  = pSUBWIN_FEATURE (paxesmdl)->grid[i] ;
      pSUBWIN_FEATURE (pobj)->isaxes  = pSUBWIN_FEATURE (paxesmdl)->isaxes;
      pSUBWIN_FEATURE (pobj)->alpha  = pSUBWIN_FEATURE (paxesmdl)->alpha;
      pSUBWIN_FEATURE (pobj)->theta  = pSUBWIN_FEATURE (paxesmdl)->theta;
      pSUBWIN_FEATURE (pobj)->alpha_kp  = pSUBWIN_FEATURE (paxesmdl)->alpha_kp;
      pSUBWIN_FEATURE (pobj)->theta_kp  = pSUBWIN_FEATURE (paxesmdl)->theta_kp;
      pSUBWIN_FEATURE (pobj)->is3d  = pSUBWIN_FEATURE (paxesmdl)->is3d;
       
      for (i=0 ; i<4 ; i++)
        {  
	  pSUBWIN_FEATURE (pobj)->axes.xlim[i]= pSUBWIN_FEATURE (paxesmdl)->axes.xlim[i]; 
	  pSUBWIN_FEATURE (pobj)->axes.ylim[i]= pSUBWIN_FEATURE (paxesmdl)->axes.ylim[i]; 
	}
      pSUBWIN_FEATURE (pobj)->axes.zlim[0]= pSUBWIN_FEATURE (paxesmdl)->axes.zlim[0];
      pSUBWIN_FEATURE (pobj)->axes.zlim[1]= pSUBWIN_FEATURE (paxesmdl)->axes.zlim[1];
      pSUBWIN_FEATURE (pobj)->axes.flag[0]= pSUBWIN_FEATURE (paxesmdl)->axes.flag[0];
      pSUBWIN_FEATURE (pobj)->axes.flag[1]= pSUBWIN_FEATURE (paxesmdl)->axes.flag[1];
      pSUBWIN_FEATURE (pobj)->axes.flag[2]= pSUBWIN_FEATURE (paxesmdl)->axes.flag[2];
      pSUBWIN_FEATURE (pobj)->project[0]= pSUBWIN_FEATURE (paxesmdl)->project[0];
      pSUBWIN_FEATURE (pobj)->project[1]= pSUBWIN_FEATURE (paxesmdl)->project[1];
      pSUBWIN_FEATURE (pobj)->project[2]= pSUBWIN_FEATURE (paxesmdl)->project[2];
      pSUBWIN_FEATURE (pobj)->hiddencolor= pSUBWIN_FEATURE (paxesmdl)->hiddencolor;
      pSUBWIN_FEATURE (pobj)->hiddenstate= pSUBWIN_FEATURE (paxesmdl)->hiddenstate;
      pSUBWIN_FEATURE (pobj)->isoview= pSUBWIN_FEATURE (paxesmdl)->isoview;
      pSUBWIN_FEATURE (pobj)->facetmerge = pSUBWIN_FEATURE (paxesmdl)->facetmerge; 
      pSUBWIN_FEATURE (pobj)->WRect[0]   = pSUBWIN_FEATURE (paxesmdl)->WRect[0];
      pSUBWIN_FEATURE (pobj)->WRect[1]   = pSUBWIN_FEATURE (paxesmdl)->WRect[1];
      pSUBWIN_FEATURE (pobj)->WRect[2]   = pSUBWIN_FEATURE (paxesmdl)->WRect[2];
      pSUBWIN_FEATURE (pobj)->WRect[3]   = pSUBWIN_FEATURE (paxesmdl)->WRect[3];

      pSUBWIN_FEATURE (pobj)->ARect[0]   = pSUBWIN_FEATURE (paxesmdl)->ARect[0];
      pSUBWIN_FEATURE (pobj)->ARect[1]   = pSUBWIN_FEATURE (paxesmdl)->ARect[1];
      pSUBWIN_FEATURE (pobj)->ARect[2]   = pSUBWIN_FEATURE (paxesmdl)->ARect[2];
      pSUBWIN_FEATURE (pobj)->ARect[3]   = pSUBWIN_FEATURE (paxesmdl)->ARect[3];

      pSUBWIN_FEATURE (pobj)->FRect[0]   = pSUBWIN_FEATURE (paxesmdl)->FRect[0];
      pSUBWIN_FEATURE (pobj)->FRect[1]   = pSUBWIN_FEATURE (paxesmdl)->FRect[1] ;
      pSUBWIN_FEATURE (pobj)->FRect[2]   = pSUBWIN_FEATURE (paxesmdl)->FRect[2];
      pSUBWIN_FEATURE (pobj)->FRect[3]   = pSUBWIN_FEATURE (paxesmdl)->FRect[3];
      pSUBWIN_FEATURE (pobj)->FRect[4]   = pSUBWIN_FEATURE (paxesmdl)->FRect[4] ;
      pSUBWIN_FEATURE (pobj)->FRect[5]   = pSUBWIN_FEATURE (paxesmdl)->FRect[5];
     

      /* ppsubwin =  pSUBWIN_FEATURE (pobj); */ /* debug */
     
      pSUBWIN_FEATURE (pobj)->isselected = pSUBWIN_FEATURE (paxesmdl)->isselected;  
      pSUBWIN_FEATURE (pobj)->visible = pSUBWIN_FEATURE (paxesmdl)->visible; 
      pSUBWIN_FEATURE (pobj)->isclip = pSUBWIN_FEATURE (paxesmdl)->isclip;

      /* F.Leray 26.04.04 : Pb init axes.aaint DONE HERE: default values */
      pSUBWIN_FEATURE (pobj)->axes.aaint[0] = pSUBWIN_FEATURE (paxesmdl)->axes.aaint[0];
      pSUBWIN_FEATURE (pobj)->axes.aaint[1] = pSUBWIN_FEATURE (paxesmdl)->axes.aaint[1];
      pSUBWIN_FEATURE (pobj)->axes.aaint[2] = pSUBWIN_FEATURE (paxesmdl)->axes.aaint[2];
      pSUBWIN_FEATURE (pobj)->axes.aaint[3] = pSUBWIN_FEATURE (paxesmdl)->axes.aaint[3];

      pSUBWIN_FEATURE (pobj)->cube_scaling = pSUBWIN_FEATURE (paxesmdl)->cube_scaling;


      pSUBWIN_FEATURE (pobj)->SRect[0]  =  pSUBWIN_FEATURE (paxesmdl)->SRect[0];
      pSUBWIN_FEATURE (pobj)->SRect[1]  =  pSUBWIN_FEATURE (paxesmdl)->SRect[1];
      pSUBWIN_FEATURE (pobj)->SRect[2]  =  pSUBWIN_FEATURE (paxesmdl)->SRect[2];
      pSUBWIN_FEATURE (pobj)->SRect[3]  =  pSUBWIN_FEATURE (paxesmdl)->SRect[3];
      pSUBWIN_FEATURE (pobj)->SRect[4]  =  pSUBWIN_FEATURE (paxesmdl)->SRect[4];
      pSUBWIN_FEATURE (pobj)->SRect[5]  =  pSUBWIN_FEATURE (paxesmdl)->SRect[5];
      
      pSUBWIN_FEATURE (pobj)->tight_limits = pSUBWIN_FEATURE (paxesmdl)->tight_limits;
      pSUBWIN_FEATURE (pobj)->FirstPlot = pSUBWIN_FEATURE (paxesmdl)->FirstPlot;
      pSUBWIN_FEATURE (pobj)->with_leg =  pSUBWIN_FEATURE (paxesmdl)->with_leg;
      
      if (sciSetSelectedSubWin(pobj) != 1) 
	return (sciPointObj *)NULL; 
      
      /* Construction des labels: x,y,z et Title */
      ppsubwin =  pSUBWIN_FEATURE (pobj);

      if ((ppsubwin->mon_title =  ConstructLabel (pobj, "",1)) == NULL){
	sciDelThisToItsParent (pobj, sciGetParent (pobj)); /* pobj type = scisubwindow*/
	sciDelHandle (pobj);
	FREE(pobj->pfeatures);
	FREE(pobj);
	return (sciPointObj *) NULL;
      }

      if (sciInitFontContext (ppsubwin->mon_title) == -1)
	{
	  DestroyLabel(ppsubwin->mon_title);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);	  
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      
      sciSetText(ppsubwin->mon_title, pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_title)->text.ptextstring,  
		 pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_title)->text.textlen);
      sciSetFontStyle(ppsubwin->mon_title, sciGetFontStyle(pSUBWIN_FEATURE(paxesmdl)->mon_title));
      sciSetFontDeciWidth(ppsubwin->mon_title,sciGetFontDeciWidth(pSUBWIN_FEATURE(paxesmdl)->mon_title));
      sciSetVisibility(ppsubwin->mon_title, sciGetVisibility(pSUBWIN_FEATURE(paxesmdl)->mon_title));
      
      /*------------------------------------*/
      if ((ppsubwin->mon_x_label =  ConstructLabel (pobj, "",2)) == NULL){
	DestroyLabel(ppsubwin->mon_title);
	sciDelThisToItsParent (pobj, sciGetParent (pobj));
	sciDelHandle (pobj);
	FREE(pobj->pfeatures);
	FREE(pobj);
	return (sciPointObj *) NULL;
      }
      
      if (sciInitFontContext (ppsubwin->mon_x_label) == -1)
	{
	  DestroyLabel(ppsubwin->mon_title);
	  DestroyLabel(ppsubwin->mon_x_label);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      sciSetText(ppsubwin->mon_x_label, pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_x_label)->text.ptextstring,  
		 pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_x_label)->text.textlen);
      sciSetFontStyle(ppsubwin->mon_x_label, sciGetFontStyle(pSUBWIN_FEATURE(paxesmdl)->mon_x_label));
      sciSetFontDeciWidth(ppsubwin->mon_x_label,sciGetFontDeciWidth(pSUBWIN_FEATURE(paxesmdl)->mon_x_label));
      sciSetVisibility(ppsubwin->mon_x_label, sciGetVisibility(pSUBWIN_FEATURE(paxesmdl)->mon_x_label));

      /*------------------------------------*/
      if ((ppsubwin->mon_y_label =  ConstructLabel (pobj, "",3)) == NULL){
	DestroyLabel(ppsubwin->mon_title);
	DestroyLabel(ppsubwin->mon_x_label);
	sciDelThisToItsParent (pobj, sciGetParent (pobj));
	sciDelHandle (pobj);
	FREE(pobj->pfeatures);
	FREE(pobj);
	return (sciPointObj *) NULL;
      }
  
      if (sciInitFontContext (ppsubwin->mon_y_label) == -1)
	{
	  DestroyLabel(ppsubwin->mon_title);
	  DestroyLabel(ppsubwin->mon_x_label);
	  DestroyLabel(ppsubwin->mon_y_label);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      sciSetText(ppsubwin->mon_y_label, pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_y_label)->text.ptextstring,  
		 pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_y_label)->text.textlen);
      sciSetFontStyle(ppsubwin->mon_y_label, sciGetFontStyle(pSUBWIN_FEATURE(paxesmdl)->mon_y_label));
      sciSetFontDeciWidth(ppsubwin->mon_y_label,sciGetFontDeciWidth(pSUBWIN_FEATURE(paxesmdl)->mon_y_label));
      sciSetVisibility(ppsubwin->mon_y_label, sciGetVisibility(pSUBWIN_FEATURE(paxesmdl)->mon_y_label));


      /*------------------------------------*/
      if ((ppsubwin->mon_z_label =  ConstructLabel (pobj, "",4)) == NULL){
	DestroyLabel(ppsubwin->mon_title);
	DestroyLabel(ppsubwin->mon_x_label);
	DestroyLabel(ppsubwin->mon_y_label);
	sciDelThisToItsParent (pobj, sciGetParent (pobj));
	sciDelHandle (pobj);
	FREE(pobj->pfeatures);
	FREE(pobj);
	return (sciPointObj *) NULL;
      }

      if (sciInitFontContext (ppsubwin->mon_z_label) == -1)
	{
	  DestroyLabel(ppsubwin->mon_title);
	  DestroyLabel(ppsubwin->mon_x_label);
	  DestroyLabel(ppsubwin->mon_y_label);
	  DestroyLabel(ppsubwin->mon_z_label);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pobj->pfeatures);
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}

      sciSetText(ppsubwin->mon_z_label, pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_z_label)->text.ptextstring,  
		 pLABEL_FEATURE(pSUBWIN_FEATURE(paxesmdl)->mon_z_label)->text.textlen);
      sciSetFontStyle(ppsubwin->mon_z_label, sciGetFontStyle(pSUBWIN_FEATURE(paxesmdl)->mon_z_label));
      sciSetFontDeciWidth(ppsubwin->mon_z_label,sciGetFontDeciWidth(pSUBWIN_FEATURE(paxesmdl)->mon_z_label));
      sciSetVisibility(ppsubwin->mon_z_label, sciGetVisibility(pSUBWIN_FEATURE(paxesmdl)->mon_z_label));
      
      
      pSUBWIN_FEATURE (pobj)->pPopMenu = (sciPointObj *)NULL;/* initialisation of popup menu*/
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

/* DJ.A 08/01/04 */
int C2F(graphicsmodels) ()
{
 
  integer i ,m;
  char dir;
  sciHandleTab *newhd1, *newhd2;
  sciPointObj * pobj = NULL;
  sciSubWindow * ppobj = NULL;

  if ((pfiguremdl = MALLOC ((sizeof (sciPointObj)))) == NULL)
    {
      strcpy(error_message,"Default figure cannot be create");
      return 0;  	  
    }
  sciSetEntityType (pfiguremdl, SCI_FIGURE);
  if ((pfiguremdl->pfeatures = MALLOC ((sizeof (sciFigure)))) == NULL)
    {
      FREE(pfiguremdl);
      strcpy(error_message,"Default figure cannot be create");
      return 0;
    }
  
  if ((newhd1 = MALLOC ((sizeof (sciHandleTab)))) == NULL)
    {
      FREE(pfiguremdl->pfeatures);
      FREE(pfiguremdl);
      strcpy(error_message,"Default figure cannot be create");
      return 0;
    }
  newhd1->pnext = (sciHandleTab *) NULL;
  newhd1->pprev = (sciHandleTab *) NULL;
  newhd1->index = (long)pfiguremdl;
  (sciGetRelationship (pfiguremdl))->phandle = newhd1;
    
  if (!(sciAddThisToItsParent(pfiguremdl, (sciPointObj *)NULL))) 
    {
      sciDelHandle (pfiguremdl);
      FREE(pfiguremdl->pfeatures);
      FREE(pfiguremdl);
      strcpy(error_message,"Default figure cannot be create");
      return 0;
    }
  
  sciSetCurrentSon (pfiguremdl, (sciPointObj *) NULL);

  pFIGURE_FEATURE (pfiguremdl)->relationship.psons = (sciSons *) NULL;
  pFIGURE_FEATURE (pfiguremdl)->relationship.plastsons = (sciSons *) NULL;
  pFIGURE_FEATURE (pfiguremdl)->numcolors=  NUMCOLORS_SCI;
  /** the colormap is mx3 matrix */
  m = NUMCOLORS_SCI; /* F.Leray 30.03.04 */
  if((pFIGURE_FEATURE(pfiguremdl)->pcolormap = (double *) MALLOC (m * 3 * sizeof (double))) == (double *) NULL)
    {
      sciDelHandle (pfiguremdl);
      FREE(pfiguremdl->pfeatures);
      FREE(pfiguremdl);
      strcpy(error_message,"Default figure cannot be create");
      return 0;
    }  
  for (i= 0 ; i < m ; i++)
    {
      pFIGURE_FEATURE(pfiguremdl)->pcolormap[i] = (double) (defcolors[3*i]/255.0);
      pFIGURE_FEATURE(pfiguremdl)->pcolormap[i+m] = (double) (defcolors[3*i+1]/255.0); 
      pFIGURE_FEATURE(pfiguremdl)->pcolormap[i+2*m] = (double) (defcolors[3*i+2]/255.0);
    }

  /* initialisation de context et mode graphique par defaut */
  if (sciInitGraphicContext (pfiguremdl) == -1)
    {
      sciDelHandle (pfiguremdl);
      FREE(pFIGURE_FEATURE(pfiguremdl)->pcolormap);
      FREE(pfiguremdl->pfeatures);
      FREE(pfiguremdl);
      strcpy(error_message,"Default figure cannot be create");
      return 0;
    }
  if (sciInitGraphicMode (pfiguremdl) == -1)
    {
      sciDelHandle (pfiguremdl);    
      FREE(pFIGURE_FEATURE(pfiguremdl)->pcolormap);
      FREE(pfiguremdl->pfeatures);
      FREE(pfiguremdl);
      strcpy(error_message,"Default figure cannot be create");
      return 0;
    }   

  /* F.Leray 09.04.04 */
  if (sciInitFontContext (pfiguremdl) == -1)
    {
      sciDelHandle (pfiguremdl);
      FREE(pfiguremdl->pfeatures);	  
      FREE(pfiguremdl);
      strcpy(error_message,"Default figure cannot be create");
      return 0;
    }
  
  strncpy (pFIGURE_FEATURE (pfiguremdl)->name, "Scilab Graphic (%d)", sizeof ("Scilab Graphic (%d)") + 4);
  pFIGURE_FEATURE (pfiguremdl)->namelen = Min (60, 19); 
  pFIGURE_FEATURE (pfiguremdl)->number=0;
  pFIGURE_FEATURE (pfiguremdl)->figuredimwidth = 610;
  pFIGURE_FEATURE (pfiguremdl)->figuredimheight = 461;
  pFIGURE_FEATURE (pfiguremdl)->windowdimwidth = 600;
  pFIGURE_FEATURE (pfiguremdl)->windowdimheight = 400;
  pFIGURE_FEATURE (pfiguremdl)->inrootposx = -1;
  pFIGURE_FEATURE (pfiguremdl)->inrootposy = -1;
  pFIGURE_FEATURE (pfiguremdl)->isiconified = FALSE;
  pFIGURE_FEATURE (pfiguremdl)->isselected = TRUE;
  pFIGURE_FEATURE (pfiguremdl)->rotstyle = 0;
  pFIGURE_FEATURE (pfiguremdl)->visible = TRUE;
  pFIGURE_FEATURE (pfiguremdl)->numsubwinselected = 0; 
  pFIGURE_FEATURE (pfiguremdl)->pixmap = 0; 
  pFIGURE_FEATURE (pfiguremdl)->wshow = 0; 

  /* F.Leray Adding some FontContext Info for InitFontContext function */
  /*  pFIGURE_FEATURE (pfiguremdl)->fontcontext.backgroundcolor = */

  
  if ((paxesmdl = MALLOC ((sizeof (sciPointObj)))) == NULL)
    {
      strcpy(error_message,"Default axes cannot be create");
      return 0;
    }
  sciSetEntityType (paxesmdl, SCI_SUBWIN);
  if ((paxesmdl->pfeatures = MALLOC ((sizeof (sciSubWindow)))) == NULL)
    {
      FREE(paxesmdl);
      strcpy(error_message,"Default axes cannot be create");
      return 0;
    }
  if ((newhd2 = MALLOC ((sizeof (sciHandleTab)))) == NULL)
    {
      FREE(paxesmdl->pfeatures);
      FREE(paxesmdl);
      strcpy(error_message,"Default axes cannot be create");
      return 0;
    }
  newhd2->pnext = (sciHandleTab *) NULL;
  newhd2->pprev = (sciHandleTab *) NULL;
  newhd2->index = (long)paxesmdl;
  (sciGetRelationship (paxesmdl))->phandle = newhd2;
  
  if (!(sciAddThisToItsParent (paxesmdl, pfiguremdl)))
    {
      sciDelHandle (paxesmdl);
      FREE(paxesmdl->pfeatures);
      FREE(paxesmdl);
      strcpy(error_message,"Default axes cannot be create");
      return 0;
    }
  sciSetCurrentSon (paxesmdl, (sciPointObj *) NULL);
  pSUBWIN_FEATURE (paxesmdl)->relationship.psons = (sciSons *) NULL;
  pSUBWIN_FEATURE (paxesmdl)->relationship.plastsons = (sciSons *) NULL;
  pSUBWIN_FEATURE (paxesmdl)->callback = (char *)NULL;
  pSUBWIN_FEATURE (paxesmdl)->callbacklen = 0;
  pSUBWIN_FEATURE (paxesmdl)->callbackevent = 100;
  
  if (sciInitGraphicContext (paxesmdl) == -1)
    {
      sciDelThisToItsParent (paxesmdl, sciGetParent (paxesmdl));
      sciDelHandle (paxesmdl);
      FREE(paxesmdl->pfeatures);
      FREE(paxesmdl);          
      strcpy(error_message,"Default axes cannot be create");
      return 0;
    }   
  if (sciInitGraphicMode (paxesmdl) == -1)
    {
      sciDelThisToItsParent (paxesmdl, sciGetParent (paxesmdl));
      sciDelHandle (paxesmdl);
      FREE(paxesmdl->pfeatures);
      FREE(paxesmdl);
      strcpy(error_message,"Default axes cannot be create");
      return 0;
    } 
  
  
  /* F.Leray 09.04.04 */
  if (sciInitFontContext (paxesmdl) == -1)
    {
      sciDelThisToItsParent (paxesmdl, sciGetParent (paxesmdl));
      sciDelHandle (paxesmdl);
      FREE(paxesmdl->pfeatures);	  
      FREE(paxesmdl);
      strcpy(error_message,"Default axes cannot be create");
      return 0;
    }
  
  pSUBWIN_FEATURE (paxesmdl)->logflags[0] = 'n';
  pSUBWIN_FEATURE (paxesmdl)->logflags[1] = 'n';
  
  
  /* axes labelling values*/
  pSUBWIN_FEATURE (paxesmdl)->axes.ticscolor  = -1;
  /* F.Leray 08.04.04 : No need anymore:  */
  /*pSUBWIN_FEATURE (paxesmdl)->axes.textcolor  = -1;
    pSUBWIN_FEATURE (paxesmdl)->axes.fontsize  = -1;  */
  pSUBWIN_FEATURE (paxesmdl)->axes.subint[0]  = 1;   
  pSUBWIN_FEATURE (paxesmdl)->axes.subint[1]  = 1; 
  pSUBWIN_FEATURE (paxesmdl)->axes.subint[2]  = 1;
  pSUBWIN_FEATURE (paxesmdl)->axes.xdir='d'; 
  pSUBWIN_FEATURE (paxesmdl)->axes.ydir='l';
  
  /* F.Leray 26.04.04 : Pb init axes.aaint DONE HERE: default values */
  pSUBWIN_FEATURE (paxesmdl)->axes.aaint[0] = 2;
  pSUBWIN_FEATURE (paxesmdl)->axes.aaint[1] = 10;
  pSUBWIN_FEATURE (paxesmdl)->axes.aaint[2] = 2;
  pSUBWIN_FEATURE (paxesmdl)->axes.aaint[3] = 10;
   
  pSUBWIN_FEATURE (paxesmdl)->axes.rect  = 1;
  for (i=0 ; i<7 ; i++)
    pSUBWIN_FEATURE (paxesmdl)->axes.limits[i]  = 0;
  
  pSUBWIN_FEATURE (paxesmdl)->cube_scaling = FALSE;
  for (i=0 ; i<3 ; i++)  pSUBWIN_FEATURE (paxesmdl)->grid[i]  = -1;
  pSUBWIN_FEATURE (paxesmdl)->isaxes  = FALSE;
  pSUBWIN_FEATURE (paxesmdl)->alpha  = 0.0;
  pSUBWIN_FEATURE (paxesmdl)->theta  = 270.0;
  pSUBWIN_FEATURE (paxesmdl)->alpha_kp  = 45.0;
  pSUBWIN_FEATURE (paxesmdl)->theta_kp  = 215.0;
  pSUBWIN_FEATURE (paxesmdl)->is3d  = FALSE;
  pSUBWIN_FEATURE (paxesmdl)->FirstPlot = TRUE;
  pSUBWIN_FEATURE (paxesmdl)->with_leg = 0;

  dir= 'd'; pSUBWIN_FEATURE (paxesmdl)->axes.xdir=dir;
  dir= 'l'; pSUBWIN_FEATURE (paxesmdl)->axes.ydir=dir;      
  for (i=0 ; i<4 ; i++){  
    pSUBWIN_FEATURE (paxesmdl)->axes.xlim[i]= Cscale.xtics[i]; 
    pSUBWIN_FEATURE (paxesmdl)->axes.ylim[i]= Cscale.ytics[i]; }
  
  pSUBWIN_FEATURE (paxesmdl)->axes.zlim[0]= -1.0;
  pSUBWIN_FEATURE (paxesmdl)->axes.zlim[1]= 1.0;
  
  pSUBWIN_FEATURE (paxesmdl)->axes.flag[0]= 2;
  pSUBWIN_FEATURE (paxesmdl)->axes.flag[1]= 2;
  pSUBWIN_FEATURE (paxesmdl)->axes.flag[2]= 4;
 
  pSUBWIN_FEATURE (paxesmdl)->project[0]= 1;
  pSUBWIN_FEATURE (paxesmdl)->project[1]= 1;
  pSUBWIN_FEATURE (paxesmdl)->project[2]= 0;
  pSUBWIN_FEATURE (paxesmdl)->hiddencolor=4;
  pSUBWIN_FEATURE (paxesmdl)->hiddenstate=0;
  pSUBWIN_FEATURE (paxesmdl)->isoview= FALSE; 
  pSUBWIN_FEATURE (paxesmdl)->facetmerge = FALSE;

  pSUBWIN_FEATURE (paxesmdl)->WRect[0]   = 0;
  pSUBWIN_FEATURE (paxesmdl)->WRect[1]   = 0;
  pSUBWIN_FEATURE (paxesmdl)->WRect[2]   = 1;
  pSUBWIN_FEATURE (paxesmdl)->WRect[3]   = 1;

  pSUBWIN_FEATURE (paxesmdl)->ARect[0]   = 0.125;
  pSUBWIN_FEATURE (paxesmdl)->ARect[1]   = 0.125;
  pSUBWIN_FEATURE (paxesmdl)->ARect[2]   = 0.125;
  pSUBWIN_FEATURE (paxesmdl)->ARect[3]   = 0.125;
  
  pSUBWIN_FEATURE (paxesmdl)->FRect[0]   = 0.0;
  pSUBWIN_FEATURE (paxesmdl)->FRect[1]   = 0.0;
  pSUBWIN_FEATURE (paxesmdl)->FRect[2]   = 1.0;
  pSUBWIN_FEATURE (paxesmdl)->FRect[3]   = 1.0;
  pSUBWIN_FEATURE (paxesmdl)->FRect[4]   = -1.0;
  pSUBWIN_FEATURE (paxesmdl)->FRect[5]   = 1.0;
  
  pSUBWIN_FEATURE (paxesmdl)->SRect[0]   = 0.0;
  pSUBWIN_FEATURE (paxesmdl)->SRect[1]   = 1.0;
  pSUBWIN_FEATURE (paxesmdl)->SRect[2]   = 0.0;
  pSUBWIN_FEATURE (paxesmdl)->SRect[3]   = 1.0;
  pSUBWIN_FEATURE (paxesmdl)->SRect[4]   = -1.0;
  pSUBWIN_FEATURE (paxesmdl)->SRect[5]   = 1.0;  
  
  pSUBWIN_FEATURE (paxesmdl)->tight_limits = FALSE;
  
  pSUBWIN_FEATURE (paxesmdl)->isselected = FALSE;

  pSUBWIN_FEATURE (paxesmdl)->visible = sciGetVisibility(pfiguremdl); 
  pSUBWIN_FEATURE (paxesmdl)->isclip = -1;
  
  pSUBWIN_FEATURE (paxesmdl)->pPopMenu = (sciPointObj *)NULL;
  
  /* F.Leray 10.06.04 */
  /* Adding default Labels inside Axes */
  /*------------------------------------------------------------------------------------*/
 
  pobj = paxesmdl;
  ppobj = pSUBWIN_FEATURE(paxesmdl);
  
  /******************************  title *************************/
  
  if ((ppobj->mon_title = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_title, SCI_LABEL);
  if (((ppobj->mon_title)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_title));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_title)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
    }
  
  if (!(sciAddThisToItsParent ((ppobj->mon_title), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_title));
      FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
    }
      
  sciSetCurrentSon ((ppobj->mon_title), (sciPointObj *) NULL);
      
  pLABEL_FEATURE ((ppobj->mon_title))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_title))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_title))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_title))->text.callbacklen = 0;
  /*   pLABEL_FEATURE ((ppobj->mon_title))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_title))); */
  pLABEL_FEATURE ((ppobj->mon_title))->visible = sciGetVisibility(paxesmdl);
  
  if ((pLABEL_FEATURE ((ppobj->mon_title))->text.ptextstring =calloc (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_title), sciGetParent ((ppobj->mon_title)));
      sciDelHandle ((ppobj->mon_title));
      FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
    }
  /* init a "" pour le chanmp title */
  strcpy (pLABEL_FEATURE ((ppobj->mon_title))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_title))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_title))->ptype = 1; /* type = 1 <=> title, 2 <=> x_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_title))->text.fontcontext.textorientation = 0;

  /*   pLABEL_FEATURE ((ppobj->mon_title))->titleplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_title))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_title)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_title))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_title), sciGetParent ((ppobj->mon_title)));
      sciDelHandle ((ppobj->mon_title));
      FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
    }


      
  /******************************  x_label *************************/
  
  if ((ppobj->mon_x_label = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_x_label, SCI_LABEL);
  if (((ppobj->mon_x_label)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_x_label));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_x_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }
    
  if (!(sciAddThisToItsParent ((ppobj->mon_x_label), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_x_label));
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }
      
  sciSetCurrentSon ((ppobj->mon_x_label), (sciPointObj *) NULL);
      
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_x_label))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.callbacklen = 0;
 /*  pLABEL_FEATURE ((ppobj->mon_x_label))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_x_label))); */
  pLABEL_FEATURE ((ppobj->mon_x_label))->visible = sciGetVisibility(paxesmdl);
     

  if ((pLABEL_FEATURE ((ppobj->mon_x_label))->text.ptextstring =calloc (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_x_label), sciGetParent ((ppobj->mon_x_label)));
      sciDelHandle ((ppobj->mon_x_label));
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }
  /* init a "" pour le chanmp x_label */
  strcpy (pLABEL_FEATURE ((ppobj->mon_x_label))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_x_label))->ptype = 2; /* type = 1 <=> x_label, 2 <=> x_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.fontcontext.textorientation = 0;

  /*   pLABEL_FEATURE ((ppobj->mon_x_label))->x_labelplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_x_label))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_x_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_x_label))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_x_label), sciGetParent ((ppobj->mon_x_label)));
      sciDelHandle ((ppobj->mon_x_label));
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }


  
  /******************************  y_label *************************/
  
  if ((ppobj->mon_y_label = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_y_label, SCI_LABEL);
  if (((ppobj->mon_y_label)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_y_label));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_y_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }
    
  if (!(sciAddThisToItsParent ((ppobj->mon_y_label), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_y_label));
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }
      
  sciSetCurrentSon ((ppobj->mon_y_label), (sciPointObj *) NULL);
      
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_y_label))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.callbacklen = 0;
 /*  pLABEL_FEATURE ((ppobj->mon_y_label))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_y_label))); */
  pLABEL_FEATURE ((ppobj->mon_y_label))->visible = sciGetVisibility(paxesmdl);
  

  if ((pLABEL_FEATURE ((ppobj->mon_y_label))->text.ptextstring =calloc (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_y_label), sciGetParent ((ppobj->mon_y_label)));
      sciDelHandle ((ppobj->mon_y_label));
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }
  /* init a "" pour le chanmp y_label */
  strcpy (pLABEL_FEATURE ((ppobj->mon_y_label))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_y_label))->ptype = 3; /* type = 1 <=> y_label, 2 <=> y_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.fontcontext.textorientation = 0;

  /*   pLABEL_FEATURE ((ppobj->mon_y_label))->y_labelplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_y_label))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_y_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_y_label))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_y_label), sciGetParent ((ppobj->mon_y_label)));
      sciDelHandle ((ppobj->mon_y_label));
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }

  
  /******************************  z_label *************************/
  
  if ((ppobj->mon_z_label = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_z_label, SCI_LABEL);
  if (((ppobj->mon_z_label)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_z_label));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_z_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }
    
  if (!(sciAddThisToItsParent ((ppobj->mon_z_label), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_z_label));
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }
      
  sciSetCurrentSon ((ppobj->mon_z_label), (sciPointObj *) NULL);
      
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_z_label))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.callbacklen = 0;
 /*  pLABEL_FEATURE ((ppobj->mon_z_label))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_z_label))); */
  pLABEL_FEATURE ((ppobj->mon_z_label))->visible = sciGetVisibility(paxesmdl);
    

  if ((pLABEL_FEATURE ((ppobj->mon_z_label))->text.ptextstring =calloc (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_z_label), sciGetParent ((ppobj->mon_z_label)));
      sciDelHandle ((ppobj->mon_z_label));
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }
  /* init a "" pour le chanmp z_label */
  strcpy (pLABEL_FEATURE ((ppobj->mon_z_label))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_z_label))->ptype = 4; /* type = 1 <=> z_label, 2 <=> z_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.fontcontext.textorientation = 0;

  /*   pLABEL_FEATURE ((ppobj->mon_z_label))->z_labelplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_z_label))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_z_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_z_label))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_z_label), sciGetParent ((ppobj->mon_z_label)));
      sciDelHandle ((ppobj->mon_z_label));
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }

  
  return 1;
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
	       double y, double *wh)
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
      if (wh != (double *)NULL){
	pTEXT_FEATURE (pobj)->wh[0] = wh[0];
	pTEXT_FEATURE (pobj)->wh[1] = wh[1];
      }
      pTEXT_FEATURE (pobj)->z = 0.0; /**DJ.Abdemouche 2003**/
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
	 && ((int) sciGetEntityType(subwinparent) != -1));
  if ((int) sciGetEntityType(subwinparent) == -1)
    return (sciPointObj *)NULL;
  if (!(pobj = ConstructText (subwinparent, sciGetText(pthis), sciGetTextLength(pthis), 
			      sciGetTextPosX(pthis), sciGetTextPosY(pthis),0))){
    return (sciPointObj *)NULL;
  }
  else{
    sciSetCurrentObj(pobj);;} /* F.Leray Adding 26.03.04*/
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
  pTEXT_FEATURE (pobj)->wh[0] = pTEXT_FEATURE (pthis)->wh[0];
  pTEXT_FEATURE (pobj)->wh[1] = pTEXT_FEATURE (pthis)->wh[1];	
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
ConstructPolyline (sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pvecz,
		   int closed, int n1, int n2,int plot)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciPolyline *ppoly = (sciPolyline *) NULL;
  /* Adding F.Leray */
  /* sciPointObj *psubwin = ( sciPointObj *) NULL;*/
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
      ppoly->xmax   = pvecx[0];  /* Adding F.Leray 07.04.04 */
      ppoly->ymax   = pvecy[0];  /* Adding F.Leray 07.04.04 */
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
      /**DJ.Abdemouche 2003**/
      if (pvecz == (double *) NULL)
	{
	  /* Change here: F.Leray 25.06.04 */
	  /* Whenever pvecz == NULL, we create a 0. array of size n1 */
	  /* in order to better allow 3d switching with all polyline_style modes*/
	  /* Indeed, some modes require pvz knowledge. */

	  /*ppoly->pvz = (double *) NULL;*/
	  if ((ppoly->pvz = MALLOC (n1 * sizeof (double))) == NULL)
	    {
	      FREE(pPOLYLINE_FEATURE (pobj)->pvx);
	      FREE(pPOLYLINE_FEATURE (pobj)->pvector);
	      FREE(pPOLYLINE_FEATURE (pobj)->pvy);
	      sciDelThisToItsParent (pobj, sciGetParent (pobj));
	      sciDelHandle (pobj);
	      FREE(pPOLYLINE_FEATURE(pobj));
	      FREE(pobj);
	      return (sciPointObj *) NULL;
	    } 
	  for (i = 0; i < n1; i++)
	    ppoly->pvz[i] = 0.;
	  
	  ppoly->zmin   = 0.;
	  ppoly->zmax   = 0.;
	}
      else
	{
	  if ((ppoly->pvz = MALLOC (n1 * sizeof (double))) == NULL)
	    {
	      FREE(pPOLYLINE_FEATURE (pobj)->pvx);
	      FREE(pPOLYLINE_FEATURE (pobj)->pvector);
	      FREE(pPOLYLINE_FEATURE (pobj)->pvy);
	      sciDelThisToItsParent (pobj, sciGetParent (pobj));
	      sciDelHandle (pobj);
	      FREE(pPOLYLINE_FEATURE(pobj));
	      FREE(pobj);
	      return (sciPointObj *) NULL;
	    } 
	  for (i = 0; i < n1; i++)
	    ppoly->pvz[i] = pvecz[i];
	  
	  ppoly->zmin   = pvecz[0];  /* Adding F.Leray 07.04.04 */
	  ppoly->zmax   = pvecz[0];  /* Adding F.Leray 07.04.04 */

	  /* Adding F.Leray 07.04.04 : Find the min and max for z*/
	  for (i = 0; i < n1; i++)
	    {
	      ppoly->zmin   = Min(ppoly->pvz[i], ppoly->zmin);
	      ppoly->zmax   = Max(ppoly->pvz[i], ppoly->zmax);
	    }

	}
      ppoly->width  = fabs(xmax - ppoly->xmin);
      ppoly->height = fabs(ymax - ppoly->ymin); 
      ppoly->n1 = n1;		/* memorisation du nombre des courbes */
      ppoly->n2 = n2;		/* memorisation du nombre de points */
      ppoly->closed = closed;
      ppoly->plot = plot; 
      ppoly->xmax = xmax;  /** xmax Adding  F.Leray 02.04.04 */
      ppoly->ymax = ymax;  /** ymax Adding  F.Leray 02.04.04 */

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
	 && ((int)sciGetEntityType(subwinparent) != -1));
  if ((int)sciGetEntityType(subwinparent) == -1)
    return (sciPointObj *)NULL;
  /* DJ.A 2003 */
  if (!(pobj = ConstructPolyline (subwinparent, pPOLYLINE_FEATURE(pthis)->pvx, pPOLYLINE_FEATURE(pthis)->pvy,pPOLYLINE_FEATURE(pthis)->pvz,
				  pPOLYLINE_FEATURE(pthis)->closed, pPOLYLINE_FEATURE(pthis)->n1,pPOLYLINE_FEATURE(pthis)->n2,pPOLYLINE_FEATURE(pthis)->plot))){
    return (sciPointObj *)NULL;
  }
  else {
    sciSetCurrentObj(pobj);}; /* F.Leray Adding 26.03.04*/

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
  if (pPOLYLINE_FEATURE (pthis)->pvz != NULL) /**DJ.Abdemouche 2003**/
    FREE (pPOLYLINE_FEATURE (pthis)->pvz);
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
      pGRAYPLOT_FEATURE (pobj)->pvecx = (double *)NULL;
      pGRAYPLOT_FEATURE (pobj)->pvecy = (double *)NULL;

      strncpy (pGRAYPLOT_FEATURE (pobj)->datamapping, "scaled", 6);
      pgray = pGRAYPLOT_FEATURE (pobj);
      
      if (pvecx && (pgray->pvecx = MALLOC (n1 * sizeof (double))) == NULL)
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pGRAYPLOT_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      cmpt = (type == 2)? 4:n2 ;
      if (type != 2)
	if (pvecy && (pgray->pvecy = MALLOC (cmpt * sizeof (double))) == NULL)
	  {
	    if (pvecx) FREE(pGRAYPLOT_FEATURE (pobj)->pvecx);
	    sciDelThisToItsParent (pobj, sciGetParent (pobj));
	    sciDelHandle (pobj);
	    FREE(pGRAYPLOT_FEATURE(pobj));
	    FREE(pobj);
	    return (sciPointObj *) NULL;
	  }
      if ((pgray->pvecz = MALLOC ((n1*n2) * sizeof (double))) == NULL){
	if (pvecx) FREE(pGRAYPLOT_FEATURE (pobj)->pvecx);
	if (pvecy) FREE(pGRAYPLOT_FEATURE (pobj)->pvecy);
	sciDelThisToItsParent (pobj, sciGetParent (pobj));
	sciDelHandle (pobj);
	FREE(pGRAYPLOT_FEATURE(pobj));
	FREE(pobj);
	return (sciPointObj *) NULL;
      }
      if (pvecx) {
	for (i = 0; i < n1; i++) pgray->pvecx[i] = pvecx[i];
      } 
   

      if (pvecy) {
	if (type != 2)
	  for (i = 0; i < n2; i++) pgray->pvecy[i] = pvecy[i];
      }
    
      pgray->nx = n1;pgray->ny = n2;
      for (i = 0; i < (n1*n2); i++) pgray->pvecz[i] = pvecz[i];
	
      if (sciInitGraphicContext (pobj) == -1)
	{
	  if (pvecx) FREE(pGRAYPLOT_FEATURE (pobj)->pvecx);
	  if (pvecy) FREE(pGRAYPLOT_FEATURE (pobj)->pvecy); 
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

      /* psegs->arrowsize = 50.;  */ /* default value F.Leray 25.03.04*/
          
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
      pSEGS_FEATURE (pobj)->vz=(double *) NULL; /**DJ.Abdemouche 2003**/
      psegs->ptype = type;

      /* F.Leray Test imprortant sur type ici*/
      if (type == 0) /* attention ici type = 0 donc...*/
	{   
	  psegs->arrowsize = arsize /** 100*/;       /* A revoir: F.Leray 06.04.04 */
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
      else /* attention ici type = 1 donc...*/
	{ 
	  /* Rajout de psegs->arrowsize = arsize; F.Leray 18.02.04*/
	  psegs->arrowsize = arsize /* * 100 */;
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
	      FREE(pSEGS_FEATURE (pobj)->vfx); 
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
	  pSEGS_FEATURE (pobj)->vfz=(double *) NULL; /**DJ.Abdemouche 2003**/
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
  if (pSEGS_FEATURE (pthis)->vz != (double *)NULL) 
    FREE (pSEGS_FEATURE (pthis)->vz);  
  if (pSEGS_FEATURE (pthis)->ptype ==0) 
    {
      FREE(pSEGS_FEATURE (pthis)->pstyle);
    } 
  else 
    {
	FREE(pSEGS_FEATURE (pthis)->vfx); pSEGS_FEATURE (pthis)->vfx = NULL;
	FREE(pSEGS_FEATURE (pthis)->vfy); pSEGS_FEATURE (pthis)->vfy = NULL;
	FREE(pSEGS_FEATURE (pthis)->vfz); pSEGS_FEATURE (pthis)->vfz = NULL;
    } 
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
      pARC_FEATURE (pobj)->z = 0;
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
	 && ((int)sciGetEntityType(subwinparent) != -1));
  if ((int)sciGetEntityType(subwinparent) == -1)
    return (sciPointObj *)NULL;
  if (!(pobj = ConstructArc (subwinparent, pARC_FEATURE(pthis)->x, 
			     pARC_FEATURE(pthis)->y, pARC_FEATURE(pthis)->height,pARC_FEATURE(pthis)->width,
			     pARC_FEATURE(pthis)->alphabegin, pARC_FEATURE(pthis)->alphaend,-1,0))){
    return (sciPointObj *)NULL;
  }
  else {
    sciSetCurrentObj(pobj);}; /* F.Leray Adding 26.03.04*/
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
      pRECTANGLE_FEATURE (pobj)->z = 0; 
      pRECTANGLE_FEATURE (pobj)->height = height;
      pRECTANGLE_FEATURE (pobj)->width = width;
      pRECTANGLE_FEATURE (pobj)->fillflag = fillflag; 
      pRECTANGLE_FEATURE (pobj)->fillcolor = fillcolor; 
      pRECTANGLE_FEATURE (pobj)->str = str;
      pRECTANGLE_FEATURE (pobj)->strheight = 0;
      pRECTANGLE_FEATURE (pobj)->strwidth = 0;
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
	 && ((int)sciGetEntityType(subwinparent) != -1));
  if ((int)sciGetEntityType(subwinparent) == -1)
    return (sciPointObj *)NULL;
  if (!(pobj = ConstructRectangle (subwinparent, pRECTANGLE_FEATURE(pthis)->x, 
				   pRECTANGLE_FEATURE(pthis)->y, pRECTANGLE_FEATURE(pthis)->height,pRECTANGLE_FEATURE(pthis)->width, 
				   pRECTANGLE_FEATURE(pthis)->horzcurvature, pRECTANGLE_FEATURE(pthis)->vertcurvature,0,0,0))){
    return (sciPointObj *)NULL;
  }
  else {
    sciSetCurrentObj(pobj);}; /* F.Leray Adding 26.03.04*/
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
		  integer izcol, integer dimzx, integer dimzy,  
		  integer *flag, double *ebox,integer flagcolor, 
		  integer *isfac, integer *m1, integer *n1, integer *m2, 
		  integer *n2, integer *m3, integer *n3, integer *m3n, integer *n3n)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  /*debug F.Leray*/
  sciSurface *psurf;

  int i=0, j=0;
  int nx,ny,nz,nc,izc=izcol;

  if (typeof3d == SCI_PLOT3D) {
    nx=dimzx;
    ny=dimzy;
    nz=dimzx*dimzy;
    if (flagcolor == 2)
      nc=nz; /* one color per facet */    /* nc = dimzx * dimzy */
    else if (flagcolor == 3)
      nc=nz*4; /*one color per edge */    /* nc = 4* dimzx * dimzy ?????? */ /* 3 or 4 vectices are needed: 
									I think we take 4 to have enough allocated memory*/ 
    /* made by Djalel : comes from the genfac3d case*/
    else 
      nc=0;
  }
  /* DJ.A 2003 */
  else { /* case SCI_FAC3D */
    nx=dimzx*dimzy;
    ny=dimzx*dimzy;
    nz=dimzx*dimzy;
    if (flagcolor == 2)
      nc=dimzy; /* one color per facet */ /* nc = dimzy */
    else if (flagcolor == 3)
      nc=nz; /*one color per edge */      /* nc = dimzx * dimzy */
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
	  FREE(pobj); pobj = NULL;
	  return (sciPointObj *) NULL;
	}
      /*sciSetParent (pobj, pparentsubwin);*/
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pSURFACE_FEATURE (pobj));
	  FREE(pobj); pobj = NULL;
	  return (sciPointObj *) NULL;
	}
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pSURFACE_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pSURFACE_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;

      pSURFACE_FEATURE (pobj)->callback = (char *)NULL;
      pSURFACE_FEATURE (pobj)->callbacklen = 0;
      pSURFACE_FEATURE (pobj)->callbackevent = 100; 
      pSURFACE_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj)); 
 

      /*debug F.Leray*/
      psurf = pSURFACE_FEATURE (pobj);
  
      /*F.Leray 12.03.04 Adding here to know the length of arrays pvecx, pvecy and pvecz*/
      pSURFACE_FEATURE (pobj)->nc = nc;
      pSURFACE_FEATURE (pobj)->nx = nx;
      pSURFACE_FEATURE (pobj)->ny = ny;
      pSURFACE_FEATURE (pobj)->nz = nz;
      pSURFACE_FEATURE (pobj)->isfac = *isfac;
      pSURFACE_FEATURE (pobj)->m1= *m1;
      pSURFACE_FEATURE (pobj)->m2= *m2;
      pSURFACE_FEATURE (pobj)->m3= *m3;
      pSURFACE_FEATURE (pobj)->n1= *n1;
      pSURFACE_FEATURE (pobj)->n2= *n2;
      pSURFACE_FEATURE (pobj)->n3= *n3;
      
      /*Adding F.Leray 19.03.04*/
      pSURFACE_FEATURE (pobj)->m3n= *m3n;
      pSURFACE_FEATURE (pobj)->n3n= *n3n;

      if (((pSURFACE_FEATURE (pobj)->pvecx = MALLOC ((nx * sizeof (double)))) == NULL))
	{
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pSURFACE_FEATURE (pobj));
	  FREE(pobj); pobj = NULL;
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
	  FREE(pobj); pobj = NULL;
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
	  FREE(pobj); pobj = NULL;
	  return (sciPointObj *) NULL;
	}
      else
	{
	  for (j = 0;j < nz; j++)
	    pSURFACE_FEATURE (pobj)->pvecz[j] = pvecz[j];
	}

      /*Storage of the input Color Matrix or Vector Data */ /* F.Leray 23.03.04*/
      pSURFACE_FEATURE (pobj)->inputCMoV = NULL;
      if (((pSURFACE_FEATURE (pobj)->inputCMoV = MALLOC (( (*m3n)*(*n3n) * sizeof (integer)))) == NULL))
	{
	  FREE(pSURFACE_FEATURE (pobj)->pvecy); pSURFACE_FEATURE (pobj)->pvecy = NULL;
	  FREE(pSURFACE_FEATURE (pobj)->pvecx); pSURFACE_FEATURE (pobj)->pvecx = NULL;
	  FREE(pSURFACE_FEATURE (pobj)->pvecz); pSURFACE_FEATURE (pobj)->pvecz = NULL;
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pSURFACE_FEATURE (pobj));
	  FREE(pobj); pobj = NULL;
	  return (sciPointObj *) NULL;
	}
      
      for (j = 0;j < (*m3n)*(*n3n); j++)  
	pSURFACE_FEATURE (pobj)->inputCMoV[j] = zcol[j];
      
      /* Init. zcol to NULL F.Leray 17.03.04*/
      pSURFACE_FEATURE (pobj)->zcol = NULL;
      
      /*
      if (izc !=0&&nc>0 ) {
	if (((pSURFACE_FEATURE (pobj)->zcol = MALLOC ((nc * sizeof (integer)))) == NULL))
	  {
	    FREE(pSURFACE_FEATURE (pobj)->pvecy); pSURFACE_FEATURE (pobj)->pvecy = NULL;
	    FREE(pSURFACE_FEATURE (pobj)->pvecx); pSURFACE_FEATURE (pobj)->pvecx = NULL;
	    FREE(pSURFACE_FEATURE (pobj)->pvecz); pSURFACE_FEATURE (pobj)->pvecz = NULL;
	    sciDelThisToItsParent (pobj, sciGetParent (pobj));
	    sciDelHandle (pobj);
	    FREE(pSURFACE_FEATURE (pobj));
	    FREE(pobj); pobj = NULL;
	    return (sciPointObj *) NULL;
	  }
	else
	  {
	    if (izcol !=0)
	      for (j = 0;j < nc; j++)  
	      pSURFACE_FEATURE (pobj)->zcol[j]= zcol[j];  */ /* DJ.A 2003 */
      /*}
	} */
      
      /*-------Replaced by: --------*/

      if (izc !=0&&nc>0 ) { /* Allocation of good size depending on flagcolor for nc (see above)*/
	if (((pSURFACE_FEATURE (pobj)->zcol = MALLOC ((nc * sizeof (integer)))) == NULL))
	  {
	    FREE(pSURFACE_FEATURE (pobj)->pvecy); pSURFACE_FEATURE (pobj)->pvecy = NULL;
	    FREE(pSURFACE_FEATURE (pobj)->pvecx); pSURFACE_FEATURE (pobj)->pvecx = NULL;
	    FREE(pSURFACE_FEATURE (pobj)->pvecz); pSURFACE_FEATURE (pobj)->pvecz = NULL;
	    sciDelThisToItsParent (pobj, sciGetParent (pobj));
	    sciDelHandle (pobj);
	    FREE(pSURFACE_FEATURE (pobj));
	    FREE(pobj); pobj = NULL;
	    return (sciPointObj *) NULL;
	  }
      }
      
      if(nc>0)
	{
	  /* case flagcolor == 2*/
	  if(flagcolor==2 && ( *m3n==1 || *n3n ==1)) /* it means we have a vector in Color input: 1 color per facet in input*/
	    {
	      /* We have just enough information to fill the pSURFACE_FEATURE (pobj)->zcol array*/
	      for (j = 0;j < nc; j++)  /* nc value is dimzx*dimzy == m3 * n3 */
		pSURFACE_FEATURE (pobj)->zcol[j]= pSURFACE_FEATURE (pobj)->inputCMoV[j];  /* DJ.A 2003 */
		  }
	  else if(flagcolor==2 && !( *m3n==1 || *n3n ==1)) /* it means we have a matrix in Color input: 1 color per vertex in input*/
	    {
	      /* We have too much information and we take only the first dimzy colors to fill the pSURFACE_FEATURE (pobj)->zcol array*/
	      /* NO !! Let's do better; F.Leray 08.05.04 : */
	      /* We compute the average value (sum of the value of the nf=m3n vertices on a facet) / (nb of vertices per facet which is nf=m3n) */
	      /* in our example: m3n=4 and n3n=400 */
	      for (j = 0;j < nc; j++)   /* nc value is dimzy*/
		{
		  double tmp = 0;
		  int ii=0;
		  for(ii=0;ii<(*m3n);ii++)
		    tmp = tmp +  pSURFACE_FEATURE (pobj)->inputCMoV[j*(*m3n) + ii];
		  tmp = tmp / (*m3n);
		  pSURFACE_FEATURE (pobj)->zcol[j]= (integer)tmp;
		}
	    }
	  /* case flagcolor == 3*/
	  else if(flagcolor==3 && ( *m3n==1 || *n3n ==1)) /* it means we have a vector in Color input: 1 color per facet in input*/
	    {
	      /* We have insufficient info. to fill the entire zcol array of dimension nc = dimzx*dimzy*/
	      /* We repeat the data:*/
	      for(i = 0; i< dimzy; i++){
		for (j = 0;j < dimzx; j++)  /* nc value is dimzx*dimzy == m3 * n3 */
		  pSURFACE_FEATURE (pobj)->zcol[dimzx*i+j]= pSURFACE_FEATURE (pobj)->inputCMoV[i];  /* DJ.A 2003 */
		    }
	    }
	  else if(flagcolor==3 && !( *m3n==1 || *n3n ==1)) /* it means we have a matrix in Color input: 1 color per vertex in input*/
	    {
	      /* We have just enough information to fill the pSURFACE_FEATURE (pobj)->zcol array*/
	      for (j = 0;j < nc; j++)   /* nc value is dimzy*/
		pSURFACE_FEATURE (pobj)->zcol[j]= pSURFACE_FEATURE (pobj)->inputCMoV[j];
	    }
	}
      /*-------END Replaced by: --------*/

      pSURFACE_FEATURE (pobj)->dimzx = dimzx; /* dimzx is completly equal to m3*/
      pSURFACE_FEATURE (pobj)->dimzy = dimzy; /* dimzx is completly equal to n3*/
      pSURFACE_FEATURE (pobj)->izcol = izc;
      pSURFACE_FEATURE (pobj)->pproj = NULL;	/* Les projections ne sont pas encore calculees */
      pSURFACE_FEATURE (pobj)->isselected = TRUE;

      pSURFACE_FEATURE (pobj)->flag[0] = flag[0]; /* F.Leray 16.04.04 HERE We store the flag=[mode (hidden part ), type (scaling), box (frame around the plot)] */
      pSURFACE_FEATURE (pobj)->flag[1] = flag[1];
      pSURFACE_FEATURE (pobj)->flag[2] = flag[2];

      /* DJ.A 2003 */
   
      pSURFACE_FEATURE (pobj)->ebox[0] = ebox[0];
      pSURFACE_FEATURE (pobj)->ebox[1] = ebox[1];
      pSURFACE_FEATURE (pobj)->ebox[2] = ebox[2];
      pSURFACE_FEATURE (pobj)->ebox[3] = ebox[3];
      pSURFACE_FEATURE (pobj)->ebox[4] = ebox[4];
      pSURFACE_FEATURE (pobj)->ebox[5] = ebox[5];
      pSURFACE_FEATURE (pobj)->flagcolor =flagcolor;
      pSURFACE_FEATURE (pobj)->typeof3d = typeof3d;
      pSURFACE_FEATURE (pobj)->hiddencolor = pSUBWIN_FEATURE(pparentsubwin)->hiddencolor;

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
  sciPointObj *psubwin, *pobj;
  sciSons *psonstmp;
  integer cmpt;
  
  psubwin = (sciPointObj *) sciGetParentSubwin(pthis);
  FREE(pSURFACE_FEATURE (pthis)->pvecz);
  FREE(pSURFACE_FEATURE (pthis)->pvecy);
  FREE(pSURFACE_FEATURE (pthis)->pvecx);
  FREE(pSURFACE_FEATURE (pthis)->inputCMoV); /* Adding F.Leray 24.03.04*/
  if (pSURFACE_FEATURE (pthis)->izcol != 0 ) 
    FREE(pSURFACE_FEATURE (pthis)->zcol);

  /* DJ.A 2003 */
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  /*FREE (pSURFACE_FEATURE (pthis)->pproj);*/
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  cmpt=0;
  psonstmp = sciGetSons (psubwin);
  while (psonstmp != (sciSons *) NULL)	
    {   
      if(sciGetEntityType (psonstmp->pointobj) == SCI_SURFACE) 
	cmpt=cmpt+1;
      psonstmp = psonstmp->pnext;
    }
  if (cmpt < 2){
    if ((pobj= sciGetMerge(psubwin)) != (sciPointObj *) NULL)
      DestroyMerge(pobj); 
  }
  else
    Merge3d(psubwin);
  /* on peut alors detruire l'entite merge */

  return 0;
}



/**ConstructAxis
 * @memo This function creates Axis structure
 */
/*
  sciPointObj *
  ConstructAxis (sciPointObj * pparentsubwin, char *strflag, int style, double minx,
  double miny, double minz, double maxx, double maxy,
  double maxz) */ /* Apparently this Constructor is unused (except in CloneaAxis) F.Leray 10.03.04*/
  /*
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
  pAXIS_FEATURE (pobj)->isclip = -1; 

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
*/


/**CloneAxis
 * @memo This function destroies Text structure and only this to destroy all sons use DelGraphicsSon
 * @param sciPointObj * pthis: the pointer to the entity
 */

/*
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
*/

/**DestroyAxis
 * @memo This function destroies axis and the elementaries structures and only this to destroy all sons use DelGraphicsSon
 */
/*
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
  
return 0;
}
*/


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
      /*pAXES_FEATURE (pobj)->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); */
       pAXES_FEATURE (pobj)->isclip = -1;  /*F.Leray Change here: by default Axis are not clipped. 10.03.04 */

     
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
     

      /* pAXES_FEATURE(pobj)->str = str;*/ /* Pb here, F.Leray : Weird init.: can not copy a string using '='*/
      if(str != (char **) NULL)
	{
	  if ((pAXES_FEATURE(pobj)->str= malloc (Max(nx,ny) * sizeof (char*))) == NULL)
	    return (sciPointObj *) NULL;

	  for(i=0;i<Max(nx,ny);i++) 
	    {
	      if(str[i] != (char *) NULL)
		{
		  if((pAXES_FEATURE (pobj)->str[i] = malloc( (strlen(str[i])+1) * sizeof(char))) == NULL)
		    return (sciPointObj *) NULL;
		  else
		    strcpy(pAXES_FEATURE (pobj)->str[i],str[i]);
		}
	      else
		pAXES_FEATURE (pobj)->str[i] = (char *) NULL;
	    }
	}
      else
	{
	  pAXES_FEATURE (pobj)->str = (char **) NULL;
	}

      pAXES_FEATURE (pobj)->subint = subint;
      pAXES_FEATURE (pobj)->fontsize =fontsize; 
      pAXES_FEATURE (pobj)->textcolor =textcolor;
      pAXES_FEATURE (pobj)->ticscolor =ticscolor;
      pAXES_FEATURE (pobj)->seg =seg;    
      /*    pAXES_FEATURE (pobj)->format =format; */ /* Pb here, F.Leray : Weird init.: can not copy a string using '='*/
      pAXES_FEATURE (pobj)->logscale=logscale;
	  if(format != (char *) NULL)
	  {
	    if((pAXES_FEATURE (pobj)->format = malloc( (strlen(format)+1) * sizeof(char))) == NULL)
	      return (sciPointObj *) NULL;
	    else
	      strcpy(pAXES_FEATURE (pobj)->format,format);
	  }
	  else
	    pAXES_FEATURE (pobj)->format = (char *) NULL;
	  
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
      /*   case SCI_AXIS  */
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
  char str[2] = "xv"/*,locstr*/;
  integer n,n1,uc,verbose=0,narg,xz[10],na,arssize,sflag=0,un=1;
  integer *xm, *ym,*zm,n2 = 1, xtmp[4], ytmp[4], *pstyle/*,rect1[4]*/;
  integer closeflag = 0,ias,ias1;
  integer width, height;
  double anglestr,w2,h2,as;
  double xx[2],yy[2];   
  integer px1[2],py1[2],pn1=1,pn2=2;
  integer nn1,nn2, arsize,lstyle,iflag;
  double arsize1=5.0,arsize2=5.0,dv;
  integer angle1, angle2;
  integer x1, yy1, w1, h1, wstr,hstr/*,hh1*/;
  integer x[6], v;
  integer xold[5], vold = 0, flagx = 0;
  sciSons *psonstmp;
  integer itmp[5];		
  integer markidsizeold[2], markidsizenew[2];
  sciPointObj /* *psubwin, */ *currentsubwin;
/*   double locx,locy,loctit; */
  char logflags[4];
  double xbox[8],ybox[8],zbox[8], *xzz,*yzz,*zzz;
  static integer InsideU[4],InsideD[4];
  	
  integer xxx[6];
  int fontstyle_zero = 0; /*To fill Sci_Axis for Axes objects */
  integer isoflag =0;     /*  for 3d isoview mode*/

  sciAxes *paxes = (sciAxes *) NULL;

  int i,j;
  /* variable pour le set_scale update_frame_bounds*/
  double subwin[4], framevalues[4];

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
     flag_DO = MaybeSetWinhdc();
#endif

     C2F(dr)("xset","pixmap",&(pFIGURE_FEATURE (pobj)->pixmap),PI0,PI0,PI0,PI0,PI0,PD0,
	     PD0,PD0,PD0,0L,0L);
     if (pFIGURE_FEATURE (pobj)->pixmap == 0){
       /* Change background BEFORE xclear F.Leray */
       C2F(dr)("xset","background",&x[1],PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,7L); 
       C2F (dr) ("xclear", "v", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0,0L, 0L);
     }
     else
       C2F (dr) ("xset","wwpc", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0,0L, 0L);

     C2F(dr)("xset","background",&x[1],PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,7L); /* Change background F.Leray*/
     C2F(dr)("xset","alufunction",&(sciGetScilabXgc (pobj)->CurDrawFunction),PI0,PI0,PI0,
	     PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
#ifdef WIN32
     if ( flag_DO == 1) ReleaseWinHdc();
#endif
     psonstmp = sciGetLastSons (pobj);
     while (psonstmp != (sciSons *) NULL) {
       if (sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN) sciDrawObj (psonstmp->pointobj);
       psonstmp = psonstmp->pprev;
     }
     break;
   case SCI_SUBWIN: 
     if (!sciGetVisibility(pobj)) break;
     sciSetSelectedSubWin(pobj); 
     
     set_scale ("tttttt", pSUBWIN_FEATURE (pobj)->WRect, pSUBWIN_FEATURE (pobj)->FRect,
		pSUBWIN_FEATURE (pobj)->axes.aaint, pSUBWIN_FEATURE (pobj)->logflags, 
		pSUBWIN_FEATURE (pobj)->ARect); 
     
     
     if (pSUBWIN_FEATURE (pobj)->is3d) 
       {  /* 3D Coordinates */ /* verifier si c'est encore utile SS */
	 /*To have directly all the possible ISOVIEW Modes*/
	 isoflag = (long)(pSUBWIN_FEATURE (pobj)->axes.flag[1]+1)/2; 
	 
	 if(pSUBWIN_FEATURE (pobj)->isoview == TRUE) {
	   if(isoflag ==2 || isoflag == 3){		}
	   else {
	     if((pSUBWIN_FEATURE (pobj)->axes.flag[1] == 0)
		|| (pSUBWIN_FEATURE (pobj)->axes.flag[1] == 2))
	       /* The default isoview mode is type=4 3d isometric bounds 
		  derived from the data, to similarily type=2  */
	       pSUBWIN_FEATURE (pobj)->axes.flag[1] = 4; 
	     else if(pSUBWIN_FEATURE (pobj)->axes.flag[1] == 1)
	       pSUBWIN_FEATURE (pobj)->axes.flag[1] = 3;
	   }
	 }
	 else {
	   if((pSUBWIN_FEATURE (pobj)->axes.flag[1] == 3) 
	      || (pSUBWIN_FEATURE (pobj)->axes.flag[1] == 5))
	     pSUBWIN_FEATURE (pobj)->axes.flag[1] = 1; /* computed from ebox*/
	   else if((pSUBWIN_FEATURE (pobj)->axes.flag[1] == 4) 
		   || (pSUBWIN_FEATURE (pobj)->axes.flag[1] == 6))
	     /* The default NON-isoview mode is 2 computed from data*/
	     pSUBWIN_FEATURE (pobj)->axes.flag[1] = 2; 
	 }
	 
	 axis_3ddraw(pobj,xbox,ybox,zbox,InsideU,InsideD); 
	 
	 psonstmp = sciGetLastSons (pobj);
	 while (psonstmp != (sciSons *) NULL) {
	   sciDrawObj (psonstmp->pointobj);
	   psonstmp = psonstmp->pprev;
	 }
	 triedre(pobj,xbox,ybox,zbox,InsideU,InsideD);
	 wininfo("alpha=%.1f,theta=%.1f",pSUBWIN_FEATURE (pobj)->alpha,pSUBWIN_FEATURE (pobj)->theta); 
       }/***/
     else /* we are in 2D mode...*/
       {
	 x[0] = sciGetForeground (pobj);
	 x[2] = sciGetLineWidth (pobj);
	 x[3] = sciGetLineStyle (pobj);
	 markidsizenew[0] = sciGetMarkStyle(pobj);
	 markidsizenew[1] = sciGetLineWidth (pobj);x[4] = 0;v = 0;dv = 0;
	 
#ifdef WIN32
	 flag_DO=MaybeSetWinhdc();
#endif
	 C2F (dr) ("xset","dashes",x,x,x+4,x+4,x+4,&v,&dv,&dv,&dv,&dv,5L,4096);
	 C2F (dr) ("xset","foreground",x,x,x+4,x+4,x+4,&v,&dv,&dv,&dv,&dv,5L,4096);
	 C2F (dr) ("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	 C2F (dr) ("xset","mark",&markidsizenew[0],&markidsizenew[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	 
	 sci_update_frame_bounds(0);
	 
	 if (pSUBWIN_FEATURE (pobj)->isaxes) {
	   char STRFLAG[4];
	   rebuild_strflag(pobj,STRFLAG);
	   
	   
	   /*  sciprint("JUSTE AVANT axis_draw...\n"); */
	   /* 	    sciprint("Cscale.ytics[0] = %f\n",Cscale.ytics[0]); */
	   /* 	    sciprint("Cscale.ytics[1] = %f\n",Cscale.ytics[1]); */
	   /* 	    sciprint("Cscale.ytics[2] = %f\n",Cscale.ytics[2]); */
	   /* 	    sciprint("Cscale.ytics[3] = %f\n",Cscale.ytics[3]); */
	   
	   
	   axis_draw (STRFLAG); 
	   labels2D_draw(pobj);
	   /*  sciprint("JUSTE APRES axis_draw...\n"); */
	   /* 	    sciprint("Cscale.ytics[0] = %f\n",Cscale.ytics[0]); */
	   /* 	    sciprint("Cscale.ytics[1] = %f\n",Cscale.ytics[1]); */
	   /* 	    sciprint("Cscale.ytics[2] = %f\n",Cscale.ytics[2]); */
	   /* 	    sciprint("Cscale.ytics[3] = %f\n",Cscale.ytics[3]); */
	   
	 }
	 /** walk subtree **/
	 psonstmp = sciGetLastSons (pobj);
	 while (psonstmp != (sciSons *) NULL) {
	   sciDrawObj (psonstmp->pointobj);
	   psonstmp = psonstmp->pprev;
	 }
	 
#ifdef WIN32
	 if ( flag_DO == 1) ReleaseWinHdc();
#endif
	 wininfo("");  
       }
     break;                      
     /******************/
	  
   case SCI_AGREG: 
     
      if (!sciGetVisibility(pobj)) break;
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj) )->facetmerge) break;  
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
      /* sciSetCurrentObj (pobj);	F.Leray 25.03.04*/
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

      xxx[0] = sciGetFontForeground (pobj);/*la dash est de la meme couleur que le foreground*/
      xxx[2] = sciGetFontDeciWidth (pobj)/100;
      xxx[3] = 0;
      xxx[4] = sciGetFontStyle(pobj);
      
#ifdef WIN32 
      flag_DO=MaybeSetWinhdc();
#endif
      C2F (dr1) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
		 &dv, &dv, &dv, 5L, 4096);
      C2F (dr1) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
		 &dv, &dv, &dv, &dv, 5L, 4096);

      C2F(dr)("xset","font",xxx+4,xxx+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L); /* Adding F.Leray*/


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
      /*sciSetCurrentObj(pobj); F.Leray 25.03.04*/
      Legends(pLEGEND_FEATURE (pobj)->pstyle, &(pLEGEND_FEATURE (pobj)->nblegends), sciGetText(pobj));
       
          
      /* restore the graphic context */

      C2F (dr1) ("xset", "dashes", &xold[0], &vold, &vold, &vold, &vold, &v,
		 &dv, &dv, &dv, &dv, 5L, 6L);
      C2F (dr1) ("xset", "foreground", &xold[1], &vold, &vold, &vold, &vold,
		 &v, &dv, &dv, &dv, &dv, 5L, 10L);
#ifdef WIN32 
      if ( flag_DO == 1) ReleaseWinHdc ();
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
      flag_DO=MaybeSetWinhdc();
#endif   
      newfec(xm,ym,pFEC_FEATURE (pobj)->pnoeud,pFEC_FEATURE (pobj)->pfun,
	     &pFEC_FEATURE (pobj)->Nnode,&pFEC_FEATURE (pobj)->Ntr,
	     pFEC_FEATURE (pobj)->zminmax,pFEC_FEATURE (pobj)->colminmax);
#ifdef WIN32
      if ( flag_DO == 1) ReleaseWinHdc();
#endif
	  
      /* FREE(xm);FREE(ym); */ /* SS 02/04 */
      FREE(xm); xm = (integer *) NULL;
      FREE(ym); ym = (integer *) NULL; /* et F.Leray 18.02.04*/

      break;      
      /******************************** 22/05/2002 ***************************/    
    case SCI_SEGS:    
        
      if (!sciGetVisibility(pobj)) break;
      
      sciClip(sciGetIsClipping(pobj)); 

      /* load the object foreground and dashes color */
      x[0] = sciGetForeground(pobj); /* Adding F.leray 27.04.04 */
      x[2] = sciGetLineWidth (pobj);
      x[3] = sciGetLineStyle (pobj);
      x[4] = 0;
      markidsizenew[0] =  sciGetMarkStyle(pobj);;
      markidsizenew[1] =  sciGetLineWidth (pobj);;

#ifdef WIN32 
      flag_DO=MaybeSetWinhdc();
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
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif 

      if (pSEGS_FEATURE (pobj)->ptype == 0) /* ptype == 0 */
        {  
	  n=pSEGS_FEATURE (pobj)->Nbr1;
	  /* F.Leray 18.02.04 Correction suivante ANNULEE:
	     in1 = pSEGS_FEATURE (pobj)->Nbr1; ANNULATION MODIF DU 18.02.04
	     in2 = pSEGS_FEATURE (pobj)->Nbr2;*/
	  arsize = (integer) (pSEGS_FEATURE (pobj)->arrowsize); 
	  if ((xm = MALLOC (n*sizeof (integer))) == NULL)	return -1;
	  if ((ym = MALLOC (n*sizeof (integer))) == NULL)	return -1; /* F.Leray 18.02.04 Correction suivante:*/
	  /*	if ((xm = MALLOC (in1*sizeof (integer))) == NULL)	return -1;*/ /* ANNULATION MODIF DU 18.02.04*/
	  /*	if ((ym = MALLOC (in2*sizeof (integer))) == NULL)	return -1;*/
	  if ((pstyle = MALLOC (n*sizeof (integer))) == NULL)	return -1; /* SS 19.04*/
	  if (pSEGS_FEATURE (pobj)->iflag == 1) {
	    for ( i =0 ; i <n ; i++) {
	      pstyle[i]=sciGetGoodIndex(pobj, pSEGS_FEATURE (pobj)->pstyle[i]);
	    }
	  }
	  else
	    pstyle[0]=sciGetGoodIndex(pobj, pSEGS_FEATURE (pobj)->pstyle[0]);
	  if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
	    {
	      trans3d(sciGetParentSubwin(pobj),n,xm,ym,pSEGS_FEATURE (pobj)->vx,pSEGS_FEATURE (pobj)->vy,pSEGS_FEATURE (pobj)->vz);
	   
	    }
	  else
	    {
	      for ( i =0 ; i <n ; i++) {
		xm[i]= XScale(pSEGS_FEATURE (pobj)->vx[i]); 
		ym[i]= YScale(pSEGS_FEATURE (pobj)->vy[i]);}
	    } 
#ifdef WIN32 
	  flag_DO = MaybeSetWinhdc();
#endif
	  if (pSEGS_FEATURE (pobj)->arrowsize == 0)
	    C2F(dr)("xsegs","v",xm,ym,&n,pstyle,&pSEGS_FEATURE (pobj)->iflag,
		    PI0,PD0,PD0,PD0,PD0,0L,0L);
	  else{ 
	    if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
	      {  
		as=pSEGS_FEATURE (pobj)->arrowsize;
		C2F(echelle2dl)(&as,&as,&ias,&ias1,&un,&un,"f2i"); 
		if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->isoview)
		  ias=ias*10;
		else 
		  ias =ias/2;
		C2F(dr)("xarrow","v",xm,ym,&n,&ias,pstyle,&pSEGS_FEATURE (pobj)->iflag,PD0,PD0,PD0,PD0,0L,0L);
 	      }
	    else
	      C2F(dr1)("xarrow","v",pstyle,&pSEGS_FEATURE (pobj)->iflag
		       ,&n,PI0,PI0,PI0,pSEGS_FEATURE (pobj)->vx,pSEGS_FEATURE (pobj)->vy,&pSEGS_FEATURE (pobj)->arrowsize,PD0,0L,0L);
	    /* F.Leray appel bizarre ci dessus a dr1 pour le NG??!! A voir... 19.02.04*/
	    /* TEST 13.05.04 avec C2F(dr)("xarrow",... ne marche pas: pourquoi? Que fait dr1 en plus que ne fait pas dr en nouveau graphique ?? */
	  } /***/
#ifdef WIN32 
	  if ( flag_DO == 1) ReleaseWinHdc ();
#endif 
 	  FREE(xm);         xm = (integer *) NULL;
	  FREE(ym);         ym = (integer *) NULL; 
	  FREE(pstyle); pstyle = (integer *) NULL; /* SS 19.04*/
	}
      else    /*ptype == 1*/
        {
#ifdef WIN32 
	  flag_DO = MaybeSetWinhdc();
#endif
	  /*verbose = 1; */ /* debug F.Leray release mode */
	  C2F(dr)("xget","use color",&verbose, &uc, &narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
	  if (uc)
		C2F(dr)("xget","color",&verbose,xz,&narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
	  else
	    C2F(dr)("xget","line style",&verbose,xz,&narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
#ifdef WIN32 
	  if ( flag_DO == 1) ReleaseWinHdc ();
#endif 

	  /*n=2*(pSEGS_FEATURE (pobj)->Nbr1)*(pSEGS_FEATURE (pobj)->Nbr2); F.Leray 17.02.04*/
	  n=2*(pSEGS_FEATURE (pobj)->Nbr1)*((pSEGS_FEATURE (pobj)->Nbr2)+1);
	 

	  /* On laisse tomber le graphic_alloc ICI F.Leray 20.02.04*/
	 /* 	  xm = graphic_alloc(0,n,sizeof(int));
		  ym = graphic_alloc(1,n,sizeof(int)); */

	  if ((xm = MALLOC (n*sizeof (integer))) == NULL)	return -1;
	  if ((ym = MALLOC (n*sizeof (integer))) == NULL)	return -1;

	  zm = NULL;/* SS 02/04 */
	  if ( xm == NULL || ym == NULL) 
	    {
	      sciprint("Running out of memory \n");
	      return -1;
	    }      
	  if ( pSEGS_FEATURE (pobj)->pcolored != 0) {
	    /*	    zm = graphic_alloc(2,n/2,sizeof(int)); */ /* F.Leray a voir le n/2... 20.02.04 */
	  if ((zm = MALLOC (((int) (n/2))*sizeof (integer))) == NULL)	return -1;
	    
	    if (  zm == NULL ) 
	      {
		sciprint("Running out of memory \n");
		return -1;
	      }      
	  }
	  /* Prototype de Champ2DRealToPixel:
extern void Champ2DRealToPixel(xm,ym,zm,na,arsize,colored,x,y,fx,fy,n1,n2,arfact)

     integer *xm,*ym,*zm;
     integer *na,*arsize,*colored;
     integer *n1,*n2;
     double *x, *y, *fx, *fy;
     double *arfact;
	  */

	  Champ2DRealToPixel(xm,ym,zm,&na,&arssize,&(pSEGS_FEATURE (pobj)->pcolored),
			     pSEGS_FEATURE (pobj)->vx,pSEGS_FEATURE (pobj)->vy,pSEGS_FEATURE (pobj)->vfx,
			     pSEGS_FEATURE (pobj)->vfy,&(pSEGS_FEATURE (pobj)->Nbr1),
			     &(pSEGS_FEATURE (pobj)->Nbr2),&(pSEGS_FEATURE (pobj)->parfact));
#ifdef WIN32 
	  flag_DO = MaybeSetWinhdc();
#endif

	  /* F.Leray Addings here 24.03.04*/
	 /*  if (pSEGS_FEATURE (pobj)->arrowsize > 1) */
/* 	    arssize = (int) pSEGS_FEATURE (pobj)->arrowsize; */

	  /** size of arrow **/
	  if (pSEGS_FEATURE (pobj)->arrowsize >= 1){
	    arsize1= ((double) Cscale.WIRect1[2])/(5*(pSEGS_FEATURE (pobj)->Nbr1));
	    arsize2= ((double) Cscale.WIRect1[3])/(5*(pSEGS_FEATURE (pobj)->Nbr2));
	    arssize=  (arsize1 < arsize2) ? inint(arsize1*10.0) : inint(arsize2*10.0) ;
	    arssize = (int)((arssize)*(pSEGS_FEATURE (pobj)->arrowsize));
	  }
	  
	  if ( pSEGS_FEATURE (pobj)->pcolored ==0)
		C2F(dr)("xarrow","v",xm,ym,&na,&arssize,xz,(sflag=0,&sflag),&dv,&dv,&dv,&dv,0L,0L);
	  else
	    C2F(dr)("xarrow","v",xm,ym,&na,&arssize,zm,(sflag=1,&sflag),&dv,&dv,&dv,&dv,0L,0L);   
#ifdef WIN32 
	  if ( flag_DO == 1) ReleaseWinHdc ();
#endif 
	  FREE(xm) ; xm = (integer *) NULL;
	  FREE(ym) ; ym = (integer *) NULL;/* SS 02/04 */ /* et F.Leray 18.02.04*/
	  if ( pSEGS_FEATURE (pobj)->pcolored != 0) 
	    {
	      FREE(zm); zm = (integer *) NULL;/* F.Leray 1802.04 modif ici*/
	    }
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
	    FREE(xm); xm = (integer *) NULL; return -1; 
	  }
 
	  for ( i =0 ; i < n1 ; i++)  
	    xm[i]= XScale(pGRAYPLOT_FEATURE (pobj)->pvecx[i]); 
	  for ( i =0 ; i < n2 ; i++)  
	    ym[i]= YScale(pGRAYPLOT_FEATURE (pobj)->pvecy[i]);   
#ifdef WIN32
	  flag_DO = MaybeSetWinhdc();
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
	  if ( flag_DO == 1) ReleaseWinHdc();
#endif

	  /*  FREE(xm);FREE(ym); */ /* SS 03/01/03 */
	  FREE(xm); xm = (integer *) NULL; /* F.Leray c'est mieux.*/
	  FREE(ym); ym = (integer *) NULL;
	  break;
	case 1:
	  if ((xm = MALLOC (n2*sizeof (integer))) == NULL) 
	    return -1;
	  if ((ym = MALLOC (n1*sizeof (integer))) == NULL){
	    FREE(xm);xm = (integer *) NULL; return -1;  /* F.Leray Rajout de xm = (integer *) NULL; 18.02.04*/
	  }

          for ( j =0 ; j < n2 ; j++) xm[j]= XScale(j+0.5);
          for ( j =0 ; j < n1 ; j++) ym[j]= YScale(((n1-1)-j+0.5));
#ifdef WIN32
	  flag_DO = MaybeSetWinhdc();
#endif
          frame_clip_on(); 
          GraySquare1(xm,ym,pGRAYPLOT_FEATURE (pobj)->pvecz,n1,n2);  
          frame_clip_off();  
          C2F(dr)("xrect","v",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],
		  &Cscale.WIRect1[3],PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
#ifdef WIN32
	  if ( flag_DO == 1) ReleaseWinHdc();
#endif

	  /*  FREE(xm);FREE(ym); */ /* SS 03/01/03 */
	  FREE(xm); xm = (integer *) NULL; /* F.Leray c'est mieux.*/
	  FREE(ym); ym = (integer *) NULL;
          break;
	case 2:
	  if ((xm = MALLOC (n2*sizeof (integer))) == NULL) 
	    return -1;
	  if ((ym = MALLOC (n1*sizeof (integer))) == NULL){
	    FREE(xm);xm = (integer *) NULL; return -1; /* F.Leray Rajout de xm = (integer *) NULL; 18.02.04*/
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
	  flag_DO = MaybeSetWinhdc();
#endif

	  frame_clip_on(); 
	  GraySquare1(xm,ym,pGRAYPLOT_FEATURE (pobj)->pvecz,n1,n2); 
	  frame_clip_off();
#ifdef WIN32
	  if ( flag_DO == 1) ReleaseWinHdc();
#endif
	  /*	  FREE(xm);FREE(ym); */ /* SS 03/01/03 */
	  FREE(xm); xm = (integer *) NULL; /* F.Leray c'est mieux.*/
	  FREE(ym); ym = (integer *) NULL;
	  break;
	default:
	  break;
	}
      break; 
    case SCI_POLYLINE: 
     
      if (!sciGetVisibility(pobj)) break;

      /*sciSetCurrentObj (pobj);	  F.Leray 25.03.04 */
      
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
      flag_DO = MaybeSetWinhdc();
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
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif  
      n1 = pPOLYLINE_FEATURE (pobj)->n1;
      n2 = pPOLYLINE_FEATURE (pobj)->n2;
      closeflag = pPOLYLINE_FEATURE (pobj)->closed;    
      
      if ((xm = MALLOC ((2*n1*n2)*sizeof (integer))) == NULL)	return -1;
      if ((ym = MALLOC ((2*n1*n2)*sizeof (integer))) == NULL)	return -1;
      if ((xzz = MALLOC ((2*n1*n2)*sizeof (double))) == NULL)	return -1;
      if ((yzz = MALLOC ((2*n1*n2)*sizeof (double))) == NULL)	return -1;
      if ((zzz = MALLOC ((2*n1*n2)*sizeof (double))) == NULL)	return -1;
      /***/
      sciClip(sciGetIsClipping(pobj));
#ifdef WIN32 
      flag_DO = MaybeSetWinhdc ();
#endif

      /**DJ.Abdemouche 2003**/
      switch (pPOLYLINE_FEATURE (pobj)->plot)
	{
	case 0:
	  if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
	    trans3d(sciGetParentSubwin(pobj),n1,xm,ym,pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy,pPOLYLINE_FEATURE (pobj)->pvz);
	  else
	    C2F (echelle2d) (pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy, xm, ym, &n1, &n2, "f2i",3L); 
	  /**DJ.Abdemouche 2003**/
	  break; 
	case 1:
	  if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
	    trans3d(sciGetParentSubwin(pobj),n1,xm,ym,pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy,pPOLYLINE_FEATURE (pobj)->pvz);
	  else
	    Plo2d1RealToPixel(&n2,&n1,pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy,xm,ym,logflags);  
	  break;
	case 2:
	  if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
	    {
	      Plo2dTo3d(2,&n2,&n1,pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy,pPOLYLINE_FEATURE (pobj)->pvz,xzz,yzz,zzz);
	      trans3d(sciGetParentSubwin(pobj),n1*2,xm,ym,xzz,yzz,zzz);
	    }
	  else
	    {
	      Plo2d2RealToPixel(&n2,&n1,pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy,xm,ym,logflags);
	    }
	  n1=n1*2;
	  break;
	case 3:  
	  if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
	    {
	      Plo2dTo3d(3,&n2,&n1,pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy,pPOLYLINE_FEATURE (pobj)->pvz,xzz,yzz,zzz);
	      trans3d(sciGetParentSubwin(pobj),n1*2,xm,ym,xzz,yzz,zzz);
	    }
	  else
	    {
	      Plo2d3RealToPixel(&n2,&n1,pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy,xm,ym,logflags); 
	    }
	  for ( j = 0 ; j < n2 ; j++)
	    {
	      lstyle=x[0];
	      iflag=0; nn1= n1*2;
	      C2F(dr)("xsegs","v",&xm[2*n1*j],&ym[2*n1*j],&nn1,&lstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    }
	  /**DJ.Abdemouche 2003**/
	  n1=n2;
	  break;
	case 4: 
	  if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
	    {
	      Plo2dTo3d(4,&n2,&n1,pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy,pPOLYLINE_FEATURE (pobj)->pvz,xzz,yzz,zzz);
	      trans3d(sciGetParentSubwin(pobj),n1*2,xm,ym,xzz,yzz,zzz);
	    }
	  else
	    {
	      Plo2d4RealToPixel(&n2,&n1,pPOLYLINE_FEATURE (pobj)->pvx,pPOLYLINE_FEATURE (pobj)->pvy,xm,ym,logflags);  
	    }
	  nn2=2*(n1)-1;
	  arsize1= Cscale.WIRect1[2]/70.0;arsize2= Cscale.WIRect1[3]/70.0;
	  arsize=  (arsize1 < arsize2) ? inint(10*arsize1) : inint(10*arsize2) ;
	  for ( j = 0 ; j < n2 ; j++)
	    {
	      integer lstyle=sciGetMarkStyle(pobj) ,iflag=0;
	      C2F(dr)("xarrow","v",&xm[2*n1*j],&ym[2*n1*j],&nn2,&arsize,&lstyle,&iflag,PD0,PD0,PD0,PD0,0L,0L); 
	    } 
	  break;
	case 5:
	  if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
	    trans3d(sciGetParentSubwin(pobj),n1,xm,ym,pPOLYLINE_FEATURE (pobj)->pvx,
		    pPOLYLINE_FEATURE (pobj)->pvy,pPOLYLINE_FEATURE (pobj)->pvz);
	  else
	    C2F (echelle2d) (pPOLYLINE_FEATURE (pobj)->pvx,
			     pPOLYLINE_FEATURE (pobj)->pvy, xm, ym, &n1, &n2, "f2i",3L);
	  sciClip(sciGetIsClipping(pobj));
	  C2F (dr) ("xarea", str, &n1, xm, ym, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));

	  break;
	default:
	  sciprint ("This Polyline cannot be drawn !\n");
#ifdef WIN32 
	  if ( flag_DO == 1) ReleaseWinHdc ();
#endif  
	  break;     
	}
      FREE(xzz); xzz = (double *) NULL;
      FREE(yzz); yzz = (double *) NULL;
      FREE(zzz); zzz = (double *) NULL;/* SS 02/04 */
      if (! sciGetIsMark(pobj))
	C2F (dr) ("xlines", "xv", &n1, xm, ym, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
      else
	C2F (dr) ("xmarks", "xv", &n1, xm, ym, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 8L, 2L);
#ifdef WIN32 
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif  
      sciUnClip(sciGetIsClipping(pobj));
      FREE (xm); xm = (integer *) NULL;
      FREE (ym); ym = (integer *) NULL;
      break;
    case SCI_ARC: 
     
      if (!sciGetVisibility(pobj)) break;
      /*sciSetCurrentObj (pobj);	F.Leray 25.03.04 */
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
      flag_DO = MaybeSetWinhdc ();
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
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif
      /**DJ.Abdemouche 2003**/
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
	{
	  trans3d(sciGetParentSubwin(pobj),un,&x1,&yy1,&pARC_FEATURE(pobj)->x,
		  &pARC_FEATURE (pobj)->y,&pARC_FEATURE (pobj)->z);

	}
      else
	{
	  x1 = XDouble2Pixel (pARC_FEATURE (pobj)->x);
	  yy1 = YDouble2Pixel (pARC_FEATURE (pobj)->y);
	}
      w2 = pARC_FEATURE (pobj)->width;
      h2 = pARC_FEATURE (pobj)->height; 
      /* Nouvelles fonctions de changement d'echelle pour les longueurs --> voir PloEch.h */ 
      w1 = WScale(w2);
      h1 = HScale(h2);
      angle1 = (integer) (pARC_FEATURE (pobj)->alphabegin);
      angle2 = (integer) (pARC_FEATURE (pobj)->alphaend);   
     
      
#ifdef WIN32 
      flag_DO = MaybeSetWinhdc ();
#endif
      sciClip(sciGetIsClipping(pobj));
      if (pARC_FEATURE (pobj)->fill  <= 0)
	C2F (dr) ("xarc", str, &x1, &yy1, &w1, &h1, &angle1, &angle2, PD0, PD0, PD0,PD0, 5L, 0L);
      else
	C2F (dr) ("xfarc", str, &x1, &yy1, &w1, &h1, &angle1, &angle2, PD0, PD0, PD0,PD0, 5L, 0L);
      sciUnClip(sciGetIsClipping(pobj));
#ifdef WIN32 
      if ( flag_DO == 1)  ReleaseWinHdc ();
#endif
      break;
    case SCI_RECTANGLE:  
     
      if (!sciGetVisibility(pobj)) break;

      /*sciSetCurrentObj (pobj); F.Leray 25.03.04 */
      n = 1;
      if (sciGetFillStyle(pobj) != 0)
	{       x[0] = 64;	/*la dash est de la meme couleur que le foreground*/
	x[1] = 1;
	x[2] = 0;
	x[3] = 0;
	x[4] = 0;
	x[5] = sciGetFillColor(pobj);
#ifdef WIN32 
	flag_DO = MaybeSetWinhdc ();
#endif
		 
	C2F (dr1) ("xset", "pattern", &x[5], x+3, x, x+1, x+3, &v, &dv,
		   &dv, &dv, &dv, 5L, 4096);
#ifdef WIN32 
	if ( flag_DO == 1) ReleaseWinHdc ();
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
      flag_DO = MaybeSetWinhdc ();
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
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif 
      /**DJ.Abdemouche 2003**/
      if (!(pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d))
	{
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
	  flag_DO = MaybeSetWinhdc ();
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
	  if ( flag_DO == 1)  ReleaseWinHdc ();
#endif
	}
      else
	{ 
	  double rectx[4],recty[4],rectz[4];
	  int close=1;
	  n=4;
	  xm = graphic_alloc(0,4,sizeof(int));
	  ym = graphic_alloc(1,4,sizeof(int));
	  rectx[0]= rectx[3] =pRECTANGLE_FEATURE (pobj)->x;
	  rectx[1]= rectx[2] =pRECTANGLE_FEATURE (pobj)->x+pRECTANGLE_FEATURE (pobj)->width;   
	  recty[0]= recty[1] =pRECTANGLE_FEATURE (pobj)->y;   
	  recty[2]= recty[3] =pRECTANGLE_FEATURE (pobj)->y-pRECTANGLE_FEATURE (pobj)->height;
	  rectz[0]= rectz[1]=rectz[2]= rectz[3]=pRECTANGLE_FEATURE (pobj)->z;
	  trans3d(sciGetParentSubwin(pobj),n,xm,ym,rectx,recty,rectz);
#ifdef WIN32 
	  flag_DO = MaybeSetWinhdc ();
#endif
	  sciClip(sciGetIsClipping(pobj));
	  if (! sciGetIsMark(pobj))
	    if (pRECTANGLE_FEATURE (pobj)->fillflag == 0)	
	      C2F (dr) ("xlines", "xv", &n, xm, ym, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
	    else
	      if (pRECTANGLE_FEATURE (pobj)->fillflag == 1)
		C2F (dr) ("xarea", str, &n, xm, ym, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
	      else
		sciprint("  The value must be 1  or 0\r\n");
	  else 
	    {
	      n=4;
	      C2F (dr) ("xmarks", str, &n, xm, ym, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 6L, 0L);
	    }
	  sciUnClip(sciGetIsClipping(pobj));
#ifdef WIN32 
	  if ( flag_DO == 1)  ReleaseWinHdc ();
#endif
	}
      break;
    case SCI_TEXT: 
     
      if (!sciGetVisibility(pobj)) break;
      /*sciSetCurrentObj (pobj);	F.Leray 25.03.04 */
      n = 1;
      /* load the object foreground and dashes color */
      x[0] = sciGetFontForeground (pobj);/*la dash est de la meme couleur que le foreground*/
      x[2] = sciGetFontDeciWidth (pobj)/100;
      x[3] = 0;
      x[4] = sciGetFontStyle(pobj);
      v = 0;
      dv = 0;
#ifdef WIN32 
      flag_DO = MaybeSetWinhdc ();
#endif
      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
      C2F(dr)("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);
#ifdef WIN32 
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif
      /**DJ.Abdemouche 2003**/

      flagx = 0;
#ifdef WIN32 
      flag_DO = MaybeSetWinhdc ();
#endif
      sciClip(sciGetIsClipping(pobj));

      if (pTEXT_FEATURE (pobj)->fill==-1) {
	if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
	  {trans3d(sciGetParentSubwin(pobj),n,&x1,&yy1,
		   &pTEXT_FEATURE (pobj)->x,&pTEXT_FEATURE (pobj)->y,&pTEXT_FEATURE (pobj)->z);}
	else {
	  x1  = XDouble2Pixel (pTEXT_FEATURE (pobj)->x);
	  yy1 = YDouble2Pixel (pTEXT_FEATURE (pobj)->y);
	}
	anglestr = (sciGetFontOrientation (pobj)/10); 	
	/* *10 parce que l'angle est concerve en 1/10eme de degre*/

	C2F(dr)("xstring",sciGetText (pobj),&x1,&yy1,PI0,&flagx,PI0,PI0,&anglestr, PD0,PD0,PD0,0L,0L);
      }
      else { /* SS for xstringb should be improved*/
	integer w1, h1;
	w1  = XDouble2Pixel (pTEXT_FEATURE (pobj)->wh[0]);
	h1 = YDouble2Pixel (pTEXT_FEATURE (pobj)->wh[1]);
        C2F(dr1)("xstringb",sciGetText (pobj),&(pTEXT_FEATURE (pobj)->fill),&v,&v,&v,&v,&v,
		 &(pTEXT_FEATURE (pobj)->x),&(pTEXT_FEATURE (pobj)->y),
		 &(pTEXT_FEATURE (pobj)->wh[0]),&(pTEXT_FEATURE (pobj)->wh[1]),9L,0L);
      } 

      sciUnClip(sciGetIsClipping(pobj));
#ifdef WIN32 
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif
      break;
      /*   case SCI_AXIS 
      break; */
/*     case SCI_TITLE: */
     
/*       if (!sciGetVisibility(pobj)) break; */
/*       /\*sciSetCurrentObj (pobj);       F.Leray 25.03.04*\/ */
/*       /\* load the object foreground and dashes color *\/ */
/*       x[0] = sciGetFontForeground (pobj); */
/*       x[2] = sciGetFontDeciWidth (pobj)/100; */
/*       x[3] = 0; */
/*       x[4] = sciGetFontStyle(pobj); */
/*       v = 0; */
/*       dv = 0; */
/* #ifdef WIN32 */
/*       flag_DO = MaybeSetWinhdc (); */
/* #endif */
/*       C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L); */
/*       C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L); */
/*       C2F(dr)("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L); */
/* #ifdef WIN32 */
/*       if ( flag_DO == 1) ReleaseWinHdc (); */
/* #endif */
/*       sciClip(sciGetIsClipping(pobj)); */
/*       flagx = 0; */
/*       anglestr = 0; */
/*       psubwin = sciGetParentSubwin(pobj); */
/*       if (sciGetEntityType(psubwin) == SCI_SUBWIN) */
/* 	{ */
/* 	  locstr=pSUBWIN_FEATURE(psubwin)->axes.xdir; */
/* 	  switch (locstr) */
/* 	    { */
/* 	    case 'd': */
/* 	      locy=pSUBWIN_FEATURE (psubwin)->FRect[1]; */
/* 	      //loctit=pSUBWIN_FEATURE (psubwin)->FRect[3]; */
/* 	      break; */
/* 	    case 'c': */
/* 	      locy=(pSUBWIN_FEATURE (psubwin)->FRect[1]>0.0)?pSUBWIN_FEATURE (psubwin)->FRect[1]: 0.0; */
/* 	      locy=(pSUBWIN_FEATURE (psubwin)->FRect[3]<0.0)?pSUBWIN_FEATURE (psubwin)->FRect[1]: locy; */
/* 	      //loctit=pSUBWIN_FEATURE (psubwin)->FRect[3]; */
/* 	      break; */
/* 	    case 'u': */
/* 	      locy=pSUBWIN_FEATURE (psubwin)->FRect[3]; */
/* 	      //loctit=pSUBWIN_FEATURE (psubwin)->FRect[1]; */
/* 	      break; */
/* 	    default: */
/* 	      locy=pSUBWIN_FEATURE (psubwin)->FRect[1]; */
/* 	      //loctit=pSUBWIN_FEATURE (psubwin)->FRect[3]; */
/* 	      break; */
/* 	    } */
/* 	  locstr=pSUBWIN_FEATURE(psubwin)->axes.ydir; */
/* 	  switch (locstr) */
/* 	    { */
/* 	    case 'l': */
/* 	      locx=pSUBWIN_FEATURE (psubwin)->FRect[0]; */
/* 	      break; */
/* 	    case 'c': */
/* 	      locx=(pSUBWIN_FEATURE (psubwin)->FRect[0]>0.0)?pSUBWIN_FEATURE (psubwin)->FRect[0]: 0.0; */
/* 	      locx=(pSUBWIN_FEATURE (psubwin)->FRect[2]<0.0)?pSUBWIN_FEATURE (psubwin)->FRect[0]: locx; */
/* 	      //loctit=pSUBWIN_FEATURE (psubwin)->FRect[1]; */
/* 	      break; */
/* 	    case 'r': */
/* 	      locx=pSUBWIN_FEATURE (psubwin)->FRect[2]; */
/* 	      break; */
/* 	    default: */
/* 	      locx=pSUBWIN_FEATURE (psubwin)->FRect[0]; */
/* 	      break; */
/* 	    } */
/* 	} */
/*       switch (pTITLE_FEATURE (pobj)->ptype) */
/* 	{ */
/* 	case 1: /\* main title *\/ /\* We fix the title always at the top *\/ */
/* 	  rect1[0]= Cscale.WIRect1[0]; //XScale(pSUBWIN_FEATURE (psubwin)->FRect[0]); */
/* 	  rect1[1]= Cscale.WIRect1[1]; //(loctit==pSUBWIN_FEATURE (psubwin)->FRect[1])?(YScale(loctit)+Cscale.WIRect1[3]/6):YScale(loctit); */
/* 	  rect1[2]= Cscale.WIRect1[2]; */
/* 	  rect1[3]= Cscale.WIRect1[3]/6; */
/* #ifdef WIN32 */
/* 	  flag_DO = MaybeSetWinhdc (); */
/* #endif */
/* 	  C2F(dr1)("xstringtt",sciGetText (pobj),&rect1[0],&rect1[1],&rect1[2],&rect1[3],&v,&v,&dv,&dv,&dv,&dv,10L,0L); */
/* #ifdef WIN32 */
/* 	  if ( flag_DO == 1) ReleaseWinHdc (); */
/* #endif */
/* 	  break; */
/* 	case 2: /\* x label *\/ */
/* 	  if(!pSUBWIN_FEATURE (psubwin)->is3d){ /\* display for 2D mode only *\/ */
/* 	    rect1[0]= Cscale.WIRect1[0]+Cscale.WIRect1[2]; */
/* 	    rect1[1]= Cscale.WIRect1[1]+ Cscale.WIRect1[3];//YScale(locy-(pSUBWIN_FEATURE (psubwin)->FRect[3]-pSUBWIN_FEATURE (psubwin)->FRect[1])/12); */
/* 	    rect1[2]= Cscale.WIRect1[2]/6; */
/* 	    rect1[3]= Cscale.WIRect1[3]/6; */
/* #ifdef WIN32 */
/* 	    flag_DO = MaybeSetWinhdc (); */
/* #endif */
/* 	    C2F(dr1)("xstringtt",sciGetText (pobj),&rect1[0],&rect1[1],&rect1[2],&rect1[3],&v,&v,&dv,&dv,&dv,&dv,10L,0L); */
/* #ifdef WIN32 */
/* 	    if ( flag_DO == 1) ReleaseWinHdc (); */
/* #endif */
/* 	  } */
/* 	  break; */
/* 	case 3: /\* y label *\/ */
/* /\* 	  if(!pSUBWIN_FEATURE (psubwin)->is3d){ /\\* display for 2D mode only *\\/ *\/ */
/* /\* 	    rect1[0]= XScale(locx-(pSUBWIN_FEATURE (psubwin)->FRect[2]-pSUBWIN_FEATURE (psubwin)->FRect[0])/12); *\/ */
/* /\* 	    rect1[1]= Cscale.WIRect1[1]-Cscale.WIRect1[3]/24; *\/ */
/* /\* 	    rect1[2]= Cscale.WIRect1[2]/6; *\/ */
/* /\* 	    rect1[3]= Cscale.WIRect1[3]/12; *\/ */
/* /\* #ifdef WIN32 *\/ */
/* /\* 	  flag_DO = MaybeSetWinhdc (); *\/ */
/* /\* #endif *\/ */
/* /\* 	  C2F(dr1)("xstringtt",sciGetText (pobj),&rect1[0],&rect1[1],&rect1[2],&rect1[3],&v,&v,&dv,&dv,&dv,&dv,10L,0L); *\/ */
/* /\* #ifdef WIN32 *\/ */
/* /\* 	  if ( flag_DO == 1) ReleaseWinHdc (); *\/ */
/* /\* #endif *\/ */
/* /\* 	  } *\/ */
/* 	  break; */
/* 	} */

/*       sciUnClip(sciGetIsClipping(pobj)); */
/*       break; */

    case SCI_AXES:
      if (!sciGetVisibility(pobj)) break;
      /*sciSetCurrentObj (pobj);	F.Leray 25.03.04 */
    
      /* load the object foreground and dashes color */
      
      x[0] = sciGetForeground (pobj);
      x[2] = sciGetLineWidth (pobj);
      x[3] = 0;
      x[4] = 0;
      v = 0;
      dv = 0;
#ifdef WIN32 
      flag_DO = MaybeSetWinhdc ();
#endif
      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,
		&dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,
		&dv, &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);
      sciClip(sciGetIsClipping(pobj));
     
/* Prototype Sci_Axis HAS CHANGED:  ************* F.Leray 19.05.04
   void Sci_Axis(pos,xy_type,x,nx,y,ny,str,subtics,format,fontsize,textcolor,fontstyle,ticscolor,logflag,seg_flag, axisbuild_flag)
   For the moment, for a simple axes ( coming from a scilab command as 'drawaxis'), we set the fontstyle to 0.
*/

/* variable tmpAxes init. for debugging: */
      paxes = pAXES_FEATURE(pobj);


      Sci_Axis(pAXES_FEATURE(pobj)->dir,pAXES_FEATURE (pobj)->tics,pAXES_FEATURE(pobj)->vx,
               &pAXES_FEATURE (pobj)->nx,pAXES_FEATURE(pobj)->vy,&pAXES_FEATURE (pobj)->ny,
               pAXES_FEATURE(pobj)->str,pAXES_FEATURE (pobj)->subint,pAXES_FEATURE (pobj)->format,
               pAXES_FEATURE (pobj)->fontsize,pAXES_FEATURE (pobj)->textcolor,fontstyle_zero, /* F.Leray 08.04.04 : Adding here fontstyle_zero*/
               pAXES_FEATURE (pobj)->ticscolor,(char)(pAXES_FEATURE (pobj)->logscale),pAXES_FEATURE (pobj)->seg, 1); 
#ifdef WIN32 
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif
      sciUnClip(sciGetIsClipping(pobj));   
			  
      break;
    case SCI_MERGE: 
      if (!(pSUBWIN_FEATURE (sciGetParentSubwin(pobj) )->facetmerge)) break; 

      DrawMerge3d(sciGetParentSubwin(pobj), pobj);
      break;
    case SCI_SURFACE:
      
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj) )->facetmerge) break;  
      if (!sciGetVisibility(pobj)) break;
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
      flag_DO = MaybeSetWinhdc();
#endif
      C2F (dr) ("xset", "dashes",     x,   x,   x+4, x+4, x+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x,   x,   x+4, x+4, x+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
      C2F (dr) ("xset", "thickness",  x+2, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 9L);
      C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L); /*D.A 17/12/2003*/
      C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 4L, 4L);
#ifdef WIN32
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif
		

      n=1;               
#ifdef WIN32
      flag_DO = MaybeSetWinhdc();
#endif     
      switch(pSURFACE_FEATURE (pobj)->typeof3d)
	{
	case SCI_FAC3D:
	  C2F(fac3dn)(pobj,pSURFACE_FEATURE (pobj)->pvecx,pSURFACE_FEATURE (pobj)->pvecy,
		      pSURFACE_FEATURE (pobj)->pvecz,
		      pSURFACE_FEATURE (pobj)->zcol,
		      &pSURFACE_FEATURE (pobj)->dimzx,&pSURFACE_FEATURE (pobj)->dimzy);

	  break;
	case SCI_PLOT3D:
	  C2F(plot3dn)(pobj,pSURFACE_FEATURE (pobj)->pvecx,pSURFACE_FEATURE (pobj)->pvecy,
		       pSURFACE_FEATURE (pobj)->pvecz,
		       &pSURFACE_FEATURE (pobj)->dimzx,&pSURFACE_FEATURE (pobj)->dimzy);
	  break;
	default:
	  break;
	}
#ifdef WIN32
      if ( flag_DO == 1) ReleaseWinHdc ();
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
  /* if(pcurrentpobj != NULL){
     sciprint (" \nIN sciSetCurrentObj, BEF setting, *pcurrentpobj = %d\r\n",&(*pcurrentpobj));
     sciprint (" IN sciSetCurrentObj, BEF setting, *pcurrentpobj->EntityType = %s\r\n\n",sciGetCharEntityType(pcurrentpobj));}
     else
     sciprint ("\npcurrentpobj is NULL (Only at the beginning normally)\n");*/
  
  pcurrentpobj = pobj;
  /*
  sciprint (" IN sciSetCurrentObj, AFTER setting, *pcurrentpobj = %d\r\n",&(*pcurrentpobj));
  sciprint (" IN sciSetCurrentObj, AFTER setting, *pcurrentpobj->EntityType = %s\r\n",sciGetCharEntityType(pcurrentpobj));
  sciprint ("------------------------------------------------------------------------\n");*/
  return 0;
}



/**sciGetCurrentFigure
 * @memo Returns the pointer to the current selected figure. 
 */
sciPointObj *
sciGetCurrentFigure ()
{
  return (sciPointObj *) sciGetCurrentScilabXgc ()->mafigure  ;
  cf_type=1;/* current figure is a graphic one */
}                                                               


/**sciSetCurrentFigure
 * @memo Sets the pointer to the current selected figure. 
 */

void 
sciSetCurrentFigure (sciPointObj * mafigure)
{
  sciGetCurrentScilabXgc ()->mafigure = mafigure ;
  cf_type=1;/* current figure is a graphic one */
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
      /*   case SCI_AXIS*/
    case SCI_TITLE:
      return -1;
      break;  
    case SCI_SEGS:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:    
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
      /*   case SCI_AXIS*/
    case SCI_TITLE:
      return -1;
      break; 
    case SCI_SEGS:
    case SCI_SBV:
    case SCI_SBH:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_LIGHT:
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_AGREG:
      return pAGREG_FEATURE (pthis)->xmin;
      break; 
    case SCI_LEGEND:
      /*   case SCI_AXIS*/
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
      C2F(dr)("xget","wdim",&itmp,x,&itmp,PI0,PI0,PI0,&xtmp2,PD0,PD0,PD0,0L,0L);
      pFIGURE_FEATURE (pthis)->windowdimwidth=x[0];
      return pFIGURE_FEATURE (pthis)->windowdimwidth;
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
    case SCI_AGREG:
      xtmp1 = pAGREG_FEATURE(pthis)->xmax;
      xtmp2 = pAGREG_FEATURE(pthis)->xmin;
      return fabs(pAGREG_FEATURE(pthis)->xmax - pAGREG_FEATURE(pthis)->xmin);
      break;
      break; 
    case SCI_LEGEND:   
      /*   case SCI_AXIS*/
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_AGREG:
      return pAGREG_FEATURE (pthis)->ymin;
      break;   
    case SCI_LEGEND:
      /*   case SCI_AXIS*/
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
      C2F(dr)("xget","wdim",&itmp,x,&itmp,PI0,PI0,PI0,&xtmp2,PD0,PD0,PD0,0L,0L);
      pFIGURE_FEATURE (pthis)->windowdimheight=x[1];
      return pFIGURE_FEATURE (pthis)->windowdimheight;
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
    case SCI_AGREG:
      return fabs(pAGREG_FEATURE (pthis)->ymax - pAGREG_FEATURE (pthis)->ymin);
      break;  
    case SCI_LEGEND: 
      /*  case SCI_AXIS*/
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("This object has no place X\n");
      return -1;
      break;
    }
  return -1;
}
/**DJ.Abdemouche 2003**/
/**sciGetPosX
 * @memo returns coordonee Z  in REAL for entity 
 */
double
sciGetPosZ (sciPointObj * pthis)
{
  switch (sciGetEntityType (pthis))
    {
    case SCI_TEXT:
      return pTEXT_FEATURE (pthis)->z;
      break;
    case SCI_SBH:
    case SCI_SBV:
    case SCI_RECTANGLE:
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:		
    case SCI_POLYLINE:
    case SCI_AGREG:
    case SCI_LEGEND:
      /*   case SCI_AXIS*/
    case SCI_TITLE:
    case SCI_FEC: 
    case SCI_GRAYPLOT: 
    case SCI_SEGS:
    case SCI_SURFACE:
    case SCI_LIGHT:  
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("This object has no place Z\n");
      return -1;
      break;
    }
  return -1;
}


/**sciGetPoint
 * @memo returns pointer to the points of the entity, and a pointer to the number of points. This function allocates memory for the tab of point, so after using the tab don't forget to free it
 */

/**MAJ pour le 3D DJ.Abdemouche 2003**/
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
    case SCI_POLYLINE:
      *numrow = pPOLYLINE_FEATURE (pthis)->n1;
      *numcol=((pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	       && (pPOLYLINE_FEATURE (pthis)->pvz != NULL))? 3:2;
      if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      for (i=0;i < *numrow;i++)
	{
	  tab[i] = pPOLYLINE_FEATURE (pthis)->pvx[i];	
	  tab[*numrow+i]= pPOLYLINE_FEATURE (pthis)->pvy[i];
	  if (*numcol== 3)
	    tab[(2*(*numrow))+i]= pPOLYLINE_FEATURE (pthis)->pvz[i]; 
	}
      return (double*)tab;
      break;
    case SCI_RECTANGLE:
      *numrow = 1;
      *numcol= (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d) ? 5: 4;
      if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] = pRECTANGLE_FEATURE (pthis)->x;
      tab[1] = pRECTANGLE_FEATURE (pthis)->y;
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	{
	  tab[2] = pRECTANGLE_FEATURE (pthis)->z;
	  tab[3] = pRECTANGLE_FEATURE (pthis)->width;
	  tab[4] = pRECTANGLE_FEATURE (pthis)->height;
	}
      else
	{
	  tab[2] = pRECTANGLE_FEATURE (pthis)->width;
	  tab[3] = pRECTANGLE_FEATURE (pthis)->height; 
	}
      return (double*)tab;
      break;
    case SCI_ARC:
      *numrow = 1;
      *numcol= (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d) ? 7: 6;
      if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] = sciGetPosX (pthis);
      tab[1] = sciGetPosY (pthis);
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	{
	  tab[2] = pARC_FEATURE (pthis)->z;
	  tab[3] = pARC_FEATURE (pthis)->width;
	  tab[4] = pARC_FEATURE (pthis)->height;
	  tab[5] = pARC_FEATURE (pthis)->alphabegin;
	  tab[6] = pARC_FEATURE (pthis)->alphaend;
	}
      else
	{
	  tab[2] = pARC_FEATURE (pthis)->width;
	  tab[3] = pARC_FEATURE (pthis)->height;
	  tab[4] = pARC_FEATURE (pthis)->alphabegin;
	  tab[5] = pARC_FEATURE (pthis)->alphaend;
 
	}
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
      *numcol= (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d) ? 3: 2;
      if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	return (double*)NULL;
      tab[0] = sciGetPosX (pthis);
      tab[1] = sciGetPosY (pthis); 
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	tab[2] = sciGetPosZ (pthis);
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
	*numcol = ((pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d) 
		   && (pSEGS_FEATURE (pthis)->vz != NULL))? 3:2;
	if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	  return (double*)NULL;
	for (i=0;i < *numrow;i++)
	  {
	    tab[i] = pSEGS_FEATURE (pthis)->vx[i];	
	    tab[*numrow+i]= pSEGS_FEATURE (pthis)->vy[i];
	    if (*numcol== 3)
	      tab[2*(*numrow)+i]= pSEGS_FEATURE (pthis)->vz[i];   
	  }
      }
      else {/*djalel je suis la **/
	*numcol = ((pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d) 
		   && (pSEGS_FEATURE (pthis)->vz != NULL))? 3 + *numrow*3:2 + *numrow*2;
	if ((tab = calloc((*numrow)*(*numcol),sizeof(double))) == NULL)
	  return (double*)NULL;
	for (i=0;i < *numrow;i++)
	  {
	    tab[i] = pSEGS_FEATURE (pthis)->vx[i];	
	    tab[*numrow+i]= pSEGS_FEATURE (pthis)->vy[i];
	    if (*numcol == 3 + *numrow*3)
	      tab[2*(*numrow)+i]= pSEGS_FEATURE (pthis)->vz[i];    
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
	k=k+2* *numrow * *numrow;
	if (*numcol == 3 + *numrow*3)
	  for (i=0;i < 2* *numrow * *numrow;i++)
	    {
	      tab[k+i] = pSEGS_FEATURE (pthis)->vfz[i];	
	    }
      }
      return (double*)tab;
      break;
    case SCI_SURFACE:
      /* F.Leray 17.03.04*/

      sciprint("Impossible case happened ! Check if instruction in matdes.c inside gget function");
      return (double*) NULL;
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
      /*   case SCI_AXIS    */
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
/** MAJ pour le 3D DJ.Abdemouche 2003**/
int
sciSetPoint(sciPointObj * pthis, double *tab, int *numrow, int *numcol)
{
  int i,n1,k,k1,k2;
  double *pvx = NULL,
    *pvy = NULL,
    *pvz = NULL,
    *pvfx = NULL,
    *pvfy = NULL,
    *pvfz = NULL;
  int *pstyle = NULL;
  POINT2D *pvector;

  /* Adding F.Leray 07.04.04 for value_min -max update for isoview*/
  /*sciPointObj *psubwin = ( sciPointObj *) NULL;*/
  double xmin,xmax,ymin,ymax,zmin,zmax;

  switch (sciGetEntityType (pthis))
    {
    case SCI_POLYLINE:
      n1=pPOLYLINE_FEATURE (pthis)->n1;
      if ((*numcol != 3)&&(*numcol != 2))
	{
	  sciprint("The number of columns must be 2 (3 if three-dimensional axes) \n");
	  return -1;
	}
      if (*numrow != n1) /* SS 30/1/02 */
	{
	  n1=*numrow;
	  if ((pvx = MALLOC (n1 * sizeof (double))) == NULL) return -1;
	  if ((pvy = MALLOC (n1 * sizeof (double))) == NULL) {
	    FREE(pvx); pvx = (double *) NULL;
	    return -1;
	  } 
	  if ((pvector = MALLOC (n1 * sizeof (POINT2D))) == NULL) {
	    FREE(pvx); pvx = (double *) NULL;
	    FREE(pvy); pvy = (double *) NULL;
	    return -1;
	  }
	  if (*numcol == 3)
	    if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	      if ((pvz = MALLOC (n1 * sizeof (double))) == NULL) {
		FREE(pvx); pvx = (double *) NULL;
		FREE(pvy); pvy = (double *) NULL;
		FREE(pvector); pvector = (POINT2D *) NULL;
		return -1;
	      }
	  
	  FREE(pPOLYLINE_FEATURE (pthis)->pvx); pPOLYLINE_FEATURE (pthis)->pvx = NULL;
	  FREE(pPOLYLINE_FEATURE (pthis)->pvy); pPOLYLINE_FEATURE (pthis)->pvy = NULL;
	  FREE(pPOLYLINE_FEATURE (pthis)->pvector); pPOLYLINE_FEATURE (pthis)->pvector = NULL;
	  if ((pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	      && (pPOLYLINE_FEATURE (pthis)->pvz != NULL))
	    {
	      FREE(pPOLYLINE_FEATURE (pthis)->pvz); 
	      pPOLYLINE_FEATURE (pthis)->pvz = NULL;
	    }

	  /* Adding F.Leray 07.04.04 : Init. phase*/
	  xmin=xmax=tab[0];
	  ymin=ymax=tab[0+ (*numrow)];
	  if (*numcol == 3)
	    zmin=zmax=tab[0+ 2*(*numrow)];
	    /* END Adding */

	  for (i=0;i < *numrow;i++)
	    {
	      pvx[i] = tab[i];
	      pvy[i] = tab[i+ (*numrow)];
	      pvector[i].x =  tab[i];
	      pvector[i].y =  tab[i+ (*numrow)];
	      if (*numcol == 3)
		pvz[i] = tab[i+ 2*(*numrow)];

	      /* Adding F.Leray 07.04.04 */
	      xmin=Min(pvx[i],xmin);
	      xmax=Max(pvx[i],xmax);
	      ymin=Min(pvy[i],ymin);
	      ymax=Max(pvy[i],ymax);
	      if (*numcol == 3){
		zmin=Min(pvz[i],zmin);
		zmax=Max(pvz[i],zmax);
	      }
		 /* END Adding */
	    }
	  pPOLYLINE_FEATURE (pthis)->pvx=pvx;
	  pPOLYLINE_FEATURE (pthis)->pvy=pvy; 
	  pPOLYLINE_FEATURE (pthis)->pvector=pvector;
	  pPOLYLINE_FEATURE (pthis)->n1=n1;
	  if (*numcol == 3)
	    pPOLYLINE_FEATURE (pthis)->pvz=pvz;
	}
      else
	{
	  if (*numcol == 3)
	    if ((pvz = MALLOC (*numrow * sizeof (double))) == NULL) 
	      return -1;
	  
	  /* Adding F.Leray 07.04.04 : Init. phase*/
	  xmin=xmax=tab[0];
	  ymin=ymax=tab[0+ (*numrow)];
	  if (*numcol == 3)
	    zmin=zmax=tab[0+ 2*(*numrow)];
	  /* END Adding */
	  
	  for (i=0;i < *numrow;i++)
	    {
	      pPOLYLINE_FEATURE (pthis)->pvx[i] = tab[i];
	      pPOLYLINE_FEATURE (pthis)->pvy[i] = tab[i+ (*numrow)];
	      if (*numcol == 3)
		pvz[i] = tab[i+ 2*(*numrow)];
	      
	      /* Adding F.Leray 07.04.04 */
	      xmin=Min(pPOLYLINE_FEATURE (pthis)->pvx[i],xmin);
	      xmax=Max(pPOLYLINE_FEATURE (pthis)->pvx[i],xmax);
	      ymin=Min(pPOLYLINE_FEATURE (pthis)->pvy[i],ymin);
	      ymax=Max(pPOLYLINE_FEATURE (pthis)->pvy[i],ymax);
	      if (*numcol == 3){
		zmin=Min(pvz[i],zmin); /* F.Leray 07.04.04: Strange: different management with z (use pvz and not pPOLYLINE_FEATURE (pthis)->pvz) */
		zmax=Max(pvz[i],zmax); /* Why not but it is not consistant...*/
	      }
	      /* END Adding */
	    }
	  if (*numcol == 3)
	    pPOLYLINE_FEATURE (pthis)->pvz = pvz; /* ..and here we put pvz...*/
	}
      

   
      return 0;
      break;
    case SCI_RECTANGLE:
      if ((*numrow * *numcol != 5)&&(*numrow * *numcol != 4))
	{
	  sciprint("The number of element must be 4 (5 if z coordinate )\n");
	  return -1;
	}

      pRECTANGLE_FEATURE (pthis)->x          = tab[0];
      pRECTANGLE_FEATURE (pthis)->y          = tab[1];
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	{
	  pRECTANGLE_FEATURE (pthis)->z          = tab[2];
	  pRECTANGLE_FEATURE (pthis)->width      = tab[3];
	  pRECTANGLE_FEATURE (pthis)->height     = tab[4];
	}
      else
	{
	  pRECTANGLE_FEATURE (pthis)->width      = tab[2];
	  pRECTANGLE_FEATURE (pthis)->height     = tab[3];
	}
      return 0;
      break;
    case SCI_ARC:
      if ((*numrow * *numcol != 7)&&(*numrow * *numcol != 6))
	{
	  sciprint("The number of elements must be 6 (7 if z coordinate )\n");
	  return -1;
	}
      
      pARC_FEATURE (pthis)->x          = tab[0];
      pARC_FEATURE (pthis)->y          = tab[1];  
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	{
	  pARC_FEATURE (pthis)->z          = tab[2];
	  pARC_FEATURE (pthis)->width      = tab[3];
	  pARC_FEATURE (pthis)->height     = tab[4];
	  pARC_FEATURE (pthis)->alphabegin = tab[5];
	  pARC_FEATURE (pthis)->alphaend   = tab[6];
	}
      else
	{
	  pARC_FEATURE (pthis)->width      = tab[2];
	  pARC_FEATURE (pthis)->height     = tab[3];
	  pARC_FEATURE (pthis)->alphabegin = tab[4];
	  pARC_FEATURE (pthis)->alphaend   = tab[5]; 
	}
      return 0;
      break;
    case SCI_TEXT:
      if ((*numrow * *numcol != 2)&&(*numrow * *numcol != 3))
	{
	  sciprint("The number of elements must be 2 (3 if z coordinate)\n");
	  return -1;
	}
      pTEXT_FEATURE (pthis)->x = tab[0];
      pTEXT_FEATURE (pthis)->y = tab[1];
      if (pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->is3d)
	pTEXT_FEATURE (pthis)->z = tab[2];
      return 0;
      break;
    case SCI_SEGS:
      if (pSEGS_FEATURE (pthis)->ptype == 0) {
	if ((*numcol != 3)&&(*numcol != 2)) {
	  sciprint("The number of columns must be 2 (3 if three-dimensional axes) \n");
	  return -1;
	}
	n1=pSEGS_FEATURE (pthis)->Nbr1;
	if (*numrow != n1) {
	  n1=*numrow;
	  if ((pvx = MALLOC (n1 * sizeof (double))) == NULL) return -1;
	  if ((pvy = MALLOC (n1 * sizeof (double))) == NULL) {
	    FREE(pvx); pvx = (double *) NULL;
	    return -1;
	  }
	  if (*numcol == 3)
	    if ((pvz = MALLOC (n1 * sizeof (double))) == NULL) {
	      FREE(pvx); pvx = (double *) NULL;
	      FREE(pvy); pvy = (double *) NULL;
	      return -1;
	    }
	  if ((pstyle = MALLOC (n1 * sizeof (int))) == NULL) {
	    FREE(pvx); pvx = (double *) NULL;
	    FREE(pvy); pvy = (double *) NULL;
	    FREE(pvz); pvz = (double *) NULL;
	    return -1;
	  }
	  FREE(pSEGS_FEATURE (pthis)->vx); pSEGS_FEATURE (pthis)->vx = NULL;
	  FREE(pSEGS_FEATURE (pthis)->vy); pSEGS_FEATURE (pthis)->vx = NULL;
	  if (*numcol == 3)
	    FREE(pSEGS_FEATURE (pthis)->vz); pSEGS_FEATURE (pthis)->vz = NULL;
	  /* Attention ici on detruit pstyle !! F.Leray 20.02.04*/
	  FREE(pSEGS_FEATURE (pthis)->pstyle); pSEGS_FEATURE (pthis)->pstyle = NULL;
	  for (i=0;i < *numrow;i++)
	    {
	      pvx[i] = tab[i];
	      pvy[i] = tab[i+ (*numrow)];
	      if (*numcol == 3)
		pvz[i] = tab[i+ 2*(*numrow)];
	      pstyle[i] = 0;
	    }
	  pSEGS_FEATURE (pthis)->vx=pvx;
	  pSEGS_FEATURE (pthis)->vy=pvy;
	  if (*numcol == 3)
	    pSEGS_FEATURE (pthis)->vz=pvz;
	  pSEGS_FEATURE (pthis)->Nbr1=n1;
	  pSEGS_FEATURE (pthis)->pstyle=pstyle;
	}
	else {
	  if ((*numcol == 3) && (pSEGS_FEATURE (pthis)->vz == NULL)) 
	    if ((pSEGS_FEATURE (pthis)->vz = MALLOC (n1 * sizeof (double))) == NULL) return -1;

	  for (i=0;i < *numrow;i++) {
	    pSEGS_FEATURE (pthis)->vx[i] = tab[i];
	    pSEGS_FEATURE (pthis)->vy[i] = tab[i+ (*numrow)];
	    if (*numcol == 3)
	      pSEGS_FEATURE (pthis)->vz[i] = tab[i+ 2*(*numrow)];
	  }
	}
      }
      else {
	if ((*numcol != 3 +3*(*numrow * *numrow))&&(*numcol != 2 +2*(*numrow * *numrow)))
	  {
	    sciprint("The number of columns must be %d (%d if three-dimensional axes)\n",
		     2+2*(*numrow * *numrow),3+3*(*numrow * *numrow));
	    return -1;
	  }
	n1=pSEGS_FEATURE (pthis)->Nbr1;
	if (*numrow != n1) /* SS 30/1/02 */
	  {
	    n1=*numrow;
	    if ((pvx = MALLOC (n1 * sizeof (double))) == NULL) return -1;
	    if ((pvy = MALLOC (n1 * sizeof (double))) == NULL) {
	      FREE(pvx); pvx = (double *) NULL;
	      return -1;
	    }
	    if ((pstyle = MALLOC (n1 * sizeof (int))) == NULL) {
	      FREE(pvx); pvx = (double *) NULL;
	      FREE(pvy); pvy = (double *) NULL;
	      FREE(pvz); pvz = (double *) NULL;
	      return -1;
	    }
	    if ((pvfx = MALLOC ((n1*n1) * sizeof (double))) == NULL) return -1;
	    if ((pvfy = MALLOC ((n1*n1) * sizeof (double))) == NULL) {
	      FREE(pvx); pvx = (double *) NULL;
	      FREE(pvy); pvy = (double *) NULL;
	      FREE(pvz); pvz = (double *) NULL;
	      FREE(pvfx); pvfx = (double *) NULL;
	      return -1;
	    }
	    if (*numcol == 3 +3*(*numrow * *numrow))
	      {
		if ((pvz = MALLOC (n1 * sizeof (double))) == NULL) {
		  FREE(pvx); pvx = (double *) NULL;
		  FREE(pvy); pvy = (double *) NULL;
		  return -1;
		}
		if ((pvfz = MALLOC ((n1*n1) * sizeof (double))) == NULL) {
		  FREE(pvx); pvx = (double *) NULL;
		  FREE(pvy); pvy = (double *) NULL;
		  FREE(pvz); pvz = (double *) NULL;
		  FREE(pvfx); pvfx = (double *) NULL;
		  FREE(pvfy); pvfy = (double *) NULL;
		  return -1;
		}
		FREE(pSEGS_FEATURE (pthis)->vz); pSEGS_FEATURE (pthis)->vz = NULL;
		FREE(pSEGS_FEATURE (pthis)->vfz); pSEGS_FEATURE (pthis)->vfz = NULL;
	      }
	    FREE(pSEGS_FEATURE (pthis)->vx); pSEGS_FEATURE (pthis)->vx = NULL;
	    FREE(pSEGS_FEATURE (pthis)->vy); pSEGS_FEATURE (pthis)->vy = NULL;
	    FREE(pSEGS_FEATURE (pthis)->vfx); pSEGS_FEATURE (pthis)->vfx = NULL;
	    FREE(pSEGS_FEATURE (pthis)->vfy); pSEGS_FEATURE (pthis)->vfy = NULL;
	    for (i=0;i < n1;i++)
	      {
		pvx[i] = tab[i];
		pvy[i] = tab[i+ (*numrow)];
		if (*numcol == 3 +3*(*numrow * *numrow))
		  pvz[i] = tab[i+ 2*(*numrow)];

	      }
	    k=3*n1;
	    for (i=0;i < n1*n1;i++)
	      {
		pvfx[i] = tab[k+i];
		pvfy[i] = tab[k+n1*n1+i];
		if (*numcol == 3 +3*(*numrow * *numrow))
		  pvfz[i] = tab[2*k+n1*n1+i];

	      }
	    pSEGS_FEATURE (pthis)->vx=pvx;
	    pSEGS_FEATURE (pthis)->vy=pvy;
	    pSEGS_FEATURE (pthis)->vx=pvfx;
	    pSEGS_FEATURE (pthis)->vy=pvfy; 
	    pSEGS_FEATURE (pthis)->Nbr1=n1;
	    if (*numcol == 3 +3*(*numrow * *numrow))
	      {
		pSEGS_FEATURE (pthis)->vz=pvz;
		pSEGS_FEATURE (pthis)->vy=pvfz;
	      }

	  }
	else {
	  for (i=0;i < *numrow;i++)   {
	    pSEGS_FEATURE (pthis)->vx[i] = tab[i];
	    pSEGS_FEATURE (pthis)->vy[i] = tab[i+ (*numrow)];
	    if (pSEGS_FEATURE (pthis)->vz != (double *)NULL)
	      pSEGS_FEATURE (pthis)->vz[i] = tab[i+ 2*(*numrow)];
	  }
	  k=2* (*numrow);
	  k1=k+ (*numrow * *numrow);
	  k2=2*k+ (*numrow * *numrow);
	  for (i=0;i < *numrow * *numrow ;i++)   {
	    pSEGS_FEATURE (pthis)->vfx[i] = tab[k+i];
	    pSEGS_FEATURE (pthis)->vfy[i] = tab[k1+i];
	    if (pSEGS_FEATURE (pthis)->vfz != (double *)NULL)
	      pSEGS_FEATURE (pthis)->vfz[i] = tab[k2+i];
	  }
	}
      }
      return 0;
      break;


    case SCI_SURFACE:/* DJ.A 2003 */
      sciprint ("Un handled data field\n");
      return -1;
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
      /*   case SCI_AXIS*/
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("This object has no possibility to set points !\n");
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
	  /*set_scale ("tttfff", pSUBWIN_FEATURE (pthis)->WRect, 
	    pSUBWIN_FEATURE (pthis)->FRect, NULL, "nn", NULL);*/
	  set_scale ("tttftf", pSUBWIN_FEATURE (pthis)->WRect, 
		     pSUBWIN_FEATURE (pthis)->FRect, NULL, "nn", NULL);
	  return TRUE;
	}
      else
	return FALSE;
      break;
    case SCI_ARC:
      /* on recupere la dimension de la sous fenetre parente */
      /*set_scale ("tttfff", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
	pSUBWIN_FEATURE (sciGetParent(pthis))->FRect, NULL, "nn", NULL);*/
      set_scale ("tttftf", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
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
      /*set_scale ("tttfff", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
	pSUBWIN_FEATURE (sciGetParent(pthis))->FRect, NULL, "nn", NULL);*/
      set_scale ("tttftf", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
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
      /*set_scale ("tttfff", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
	pSUBWIN_FEATURE (sciGetParent(pthis))->FRect, NULL, "nn", NULL);*/
      set_scale ("tttftf", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
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
      /*set_scale ("tttfff", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
	pSUBWIN_FEATURE (sciGetParent(pthis))->FRect, NULL, "nn", NULL);*/
      set_scale ("tttftf", pSUBWIN_FEATURE (sciGetParent(pthis))->WRect, 
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
    case SCI_SEGS: 
    case SCI_FEC: 
    case SCI_GRAYPLOT:
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SURFACE:
      /*  case SCI_AXIS    */
    case SCI_AXES:
    case SCI_PANNER:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_FIGURE:
    case SCI_SBV:
    case SCI_SBH:
    case SCI_LIGHT:
    case SCI_LABEL: /* F.Leray 28.05.04 */
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
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SURFACE:
      /*   case SCI_AXIS    */
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
      /*  case SCI_AXIS	
	  return (char *)(pAXIS_FEATURE(pthis)->callback);
	  break;*/
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
      /*   case SCI_AXIS	
	   return pAXIS_FEATURE(pthis)->callbackevent;
	   break;*/
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
      /*   case SCI_AXIS	
	   pAXIS_FEATURE(pthis)->callbackevent = mevent;
	   break;*/
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
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SURFACE:
      /*   case SCI_AXIS   */
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
    case SCI_TEXT:
    case SCI_TITLE:
    case SCI_LEGEND:
    case SCI_SURFACE:
      /*   case SCI_AXIS    */
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
      /*   case SCI_AXIS    */
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
  double xtmp1; long xtmp; /*double xtmp,xtmp1;*/

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
	case SCI_TEXT:
	case SCI_TITLE:
	case SCI_LEGEND:
	case SCI_SURFACE:
	  /*   case SCI_AXIS    */
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
	case SCI_LABEL: /* F.Leray 28.05.04 A REVOIR...*/
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
      if (sonsnext) sonsnext->pprev = sons;
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

/* Apparently, what is following is USELESS; F.Leray 25.03.04 */
/*
  void sciInitObj(pointXGC)
  double *pointXGC; 
  { 
  struct BCG *InitScilabXgc; 
  static sciPointObj *mafigure;
  static sciPointObj *masousfen;
   
  InitScilabXgc=(struct BCG *)pointXGC; */
  /*  si le ScilabXgc a été initialisé et s'il est different de celui pocedé par la figure courante
   *  construire une nouvelle fenetre et une sous fenetre 
   * une autre facon de comparer c'est de voir est ce que InitScilabXgc->mafigure n'est pas vide (!=NULL */
/*  if (tmpScilabXgc != InitScilabXgc)
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
*/
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
  integer v=0;
  double dv=0.0; 
  /* find if exist figure winnum */
  /* une autre methode c est de tester CurXGC->mafigure = NULL */
  if ( (sciPointObj *) sciIsExistingFigure(winnum) == (sciPointObj *) NULL) 
    {
      /** Figure winnum don't exist **/
      /** Create Figure **/ 
      C2F(dr)("xget","gc",&v,&v,&v,&v,&v,&v,(double *)&CurXGC,&dv,&dv,&dv,5L,10L);/* ????? SS*/
      /*    if ((mafigure = ConstructFigure (CurXGC)) != NULL)*/ /*F.Leray 24.03.04 */
       if ((mafigure = ConstructFigure (CurXGC)) != NULL)
	{
	  sciSetCurrentObj (mafigure); /* F.Leray 25.03.04*/
	  CurXGC->mafigure = mafigure;
          CurXGC->graphicsversion = 1;
	  if ((masousfen = ConstructSubWin (mafigure, CurXGC->CurWindow)) != NULL) {
	    sciSetCurrentObj (masousfen);
	    sciSetOriginalSubWin (mafigure, masousfen);
	    cf_type=1;/* current figure is a graphic one */
	  }
	}
       
    }
  else
    cf_type=1;/* current figure is a graphic one */
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
  /* F.Leray 13.04.04 : Test if returned sciPointObj* is NULL (means Figure has been destroyed) */

  if(figure == (sciPointObj *)  NULL)
    {
      /* Do nothing */
    }
  else
    {
      C2F (dr) ("xget", "window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,6L);
      C2F (dr) ("xset", "window",value,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,6L);
      /* sciSetCurrentObj(figure); */ /*F.Leray 25.03.04*/
      sciSetReplay (TRUE);
      sciDrawObj (figure);
      sciSetReplay (FALSE);
      C2F (dr) ("xset", "window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,6L);
    }
  
}

void sciXbasc()
{  
  static sciPointObj *mafigure;
  static sciPointObj *masousfen;  
 
  mafigure= (sciPointObj *) sciGetCurrentFigure(); 
  DestroyAllGraphicsSons((sciPointObj *)mafigure);
  
   
  if ((masousfen = ConstructSubWin (mafigure, 'o')) != NULL){
    sciSetCurrentObj(masousfen);
    sciSetOriginalSubWin (mafigure, masousfen);}
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
  /*sciSetCurrentObj (pobj); */ /* Useless*/
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
      /*   case SCI_AXIS        */
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
    case SCI_LABEL: /* F.Leray 28.05.04 */
    default:
      sciprint ("This object can not be moved\r\n");
      return -1;
      break;
    }    
  if (opt)
    sciDrawObj(pobj);
  else
    sciDrawObj(sciGetParentFigure(pobj));
    /* sciDrawObj(sciGetParentFigure(sciGetCurrentObj ()));*/
    
  return 0;
}

int sciType (marker, pobj)
     char *marker;
     sciPointObj * pobj;
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
  else if (strncmp(marker,"hiddencolor", 11) == 0)   {return 1;}/* DJ.A 2003 */
  else if (strncmp(marker,"isoview", 7) == 0)   {return 10;}/**DJ.Abdemouche 2003**/
  else if (strncmp(marker,"view", 4) == 0)   {return 10;}/**DJ.Abdemouche 2003**/	
  else if (strncmp(marker,"figure_size", 11) == 0){return 1;}	
  else if (strncmp(marker,"figure_id", 9) == 0)   {return 1;}	
  else if (strncmp(marker,"figure_name", 11) == 0){return 10;}   
  else if (strncmp(marker,"figures_id", 10) == 0)   {return 1;}
  else if (strncmp(marker,"pixmap", 10) == 0)   {return 10;}/*Ajout A.Djalel*/
  else if (strncmp(marker,"polyline_style", 14) == 0){return 1;} 
  else if (strncmp(marker,"font_size", 9) == 0)   {return 1;}	
  else if (strncmp(marker,"font_angle", 10) == 0) {return 1;}		
  else if (strncmp(marker,"font_foreground", 15) == 0){return 1;}
  else if (strncmp(marker,"font_color", 10) == 0)   {return 1;} /* F.Leray 09.04.04 : Adding to support font_color user interface */
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
  else if (strncmp(marker,"default_values", 14) == 0) {return 1;} 
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
  /* else if (strcmp(marker,"data") == 0)            {return 1;}*/ /* F.Leray modif. for SCI_SURFACE case*/
  else if (strcmp(marker,"data") == 0 && sciGetEntityType(pobj) != SCI_SURFACE)  
    {return 1;} 
  else if (strcmp(marker,"data") == 0 && sciGetEntityType(pobj) == SCI_SURFACE) 
    {return 16;}
  else if (strncmp(marker,"hdl", 3) == 0)         {return 1;}		
  else if (strncmp(marker,"callbackmevent", 14) == 0) {return 1;}
  else if (strncmp(marker,"callback", 8) == 0)    {return 10;} 	
  else if (strncmp(marker,"log_flags", 9) == 0)   {return 10;}
  else if (strcmp(marker,"data_mapping") == 0)    {return 10;}
  else if (strcmp(marker,"surface_color") == 0)    {return 1;}
  else if (strcmp(marker,"rotation_angles") == 0)    {return 1;}
  else if (strcmp(marker,"color_mode") == 0)    {return 1;}/*DJ.A merge*/ 
  else if (strcmp(marker,"color_flag") == 0)    {return 1;}
  else if (strcmp(marker,"axes_bounds") == 0)    {return 1;}
  else if (strcmp(marker,"data_bounds") == 0)    {return 1;}
  else if (strcmp(marker,"margins") == 0)    {return 1;}
  else if (strcmp(marker,"surface_color") == 0)    {return 1;}
  else if (strcmp(marker,"rotation_style") == 0)    {return 10;}
  else if (strcmp(marker,"triangles") == 0)    {return 1;}
  else if (strcmp(marker,"z_bounds") == 0)    {return 1;}
  else if (strcmp(marker,"current_axes") == 0)    {return 9;}
  else if (strcmp(marker,"current_figure") == 0)    {return 9;}
  else if (strcmp(marker,"default_axes") == 12)    {return 9;}/* DJ.A 08/01/04 */
  else if (strcmp(marker,"default_figure") == 14)    {return 9;}/* DJ.A 08/01/04 */
  else if (strcmp(marker,"children") == 0)    {return 9;}
  else if (strcmp(marker,"cube_scaling") == 0)    {return 10;} /* F.Leray 22.04.04 */
  else if (strcmp(marker,"x_label") == 0)    {return 9;}  /* F.Leray 27.05.04 */
  else if (strcmp(marker,"y_label") == 0)    {return 9;} 
  else if (strcmp(marker,"z_label") == 0)    {return 9;} 
  else if (strcmp(marker,"title") == 0)    {return 9;} 

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

void initsubwin()  /* Interesting / F.Leray 05.04.04 */
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

  /* F.Leray 05.04.04 */
  pSUBWIN_FEATURE (psubwin)->SRect[0]   = 0.0; /* xmin */
  pSUBWIN_FEATURE (psubwin)->SRect[1]   = 1.0; /* xmax */
  pSUBWIN_FEATURE (psubwin)->SRect[2]   = 0.0;/* ymin */
  pSUBWIN_FEATURE (psubwin)->SRect[3]   = 1.0; /* ymax */
  pSUBWIN_FEATURE (psubwin)->SRect[4]   = -1.0; /* zmin */
  pSUBWIN_FEATURE (psubwin)->SRect[5]   = 1.0; /* zmax */
            
  dir= 'd'; pSUBWIN_FEATURE (psubwin)->axes.xdir=dir;
  dir= 'l'; pSUBWIN_FEATURE (psubwin)->axes.ydir=dir;
  pSUBWIN_FEATURE (psubwin)->isaxes  = FALSE;
  pSUBWIN_FEATURE (psubwin)->axes.rect = 1;  
  pSUBWIN_FEATURE (psubwin)->axes.ticscolor = -1;
  pSUBWIN_FEATURE (psubwin)->axes.subint[0] =  1;
  pSUBWIN_FEATURE (psubwin)->axes.subint[1] =  1;
  pSUBWIN_FEATURE (psubwin)->axes.subint[2] =  1;
  pSUBWIN_FEATURE (psubwin)->axes.limits[0]  = 0;  
  pSUBWIN_FEATURE (psubwin)->visible = TRUE;
  pSUBWIN_FEATURE (psubwin)->is3d = FALSE;  
  pSUBWIN_FEATURE (psubwin)->alpha  = 0.0;
  pSUBWIN_FEATURE (psubwin)->theta  = 270.0;
  pSUBWIN_FEATURE (psubwin)->FirstPlot = TRUE;
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

  /* sciprint("DANS scizoom: *+*+*+*+*+*+*+*+*+*\n");
  sciprint(" ppsubwin->FRect[0] = xmin = %f\n",pSUBWIN_FEATURE (psousfen)->FRect[0]);
  sciprint(" ppsubwin->FRect[1] = ymin = %f\n",pSUBWIN_FEATURE (psousfen)->FRect[1]);
  sciprint(" ppsubwin->FRect[2] = xmax = %f\n",pSUBWIN_FEATURE (psousfen)->FRect[2]);
  sciprint(" ppsubwin->FRect[3] = ymax = %f\n\n",pSUBWIN_FEATURE (psousfen)->FRect[3]);
  sciprint("fin DANS scizoom: *+*+*+*+*+*+*+*+*+*\n");*/

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
sciGetIdFigure (int *vect, int *id, int *flag)
{
  sciHandleTab *hdl;
  sciPointObj  *pobj;
  
  hdl = pendofhandletab;  
  *id=0;
  while (hdl != NULL)
    { 
      pobj=(sciPointObj *) sciGetPointerFromHandle (hdl->index);
      if (sciGetEntityType(pobj) == SCI_FIGURE)
	{
	  if (*flag) vect[*id] = sciGetNum(pobj);
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
  /*  if (CurrentScilabXgc==(struct BCG *)NULL) return 1; */
  if (CurrentScilabXgc==(struct BCG *)NULL) return 0; /* New Graphic mode returned by default F.Leray 11.06.04 */
  
  return (CurrentScilabXgc->graphicsversion == 0) ? 1 : 0; 
}

void set_version_flag(int flag) 
{ 
  double *XGC,dv=0;
  struct BCG *CurrentScilabXgc; 
  int v=0;
  
  C2F(dr)("xget","gc",&v,&v,&v,&v,&v,&v,(double *)&XGC,&dv,&dv,&dv,5L,10L); /* ajout cast ???*/
  CurrentScilabXgc=(struct BCG *)XGC;
  if (CurrentScilabXgc !=(struct BCG *)NULL) 
    CurrentScilabXgc->graphicsversion = flag; 
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

void Obj_RedrawNewAngle(sciPointObj *psubwin,double theta,double alpha)
{
  /**dj20003***/ 
  if ((alpha == 0.0) && (theta == 270.0))
    {
      pSUBWIN_FEATURE (psubwin)-> is3d = FALSE; 
      return;
    }
  
  pSUBWIN_FEATURE (psubwin)->alpha = alpha;
  pSUBWIN_FEATURE (psubwin)->theta  = theta;
  pSUBWIN_FEATURE (psubwin)-> is3d = TRUE;
  if ((alpha == 0.0 ) || (alpha == 180.0 ) || (alpha == -180.0 ))  /* DJ.A 30/12 */
    pSUBWIN_FEATURE (psubwin)->project[2]= 0;
  else 
    {
      pSUBWIN_FEATURE (psubwin)->project[2]= 1;
      if (((alpha == 90.0 ) || (alpha == 270.0 )|| (alpha == -90.0) || (alpha == -270.0 ))
	  && ((theta == 90.0 ) || (theta == -90.0 )
	      || (theta == 270.0 )|| (theta == -270.0 )))
	pSUBWIN_FEATURE (psubwin)->project[1]= 0;
      else
	{
	  pSUBWIN_FEATURE (psubwin)->project[1]= 1;
	  if (((alpha == 90.0 )|| (alpha == 270.0 )|| (alpha == -90.0) || (alpha == -270.0 ))  
	      && ((theta == 0.0 ) || (theta == 180.0 )|| (alpha == -180.0 )))
	    pSUBWIN_FEATURE (psubwin)->project[0]= 0;
	  else
	    pSUBWIN_FEATURE (psubwin)->project[0]= 1;
	}
    }
}
/* DJ.A 2003 */
BOOL Check3DObjs(sciPointObj *pobj)
{  
  sciSons *psonstmp;

  psonstmp = sciGetSons (pobj); 
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
sciPointObj *CheckClickedSubwin(integer x, integer y)
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

 
sciPointObj *sciGetSurface(sciPointObj *psubwin)
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

/**axis_3ddraw 10/2003
 * @author Djalel Abdemouche
 * Should be in Axes.c file
 */
void axis_3ddraw(sciPointObj *pobj, double *xbox, double *ybox, double *zbox, integer *InsideU, integer *InsideD) 
{
  double dbox[6];
  char /* *legend="x@y@z",*/logf[2];
  integer flag,ib,i,p,n,pat,hiddencolor, x[5]; /* F. Leray : redimmensionnment (+1) du tableau x[4];*/
  static double Alpha, Teta,cost=0.5,sint=0.5,cosa=0.5,sina=0.5;
  double xmmin,ymmax,xmmax,ymmin,FRect[4],WRect[4],ARect[4];
  static integer aaint[]={2,10,2,10};
  int verbose=0,wdim[2],narg;
  double R,xo,yo,zo,dx,dy,dz,hx,hy,hx1,hy1,xmaxi;
  integer wmax,hmax,ind2,ind3,ind,tmpind;
  integer ixbox[8],iybox[8],xind[8],dash[6];
  integer background,zero=0, color_old; /* Adding color_old 04.03.04*/
  
  BOOL cube_scaling; 

  /* Initialisation phase for x (to detect bug): x set to -1000 F.Leray 05.03.04*/
  for(i=0;i<5;i++) x[i] = -1000;

  if(sciGetEntityType (pobj) == SCI_SUBWIN)
    {  
      update_3dbounds(pobj);
      /*update_graduation(pobj);*/

 
      dbox[0] =  pSUBWIN_FEATURE (pobj)->FRect[0]; /*xmin*/
      dbox[1] =  pSUBWIN_FEATURE (pobj)->FRect[2]; /*ymin*/
      dbox[2] =  pSUBWIN_FEATURE (pobj)->FRect[1]; /*xmax*/
      dbox[3] =  pSUBWIN_FEATURE (pobj)->FRect[3]; /*ymax*/
      dbox[4] =  pSUBWIN_FEATURE (pobj)->FRect[4]; /*zmin*/
      dbox[5] =  pSUBWIN_FEATURE (pobj)->FRect[5]; /*zmax*/

      cube_scaling =  pSUBWIN_FEATURE (pobj)->cube_scaling;

      if(cube_scaling == TRUE)
	{
	  dbox[0] =  0.; 
	  dbox[1] =  1.;
	  dbox[2] =  0.;
	  dbox[3] =  1.;
	  dbox[4] =  0.;
	  dbox[5] =  1.;
	}


      /** changement de coordonnees 3d */
      flag = (long)(pSUBWIN_FEATURE (pobj)->axes.flag[1]+1)/2; /* F.Leray Adding HERE 19.04.04 */

      Cscale.alpha = Alpha =pSUBWIN_FEATURE (pobj)->alpha;
      Cscale.theta = Teta =pSUBWIN_FEATURE (pobj)->theta;
      
      cost=cos((Teta)*M_PI/180.0);cosa=cos((Alpha)*M_PI/180.0);
      sint=sin((Teta)*M_PI/180.0);sina=sin((Alpha)*M_PI/180.0);
      
      Cscale.m[0][0]= -sint    ;    Cscale.m[0][1]= cost      ;    Cscale.m[0][2]= 0;
      Cscale.m[1][0]= -cost*cosa;   Cscale.m[1][1]= -sint*cosa;    Cscale.m[1][2]= sina;
      Cscale.m[2][0]=  cost*sina;   Cscale.m[2][1]= sint*sina;     Cscale.m[2][2]= cosa;
      
      for (ib=0;ib<6 ;ib++) 
	{ 
	  if (flag==0) 
	    dbox[ib]=Cscale.bbox1[ib];
	  else 
	    Cscale.bbox1[ib]=dbox[ib];
	}

      xbox[0]=TRX(dbox[0],dbox[2],dbox[4]);
      ybox[0]=TRY(dbox[0],dbox[2],dbox[4]);
      zbox[0]=TRZ(dbox[0],dbox[2],dbox[4]);
      xbox[1]=TRX(dbox[0],dbox[3],dbox[4]);
      ybox[1]=TRY(dbox[0],dbox[3],dbox[4]);
      zbox[1]=TRZ(dbox[0],dbox[3],dbox[4]);
      xbox[2]=TRX(dbox[1],dbox[3],dbox[4]);
      ybox[2]=TRY(dbox[1],dbox[3],dbox[4]);
      zbox[2]=TRZ(dbox[1],dbox[3],dbox[4]);
      xbox[3]=TRX(dbox[1],dbox[2],dbox[4]);
      ybox[3]=TRY(dbox[1],dbox[2],dbox[4]);
      zbox[3]=TRZ(dbox[1],dbox[2],dbox[4]);
      xbox[4]=TRX(dbox[0],dbox[2],dbox[5]);
      ybox[4]=TRY(dbox[0],dbox[2],dbox[5]);
      zbox[4]=TRZ(dbox[0],dbox[2],dbox[5]);
      xbox[5]=TRX(dbox[0],dbox[3],dbox[5]);
      ybox[5]=TRY(dbox[0],dbox[3],dbox[5]);
      zbox[5]=TRZ(dbox[0],dbox[3],dbox[5]);
      xbox[6]=TRX(dbox[1],dbox[3],dbox[5]);
      ybox[6]=TRY(dbox[1],dbox[3],dbox[5]);
      zbox[6]=TRZ(dbox[1],dbox[3],dbox[5]);
      xbox[7]=TRX(dbox[1],dbox[2],dbox[5]);
      ybox[7]=TRY(dbox[1],dbox[2],dbox[5]);
      zbox[7]=TRZ(dbox[1],dbox[2],dbox[5]);


      /** Calcul des echelles en fonction de la taille du dessin **/
      if ( flag == 1 || flag == 3 )
	{
	  xmmin=  (double) Mini(xbox,8L);xmmax= (double) Maxi(xbox,8L);
	  ymmax=  (double) - Mini(ybox,8L);
	  ymmin=  (double) - Maxi(ybox,8L);
	}
      if ( flag == 2 || flag == 3 )
	{
	  /* get current window size */
	  C2F(dr)("xget","wdim",&verbose,wdim,&narg, PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  getscale2d(WRect,FRect,logf,ARect);
	  wmax=linint((double)wdim[0] * WRect[2]);
	  hmax=linint((double)wdim[1] * WRect[3]); 
	}
      if ( flag == 2 )
	{
	  /* radius and center of the sphere circumscribing the box */
	  dx=dbox[1]-dbox[0]; dy=dbox[3]-dbox[2]; dz=dbox[5]-dbox[4];
	  R= (double) sqrt(dx*dx + dy*dy + dz*dz)/2;
	  xo= (double) (xbox[0]+xbox[6])/2 ;
	  yo= (double) (ybox[0]+ybox[6])/2 ;
	  zo= (double) (zbox[0]+zbox[6])/2 ;
	  xmmin=  (double) xo - R ;
	  xmmax=  (double) xo + R ;
	  ymmax=  (double) -yo + R ;
	  ymmin=  (double) -yo - R ;
	}
      if (flag==2 || flag==3)
	{
	  hx=xmmax-xmmin;
	  hy=ymmax-ymmin;
	  if ( hx/(double)wmax  < hy/(double)hmax ) 
	    {
	      hx1=wmax*hy/hmax;
	      xmmin=xmmin-(hx1-hx)/2.0;
	      xmmax=xmmax+(hx1-hx)/2.0;
	    }
	  else 
	    {
	      hy1=hmax*hx/wmax;
	      ymmin=ymmin-(hy1-hy)/2.0;
	      ymmax=ymmax+(hy1-hy)/2.0;
	    }
	}
      if (flag !=0 )
	{
	  FRect[0]=xmmin;FRect[1]= -ymmax;FRect[2]=xmmax;FRect[3]= -ymmin;
	  set_scale("tftttf",NULL,FRect,aaint,"nn",NULL);
	  Cscale.metric3d=flag; 
	}
           
      /* F.Leray 23.02.04 Mise a 0 du tableau xind pour corriger bug*/
      /* dans le cas ind < 3 ET ybox[tmpind] < ybox[tmpind]*/
	for(i=0;i<8;i++) xind[i] = 0;

      /******/
      if(pSUBWIN_FEATURE (pobj)->isaxes)
	{  
	  flag = pSUBWIN_FEATURE (pobj)->axes.flag[2];
	  /* indices */
	  xmaxi=((double) Maxi(xbox,8L));
	  ind= -1;
	  MaxiInd(xbox,8L,&ind,xmaxi);
	  if ( ind > 3)
	    xind[0]=ind;
	  tmpind=ind;  
	  MaxiInd(xbox,8L,&ind,xmaxi);
	  if ( ind > 3)
	    xind[0]=ind;
	  if (ybox[tmpind] > ybox[ind] )
	    xind[0]=tmpind; 	 
	   
	  if (ind < 0 || ind > 8) 
	    {
	      Scistring("xind out of bounds");
	      xind[0]=0;
	    }
	  Nextind(xind[0],&ind2,&ind3);
	  if (ybox[ind2] > ybox[ind3]) 
	    {
	      xind[1]=ind2;InsideU[0]=ind3;
	    }
	  else 
	    {
	      xind[1]=ind3;InsideU[0]=ind2;
	    }
	  Nextind(ind2,&ind2,&ind3); InsideU[1]=xind[0];
	  InsideU[2]=ind2; 
	  if (InsideU[0] > 3 )
	    InsideU[3]=InsideU[0]-4; 
	  else
	    InsideU[3]=InsideU[0]+4; 
	  xind[2]=ind2;
	  /* le pointeger en bas qui correspond */	  
	  if (ind2 > 3 )
	    xind[3]=ind2-4;
	  else
	    xind[3]=ind2+4;
	  Nextind(xind[3],&ind2,&ind3);
	  if (ybox[ind2] < ybox[ind3]) 
	    {
	      xind[4]=ind2;InsideD[0]=ind3;
	    }
	  else  
	    {
	      xind[4]=ind3;InsideD[0]=ind2;
	    }
	  Nextind(ind2,&ind2,&ind3);
	  InsideD[1]=xind[3];
	  InsideD[2]=ind2;
	  if (InsideD[0] > 3 )
	    InsideD[3]=InsideD[0]-4;
	  else
	    InsideD[3]=InsideD[0]+4;
	  xind[5]=ind2;

	  /* F.Leray Rajout 02.04.04 :*/
	  background=sciGetBackground(pobj);
	  
	  for (i=0; i < 6 ; i++)
	    {
	      ixbox[i]=XScale(xbox[xind[i]]);
	      iybox[i]=YScale(ybox[xind[i]]);
	    }
	  ixbox[6]=ixbox[0];iybox[6]=iybox[0]; p=7,n=1; 
	  /* C2F (dr) ("xset","foreground",&background,&background,&zero,&zero,&zero,PI0,PD0,PD0,PD0,PD0,5L,4096); F.Leray 04.03.04*/
	  C2F(dr)("xget","pattern",&verbose,&color_old,&zero,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xset","pattern",&background,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	 
	  C2F (dr) ("xarea", "v", &p, ixbox, iybox, &n, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);
	  C2F(dr)("xset","pattern",&color_old,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  /***********/
	  /***  hidden axis */
	  if(pSUBWIN_FEATURE (pobj)->axes.rect== 1)
	    { 
	      x[2] = sciGetLineWidth (pobj);
	      C2F (dr) ("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      /* DJ.A 2003 */
	      hiddencolor=pSUBWIN_FEATURE (pobj)->hiddencolor;
	      if (hiddencolor==-1) hiddencolor=0;
	      if (zbox[InsideU[0]] > zbox[InsideD[0]])
		DrawAxis(xbox,ybox,InsideD,hiddencolor);
	      else 
		DrawAxis(xbox,ybox,InsideU,hiddencolor); 	
	      if (Ishidden(pobj))
		pSUBWIN_FEATURE (pobj)->hiddenstate=(InsideU[0] % 4);
	      else
		pSUBWIN_FEATURE (pobj)->hiddenstate=(InsideD[0] % 4);
	    }
	  /**  l'enveloppe cvxe*/
	  x[0] = sciGetForeground (pobj);	 /* F.Leray 05.03.04 Useless or not?? because we used set pattern instead of set foreground (because Windows uses BRUSH and PEN...)*/
	  /* Wrong explanation: We use sciGetForeground in NG mode and used set foreground in old graphic mode*/
	  x[2] = sciGetLineWidth (pobj); /* Adding this line 05.03.04*/
	  x[3] = sciGetLineStyle (pobj);
	  x[4] = 0; 
	  C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
	  C2F(dr)("xget","pattern",&verbose,&pat,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  /* We are in axis_3ddraw() and sciGetEntityType (pobj) == SCI_SUBWIN*/
	  C2F (dr) ("xset","foreground",x,x,x+4,x+4,x+4,PI0,PD0,PD0,PD0,PD0,5L,4096); /* F.Leray 05.03.04 Useless too*/
	  C2F (dr) ("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F (dr) ("xset", "line style", x+3,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  if(pSUBWIN_FEATURE (pobj)->axes.rect!= 1)
	    {
	      for (i=0; i < 4 ; i++)
		{
		  ixbox[i]=XScale(xbox[xind[i+2]]);
		  iybox[i]=YScale(ybox[xind[i+2]]);
		}
	      p=4,n=1;
	      if (flag >=3){C2F(dr)("xpolys","v",ixbox,iybox,x,&n,&p,PI0,PD0,PD0,PD0,PD0,0L,0L);}
	    } 
	  for (i=0; i < 6 ; i++)
	    {
	      ixbox[i]=XScale(xbox[xind[i]]);
	      iybox[i]=YScale(ybox[xind[i]]);
	    }
	  ixbox[6]=ixbox[0];iybox[6]=iybox[0]; p=7,n=1; 
	  if(pSUBWIN_FEATURE (pobj)->axes.rect == 1)
	    if (flag >=3){C2F(dr)("xpolys","v",ixbox,iybox,x,&n,&p,PI0,PD0,PD0,PD0,PD0,0L,0L);}
	  /** graduation ***/
	/*   if (flag>=3) {Axes3dStrings(ixbox,iybox,xind,legend);} */
	  if (flag>=3) {Axes3dStrings(ixbox,iybox,xind);}
	}
      C2F(dr)("xset","pattern",&pat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
} 
void triedre(sciPointObj *pobj, double *xbox, double *ybox, double *zbox, integer *InsideU, integer *InsideD)
{
  integer  x[5],narg = 0;
  integer color_kp,verbose = 0,thick_kp,style_kp;

  C2F(dr)("xget","pattern",&verbose,&color_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /*F.Leray Replacement*/
  C2F(dr)("xget","thickness",&verbose,&thick_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /*F.Leray addings here*/
  C2F(dr)("xget","line style",&verbose,&style_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /*F.Leray addings here*/

  if(sciGetEntityType (pobj) == SCI_SUBWIN) 
    if(pSUBWIN_FEATURE (pobj)->isaxes)
      if(pSUBWIN_FEATURE (pobj)->axes.rect== 1)
	{ 
	  x[0] = sciGetForeground (pobj);	
	  x[2] = sciGetLineWidth (pobj);
	  x[3] = sciGetLineStyle (pobj);
	  x[4] = 0;

	  /* C2F (dr) ("xset","foreground",x,x,x+4,x+4,x+4,PI0,PD0,PD0,PD0,PD0,5L,4096);*/
	  C2F(dr)("xset","pattern",x,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
	  C2F(dr)("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  	  C2F(dr)("xset", "line style", x+3,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

	  if (zbox[InsideU[0]] > zbox[InsideD[0]])
	    DrawAxis(xbox,ybox,InsideU,x[0]);
	  else 
	    DrawAxis(xbox,ybox,InsideD,x[0]);
	}

  C2F(dr)("xset", "line style",&style_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);/*F.Leray addings here*/
  C2F(dr)("xset","thickness",&thick_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);/*F.Leray addings here*/
  C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /*F.Leray addings here*/

}

/**Nextind
 * @author Djalel Abdemouche 10/2003
 * Should be in Action.c file
 */
void Nextind(integer ind1, integer *ind2, integer *ind3)
{
  *ind2 = ind1+1;
  *ind3 = ind1-1;
  if (ind1 > 3)
    {
      if (*ind2 == 8) *ind2 = 4;
      if (*ind3 == 3) *ind3 = 7;
    }
  else
    {
      if (*ind2 == 4) *ind2 = 0;
      if (*ind3 == -1) *ind3 = 3;
    }
}
/**Axes3dStrings
 * @author Djalel Abdemouche 10/2003
 * Should be in Axes.c file
 */
/* int Axes3dStrings(integer *ixbox, integer *iybox, integer *xind, char *legend) */
int Axes3dStrings(integer *ixbox, integer *iybox, integer *xind)
{
  integer verbose=0,narg,xz[2],fontid[2],fontsize_kp,color_kp,size;
  integer iof,barlengthx = 0,barlengthy = 0, posi[2]; 
  char *legx = NULL;
  char *legy = NULL;
  char *legz = NULL;
  char *title = NULL;
  integer rect[4],flag=0,x=0,y=0;
  double ang=0.0, bbox[6];
  int fontsize=-1,textcolor=-1,ticscolor=-1, doublesize ;
  int fontstyle=0; /* F.Leray 08.04.04 */
  sciPointObj *psubwin = NULL;
  sciSubWindow * ppsubwin = NULL;
  int ns=2,style=0,iflag=0,gstyle,trois=3,dash[6];
  double xx[4],yy[4],zz[4],vxx,vxx1,vyy,vyy1,vzz,vzz1,dx;
  integer i,xm,ym,vx[2],vy[2],xg[2],yg[2],j,subtics;
  integer fontid_old[2], textcolor_old;


  psubwin= (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());
  ppsubwin = pSUBWIN_FEATURE (psubwin);

  title= sciGetText(ppsubwin->mon_title);
  legx = sciGetText(ppsubwin->mon_x_label);
  legy = sciGetText(ppsubwin->mon_y_label);
  legz = sciGetText(ppsubwin->mon_z_label);

  /** le cot\'e gauche ( c'est tjrs un axe des Z **/
  xz[0]=Cscale.WIRect1[2] ;
  xz[1]= Cscale.WIRect1[2];
  iof = (xz[0]+xz[1])/50;
  /*x=ixbox[2]-(xz[0]+xz[1])/20 ;y=0.5*iybox[3]+0.5*iybox[2];*/
  
  psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
  ticscolor=pSUBWIN_FEATURE (psubwin)->axes.ticscolor;
  textcolor=sciGetFontForeground(psubwin);
  fontsize=sciGetFontDeciWidth(psubwin)/100;
  fontstyle=sciGetFontStyle(psubwin);
  
  if(sciGetEntityType (psubwin) != SCI_SUBWIN) { 
    sciprint("Impossible case\n");
    return 0;
  }

  bbox[0] =  pSUBWIN_FEATURE (psubwin)->FRect[0]; /*xmin*/
  bbox[1] =  pSUBWIN_FEATURE (psubwin)->FRect[2]; /*xmax*/
  bbox[2] =  pSUBWIN_FEATURE (psubwin)->FRect[1]; /*ymin*/
  bbox[3] =  pSUBWIN_FEATURE (psubwin)->FRect[3]; /*ymax*/ 
  bbox[4] =  pSUBWIN_FEATURE (psubwin)->FRect[4]; /*zmin*/
  bbox[5] =  pSUBWIN_FEATURE (psubwin)->FRect[5]; /*zmax*/
  
  C2F(dr)("xget","font",&verbose,fontid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  fontid[0]= fontstyle;
  fontsize_kp = fontid[1] ;
  if( fontsize == -1 ){ 
    fontid[1]= 1; doublesize = 2;
    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  else {
    fontid[1] = fontsize ;
    doublesize = 2*fontsize;
    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  if ( textcolor != -1 || ticscolor != -1 ) 
    C2F(dr)("xget","pattern",&verbose,&color_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    
 
  for (i=0; i<3 ; i++) {
    xx[i]=pSUBWIN_FEATURE (psubwin)->axes.xlim[i];
    yy[i]=pSUBWIN_FEATURE (psubwin)->axes.ylim[i];
    zz[i]=pSUBWIN_FEATURE (psubwin)->axes.zlim[i];
  } 

  /* main title */ /* We fix the title always at the top */
  rect[0]= Cscale.WIRect1[0];
  rect[1]= Cscale.WIRect1[1];
  rect[2]= Cscale.WIRect1[2];
  rect[3]= Cscale.WIRect1[3]/6;
  textcolor_old = textcolor;
  fontid_old[0] = fontid[0];
  fontid_old[1] = fontid[1];

  textcolor = sciGetFontForeground(ppsubwin->mon_title);
  fontid[0] = sciGetFontStyle(ppsubwin->mon_title);
  fontid[1] = sciGetFontDeciWidth(ppsubwin->mon_title)/100;
  
  C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  
  if( sciGetVisibility(ppsubwin->mon_title) == TRUE)
    C2F(dr1)("xstringtt",title,&rect[0],&rect[1],&rect[2],&rect[3],PI0,PI0,PD0,PD0,PD0,PD0,10L,0L);
  
  textcolor = textcolor_old;
  fontid[0] = fontid_old[0];
  fontid[1] = fontid_old[1];

  size = xz[0]>=xz[1] ? (integer) (xz[1]/50.0) : (integer) (xz[0]/50.0); 
    
  /*** le z scaling ***/
  if ( pSUBWIN_FEATURE (psubwin)->project[2]==1)
    {
      double fx,fy,fz; 
      char str[100];
      integer Ticsdir[2];
      subtics=pSUBWIN_FEATURE (psubwin)->axes.subint[2];
      Ticsdir[0]=ixbox[3]-ixbox[4];
      Ticsdir[1]=iybox[3]-iybox[4];
      BBoxToval(&fx,&fy,&fz,xind[3],bbox);
      NumberFormat(str,((integer) zz[0]),((integer) zz[2]));
      C2F(dr)("xstringl",str,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
      x=ixbox[2]-(xz[0]+xz[1])/20;y=(iybox[3]+iybox[2])/2;/* DJ.A 2003 */
      zz[3]=inint(zz[1]-zz[0]);
      while (zz[3]>10)  zz[3]=floor(zz[3]/2);
      zz[3]=10; 
      while (((zz[3]*1.5*rect[3]) > (iybox[3]-iybox[2])) && (zz[3]>2))
	zz[3]=floor(zz[3]/2) +1;
      if (zz[3]<=2) zz[3]=2;
      /** loop on the ticks **/
      for (i=0 ; i < zz[3]+1 ; i++)
	{  char foo[100]; 
	vzz = exp10(zz[2])*(zz[0] + i*ceil((zz[1]-zz[0])/zz[3]));
	trans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz);
	vx[0]=xm;vy[0]=ym;
	if ((ym >= iybox[2]) && (ym <= iybox[3]))
	  {
	    barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
	    barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
	    vx[1]=vx[0]+barlengthx;
	    vy[1]=vy[0]+barlengthy;
	    NumberFormat(foo,((integer) (zz[0] + i*ceil((zz[1]-zz[0])/zz[3]))),
			 ((integer) zz[2]));
	    C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    posi[0] = inint( xm+2*barlengthx - rect[2]); 
	    posi[1]=inint( ym + 2*barlengthy + rect[3]/2);
	    C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
	    C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
	    C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    if (pSUBWIN_FEATURE (psubwin)->grid[2] > -1)
	      {
		gstyle = pSUBWIN_FEATURE (psubwin)->grid[2];
		if ((ym != iybox[3]) && (ym != iybox[2]))
		  { 
		    C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F (dr) ("xset", "line style",&trois,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
		    xg[0]= ixbox[3];yg[0]= ym; 
		    if (Ishidden(psubwin))
		      {  xg[1]=ixbox[4];  yg[1]= iybox[4]- iybox[3]+ym;}
		    else
		      {xg[1]=ixbox[1];  yg[1]= iybox[1]- iybox[2]+ym;} 
		    C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    xg[0]=xg[1];  ; xg[1] =ixbox[0];
		    yg[0]=yg[1]; yg[1]= ym- iybox[3]+ iybox[5];
		    C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }
	      }
	  }
	/* Ajout subtics  Dj.A 17/12/2003 **/
	vzz1= exp10(zz[2])*(zz[0] + (i+1)*ceil((zz[1]-zz[0])/zz[3]));
	dx = (vzz1-vzz)/(subtics+1);
	for ( j = 1 ; j < subtics+1; j++)
	  {  
	    vzz1=vzz+dx*j;
	    trans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1);
	    vx[0]=xm;vy[0]=ym;
	    vx[1]= (integer) (vx[0]+barlengthx/2.0);
	    vy[1]= (integer) (vy[0]+barlengthy/2.0);
	    if ((ym <= iybox[3]) && (ym >= iybox[2]))
	      C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  }
	/***/
	}
      if (legz != 0)
	{
	  /* F.Leray Adding 1 line here ("xset","pattern") to force the color and style of the */
	  /* legend to be the same as those used for the numbers for the axes*/

	  textcolor_old = textcolor;
	  fontid_old[0] = fontid[0];
	  fontid_old[1] = fontid[1];
	  
	  textcolor = sciGetFontForeground(ppsubwin->mon_z_label);
	  fontid[0] = sciGetFontStyle(ppsubwin->mon_z_label);
	  fontid[1] = sciGetFontDeciWidth(ppsubwin->mon_z_label)/100;
	  	  
	  C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xstringl",legz,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  if( sciGetVisibility(ppsubwin->mon_z_label) == TRUE)
	    C2F(dr)("xstring",legz,(x=x - rect[3],&x),&y,PI0,&flag
		    ,PI0,PI0,&ang,PD0,PD0,PD0,0L,0L);
	  
	  textcolor = textcolor_old;
	  fontid[0] = fontid_old[0];
	  fontid[1] = fontid_old[1];
	}
    }
  /*** le x-y scaling ***/
  /** le cote en bas a droite ***/
  if (( xind[4]+xind[5] == 3) || ( xind[4]+xind[5] == 11))
    {
      if (pSUBWIN_FEATURE (psubwin)->project[0]==1)
	{
	  double fx,fy,fz;
	  char str[100];
	  integer Ticsdir[2]; 
	  subtics=pSUBWIN_FEATURE (psubwin)->axes.subint[0];
	  Ticsdir[0]=ixbox[4]-ixbox[3];
	  Ticsdir[1]=iybox[4]-iybox[3];
	  BBoxToval(&fx,&fy,&fz,xind[4],bbox);
	  NumberFormat(str,((integer) xx[0]),((integer) xx[2]));
	  C2F(dr)("xstringl",str,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  x=inint((ixbox[4]+ixbox[5])/2+1.5*rect[2] +iof);
	  y=inint(((2/3.0)*iybox[4]+(1/3.0)*iybox[5])+1.5*rect[3]+iof);  	      
	  xx[3]=inint(xx[1]-xx[0]);
	  while (xx[3]>10)  xx[3]=floor(xx[3]/2);
	  while (((xx[3]*1.5*rect[3]) > (iybox[4]-iybox[5])) 
		 && ((xx[3]*1.5*rect[2]) > (ixbox[5]-ixbox[4])) && (xx[3]>2))
	    xx[3]=floor(xx[3]/2)+1;
	  if (xx[3]<=2) xx[3]=2;		       
	  /** loop on the ticks **/
	  for (i=0 ; i < xx[3]+1 ; i++)
	    {  char foo[100]; 
	    vxx = exp10(xx[2])*(xx[0] + i*ceil((xx[1]-xx[0])/xx[3]));
	    trans3d(psubwin,1,&xm,&ym,&vxx,&fy,&fz);
	    vx[0]=xm;vy[0]=ym; 
	    /**/
	    if ((xm <= ixbox[5]) && (xm >= ixbox[4]) && (ym >= iybox[5]) && (ym <= iybox[4]))
	      {
		barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		NumberFormat(foo,((integer) (xx[0] + i*ceil((xx[1]-xx[0])/xx[3]))),
			     ((integer) xx[2])); 
		C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		if (IsDownAxes(psubwin)){
		  vx[1]=vx[0];
		  vy[1]=vy[0]+iof/2;
		  posi[0] = inint(xm-rect[2]/2); 
		  posi[1]=inint( vy[0] + iof + rect[3]);}
		else{
		  vx[1]=vx[0]+barlengthx;
		  vy[1]=vy[0]+barlengthy;
		  posi[0] = inint( xm+2*barlengthx);
		  posi[1]=inint( ym + 2*barlengthy + rect[3]);}
		C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
		C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		if (pSUBWIN_FEATURE (psubwin)->grid[0] > -1)
		  {
		    gstyle = pSUBWIN_FEATURE (psubwin)->grid[0];
		    if ((xm != ixbox[5]) && (xm != ixbox[4]))
		      { 
			xg[0]= xm;  yg[0]= ym;  
			if (Ishidden(psubwin)) 
			  { xg[1]= xm; yg[1]= iybox[2] -iybox[3]+ym; }
			else
			  {xg[1]= ixbox[3] - ixbox[4] +xm; yg[1]= iybox[3] - iybox[4] +ym; } 
			C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F (dr) ("xset", "line style",&trois,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			xg[0]= xg[1]; yg[0]= yg[1];
			xg[1] = ixbox[3] - ixbox[4] +xm; 
			yg[1]=  iybox[2] - iybox[4] +ym;
			C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
		      }	
		  } 
	      }
	    /* Ajout subtics  Dj.A 17/12/2003 **/
	    vxx1 = exp10(xx[2])*(xx[0] + (i+1)*ceil((xx[1]-xx[0])/xx[3]));
	    dx = (vxx1-vxx)/(subtics+1);
	    for ( j = 1 ; j < subtics+1; j++)
	      {  
		vxx1=vxx+dx*j;
		trans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
		if (IsDownAxes(psubwin))
		  {
		    vx[1]=vx[0]=xm;
		    vy[0]=ym;
		    vy[1]=vy[0]+iof/4;
		  }
		else
		  {
		    vx[0]=xm;vy[0]=ym;
		    vx[1]= (integer) (vx[0]+barlengthx/2.0);
		    vy[1]= (integer) (vy[0]+barlengthy/2.0);
		  }
		     
		if ((ym >= iybox[5]) && (ym <= iybox[4]) && (xm <= ixbox[5]) && (xm >= ixbox[4])) 
		  C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      }
	    } 
	  if (legx != 0)
	    {
	      /* F.Leray Adding 1 line here ("xset","pattern") to force the color and style of the */
	      /* legend to be the same as those used for the numbers for the axes*/
	      
	      textcolor_old = textcolor;
	      fontid_old[0] = fontid[0];
	      fontid_old[1] = fontid[1];
	      
	      textcolor = sciGetFontForeground(ppsubwin->mon_x_label);
	      fontid[0] = sciGetFontStyle(ppsubwin->mon_x_label);
	      fontid[1] = sciGetFontDeciWidth(ppsubwin->mon_x_label)/100;
	      
	      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      C2F(dr)("xstringl",legx,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

	      if( sciGetVisibility(ppsubwin->mon_x_label) == TRUE)
		C2F(dr)("xstring",legx,(x=x-rect[2],&x),&y,PI0,&flag
			,PI0,PI0,&ang,PD0,PD0,PD0,0L,0L);

	      textcolor = textcolor_old;
	      fontid[0] = fontid_old[0];
	      fontid[1] = fontid_old[1];

	    }
	}
    }
  else
    {
      if ( pSUBWIN_FEATURE (psubwin)->project[1]==1)
	{
	  double fx,fy,fz; 
	  char str[100];
	  integer Ticsdir[2];
	  subtics=pSUBWIN_FEATURE (psubwin)->axes.subint[1];
	  Ticsdir[0]=ixbox[4]-ixbox[3];
	  Ticsdir[1]=iybox[4]-iybox[3];
	  BBoxToval(&fx,&fy,&fz,xind[4],bbox);
	  NumberFormat(str,((integer) yy[0]),((integer) yy[2]));
	  C2F(dr)("xstringl",str,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	      
	  x=inint((ixbox[4]+ixbox[5])/2+1.5*rect[2] +iof);
	  y=inint(((2/3.0)*iybox[4]+(1/3.0)*iybox[5])+1.5*rect[3]+iof);
	  yy[3]=inint(yy[1]-yy[0]);
	  while (yy[3]>10)  yy[3]=floor(yy[3]/2);
	  while (((yy[3]*1.5*rect[3]) > (iybox[4]-iybox[5])) 
		 && ((yy[3]*1.5*rect[2]) > (ixbox[5]-ixbox[4])) && (yy[3]>2))
	    yy[3]=floor(yy[3]/2)+1;
	  if (yy[3]<=2) yy[3]=2;
	  /** loop on the ticks **/
	  for (i=0 ; i < yy[3]+1 ; i++)
	    { char foo[100]; 
	    vyy = exp10(yy[2])*(yy[0] + i*ceil((yy[1]-yy[0])/yy[3]));
	    trans3d(psubwin,1,&xm,&ym,&fx,&vyy,&fz);
	    vx[0]=xm;vy[0]=ym;
	    /**/
	    if ((xm <= ixbox[5]) && (xm >= ixbox[4]) && (ym >= iybox[5]) && (ym <= iybox[4]))
	      {
		barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		NumberFormat(foo,((integer) (yy[0] + i*ceil((yy[1]-yy[0])/yy[3]))),
			     ((integer) yy[2]));
		C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		if (IsDownAxes(psubwin)){
		  vx[1]=vx[0];
		  vy[1]=vy[0]+iof/2;
		  posi[0] = inint(xm-rect[2]/2); 
		  posi[1]=inint( vy[0] + iof + rect[3]);}
		else{ 
		  vx[1]=vx[0]+barlengthx;
		  vy[1]=vy[0]+barlengthy;
		  posi[0] = inint( xm+2*barlengthx - rect[2]/2);
		  posi[1]=inint( ym + 2*barlengthy + rect[3]);}
		C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
		C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		if (pSUBWIN_FEATURE (psubwin)->grid[1] > -1)
		  {
		    gstyle = pSUBWIN_FEATURE (psubwin)->grid[1];
		    if ((xm != ixbox[5]) && (xm != ixbox[4]))
		      { 
			xg[0]= xm;  yg[0]= ym;  
			if (Ishidden(psubwin))
			  { xg[1]= xm; yg[1]= iybox[2] -iybox[3]+ym; }
			else
			  {xg[1]= ixbox[3] - ixbox[4] +xm; yg[1]= iybox[3] - iybox[4] +ym; } 
			C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F (dr) ("xset", "line style",&trois,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			xg[0]= xg[1]; yg[0]= yg[1];
			xg[1] = ixbox[3] - ixbox[4] +xm; 
			yg[1]=  iybox[2] - iybox[4] +ym;
			C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
		      }	
		  } 
	      }
	    /* Ajout subtics  Dj.A 17/12/2003 **/
	    vyy1 = exp10(yy[2])*(yy[0] + (i+1)*ceil((yy[1]-yy[0])/yy[3]));
	    dx = (vyy1-vyy)/(subtics+1);
	    for ( j = 1 ; j < subtics+1; j++)
	      {  
		vyy1=vyy+dx*j;
		trans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
		if (IsDownAxes(psubwin))
		  {
		    vx[1]=vx[0]=xm;
		    vy[0]=ym;
		    vy[1]=vy[0]+iof/4;
		  }
		else
		  {
		    vx[0]=xm;vy[0]=ym;
		    vx[1]= (integer) (vx[0]+barlengthx/2.0);
		    vy[1]= (integer) (vy[0]+barlengthy/2.0);
		  }
		if ((ym >= iybox[5]) && (ym <= iybox[4]) && (xm <= ixbox[5]) && (xm >= ixbox[4]))
		  C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      }
	    }
	  if (legy != 0) 
	    { 
	      /* F.Leray Adding 1 line here ("xset","pattern") to force the color and style of the */
	      /* legend to be the same as those used for the numbers for the axes*/

	      textcolor_old = textcolor;
	      fontid_old[0] = fontid[0];
	      fontid_old[1] = fontid[1];
	      
	      textcolor = sciGetFontForeground(ppsubwin->mon_y_label);
	      fontid[0] = sciGetFontStyle(ppsubwin->mon_y_label);
	      fontid[1] = sciGetFontDeciWidth(ppsubwin->mon_y_label)/100;
	      	      
	      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      C2F(dr)("xstringl",legy,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* Adding F.Leray too */
	      if( sciGetVisibility(ppsubwin->mon_y_label) == TRUE)
		C2F(dr)("xstring",legy,&x,&y,PI0,&flag,PI0,PI0,&ang,PD0,PD0,PD0,0L,0L);

	      textcolor = textcolor_old;
	      fontid[0] = fontid_old[0];
	      fontid[1] = fontid_old[1];
	    }
	}
    }
  /*** le cote en bas a gauche **/
  if (( xind[3]+xind[4] == 3) || ( xind[3]+xind[4] == 11))
    {
      if (pSUBWIN_FEATURE (psubwin)->project[0]==1)
	{
	  double fx,fy,fz;
	  char str[100];
	  integer Ticsdir[2];
	  subtics=pSUBWIN_FEATURE (psubwin)->axes.subint[0];
	  Ticsdir[0]=ixbox[4]-ixbox[5];
	  Ticsdir[1]=iybox[4]-iybox[5];
	  BBoxToval(&fx,&fy,&fz,xind[3],bbox);
	  NumberFormat(str,((integer) xx[0]),((integer) xx[2]));
	  C2F(dr)("xstringl",str,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	      
	  x=inint((ixbox[3]+ixbox[4])/2.0 -rect[2] -iof);
	  y=inint((1/3.0)*iybox[3]+(2/3.0)*iybox[4]+ iof+ 1.5*rect[3]);  
	  xx[3]=inint(xx[1]-xx[0]);
	  while (xx[3]>10)  xx[3]=floor(xx[3]/2); 
	  while (((xx[3]*1.5*rect[3]) > (iybox[4]-iybox[3])) 
		 && ((xx[3]*1.5*rect[2]) > (ixbox[4]-ixbox[3])) && (xx[3]>2))
	    xx[3]=floor(xx[3]/2)+1;
	  if (xx[3]<=2) xx[3]=2;
	  /** loop on the ticks **/
	  for (i=0 ; i < xx[3]+1 ; i++)
	    { char foo[100];
	    vxx = exp10(xx[2])*(xx[0] + i*ceil((xx[1]-xx[0])/xx[3]));
	    trans3d(psubwin,1,&xm,&ym,&vxx,&fy,&fz);
	    vx[0]=xm;vy[0]=ym;
	    /**/
	    if ((xm <= ixbox[4]) && (xm >= ixbox[3]) && (ym <= iybox[4]) && (ym >= iybox[3]))
	      {
		barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		NumberFormat(foo,((integer) (xx[0] + i*ceil((xx[1]-xx[0])/xx[3]))),
			     ((integer) xx[2]));
		C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		if (IsDownAxes(psubwin)){
		  vx[1]=vx[0];
		  vy[1]=vy[0]+iof/2;
		  posi[0] = inint(xm-rect[2]/2); 
		  posi[1]=inint( vy[0] + iof + rect[3]);}
		else{
		  vx[1]=vx[0]+barlengthx;
		  vy[1]=vy[0]+barlengthy;
		  posi[0] = inint( xm+2*barlengthx-rect[2]); 
		  posi[1]=inint( ym + 2*barlengthy + rect[3]);}
		C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
		C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		if (pSUBWIN_FEATURE (psubwin)->grid[0] > -1)
		  {
		    gstyle = pSUBWIN_FEATURE (psubwin)->grid[0];
		    if ((xm != ixbox[3]) && (xm != ixbox[4]))
		      { 
			xg[0]= xm;  yg[0]= ym;  
			if (Ishidden(psubwin))
			  { xg[1]= xm; yg[1]= iybox[0] -iybox[5]+ym; }
			else
			  {xg[1]= ixbox[1] - ixbox[3] +xm; yg[1]= iybox[5] - iybox[4] +ym; } 
			C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F (dr) ("xset", "line style",&trois,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			xg[0]= xg[1]; yg[0]= yg[1];
			xg[1] = ixbox[1] - ixbox[3] +xm; yg[1]=  iybox[0] - iybox[4] +ym;
			C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
		      }	
		  } 	    
	      }
	    /* Ajout subtics  Dj.A 17/12/2003 **/
	    vxx1 = exp10(xx[2])*(xx[0] + (i+1)*ceil((xx[1]-xx[0])/xx[3]));
	    dx = (vxx1-vxx)/(subtics+1);
	    for ( j = 1 ; j < subtics+1; j++)
	      {  
		vxx1=vxx+dx*j;
		trans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
		if (IsDownAxes(psubwin))
		  {
		    vx[1]=vx[0]=xm;
		    vy[0]=ym;
		    vy[1]=vy[0]+iof/4;
		  }
		else
		  {
		    vx[0]=xm;vy[0]=ym;
		    vx[1]= (integer) (vx[0]+barlengthx/2.0);
		    vy[1]= (integer) (vy[0]+barlengthy/2.0);
		  }
		if ((ym >= iybox[3]) && (ym <= iybox[4]) && (xm >= ixbox[3]) && (xm <= ixbox[4]))
		  C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      }
	    }
	  if (legx != 0)
	    { /* F.Leray Adding 1 line here ("xset","pattern") to force the color and style of the */
	      /* legend to be the same as those used for the numbers for the axes*/
	      
	      textcolor_old = textcolor;
	      fontid_old[0] = fontid[0];
	      fontid_old[1] = fontid[1];
	      
	      textcolor = sciGetFontForeground(ppsubwin->mon_x_label);
	      fontid[0] = sciGetFontStyle(ppsubwin->mon_x_label);
	      fontid[1] = sciGetFontDeciWidth(ppsubwin->mon_x_label)/100;
	      
	      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      C2F(dr)("xstringl",legx,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      if( sciGetVisibility(ppsubwin->mon_x_label) == TRUE)
		C2F(dr)("xstring",legx,(x=x-rect[2],&x),&y,PI0,&flag
			,PI0,PI0,&ang,PD0,PD0,PD0,0L,0L);

	      textcolor = textcolor_old;
	      fontid[0] = fontid_old[0];
	      fontid[1] = fontid_old[1];
	    }
	}
    }
  else 
    {
      if  (pSUBWIN_FEATURE (psubwin)->project[1]==1)
	{
	  double fx,fy,fz;
	  char str[100]; 
	  integer Ticsdir[2];
	  subtics=pSUBWIN_FEATURE (psubwin)->axes.subint[1];
	  Ticsdir[0]=ixbox[4]-ixbox[5];
	  Ticsdir[1]=iybox[4]-iybox[5];
	  BBoxToval(&fx,&fy,&fz,xind[3],bbox);
	  NumberFormat(str,((integer) yy[0]),((integer) yy[2]));
	  C2F(dr)("xstringl",str,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	      
	  x=inint((ixbox[3]+ixbox[4])/2.0 -rect[2] -iof);
	  y=inint((1/3.0)*iybox[3]+(2/3.0)*iybox[4]+ iof + 1.5*rect[3]);  
	  yy[3]=inint(yy[1]-yy[0]);
	  while (yy[3]>10)  yy[3]=floor(yy[3]/2);
	  while (((yy[3]*1.5*rect[3]) > (iybox[4]-iybox[3])) 
		 && ((yy[3]*1.5*rect[2]) > (ixbox[4]-ixbox[3])) && (yy[3]>2))
	    yy[3]=floor(yy[3]/2)+1;
	  if (yy[3]<=2) yy[3]=2;
	  /** loop on the ticks **/
	  for (i=0 ; i < yy[3]+1 ; i++)
	    { char foo[100];
	    vyy = exp10(yy[2])*(yy[0] + i*ceil((yy[1]-yy[0])/yy[3]));
	    trans3d(psubwin,1,&xm,&ym,&fx,&vyy,&fz);
	    vx[0]=xm;vy[0]=ym; 
	    /**/
	    if ((xm <= ixbox[4]) && (xm >= ixbox[3]) && (ym <= iybox[4]) && (ym >= iybox[3]))
	      {
		barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		NumberFormat(foo,((integer) (yy[0] + i*ceil((yy[1]-yy[0])/yy[3]))),
			     ((integer) yy[2]));
		C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		if (IsDownAxes(psubwin)){
		  vx[1]=vx[0];
		  vy[1]=vy[0]+iof/2;
		  posi[0] = inint(xm-rect[2]/2); 
		  posi[1]=inint( vy[0] + iof + rect[3]);}
		else{
		  vx[1]=vx[0]+barlengthx;
		  vy[1]=vy[0]+barlengthy;
		  posi[0] = inint( xm+2*barlengthx-rect[2]/2); 
		  posi[1]=inint( ym + 2*barlengthy + rect[3]);}
		C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
		C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		if (pSUBWIN_FEATURE (psubwin)->grid[1] > -1)
		  {
		    gstyle = pSUBWIN_FEATURE (psubwin)->grid[1];
		    if ((xm != ixbox[3]) && (xm != ixbox[4]))
		      { 
			xg[0]= xm;  yg[0]= ym;  
			if (Ishidden(psubwin))
			  { xg[1]= xm; yg[1]= iybox[0] -iybox[5]+ym; }
			else
			  {xg[1]= ixbox[1] - ixbox[3] +xm; yg[1]= iybox[5] - iybox[4] +ym; } 
			C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F (dr) ("xset", "line style",&trois,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			xg[0]= xg[1]; yg[0]= yg[1];
			xg[1] = ixbox[1] - ixbox[3] +xm; yg[1]=  iybox[0] - iybox[4] +ym;
			C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
		      }	
		  } 	     
	      }
	    /* Ajout subtics  Dj.A 17/12/2003 **/
	    vyy1 = exp10(yy[2])*(yy[0] + (i+1)*ceil((yy[1]-yy[0])/yy[3]));
	    dx = (vyy1-vyy)/(subtics+1);
	    for ( j = 1 ; j < subtics+1; j++)
	      {  
		vyy1=vyy+dx*j;
		trans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
		if (IsDownAxes(psubwin))
		  {
		    vx[1]=vx[0]=xm;
		    vy[0]=ym;
		    vy[1]=vy[0]+iof/4;
		  }
		else
		  {
		    vx[0]=xm;vy[0]=ym;
		    vx[1]= (integer) (vx[0]+barlengthx/2.0);
		    vy[1]= (integer) (vy[0]+barlengthy/2.0);
		  }
		if ((ym >= iybox[3]) && (ym <= iybox[4]) && (ym >= ixbox[3]) && (xm <= ixbox[4]))
		  C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      }
	    }
	  if (legy != 0)
	    {  /* F.Leray Adding 1 line here ("xset","pattern") to force the color and style of the */
	      /* legend to be the same as those used for the numbers for the axes*/

	      textcolor_old = textcolor;
	      fontid_old[0] = fontid[0];
	      fontid_old[1] = fontid[1];
	      
	      textcolor = sciGetFontForeground(ppsubwin->mon_y_label);
	      fontid[0] = sciGetFontStyle(ppsubwin->mon_y_label);
	      fontid[1] = sciGetFontDeciWidth(ppsubwin->mon_y_label)/100;
	      
	      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      C2F(dr)("xstringl",legy,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      if( sciGetVisibility(ppsubwin->mon_y_label) == TRUE)
		C2F(dr)("xstring",legy,(x=x-rect[2],&x),&y,PI0,&flag
			,PI0,PI0,&ang,PD0,PD0,PD0,0L,0L);
	      
	      textcolor = textcolor_old;
	      fontid[0] = fontid_old[0];
	      fontid[1] = fontid_old[1];
	    }
	}
    }
  /* reset font to its current size & to current color*/ 
  if ( fontsize != -1 ){
    fontid[1] = fontsize_kp;
    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  if ( textcolor != -1 || ticscolor != -1 ) 
    C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  /***/
  /* FREE(loc); */

  
  return 0;
}

int trans3d(sciPointObj *pobj,integer n,integer *xm,integer *ym,double *x, double *y,double *z)
{
  integer i;
  double tmpx,tmpy,tmpz;
  /* TEST F.Leray 20.04.04: I fix HERE temporarily BOOL cube_scaling = FALSE; */
  BOOL cube_scaling;

  if (sciGetEntityType(pobj) == SCI_SUBWIN){

    cube_scaling = pSUBWIN_FEATURE(pobj)->cube_scaling;

    /*if (pSUBWIN_FEATURE (pobj)->isoview)*/
    if(cube_scaling == FALSE)
      {
	if (z == (double *) NULL)
	  for ( i=0 ; i < n ; i++)
	    {
	      xm[i]= TX3D(x[i],y[i],0.0);
	      ym[i]= TY3D(x[i],y[i],0.0);
	      if ( finite(xz1)==0||finite(yz1)==0 ) return(0);
	    }
	else
	  for ( i=0 ; i < n ; i++)
	    {
	      xm[i]= TX3D(x[i],y[i],z[i]);
	      ym[i]= TY3D(x[i],y[i],z[i]);
	      if ( finite(xz1)==0||finite(yz1)==0 ) return(0);
	    }
      }
    else
      {
	if (z == (double *) NULL)
	  for ( i=0 ; i < n ; i++)
	    {
	      tmpx=(x[i]-pSUBWIN_FEATURE (pobj)->FRect[0])/(pSUBWIN_FEATURE (pobj)->FRect[2]-pSUBWIN_FEATURE (pobj)->FRect[0]);
	      tmpy= (y[i]-pSUBWIN_FEATURE (pobj)->FRect[1])/(pSUBWIN_FEATURE (pobj)->FRect[3]-pSUBWIN_FEATURE (pobj)->FRect[1]);
	      xm[i]= TX3D(tmpx,tmpy,0.0);
	      ym[i]= TY3D(tmpx,tmpy,0.0);
	      if ( finite(xz1)==0||finite(yz1)==0 ) return(0);
	    }
	else
	  for ( i=0 ; i < n ; i++)
	    {
	      tmpx=(x[i]-pSUBWIN_FEATURE (pobj)->FRect[0])/(pSUBWIN_FEATURE (pobj)->FRect[2]-pSUBWIN_FEATURE (pobj)->FRect[0]);
	      tmpy= (y[i]-pSUBWIN_FEATURE (pobj)->FRect[1])/(pSUBWIN_FEATURE (pobj)->FRect[3]-pSUBWIN_FEATURE (pobj)->FRect[1]);
	      tmpz= (z[i]-pSUBWIN_FEATURE (pobj)->FRect[4])/(pSUBWIN_FEATURE (pobj)->FRect[5]-pSUBWIN_FEATURE (pobj)->FRect[4]); /* Adding F.Leray 28.04.04 */
	      xm[i]= TX3D(tmpx,tmpy,tmpz);
	      ym[i]= TY3D(tmpx,tmpy,tmpz);
	      if ( finite(xz1)==0||finite(yz1)==0 ) return(0);
	    }
      }
  }
  return(1);
}

BOOL Ishidden(sciPointObj *pobj)
{
  double alpha;
  if (sciGetEntityType(pobj) == SCI_SUBWIN){
    alpha = pSUBWIN_FEATURE (pobj)->alpha;
    if ((alpha <0.0 ) && (alpha > -90.0)) 
      return TRUE;
    if ((alpha <-180.0 ) && (alpha > -270.0))
      return TRUE;
    if ((alpha <180.0 ) && (alpha > 90.0))
      return TRUE;
  }
  return FALSE;
}
BOOL IsDownAxes(sciPointObj *pobj)
{
  double alpha,cof;
  
  if (sciGetEntityType(pobj) == SCI_SUBWIN){
    alpha = pSUBWIN_FEATURE (pobj)->alpha;   
    if (!(pSUBWIN_FEATURE (pobj)->isoview))
      cof=10.0;
    else
		/* Correction Warnings Attention Precision*/
      cof= (double) (Min(5.0,ceil(Max(
				    abs((int)pSUBWIN_FEATURE (pobj)->axes.xlim[1]-(int)pSUBWIN_FEATURE (pobj)->axes.xlim[0])/
				    abs((int)pSUBWIN_FEATURE (pobj)->axes.ylim[1]-(int)pSUBWIN_FEATURE (pobj)->axes.ylim[0]),
				    abs((int)pSUBWIN_FEATURE (pobj)->axes.ylim[1]-(int)pSUBWIN_FEATURE (pobj)->axes.ylim[0])/
				    abs((int)pSUBWIN_FEATURE (pobj)->axes.xlim[1]-(int)pSUBWIN_FEATURE (pobj)->axes.xlim[0])))));
    if (cof == 0 ) cof =5;
    if ((alpha <=(-90.0+cof) ) && (alpha >= (-90.0-cof))) 
      return TRUE;
    if ((alpha <=(-270.0+cof) ) && (alpha >= (-270.0-cof)))
      return TRUE;
    if ((alpha <=(90.0+cof) ) && (alpha >= (90.0-cof)))
      return TRUE;
    if ((alpha <=(270.0+cof) ) && (alpha >= (270.0-cof)))
      return TRUE;
  }
  return FALSE;
}
void Plo2dTo3d(integer type, integer *n1, integer *n2, double *x, double *y, double *z, double *x1, double *y1, double *z1)
{
  integer i,j;
  switch (type)
    {
    case 2:
      /** Computing y/z-values **/
      for ( i=0 ; i < (*n2) ; i++)
	for (j=0 ; j< (*n1) ; j++)
	  {
	    y1[2*i+1+2*(*n2)*j]= y1[2*i+2*(*n2)*j]= y[i+(*n2)*j];
	    if (z == NULL)
	      z1 = (double *) NULL;
	    else
	      z1[2*i+1+2*(*n2)*j]= z1[2*i+2*(*n2)*j]= z[i+(*n2)*j];
	  }
      /*ym[2*i+1+2*(*n2)*j]= ym[2*i+2*(*n2)*j]= YScale(y[i+(*n2)*j]);*/
     
      /** Computing x-values **/
      for (j=0 ; j< (*n1) ; j++)
	{
	  for ( i=1 ; i < (*n2) ; i++)
	    {
	      x1[2*i+2*(*n2)*j]= x[i+(*n2)*j];
	      x1[2*i-1+2*(*n2)*j]=x1[2*i+2*(*n2)*j];
	    }
	  x1[2*(*n2)*j]= x[(*n2)*j];
	  x1[2*(*n2)-1+ 2*(*n2)*j]= x1[2*(*n2-1)+ 2*(*n2)*j];
	}
      break;
    case 3:
      /** Computing y-values **/
      for ( i=0 ; i < (*n2) ; i++)
	for (j=0 ; j< (*n1) ; j++)
	  {
	    y1[2*i+1+2*(*n2)*j]= 0.0;
	    y1[2*i+2*(*n2)*j]= y[i+(*n2)*j];
	  }
      /** Computing x/z-values **/
      for (j=0 ; j< (*n1) ; j++)
	{
	  for ( i=0 ; i < (*n2) ; i++)
	    {
	      x1[2*i+2*(*n2)*j]= x[i+(*n2)*j];
	      x1[2*i+1+2*(*n2)*j]=x1[2*i+2*(*n2)*j]; 
	      if (z == NULL)
		z1 = (double *) NULL;
	      else
		{
		  z1[2*i+2*(*n2)*j]= z[i+(*n2)*j];
		  z1[2*i+1+2*(*n2)*j]=z1[2*i+2*(*n2)*j];
		}
	    }
	}
      break;
    case 4: 
      /** Computing y-values **/
      for ( i=0 ; i < (*n2) ; i++)
	for (j=0 ; j< (*n1) ; j++)
	  y1[2*i+2*(*n2)*j]= y[i+(*n2)*j];
      for ( i=0 ; i < (*n2)-1 ; i++)
	for (j=0 ; j< (*n1) ; j++)
	  y1[2*i+1+2*(*n2)*j]=y1[2*i+2+2*(*n2)*j]; 
      /** Computing x-values **/
      for (j=0 ; j< (*n1) ; j++)
	for ( i=0 ; i < (*n2) ; i++)
	  x1[2*i+2*(*n2)*j]= x[i+(*n2)*j];
      for (j=0 ; j< (*n1) ; j++)
	for ( i=0 ; i < (*n2)-1 ; i++)
	  x1[2*i+1+2*(*n2)*j]=x1[2*i+2+2*(*n2)*j];
      /** Computing z-values **/
      if (z == NULL)
	z1 = (double *) NULL;
      else
	{
	  for (j=0 ; j< (*n1) ; j++)
	    for ( i=0 ; i < (*n2) ; i++)
	      z1[2*i+2*(*n2)*j]= z[i+(*n2)*j];
	  for (j=0 ; j< (*n1) ; j++)
	    for ( i=0 ; i < (*n2)-1 ; i++)
	      z1[2*i+1+2*(*n2)*j]=z1[2*i+2+2*(*n2)*j];
	}
      break;
    default:
      break;
    } 
}

double Fill_XYdec01_TLO_and_ISO_case(int Xdec3, double val)
{
 
  return (double) (val / (exp10(Xdec3)));
}

/*** F.Leray 02.04.04 */
/* FUNCTION FOR 2D UPDATE ONLY !!!!! <=> beginning of axis_3ddraw (in 2d HERE of course! ) */
/* Copy on update_frame_bounds */
void  sci_update_frame_bounds(int cflag)
{
  double xmax, xmin, ymin, ymax,xmax_tmp, xmin_tmp, ymin_tmp, ymax_tmp;
  double hx,hy,hx1,hy1;
  int Xdec[3],Ydec[3],i;
  double dXdec[3], dYdec[3]; /* double HERE F.Leray 03.06.04 */
  sciPointObj *subwindowtmp; 
  sciSubWindow * ppsubwin ;
  double FRect[4],WRect[4],ARect[4]; 
  char logscale[2];                  

  subwindowtmp = sciGetSelectedSubWin(sciGetCurrentFigure());
  ppsubwin =  pSUBWIN_FEATURE (subwindowtmp); 


  /*****************************************************************
   * get initial bounds
   *****************************************************************/
  if(sciGetZooming(subwindowtmp) == TRUE) {
    xmin= ppsubwin->FRect[0]; 
    ymin= ppsubwin->FRect[1]; 
    xmax= ppsubwin->FRect[2];
    ymax= ppsubwin->FRect[3];
  }
  else {
    xmin = ppsubwin->SRect[0];
    ymin = ppsubwin->SRect[2];
    xmax = ppsubwin->SRect[1];
    ymax = ppsubwin->SRect[3];
  }
 
 
  /*****************************************************************
   * modify  bounds and aaint  if using log scaling X axis
   *****************************************************************/
   if ( ppsubwin->logflags[0]=='l') {
      if ( xmin >  0) {
	xmax=ceil(log10(xmax));  xmin=floor(log10(xmin));
      }
      else {
	Scistring("Warning: Can't use Log on X-axis xmin is negative \n");
	xmax= 1; xmin= 0;
      }
      ppsubwin->axes.aaint[0]=1;
      ppsubwin->axes.aaint[1]=inint(xmax-xmin);
    }

   /*****************************************************************
   * modify  bounds and aaint  if using log scaling Y axis
   *****************************************************************/
   if ( ppsubwin->logflags[1]=='l') {
      if ( ymin > 0 ) {
	ymax= ceil(log10(ymax)); ymin= floor(log10(ymin));
      }
      else {
	Scistring(" Can't use Log on y-axis ymin is negative \n");
	ymax= 1; ymin= 0;
      }
      ppsubwin->axes.aaint[2]=1;
      ppsubwin->axes.aaint[3]=inint(ymax-ymin);
   }
   
   /*****************************************************************
   * modify  bounds to get pretty graduations if required
   *****************************************************************/
  
   ppsubwin->axes.xlim[0]=inint(xmin);
   ppsubwin->axes.xlim[1]=inint(xmax);
   ppsubwin->axes.xlim[2]=0;
   ppsubwin->axes.ylim[0]=inint(ymin);
   ppsubwin->axes.ylim[1]=inint(ymax);
   ppsubwin->axes.ylim[2]=0;

   /* default values in the case where graduate is not called  1/2 */
   /*  Xdec[0]=inint(xmin);Xdec[1]=inint(xmax);Xdec[2]=0; */
   /*    Ydec[0]=inint(ymin);Ydec[1]=inint(ymax);Ydec[2]=0; */

   Xdec[2]=0;
   Ydec[2]=0;
   
   /* I need to know the good power value axes.x/ylim[2] to draw correctly in draw_axis */
   /* This value is correctly computed above: pb. there is a if instruction but I still */
   /* need to know the axes.x/ylim[2] even if I am not in tight_limits == FALSE && 
      isoview == FALSE*/
   /* I DO IT NOW :*/

   if ( ppsubwin->tight_limits == TRUE){
     if(ppsubwin->logflags[0] == 'n'){
       C2F(graduate)(&xmin,&xmax,&xmin_tmp,&xmax_tmp,&(ppsubwin->axes.aaint[0]),&(ppsubwin->axes.aaint[1]),Xdec,Xdec+1,Xdec+2);
       ppsubwin->axes.xlim[2]=Xdec[2];
       dXdec[0] = Fill_XYdec01_TLO_and_ISO_case(Xdec[2], xmin);
       dXdec[1] = Fill_XYdec01_TLO_and_ISO_case(Xdec[2], xmax);
       dXdec[2] = (double) Xdec[2];

     }
     else{
       dXdec[0] = xmin;
       dXdec[1] = xmax;
       dXdec[2] = 0;
     }
   
     if(ppsubwin->logflags[1] == 'n'){
       C2F(graduate)(&ymin,&ymax,&ymin_tmp,&ymax_tmp,&(ppsubwin->axes.aaint[2]),&(ppsubwin->axes.aaint[3]),Ydec,Ydec+1,Ydec+2);
       ppsubwin->axes.ylim[2]=Ydec[2];
       dYdec[0] = Fill_XYdec01_TLO_and_ISO_case(Ydec[2], ymin);
       dYdec[1] = Fill_XYdec01_TLO_and_ISO_case(Ydec[2], ymax);
       dYdec[2] = (double) Ydec[2];
     }
     else{
       dYdec[0] = ymin;
       dYdec[1] = ymax;
       dYdec[2] = 0;
     }
   }

  
   /* default values in the case where graduate is no called  2/2 */
   /*  Xdec[0]=inint(xmin);Xdec[1]=inint(xmax); */
   /*    Ydec[0]=inint(ymin);Ydec[1]=inint(ymax); */
   

   if ( ppsubwin->tight_limits == FALSE) {
     if ( ppsubwin->logflags[0]=='n') { /* x-axis */
       C2F(graduate)(&xmin,&xmax,&xmin_tmp,&xmax_tmp,&(ppsubwin->axes.aaint[0]),&(ppsubwin->axes.aaint[1]),Xdec,Xdec+1,Xdec+2);
       for (i=0; i < 3 ; i++ ) ppsubwin->axes.xlim[i]=Xdec[i];
       xmin=xmin_tmp;xmax=xmax_tmp;
       dXdec[0] = (double) Xdec[0];
       dXdec[1] = (double) Xdec[1];
       dXdec[2] = (double) Xdec[2];
     }
     else{
       dXdec[0] = xmin;
       dXdec[1] = xmax;
       dXdec[2] = 0;
     }

     if ( ppsubwin->logflags[1]=='n') { /* y-axis */
       C2F(graduate)(&ymin,&ymax,&ymin_tmp,&ymax_tmp,&(ppsubwin->axes.aaint[2]),&(ppsubwin->axes.aaint[3]),Ydec,Ydec+1,Ydec+2);
       for (i=0; i < 3 ; i++ ) ppsubwin->axes.ylim[i]=Ydec[i];
       ymin=ymin_tmp;ymax=ymax_tmp;
       dYdec[0] = (double) Ydec[0];
       dYdec[1] = (double) Ydec[1];
       dYdec[2] = (double) Ydec[2];
     }
     else{
       dYdec[0] = ymin;
       dYdec[1] = ymax;
       dYdec[2] = 0;
     }
   }
   
   

   
   /*****************************************************************
    * modify  bounds if  isoview requested 
    *****************************************************************/
   if ( ppsubwin->isoview == TRUE) {      
     int verbose=0,wdim[2],narg; 
     C2F(dr)("xget","wdim",&verbose,wdim,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
     hx=xmax-xmin;
     hy=ymax-ymin;
     getscale2d(WRect,FRect,logscale,ARect);
     
     wdim[0]=linint((double)wdim[0] *WRect[2]);
     wdim[1]=linint((double)wdim[1] *WRect[3]);
     
     if ( hx/(double)wdim[0]  <hy/(double) wdim[1] ) {
       hx1=wdim[0]*hy/wdim[1];
       xmin=xmin-(hx1-hx)/2.0;
       xmax=xmax+(hx1-hx)/2.0;
     }
     else {
       hy1=wdim[1]*hx/wdim[0];
       ymin=ymin-(hy1-hy)/2.0;
       ymax=ymax+(hy1-hy)/2.0;
     }

     dXdec[0] = Fill_XYdec01_TLO_and_ISO_case(Xdec[2], xmin);
     dXdec[1] = Fill_XYdec01_TLO_and_ISO_case(Xdec[2], xmax);
     dYdec[0] = Fill_XYdec01_TLO_and_ISO_case(Ydec[2], ymin);
     dYdec[1] = Fill_XYdec01_TLO_and_ISO_case(Ydec[2], ymax);
   }
   
   /*   Xdec[0]=xmin;Xdec[1]=xmax; */ /* F.Leray 03.06.04 */
/*    Ydec[0]=ymin;Ydec[1]=ymax; */
   
   
   /*****************************************************************
    * set the actual bounds in subwindow data structure
    *****************************************************************/

   ppsubwin->FRect[0]=xmin;
   ppsubwin->FRect[2]=xmax;
   ppsubwin->FRect[1]=ymin;
   ppsubwin->FRect[3]=ymax;

   ppsubwin->axes.xlim[3] = ppsubwin->axes.aaint[1];
   ppsubwin->axes.ylim[3] = ppsubwin->axes.aaint[3];

    /* Faut-il garder la suite ? */

  /* Update the current scale */
  set_scale("tftttf",NULL,ppsubwin->FRect,ppsubwin->axes.aaint,ppsubwin->logflags,NULL); 


  /* Should be added to set_scale */

  /* A voir s'il faut garder en + de update_graduation */
  for (i=0; i < 3 ; i++ ) Cscale.xtics[i] = dXdec[i];
  for (i=0; i < 3 ; i++ ) Cscale.ytics[i] = dYdec[i];
  Cscale.xtics[3] = ppsubwin->axes.aaint[1];
  Cscale.ytics[3] = ppsubwin->axes.aaint[3]; 


  
  for (i=0 ; i<4 ; i++) {  
    ppsubwin->axes.xlim[i]=Cscale.xtics[i]; 
    ppsubwin->axes.ylim[i]=Cscale.ytics[i];
  } 
  

 /*  sciprint("DANS sci_update_framebounds\n"); */
/*   sciprint("Cscale.ytics[0] = %f\n",Cscale.ytics[0]); */
/*   sciprint("Cscale.ytics[1] = %f\n",Cscale.ytics[1]); */
/*   sciprint("Cscale.ytics[2] = %f\n",Cscale.ytics[2]); */
/*   sciprint("Cscale.ytics[3] = %f\n",Cscale.ytics[3]); */
  

}

/**update_3dbounds
 * @author Djalel Abdemouche 10/2003
 * Should be in Plo2dEch.c file
 */
void update_3dbounds(sciPointObj *pobj)
{
  double xmin,xmax,ymin,ymax,zmin,zmax; 
  double lmin,lmax;
  integer min,max,puiss,deux=2,dix=10,n1,n2;
  sciSubWindow * ppsubwin ;
 
  ppsubwin =  pSUBWIN_FEATURE (pobj); 

  /*****************************************************************
   * get initial bounds
   *****************************************************************/

  if(sciGetZooming(pobj) == TRUE) { /* a revoir */
    xmin= ppsubwin->FRect[0]; 
    ymin= ppsubwin->FRect[1]; 
    xmax= ppsubwin->FRect[2];
    ymax= ppsubwin->FRect[3];
    zmin= ppsubwin->FRect[4];
    zmax= ppsubwin->FRect[5];
  }
  else {
    xmin=ppsubwin->SRect[0];
    xmax=ppsubwin->SRect[1];
    ymin=ppsubwin->SRect[2];
    ymax=ppsubwin->SRect[3];
    zmin=ppsubwin->SRect[4];
    zmax=ppsubwin->SRect[5];
  }

  ppsubwin->axes.limits[1]=xmin;
  ppsubwin->axes.limits[3]=xmax;
  ppsubwin->axes.limits[2]=ymin;
  ppsubwin->axes.limits[4]=ymax;
  ppsubwin->axes.limits[5]=zmin;
  ppsubwin->axes.limits[6]=zmax;

  if ( ppsubwin->tight_limits == FALSE) {
    n1=deux;n2=dix;
    C2F(graduate)(&xmin, &xmax,&lmin,&lmax,&n1,&n2,&min,&max,&puiss) ; 
    ppsubwin->axes.xlim[0]=min;
    ppsubwin->axes.xlim[1]=max;
    ppsubwin->axes.xlim[2]=puiss;
    ppsubwin->axes.xlim[3]=n2;
    ppsubwin->FRect[0]=lmin;
    ppsubwin->FRect[2]=lmax;

 
    n1=deux;n2=dix;
    C2F(graduate)(&ymin, &ymax,&lmin,&lmax,&n1,&n2,&min,&max,&puiss) ; 
    ppsubwin->axes.ylim[0]=min;
    ppsubwin->axes.ylim[1]=max;
    ppsubwin->axes.ylim[2]=puiss;
    ppsubwin->axes.ylim[3]=n2;
    ppsubwin->FRect[1]=lmin;
    ppsubwin->FRect[3]=lmax;
      

    n1=deux;n2=dix;
    C2F(graduate)(&zmin, &zmax,&lmin,&lmax,&n1,&n2,&min,&max,&puiss) ; 
    ppsubwin->axes.zlim[0]=min;
    ppsubwin->axes.zlim[1]=max;
    ppsubwin->axes.zlim[2]=puiss;
    ppsubwin->axes.zlim[3]=n2;
    ppsubwin->FRect[4]=lmin;
    ppsubwin->FRect[5]=lmax;}
  else {
   ppsubwin->axes.xlim[0]=inint(xmin);
   ppsubwin->axes.xlim[1]=inint(xmax);
   ppsubwin->axes.xlim[2]=0;
   ppsubwin->axes.xlim[3]=10;
   ppsubwin->FRect[0]=xmin;
   ppsubwin->FRect[2]=xmax;


   ppsubwin->axes.ylim[0]=inint(ymin);
   ppsubwin->axes.ylim[1]=inint(ymax);
   ppsubwin->axes.ylim[2]=0;
   ppsubwin->axes.ylim[3]=10;
   ppsubwin->FRect[1]=ymin;
   ppsubwin->FRect[3]=ymax;

   ppsubwin->axes.zlim[0]=inint(zmin);
   ppsubwin->axes.zlim[1]=inint(zmax);
   ppsubwin->axes.zlim[2]=0;
   ppsubwin->axes.zlim[3]=10;
   ppsubwin->FRect[4]=zmin;
   ppsubwin->FRect[5]=zmax;}

  wininfo("alpha=%.1f,theta=%.1f",ppsubwin->alpha,ppsubwin->theta); 
      
}


/**search
 * @author Djalel Abdemouche 10/2003
 * Should be in Action.c file
 */
double graphic_search(double *id, double *tab1, double *tab2, integer *n)
{
  integer i;

  for (i=0;i<*n;i++)
    if (tab1[i]== *id) return tab2[i];
  return 0.0;
}

/**update_graduation
 * @author Djalel Abdemouche 10/2003
 * Should be in Axes.c file
 */
void update_graduation(sciPointObj *pobj)
{
  integer min,max,puiss,deux=2,dix=10;
  double lmin,lmax;

  if(sciGetEntityType (pobj) == SCI_SUBWIN)
    {	
      C2F(graduate)(pSUBWIN_FEATURE (pobj)->FRect,pSUBWIN_FEATURE (pobj)->FRect+2,
		    &lmin,&lmax,&deux,&dix,&min,&max,&puiss);
      pSUBWIN_FEATURE(pobj)->axes.xlim[0]=min;
      pSUBWIN_FEATURE(pobj)->axes.xlim[1]=max;
      pSUBWIN_FEATURE(pobj)->axes.xlim[2]=puiss; 
      C2F(graduate)(pSUBWIN_FEATURE (pobj)->FRect+1,pSUBWIN_FEATURE (pobj)->FRect+3,
		    &lmin,&lmax,&deux,&dix,&min,&max,&puiss);
      pSUBWIN_FEATURE(pobj)->axes.ylim[0]=min;
      pSUBWIN_FEATURE(pobj)->axes.ylim[1]=max;
      pSUBWIN_FEATURE(pobj)->axes.ylim[2]=puiss;
      C2F(graduate)(pSUBWIN_FEATURE (pobj)->FRect+4,pSUBWIN_FEATURE (pobj)->FRect+5,
		    &lmin,&lmax,&deux,&dix,&min,&max,&puiss);
      pSUBWIN_FEATURE(pobj)->axes.zlim[0]=min;
      pSUBWIN_FEATURE(pobj)->axes.zlim[1]=max;
      pSUBWIN_FEATURE(pobj)->axes.zlim[2]=puiss;
    }

}

int Gen3DPoints(integer type,integer *polyx, integer *polyy, integer *fill, integer whiteid, double zmin, double zmax, double *x, double *y, double *z, integer i, integer j, integer jj1, integer *p, integer dc, integer fg)
{
  sciPointObj *pobj;

  pobj = sciGetSelectedSubWin (sciGetCurrentFigure ()); 
  if (trans3d(pobj ,1, &(polyx[  5*jj1]),&(polyy[  5*jj1]),&(x[i]),&(y[j]),&(z[i+(*p)*j]))==0) return 0; 
  if (trans3d(pobj ,1, &(polyx[1+  5*jj1]),&(polyy[1+  5*jj1]),&(x[i]),&(y[j+1]),&(z[i+(*p)*(j+1)]))==0) return 0; 
  if (trans3d(pobj ,1, &(polyx[2+  5*jj1]),&(polyy[2+  5*jj1]),&(x[i+1]),&(y[j+1]),&(z[(i+1)+(*p)*(j+1)]))==0) return 0; 
  if (trans3d(pobj ,1, &(polyx[3+  5*jj1]),&(polyy[3+  5*jj1]),&(x[i+1]),&(y[j]),&(z[(i+1)+(*p)*j]))==0) return 0;   
  if (trans3d(pobj ,1, &(polyx[4+  5*jj1]),&(polyy[4+  5*jj1]),&(x[i]),&(y[j]),&(z[i+(*p)*j]))==0) return 0; 
  
  if ((((polyx[1+5*jj1]-polyx[0+5*jj1])*(polyy[2+5*jj1]-polyy[0+5*jj1])-
	(polyy[1+5*jj1]-polyy[0+5*jj1])*(polyx[2+5*jj1]-polyx[0+5*jj1])) <  0) && (fg >=0 )) 
    if (type != 0)
      fill[jj1]= (dc < 0 ) ? -fg : fg ;
    else
      fill[jj1]=  (dc != 0 ) ? fg : dc ;
  else
    {
      if (type != 0)
	{
	  fill[jj1]=inint((whiteid-1)*((1/4.0*( z[i+(*p)*j]+ z[i+1+(*p)*j]+
						z[i+(*p)*(j+1)]+ z[i+1+(*p)*(j+1)])-zmin)
				       /(zmax-zmin)))+1;
	  if ( dc < 0 ) fill[jj1]= -fill[jj1];  
	}
      else
	fill[jj1]= dc;     
    }
  
  return(1);
  
}



sciPointObj *sciGetMerge(sciPointObj *psubwin)
{
  sciSons *psonstmp;
  
  psonstmp = sciGetSons (psubwin);
  while (psonstmp != (sciSons *) NULL)	
    {   
      if(sciGetEntityType (psonstmp->pointobj) == SCI_MERGE) 
	return (sciPointObj *) psonstmp->pointobj;
      psonstmp = psonstmp->pnext;
    }
  return (sciPointObj *) NULL;
}

int InitFigureModel()
{ 
  int i, m = NUMCOLORS_SCI;
  /*sciPointObj *pfiguremdl = (sciPointObj *) NULL;*/ /* DJ.A 08/01/04 */

  sciInitGraphicContext (pfiguremdl);
  sciInitGraphicMode (pfiguremdl);
  sciInitFontContext (pfiguremdl);  /* F.Leray 10.06.04 */
  strncpy (pFIGURE_FEATURE (pfiguremdl)->name, "Scilab Graphic", sizeof ("Scilab Graphic") + 4);
  pFIGURE_FEATURE (pfiguremdl)->namelen = Min (sizeof ("Scilab Graphic") + 4, 14); 
  pFIGURE_FEATURE (pfiguremdl)->number=0;
  pFIGURE_FEATURE (pfiguremdl)->figuredimwidth = 610;
  pFIGURE_FEATURE (pfiguremdl)->figuredimheight = 461;
  pFIGURE_FEATURE (pfiguremdl)->inrootposx = 200;
  pFIGURE_FEATURE (pfiguremdl)->inrootposy = 200;
  if((pFIGURE_FEATURE(pfiguremdl)->pcolormap = (double *) MALLOC (m * 3 * sizeof (double))) == (double *) NULL)
    {
      strcpy(error_message,"Cannot init color map");
      return 0;
    }  
  for (i= 0 ; i < m ; i++)
    {
      pFIGURE_FEATURE(pfiguremdl)->pcolormap[i] = (double) (defcolors[3*i]/255.0);
      pFIGURE_FEATURE(pfiguremdl)->pcolormap[i+m] = (double) (defcolors[3*i+1]/255.0); 
      pFIGURE_FEATURE(pfiguremdl)->pcolormap[i+2*m] = (double) (defcolors[3*i+2]/255.0);
    }
  pFIGURE_FEATURE (pfiguremdl)->numcolors = m;
  pFIGURE_FEATURE (pfiguremdl)->isiconified = FALSE;
  pFIGURE_FEATURE (pfiguremdl)->isselected = TRUE;
  pFIGURE_FEATURE (pfiguremdl)->rotstyle = 0;
  pFIGURE_FEATURE (pfiguremdl)->visible = TRUE;
  pFIGURE_FEATURE (pfiguremdl)->numsubwinselected = 0; 
  pFIGURE_FEATURE (pfiguremdl)->pixmap = 0; 
  pFIGURE_FEATURE (pfiguremdl)->wshow = 0;
  return 1;
}


int InitAxesModel()
{ 
  char dir;
  int i;
  sciPointObj * pobj = NULL;
  sciSubWindow * ppobj = NULL;
  
  sciInitGraphicContext (paxesmdl);
  sciInitGraphicMode (paxesmdl);
  sciInitFontContext (paxesmdl);  /* F.Leray 10.06.04 */
  pSUBWIN_FEATURE (paxesmdl)->cube_scaling = FALSE;
  pSUBWIN_FEATURE (paxesmdl)->callback = (char *)NULL;
  pSUBWIN_FEATURE (paxesmdl)->callbacklen = 0;
  pSUBWIN_FEATURE (paxesmdl)->callbackevent = 100;  
  pSUBWIN_FEATURE (paxesmdl)->logflags[0] = 'n';
  pSUBWIN_FEATURE (paxesmdl)->logflags[1] = 'n';
  pSUBWIN_FEATURE (paxesmdl)->axes.ticscolor  = -1;
  pSUBWIN_FEATURE (paxesmdl)->axes.subint[0]  = 1;   
  pSUBWIN_FEATURE (paxesmdl)->axes.subint[1]  = 1; 
  pSUBWIN_FEATURE (paxesmdl)->axes.subint[2]  = 1;
  pSUBWIN_FEATURE (paxesmdl)->axes.xdir='d'; 
  pSUBWIN_FEATURE (paxesmdl)->axes.ydir='l';  
  pSUBWIN_FEATURE (paxesmdl)->axes.rect  = 1;

  /* F.Leray 26.04.04 : Pb init axes.aaint DONE HERE: default values */
  pSUBWIN_FEATURE (paxesmdl)->axes.aaint[0] = 2;
  pSUBWIN_FEATURE (paxesmdl)->axes.aaint[1] = 10;
  pSUBWIN_FEATURE (paxesmdl)->axes.aaint[2] = 2;
  pSUBWIN_FEATURE (paxesmdl)->axes.aaint[3] = 10;
  
  for (i=0 ; i<7 ; i++)
    pSUBWIN_FEATURE (paxesmdl)->axes.limits[i]  = 0;

  for (i=0 ; i<3 ; i++)
    pSUBWIN_FEATURE (paxesmdl)->grid[i]  = -1;
  pSUBWIN_FEATURE (paxesmdl)->isaxes  = FALSE;
  pSUBWIN_FEATURE (paxesmdl)->alpha  = 0.0;
  pSUBWIN_FEATURE (paxesmdl)->theta  = 270.0;
  pSUBWIN_FEATURE (paxesmdl)->alpha_kp  = 45.0;
  pSUBWIN_FEATURE (paxesmdl)->theta_kp  = 215.0;
  pSUBWIN_FEATURE (paxesmdl)->is3d  = FALSE;  
  dir= 'd'; pSUBWIN_FEATURE (paxesmdl)->axes.xdir=dir;
  dir= 'l'; pSUBWIN_FEATURE (paxesmdl)->axes.ydir=dir;      
  for (i=0 ; i<4 ; i++)
    {  
      pSUBWIN_FEATURE (paxesmdl)->axes.xlim[i]= Cscale.xtics[i]; 
      pSUBWIN_FEATURE (paxesmdl)->axes.ylim[i]= Cscale.ytics[i]; 
    }
  pSUBWIN_FEATURE (paxesmdl)->axes.zlim[0]= -1.0;
  pSUBWIN_FEATURE (paxesmdl)->axes.zlim[1]= 1.0;  
  pSUBWIN_FEATURE (paxesmdl)->axes.flag[0]= 2;
  pSUBWIN_FEATURE (paxesmdl)->axes.flag[1]= 2;
  pSUBWIN_FEATURE (paxesmdl)->axes.flag[2]= 4; 
  pSUBWIN_FEATURE (paxesmdl)->project[0]= 1;
  pSUBWIN_FEATURE (paxesmdl)->project[1]= 1;
  pSUBWIN_FEATURE (paxesmdl)->project[2]= 0;
  pSUBWIN_FEATURE (paxesmdl)->hiddencolor=4;
  pSUBWIN_FEATURE (paxesmdl)->hiddenstate=0; 
   pSUBWIN_FEATURE (paxesmdl)->isoview= FALSE;/*TRUE;*/
 
  pSUBWIN_FEATURE (paxesmdl)->facetmerge = FALSE; 
  pSUBWIN_FEATURE (paxesmdl)->WRect[0]   = 0;
  pSUBWIN_FEATURE (paxesmdl)->WRect[1]   = 0;
  pSUBWIN_FEATURE (paxesmdl)->WRect[2]   = 1;
  pSUBWIN_FEATURE (paxesmdl)->WRect[3]   = 1;  
  pSUBWIN_FEATURE (paxesmdl)->FRect[0]   = 0.0; /* xmin */
  pSUBWIN_FEATURE (paxesmdl)->FRect[1]   = 0.0; /* ymin */
  pSUBWIN_FEATURE (paxesmdl)->FRect[2]   = 1.0; /* xmax */
  pSUBWIN_FEATURE (paxesmdl)->FRect[3]   = 1.0; /* ymax */
  pSUBWIN_FEATURE (paxesmdl)->FRect[4]   = -1.0;/* zmin */
  pSUBWIN_FEATURE (paxesmdl)->FRect[5]   = 1.0;   /* zmax */
  pSUBWIN_FEATURE (paxesmdl)->isselected = FALSE;
  pSUBWIN_FEATURE (paxesmdl)->visible = sciGetVisibility(pfiguremdl); 
  pSUBWIN_FEATURE (paxesmdl)->isclip = -1;  
  pSUBWIN_FEATURE (paxesmdl)->pPopMenu = (sciPointObj *)NULL;

  /* Les SRect sont rentrés dans l'ordre: 
     [xmin ymin zmin xmax ymax zmax] */
  pSUBWIN_FEATURE (paxesmdl)->SRect[0]   = 0.0; /* xmin */
  pSUBWIN_FEATURE (paxesmdl)->SRect[1]   = 1.0; /* xmax */
  pSUBWIN_FEATURE (paxesmdl)->SRect[2]   = 0.0; /* ymin */
  pSUBWIN_FEATURE (paxesmdl)->SRect[3]   = 1.0; /* ymax */
  pSUBWIN_FEATURE (paxesmdl)->SRect[4]   = -1.0;/* zmin */
  pSUBWIN_FEATURE (paxesmdl)->SRect[5]   = 1.0;  /* zmax */
  
  pSUBWIN_FEATURE (paxesmdl)->tight_limits = FALSE;

  /* F.Leray 10.06.04 */
  /* Adding default Labels inside Axes */
  /*------------------------------------------------------------------------------------*/
 
  pobj = paxesmdl;
  ppobj = pSUBWIN_FEATURE(paxesmdl);
  
  /******************************  title *************************/
  
  if ((ppobj->mon_title = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_title, SCI_LABEL);
  if (((ppobj->mon_title)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_title));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_title)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
    }
  
  if (!(sciAddThisToItsParent ((ppobj->mon_title), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_title));
      FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
    }
      
  sciSetCurrentSon ((ppobj->mon_title), (sciPointObj *) NULL);
      
  pLABEL_FEATURE ((ppobj->mon_title))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_title))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_title))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_title))->text.callbacklen = 0; 
 /*  pLABEL_FEATURE ((ppobj->mon_title))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_title)));  */
  pLABEL_FEATURE ((ppobj->mon_title))->visible = sciGetVisibility(paxesmdl);

  if ((pLABEL_FEATURE ((ppobj->mon_title))->text.ptextstring =calloc (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_title), sciGetParent ((ppobj->mon_title)));
      sciDelHandle ((ppobj->mon_title));
      FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
    }
  /* init a "" pour le chanmp title */
  strcpy (pLABEL_FEATURE ((ppobj->mon_title))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_title))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_title))->ptype = 1; /* type = 1 <=> title, 2 <=> x_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_title))->text.fontcontext.textorientation = 0; 

  /*   pLABEL_FEATURE ((ppobj->mon_title))->titleplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_title))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_title)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_title))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_title), sciGetParent ((ppobj->mon_title)));
      sciDelHandle ((ppobj->mon_title));
      FREE(pLABEL_FEATURE((ppobj->mon_title)));
      FREE((ppobj->mon_title));
      return -1;
    }


      
  /******************************  x_label *************************/
  
  if ((ppobj->mon_x_label = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_x_label, SCI_LABEL);
  if (((ppobj->mon_x_label)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_x_label));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_x_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }
    
  if (!(sciAddThisToItsParent ((ppobj->mon_x_label), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_x_label));
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }
      
  sciSetCurrentSon ((ppobj->mon_x_label), (sciPointObj *) NULL);
      
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_x_label))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.callbacklen = 0; 
 /*  pLABEL_FEATURE ((ppobj->mon_x_label))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_x_label)));  */
  pLABEL_FEATURE ((ppobj->mon_x_label))->visible = sciGetVisibility(paxesmdl);
      

  if ((pLABEL_FEATURE ((ppobj->mon_x_label))->text.ptextstring =calloc (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_x_label), sciGetParent ((ppobj->mon_x_label)));
      sciDelHandle ((ppobj->mon_x_label));
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }
  /* init a "" pour le chanmp x_label */
  strcpy (pLABEL_FEATURE ((ppobj->mon_x_label))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_x_label))->ptype = 2; /* type = 1 <=> x_label, 2 <=> x_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_x_label))->text.fontcontext.textorientation = 0; 

  /*   pLABEL_FEATURE ((ppobj->mon_x_label))->x_labelplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_x_label))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_x_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_x_label))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_x_label), sciGetParent ((ppobj->mon_x_label)));
      sciDelHandle ((ppobj->mon_x_label));
      FREE(pLABEL_FEATURE((ppobj->mon_x_label)));
      FREE((ppobj->mon_x_label));
      return -1;
    }


  
  /******************************  y_label *************************/
  
  if ((ppobj->mon_y_label = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_y_label, SCI_LABEL);
  if (((ppobj->mon_y_label)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_y_label));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_y_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }
    
  if (!(sciAddThisToItsParent ((ppobj->mon_y_label), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_y_label));
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }
      
  sciSetCurrentSon ((ppobj->mon_y_label), (sciPointObj *) NULL);
      
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_y_label))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.callbacklen = 0; 
 /*  pLABEL_FEATURE ((ppobj->mon_y_label))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_y_label)));  */
  pLABEL_FEATURE ((ppobj->mon_y_label))->visible = sciGetVisibility(paxesmdl);
    

  if ((pLABEL_FEATURE ((ppobj->mon_y_label))->text.ptextstring =calloc (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_y_label), sciGetParent ((ppobj->mon_y_label)));
      sciDelHandle ((ppobj->mon_y_label));
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }
  /* init a "" pour le chanmp y_label */
  strcpy (pLABEL_FEATURE ((ppobj->mon_y_label))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_y_label))->ptype = 3; /* type = 1 <=> y_label, 2 <=> y_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_y_label))->text.fontcontext.textorientation = 0; 

  /*   pLABEL_FEATURE ((ppobj->mon_y_label))->y_labelplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_y_label))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_y_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_y_label))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_y_label), sciGetParent ((ppobj->mon_y_label)));
      sciDelHandle ((ppobj->mon_y_label));
      FREE(pLABEL_FEATURE((ppobj->mon_y_label)));
      FREE((ppobj->mon_y_label));
      return -1;
    }

  
  /******************************  z_label *************************/
  
  if ((ppobj->mon_z_label = MALLOC (sizeof (sciPointObj))) == NULL)
    return -1;
  sciSetEntityType (ppobj->mon_z_label, SCI_LABEL);
  if (((ppobj->mon_z_label)->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE((ppobj->mon_z_label));
      return -1;
    }
  
  
  if (sciAddNewHandle ((ppobj->mon_z_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }
    
  if (!(sciAddThisToItsParent ((ppobj->mon_z_label), paxesmdl)))
    {
      sciDelHandle ((ppobj->mon_z_label));
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }
      
  sciSetCurrentSon ((ppobj->mon_z_label), (sciPointObj *) NULL);
      
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.relationship.psons = (sciSons *) NULL;
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.relationship.plastsons = (sciSons *) NULL;

  pLABEL_FEATURE ((ppobj->mon_z_label))->text.callback = (char *)NULL;
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.callbacklen = 0; 
 /*  pLABEL_FEATURE ((ppobj->mon_z_label))->visible = sciGetVisibility(sciGetParentFigure((ppobj->mon_z_label)));  */
  pLABEL_FEATURE ((ppobj->mon_z_label))->visible = sciGetVisibility(paxesmdl);
      

  if ((pLABEL_FEATURE ((ppobj->mon_z_label))->text.ptextstring =calloc (1, sizeof (char))) == NULL)
    {
      sciprint("No more place to allocates text string, try a shorter string");
      sciDelThisToItsParent ((ppobj->mon_z_label), sciGetParent ((ppobj->mon_z_label)));
      sciDelHandle ((ppobj->mon_z_label));
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }
  /* init a "" pour le chanmp z_label */
  strcpy (pLABEL_FEATURE ((ppobj->mon_z_label))->text.ptextstring, "");
      
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.textlen = 0;
  pLABEL_FEATURE ((ppobj->mon_z_label))->ptype = 4; /* type = 1 <=> z_label, 2 <=> z_label...*/
      
  pLABEL_FEATURE ((ppobj->mon_z_label))->text.fontcontext.textorientation = 0; 

  /*   pLABEL_FEATURE ((ppobj->mon_z_label))->z_labelplace = SCI_LABEL_IN_TOP; */
  pLABEL_FEATURE ((ppobj->mon_z_label))->isselected = TRUE;
  if (sciInitFontContext ((ppobj->mon_z_label)) == -1)
    {
      FREE(pLABEL_FEATURE((ppobj->mon_z_label))->text.ptextstring);
      sciDelThisToItsParent ((ppobj->mon_z_label), sciGetParent ((ppobj->mon_z_label)));
      sciDelHandle ((ppobj->mon_z_label));
      FREE(pLABEL_FEATURE((ppobj->mon_z_label)));
      FREE((ppobj->mon_z_label));
      return -1;
    }

  return 1; 
}

/* F.Leray 07.05.04 */
/* This routine rebuild a needed strflag for axis_draw call in sciDrawObj*/
/* It may be less complete than the original strflag given in plot2dn (or other 2D - plot function)
   due to less info. Nevertheless this new strflag is sufficient for axis_draw because
  tests are on strflag[1] (case =='5' or '6') and strflag[2] */
void rebuild_strflag( sciPointObj * psubwin, char * STRFLAG)
{

  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  /* strflag[0]*/
  STRFLAG[0] = '0'; /* flag for caption display unused here so set to NULL by default */
  if( pSUBWIN_FEATURE (psubwin)->with_leg == 1)
    STRFLAG[0] = '0';
  else
    STRFLAG[0] = '1';

  /* strflag[1]*/
  /* Here isoview mode test is not needed because axis_draw do not use it */
  STRFLAG[1] = '0'; /* Init. to NULL <=> use the previous scale */
  if(ppsubwin->tight_limits == TRUE)
    {
      STRFLAG[1] = '8';
    }
  else /* using auto rescale : enlarged for pretty axes*/
    {
      STRFLAG[1] = '6';
    }
  
  /* strflag[2]*/
  STRFLAG[2] = '1'; /* Init with y-axis on the left AND axes is on*/
  if(ppsubwin->isaxes == TRUE)
    {
      if(ppsubwin->axes.ydir =='l')
	STRFLAG[2] = '1';
      else if(ppsubwin->axes.ydir =='r')
	STRFLAG[2] = '3';
      else if((ppsubwin->axes.xdir =='c') && (ppsubwin->axes.ydir =='c'))
	STRFLAG[2] = '5';
    }
  else
    {
      STRFLAG[2] = '0';
    }

  STRFLAG[3] = '\0';
}


sciPointObj *
ConstructMerge (sciPointObj * pparentsubwin,int N,int *index_in_entity,long *from_entity)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
 
  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL) return (sciPointObj *) NULL;
  
      sciSetEntityType (pobj, SCI_MERGE);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciMerge)))) == NULL) {
	FREE(pobj);
	return (sciPointObj *) NULL;
      }
      if (sciAddNewHandle (pobj) == -1) {
	FREE(pMERGE_FEATURE (pobj));
	FREE(pobj);
	return (sciPointObj *) NULL;
      }
      if (!(sciAddThisToItsParent (pobj, pparentsubwin))){
	sciDelHandle (pobj);
	FREE(pMERGE_FEATURE (pobj));
	FREE(pobj);
	return (sciPointObj *) NULL;
      }

      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      pMERGE_FEATURE (pobj)->relationship.psons = (sciSons *) NULL;
      pMERGE_FEATURE (pobj)->relationship.plastsons = (sciSons *) NULL;
      pMERGE_FEATURE (pobj)->index_in_entity=index_in_entity;
      pMERGE_FEATURE (pobj)->from_entity=from_entity;
      pMERGE_FEATURE (pobj)->N=N;
      return pobj;
    }
  else
    return (sciPointObj *) NULL;
}

int DestroyMerge (sciPointObj * pthis)
{
  pSUBWIN_FEATURE (sciGetParentSubwin(pthis))->facetmerge = FALSE;
  FREE(pMERGE_FEATURE (pthis)->index_in_entity);
  FREE(pMERGE_FEATURE (pthis)->from_entity);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  return 0;
}


int  Merge3dDimension(sciPointObj *pparent)
{
  integer N,q; 
  sciSons *psonstmp;

  /* ========================================================================
   * Compute the number of facets, segments,... included in all the subwin 
   * children
   * Each entities to merge; is decomposed in a set of basic elements 
   *  (facet, segment, point,...)
   * Each basic element is represented in the Merge structure by the handle of its entity and an 
   *   index within this entity
   * ========================================================================*/

  q=0;
  psonstmp = sciGetSons (pparent);
  while (psonstmp != (sciSons *) NULL) {   
    switch (sciGetEntityType (psonstmp->pointobj)) {  
    case SCI_SURFACE:
      if (pSURFACE_FEATURE (psonstmp->pointobj)->typeof3d == SCI_PLOT3D) 
	N=(pSURFACE_FEATURE (psonstmp->pointobj)->dimzx-1)*(pSURFACE_FEATURE (psonstmp->pointobj)->dimzy-1);
      else
	N = pSURFACE_FEATURE (psonstmp->pointobj)->dimzy;
      break;

    case  SCI_POLYLINE:
      if (pPOLYLINE_FEATURE (psonstmp->pointobj)->plot != 5) {/*polyline*/
	N = pPOLYLINE_FEATURE (psonstmp->pointobj)->n1-1;
	if ((pPOLYLINE_FEATURE (psonstmp->pointobj)->plot != 2) && 
	    (sciGetIsMark((sciPointObj *)psonstmp->pointobj) == 1)) N=N+1;
      }
      else/* patch */
	N = 1; 
      break;
    case  SCI_SEGS: 
      N=pSEGS_FEATURE (psonstmp->pointobj)->Nbr1/2;
      break;
    case  SCI_RECTANGLE: 
      N = 4;
      break;
    case SCI_AGREG:
      N =  Merge3dDimension(psonstmp->pointobj);
      break;
    default:
      N=0;
    }
    q+=N;
    psonstmp = psonstmp->pnext;
  }
  return q;
}



void Merge3dBuildTable(sciPointObj *pparent, int *index_in_entity, long *from_entity, int *pos)
{
  sciSons *psonstmp;
  int i,N;

  psonstmp = sciGetSons (pparent);

  while (psonstmp != (sciSons *) NULL) {   
    switch (sciGetEntityType (psonstmp->pointobj)) {  
    case SCI_SURFACE:
      if (pSURFACE_FEATURE (psonstmp->pointobj)->typeof3d == SCI_PLOT3D) 
	N=(pSURFACE_FEATURE (psonstmp->pointobj)->dimzx-1)*(pSURFACE_FEATURE (psonstmp->pointobj)->dimzy-1);
      else
	N = pSURFACE_FEATURE (psonstmp->pointobj)->dimzy;
      break;
    case  SCI_POLYLINE:
      if (pPOLYLINE_FEATURE (psonstmp->pointobj)->plot != 5) {/*polyline*/
	N = pPOLYLINE_FEATURE (psonstmp->pointobj)->n1-1;
	if ((pPOLYLINE_FEATURE (psonstmp->pointobj)->plot != 2) && 
	    (sciGetIsMark((sciPointObj *)psonstmp->pointobj) == 1)) N=N+1;
      }
      else/* patch */
	N = 1; 
      break;
    case  SCI_SEGS: 
      N=pSEGS_FEATURE (psonstmp->pointobj)->Nbr1/2;
      break;
    case  SCI_RECTANGLE: 
      N = 4;
      break;
    case SCI_AGREG:
      Merge3dBuildTable(psonstmp->pointobj, index_in_entity, from_entity, pos);
      break;
    default:
      N = 0;
    }
    if (sciGetEntityType (psonstmp->pointobj) != SCI_AGREG)
      for (i=0 ; i<N; i++) {
	index_in_entity[*pos]=i;
	from_entity[*pos]=(long) sciGetHandle (psonstmp->pointobj);
	*pos=*pos+1;
      }
    psonstmp = psonstmp->pnext;
  }
}


void Merge3d(sciPointObj *psubwin)
{
  integer q,k; 
  sciPointObj *pmerge; 
  int *index_in_entity;
  long *from_entity;
  
  if(sciGetEntityType (psubwin) != SCI_SUBWIN) return; 
  if ((pmerge= sciGetMerge(psubwin)) != (sciPointObj *) NULL)
    DestroyMerge(pmerge); 
  
  /* ========================================================================
   * Compute the number of facets, segments,... included in all the subwin 
   * children
   * ========================================================================*/
  
  
  q =  Merge3dDimension(psubwin);
  
  
  /* ========================================================================
   * allocate tables for index and handles
   * ========================================================================*/
  
  /* q now contains the total number of elements */
  if ((index_in_entity = (int *) malloc (q * sizeof (int))) == (int *)NULL) {
    Scistring("Merge3d : not enough memory to allocate \n");
    return;
  }
  if ((from_entity   = (long *) malloc (q * sizeof (long))) == (long *) NULL) {
    Scistring("Merge3d : not enough memory to allocate \n");
    free(index_in_entity);
  }
  
  /* ========================================================================
   * fill the index and handles tables
   * ========================================================================*/
  k=0;
  Merge3dBuildTable(psubwin, index_in_entity, from_entity, &k);
  
  /* ========================================================================
   * create the Merge data structure
   * ========================================================================*/
  
  if ((pmerge=ConstructMerge ((sciPointObj *) psubwin,q,index_in_entity,from_entity)) == (sciPointObj *) NULL) {
    free(index_in_entity);
    free(from_entity);
    sciprint ("\r\n No merge supported");}
  else /* inform the subwindow to display Merge instead of individual children */
    pSUBWIN_FEATURE (psubwin)->facetmerge = TRUE;
  
}

void DrawMerge3d(sciPointObj *psubwin, sciPointObj *pmerge)
{
  int N,i,j,index,p,max_p,n1,npoly;
  double * dist;
  double X[4],Y[4],Z[4];
  double *x,*y,*z;
  sciPointObj *pobj; 
  int *locindex;
  int *polyx,*polyy,fill[20];/* here we suppose there is no more than 20 edge in a facet */
  int k1, pstyle,iflag;
  int whiteid,verbose=0,narg;
  static double zmin,zmax,xmoy,ymoy,zmoy,zl;
  int context[6];
#ifdef WIN32 
  int hdcflag;
#endif

  if(sciGetEntityType (psubwin) != SCI_SUBWIN) return;
  N=pMERGE_FEATURE (pmerge)->N; /* total number of elements */

  if ((dist=(double *)MALLOC(N*sizeof(double)))==(double *) NULL) {
    Scistring("DrawMerge3d : malloc No more Place\n");
    return;
  }
  if ((locindex=(int *)MALLOC(N*sizeof(int)))==(int *) NULL) {
    Scistring("DrawMerge3d : malloc No more Place\n");
    return;
  }

  /* ========================================================================
   * compute drawing order (painter algorithm) *
   may be moved into Merge3d
   * ========================================================================*/
  max_p=0; /* the maximum number of edge in a facet */
  for ( i =0 ; i < N ; i++) { /* loop on element*/
    pobj=(sciPointObj *) sciGetPointerFromHandle (pMERGE_FEATURE (pmerge)->from_entity[i]);
    index=pMERGE_FEATURE (pmerge)->index_in_entity[i];

    /*compute element coordinates */
    switch (sciGetEntityType (pobj)) {  
    case SCI_SURFACE:
      if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D) { /* x,y,Z */
	int l,k,n1,n2;
	n1= pSURFACE_FEATURE (pobj)->dimzx;
	n2= pSURFACE_FEATURE (pobj)->dimzy;
	l=(int)(index/(n1-1));
	k=index-l*(n1-1);

	n2= pSURFACE_FEATURE (pobj)->dimzy;
	X[0]=X[1]=pSURFACE_FEATURE (pobj)->pvecx[k];
	X[2]=X[3]=pSURFACE_FEATURE (pobj)->pvecx[k+1];
	Z[0]=pSURFACE_FEATURE (pobj)->pvecz[k+l*n1];
	Z[1]=pSURFACE_FEATURE (pobj)->pvecz[k+l*n1+n1];
	Z[2]=pSURFACE_FEATURE (pobj)->pvecz[k+l*n1+n1+1];
	Z[3]=pSURFACE_FEATURE (pobj)->pvecz[k+l*n1+1];
	Y[0]=Y[3]=pSURFACE_FEATURE (pobj)->pvecy[l];
	Y[1]=Y[2]=pSURFACE_FEATURE (pobj)->pvecy[l+1];
	p=4;
	x=X;y=Y;z=Z;
      }
      else{ /* facets */
	p=pSURFACE_FEATURE (pobj)->dimzx;
	x=&(pSURFACE_FEATURE (pobj)->pvecx[index*p]);
	y=&(pSURFACE_FEATURE (pobj)->pvecy[index*p]);
	z=&(pSURFACE_FEATURE (pobj)->pvecz[index*p]);
      }
      break;
    case  SCI_POLYLINE:
      n1= pPOLYLINE_FEATURE (pobj)->n1;
      p=2;
      if (sciGetIsMark((sciPointObj *)pobj) == 1) p=1;
      switch (pPOLYLINE_FEATURE (pobj)->plot) {
      case 0: case 1: case 4: /*linear interpolation */
	x=&(pPOLYLINE_FEATURE (pobj)->pvx[index]);
	y=&(pPOLYLINE_FEATURE (pobj)->pvy[index]);
	if (pPOLYLINE_FEATURE (pobj)->pvz != (double *) NULL) 
	  z=&(pPOLYLINE_FEATURE (pobj)->pvz[index]);
	else
	  z=(double *)NULL;
	break;
      case 2: /* staircase */ /* just for completion  */
	p=2;
	X[0]=pPOLYLINE_FEATURE (pobj)->pvx[index];
	X[1]=pPOLYLINE_FEATURE (pobj)->pvx[index+1];
	Y[0]=pPOLYLINE_FEATURE (pobj)->pvy[index];
	Y[1]=pPOLYLINE_FEATURE (pobj)->pvy[index];
	if (pPOLYLINE_FEATURE (pobj)->pvz != (double *) NULL) {
	  Z[0]=pPOLYLINE_FEATURE (pobj)->pvz[index];
	  Z[1]=pPOLYLINE_FEATURE (pobj)->pvz[index];
	  z=Z;
	}
	else 
	  z=(double *)NULL;
	x=X;y=Y;
	break;
      case 3 : /* vertical bar */ /* just for completion  */
	X[0]=pPOLYLINE_FEATURE (pobj)->pvx[index];
	X[1]=pPOLYLINE_FEATURE (pobj)->pvx[index];
	Y[0]=0.0;
	Y[1]=pPOLYLINE_FEATURE (pobj)->pvy[index];
	if (pPOLYLINE_FEATURE (pobj)->pvz != (double *) NULL) {
	  Z[0]=pPOLYLINE_FEATURE (pobj)->pvz[index];
	  Z[1]=pPOLYLINE_FEATURE (pobj)->pvz[index];
	  z=Z;
	}
	else 
	  z=(double *)NULL;
	x=X;y=Y;
	break;
      case 5: /* patch*/
	x=pPOLYLINE_FEATURE (pobj)->pvx;
	y=pPOLYLINE_FEATURE (pobj)->pvy;
	z=pPOLYLINE_FEATURE (pobj)->pvz;
	p=n1;
	break;
      }
      break;
    case  SCI_SEGS: 
      p = 2;
      X[0]=pSEGS_FEATURE (pobj)->vx[2*index];
      X[1]=pSEGS_FEATURE (pobj)->vx[2*index+1];
      Y[0]=pSEGS_FEATURE (pobj)->vy[2*index];
      Y[1]=pSEGS_FEATURE (pobj)->vy[2*index+1];
      if (pSEGS_FEATURE (pobj)->vz != (double *) NULL) {
	Z[0]=pSEGS_FEATURE (pobj)->vz[2*index];
	Z[1]=pSEGS_FEATURE (pobj)->vz[2*index+1];
	z=Z;
      }
      else
	z=(double *)NULL;
      x=X;y=Y;
      break;
    case  SCI_RECTANGLE: 
      p = 5;
      pstyle=0; /* arevoir */
      iflag=0; /* arevoir */
      X[0]=X[1]=X[4]=pRECTANGLE_FEATURE (pobj)->x;
      Y[0]=Y[3]=Y[4]=pRECTANGLE_FEATURE (pobj)->y;
      X[2]=X[3]=pRECTANGLE_FEATURE (pobj)->x + pRECTANGLE_FEATURE (pobj)->width;
      Y[1]=Y[2]=pRECTANGLE_FEATURE (pobj)->y - pRECTANGLE_FEATURE (pobj)->height;
      Z[0]=Z[1]=Z[2]=Z[3]=Z[4]=pRECTANGLE_FEATURE (pobj)->z;
      x=X;y=Y;z=Z; 
      break;
    default:
      p = 0;
    }
    /* each element is represented by its barycenter */
    xmoy=0.0;ymoy=0.0;zmoy=0.0;
    if (z != (double *)NULL) {
      for ( j= 0 ; j < p ; j++) {
	xmoy += x[j];  ymoy += x[j];  zmoy += z[j];
      }
    }
    else {
      for ( j= 0 ; j < p ; j++) {
	xmoy += x[j];  ymoy += x[j];
      }
    }
    /* Compute the distance from the observer */
    dist[i]=  TRZ(xmoy/p,ymoy/p,zmoy/p);
    max_p=Max(max_p,p);
  }

  /* sort the distance in decreasing order */
  C2F(dsort)(dist,&N,locindex); 

  /* ========================================================================
   * draw each element in the order given by locindex
   * ========================================================================*/
  C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  zmin=  pSUBWIN_FEATURE (psubwin)->FRect[4];
  zmax= pSUBWIN_FEATURE (psubwin)->FRect[5];
  if ((polyx=(int *)MALLOC((max_p+1)*sizeof(int)))==(int *) NULL) {
    FREE(dist);FREE(locindex);
    Scistring("DrawMerge3d : malloc No more Place\n");

    return;
  }
  if ((polyy=(int *)MALLOC((max_p+1)*sizeof(int)))==(int *) NULL) {
    FREE(dist);FREE(locindex);
    Scistring("DrawMerge3d : malloc No more Place\n");
    return;
  }
  npoly=1; 
  for ( i = N ; i>0 ; i--) { /* loop on elements */
    int j,nok=0;
    j=locindex[i-1]-1;
    index=pMERGE_FEATURE (pmerge)->index_in_entity[j];
    pobj=(sciPointObj *) sciGetPointerFromHandle (pMERGE_FEATURE (pmerge)->from_entity[j]);
    if (sciGetVisibility (pobj)) {
      
      /* build the element coordinates */
      switch (sciGetEntityType (pobj)) {  
      case SCI_SURFACE:
	if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D) { /* x,y,Z */
	  int l,k,n1,n2;
	  n1= pSURFACE_FEATURE (pobj)->dimzx;
	  l=(int)(index/(n1-1));
	  k=index-l*(n1-1);
	  n2= pSURFACE_FEATURE (pobj)->dimzy;
	  X[0]=X[1]=pSURFACE_FEATURE (pobj)->pvecx[k];
	  X[2]=X[3]=pSURFACE_FEATURE (pobj)->pvecx[k+1];
	  Z[0]=pSURFACE_FEATURE (pobj)->pvecz[k+l*n1];
	  Z[1]=pSURFACE_FEATURE (pobj)->pvecz[k+l*n1+n1];
	  Z[2]=pSURFACE_FEATURE (pobj)->pvecz[k+l*n1+n1+1];
	  Z[3]=pSURFACE_FEATURE (pobj)->pvecz[k+l*n1+1];
	  Y[0]=Y[3]=pSURFACE_FEATURE (pobj)->pvecy[l];
	  Y[1]=Y[2]=pSURFACE_FEATURE (pobj)->pvecy[l+1];
	  p=4;
	  x=X;y=Y;z=Z;
 	}
	else{ /* facets */
	  p=pSURFACE_FEATURE (pobj)->dimzx;
	  x=&(pSURFACE_FEATURE (pobj)->pvecx[index*p]);
	  y=&(pSURFACE_FEATURE (pobj)->pvecy[index*p]);
	  z=&(pSURFACE_FEATURE (pobj)->pvecz[index*p]);
	}
	break;
      case  SCI_POLYLINE:
	n1= pPOLYLINE_FEATURE (pobj)->n1;
	p=2;
	if (sciGetIsMark((sciPointObj *)pobj) == 1) p=1;
	pstyle=0; /* arevoir */
	iflag=0; /* arevoir */
	switch (pPOLYLINE_FEATURE (pobj)->plot) {
	case 0: case 1: case 4: /*linear interpolation */
	  x=&(pPOLYLINE_FEATURE (pobj)->pvx[index]);
	  y=&(pPOLYLINE_FEATURE (pobj)->pvy[index]);
	  if (pPOLYLINE_FEATURE (pobj)->pvz != (double *) NULL) 
	    z=&(pPOLYLINE_FEATURE (pobj)->pvz[index]);
	  else
	    z=(double *)NULL;
	  break;
	case 2: /* staircase */ /* just for completion  */
	  X[0]=pPOLYLINE_FEATURE (pobj)->pvx[index];
	  X[1]=pPOLYLINE_FEATURE (pobj)->pvx[index+1];
	  Y[0]=pPOLYLINE_FEATURE (pobj)->pvy[index];
	  Y[1]=pPOLYLINE_FEATURE (pobj)->pvy[index];
	  if (pPOLYLINE_FEATURE (pobj)->pvz != (double *) NULL) {
	    Z[0]=pPOLYLINE_FEATURE (pobj)->pvz[index];
	    Z[1]=pPOLYLINE_FEATURE (pobj)->pvz[index];
	    z=Z;
	  }
	  else 
	    z=(double *) NULL;
	  x=X;y=Y;z=Z;
	  break;
	case 3 : /* vertical bar */ /* just for completion  */
	  X[0]=pPOLYLINE_FEATURE (pobj)->pvx[index];
	  X[1]=pPOLYLINE_FEATURE (pobj)->pvx[index];
	  Y[0]=0.0;
	  Y[1]=pPOLYLINE_FEATURE (pobj)->pvy[index];
	  if (pPOLYLINE_FEATURE (pobj)->pvz != (double *) NULL) {
	    Z[0]=pPOLYLINE_FEATURE (pobj)->pvz[index];
	    Z[1]=pPOLYLINE_FEATURE (pobj)->pvz[index];
	    z=Z;
	  }
	  else 
	    z=(double *) NULL;
	  x=X;y=Y;z=Z;
	  break;
	case 5: /* patch*/
	  x=pPOLYLINE_FEATURE (pobj)->pvx;
	  y=pPOLYLINE_FEATURE (pobj)->pvy;
	  z=pPOLYLINE_FEATURE (pobj)->pvz;
	  p=n1;
	break;
	}
	break;
      case  SCI_SEGS: 
	if (pSEGS_FEATURE (pobj)->iflag == 1) 
	  pstyle=sciGetGoodIndex(pobj, pSEGS_FEATURE (pobj)->pstyle[index]);
	else
	  pstyle=sciGetGoodIndex(pobj, pSEGS_FEATURE (pobj)->pstyle[0]);
	iflag=pSEGS_FEATURE (pobj)->iflag;
	p = 2;
	X[0]=pSEGS_FEATURE (pobj)->vx[2*index];
	X[1]=pSEGS_FEATURE (pobj)->vx[2*index+1];
	Y[0]=pSEGS_FEATURE (pobj)->vy[2*index];
	Y[1]=pSEGS_FEATURE (pobj)->vy[2*index+1];
	if (pSEGS_FEATURE (pobj)->vz != (double *) NULL) {
	  Z[0]=pSEGS_FEATURE (pobj)->vz[2*index];
	  Z[1]=pSEGS_FEATURE (pobj)->vz[2*index+1];
	  z=Z;
	}
	else 
	  z=(double *) NULL;
	x=X;y=Y;
	break;
      case  SCI_RECTANGLE: 
	p = 5;
	pstyle=0; /* arevoir */
	iflag=0; /* arevoir */
	X[0]=X[1]=X[4]=pRECTANGLE_FEATURE (pobj)->x;
	Y[0]=Y[3]=Y[4]=pRECTANGLE_FEATURE (pobj)->y;
	X[2]=X[3]=pRECTANGLE_FEATURE (pobj)->x + pRECTANGLE_FEATURE (pobj)->width;
	Y[1]=Y[2]=pRECTANGLE_FEATURE (pobj)->y - pRECTANGLE_FEATURE (pobj)->height;
	Z[0]=Z[1]=Z[2]=Z[3]=Z[4]=pRECTANGLE_FEATURE (pobj)->z;
	x=X;y=Y;z=Z; 
	break;
      default:
	p = 0;
      }
      if (p > 0) {
	/* project 3D on 2D coordinates */
	if (z != (double *)NULL) {
	  for ( j =0 ; j < p ; j++) { 
	    if (trans3d(psubwin ,1, &(polyx[j]),&(polyy[j]),&(x[j]),&(y[j]),&(z[j]))==0) {
	      nok=1;break; 
	    }
	  }
	}
	else {
	  double zz=0.0;
	  for ( j =0 ; j < p ; j++) { 
	    if (trans3d(psubwin ,1, &(polyx[j]),&(polyy[j]),&(x[j]),&(y[j]),&zz)==0) {
	      nok=1;break; 
	    }
	  }
	}


	/* draw element */
	context[0] = sciGetForeground (pobj);	
	context[1] = sciGetLineWidth (pobj);
	context[2] = sciGetLineStyle (pobj); 
	context[3] = 0;
	context[4] = sciGetMarkStyle(pobj);
	context[5] = sciGetLineWidth (pobj);
#ifdef WIN32
	hdcflag=MaybeSetWinhdc();
#endif
	C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, 
		  PD0, PD0, PD0, 5L, 6L);
	C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, 
		  PD0, PD0, PD0, 5L, 10L);
	C2F (dr) ("xset", "thickness",  context+1, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 9L);
	C2F (dr) ("xset", "line style", context+2, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L); 
	C2F (dr) ("xset", "mark", context+4, context+5, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 4L, 4L);
#ifdef WIN32
	if ( hdcflag == 1) ReleaseWinHdc ();
#endif	  

	if (sciGetEntityType (pobj)==SCI_SURFACE) { 
	  int fg1  = pSURFACE_FEATURE (pobj)->hiddencolor;
	  int flag = pSURFACE_FEATURE (pobj)->flag[0];
	  polyx[p]=polyx[0];polyy[p]=polyy[0];p++; /*close the facet*/

	  if ((((polyx[1]-polyx[0])*(polyy[2]-polyy[0])-(polyy[1]-polyy[0])*(polyx[2]-polyx[0])) <  0) &&
	      (fg1 >= 0)) { /* hidden face */

	    if ( pSURFACE_FEATURE (pobj)->flagcolor != 0)
	      fill[0] = (flag < 0 ) ? -fg1 : fg1 ;
	    else
	      fill[0] = (flag != 0 ) ? fg1 : flag ;
	    C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&p,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  }
	  else {
	    switch ( pSURFACE_FEATURE (pobj)->flagcolor) {
	    case 0:
	      fill[0]= flag ;
	      C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&p, PI0,PD0,PD0,PD0,PD0,0L,0L);
	      break;
	    case 1:
	      zl=0;
	      for ( k1= 0 ; k1 < p ; k1++) zl+= z[k1];
	      fill[0]=inint((whiteid-1)*((zl/p)-zmin)/(zmax-zmin))+1;
	      if ( flag  < 0 ) fill[0]=-fill[0];
	      C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&p ,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      break;
	    case 2:
	      fill[0]= pSURFACE_FEATURE (pobj)->zcol[index];
	      if ( flag < 0 ) fill[0]=-fill[0];
	      C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&p ,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      break;
	    case 3:
	      p--;
	      if ( (p) != 3 && (p) !=4 ) {
		Scistring("Interpolated shading is only allowed for polygons with 3 or 4 vertices\n");
		return;
	      } 
	      else  {
		/*for ( k1= 0 ; k1 < p ; k1++) fill[k1]=pSURFACE_FEATURE (pobj)->zcol[index];*/
		shade(polyx,polyy,&(pSURFACE_FEATURE (pobj)->zcol[p*index]),p,pSURFACE_FEATURE (pobj)->flag[0]);
	      }
	      break;
	    }
	  }
	} /* end SCI_SURFACE*/
	else { 
	  if (p == 1)/*point */
	    C2F (dr) ("xmarks", "str", &p, polyx,polyy, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 8L, 2L);
	  else if (p==2) /*segment*/
	    C2F(dr)("xsegs","v",polyx,polyy,&p,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  else {/*patch*/
	    int close=1;
	    C2F (dr) ("xarea", "v", &p, polyx, polyy, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);
	  }
	}
      }
    }
  }
  FREE(dist);FREE(locindex);FREE(polyx);FREE(polyy);
}



int labels2D_draw(sciPointObj * psubwin)
{
  integer x[6], v,rect1[4];
  double dv, locx, locy;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  char locstr;
/*   integer fontid, fontsize, textcolor; */

  if (!sciGetVisibility(psubwin)) return 0;
  /*sciSetCurrentObj (pobj);       F.Leray 25.03.04*/
  /* load the object foreground and dashes color */
 
  if (sciGetEntityType(psubwin) == SCI_SUBWIN)
    {
      locstr=ppsubwin->axes.xdir;
      switch (locstr)
	{
	case 'd':
	  locy=ppsubwin->FRect[1];
	  break;
	case 'c':
	  locy=(ppsubwin->FRect[1]>0.0)?ppsubwin->FRect[1]: 0.0;
	  locy=(ppsubwin->FRect[3]<0.0)?ppsubwin->FRect[1]: locy;
	  break;
	case 'u':
	  locy=ppsubwin->FRect[3];
	  break;
	default:
	  locy=ppsubwin->FRect[1];
	  break;
	}
      locstr=ppsubwin->axes.ydir;
      switch (locstr)
	{
	case 'l':
	  locx=ppsubwin->FRect[0];
	  break;
	case 'c':
	  locx=(ppsubwin->FRect[0]>0.0)?ppsubwin->FRect[0]: 0.0;
	  locx=(ppsubwin->FRect[2]<0.0)?ppsubwin->FRect[0]: locx;
	  break;
	case 'r':
	  locx=ppsubwin->FRect[2];
	  break;
	default:
	  locx=ppsubwin->FRect[0];
	  break;
	}
    }

  /* main title */ /* We fix the title always at the top */
  rect1[0]= Cscale.WIRect1[0];
  rect1[1]= Cscale.WIRect1[1];
  rect1[2]= Cscale.WIRect1[2];
  rect1[3]= Cscale.WIRect1[3]/6;

  x[0] = sciGetFontForeground (ppsubwin->mon_title);
  x[2] = sciGetFontDeciWidth (ppsubwin->mon_title)/100;
  x[3] = 0;
  x[4] = sciGetFontStyle(ppsubwin->mon_title);
  
  C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
  C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
  C2F(dr)("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);
  
  if( sciGetVisibility(ppsubwin->mon_title) == TRUE)
    C2F(dr1)("xstringtt",sciGetText(ppsubwin->mon_title),&rect1[0],&rect1[1],&rect1[2],&rect1[3],&v,&v,&dv,&dv,&dv,&dv,10L,0L);
  
  
  /* x label */
  rect1[0]= Cscale.WIRect1[0]+Cscale.WIRect1[2];
  rect1[1]= YScale(locy-(ppsubwin->FRect[3]-ppsubwin->FRect[1])/12);
  rect1[2]= Cscale.WIRect1[2]/6;
  rect1[3]= Cscale.WIRect1[3]/6;
  
  x[0] = sciGetFontForeground (ppsubwin->mon_x_label);
  x[2] = sciGetFontDeciWidth (ppsubwin->mon_x_label)/100;
  x[3] = 0;
  x[4] = sciGetFontStyle(ppsubwin->mon_x_label);
  
  C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
  C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
  C2F(dr)("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);
  
   if( sciGetVisibility(ppsubwin->mon_x_label) == TRUE)
    C2F(dr1)("xstringtt",sciGetText(ppsubwin->mon_x_label),&rect1[0],&rect1[1],&rect1[2],&rect1[3],&v,&v,&dv,&dv,&dv,&dv,10L,0L);
  
  /* y label */
  rect1[0]= XScale(locx-(ppsubwin->FRect[2]-ppsubwin->FRect[0])/12);
  rect1[1]= Cscale.WIRect1[1]-Cscale.WIRect1[3]/24;
  rect1[2]= Cscale.WIRect1[2]/6;
  rect1[3]= Cscale.WIRect1[3]/12;
  
  x[0] = sciGetFontForeground (ppsubwin->mon_y_label);
  x[2] = sciGetFontDeciWidth (ppsubwin->mon_y_label)/100;
  x[3] = 0;
  x[4] = sciGetFontStyle(ppsubwin->mon_y_label);
  
  C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
  C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
  C2F(dr)("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);
  
  if( sciGetVisibility(ppsubwin->mon_y_label) == TRUE)
    C2F(dr1)("xstringtt",sciGetText(ppsubwin->mon_y_label),&rect1[0],&rect1[1],&rect1[2],&rect1[3],&v,&v,&dv,&dv,&dv,&dv,10L,0L);
  
  return 0;
}



/**ConstructLabel
 * @memo This function creates Label structure used for x,y,z labels and for the Title.
 * @param  sciPointObj *pparentsubwin
 * @param  char text[] : intial text string.
 * @param  int type to get info. on the type of label
 * @return  : pointer sciPointObj if ok , NULL if not
 */
sciPointObj *
ConstructLabel (sciPointObj * pparentsubwin, char *text, int type)
{
  sciPointObj *pobj = (sciPointObj *) NULL;


  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
    {
      if ((pobj = MALLOC (sizeof (sciPointObj))) == NULL)
	return (sciPointObj *) NULL;
      sciSetEntityType (pobj, SCI_LABEL);
      if ((pobj->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
	{
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      if (sciAddNewHandle (pobj) == -1)
	{
	  FREE(pLABEL_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
     /*  sciSetParent (pobj, pparentsubwin); */
      if (!(sciAddThisToItsParent (pobj, pparentsubwin)))
	{
	  sciDelHandle (pobj);
	  FREE(pLABEL_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      
      sciSetCurrentSon (pobj, (sciPointObj *) NULL);
      
      pLABEL_FEATURE (pobj)->text.relationship.psons = (sciSons *) NULL;
      pLABEL_FEATURE (pobj)->text.relationship.plastsons = (sciSons *) NULL;

      pLABEL_FEATURE (pobj)->text.callback = (char *)NULL;
      pLABEL_FEATURE (pobj)->text.callbacklen = 0; 
      pLABEL_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentFigure(pobj)); 
     

      if ((pLABEL_FEATURE (pobj)->text.ptextstring =calloc (strlen(text)+1, sizeof (char))) == NULL)
	{
	  sciprint("No more place to allocates text string, try a shorter string");
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pLABEL_FEATURE(pobj));
	  FREE(pobj);
	  return (sciPointObj *) NULL;
	}
      /* on copie le texte du label dans le champs specifique de l'objet */
      strcpy (pLABEL_FEATURE (pobj)->text.ptextstring, text);
      
      pLABEL_FEATURE (pobj)->text.textlen = strlen(text);
      pLABEL_FEATURE (pobj)->ptype = type;
      
      pLABEL_FEATURE (pobj)->text.fontcontext.textorientation = 0; 

    /*   pLABEL_FEATURE (pobj)->titleplace = SCI_LABEL_IN_TOP; */
      pLABEL_FEATURE (pobj)->isselected = TRUE;
      if (sciInitFontContext (pobj) == -1)
	{
	  FREE(pLABEL_FEATURE(pobj)->text.ptextstring);
	  sciDelThisToItsParent (pobj, sciGetParent (pobj));
	  sciDelHandle (pobj);
	  FREE(pLABEL_FEATURE(pobj));
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
DestroyLabel (sciPointObj * pthis)
{
  FREE (pLABEL_FEATURE (pthis)->text.ptextstring);
  sciDelThisToItsParent (pthis, sciGetParent (pthis));
  if (sciDelHandle (pthis) == -1)
    return -1;
  FREE ((sciGetFontContext(pthis))->pfontname);
  FREE (sciGetPointerToFeature (pthis));
  FREE (pthis);
  /* on peut alors destroyer le parent */
  return 0;
}

/**delete_sgwin_entities(int win_num)
 * @memo This function is to be called after window deletion 
 */
void delete_sgwin_entities(int win_num,int v_flag)
{
  double dv=0;
  double *XGC;
  struct BCG *CurrentScilabXgc; 
  int v=0;

  C2F(dr)("xget","gc",&v,&v,&v,&v,&v,&v,(double *)&XGC,&dv,&dv,&dv,5L,10L); /* ajout cast ???*/
  CurrentScilabXgc=(struct BCG *)XGC;

  if(v_flag == 0)
    {
      /* Need to reset the new current figure returned by sciGetCurrentFigure */
      sciHandleTab *hdl = NULL;
      sciPointObj  *pobj= NULL;
      
      hdl = sciGetpendofhandletab();
		 
      if(CurrentScilabXgc != NULL)
	while (hdl != NULL)
	  {
	    pobj=(sciPointObj *) sciGetPointerFromHandle (hdl->index);
	    if (sciGetEntityType(pobj) == SCI_FIGURE && sciGetNum(pobj) == CurrentScilabXgc->CurWindow ) /* Adding F.Leray 19.04.04 */
	      {
		sciSetCurrentFigure(pobj);
		/* sciGetScilabXgc (pobj)->CWindow */
		/* cur =  sciGetScilabXgc (pobj)->CWindow;*/
		/* to force a reset in the graphic scales : COPY from Actions.c line 237 */
		/* SwitchWindow(&cur);*/
		/*C2F(dr)("xset","window",&(pFIGURE_FEATURE(pobj)->number),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,7L);*/
		/*sciDrawObj(pobj);*/
		/*sciSetSelectedSubWin((sciPointObj *) sciGetSelectedSubWin
		  (pobj));*/
		sciSetCurrentObj(pobj); /* The current object will always be the figure too. */
		break;
		
	      }
	    hdl = hdl->pprev;
	  }
    }
}



