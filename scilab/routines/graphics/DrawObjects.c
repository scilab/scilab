/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE,   INRIA 2001-2002
 *    Djalel ABDEMOUCHE,   INRIA 2002-2004
 *    Fabrice Leray,       INRIA 2004-2006
 *    Jean-Baptiste SILVY, INRIA 2006-xxxx
 *    Comment:
 *    This file contains all functions used to Draw the content of a window.
 *    The main functions is sciDrawObj that draws the objects recursively.
 --------------------------------------------------------------------------*/
#include <math.h>

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "DrawObjects.h"
#include "GetProperty.h"
#include "bcg.h"
#include "SetProperty.h"
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "ObjectStructure.h"
#include "Graphics.h"

#if WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

#if WIN32
extern HDC TryToGetDC(HWND hWnd);
#endif

#define		round(a)	(int)(((a)<0.0)?(a)-.5:(a)+.5)

extern int GetScreenDPI(int *ixres, int *iyres);
int WindowsPrintScreen = 0;
int Printer_XRes = 0;
int Printer_YRes = 0;

void drawAxesGrid(sciPointObj * psubwin);

extern double C2F(dsort)();/*DJ.A merge*/ 
extern int scilab_shade(integer *polyx, integer *polyy, integer *fill, integer polysize, integer flag);
extern void xstringb_angle (char *string, integer x, integer y, integer w, integer h, double angle);
extern void xstringb_bbox (char *string, integer x, integer y, integer w, integer h, double angle, int *bbox);
extern int GlobalFlag_Zoom3dOn;
extern void updateScaleIfRequired( sciPointObj * pSubWin ) ;

extern int index_vertex;
int Store3DPixelValues(sciPointObj * pobj, int xm, int ym, double x, double y, double z);
static Vertices * pHead = (Vertices *) NULL;
static Vertices * pHead2 = (Vertices *) NULL;

int xinitxend_flag = 0;
/* sciClipTab ptabclip[15]; */

static double xz1,yz1;
#define TX3D(x1,y1,z1) inint(xz1= Cscale.Wscx1*(TRX(x1,y1,z1)-Cscale.frect[0]) +Cscale.Wxofset1);
#define TY3D(x1,y1,z1) inint(yz1= Cscale.Wscy1*(-TRY(x1,y1,z1)+Cscale.frect[3])+Cscale.Wyofset1);

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


#ifdef WIN32
static int flag_DO; /* F.Leray 16.02.04 flag global pour la fonction recursive DrawObj*/
#endif



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
      return (HDC)TryToGetDC (sciGetScilabXgc (pobj)->CWindow);
      break;
    default:
      return sciGetDC (sciGetParent (pobj));
      break;
    }
  return (HDC ) NULL;        /* Type HDC ! "periWin-bgc"*/
}
#endif


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

/* Completely destroy the handle hierearchy */
/* when switching from new to old figure_style */
/* see intgset.c */
void sciXClearFigure()
{  
  static sciPointObj *mafigure;
  struct BCG * XGC = (struct BCG*) NULL;
  int v = 1, verb = 0;
  double dv = 0.;

  mafigure= (sciPointObj *) sciGetCurrentFigure(); 
  DestroyAllGraphicsSons((sciPointObj *)mafigure);
  DestroyFigure (mafigure); 
  C2F(dr)("xget","gc",&verb,&v,&v,&v,&v,&v,(double *)&XGC,&dv,&dv,&dv,5L,10L);
  XGC->mafigure = (sciPointObj *) NULL;
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


void updateScale3d( sciPointObj * pobj    ,
                    double        dBox[6] ,
                    double        xBox[8] ,
                    double        yBox[8] ,
                    double        zBox[8]  )
{
  double Alpha ;
  double Theta ;
  double cost  ;
  double sint  ;
  double cosa  ;
  double sina  ;
  double xmmin ;
  double ymmax ;
  double xmmax ;
  double ymmin ;
  double wmax  ;
  double hmax  ;
  int i        ;
  int flag     ;
  static integer aaint[4] = {2,10,2,10} ;
  sciSubWindow * ppSubWin = pSUBWIN_FEATURE(pobj) ;

  sci_update_frame_bounds_3d( pobj ) ;
  
  
  dBox[0] =  ppSubWin->FRect[0]; /*xmin*/
  dBox[1] =  ppSubWin->FRect[2]; /*ymin*/ /* FAUX */ /* c'est xmax */
  dBox[2] =  ppSubWin->FRect[1]; /*xmax*/ /* FAUX */ /* c'est ymin */
  dBox[3] =  ppSubWin->FRect[3]; /*ymax*/
  dBox[4] =  ppSubWin->FRect[4]; /*zmin*/
  dBox[5] =  ppSubWin->FRect[5]; /*zmax*/
  
  if( ppSubWin->cube_scaling )
  {
    dBox[0] =  0. ;
    dBox[1] =  1. ;
    dBox[2] =  0. ;
    dBox[3] =  1. ;
    dBox[4] =  0. ;
    dBox[5] =  1. ;
  }
  
  /** changement de coordonnees 3d */
  flag = (long)(pSUBWIN_FEATURE (pobj)->axes.flag[1] + 1 ) / 2 ; /* F.Leray Adding HERE 19.04.04 */

  Alpha = DEG2RAD( pSUBWIN_FEATURE (pobj)->alpha ) ;
  Theta = DEG2RAD( pSUBWIN_FEATURE (pobj)->theta ) ;
  
  Cscale.alpha = pSUBWIN_FEATURE (pobj)->alpha ;
  Cscale.theta = pSUBWIN_FEATURE (pobj)->theta ;
  
  cost = cos( Theta ) ;
  cosa = cos( Alpha ) ;
  sint = sin( Theta ) ;
  sina = sin( Alpha ) ;
  
  Cscale.m[0][0]= -sint      ;  Cscale.m[0][1]= cost       ;   Cscale.m[0][2]= 0    ;
  Cscale.m[1][0]= -cost*cosa ;  Cscale.m[1][1]= -sint*cosa ;   Cscale.m[1][2]= sina ;
  Cscale.m[2][0]=  cost*sina ;  Cscale.m[2][1]= sint*sina  ;   Cscale.m[2][2]= cosa ;
  
  for ( i = 0 ; i < 6 ; i++ )
  { 
    if ( flag == 0 )
    {
      dBox[i] = Cscale.bbox1[i] ;
    }
    else
    { 
      Cscale.bbox1[i] = dBox[i] ;
    }
  }

  xBox[0]=TRX(dBox[0],dBox[2],dBox[4]); /* transfo. 3D of [xmin,ymin,zmin] */
  yBox[0]=TRY(dBox[0],dBox[2],dBox[4]); /* into [xbox[0],ybox[0],zbox[0] ] */
  zBox[0]=TRZ(dBox[0],dBox[2],dBox[4]); /*                                 */ 

  xBox[1]=TRX(dBox[0],dBox[3],dBox[4]); /* transfo. 3D of [xmin,ymax,zmin] */
  yBox[1]=TRY(dBox[0],dBox[3],dBox[4]); /* into [xbox[1],ybox[1],zbox[1] ] */
  zBox[1]=TRZ(dBox[0],dBox[3],dBox[4]); /*                                 */

  xBox[2]=TRX(dBox[1],dBox[3],dBox[4]); /* transfo. 3D of [xmax,ymax,zmin] */
  yBox[2]=TRY(dBox[1],dBox[3],dBox[4]); /* into [xbox[2],ybox[2],zbox[2] ] */
  zBox[2]=TRZ(dBox[1],dBox[3],dBox[4]); /*                                 */

  xBox[3]=TRX(dBox[1],dBox[2],dBox[4]); /* transfo. 3D of [xmax,ymin,zmin] */
  yBox[3]=TRY(dBox[1],dBox[2],dBox[4]); /* into [xbox[3],ybox[3],zbox[3] ] */
  zBox[3]=TRZ(dBox[1],dBox[2],dBox[4]); /*                                 */

  xBox[4]=TRX(dBox[0],dBox[2],dBox[5]); /* transfo. 3D of [xmin,ymin,zmax] */
  yBox[4]=TRY(dBox[0],dBox[2],dBox[5]); /* into [xbox[4],ybox[4],zbox[4] ] */
  zBox[4]=TRZ(dBox[0],dBox[2],dBox[5]); /*                                 */

  xBox[5]=TRX(dBox[0],dBox[3],dBox[5]); /* transfo. 3D of [xmin,ymax,zmax] */
  yBox[5]=TRY(dBox[0],dBox[3],dBox[5]); /* into [xbox[5],ybox[5],zbox[5] ] */
  zBox[5]=TRZ(dBox[0],dBox[3],dBox[5]); /*                                 */

  xBox[6]=TRX(dBox[1],dBox[3],dBox[5]); /* transfo. 3D of [xmax,ymax,zmax] */
  yBox[6]=TRY(dBox[1],dBox[3],dBox[5]); /* into [xbox[6],ybox[6],zbox[6] ] */
  zBox[6]=TRZ(dBox[1],dBox[3],dBox[5]); /*                                 */

  xBox[7]=TRX(dBox[1],dBox[2],dBox[5]); /* transfo. 3D of [xmax,ymin,zmax] */
  yBox[7]=TRY(dBox[1],dBox[2],dBox[5]); /* into [xbox[7],ybox[7],zbox[7] ] */
  zBox[7]=TRZ(dBox[1],dBox[2],dBox[5]); /*                                 */


  /** Calcul des echelles en fonction de la taille du dessin **/
  if ( flag == 1 || flag == 3 ) /* ALL the expanded cases : flag[1] = 1 or 2 or 5 or 6 */
  {
    xmmin =  Mini( xBox, 8L ) ;
    xmmax =  Maxi( xBox, 8L ) ; /* search for x Min/Max on all the edges (there are 8 edges that compose the box) F.Leray 13.10.04 */
    ymmax = -Mini( yBox, 8L ) ; /* same thing on ybox vector ( 1) why - (minus) ? see 2) )*/
    ymmin = -Maxi( yBox, 8L ) ;
  }


  if ( flag == 2 || flag == 3 ) /* ALL the isometric cases : flag[1] = 3 or 4 or 5 or 6 */
  {
    int wdim[2]     ;
    int narg        ;
    int verbose = 0 ;
    double FRect[4] ;
    double WRect[4] ;
    double ARect[4] ;
    char   logf[2]  ;
    /* get current window size */
    C2F(dr)("xget","wdim",&verbose,wdim,&narg, PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L) ;
    getscale2d( WRect, FRect, logf, ARect ) ;
    wmax = linint( (double)wdim[0] * WRect[2] ) ;
    hmax = linint( (double)wdim[1] * WRect[3] ) ; 
  }

  if ( flag == 2 ) /* the "NON expanded isometric" cases : flag[1] = 3 or 4 */
  {
    /* radius and center of the sphere circumscribing the box */
    double dx = dBox[1] - dBox[0]                   ;
    double dy = dBox[3] - dBox[2]                   ;
    double dz = dBox[5] - dBox[4]                   ;
    double R  = sqrt( dx*dx + dy*dy + dz*dz ) / 2.0 ;
    double xo = ( xBox[0] + xBox[6] ) / 2.0         ;
    double yo = ( yBox[0] + yBox[6] ) / 2.0         ;

    xmmin =  xo - R ;
    xmmax =  xo + R ;
    ymmax = -yo + R ;
    ymmin = -yo - R ;
  }

  if ( flag == 2 || flag == 3 )
  {
    double hx = xmmax - xmmin ;
    double hy = ymmax - ymmin ;
    if ( hx / wmax < hy / hmax ) 
    {
      double hx1 = wmax * hy / hmax ;
      xmmin = xmmin - (hx1-hx) / 2.0 ;
      xmmax = xmmax + (hx1-hx) / 2.0;
    }
    else 
    {
      double hy1 = hmax * hx / wmax ;
      ymmin = ymmin - (hy1-hy) / 2.0 ;
      ymmax = ymmax + (hy1-hy) / 2.0 ;
    }
  }

  if (flag != 0 ) /* != using current 3D scale */
  {
    double FRect[4] ;
    /* FRect = [Xmin,Ymin,Xmax,Ymax] */
    FRect[0] =  xmmin ;
    FRect[1] = -ymmax ;
    FRect[2] =  xmmax ;
    FRect[3] = -ymmin ; /* 2) ... (why - (minus) ? )*/
    set_scale("tftttf",NULL,FRect,aaint,"nn",NULL);
    Cscale.metric3d = flag; 
  }

}


void sciAxesVerticesIndices( integer InsideU[4],
                             integer InsideD[4],
                             double  xbox[8]   ,
                             double  ybox[8]   ,
                             integer xind[8]    )
{
  double xmaxi ;
  int ind      ;
  int ind2     ;
  int ind3     ;
  int tmpind   ;
  /* indices */
  /* determine the indices for the 3d represention */
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
}

/**axis_3ddraw 10/2003
 * @author Djalel Abdemouche
 * Should be in Axes.c file
 */
void axis_3ddraw(sciPointObj *pobj, double *xbox, double *ybox, double *zbox, integer *InsideU, integer *InsideD) 
{
  double dbox[6] ;
  integer flag,i,p,n,pat,hiddencolor, x[5]; /* F. Leray : redimmensionnment (+1) du tableau x[4];*/
  double Alpha, Theta ;
  int verbose=0,narg;
  integer ixbox[8],iybox[8],xind[8],dash[6];
  integer background,zero=0, color_old; /* Adding color_old 04.03.04 */
  

  /* Initialisation phase for x (to detect bug): x set to -1000 F.Leray 05.03.04*/
  for(i=0;i<5;i++) { x[i] = -1000 ; }

  if(sciGetEntityType (pobj) == SCI_SUBWIN)
    {

      updateScale3d( pobj, dbox, xbox, ybox, zbox ) ;
           
      /* F.Leray 23.02.04 Mise a 0 du tableau xind pour corriger bug*/
      /* dans le cas ind < 3 ET ybox[tmpind] < ybox[tmpind]*/
      for( i = 0 ; i < 8 ; i++ ) { xind[i] = 0 ; }


      /* Until here we have computed + reset the 3d scale*/
      
      flag = pSUBWIN_FEATURE (pobj)->axes.flag[2]; /* box drawing */
      
      Alpha = pSUBWIN_FEATURE (pobj)->alpha ;
      Theta = pSUBWIN_FEATURE (pobj)->theta ;

      /* modify the test with |teta| < eps */
      if( Theta == 0.0 )
      {
	/* to avoid bug at limit when theta == 0 */
	/* I recompute temp value xyzbox with theta == 0.1 */
	/* to have a correct xind, InsideU et InsideD */
	ComputeCorrectXindAndInsideUD(Theta,Alpha,dbox,xind,InsideU,InsideD);
      }
      else
      {
        sciAxesVerticesIndices( InsideU, InsideD, xbox, ybox, xind ) ;
      }

      /* F.Leray Rajout 02.04.04 :*/
      background = sciGetBackground(pobj) ;
      
      for (i=0; i < 6 ; i++)
      {
        ixbox[i]=XScale(xbox[xind[i]]);
        iybox[i]=YScale(ybox[xind[i]]);
      }
      ixbox[6]=ixbox[0];iybox[6]=iybox[0]; p=7,n=1; 
      
      C2F(dr)("xget","pattern",&verbose,&color_old,&zero,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","pattern",&background,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	 
      C2F (dr) ("xarea", "v", &p, ixbox, iybox, &n, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);
      C2F(dr)("xset","pattern",&color_old,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      /***********/
      /***  hidden axis */
      if( flag != 0 && flag != 1 )
      { 
        x[2] = sciGetLineWidth (pobj);
        C2F (dr) ("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
        /* DJ.A 2003 */
        hiddencolor = pSUBWIN_FEATURE(pobj)->hiddencolor ;
        if (hiddencolor==-1) { hiddencolor = 0 ; }
        if (zbox[InsideU[0]] > zbox[InsideD[0]])
          DrawAxis(xbox,ybox,InsideD,hiddencolor);
        else
        {
          DrawAxis(xbox,ybox,InsideU,hiddencolor); 	
        }
        if (Ishidden(pobj))
        {
          pSUBWIN_FEATURE (pobj)->hiddenstate=(InsideU[0] % 4) ;
        }
        else
        {
          pSUBWIN_FEATURE (pobj)->hiddenstate=(InsideD[0] % 4) ;
        }
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
      C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4,PI0,PD0,PD0,PD0,PD0, 5L, 6L);
      C2F (dr) ("xset","foreground",x,x,x+4,x+4,x+4,PI0,PD0,PD0,PD0,PD0,5L,4096); /* F.Leray 05.03.04 Useless too*/
      C2F (dr) ("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F (dr) ("xset", "line style", x+3,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      for (i=0; i < 6 ; i++)
	{
	  ixbox[i]=XScale(xbox[xind[i]]);
	  iybox[i]=YScale(ybox[xind[i]]);
	}
      ixbox[6]=ixbox[0];iybox[6]=iybox[0]; p=7,n=1;

      if (flag >=3){C2F(dr)("xpolys","v",ixbox,iybox,x,&n,&p,PI0,PD0,PD0,PD0,PD0,0L,0L);}
      /** graduation ***/
      if (flag>=3) {Axes3dStrings2(ixbox,iybox,xind);}
      
      C2F(dr)("xset","pattern",&pat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
}

void triedre(sciPointObj *pobj, double *xbox, double *ybox, double *zbox, integer *InsideU, integer *InsideD)
{
  integer  x[5],narg = 0;
  integer color_kp,verbose = 0,thick_kp ;
  integer style_kp[3];

  C2F(dr)("xget","pattern",&verbose,&color_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /*F.Leray Replacement*/
  C2F(dr)("xget","thickness",&verbose,&thick_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /*F.Leray addings here*/
  C2F(dr)("xget","line style",&verbose,style_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /*F.Leray addings here*/

  if(sciGetEntityType (pobj) == SCI_SUBWIN) 
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
  
  C2F(dr)("xset", "line style",&(style_kp[0]),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);/*F.Leray addings here*/
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


/**Axes3dStrings2
 * @author F.Leray 18.10.04
 * Should be in Axes.c file
 */
int Axes3dStrings2(integer *ixbox, integer *iybox, integer *xind)
{
  integer verbose=0,narg,xz[2],fontid[2],fontsize_kp,color_kp,size;
  integer iof,barlengthx = 0,barlengthy = 0, posi[2]; 
  char *legx = NULL;
  char *legy = NULL;
  char *legz = NULL;
  char *title = NULL;
  integer rect[4],flag=0,x=0,y=0;
  double ang=0.0, bbox[6];
  int fontsize=-1,textcolor=-1,ticscolor=-1;
  int fontstyle=0; /* F.Leray 08.04.04 */
  sciPointObj *psubwin = NULL;
  sciSubWindow * ppsubwin = NULL;
  int ns=2,iflag=0,gstyle,trois=3,dash[6];
  double xx[4],yy[4],zz[4],vxx1,vyy1,vzz1;
  integer i,xm,ym,vx[2],vy[2],xg[2],yg[2],j;
  integer fontid_old[2], textcolor_old;

  int lastzindex = 0, lastxindex = 0, lastyindex = 0;
  double xminval, yminval, zminval, xmaxval, ymaxval, zmaxval;
  int nbtics = 0;
  int nbsubtics = 0;

  int logrect[4], XX = 0, YY = 0; /* see below */ /* ah ouais ou ca ? */
  double angle=0.0;
  
  double cosangle, sinangle;
  
  int xm4[4], ym4[4];
  char str[2] = "xv";
  int close=1;
  int largeur, hauteur;
  int zero=0;
  int rect1[4];
/*   int old_rect[4]; */

  /*   printf("DEBUT DE Axes3dStrings2\n"); */
  /*   fflush(NULL); */

  psubwin= (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());
  ppsubwin = pSUBWIN_FEATURE (psubwin);

  title= sciGetText(ppsubwin->mon_title);
  legx = sciGetText(ppsubwin->mon_x_label);
  legy = sciGetText(ppsubwin->mon_y_label);
  legz = sciGetText(ppsubwin->mon_z_label);

/*   /\* compute bounding of "10"  string used for log scale ON and auto_ticks ON *\/ */
/*   C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	 */
    
  /** le cot\'e gauche ( c'est tjrs un axe des Z **/
  xz[0]=Cscale.WIRect1[2] ;
  xz[1]= Cscale.WIRect1[2];
  iof = (xz[0]+xz[1])/50;
  /*x=ixbox[2]-(xz[0]+xz[1])/20 ;y=0.5*iybox[3]+0.5*iybox[2];*/
  
  /*   psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ()); */
/*   ticscolor = pSUBWIN_FEATURE (psubwin)->axes.ticscolor; /\* no more use : property has been removed (except for AXIS (see drawaxis)) *\/ */
  ticscolor = sciGetForeground(psubwin);
  textcolor=sciGetFontForeground(psubwin);

  fontsize=sciGetFontDeciWidth(psubwin)/100;
  fontstyle=sciGetFontStyle(psubwin);
  
  if(sciGetEntityType (psubwin) != SCI_SUBWIN) { 
    sciprint("Impossible case\n");
    return 0;
  }
  
/*   /\* if ticscolor is -1 or -2 *\/ */
/*   /\* compute the good index *\/ */
/*   ticscolor=sciGetGoodIndex(psubwin,ticscolor); */

  bbox[0] =  xminval = pSUBWIN_FEATURE (psubwin)->FRect[0]; /*xmin*/
  bbox[1] =  xmaxval = pSUBWIN_FEATURE (psubwin)->FRect[2]; /*xmax*/
  bbox[2] =  yminval = pSUBWIN_FEATURE (psubwin)->FRect[1]; /*ymin*/
  bbox[3] =  ymaxval = pSUBWIN_FEATURE (psubwin)->FRect[3]; /*ymax*/ 
  bbox[4] =  zminval = pSUBWIN_FEATURE (psubwin)->FRect[4]; /*zmin*/
  bbox[5] =  zmaxval = pSUBWIN_FEATURE (psubwin)->FRect[5]; /*zmax*/
  
  C2F(dr)("xget","font",&verbose,fontid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  fontid[0]= fontstyle;
  fontsize_kp = fontid[1] ;
  if( fontsize == -1 ){ 
    fontid[1]= 1;
    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  else {
    fontid[1] = fontsize ;
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

  
  if((title != NULL) && (sciGetVisibility(ppsubwin->mon_title) == TRUE))
  {
    int bboxtitle[4];
    double cosAngle   ;
    double sinAngle   ;
    int    x[5]       ;
    double font_angle ;
    int    v  = 0     ;
    double dv = 0.0   ;
    int    n  = 4     ;

    /* get the pointer on the title */
    sciLabel * ppLabel = pLABEL_FEATURE( ppsubwin->mon_title ) ;
    sciText  * ppText  = &(ppLabel->text) ;
    
    x[0] = sciGetFontForeground (ppsubwin->mon_title);
    x[2] = sciGetFontDeciWidth (ppsubwin->mon_title)/100;
    x[3] = 0;
    x[4] = sciGetFontStyle(ppsubwin->mon_title);

    /* get position and orientation of the title */
    if ( ppLabel->auto_rotation )
    {
      font_angle = 0 ;
      sciSetFontOrientation( ppsubwin->mon_title,(int) font_angle ) ;
    }
    else
    {
      font_angle = sciGetFontOrientation( ppsubwin->mon_title ) ;
      font_angle /= 10.0 ;
    }
    cosAngle = cos( DEG2RAD( 360 - font_angle ) ) ;
    sinAngle = sin( DEG2RAD( 360 - font_angle ) ) ;

    /* make it back to this value */
    
    C2F(dr)("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);
    
    xstringb_bbox (sciGetText(ppsubwin->mon_title), rect[0], rect[1], rect[2], rect[3],
                   0, bboxtitle);
     
    if ( ppLabel->auto_position )
    {

      /* get the position given by bboxtitle */
      bboxtitle[0] = bboxtitle[0]-1; /* better display */
      bboxtitle[1] = bboxtitle[1]-2 + bboxtitle[3] ; /* better display */
      ppText->x     = XPixel2Double( bboxtitle[0] ) ;
      ppText->y     = YPixel2Double( bboxtitle[1] ) ;
      sciSetPosition( ppsubwin->mon_title, ppText->x, ppText->y ) ;
    }
    else
    {
      /* update the position of the text */
      /* by copying the one in the label */
      /* yes redundant information */
      sciGetPosition( ppsubwin->mon_title, &(ppText->x), &(ppText->y) ) ;
      bboxtitle[0] = XDouble2Pixel( ppText->x ) ;
      bboxtitle[1] = YDouble2Pixel( ppText->y ) ;
    }
    
    xm4[0] = round(bboxtitle[0]);
    xm4[1] = round(xm4[0] + cosAngle * bboxtitle[2]);
    xm4[2] = round(xm4[0] + cosAngle*bboxtitle[2] + sinAngle*(-bboxtitle[3]));
    xm4[3] = round(xm4[0] + sinAngle*(-bboxtitle[3]));
    
    ym4[0] = round(bboxtitle[1]);
    ym4[1] = round(ym4[0] - sinAngle*bboxtitle[2]);
    ym4[2] = round(ym4[0] - sinAngle*bboxtitle[2] + cosAngle*(-bboxtitle[3]));
    ym4[3] = round(ym4[0] + cosAngle*(-bboxtitle[3]));

    
    if(sciGetIsFilled(ppsubwin->mon_title) == TRUE)
    {
      x[0] = sciGetBackground(ppsubwin->mon_title);
      
      /* fill the background of the box */
      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
      
      C2F (dr) ("xarea", str, &n, xm4, ym4, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
    
      x[0] = sciGetForeground(ppsubwin->mon_title);
      
      /* draw the rectangle */
      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
      
      C2F (dr) ("xlines", "xv", &n, xm4, ym4, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
    }
    
    x[0] = sciGetFontForeground(ppsubwin->mon_title);
    
    C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
    C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
    
    /* displayed the turned string */
    xstringb_angle(sciGetText(ppsubwin->mon_title),xm4[0],ym4[0],bboxtitle[2],bboxtitle[3],font_angle);
  }
  
  textcolor = textcolor_old;
  fontid[0] = fontid_old[0];
  fontid[1] = fontid_old[1];

  size = xz[0]>=xz[1] ? (integer) (xz[1]/50.0) : (integer) (xz[0]/50.0); 
  
  /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
  C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
  

  /********************/
  /*** le z scaling ***/ /* DISPLAY Z graduations */
  /********************/


  if (pSUBWIN_FEATURE (psubwin)->project[2]==1)
    {
      double fx,fy,fz; 
      char c_format[5];


      integer Ticsdir[2];
      Ticsdir[0]=ixbox[3]-ixbox[4]; /* <=> en pixel direction/vecteur non norme(e)s des tics en x */
      Ticsdir[1]=iybox[3]-iybox[4]; /* <=> idem pour y */
      /* NB: for a default plot3d (simply calling plot3d in siclab console) */
      /* Ticsdir[0] & Ticsdir[1] are negative : we do min - max here... */

      BBoxToval(&fx,&fy,&fz,xind[3],bbox); /* xind[3] <=> en bas a gauche <=> zmin */
      x=ixbox[2]-(xz[0]+xz[1])/20;
      y=(iybox[3]+iybox[2])/2;
  
      /*       NumberFormat(str,((integer) zz[0]),((integer) zz[2])); */
  
      if(ppsubwin->axes.auto_ticks[2] == FALSE)
	{
	  /* we display the z tics specified by the user*/
	  nbtics = ppsubwin->axes.u_nzgrads;
	  nbsubtics = ppsubwin->axes.nbsubtics[2];
	  
	  for(i=0;i<nbtics;i++)
	    {
	      char *foo = ppsubwin->axes.u_zlabels[i]; 
	      double ztmp = ppsubwin->axes.u_zgrads[i];
	      
	      if(ztmp<zminval || ztmp>zmaxval) 
		{
		  /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
		  continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
		  /* donc autant ne pas aller plus loin dans l'algo... */
		}
	      	  

	      /***************************************************************/
	      /************************* COMMON PART *************************/
	      /***************************************************************/
	      
	      if(ppsubwin->axes.reverse[2] == TRUE)
		ztmp = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp);
	      	      
	      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&ztmp);
	      /* 	      trans3d(psubwin,1,&xm,&ym,&fx,&fy,&ztmp); */


	      vx[0]=xm;vy[0]=ym;

	      barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
	      barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
	      vx[1]=vx[0]+barlengthx;
	      vy[1]=vy[0]+barlengthy;
		  
	      /* foo is set above with sprintf(foo,c_format,xtmp); */
		  
	      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
	      C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      posi[0] = inint( xm+2*barlengthx - rect[2]); 
	      posi[1]=inint( ym + 2*barlengthy + rect[3]/2);


	      if(ppsubwin->axes.axes_visible[2] == TRUE){
		C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      }
	      /* grid to put here */
	      if (pSUBWIN_FEATURE (psubwin)->grid[2] > -1)
		{
		  gstyle = pSUBWIN_FEATURE (psubwin)->grid[2];

		  if((ppsubwin->logflags[2] =='l') && (i != nbtics-1))
		    {
		      double tmp[2];
		      double pas=0;
		      double * tmp_log_grads = (double *) NULL;
		      
		      
		      double * grads = ppsubwin->axes.u_zgrads;
		      
		      tmp[0] = exp10(grads[i]);
		      tmp[1] = exp10(grads[i+1]);
		      pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
		      
		      if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			sciprint("Error allocating tmp_log_grads\n");
			return -1;
		      }
		      
		      for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
		      
		      for(j=0;j<nbsubtics;j++)
			{
			  vzz1 = tmp_log_grads[j];
			  
			  if(vzz1<=zminval || vzz1>=zmaxval) continue;	 
			  
			  if(ppsubwin->axes.reverse[2] == TRUE)
			    vzz1 = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],vzz1);
	   			  
			  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1);
			  
			  /*  if ((ym != iybox[3]) && (ym != iybox[2])) */
			  /* 			    { */
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
			  /*   } */
			}
		      FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
		    }
		  else
		    { 
		      if(ztmp>zminval && ztmp<zmaxval) 
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
		  
	      /* and subtics */
	      if(i != nbtics-1)
		{
		  if(ppsubwin->logflags[2] =='l')
		    {
		      double tmp[2];
		      double pas=0;
		      double * tmp_log_grads = (double *) NULL;
		      
		      
		      double * grads = ppsubwin->axes.u_zgrads;
		      
		      tmp[0] = exp10(grads[i]);
		      tmp[1] = exp10(grads[i+1]);
		      pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
		      
		      if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			sciprint("Error allocating tmp_log_grads\n");
			return -1;
		      }
		      
		      for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
		      
		      for(j=0;j<nbsubtics;j++)
			{
			  vzz1 = tmp_log_grads[j];
			  
			  if(vzz1<zminval || vzz1>zmaxval) continue;
			  
			  if(ppsubwin->axes.reverse[2] == TRUE)
			    vzz1 = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],vzz1);
			  
			  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1);
			  
			  vx[0]=xm;vy[0]=ym;
			  vx[1]= (integer) (vx[0]+barlengthx/2.0);
			  vy[1]= (integer) (vy[0]+barlengthy/2.0);
			  
			  if(ppsubwin->axes.axes_visible[2] == TRUE)
			    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			}
		      FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
		    } /* end NEW */
		  else
		    {
		      double xtmp = ppsubwin->axes.u_zgrads[i];
		      double dx = (ppsubwin->axes.u_zgrads[i+1] - ppsubwin->axes.u_zgrads[i]) / nbsubtics;
		      for(j=0;j<nbsubtics;j++)
			{
			  vzz1=xtmp+dx*j;
			  
			  if(vzz1<zminval || vzz1>zmaxval) continue;	 
			  
			  if(ppsubwin->axes.reverse[2] == TRUE)
			    vzz1 = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],vzz1);
			  
			  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1);
			  /* 		      trans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1); */
			  
			  
			  vx[0]=xm;vy[0]=ym;
			  vx[1]= (integer) (vx[0]+barlengthx/2.0);
			  vy[1]= (integer) (vy[0]+barlengthy/2.0);
			  
			  if(ppsubwin->axes.axes_visible[2] == TRUE)
			    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			}
		    } 
		}
	      
	      /***************************************************************/
	      /************************* END OF COMMON PART ******************/
	      /***************************************************************/
	    }
	}
      else /* we display the computed tics */
	{
	  AdaptGraduationsOnZ(x,y,size,Ticsdir,fontid,psubwin,zminval,zmaxval,fx,fy,0.);

	  lastzindex = ppsubwin->axes.nzgrads - 1;
	  
	  if(lastzindex == 0)
	    ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.zgrads[0]));
	  else
	    ChoixFormatE(c_format,
			 ppsubwin->axes.zgrads[0],
			 ppsubwin->axes.zgrads[lastzindex],
			 ((ppsubwin->axes.zgrads[lastzindex])-(ppsubwin->axes.zgrads[0]))/(lastzindex));
	  
	  nbtics = ppsubwin->axes.nzgrads;
	  nbsubtics = ppsubwin->axes.nbsubtics[2];
	  
	  for(i=0;i<nbtics;i++)
	    {
	      char foo[256]; 
	      double ztmp = ppsubwin->axes.zgrads[i];
	      
	      if(ztmp<zminval || ztmp>zmaxval) 
		{
		  /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
		  continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
		  /* donc autant ne pas aller plus loin dans l'algo... */
		}
	      
	      sprintf(foo,c_format,ztmp);

	      /***************************************************************/
	      /************************* COMMON PART *************************/
	      /***************************************************************/
	      if(ppsubwin->axes.reverse[2] == TRUE)
		ztmp = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp);
  
	      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&ztmp);

	      /* 	      trans3d(psubwin,1,&xm,&ym,&fx,&fy,&ztmp); */


	      vx[0]=xm;vy[0]=ym;

	      barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
	      barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
	      vx[1]=vx[0]+barlengthx;
	      vy[1]=vy[0]+barlengthy;
		  
	      /* foo is set above with sprintf(foo,c_format,xtmp); */
	      
	      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
	      C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      posi[0] = inint( xm+2*barlengthx - rect[2]); 
	      posi[1]=inint( ym + 2*barlengthy + rect[3]/2);
	      
	      if(ppsubwin->axes.axes_visible[2] == TRUE){
		C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		if ( ppsubwin->logflags[2] == 'l' )
		  {
		    int smallersize = fontid[1]-2;
		    int old_rect10[4];
/* 		    char str[2] = "xv"; */
		    int posi10[2];
		    
		    posi10[0] = posi[0] - logrect[2];
		    posi10[1] = posi[1] + logrect[3];

		    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xstring","10",(&posi10[0]),(&posi10[1]),PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);

		    C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    
		    posi[0] = old_rect10[0] + old_rect10[2];
		    posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);

		    C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		    
		    /* put back the current fontid */
		    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }
		else{
		  C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		}
		
		C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
		C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      }
	      /* grid to put here */
	      if (pSUBWIN_FEATURE (psubwin)->grid[2] > -1)
		{
		  gstyle = pSUBWIN_FEATURE (psubwin)->grid[2];

		  if((ppsubwin->logflags[2] =='l') && (i != nbtics-1))
		    {
		      double tmp[2];
		      double pas=0;
		      double * tmp_log_grads = (double *) NULL;
		      
		      
		      double * grads = ppsubwin->axes.zgrads;
		      
		      tmp[0] = exp10(grads[i]);
		      tmp[1] = exp10(grads[i+1]);
		      pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
		      
		      if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			sciprint("Error allocating tmp_log_grads\n");
			return -1;
		      }
		      
		      for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
		      
		      for(j=0;j<nbsubtics;j++)
			{
			  vzz1 = tmp_log_grads[j];
			  
			  if(vzz1<=zminval || vzz1>=zmaxval) continue;	 

			  if(ppsubwin->axes.reverse[2] == TRUE)
			    vzz1 = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],vzz1);
			  
			  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1);
			  
			  /*  if ((ym != iybox[3]) && (ym != iybox[2])) */
			  /* 			    { */
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
			  /*   } */
			}
		      FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
		    }
		  else
		    {
		      if(ztmp>zminval && ztmp<zmaxval) 
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
	      
	      /* and subtics */
	      if(i != nbtics-1)
		{
		  if(ppsubwin->logflags[2] =='l')
		    {
		      double tmp[2];
		      double pas=0;
		      double * tmp_log_grads = (double *) NULL;
		      
		      
		      double * grads = ppsubwin->axes.zgrads;
		      
		      tmp[0] = exp10(grads[i]);
		      tmp[1] = exp10(grads[i+1]);
		      pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
		      
		      if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			sciprint("Error allocating tmp_log_grads\n");
			return -1;
		      }
		      
		      for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
		      
		      for(j=0;j<nbsubtics;j++)
			{
			  vzz1 = tmp_log_grads[j];
			  
			  if(vzz1<zminval || vzz1>zmaxval) continue;
			  
			  if(ppsubwin->axes.reverse[2] == TRUE)
			    vzz1 = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],vzz1);
			  
			  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1);
			  
			  vx[0]=xm;vy[0]=ym;
			  vx[1]= (integer) (vx[0]+barlengthx/2.0);
			  vy[1]= (integer) (vy[0]+barlengthy/2.0);
			  
			  if(ppsubwin->axes.axes_visible[2] == TRUE)
			    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			}
		      FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
		    } /* end NEW */
		  else
		    {
		      double ztmp = ppsubwin->axes.zgrads[i];
		      double dz = (ppsubwin->axes.zgrads[i+1] - ppsubwin->axes.zgrads[i]) / nbsubtics;
		      for(j=0;j<nbsubtics;j++)
			{
			  vzz1=ztmp+dz*j;
			  
			  if(vzz1<zminval || vzz1>zmaxval) continue;
			  
			  if(ppsubwin->axes.reverse[2] == TRUE)
			    vzz1 = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],vzz1);
			  
			  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1);
			  /* 		      trans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1); */
			  
			  
			  vx[0]=xm;vy[0]=ym;
			  vx[1]= (integer) (vx[0]+barlengthx/2.0);
			  vy[1]= (integer) (vy[0]+barlengthy/2.0);
			  
			  if(ppsubwin->axes.axes_visible[2] == TRUE)
			    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			}
		    }
		}
	      /***************************************************************/
	      /************************* END OF COMMON PART ******************/
	      /***************************************************************/
	      
	    }
	}
    }
  
  if ((legz != 0) && (sciGetVisibility(ppsubwin->mon_z_label) == TRUE))
    {
      /* draw z label */
      int x1, yy1;
      int rect1[4];

      sciLabel * ppZLabel = pLABEL_FEATURE(ppsubwin->mon_z_label) ;
      
      
      if(ppZLabel->auto_rotation == TRUE){
	angle =  270.;
	sciSetFontOrientation(ppsubwin->mon_z_label,(int)(angle*10));
      }
      else {
	angle = sciGetFontOrientation(ppsubwin->mon_z_label)/10.;
      }
      
      /* F.Leray Adding 1 line here ("xset","pattern") to force the color and style of the */
      /* legend to be the same as those used for the numbers for the axes*/
      
      fontid[0] = sciGetFontStyle(ppsubwin->mon_z_label);
      fontid[1] = sciGetFontDeciWidth(ppsubwin->mon_z_label)/100;
      
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      
      C2F(dr)("xstringl",legz,&zero,&zero,rect1,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      
      cosangle = cos((360-angle)*M_PI/180);
      sinangle = sin((360-angle)*M_PI/180);
      
      if(ppZLabel->auto_position == TRUE){
	x1  = x; /* c est bien le rect[3] relatif au Cscale  <=> rect[3]= Cscale.WIRect1[3]/6; */
	yy1 = round(y+rect1[2]/2);
      }
      else{
	double tmp[2];
	sciGetPosition(ppsubwin->mon_z_label,&tmp[0],&tmp[1]);
	
	x1  = XDouble2Pixel(tmp[0]);
	yy1 = YDouble2Pixel(tmp[1]);
      }
      
      /* new automatic position values */
      sciSetPosition(ppsubwin->mon_z_label,XPixel2Double(x1),YPixel2Double(yy1)); /* the lower left corner of the bounding rectangle */
      
      xm4[0] = x1;
      xm4[1] = round(x1+cosangle*rect1[2]);
      xm4[2] = round(x1+cosangle*rect1[2] + sinangle*(-rect1[3]));
      xm4[3] = round(x1+sinangle*(-rect1[3]));
      
      ym4[0] = yy1;
      ym4[1] = round(yy1-sinangle*rect1[2]);
      ym4[2] = round(yy1-sinangle*rect1[2] + cosangle*(-rect1[3]));
      ym4[3] = round(yy1+cosangle*(-rect1[3]));
      
       
      /* computation of the bounding box even when the string is turned */
      
      largeur = Max(abs(xm4[3] - xm4[1]),abs(xm4[2] - xm4[0]));
      hauteur = Max(abs(ym4[3] - ym4[1]),abs(ym4[2] - ym4[0]));
      
      if(sciGetIsFilled(ppsubwin->mon_z_label) == TRUE){
	int background = sciGetBackground(ppsubwin->mon_z_label);
	int foreground = sciGetForeground(ppsubwin->mon_z_label);
	int n = 4;
	
	C2F(dr)("xset","pattern",&background,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	
	C2F (dr) ("xarea", str, &n, xm4, ym4, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
	
	C2F(dr)("xset","pattern",&foreground,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	
	C2F (dr) ("xlines", "xv", &n, xm4, ym4, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
      }
      
      textcolor = sciGetFontForeground(ppsubwin->mon_z_label);
      
      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      
      xstringb_angle(legz,xm4[0],ym4[0],rect1[2],rect1[3],angle);
      
      textcolor = textcolor_old;
      fontid[0] = fontid_old[0];
      fontid[1] = fontid_old[1];
      
      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  

  /***********************/ /** bottom right side ***/
  /*** le  x-y scaling ***/ /* DISPLAY x or y graduations */
  /***********************/


  if (( xind[4]+xind[5] == 3) || ( xind[4]+xind[5] == 11))
    {
      if (pSUBWIN_FEATURE (psubwin)->project[0]==1) /* x HERE */
	{
	  double fx,fy,fz;
	  char c_format[5];

	  integer Ticsdir[2]; 
	  Ticsdir[0]=ixbox[4]-ixbox[3];
	  Ticsdir[1]=iybox[4]-iybox[3];
      	  BBoxToval(&fx,&fy,&fz,xind[4],bbox);
	  x=inint((ixbox[4]+ixbox[5])/2+1.5*rect[2] +iof);
	  y=inint(((2/3.0)*iybox[4]+(1/3.0)*iybox[5])+1.5*rect[3]+iof);
  	  
	  if(ppsubwin->axes.auto_ticks[0] == FALSE)
	    {
	      /* we display the x tics specified by the user*/
	      nbtics = ppsubwin->axes.u_nxgrads;
	      nbsubtics = ppsubwin->axes.nbsubtics[0];
	      
	      for(i=0;i<nbtics;i++)
		{
		  char *foo = ppsubwin->axes.u_xlabels[i]; 
		  double xtmp = ppsubwin->axes.u_xgrads[i];
		  
		  if(xtmp<xminval || xtmp>xmaxval) 
		    {
		      /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
		      continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
		      /* donc autant ne pas aller plus loin dans l'algo... */
		    }

		  /***************************************************************/
		  /************************* COMMON PART *************************/
		  /***************************************************************/
		  
		  if(ppsubwin->axes.reverse[0] == TRUE)
		    xtmp = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp);
		  		  
		  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz);
		  /*   trans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz); */


		  vx[0]=xm;vy[0]=ym;

		  barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		  barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

		  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
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
		      
		  if(ppsubwin->axes.axes_visible[0] == TRUE){    
		    C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
		    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }
		  /* grid to put here */
		  if (pSUBWIN_FEATURE (psubwin)->grid[0] > -1)
		    {
		      gstyle = pSUBWIN_FEATURE (psubwin)->grid[0];

		      if((ppsubwin->logflags[0] =='l') && (i != nbtics-1))
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.u_xgrads;

			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vxx1 = tmp_log_grads[j];
			      
			      if(vxx1<=xminval || vxx1>=xmaxval) continue;
			      
			      if(ppsubwin->axes.reverse[0] == TRUE)
				vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
			      
			      /*  if ((xm != ixbox[5]) && (xm != ixbox[4])) */
			      /* 				{  */
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
			      /* 	} */
			    }
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			}
		      else
			{
			  if(xtmp>xminval && xtmp<xmaxval) 
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

		  /* and subtics */
		  if(i != nbtics-1)
		    {
		      if(ppsubwin->logflags[0] =='l')
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.u_xgrads;

			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vxx1 = tmp_log_grads[j];
			      
			      if(vxx1<xminval || vxx1>xmaxval) continue;
			      
			      if(ppsubwin->axes.reverse[0] == TRUE)
				vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
			      
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
			      
			      if(ppsubwin->axes.axes_visible[0] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			  
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			} /* end NEW */
		      else
			{
			  double xtmp = ppsubwin->axes.u_xgrads[i];
			  double dx = (ppsubwin->axes.u_xgrads[i+1] - ppsubwin->axes.u_xgrads[i]) / nbsubtics;
			  
			  for (j=1;j<nbsubtics;j++)
			    {  
			      vxx1=xtmp+dx*j;
			      
			      if(vxx1<xminval || vxx1>xmaxval) continue;
			      
			      if(ppsubwin->axes.reverse[0] == TRUE)
				vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
			      /* 			  trans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz); */


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
			      
			      if(ppsubwin->axes.axes_visible[0] == TRUE)   
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			}
		    }
		  /***************************************************************/
		  /************************* END OF COMMON PART ******************/
		  /***************************************************************/
		  
		}
	    }
	  else /* we display the computed tics */
	    {
	      AdaptGraduationsOnXBottomRight(iof,x,y,size,Ticsdir,fontid,psubwin,xminval,xmaxval,0.,fy,fz);
	      	      
	      lastxindex = ppsubwin->axes.nxgrads - 1;
	      
	      if(lastxindex == 0)
		ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.xgrads[0]));
	      else
		ChoixFormatE(c_format,
			     ppsubwin->axes.xgrads[0],
			     ppsubwin->axes.xgrads[lastxindex],
			     ((ppsubwin->axes.xgrads[lastxindex])-(ppsubwin->axes.xgrads[0]))/(lastxindex));
	      
	      nbtics = ppsubwin->axes.nxgrads;
	      nbsubtics = ppsubwin->axes.nbsubtics[0];
	      	      
	      for(i=0;i<nbtics;i++)
		{
		  char foo[256]; 
		  double xtmp = ppsubwin->axes.xgrads[i];
		  
		  if(xtmp<xminval || xtmp>xmaxval) 
		    {
		      /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
		      continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
		      /* donc autant ne pas aller plus loin dans l'algo... */
		    }
		  
		  sprintf(foo,c_format,xtmp);

		  /***************************************************************/
		  /************************* COMMON PART *************************/
		  /***************************************************************/
		 

		  /* F.Leray 03.11.04 Test if log scale to perform a : exp10(x) because trans3d will */
		  /* re-do a log10() (that is needed for data computations) */

		  /* 	  trans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz); */
		  
		  if(ppsubwin->axes.reverse[0] == TRUE)
		    xtmp = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp);
		  
		  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz);
		  
		  vx[0]=xm;vy[0]=ym; 
		  
		  barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		  barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

		  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
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
		      
		  if(ppsubwin->axes.axes_visible[0] == TRUE){
		    C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    if ( ppsubwin->logflags[0] == 'l' )
		      {
			int smallersize = fontid[1]-2;
			int old_rect10[4];
			int posi10[2];
			
			posi10[0] = posi[0] - logrect[2];
			posi10[1] = posi[1] + logrect[3];
			
			C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xstring","10",(&posi10[0]),(&posi10[1]),PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);
			
			C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			
			posi[0] = old_rect10[0] + old_rect10[2];
			posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);
			
			C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
			
			/* put back the current fontid */
			C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		      }
		    else
		      C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		    		    
		    C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
		    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }
		  /* grid to put here */
		  if (pSUBWIN_FEATURE (psubwin)->grid[0] > -1)
		    {
		      gstyle = pSUBWIN_FEATURE (psubwin)->grid[0];
		      
		      if((ppsubwin->logflags[0] =='l') && (i != nbtics-1))
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.xgrads;

			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vxx1 = tmp_log_grads[j];
			      
			      if(vxx1<=xminval || vxx1>=xmaxval) continue;
			      
			      if(ppsubwin->axes.reverse[0] == TRUE)
				vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
			      
			      /*  if ((xm != ixbox[5]) && (xm != ixbox[4])) */
			      /* 				{  */
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
			      /* 	} */
			    }
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			}
		      else
			{
			  if(xtmp>xminval && xtmp<xmaxval) 
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

		  /* and subtics */
		  if(i != nbtics-1) /* F.Leray NEW 03.11.04 */
		    {
		      
		      if(ppsubwin->logflags[0] =='l')
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.xgrads;

			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vxx1 = tmp_log_grads[j];
			      
			      if(vxx1<xminval || vxx1>xmaxval) continue;
			      
			      if(ppsubwin->axes.reverse[0] == TRUE)
				vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
			      
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
			      
			      if(ppsubwin->axes.axes_visible[0] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			  
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			} /* end NEW */
		      else
			{
			  double xtmp = ppsubwin->axes.xgrads[i];
			  double dx = (ppsubwin->axes.xgrads[i+1] - ppsubwin->axes.xgrads[i]) / nbsubtics;
			  
			  for (j=1;j<nbsubtics;j++)
			    {  
			      vxx1=xtmp+dx*j;
			      
			      if(vxx1<xminval || vxx1>xmaxval) continue;
			      
			      if(ppsubwin->axes.reverse[0] == TRUE)
				vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);

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
			  
			      if(ppsubwin->axes.axes_visible[0] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			}
		    }
		
		  /***************************************************************/
		  /************************* END OF COMMON PART ******************/
		  /***************************************************************/
		  
		}
	    }
	}
      if (legx != 0 && (sciGetVisibility(ppsubwin->mon_x_label) == TRUE))
	{
	  int x1, yy1;
	  if(pLABEL_FEATURE(ppsubwin->mon_x_label)->auto_rotation == TRUE){
	    angle =  0.;
	    sciSetFontOrientation(ppsubwin->mon_x_label,(int)(angle*10));
	  }
	  else 
	    angle = sciGetFontOrientation(ppsubwin->mon_x_label)/10.;
	  
	  /* F.Leray Adding 1 line here ("xset","pattern") to force the color and style of the */
	  /* legend to be the same as those used for the numbers for the axes */
	  
	  fontid[0] = sciGetFontStyle(ppsubwin->mon_x_label);
	  fontid[1] = sciGetFontDeciWidth(ppsubwin->mon_x_label)/100;
	  
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  C2F(dr)("xstringl",legx,&zero,&zero,rect1,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  cosangle = cos((360-angle)*M_PI/180);
	  sinangle = sin((360-angle)*M_PI/180);
	  
	  if(pLABEL_FEATURE(ppsubwin->mon_x_label)->auto_position == TRUE){
	    x1  = x;
	    yy1 = y;
	  }
	  else{
	    double tmp[2];
	    sciGetPosition(ppsubwin->mon_x_label,&tmp[0],&tmp[1]);
	    
	    x1  = XDouble2Pixel(tmp[0]);
	    yy1 = YDouble2Pixel(tmp[1]);
	  }
	  
	  /* new automatic position values */
	  sciSetPosition(ppsubwin->mon_x_label,XPixel2Double(x1),YPixel2Double(yy1)); /* the lower left corner of the bounding rectangle */
	  
	  xm4[0] = x1;
	  xm4[1] = round(x1+cosangle*rect1[2]);
	  xm4[2] = round(x1+cosangle*rect1[2] + sinangle*(-rect1[3]));
	  xm4[3] = round(x1+sinangle*(-rect1[3]));
	  
	  ym4[0] = yy1;
	  ym4[1] = round(yy1-sinangle*rect1[2]);
	  ym4[2] = round(yy1-sinangle*rect1[2] + cosangle*(-rect1[3]));
	  ym4[3] = round(yy1+cosangle*(-rect1[3]));
	  
	  /* computation of the bounding box even when the string is turned */
	  
	  largeur = Max(abs(xm4[3] - xm4[1]),abs(xm4[2] - xm4[0]));
	  hauteur = Max(abs(ym4[3] - ym4[1]),abs(ym4[2] - ym4[0]));
	  
	  if(sciGetIsFilled(ppsubwin->mon_x_label) == TRUE){
	    int background = sciGetBackground(ppsubwin->mon_x_label);
	    int foreground = sciGetForeground(ppsubwin->mon_x_label);
	    int n = 4;
	    
	    C2F(dr)("xset","pattern",&background,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    
	    C2F (dr) ("xarea", str, &n, xm4, ym4, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
	    
	    C2F(dr)("xset","pattern",&foreground,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    
	    C2F (dr) ("xlines", "xv", &n, xm4, ym4, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
	  }
	  
	  textcolor = sciGetFontForeground(ppsubwin->mon_x_label);
	  
	  C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  xstringb_angle(legx,xm4[0],ym4[0],rect1[2],rect1[3],angle);
	  
	  textcolor = textcolor_old;
	  fontid[0] = fontid_old[0];
	  fontid[1] = fontid_old[1];

	  C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
    }
  else
    {
      if ( pSUBWIN_FEATURE (psubwin)->project[1]==1) /* y is HERE */
	{
	  double fx,fy,fz; 
	  char c_format[5];
      
	  integer Ticsdir[2];
	  Ticsdir[0]=ixbox[4]-ixbox[3];
	  Ticsdir[1]=iybox[4]-iybox[3];
	  BBoxToval(&fx,&fy,&fz,xind[4],bbox);
	  
	  x=inint((ixbox[4]+ixbox[5])/2+1.5*rect[2] +iof);
	  y=inint(((2/3.0)*iybox[4]+(1/3.0)*iybox[5])+1.5*rect[3]+iof);

	  if(ppsubwin->axes.auto_ticks[1] == FALSE)
	    {
	      /* we display the y tics specified by the user*/
	      nbtics = ppsubwin->axes.u_nygrads;
	      nbsubtics = ppsubwin->axes.nbsubtics[1];
	      
	      for(i=0;i<nbtics;i++)
		{
		  char *foo = ppsubwin->axes.u_ylabels[i]; 
		  double ytmp = ppsubwin->axes.u_ygrads[i];
		  
		  if(ytmp<yminval || ytmp>ymaxval) 
		    {
		      /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
		      continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
		      /* donc autant ne pas aller plus loin dans l'algo... */
		    }
	      	  

		  /***************************************************************/
		  /************************* COMMON PART *************************/
		  /***************************************************************/
		  
		  if(ppsubwin->axes.reverse[1] == TRUE)
		    ytmp = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp);
  
		  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz);
		  /* 		  trans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz); */
		  


		  vx[0]=xm;vy[0]=ym;

		  barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		  barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		  /* 	NumberFormat(foo,((integer) (yy[0] + i*ceil((yy[1]-yy[0])/yy[3]))), */
		  /* 			     ((integer) yy[2])); */
		  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
		  C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  if (IsDownAxes(psubwin)){
		    vx[1]=vx[0];
		    vy[1]=vy[0]+iof/2;
		    posi[0] = inint(xm-rect[2]/2); 
		    posi[1]=inint( vy[0] + iof + rect[3]);}
		  else{ 
		    vx[1]=vx[0]+barlengthx;
		    vy[1]=vy[0]+barlengthy;
/* 		    posi[0] = inint( xm+2*barlengthx - rect[2]/2); */
/* 		    posi[0] = inint( xm+2*barlengthx - rect[2]); */
		    posi[0] = inint( xm+2*barlengthx);
		    posi[1]=inint( ym + 2*barlengthy + rect[3]);}

		  if(ppsubwin->axes.axes_visible[1] == TRUE){
		    C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
		    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }
		  /* grid to put here */
		  if (pSUBWIN_FEATURE (psubwin)->grid[1] > -1)
		    {
		      gstyle = pSUBWIN_FEATURE (psubwin)->grid[1];

		      if((ppsubwin->logflags[1] =='l') && (i != nbtics-1))
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.u_ygrads;

			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vyy1 = tmp_log_grads[j];
			      
			      if(vyy1<=yminval || vyy1>=ymaxval) continue;
			      
			      if(ppsubwin->axes.reverse[1] == TRUE)
				vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);

			      /*  if ((xm != ixbox[5]) && (xm != ixbox[4])) */
			      /* 				{  */
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
			      /* 	} */
			    }
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			}
		      else
			{
			  if(ytmp>yminval && ytmp<ymaxval) 
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

		  /* and subtics */
		  if(i != nbtics-1)
		    {
		      if(ppsubwin->logflags[1] =='l')
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.u_ygrads;
			  
			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vyy1 = tmp_log_grads[j];
			      
			      if(vyy1<yminval || vyy1>ymaxval) continue;
			      
			      if(ppsubwin->axes.reverse[1] == TRUE)
				vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
			      
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
			      
			      if(ppsubwin->axes.axes_visible[1] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			  
			} /* end NEW */
		      else
			{
			  double ytmp = ppsubwin->axes.u_ygrads[i];
			  double dy = (ppsubwin->axes.u_ygrads[i+1] - ppsubwin->axes.u_ygrads[i]) / nbsubtics;
			  for(j=0;j<nbsubtics;j++)
			    {
			      vyy1=ytmp+dy*j;
			      
			      if(vyy1<yminval || vyy1>ymaxval) continue;
			      
			      if(ppsubwin->axes.reverse[1] == TRUE)
				vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
			      			      
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
			      
			      if(ppsubwin->axes.axes_visible[1] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			}
		    }
		  
		  /***************************************************************/
		  /************************* END OF COMMON PART ******************/
		  /***************************************************************/
		}
	    }
	  else /* we display the computed tics */
	    {
	      AdaptGraduationsOnYBottomRight(iof,x,y,size,Ticsdir,fontid,psubwin,yminval,ymaxval,fx,0.,fz);

	      lastyindex = ppsubwin->axes.nygrads - 1;
	      
	      if(lastyindex == 0)
		ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.ygrads[0]));
	      else
		ChoixFormatE(c_format,
			     ppsubwin->axes.ygrads[0],
			     ppsubwin->axes.ygrads[lastyindex],
			     ((ppsubwin->axes.ygrads[lastyindex])-(ppsubwin->axes.ygrads[0]))/(lastyindex));
	      
	      nbtics = ppsubwin->axes.nygrads;
	      nbsubtics = ppsubwin->axes.nbsubtics[1];
	      
	      for(i=0;i<nbtics;i++)
		{
		  char foo[256]; 
		  double ytmp = ppsubwin->axes.ygrads[i];
	      
		  if(ytmp<yminval || ytmp>ymaxval) 
		    {
		      /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
		      continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
		      /* donc autant ne pas aller plus loin dans l'algo... */
		    }
	      
		  sprintf(foo,c_format,ytmp);

		  /***************************************************************/
		  /************************* COMMON PART *************************/
		  /***************************************************************/

		  /* F.Leray 03.11.04 Test if log scale to perform a : exp10(x) because trans3d will */
		  /* re-do a log10() (that is needed for data computations) */


		  if(ppsubwin->axes.reverse[1] == TRUE)
		    ytmp = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp);
		  
		  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz);
		  
		  
		  vx[0]=xm;vy[0]=ym;
	      
		  barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		  barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		    
		  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
		  C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  if (IsDownAxes(psubwin)){
		    vx[1]=vx[0];
		    vy[1]=vy[0]+iof/2;
		    posi[0] = inint(xm-rect[2]/2); 
		    posi[1]=inint( vy[0] + iof + rect[3]);}
		  else{ 
		    vx[1]=vx[0]+barlengthx;
		    vy[1]=vy[0]+barlengthy;
/* 		    posi[0] = inint( xm+2*barlengthx - rect[2]/2); */
/* 		    posi[0] = inint( xm+2*barlengthx - rect[2]); */
		    posi[0] = inint( xm+2*barlengthx);
		    posi[1]=inint( ym + 2*barlengthy + rect[3]);}
		  
		  if(ppsubwin->axes.axes_visible[1] == TRUE){
		    C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    if ( ppsubwin->logflags[1] == 'l' )
		      {
			int smallersize = fontid[1]-2;
			int old_rect10[4];
			/* 		    char str[2] = "xv"; */
			int posi10[2];
			
			posi10[0] = posi[0] - logrect[2];
			posi10[1] = posi[1] + logrect[3];
			
			C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xstring","10",(&posi10[0]),(&posi10[1]),PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);
			
			C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			
			posi[0] = old_rect10[0] + old_rect10[2];
			posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);
			
			C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
			
			/* put back the current fontid */
			C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		      }
		    else
		      C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		    
		    C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
		    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }
		  
		  /* grid to put here */
		  
		  if (pSUBWIN_FEATURE (psubwin)->grid[1] > -1)
		    {
		      gstyle = pSUBWIN_FEATURE (psubwin)->grid[1];

		      if((ppsubwin->logflags[1] =='l') && (i != nbtics-1))
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.ygrads;

			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vyy1 = tmp_log_grads[j];
			      
			      if(vyy1<=yminval || vyy1>=ymaxval) continue;
			      
			      if(ppsubwin->axes.reverse[1] == TRUE)
				vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
			      
			      /*  if ((xm != ixbox[5]) && (xm != ixbox[4])) */
			      /* 				{  */
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
			      /* 	} */
			    }
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			}
		      else
			{
			  if(ytmp>yminval && ytmp<ymaxval) 
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

		  /* and subtics */
		  if(i != nbtics-1)
		    {
		      if(ppsubwin->logflags[1] =='l')
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.ygrads;
			  
			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vyy1 = tmp_log_grads[j];
			      
			      if(vyy1<yminval || vyy1>ymaxval) continue;
			      
			      if(ppsubwin->axes.reverse[1] == TRUE)
				vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
			      
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
			      
			      if(ppsubwin->axes.axes_visible[1] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			  
			} /* end NEW */
		      else
			{
			  double ytmp = ppsubwin->axes.ygrads[i];
			  double dy = (ppsubwin->axes.ygrads[i+1] - ppsubwin->axes.ygrads[i]) / nbsubtics;
			  for(j=0;j<nbsubtics;j++)
			    {
			      vyy1=ytmp+dy*j;
			  
			      if(vyy1<yminval || vyy1>ymaxval) continue;
			      
			      if(ppsubwin->axes.reverse[1] == TRUE)
				vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
			      /*   trans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz); */
			  
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

			      if(ppsubwin->axes.axes_visible[1] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			}
		    }
		  /***************************************************************/
		  /************************* END OF COMMON PART ******************/
		  /***************************************************************/
	      
		}
	    }
	}
      if (legy != 0 && (sciGetVisibility(ppsubwin->mon_y_label) == TRUE))
	{ 
	  int x1, yy1;
	  if(pLABEL_FEATURE(ppsubwin->mon_y_label)->auto_rotation == TRUE){
	    angle =  0.;
	    sciSetFontOrientation(ppsubwin->mon_y_label,(int)angle);
	  }
	  else 
	    angle = sciGetFontOrientation(ppsubwin->mon_y_label)/10.;
	  
	  /* F.Leray Adding 1 line here ("xset","pattern") to force the color and style of the */
	  /* legend to be the same as those used for the numbers for the axes */
	  
	  fontid[0] = sciGetFontStyle(ppsubwin->mon_y_label);
	  fontid[1] = sciGetFontDeciWidth(ppsubwin->mon_y_label)/100;
	  
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  C2F(dr)("xstringl",legy,&zero,&zero,rect1,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  cosangle = cos((360-angle)*M_PI/180);
	  sinangle = sin((360-angle)*M_PI/180);
	  
	  if(pLABEL_FEATURE(ppsubwin->mon_y_label)->auto_position == TRUE){
	    x1  = x;
	    yy1 = y;
	  }
	  else{
	    double tmp[2];
	    sciGetPosition(ppsubwin->mon_y_label,&tmp[0],&tmp[1]);
	    
	    x1  = XDouble2Pixel(tmp[0]);
	    yy1 = YDouble2Pixel(tmp[1]);
	  }
	  
	  /* new automatic position values */
	  sciSetPosition(ppsubwin->mon_y_label,XPixel2Double(x1),YPixel2Double(yy1)); /* the lower left corner of the bounding rectangle */
	  
	  xm4[0] = x1;
	  xm4[1] = round(x1+cosangle*rect1[2]);
	  xm4[2] = round(x1+cosangle*rect1[2] + sinangle*(-rect1[3]));
	  xm4[3] = round(x1+sinangle*(-rect1[3]));
	  
	  ym4[0] = yy1;
	  ym4[1] = round(yy1-sinangle*rect1[2]);
	  ym4[2] = round(yy1-sinangle*rect1[2] + cosangle*(-rect1[3]));
	  ym4[3] = round(yy1+cosangle*(-rect1[3]));
	  
	  /* computation of the bounding box even when the string is turned */
	  
	  largeur = Max(abs(xm4[3] - xm4[1]),abs(xm4[2] - xm4[0]));
	  hauteur = Max(abs(ym4[3] - ym4[1]),abs(ym4[2] - ym4[0]));
	  
	  if(sciGetIsFilled(ppsubwin->mon_y_label) == TRUE){
	    int background = sciGetBackground(ppsubwin->mon_y_label);
	    int foreground = sciGetForeground(ppsubwin->mon_y_label);
	    int n = 4;
	    
	    C2F(dr)("xset","pattern",&background,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    
	    C2F (dr) ("xarea", str, &n, xm4, ym4, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
	    
	    C2F(dr)("xset","pattern",&foreground,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    
	    C2F (dr) ("xlines", "xv", &n, xm4, ym4, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
	  }
	  
	  textcolor = sciGetFontForeground(ppsubwin->mon_y_label);
	  
	  C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  xstringb_angle(legy,xm4[0],ym4[0],rect1[2],rect1[3],angle);
	  
	  textcolor = textcolor_old;
	  fontid[0] = fontid_old[0];
	  fontid[1] = fontid_old[1];

	  C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
    }
  
  
  
  /***********************/ /** bottom left side ***/
  /*** le  x-y scaling ***/ /* DISPLAY x or y graduations */
  /***********************/
  
  if (( xind[3]+xind[4] == 3) || ( xind[3]+xind[4] == 11))
    {
      if (pSUBWIN_FEATURE (psubwin)->project[0]==1) /* x HERE */
	{
	  double fx,fy,fz;
	  char c_format[5];

	  
	  integer Ticsdir[2];
	  Ticsdir[0]=ixbox[4]-ixbox[5];
	  Ticsdir[1]=iybox[4]-iybox[5];
	  BBoxToval(&fx,&fy,&fz,xind[3],bbox);

	  x=inint((ixbox[3]+ixbox[4])/2.0 -rect[2] -iof);
	  y=inint((1/3.0)*iybox[3]+(2/3.0)*iybox[4]+ iof+ 1.5*rect[3]); 
	  
	  if(ppsubwin->axes.auto_ticks[0] == FALSE)
	    {
	      /* we display the x tics specified by the user*/
	      nbtics = ppsubwin->axes.u_nxgrads;
	      nbsubtics = ppsubwin->axes.nbsubtics[0];
	      
	      for(i=0;i<nbtics;i++)
		{
		  char *foo = ppsubwin->axes.u_xlabels[i]; 
		  double xtmp = ppsubwin->axes.u_xgrads[i];
		  
		  if(xtmp<xminval || xtmp>xmaxval) 
		    {
		      /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
		      continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
		      /* donc autant ne pas aller plus loin dans l'algo... */
		    }
		  
		  /***************************************************************/
		  /************************* COMMON PART *************************/
		  /***************************************************************/

		  if(ppsubwin->axes.reverse[0] == TRUE)
		    xtmp = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp);
		  
		  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz);
		  /* 		  trans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz); */


		  vx[0]=xm;vy[0]=ym;

		  barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		  barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		
		  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
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

		  if(ppsubwin->axes.axes_visible[0] == TRUE){
		    C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
		    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }
		  /* grid to put here */
		  if (pSUBWIN_FEATURE (psubwin)->grid[0] > -1)
		    {
		      gstyle = pSUBWIN_FEATURE (psubwin)->grid[0];

		      if((ppsubwin->logflags[0] =='l') && (i != nbtics-1))
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.u_xgrads;
			  
			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vxx1 = tmp_log_grads[j];
			      
			      if(vxx1<=xminval || vxx1>=xmaxval) continue;

			      if(ppsubwin->axes.reverse[0] == TRUE)
				vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);

			      /*   if ((xm != ixbox[3]) && (xm != ixbox[4])) */
			      /* 				{  */
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
			      /* 	} */
			    }
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			}
		      else
			{
			  if(xtmp>xminval && xtmp<xmaxval) 
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

		  /* and subtics */
		  if(i != nbtics-1)
		    {
		      if(ppsubwin->logflags[0] =='l')
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.u_xgrads;
			  
			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vxx1 = tmp_log_grads[j];
			      
			      if(vxx1<xminval || vxx1>xmaxval) continue;
			      
			      if(ppsubwin->axes.reverse[0] == TRUE)
				vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
			      
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
			      
			      if(ppsubwin->axes.axes_visible[0] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }

			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			} /* end NEW */
		      else
			{
			  double xtmp = ppsubwin->axes.u_xgrads[i];
			  double dx = (ppsubwin->axes.u_xgrads[i+1] - ppsubwin->axes.u_xgrads[i]) / nbsubtics;
			  
			  for (j=1;j<nbsubtics;j++)
			    {  
			      vxx1=xtmp+dx*j;
			      
			      if(vxx1<xminval || vxx1>xmaxval) continue;

			      if(ppsubwin->axes.reverse[0] == TRUE)
				vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
			      /* 		  trans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz); */


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

			      if(ppsubwin->axes.axes_visible[0] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			}
		    }
		  /***************************************************************/
		  /************************* COMMON PART *************************/
		  /***************************************************************/
		  

		}
	      
	    }
	  else /* we display the computed tics */
	    {
	      AdaptGraduationsOnXBottomLeft(iof,x,y,size,Ticsdir,fontid,psubwin,xminval,xmaxval,0.,fy,fz);
	      
	      lastxindex = ppsubwin->axes.nxgrads - 1;
	      
	      if(lastxindex == 0)
		ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.xgrads[0]));
	      else
		ChoixFormatE(c_format,
			     ppsubwin->axes.xgrads[0],
			     ppsubwin->axes.xgrads[lastxindex],
			     ((ppsubwin->axes.xgrads[lastxindex])-(ppsubwin->axes.xgrads[0]))/(lastxindex));
	      
	      nbtics = ppsubwin->axes.nxgrads;
	      nbsubtics = ppsubwin->axes.nbsubtics[0];
	           
	      for(i=0;i<nbtics;i++)
		{
		  char foo[256]; 
		  double xtmp = ppsubwin->axes.xgrads[i];
		  
		  if(xtmp<xminval || xtmp>xmaxval) 
		    {
		      /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
		      continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
		      /* donc autant ne pas aller plus loin dans l'algo... */
		    }
		  
		  sprintf(foo,c_format,xtmp);
		  
		  /***************************************************************/
		  /************************* COMMON PART *************************/
		  /***************************************************************/

		  if(ppsubwin->axes.reverse[0] == TRUE)
		    xtmp = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp);
		  
		  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz);

		  vx[0]=xm;vy[0]=ym;

		  barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		  barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		     
		  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
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

		  if(ppsubwin->axes.axes_visible[0] == TRUE){
		    C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    if ( ppsubwin->logflags[0] == 'l' )
		      {
			int smallersize = fontid[1]-2;
			int old_rect10[4];
			int posi10[2];
			
			posi10[0] = posi[0] - logrect[2];
			posi10[1] = posi[1] + logrect[3];
			
			C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xstring","10",(&posi10[0]),(&posi10[1]),PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);
			
			C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			
			posi[0] = old_rect10[0] + old_rect10[2];
			posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);
			
			C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
			
			/* put back the current fontid */
			C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		      }
		    else
		      C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);

		    C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
		    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }
		  /* grid to put here */
		  if (pSUBWIN_FEATURE (psubwin)->grid[0] > -1)
		    {
		      gstyle = pSUBWIN_FEATURE (psubwin)->grid[0];
		      
		      if((ppsubwin->logflags[0] =='l') && (i != nbtics-1))
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.xgrads;
			  
			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vxx1 = tmp_log_grads[j];
			      
			      if(vxx1<=xminval || vxx1>=xmaxval) continue;
			      
			      if(ppsubwin->axes.reverse[0] == TRUE)
				vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
			      
			      /*    if ((xm != ixbox[3]) && (xm != ixbox[4])) */
			      /* 				{  */
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
			      /* 	} */
			    }
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			}
		      else
			{
			  if(xtmp>xminval && xtmp<xmaxval) 
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
		  
		  /* and subtics */
		  if(i != nbtics-1)
		    {
		      if(ppsubwin->logflags[0] =='l')
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.xgrads;

			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vxx1 = tmp_log_grads[j];
			      
			      if(vxx1<xminval || vxx1>xmaxval) continue;
			      
			      if(ppsubwin->axes.reverse[0] == TRUE)
				vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
			      
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
			      
			      if(ppsubwin->axes.axes_visible[0] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			} /* end NEW */
		      else
			{
			  double xtmp = ppsubwin->axes.xgrads[i];
			  double dx = (ppsubwin->axes.xgrads[i+1] - ppsubwin->axes.xgrads[i]) / nbsubtics;
			  
			  for (j=1;j<nbsubtics;j++)
			    {  
			      vxx1=xtmp+dx*j;
			      
			      if(vxx1<xminval || vxx1>xmaxval) continue;
			      
			      if(ppsubwin->axes.reverse[0] == TRUE)
				vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
			      /* 		  trans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz); */
			      
			      
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
			      
			      if(ppsubwin->axes.axes_visible[0] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			}
		    }
		  /***************************************************************/
		  /************************* COMMON PART *************************/
		  /***************************************************************/
		  
		}
	    }
	}

      if (legx != 0&& (sciGetVisibility(ppsubwin->mon_x_label) == TRUE))
	{ 
	  int x1, yy1;
	  if(pLABEL_FEATURE(ppsubwin->mon_x_label)->auto_rotation == TRUE){
	    angle =  0.;
	    sciSetFontOrientation(ppsubwin->mon_x_label,(int)angle);
	  }
	  else 
	    angle = sciGetFontOrientation(ppsubwin->mon_x_label)/10.;
	  
	  /* F.Leray Adding 1 line here ("xset","pattern") to force the color and style of the */
	  /* legend to be the same as those used for the numbers for the axes*/
	   
	  fontid[0] = sciGetFontStyle(ppsubwin->mon_x_label);
	  fontid[1] = sciGetFontDeciWidth(ppsubwin->mon_x_label)/100;
	  
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  C2F(dr)("xstringl",legx,&zero,&zero,rect1,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  cosangle = cos((360-angle)*M_PI/180);
	  sinangle = sin((360-angle)*M_PI/180);
	  
	  if(pLABEL_FEATURE(ppsubwin->mon_x_label)->auto_position == TRUE){
	    x1  = x-rect1[2];
	    yy1 = y;
	  }
	  else{
	    double tmp[2];
	    sciGetPosition(ppsubwin->mon_x_label,&tmp[0],&tmp[1]);
	    
	    x1  = XDouble2Pixel(tmp[0]);
	    yy1 = YDouble2Pixel(tmp[1]);
	  }
	  
	  /* new automatic position values */
	  sciSetPosition(ppsubwin->mon_x_label,XPixel2Double(x1),YPixel2Double(yy1)); /* the lower left corner of the bounding rectangle */
	  
	  xm4[0] = x1;
	  xm4[1] = round(x1+cosangle*rect1[2]);
	  xm4[2] = round(x1+cosangle*rect1[2] + sinangle*(-rect1[3]));
	  xm4[3] = round(x1+sinangle*(-rect1[3]));
	  
	  ym4[0] = yy1;
	  ym4[1] = round(yy1-sinangle*rect1[2]);
	  ym4[2] = round(yy1-sinangle*rect1[2] + cosangle*(-rect1[3]));
	  ym4[3] = round(yy1+cosangle*(-rect1[3]));
	  
	  /* computation of the bounding box even when the string is turned */
	  
	  largeur = Max(abs(xm4[3] - xm4[1]),abs(xm4[2] - xm4[0]));
	  hauteur = Max(abs(ym4[3] - ym4[1]),abs(ym4[2] - ym4[0]));
	  
	  if(sciGetIsFilled(ppsubwin->mon_x_label) == TRUE){
	    int background = sciGetBackground(ppsubwin->mon_x_label);
	    int foreground = sciGetForeground(ppsubwin->mon_x_label);
	    int n = 4;
	    
	    C2F(dr)("xset","pattern",&background,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    
	    C2F (dr) ("xarea", str, &n, xm4, ym4, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
	    
	    C2F(dr)("xset","pattern",&foreground,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    
	    C2F (dr) ("xlines", "xv", &n, xm4, ym4, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
	  }
	  
	  textcolor = sciGetFontForeground(ppsubwin->mon_x_label);
	  
	  C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  xstringb_angle(legx,xm4[0],ym4[0],rect1[2],rect1[3],angle);
	  
	  textcolor = textcolor_old;
	  fontid[0] = fontid_old[0];
	  fontid[1] = fontid_old[1];
	  
	  C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
    }
  else 
    {
      if  (pSUBWIN_FEATURE (psubwin)->project[1]==1) /* y is HERE */
	{
	  double fx,fy,fz;
	  char c_format[5];

	  integer Ticsdir[2];
	  Ticsdir[0]=ixbox[4]-ixbox[5];
	  Ticsdir[1]=iybox[4]-iybox[5];
	  BBoxToval(&fx,&fy,&fz,xind[3],bbox);
	  
	  x=inint((ixbox[3]+ixbox[4])/2.0 -rect[2] -iof);
	  y=inint((1/3.0)*iybox[3]+(2/3.0)*iybox[4]+ iof + 1.5*rect[3]);  
	  
	  if(ppsubwin->axes.auto_ticks[1] == FALSE)
	    {
	      /* we display the y tics specified by the user*/
	      nbtics = ppsubwin->axes.u_nygrads;
	      nbsubtics = ppsubwin->axes.nbsubtics[1];
	      
	      for(i=0;i<nbtics;i++)
		{
		  char *foo = ppsubwin->axes.u_ylabels[i]; 
		  double ytmp = ppsubwin->axes.u_ygrads[i];
		  
		  if(ytmp<yminval || ytmp>ymaxval) 
		    {
		      /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
		      continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
		      /* donc autant ne pas aller plus loin dans l'algo... */
		    }
	      	  
		  /***************************************************************/
		  /************************* COMMON PART *************************/
		  /***************************************************************/

		  if(ppsubwin->axes.reverse[1] == TRUE)
		    ytmp = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp);
		  
		  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz);
		  /* 		  trans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz); */


		  vx[0]=xm;vy[0]=ym; 
		  
		  barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		  barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		     
		  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
		  C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  if (IsDownAxes(psubwin)){
		    vx[1]=vx[0];
		    vy[1]=vy[0]+iof/2;
		    posi[0] = inint(xm-rect[2]/2); 
		    posi[1]=inint( vy[0] + iof + rect[3]);}
		  else{
		    vx[1]=vx[0]+barlengthx;
		    vy[1]=vy[0]+barlengthy;
/* 		    posi[0] = inint( xm+2*barlengthx-rect[2]/2);  */
		    posi[0] = inint( xm+2*barlengthx-rect[2]); 
		    posi[1]=inint( ym + 2*barlengthy + rect[3]);}
		      

		  if(ppsubwin->axes.axes_visible[1] == TRUE){
		    C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
		    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }
		  /* grid to put here */
		  if (pSUBWIN_FEATURE (psubwin)->grid[1] > -1)
		    {
		      gstyle = pSUBWIN_FEATURE (psubwin)->grid[1];
		      
		      if((ppsubwin->logflags[1] =='l') && (i != nbtics-1))
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.u_ygrads;
			  
			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vyy1 = tmp_log_grads[j];
			      
			      if(vyy1<=yminval || vyy1>=ymaxval) continue;
			      
			      if(ppsubwin->axes.reverse[1] == TRUE)
				vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
			      
			      /*   if ((xm != ixbox[3]) && (xm != ixbox[4])) */
			      /* 				{ */ 
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
			      /* 	} */
			    }
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			}
		      else
			{
			  if(ytmp>yminval && ytmp<ymaxval) 
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
		    
		  /* and subtics */
		  if(i != nbtics-1)
		    {
		      if(ppsubwin->logflags[1] =='l')
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.u_ygrads;
			  
			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vyy1 = tmp_log_grads[j];
			      
			      if(vyy1<yminval || vyy1>ymaxval) continue;
			      
			      if(ppsubwin->axes.reverse[1] == TRUE)
				vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
			      
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
			      
			      if(ppsubwin->axes.axes_visible[1] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			  
			} /* end NEW */
		      else
			{
			  
			  double ytmp = ppsubwin->axes.u_ygrads[i];
			  double dy = (ppsubwin->axes.u_ygrads[i+1] - ppsubwin->axes.u_ygrads[i]) / nbsubtics;
			  for(j=0;j<nbsubtics;j++)
			    {
			      vyy1=ytmp+dy*j;
			      
			      if(vyy1<yminval || vyy1>ymaxval) continue;
			      
			      if(ppsubwin->axes.reverse[1] == TRUE)
				vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
			      /* 		  trans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz); */
			      
			      
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
			      
			      if(ppsubwin->axes.axes_visible[1] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			}
		    }
		  /***************************************************************/
		  /************************* COMMON PART *************************/
		  /***************************************************************/
		  
		}
	    }
	  else /* we display the computed tics */
	    {
	      AdaptGraduationsOnYBottomLeft(iof,x,y,size,Ticsdir,fontid,psubwin,yminval,ymaxval,fx,0.,fz);
	      
	      lastyindex = ppsubwin->axes.nygrads - 1;

	      if(lastyindex == 0)
		ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.ygrads[0]));
	      else
		ChoixFormatE(c_format,
			     ppsubwin->axes.ygrads[0],
			     ppsubwin->axes.ygrads[lastyindex],
			     ((ppsubwin->axes.ygrads[lastyindex])-(ppsubwin->axes.ygrads[0]))/(lastyindex));
	      
	      nbtics = ppsubwin->axes.nygrads;
	      nbsubtics = ppsubwin->axes.nbsubtics[1];
	      
	      for(i=0;i<nbtics;i++)
		{
		  char foo[256]; 
		  double ytmp = ppsubwin->axes.ygrads[i];
	      
		  if(ytmp<yminval || ytmp>ymaxval) 
		    {
		      /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
		      continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
		      /* donc autant ne pas aller plus loin dans l'algo... */
		    }
		  
		  sprintf(foo,c_format,ytmp);
		  
		  /***************************************************************/
		  /************************* COMMON PART *************************/
		  /***************************************************************/

		  if(ppsubwin->axes.reverse[1] == TRUE)
		    ytmp = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp);
		  
		  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz);
		  /* 		  trans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz); */


		  vx[0]=xm;vy[0]=ym; 
		  
		  barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		  barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		  
		  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
		  C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  
		  if (IsDownAxes(psubwin)){
		    vx[1]=vx[0];
		    vy[1]=vy[0]+iof/2;
		    posi[0] = inint(xm-rect[2]/2); 
		    posi[1]=inint( vy[0] + iof + rect[3]);}
		  else{
		    vx[1]=vx[0]+barlengthx;
		    vy[1]=vy[0]+barlengthy;
/* 		    posi[0] = inint( xm+2*barlengthx-rect[2]/2);  */
		    posi[0] = inint( xm+2*barlengthx-rect[2]); 
		    posi[1]=inint( ym + 2*barlengthy + rect[3]);}
		      
		  if(ppsubwin->axes.axes_visible[1] == TRUE){
		    C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    if ( ppsubwin->logflags[1] == 'l' )
		      {
			int smallersize = fontid[1]-2;
			int old_rect10[4];
			/* 		    char str[2] = "xv"; */
			int posi10[2];
			
			posi10[0] = posi[0] - logrect[2];
			posi10[1] = posi[1] + logrect[3];
			
			C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xstring","10",(&posi10[0]),(&posi10[1]),PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);
			
			C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			
			posi[0] = old_rect10[0] + old_rect10[2];
			posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);
			
			C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
			
			/* put back the current fontid */
			C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		      }
		    else
		      C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		    
		    C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
		    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }
		  /* grid to put here */
		  if (pSUBWIN_FEATURE (psubwin)->grid[1] > -1)
		    {
		      gstyle = pSUBWIN_FEATURE (psubwin)->grid[1];

		      if((ppsubwin->logflags[1] =='l') && (i != nbtics-1))
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.ygrads;
			  
			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vyy1 = tmp_log_grads[j];
			      
			      if(vyy1<=yminval || vyy1>=ymaxval) continue;
			      
			      if(ppsubwin->axes.reverse[1] == TRUE)
				vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
			      
			      /*    if ((xm != ixbox[3]) && (xm != ixbox[4])) */
			      /* 				{  */
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
			      /* 	} */
			    }
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			}
		      else
			{
			  if(ytmp>yminval && ytmp<ymaxval) 
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
		  
		  /* and subtics */
		  if(i != nbtics-1)
		    {
		      if(ppsubwin->logflags[1] =='l')
			{
			  double tmp[2];
			  double pas=0;
			  double * tmp_log_grads = (double *) NULL;
			  
			  
			  double * grads = ppsubwin->axes.ygrads;
			  
			  tmp[0] = exp10(grads[i]);
			  tmp[1] = exp10(grads[i+1]);
			  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
			  
			  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
			    sciprint("Error allocating tmp_log_grads\n");
			    return -1;
			  }
			  
			  for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);
			  
			  for(j=0;j<nbsubtics;j++)
			    {
			      vyy1 = tmp_log_grads[j];
			      
			      if(vyy1<yminval || vyy1>ymaxval) continue;
			      
			      if(ppsubwin->axes.reverse[1] == TRUE)
				vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
			      
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
			      
			      if(ppsubwin->axes.axes_visible[1] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			  FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
			  
			} /* end NEW */
		      else
			{
			  
			  double ytmp = ppsubwin->axes.ygrads[i];
			  double dy = (ppsubwin->axes.ygrads[i+1] - ppsubwin->axes.ygrads[i]) / nbsubtics;
			  for(j=0;j<nbsubtics;j++)
			    {
			      vyy1=ytmp+dy*j;
			      
			      if(vyy1<yminval || vyy1>ymaxval) continue;
			      
			      if(ppsubwin->axes.reverse[1] == TRUE)
				vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);
			      
			      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
			      			      
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
			      
			      if(ppsubwin->axes.axes_visible[1] == TRUE)
				C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			    }
			}
		    }
		  
		  /***************************************************************/
		  /************************* COMMON PART *************************/
		  /***************************************************************/
		}
	    }
	}
      
      if (legy != 0 && (sciGetVisibility(ppsubwin->mon_y_label) == TRUE))
	{  
	  int x1, yy1;
	  if(pLABEL_FEATURE(ppsubwin->mon_y_label)->auto_rotation == TRUE){
	    angle =  0.;
	    sciSetFontOrientation(ppsubwin->mon_y_label,(int)angle);
	  }
	  else 
	    angle = sciGetFontOrientation(ppsubwin->mon_y_label)/10.;

	  /* F.Leray Adding 1 line here ("xset","pattern") to force the color and style of the */
	  /* legend to be the same as those used for the numbers for the axes*/
	  
	  fontid[0] = sciGetFontStyle(ppsubwin->mon_y_label);
	  fontid[1] = sciGetFontDeciWidth(ppsubwin->mon_y_label)/100;
	  
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  C2F(dr)("xstringl",legy,&zero,&zero,rect1,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  cosangle = cos((360-angle)*M_PI/180);
	  sinangle = sin((360-angle)*M_PI/180);
	  
	  if(pLABEL_FEATURE(ppsubwin->mon_y_label)->auto_position == TRUE){
	    x1  = x-rect1[2];
	    yy1 = y;
	  }
	  else{
	    double tmp[2];
	    sciGetPosition(ppsubwin->mon_y_label,&tmp[0],&tmp[1]);
	    
	    x1  = XDouble2Pixel(tmp[0]);
	    yy1 = YDouble2Pixel(tmp[1]);
	  }
	  
	  /* new automatic position values */
	  sciSetPosition(ppsubwin->mon_y_label,XPixel2Double(x1),YPixel2Double(yy1)); /* the lower left corner of the bounding rectangle */
	  
	  xm4[0] = x1;
	  xm4[1] = round(x1+cosangle*rect1[2]);
	  xm4[2] = round(x1+cosangle*rect1[2] + sinangle*(-rect1[3]));
	  xm4[3] = round(x1+sinangle*(-rect1[3]));
	  
	  ym4[0] = yy1;
	  ym4[1] = round(yy1-sinangle*rect1[2]);
	  ym4[2] = round(yy1-sinangle*rect1[2] + cosangle*(-rect1[3]));
	  ym4[3] = round(yy1+cosangle*(-rect1[3]));
	  
	  /* computation of the bounding box even when the string is turned */
	  
	  largeur = Max(abs(xm4[3] - xm4[1]),abs(xm4[2] - xm4[0]));
	  hauteur = Max(abs(ym4[3] - ym4[1]),abs(ym4[2] - ym4[0]));
	  
	  if(sciGetIsFilled(ppsubwin->mon_y_label) == TRUE){
	    int background = sciGetBackground(ppsubwin->mon_y_label);
	    int foreground = sciGetForeground(ppsubwin->mon_y_label);
	    int n = 4;
	    
	    C2F(dr)("xset","pattern",&background,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    
	    C2F (dr) ("xarea", str, &n, xm4, ym4, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
	    
	    C2F(dr)("xset","pattern",&foreground,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    
	    C2F (dr) ("xlines", "xv", &n, xm4, ym4, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
	  }
	  
	  textcolor = sciGetFontForeground(ppsubwin->mon_y_label);
	  
	  C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  xstringb_angle(legy,xm4[0],ym4[0],rect1[2],rect1[3],angle);
	  
	  textcolor = textcolor_old;
	  fontid[0] = fontid_old[0];
	  fontid[1] = fontid_old[1];
	  
	  C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
  double  tmpx1,tmpy1,tmpz1;
  /* TEST F.Leray 20.04.04: I fix HERE temporarily BOOL cube_scaling = FALSE; */
  BOOL cube_scaling;
  /* Test to enable reverse axis in 3D */ /* F.Leray 14.10.04 */
  /*   int u; */
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(pobj);
  /*  double xmin = Mini(x,n); */
  /*   double xmax = Maxi(x,n); */

  /*   double xmoy = (xmax+xmin)/2; */
  
  /*   double xmoy = (ppsubwin->FRect[0] + ppsubwin->FRect[2])/2; */
  
  double *xtmp = NULL;
  double *ytmp = NULL;
  
  xtmp = MALLOC(n*sizeof(double));
  ytmp = MALLOC(n*sizeof(double));


  /*   for(u=0;u<n;u++) */
  /*     { */
  /*       xtmp[u] = x[u]-2*(x[u]-xmoy); */
  /*     } */
  
  if (sciGetEntityType(pobj) == SCI_SUBWIN){

    cube_scaling = pSUBWIN_FEATURE(pobj)->cube_scaling;

    /*if (pSUBWIN_FEATURE (pobj)->isoview)*/

    

    if(cube_scaling == FALSE)
      {
	if (z == (double *) NULL)
	  for ( i=0 ; i < n ; i++)
	    {
	      /* F.Leray 19.10.04 */
	      /* Test to export logscale in 3D */

	      if(ppsubwin->logflags[0] == 'l')
		tmpx = log10(x[i]);
	      else
		tmpx = x[i];
	      
	      if(ppsubwin->logflags[1] == 'l')
		tmpy = log10(y[i]);
	      else
		tmpy = y[i];

	      if(ppsubwin->logflags[2] == 'l'){
		sciprint("Warning: Value on z data is negative or zero while logarithmic scale enabled\n");
		sciprint("Object not drawn\n");
		FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
		return 0;
	      }

	      xm[i]= TX3D(tmpx,tmpy,0.0);
	      ym[i]= TY3D(tmpx,tmpy,0.0);
	      
	      if(GlobalFlag_Zoom3dOn==1)
		Store3DPixelValues(pobj,xm[i],ym[i],tmpx,tmpy,0.); /* stockage des xm, ym pour le zoom */
	
	      if ( finite(xz1)==0||finite(yz1)==0 ){
		FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
		return(0);
	      }
	    }
	else /* z != NULL */
	  for ( i=0 ; i < n ; i++) /* cas 3d le + general */
	    {

	      /* F.Leray 19.10.04 */
	      /* Test to export logscale in 3D */

	      if(ppsubwin->logflags[0] == 'l')
		tmpx = log10(x[i]);
	      else
		tmpx = x[i];
	      
	      if(ppsubwin->logflags[1] == 'l')
		tmpy = log10(y[i]);
	      else
		tmpy = y[i];

	      if(ppsubwin->logflags[2] == 'l')
		tmpz = log10(z[i]);
	      else
		tmpz = z[i];

	      xm[i]= TX3D(tmpx,tmpy,tmpz);
	      ym[i]= TY3D(tmpx,tmpy,tmpz);
	      
	      if(GlobalFlag_Zoom3dOn==1)
		Store3DPixelValues(pobj,xm[i],ym[i],tmpx,tmpy,tmpz); /* stockage des xm, ym pour le zoom */
	      
	      if ( finite(xz1)==0||finite(yz1)==0 ){
		FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
		return(0);
	      }
	      /* 	} */
	    }
	FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
      }
    else   /* cube_scaling == TRUE now */
      {
	if (z == (double *) NULL)
	  for ( i=0 ; i < n ; i++)
	    {
	      
	      /* F.Leray 03.11.04 */
	      /* Test to export logscale in 3D */
	      
	      if(ppsubwin->logflags[0] == 'l')
		tmpx = log10(x[i]);
	      else
		tmpx = x[i];
	      
	      if(ppsubwin->logflags[1] == 'l')
		tmpy = log10(y[i]);
	      else
		tmpy = y[i];
	      
	      if(ppsubwin->logflags[2] == 'l'){
		sciprint("Warning: Value on z data is negative or zero while logarithmic scale enabled\n");
		sciprint("Object not drawn\n");
		FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
		return 0;
	      }
 	     
	      tmpx1 = tmpx;
	      tmpy1 = tmpy;
	      tmpz1 = 0.;

	      tmpx=(tmpx-pSUBWIN_FEATURE (pobj)->FRect[0])/(pSUBWIN_FEATURE (pobj)->FRect[2]-pSUBWIN_FEATURE (pobj)->FRect[0]);
	      tmpy=(tmpy-pSUBWIN_FEATURE (pobj)->FRect[1])/(pSUBWIN_FEATURE (pobj)->FRect[3]-pSUBWIN_FEATURE (pobj)->FRect[1]);
	      tmpz=(0.-pSUBWIN_FEATURE (pobj)->FRect[4])/(pSUBWIN_FEATURE (pobj)->FRect[5]-pSUBWIN_FEATURE (pobj)->FRect[4]); /* Adding F.Leray 04.11.04 */

	      xm[i]= TX3D(tmpx,tmpy,tmpz);
	      ym[i]= TY3D(tmpx,tmpy,tmpz);

	      if(GlobalFlag_Zoom3dOn==1)
		Store3DPixelValues(pobj,xm[i],ym[i],tmpx1,tmpy1,tmpz1); /* stockage des xm, ym pour le zoom */
	     
	      if ( finite(xz1)==0||finite(yz1)==0 ){
		FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
		return(0);
	      }
	    }
	else /* z != NULL */
	  for ( i=0 ; i < n ; i++)
	    {

	      /* F.Leray 03.11.04 */
	      /* Test to export logscale in 3D */
	      
	      if(ppsubwin->logflags[0] == 'l')
		tmpx = log10(x[i]);
	      else
		tmpx = x[i];
	      
	      if(ppsubwin->logflags[1] == 'l')
		tmpy = log10(y[i]);
	      else
		tmpy = y[i];

	      if(ppsubwin->logflags[2] == 'l')
		tmpz = log10(z[i]);
	      else
		tmpz = z[i];
	     
	      tmpx1 = tmpx;
	      tmpy1 = tmpy;
	      tmpz1 = tmpz;

	      tmpx= (tmpx-pSUBWIN_FEATURE (pobj)->FRect[0])/(pSUBWIN_FEATURE (pobj)->FRect[2]-pSUBWIN_FEATURE (pobj)->FRect[0]);
	      tmpy= (tmpy-pSUBWIN_FEATURE (pobj)->FRect[1])/(pSUBWIN_FEATURE (pobj)->FRect[3]-pSUBWIN_FEATURE (pobj)->FRect[1]);
	      tmpz= (tmpz-pSUBWIN_FEATURE (pobj)->FRect[4])/(pSUBWIN_FEATURE (pobj)->FRect[5]-pSUBWIN_FEATURE (pobj)->FRect[4]); /* Adding F.Leray 28.04.04 */

	      xm[i]= TX3D(tmpx,tmpy,tmpz);
	      ym[i]= TY3D(tmpx,tmpy,tmpz);
	          
	      if(GlobalFlag_Zoom3dOn==1)
		Store3DPixelValues(pobj,xm[i],ym[i],tmpx1,tmpy1,tmpz1); /* stockage des xm, ym pour le zoom */
	   
	      /*    xm[i]= TX3D(tmpx,tmpy,tmpz); */
	      /* 	      ym[i]= TY3D(tmpx,tmpy,tmpz); */
	      if ( finite(xz1)==0||finite(yz1)==0 ){
		FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
		return(0);
	      }
	    }
      }
    FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
  }
  FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
  return(1);
}

BOOL Ishidden(sciPointObj *pobj)
{
  double alpha;
  if (sciGetEntityType(pobj) == SCI_SUBWIN){
    alpha = pSUBWIN_FEATURE (pobj)->alpha;
    if ((alpha <0.0 ) && (alpha > -90.0))
      return TRUE;
    if ((alpha <360.0 ) && (alpha > 270.0)) /* missing case added to fix bug 839 F.Leray */
      return TRUE;
    if ((alpha <-180.0 ) && (alpha > -270.0))
      return TRUE;
    if ((alpha <180.0 ) && (alpha > 90.0))
      return TRUE;
  }
  return FALSE;
}

/* When alpha is close to a singularity (90,-90,270...), use to determine how to draw the x and y graduations */
/* depending on cof */
BOOL IsDownAxes(sciPointObj *pobj)
{
  double alpha,cof;
  
  if (sciGetEntityType(pobj) == SCI_SUBWIN){
    alpha = pSUBWIN_FEATURE (pobj)->alpha;   
    if (!(pSUBWIN_FEATURE (pobj)->isoview))
      cof=10.0;
    else
      cof = 5.0; /* F.Leray : hard fixed here */
    /* Correction Warnings Attention Precision*/
    /*   cof= (double) (Min(5.0,ceil(Max( */
    /* 				      abs((int)ppsubwin->axes.xlim[1]-(int)ppsubwin->axes.xlim[0])/ */
    /* 				      abs((int)ppsubwin->axes.ylim[1]-(int)ppsubwin->axes.ylim[0]), */
    /* 				      abs((int)ppsubwin->axes.ylim[1]-(int)ppsubwin->axes.ylim[0])/ */
    /* 				      abs((int)ppsubwin->axes.xlim[1]-(int)ppsubwin->axes.xlim[0]))))); */
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



int GradLog(double _min, double _max, double *_grads, int * n_grads)
{
  int i;
  int log_min, log_max;
  int /*  * tab= NULL, */ size;
  

  *n_grads = 0;
  log_max =  (int) ceil(_max);
  log_min =  (int) floor(_min);
  
  size = log_max - log_min +1;
  /*  tab=(int *)MALLOC(size*sizeof(int)); */
  
  /*   for(i=0;i<size;i++) tab[i]=log_min+i; */


  if(size<=7)
    {
      for(i=0;i<size;i++)
	{
	  /*    _grads[i] = exp10(tab[i]); */
	  _grads[i] = log_min+i;
	  *n_grads = (*n_grads) + 1;
	  /* 	  sciprint("Juste en sortie, _grads[%d] = %lf\n",i, _grads[i]); */
	}
    }
  else
    {
      int pas = 0, old_pas= 0,j;
      int val = size, passed = 0;
      
      for(j=val-1;j>1;j--)
	if(val%j == 0){
	  old_pas = pas;
	  pas=j; 
	  passed = 1;

	  if((7*pas)<=val){ 
	    if(old_pas != 0) {pas = old_pas; }
	    break;
	  }
	}
      
      if(passed != 1 || (size/pas)>15 ) pas = size;
            
      if(pas==size)
	{
	  _grads[0] = log_min;
	  _grads[1] = log_max;
	  *n_grads =2;
	}
      else
	for(i=0;i<=(int )(size/pas);i++)
	  {
	    _grads[i] = log_min+(i*pas);
	    
	    *n_grads = (*n_grads) + 1;
	    /* 	    sciprint("Juste en sortie, _grads[%d] = %lf\n",i, _grads[i]); */
	  }
    }
  
  return 0;
}



/* F.Leray au 13.10.04 completly review for new axes graduations */
/*** F.Leray 02.04.04 */
/* FUNCTION FOR 2D UPDATE ONLY !!!!! <=> beginning of axis_3ddraw (in 2d HERE of course! ) */
/* Copy on update_frame_bounds */
BOOL sci_update_frame_bounds_2d(sciPointObj *pobj)
{
  double xmax, xmin, ymin, ymax;
  double hx,hy,hx1,hy1;
  int i;
  
  sciSubWindow * ppsubwin =  pSUBWIN_FEATURE (pobj);
  double FRect[4],WRect[4],ARect[4]; 
  char logscale[2];
  
  /* Temp variables only used when called from update_specification_bounds */
  /* to know if we have to redraw all the figure */
  double ExistingFRect[4]; /* the Existing FRect at start to be compared at the end of this routine */
  /* in order to determine wheter or not the bounds have changed... */
  int nbsubtics[2];
  int nbgrads[2];
  /* End of Temp variables */
  
  for(i=0;i<4;i++) ExistingFRect[i] =  ppsubwin->FRect[i]; /* store old initial bounds*/

  for(i=0;i<2;i++) nbsubtics[i] = ppsubwin->axes.nbsubtics[i];
  nbgrads[0] = ppsubwin->axes.nxgrads;
  nbgrads[1] = ppsubwin->axes.nygrads;
  
  /* nbtics on z put to 0 */
  /*   ppsubwin->axes.nzgrads = 0; */
  

  /*****************************************************************
   * get initial bounds
   *****************************************************************/
  if(sciGetZooming(pobj) == TRUE) {
    xmin= ppsubwin->ZRect[0]; 
    ymin= ppsubwin->ZRect[1]; 
    xmax= ppsubwin->ZRect[2];
    ymax= ppsubwin->ZRect[3];
    /*    sciprint("Il y a un zoom dans l'axe courrant\n"); */
    /*     sciprint(" xmin= ppsubwin->ZRect[0] = %lf\n",ppsubwin->ZRect[0]); */
    /*     sciprint(" ymin= ppsubwin->ZRect[1] = %lf\n",ppsubwin->ZRect[1]); */
    /*     sciprint(" xmax= ppsubwin->ZRect[2] = %lf\n",ppsubwin->ZRect[2]); */
    /*     sciprint(" ymax= ppsubwin->ZRect[3] = %lf\n",ppsubwin->ZRect[3]); */
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
  }

  /*****************************************************************
   * modify  bounds and aaint  if using log scaling Y axis
   *****************************************************************/
  if ( ppsubwin->logflags[1]=='l') {
    if ( ymin > 0 ) {
      ymax= ceil(log10(ymax)); ymin= floor(log10(ymin));
    }
    else {
      Scistring(" Can't use Log on Y-axis ymin is negative \n");
      ymax= 1; ymin= 0;
    }
  }
   


  /* _grad Init. to 0. */
  for(i=0;i<20;i++) 
    {
      ppsubwin->axes.xgrads[i] = 0.;
      ppsubwin->axes.ygrads[i] = 0.;
    }
   
 
  if ( ppsubwin->logflags[0]=='n') { /* x-axis */
    TheTicks(&xmin, &xmax, &(ppsubwin->axes.xgrads[0]), &ppsubwin->axes.nxgrads);
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'n',NULL,ppsubwin->axes.nbsubtics[0]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
  }
  else{ /* log. case */
    GradLog(xmin,xmax,ppsubwin->axes.xgrads,&ppsubwin->axes.nxgrads);
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'l',ppsubwin->axes.xgrads,0);
  }
   
  if ( ppsubwin->logflags[1]=='n') { /* y-axis */
    TheTicks(&ymin, &ymax, &(ppsubwin->axes.ygrads[0]), &ppsubwin->axes.nygrads);
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'n',NULL, ppsubwin->axes.nbsubtics[1]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
  }
  else{ /* log. case */
    GradLog(ymin,ymax,ppsubwin->axes.ygrads,&ppsubwin->axes.nygrads);
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'l',ppsubwin->axes.ygrads,0);
  }
   
  if(ppsubwin->tight_limits == FALSE )
    {
      xmin = ppsubwin->axes.xgrads[0];
      xmax = ppsubwin->axes.xgrads[ ppsubwin->axes.nxgrads - 1];
      ymin = ppsubwin->axes.ygrads[0];
      ymax = ppsubwin->axes.ygrads[ ppsubwin->axes.nygrads - 1];
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
    
    /* F.Leray 28.09.04 */
    /* I need to recompute the correct xgrads and ygrads vector to have a good display */
     
    if ( ppsubwin->logflags[0]=='n') { /* x-axis */
      TheTicks(&xmin, &xmax, &(ppsubwin->axes.xgrads[0]), &ppsubwin->axes.nxgrads);
      ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'n',NULL, ppsubwin->axes.nbsubtics[0]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    }
    else{ /* log. case */
      GradLog(xmin,xmax,ppsubwin->axes.xgrads,&ppsubwin->axes.nxgrads);
      ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'l',ppsubwin->axes.xgrads,0);
    }

 

    if ( ppsubwin->logflags[1]=='n') { /* y-axis */
      TheTicks(&ymin, &ymax, &(ppsubwin->axes.ygrads[0]), &ppsubwin->axes.nygrads);
      ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'n',NULL, ppsubwin->axes.nbsubtics[1]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    }
    else{ /* log. case */
      GradLog(ymin,ymax,ppsubwin->axes.ygrads,&ppsubwin->axes.nygrads);
      ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'l',ppsubwin->axes.ygrads,0);
    }
   
   
    /* END ISO if */
  }
  
  /* Changement ci-dessous F.Leray 21.09.04 */
  ppsubwin->axes.xlim[0]=xmin;
  ppsubwin->axes.xlim[1]=xmax;
  ppsubwin->axes.xlim[2]=0;
  ppsubwin->axes.ylim[0]=ymin;
  ppsubwin->axes.ylim[1]=ymax;
  ppsubwin->axes.ylim[2]=0;
   
   
   
  /*****************************************************************
   * set the actual bounds in subwindow data structure
   *****************************************************************/
   
   
  ppsubwin->FRect[0]=xmin;
  ppsubwin->FRect[2]=xmax;
  ppsubwin->FRect[1]=ymin;
  ppsubwin->FRect[3]=ymax;
   
  ppsubwin->axes.xlim[3] = ppsubwin->axes.nxgrads;
  ppsubwin->axes.ylim[3] = ppsubwin->axes.nygrads;
   
  /*   ppsubwin->axes.reverse[0] = FALSE; /\*TRUE;*\/  /\* TEST en DUR F.Leray ICIIIIIIIIIIIII 12.10.04 *\/ */
  /*    ppsubwin->axes.reverse[1] = FALSE; /\*TRUE;*\/  */
   
   
  set_scale("tftftf",NULL,ppsubwin->FRect,NULL,ppsubwin->logflags,NULL); 
   
   
  for(i=0;i<4;i++)
    if(ppsubwin->FRect[i] != ExistingFRect[i]){
      return TRUE;
      break;
    } 
   
  for(i=0;i<2;i++) 
    if(nbsubtics[i] != ppsubwin->axes.nbsubtics[i]){
      return TRUE;
      break;
    }
   
  if(nbgrads[0] != ppsubwin->axes.nxgrads) return TRUE;
  if(nbgrads[1] != ppsubwin->axes.nygrads) return TRUE;
   
  return FALSE;
}

/**update_3dbounds -> renammed sci_update_frame_bounds_3d
 * @author Djalel Abdemouche 10/2003
 * Should be in Plo2dEch.c file
 */
BOOL sci_update_frame_bounds_3d(sciPointObj *pobj)
{
  double xmin,xmax,ymin,ymax,zmin,zmax; 
  int i;
  /*   double lmin,lmax; */
  /*   integer min,max,puiss,deux=2,dix=10,n1,n2; */
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj); 

  /* Temp variables only used when called from update_specification_bounds */
  /* to know if we have to redraw all the figure */
  double ExistingFRect[6]; /* the Existing FRect at start to be compared at the end of this routine */
  /* in order to determine wheter or not the bounds have changed... */
  int nbsubtics[3];
  int nbgrads[3];
  /* End of Temp variables */

  for(i=0;i<6;i++) ExistingFRect[i] =  ppsubwin->FRect[i]; /* store old initial bounds*/
  
  for(i=0;i<3;i++) nbsubtics[i] = ppsubwin->axes.nbsubtics[i];
  nbgrads[0] = ppsubwin->axes.nxgrads;
  nbgrads[1] = ppsubwin->axes.nygrads;
  nbgrads[2] = ppsubwin->axes.nzgrads;
  
  /*****************************************************************
   * get initial bounds
   *****************************************************************/

  if(sciGetZooming(pobj) == TRUE) { /* a revoir */
    xmin= ppsubwin->ZRect[0]; 
    ymin= ppsubwin->ZRect[1]; 
    xmax= ppsubwin->ZRect[2];
    ymax= ppsubwin->ZRect[3];
    zmin= ppsubwin->ZRect[4];
    zmax= ppsubwin->ZRect[5];
  }
  else {
    xmin=ppsubwin->SRect[0];
    xmax=ppsubwin->SRect[1];
    ymin=ppsubwin->SRect[2];
    ymax=ppsubwin->SRect[3];
    zmin=ppsubwin->SRect[4];
    zmax=ppsubwin->SRect[5];
  }

  ppsubwin->axes.limits[1]=xmin; /* set a quoi?? au final ca saute normalement...*/
  ppsubwin->axes.limits[3]=xmax; /* F.Leray 19.10.04 */
  ppsubwin->axes.limits[2]=ymin;
  ppsubwin->axes.limits[4]=ymax;
  ppsubwin->axes.limits[5]=zmin;
  ppsubwin->axes.limits[6]=zmax;




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
  }
  
  /*****************************************************************
   * modify  bounds and aaint  if using log scaling Y axis
   *****************************************************************/
  if ( ppsubwin->logflags[1]=='l') {
    if ( ymin > 0 ) {
      ymax= ceil(log10(ymax)); ymin= floor(log10(ymin));
    }
    else {
      Scistring(" Can't use Log on Y-axis ymin is negative \n");
      ymax= 1; ymin= 0;
    }
  }
  

  /*****************************************************************
   * modify  bounds and aaint  if using log scaling Y axis
   *****************************************************************/
  if ( ppsubwin->logflags[2]=='l') {
    if ( zmin > 0 ) {
      zmax= ceil(log10(zmax)); zmin= floor(log10(zmin));
    }
    else {
      Scistring(" Can't use Log on Z-axis zmin is negative \n");
      zmax= 1; zmin= 0;
    }
  }
  

  /* _grad Init. to 0. */
  for(i=0;i<20;i++) 
    {
      ppsubwin->axes.xgrads[i] = 0.;
      ppsubwin->axes.ygrads[i] = 0.;
      ppsubwin->axes.zgrads[i] = 0.;
    }
   
  
  if ( ppsubwin->logflags[0]=='n') { /* x-axis */
    TheTicks(&xmin, &xmax, &(ppsubwin->axes.xgrads[0]), &ppsubwin->axes.nxgrads);
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'n',NULL,ppsubwin->axes.nbsubtics[0]); /* Nb of subtics computation and storage */
  }
  else{ /* log. case */
    GradLog(xmin,xmax,ppsubwin->axes.xgrads,&ppsubwin->axes.nxgrads);
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'l',ppsubwin->axes.xgrads,0);
  }
  
  if ( ppsubwin->logflags[1]=='n') { /* y-axis */
    TheTicks(&ymin, &ymax, &(ppsubwin->axes.ygrads[0]), &ppsubwin->axes.nygrads);
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'n',NULL, ppsubwin->axes.nbsubtics[1]); /* Nb of subtics computation and storage */
  }
  else{ /* log. case */
    GradLog(ymin,ymax,ppsubwin->axes.ygrads,&ppsubwin->axes.nygrads);
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'l',ppsubwin->axes.ygrads,0);
  }
  
  if ( ppsubwin->logflags[2]=='n') { /* z-axis */
    TheTicks(&zmin, &zmax, &(ppsubwin->axes.zgrads[0]), &ppsubwin->axes.nzgrads);
    ppsubwin->axes.nbsubtics[2] = ComputeNbSubTics(pobj,ppsubwin->axes.nzgrads,'n',NULL, ppsubwin->axes.nbsubtics[2]); /* Nb of subtics computation and storage */
  }
  else{ /* log. case */
    GradLog(zmin,zmax,ppsubwin->axes.zgrads,&ppsubwin->axes.nzgrads);
    ppsubwin->axes.nbsubtics[2] = ComputeNbSubTics(pobj,ppsubwin->axes.nzgrads,'l',ppsubwin->axes.zgrads,0);
  }
  
  if(ppsubwin->tight_limits == FALSE )
    {
      xmin = ppsubwin->axes.xgrads[0];
      xmax = ppsubwin->axes.xgrads[ ppsubwin->axes.nxgrads - 1];
      ymin = ppsubwin->axes.ygrads[0];
      ymax = ppsubwin->axes.ygrads[ ppsubwin->axes.nygrads - 1];
      zmin = ppsubwin->axes.zgrads[0];
      zmax = ppsubwin->axes.zgrads[ ppsubwin->axes.nzgrads - 1];
    }

  ppsubwin->axes.xlim[0]=xmin;
  ppsubwin->axes.xlim[1]=xmax;
  ppsubwin->axes.xlim[2]=0;
  ppsubwin->axes.ylim[0]=ymin;
  ppsubwin->axes.ylim[1]=ymax;
  ppsubwin->axes.ylim[2]=0;
  ppsubwin->axes.zlim[0]=zmin; /* rajout pour z */
  ppsubwin->axes.zlim[1]=zmax;
  ppsubwin->axes.zlim[2]=0;
     
  ppsubwin->FRect[0]=xmin;
  ppsubwin->FRect[2]=xmax;
  ppsubwin->FRect[1]=ymin;
  ppsubwin->FRect[3]=ymax;
  ppsubwin->FRect[4]=zmin;
  ppsubwin->FRect[5]=zmax;
    
  ppsubwin->axes.xlim[3] = ppsubwin->axes.nxgrads;
  ppsubwin->axes.ylim[3] = ppsubwin->axes.nygrads;
  ppsubwin->axes.zlim[3] = ppsubwin->axes.nzgrads;
   
     
  /*   ppsubwin->axes.reverse[0] = FALSE; /\*TRUE;*\/ */
  /*   ppsubwin->axes.reverse[1] = FALSE; /\*TRUE;*\/  */
  /*   ppsubwin->axes.reverse[2] = FALSE; /\*TRUE;*\/  */
     
  wininfo("alpha=%.1f,theta=%.1f",ppsubwin->alpha,ppsubwin->theta); 
    
  for(i=0;i<6;i++)
    if(ppsubwin->FRect[i] != ExistingFRect[i]){
      return TRUE;
      break;
    } 
    
  for(i=0;i<3;i++) 
    if(nbsubtics[i] != ppsubwin->axes.nbsubtics[i]){
      return TRUE;
      break;
    }
  
  if(nbgrads[0] != ppsubwin->axes.nxgrads) return TRUE;
  if(nbgrads[1] != ppsubwin->axes.nygrads) return TRUE;
  if(nbgrads[2] != ppsubwin->axes.nzgrads) return TRUE;
  
  return FALSE;  
}



/* F.Leray 07.05.04 */
/* This routine rebuild a needed strflag for axis_draw call in sciDrawObj*/
/* It may be less complete than the original strflag given in plot2dn (or other 2D - plot function)
   due to less info. Nevertheless this new strflag is sufficient for axis_draw because
   tests are on strflag[1] (case =='5' or '6') and strflag[2] */
void rebuild_strflag( sciPointObj * psubwin, char * STRFLAG)
{

  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  BOOL isaxes = GetIsAxes2D(psubwin);

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
  if(isaxes == TRUE)
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
      if(ppsubwin->axes.rect == 1)
	STRFLAG[2] = '2';
      else
	STRFLAG[2] = '0';
    }
  
  STRFLAG[3] = '\0';
}


/*----------------------------------------------------------------------------------*/
/* draw the title and the two labels of the 2d axis                                 */
/*----------------------------------------------------------------------------------*/
int labels2D_draw(sciPointObj * psubwin)
{
  integer x[6], v,verbose=0,rect1[4],fontid[2],narg;
  double dv, locx, locy;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  char locstr;
  /*   integer fontid, fontsize, textcolor; */
  
  int flagx = 0;
  int old_foreground;

  int xm[4], ym[4],n=4;
  double cosangle;
  double sinangle;
  int close=1;
  int zero = 0;
  double font_angle;
  int largeur, hauteur;
  int x1, yy1;
  char str[2] = "xv";

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



  /* save old foreground, font  values*/

  C2F(dr)("xget","font",&verbose,fontid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  
  C2F (dr) ("xget", "foreground", &flagx, &old_foreground, &v, &v, &v,
	    &v, &dv, &dv, &dv, &dv, 5L, 4096);
  
  /* main title */ /* We fix the title always at the top */
  rect1[0]= Cscale.WIRect1[0];
  rect1[1]= Cscale.WIRect1[1];
  rect1[2]= Cscale.WIRect1[2];
  rect1[3]= Cscale.WIRect1[3]/6;
  
  if(sciGetVisibility(ppsubwin->mon_title) == TRUE)
  {
    int    bboxtitle[4] ;
    double cosAngle ;
    double sinAngle ;

    /* get the pointer on the title */
    sciLabel * ppLabel = pLABEL_FEATURE( ppsubwin->mon_title ) ;
    sciText  * ppText  = &(ppLabel->text) ;
    
    x[0] = sciGetFontForeground (ppsubwin->mon_title);
    x[2] = sciGetFontDeciWidth (ppsubwin->mon_title)/100;
    x[3] = 0;
    x[4] = sciGetFontStyle(ppsubwin->mon_title);

    /* get position and orientation of the title */
    if ( ppLabel->auto_rotation )
    {
      font_angle = 0 ;
      sciSetFontOrientation( ppsubwin->mon_title, (int)font_angle ) ;
    }
    else
    {
      font_angle = sciGetFontOrientation( ppsubwin->mon_title ) ;
      font_angle /= 10.0 ;
    }
    cosAngle = cos( DEG2RAD( 360 - font_angle ) ) ;
    sinAngle = sin( DEG2RAD( 360 - font_angle ) ) ;

    /* make it back to this value */
    
    C2F(dr)("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);
    
    xstringb_bbox (sciGetText(ppsubwin->mon_title), rect1[0], rect1[1], rect1[2], rect1[3],
                   0, bboxtitle);
      /*C2F(dr)("xstringl",sciGetText(ppsubwin->mon_title),
        &rect1[0],&rect1[1],bboxtitle,&v,&v,&v,&dv,&dv,&dv,&dv,9L,sciGetTextLength(ppsubwin->mon_x_la));*/
    
    
    /* store the value in the position field of the title */
    
    /*bboxtitle[0] = bboxtitle[0]-1;*/ /* better display */
    /*bboxtitle[1] = bboxtitle[1]-2;*/ /* better display */

    if ( ppLabel->auto_position )
    {

      /* get the position given by bboxtitle */
      bboxtitle[0] = bboxtitle[0]-1; /* better display */
      bboxtitle[1] = bboxtitle[1]-2 + bboxtitle[3] ; /* better display */
      ppText->x     = XPixel2Double( bboxtitle[0] ) ;
      ppText->y     = YPixel2Double( bboxtitle[1] ) ;
      sciSetPosition( ppsubwin->mon_title, ppText->x, ppText->y ) ;
    }
    else
    {
      /* update the position of the text */
      /* by copying the one in the label */
      /* yes redundant information */
      sciGetPosition( ppsubwin->mon_title, &(ppText->x), &(ppText->y) ) ;
      bboxtitle[0] = XDouble2Pixel( ppText->x ) ;
      bboxtitle[1] = YDouble2Pixel( ppText->y ) ;
    }
    
    xm[0] = round(bboxtitle[0]);
    xm[1] = round(xm[0] + cosAngle * bboxtitle[2]);
    xm[2] = round(xm[0] + cosAngle*bboxtitle[2] + sinAngle*(-bboxtitle[3]));
    xm[3] = round(xm[0] + sinAngle*(-bboxtitle[3]));
    
    ym[0] = round(bboxtitle[1]);
    ym[1] = round(ym[0] - sinAngle*bboxtitle[2]);
    ym[2] = round(ym[0] - sinAngle*bboxtitle[2] + cosAngle*(-bboxtitle[3]));
    ym[3] = round(ym[0] + cosAngle*(-bboxtitle[3]));

    
    if(sciGetIsFilled(ppsubwin->mon_title) == TRUE)
    {
      x[0] = sciGetBackground(ppsubwin->mon_title);
      
      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
      
      C2F (dr) ("xarea", str, &n, xm, ym, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
    
      x[0] = sciGetForeground(ppsubwin->mon_title);
      
      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
      
      C2F (dr) ("xlines", "xv", &n, xm, ym, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
    }
    
    x[0] = sciGetFontForeground(ppsubwin->mon_title);
    
    C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
    C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
    
    /*xstringb_angle(sciGetText(ppsubwin->mon_title),rect1[0],rect1[1],rect1[2],rect1[3],font_angle);*/
    xstringb_angle(sciGetText(ppsubwin->mon_title),xm[0],ym[0],bboxtitle[2],bboxtitle[3],font_angle);
  }
  
  /* x label */
  
  /* F.Leray 02.09.05 */
  /* New implementation */
  
  if( sciGetVisibility(ppsubwin->mon_x_label) == TRUE){

    x[0] = sciGetFontForeground (ppsubwin->mon_x_label);
    x[2] = sciGetFontDeciWidth (ppsubwin->mon_x_label)/100;
    x[3] = 0;
    x[4] = sciGetFontStyle(ppsubwin->mon_x_label);
    
    if(pLABEL_FEATURE(ppsubwin->mon_x_label)->auto_rotation == TRUE){
      font_angle =  0.;
      sciSetFontOrientation(ppsubwin->mon_x_label,(int)font_angle);
    }
    else
    {
      font_angle = sciGetFontOrientation(ppsubwin->mon_x_label)/10.;
    }
    
    C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
    C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
    C2F (dr) ("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);
    

    if(pLABEL_FEATURE(ppsubwin->mon_x_label)->auto_position == TRUE)
    {
      C2F(dr)("xstringl",sciGetText(ppsubwin->mon_x_label),
              &zero,&zero,rect1,&v,&v,&v,&dv,&dv,&dv,&dv,9L,sciGetTextLength(ppsubwin->mon_x_label));
      
      cosangle = cos((360-font_angle)*M_PI/180);
      sinangle = sin((360-font_angle)*M_PI/180);
      
      xm[0] = 0;
      xm[1] = round(cosangle*rect1[2]);
      xm[2] = round(cosangle*rect1[2] + sinangle*(-rect1[3]));
      xm[3] = round(sinangle*(-rect1[3]));
        
      ym[0] = 0;
      ym[1] = round(-sinangle*rect1[2]);
      ym[2] = round(-sinangle*rect1[2] + cosangle*(-rect1[3]));
      ym[3] = round(cosangle*(-rect1[3]));
  
      /* computation of the bounding box even when the string is turned */
  
      largeur = Max(abs(xm[3] - xm[1]),abs(xm[2] - xm[0]));
      hauteur = Max(abs(ym[3] - ym[1]),abs(ym[2] - ym[0]));
    
      if(ppsubwin->axes.xdir != 'u'){
        /* the x axis is at the bottom or centered on 0 (grads are also on beneath the axis in this case...) */
        yy1 = round(ppsubwin->XGradMostOnBottom + (Cscale.WIRect1[2]/50.0));
	  
        /* hauteur */
        if((font_angle>=180 && font_angle <= 360)){
          yy1 = yy1 + hauteur;
        }
        else {
          int xm[4], ym[4];
          double cosangle, sinangle;
          cosangle = cos((360-89)*M_PI/180);
          sinangle = sin((360-89)*M_PI/180);
	    
          xm[0] = 0;
          xm[1] = round(cosangle*rect1[2]);
          xm[2] = round(cosangle*rect1[2] + sinangle*(-rect1[3]));
          xm[3] = round(sinangle*(-rect1[3]));
    
          ym[0] = 0;
          ym[1] = round(-sinangle*rect1[2]);
          ym[2] = round(-sinangle*rect1[2] + cosangle*(-rect1[3]));
          ym[3] = round(cosangle*(-rect1[3]));
    
          hauteur = Max(abs(xm[3] - xm[1]),abs(xm[2] - xm[0]));
    
          yy1 = yy1 + hauteur;
        }
      }
      else{
        /* the x axis is on the top */
        yy1 = round(ppsubwin->XGradMostOnTop - (Cscale.WIRect1[2]/25.0));
	  
        /* hauteur */
        if(font_angle>=180 && font_angle <= 360){
          int xm[4], ym[4];
          double cosangle, sinangle;
          cosangle = cos((360-89)*M_PI/180);
          sinangle = sin((360-89)*M_PI/180);
	    
          xm[0] = 0;
          xm[1] = round(cosangle*rect1[2]);
          xm[2] = round(cosangle*rect1[2] + sinangle*(-rect1[3]));
          xm[3] = round(sinangle*(-rect1[3]));
	    
          ym[0] = 0;
          ym[1] = round(-sinangle*rect1[2]);
          ym[2] = round(-sinangle*rect1[2] + cosangle*(-rect1[3]));
          ym[3] = round(cosangle*(-rect1[3]));
	    
          hauteur = Max(abs(xm[3] - xm[1]),abs(xm[2] - xm[0]));
	    
          yy1 = yy1 - hauteur;
        }
        else{
          yy1 = yy1 - hauteur;
        }
      }
	
      /* largeur */
      if((font_angle >=0 && font_angle <90) || (font_angle >270 && font_angle <= 360))
        x1 = round(Cscale.WIRect1[0] + Cscale.WIRect1[2]/2 - largeur/2);
      else
        x1 = round(Cscale.WIRect1[0] + Cscale.WIRect1[2]/2 + largeur/2);
	
      /* new automatic position values */
      sciSetPosition(ppsubwin->mon_x_label,XPixel2Double(x1),YPixel2Double(yy1)); /* the lower left corner of the bounding rectangle */
	
      xm[0] = round(x1);
      xm[1] = round(x1 + cosangle*rect1[2]);
      xm[2] = round(x1 + cosangle*rect1[2] + sinangle*(-rect1[3]));
      xm[3] = round(x1 + sinangle*(-rect1[3]));
	
      ym[0] = round(yy1);
      ym[1] = round(yy1 - sinangle*rect1[2]);
      ym[2] = round(yy1 - sinangle*rect1[2] + cosangle*(-rect1[3]));
      ym[3] = round(yy1 + cosangle*(-rect1[3]));
    }
    else /* manual position selected (unit is the user coord.) */
    {
      double tmp[2];
      sciGetPosition(ppsubwin->mon_x_label,&tmp[0],&tmp[1]);
	
      x1  = XDouble2Pixel(tmp[0]);
      yy1 = YDouble2Pixel(tmp[1]);
	
      C2F(dr)("xstringl",sciGetText(ppsubwin->mon_x_label),
              &zero,&zero,rect1,&v,&v,&v,&dv,&dv,&dv,&dv,9L,sciGetTextLength(ppsubwin->mon_x_label));
	
      cosangle = cos((360-font_angle)*M_PI/180);
      sinangle = sin((360-font_angle)*M_PI/180);
	
      xm[0] = round(x1);
      xm[1] = round(x1 + cosangle*rect1[2]);
      xm[2] = round(x1 + cosangle*rect1[2] + sinangle*(-rect1[3]));
      xm[3] = round(x1 + sinangle*(-rect1[3]));
  
      ym[0] = round(yy1);
      ym[1] = round(yy1 - sinangle*rect1[2]);
      ym[2] = round(yy1 - sinangle*rect1[2] + cosangle*(-rect1[3]));
      ym[3] = round(yy1 + cosangle*(-rect1[3]));
 	
    }

    if(sciGetIsFilled(ppsubwin->mon_x_label) == TRUE){
      x[0] = sciGetBackground(ppsubwin->mon_x_label);
      
      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
      
      C2F (dr) ("xarea", str, &n, xm, ym, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
      
      x[0] = sciGetForeground(ppsubwin->mon_x_label);
      
      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
      
      C2F (dr) ("xlines", "xv", &n, xm, ym, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
    }
    
    x[0] = sciGetFontForeground(ppsubwin->mon_x_label);
    
    C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
    C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
    
    xstringb_angle(sciGetText(ppsubwin->mon_x_label),xm[0],ym[0],rect1[2],rect1[3],font_angle);
  }
  
	
      

  /* y label */
  
  /* F.Leray 04.08.05 */
  /* New implementation */
  

  if( sciGetVisibility(ppsubwin->mon_y_label) == TRUE){

    x[0] = sciGetFontForeground (ppsubwin->mon_y_label);
    x[2] = sciGetFontDeciWidth (ppsubwin->mon_y_label)/100;
    x[3] = 0;
    x[4] = sciGetFontStyle(ppsubwin->mon_y_label);

    if(pLABEL_FEATURE(ppsubwin->mon_y_label)->auto_rotation == TRUE){
      font_angle =  270.;
      sciSetFontOrientation(ppsubwin->mon_y_label,(int)(font_angle*10));
    }
    else 
      font_angle = sciGetFontOrientation(ppsubwin->mon_y_label)/10.;
    
    C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
    C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
    C2F(dr)("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);
    
    
    if(pLABEL_FEATURE(ppsubwin->mon_y_label)->auto_position == TRUE)
    {
      C2F(dr)("xstringl",sciGetText(ppsubwin->mon_y_label),
              &zero,&zero,rect1,&v,&v,&v,&dv,&dv,&dv,&dv,9L,sciGetTextLength(ppsubwin->mon_y_label));
	
      cosangle = cos((360-font_angle)*M_PI/180);
      sinangle = sin((360-font_angle)*M_PI/180);
  
      xm[0] = 0;
      xm[1] = round(cosangle*rect1[2]);
      xm[2] = round(cosangle*rect1[2] + sinangle*(-rect1[3]));
      xm[3] = round(sinangle*(-rect1[3]));
  
      ym[0] = 0;
      ym[1] = round(-sinangle*rect1[2]);
      ym[2] = round(-sinangle*rect1[2] + cosangle*(-rect1[3]));
      ym[3] = round(cosangle*(-rect1[3]));
  
      /* computation of the bounding box even when the string is turned */
  
      largeur = Max(abs(xm[3] - xm[1]),abs(xm[2] - xm[0]));
      hauteur = Max(abs(ym[3] - ym[1]),abs(ym[2] - ym[0]));
    
      if(ppsubwin->axes.ydir != 'r'){
        /* the y axis is on the left or centered on 0 (grads are also on the left in this case...) */
        x1 = round(ppsubwin->YGradMostOnLeft - (Cscale.WIRect1[3]/50.0));

        /* largeur */
        if((font_angle>=0 && font_angle <= 90)  || (font_angle>270 && font_angle <= 360)){
          x1 = x1 - largeur;
        }
        else if(font_angle>=180 && font_angle <= 360){
          int xm[4], ym[4];
          double cosangle, sinangle;
          cosangle = cos((360-179)*M_PI/180);
          sinangle = sin((360-179)*M_PI/180);
    
          xm[0] = 0;
          xm[1] = round(cosangle*rect1[2]);
          xm[2] = round(cosangle*rect1[2] + sinangle*(-rect1[3]));
          xm[3] = round(sinangle*(-rect1[3]));
    
          ym[0] = 0;
          ym[1] = round(-sinangle*rect1[2]);
          ym[2] = round(-sinangle*rect1[2] + cosangle*(-rect1[3]));
          ym[3] = round(cosangle*(-rect1[3]));
    
          largeur = Max(abs(xm[3] - xm[1]),abs(xm[2] - xm[0]));
        }
        else if(font_angle>90 && font_angle < 180){
          int xm[4], ym[4];
          double cosangle, sinangle;
          cosangle = cos((360-89)*M_PI/180);
          sinangle = sin((360-89)*M_PI/180);
    
          xm[0] = 0;
          xm[1] = round(cosangle*rect1[2]);
          xm[2] = round(cosangle*rect1[2] + sinangle*(-rect1[3]));
          xm[3] = round(sinangle*(-rect1[3]));
    
          ym[0] = 0;
          ym[1] = round(-sinangle*rect1[2]);
          ym[2] = round(-sinangle*rect1[2] + cosangle*(-rect1[3]));
          ym[3] = round(cosangle*(-rect1[3]));
    
          largeur = Max(abs(xm[3] - xm[1]),abs(xm[2] - xm[0]));
    
          x1 = x1 - largeur;
        }
      }
      else
      {
        /* the y axis is on the right */
        x1 = round(ppsubwin->YGradMostOnRight + (Cscale.WIRect1[3]/50.0));
        
        if((font_angle>=0 && font_angle <= 90)){
          /* do nothing more */
        }
        else if(font_angle>270 && font_angle <= 360){
          int xm[4], ym[4];

          xm[0] = 0;
          xm[1] = round(cosangle*rect1[2]);
          xm[2] = round(cosangle*rect1[2] + sinangle*(-rect1[3]));
          xm[3] = round(sinangle*(-rect1[3]));
          
          ym[0] = 0;
          ym[1] = round(-sinangle*rect1[2]);
          ym[2] = round(-sinangle*rect1[2] + cosangle*(-rect1[3]));
          ym[3] = round(cosangle*(-rect1[3]));
    
          largeur = Max(abs(xm[3] - xm[1]),abs(xm[2] - xm[0]));
    
          x1 = x1 - xm[3];
          
        }
        else
        {
          int xm[4], ym[4];

          xm[0] = 0;
          xm[1] = round(cosangle*rect1[2]);
          xm[2] = round(cosangle*rect1[2] + sinangle*(-rect1[3]));
          xm[3] = round(sinangle*(-rect1[3]));
    
          ym[0] = 0;
          ym[1] = round(-sinangle*rect1[2]);
          ym[2] = round(-sinangle*rect1[2] + cosangle*(-rect1[3]));
          ym[3] = round(cosangle*(-rect1[3]));
    
          largeur = Max(abs(xm[3] - xm[1]),abs(xm[2] - xm[0]));
    
          x1 = x1 + largeur;
        }
      }
      /* hauteur */
      /*     printf("Cscale.WIRect1[0] = %d\tCscale.WIRect1[1] = %d\tCscale.WIRect1[2] = %d\tCscale.WIRect1[3] = %d\n", */
      /* 	   Cscale.WIRect1[0],Cscale.WIRect1[1],Cscale.WIRect1[2],Cscale.WIRect1[3]); */

      if((font_angle>=0 && font_angle <= 180)){
        yy1 = round(Cscale.WIRect1[1] + Cscale.WIRect1[3]/2 - hauteur/2);
      }
      else{
        yy1 = round(Cscale.WIRect1[1] + Cscale.WIRect1[3]/2 + hauteur/2);
      } 

      /* new automatic position values */
      sciSetPosition(ppsubwin->mon_y_label,XPixel2Double(x1),YPixel2Double(yy1)); /* the lower left corner of the bounding rectangle */
	
      xm[0] = round(x1);
      xm[1] = round(x1 + cosangle*rect1[2]);
      xm[2] = round(x1 + cosangle*rect1[2] + sinangle*(-rect1[3]));
      xm[3] = round(x1 + sinangle*(-rect1[3]));
	
      ym[0] = round(yy1);
      ym[1] = round(yy1 - sinangle*rect1[2]);
      ym[2] = round(yy1 - sinangle*rect1[2] + cosangle*(-rect1[3]));
      ym[3] = round(yy1 + cosangle*(-rect1[3]));
    }
    else /* manual position selected (unit is the user coord.) */
    {
      double tmp[2];
      sciGetPosition(ppsubwin->mon_y_label,&tmp[0],&tmp[1]);
	
      x1  = XDouble2Pixel(tmp[0]);
      yy1 = YDouble2Pixel(tmp[1]);
	
      C2F(dr)("xstringl",sciGetText(ppsubwin->mon_y_label),
              &zero,&zero,rect1,&v,&v,&v,&dv,&dv,&dv,&dv,9L,sciGetTextLength(ppsubwin->mon_y_label));
	
      cosangle = cos((360-font_angle)*M_PI/180);
      sinangle = sin((360-font_angle)*M_PI/180);
	
      xm[0] = round(x1);
      xm[1] = round(x1 + cosangle*rect1[2]);
      xm[2] = round(x1 + cosangle*rect1[2] + sinangle*(-rect1[3]));
      xm[3] = round(x1 + sinangle*(-rect1[3]));
  
      ym[0] = round(yy1);
      ym[1] = round(yy1 - sinangle*rect1[2]);
      ym[2] = round(yy1 - sinangle*rect1[2] + cosangle*(-rect1[3]));
      ym[3] = round(yy1 + cosangle*(-rect1[3]));
 	
    }
    
    if(sciGetIsFilled(ppsubwin->mon_y_label) == TRUE)
    {
      x[0] = sciGetBackground(ppsubwin->mon_y_label);
      
      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
      
      C2F (dr) ("xarea", str, &n, xm, ym, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
      
      x[0] = sciGetForeground(ppsubwin->mon_y_label);
      
      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
      
      C2F (dr) ("xlines", "xv", &n, xm, ym, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
    }
    
    x[0] = sciGetFontForeground(ppsubwin->mon_y_label);
    
    C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
    C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
    
    xstringb_angle(sciGetText(ppsubwin->mon_y_label),xm[0],ym[0],rect1[2],rect1[3],font_angle);
  }
  
  C2F (dr) ("xset", "dashes", &old_foreground, &old_foreground, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &old_foreground, &old_foreground, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
  
  C2F(dr)("xset","font",fontid,fontid+1,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);
    
  return 0;
}




int ComputeNbSubTics(sciPointObj * pobj, int nbtics, char logflag, double * grads, int nbsubtics_input)
{
  int ticsval[] =    {2 ,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
  int subticsval[] = {10,7,5,5,4,4,3,2,2 ,2 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 };
  int i;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);


  if(logflag =='l'){
    if((grads[1]-grads[0])==1) /* intervalle de type ...10^n 10^(n+1)...*/
      {
	return 9; /* 9 subtics to have a pretty tics/grid in log.*/
      }
    else
      {
	return 1; /* no subtics at all (1 but draw on a tics place) */
      }
  }
  else{
    if(ppsubwin->flagNax == FALSE) /* if auto subtics mode == ON */
      { 
	for(i=0;i<19;i++)
	  if(nbtics == ticsval[i])
	    {
	      return subticsval[i];
	    }
      }
    else /* if auto subtics mode == OFF already computed in Plo2dn.c, Champ.c routines... */
      {  /* or given via a.subtics=[nbsubtics_on_x, nbsubtics_on_y, nbsubtics_on_z] command */
	return nbsubtics_input;
      }
  }
  
  return -1;
}

/* /\* test on all 3 axes *\/ */
/* BOOL GetIsAxes(sciPointObj *psubwin) */
/* { */
/*   sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin); */
  
/*   if((ppsubwin->axes.axes_visible[0] == FALSE) */
/*      && (ppsubwin->axes.axes_visible[1] == FALSE) */
/*      && (ppsubwin->axes.axes_visible[2] == FALSE)) */
/*     return FALSE; */
/*   else */
/*     return TRUE; */
/* } */

/* test on x and y axes only : used in 2D routines only */
BOOL GetIsAxes2D(sciPointObj *psubwin)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  
  if((ppsubwin->axes.axes_visible[0] == FALSE)
     && (ppsubwin->axes.axes_visible[1] == FALSE))
    return FALSE;
  else
    return TRUE;
}

int  ComputeCorrectXindAndInsideUD(double Theta,double Alpha, double *dbox, integer *xind, integer *InsideU, integer *InsideD)
{
  
  double cost = 0.5 ;
  double sint = 0.5 ;
  double cosa = 0.5 ;
  double sina = 0.5 ;
  double xbox[8], ybox[8], zbox[8];

  if( Theta != 0.0 )
  {
    sciprint("Error: Theta must be 0. to trigger this algo");
    return -1;
  }
  
  Theta = 0.1; /* Little offset to compute correct values for xind,  InsideU and InsideD */

  cost = cos( DEG2RAD(Theta) ) ;
  cosa = cos( DEG2RAD(Alpha) ) ;
  sint = sin( DEG2RAD(Theta) ) ;
  sina = sin( DEG2RAD(Alpha) ) ;
  
  Cscale.m[0][0]= -sint    ;    Cscale.m[0][1]= cost      ;    Cscale.m[0][2]= 0;
  Cscale.m[1][0]= -cost*cosa;   Cscale.m[1][1]= -sint*cosa;    Cscale.m[1][2]= sina;
  Cscale.m[2][0]=  cost*sina;   Cscale.m[2][1]= sint*sina;     Cscale.m[2][2]= cosa;
  
  xbox[0]=TRX(dbox[0],dbox[2],dbox[4]); /* transfo. 3D of [xmin,ymin,zmin] */
  ybox[0]=TRY(dbox[0],dbox[2],dbox[4]); /* into [xbox[0],ybox[0],zbox[0] ] */
  zbox[0]=TRZ(dbox[0],dbox[2],dbox[4]); /*                                 */ 
  
  xbox[1]=TRX(dbox[0],dbox[3],dbox[4]); /* transfo. 3D of [xmin,ymax,zmin] */
  ybox[1]=TRY(dbox[0],dbox[3],dbox[4]); /* into [xbox[1],ybox[1],zbox[1] ] */
  zbox[1]=TRZ(dbox[0],dbox[3],dbox[4]); /*                                 */
  
  xbox[2]=TRX(dbox[1],dbox[3],dbox[4]); /* transfo. 3D of [xmax,ymax,zmin] */
  ybox[2]=TRY(dbox[1],dbox[3],dbox[4]); /* into [xbox[2],ybox[2],zbox[2] ] */
  zbox[2]=TRZ(dbox[1],dbox[3],dbox[4]); /*                                 */
  
  xbox[3]=TRX(dbox[1],dbox[2],dbox[4]); /* transfo. 3D of [xmax,ymin,zmin] */
  ybox[3]=TRY(dbox[1],dbox[2],dbox[4]); /* into [xbox[3],ybox[3],zbox[3] ] */
  zbox[3]=TRZ(dbox[1],dbox[2],dbox[4]); /*                                 */
  
  xbox[4]=TRX(dbox[0],dbox[2],dbox[5]); /* transfo. 3D of [xmin,ymin,zmax] */
  ybox[4]=TRY(dbox[0],dbox[2],dbox[5]); /* into [xbox[4],ybox[4],zbox[4] ] */
  zbox[4]=TRZ(dbox[0],dbox[2],dbox[5]); /*                                 */
  
  xbox[5]=TRX(dbox[0],dbox[3],dbox[5]); /* transfo. 3D of [xmin,ymax,zmax] */
  ybox[5]=TRY(dbox[0],dbox[3],dbox[5]); /* into [xbox[5],ybox[5],zbox[5] ] */
  zbox[5]=TRZ(dbox[0],dbox[3],dbox[5]); /*                                 */
  
  xbox[6]=TRX(dbox[1],dbox[3],dbox[5]); /* transfo. 3D of [xmax,ymax,zmax] */
  ybox[6]=TRY(dbox[1],dbox[3],dbox[5]); /* into [xbox[6],ybox[6],zbox[6] ] */
  zbox[6]=TRZ(dbox[1],dbox[3],dbox[5]); /*                                 */
  
  xbox[7]=TRX(dbox[1],dbox[2],dbox[5]); /* transfo. 3D of [xmax,ymin,zmax] */
  ybox[7]=TRY(dbox[1],dbox[2],dbox[5]); /* into [xbox[7],ybox[7],zbox[7] ] */
  zbox[7]=TRZ(dbox[1],dbox[2],dbox[5]); /*                                 */
  
  
  
  /* indices */
   sciAxesVerticesIndices( InsideU, InsideD, xbox, ybox, xind ) ;
 
  return 0;
}


/* F.Leray 02.11.04 */
/* BuildXYZvectForClipping_IfNanOrLogON : this function is used for polylines to determine if we have to cut the polyline data for 2 reasons: */
/* - clipping is forced because we have a Nan inside the data columns */
/* - clipping is forced because we have a log scale on an axis and polyline data < 0 on this axis (clipping on X11 or Win is based on pixel we compute so if */
/* the algo tried to evaluate log(-5.6) (for example) and then we tried to make a Xscale (or logXscale) on it, it doesn't work at all!! */
/* What we do is: we cut the polyline into several polylines and reject Nan values and data<0 if axis is in log scale */
int  BuildXYZvectForClipping_IfNanOrLogON(sciPointObj *ppolyline, sciPointObj * psubwin, int * nb_curves, double *** xvect, double *** yvect, double *** zvect,int ** curves_size)
{
  int i,j,k;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  sciPolyline * pppolyline = pPOLYLINE_FEATURE( ppolyline);
  int *indexGoodPoints = NULL;
  
  int valeur = 0;
  int nb = 0;
  
  int **store_data = (int **) NULL;
  double * pvx_plus_x_shift = NULL;
  double * pvy_plus_y_shift = NULL;
  double * pvz_plus_z_shift = NULL;

  double * x_shift = pppolyline->x_shift;
  double * y_shift = pppolyline->y_shift;
  double * z_shift = pppolyline->z_shift;
  

  if ( pppolyline->n1 == 0 )
  {
    *nb_curves   = 0    ;
    *xvect       = NULL ;
    *yvect       = NULL ;
    *zvect       = NULL ;
    *curves_size = 0    ;
    return 0 ;
  }

  if ((pvx_plus_x_shift = MALLOC ((pppolyline->n1)*sizeof (double))) == NULL) return -1;
  if(x_shift != (double *) NULL){ /* if shift is not NULL, its size is n1 */
    for(i=0;i<pppolyline->n1;i++)
      pvx_plus_x_shift[i] = pppolyline->pvx[i] + x_shift[i];
  }
  else{
    for(i=0;i<pppolyline->n1;i++)
      pvx_plus_x_shift[i] = pppolyline->pvx[i];
  }
  
    
  if ((pvy_plus_y_shift = MALLOC ((pppolyline->n1)*sizeof (double))) == NULL) return -1;
  if(y_shift != (double *) NULL){ /* if shift is not NULL, its size is n1 */
    for(i=0;i<pppolyline->n1;i++)
      pvy_plus_y_shift[i] = pppolyline->pvy[i] + y_shift[i];
  }
  else{
    for(i=0;i<pppolyline->n1;i++)
      pvy_plus_y_shift[i] = pppolyline->pvy[i];
  }
    
  
  if(pppolyline->pvz != (double *) NULL || z_shift != (double *) NULL){
    if ((pvz_plus_z_shift = MALLOC ((pppolyline->n1)*sizeof (double))) == NULL) return -1;

    if(z_shift != (double *) NULL && pppolyline->pvz != (double *) NULL){ /* if shift is not NULL, its size is n1 */
      for(i=0;i<pppolyline->n1;i++)
	pvz_plus_z_shift[i] = pppolyline->pvz[i] + z_shift[i];
    }
    else if(z_shift != (double *) NULL && pppolyline->pvz == (double *) NULL){
      for(i=0;i<pppolyline->n1;i++)
	pvz_plus_z_shift[i] = z_shift[i];
    }
    else if(z_shift == (double *) NULL && pppolyline->pvz != (double *) NULL){
      for(i=0;i<pppolyline->n1;i++)
	pvz_plus_z_shift[i] = pppolyline->pvz[i];
    }
  }
  

  if ((store_data = (int **) MALLOC ((3)*sizeof (int *))) == NULL) return -1;
  for(i=0;i<3;i++) 
    if ((store_data[i] = MALLOC ((pppolyline->n1)*sizeof (int))) == NULL) return -1;
  
  if ((indexGoodPoints = MALLOC ((pppolyline->n1)*sizeof (integer))) == NULL) return -1;
  for(i=0;i<pppolyline->n1;i++) indexGoodPoints[i] = pppolyline->n1 + 1000;
  
  /* ICI dans mon exemple plot2d indexGoodPoints[i] = 63+1 = 64 */
  
  /*  we search for != %nan */
  for(i=0;i<pppolyline->n1;i++)
    {
      if (pvz_plus_z_shift == (double *) NULL)
	{
	  if((finite(pvx_plus_x_shift[i]) == 1) &&
	     (finite(pvy_plus_y_shift[i]) == 1))
	    indexGoodPoints[i] = 1;  /* x and y are finite numbers */
	  else
	    indexGoodPoints[i] = -1;
	} 
      else
	{
	  if((finite(pvx_plus_x_shift[i]) == 1) &&
	     (finite(pvy_plus_y_shift[i]) == 1) &&
	     (finite(pvz_plus_z_shift[i]) == 1))
	    indexGoodPoints[i] = 1;
	  else
	    indexGoodPoints[i] = -1;
	  
	}
    }
  

  /* we search for values <= 0 */
  for(i=0;i<pppolyline->n1;i++)
    {
      if(ppsubwin->logflags[0] == 'l')
	{
	  if((indexGoodPoints[i] == 1) && (pvx_plus_x_shift[i] <= 0))
	    indexGoodPoints[i] = -1;
	}
      
      if(ppsubwin->logflags[1] == 'l') 
	{
	  if((indexGoodPoints[i] == 1) && (pvy_plus_y_shift[i] <= 0))
	    indexGoodPoints[i] = -1;
	}
      
      if(pppolyline->pvz != NULL)
	if(ppsubwin->logflags[2] == 'l') 
	  {
	    if((indexGoodPoints[i] == 1) && (pvz_plus_z_shift[i] <= 0))
	      indexGoodPoints[i] = -1;
	  }
      
    }
  
  valeur = indexGoodPoints[0]; /* -1 ou 1 */
  
  
  *nb_curves = 0;
  if(valeur == 1){ /* we begin by a draw point/line */
    *nb_curves = 0;
    
    j=0;
    
    while(j<pppolyline->n1)
      {
       	
	for(i=j;i<pppolyline->n1;i++)
	  if(indexGoodPoints[i] == 1)
	    continue;
	  else
	    break;

	store_data[0][(*nb_curves)] = j;
	store_data[1][(*nb_curves)] = i;
	store_data[2][(*nb_curves)] = i-j;

	*nb_curves = *nb_curves + 1; /* STOCKER AUSSI LES INDEXES EXTREMITES OU C'EST EGAL A  1 !! */
	

	for(k=i;k<pppolyline->n1;k++)
	  if(indexGoodPoints[k] == -1)
	    continue;
	  else
	    break;
      
	j=k;

      }
  }
  else if(valeur == -1){/* we begin with a not drawn point/line */
    *nb_curves = 0;
    
    j=0;
    
    while(j<pppolyline->n1)
      {
       	
	for(i=j;i<pppolyline->n1;i++)
	  if(indexGoodPoints[i] == -1)
	    continue;
	  else
	    break;

	for(k=i;k<pppolyline->n1;k++)
	  if(indexGoodPoints[k] == 1)
	    continue;
	  else
	    break;

	store_data[0][(*nb_curves)] = i;
	store_data[1][(*nb_curves)] = k;
	store_data[2][(*nb_curves)] = k-i;
	

	*nb_curves = *nb_curves + 1;
	j=k;

      }
  }
  else{
    printf("Impossible case in CheckClippingNanLogON\n");
  }
  
  
  nb = *nb_curves;

  /* XYZ vect building */
  if (( (*xvect) = (double **) MALLOC ((nb)*sizeof (double *))) == NULL) return -1;
  if (( (*yvect) = (double **) MALLOC ((nb)*sizeof (double *))) == NULL) return -1;
  if (( (*zvect) = (double **) MALLOC ((nb)*sizeof (double *))) == NULL) return -1;
    
  for(i=0;i<nb;i++)
    {
      int cmpteur = 0;
      /* Allocating arrays x, y and zvect */
      if(store_data[2][i] > 0){
	if (( (*xvect)[i] = (double *) MALLOC ((store_data[2][i])*sizeof (double))) == NULL) return -1;
	if (( (*yvect)[i] = (double *) MALLOC ((store_data[2][i])*sizeof (double))) == NULL) return -1;
	if(pvz_plus_z_shift == NULL)
	  (*zvect)[i] = NULL;
	else
	  if (( (*zvect)[i] = (double *) MALLOC ((store_data[2][i])*sizeof (double))) == NULL) return -1;
      }
      for(j=store_data[0][i];j<store_data[1][i];j++)
	{
	  (*xvect)[i][cmpteur] = pvx_plus_x_shift[j];
	  (*yvect)[i][cmpteur] = pvy_plus_y_shift[j];
	  if(pvz_plus_z_shift != NULL)
	    (*zvect)[i][cmpteur] = pvz_plus_z_shift[j];
	  
	  cmpteur++;
	  
	}
    }
  
  if (( (*curves_size) = (int *) MALLOC ((nb)*sizeof (int))) == NULL) return -1;
  
  for(i=0;i<nb;i++)
    {
      (*curves_size)[i] = store_data[2][i];
    }
  
  
  for(i=0;i<3;i++){
    FREE(store_data[i]); 
    store_data[i] = (int *) NULL;
  }
  
  FREE(store_data); store_data = (int **) NULL;
  FREE(indexGoodPoints); indexGoodPoints = (int *) NULL;
  FREE(pvx_plus_x_shift); pvx_plus_x_shift = (double *) NULL;
  FREE(pvy_plus_y_shift); pvy_plus_y_shift = (double *) NULL;
  FREE(pvz_plus_z_shift); pvz_plus_z_shift = (double *) NULL;
  return 0;
}


/* don't waste time on %NaN */
int CheckIfiisNan(int j, int dim, int * tab)
{
  int i;
  
  for(i=0;i<dim;i++)
    {
      if(tab[i] == j)
	return -1;
    }
  
  return 0;
}





int ComputeGoodTrans3d( sciPointObj * psubwin, int n, int *xm, int *ym, double * fx, double *fy, double *fz)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);

  
  double tmp_fx = *fx;
  double tmp_fy = *fy;
  double tmp_fz = *fz;
  
  if(ppsubwin->logflags[0] == 'l')
    tmp_fx = exp10(tmp_fx);
  
  if(ppsubwin->logflags[1] == 'l')
    tmp_fy = exp10(tmp_fy);
  
  if(ppsubwin->logflags[2] == 'l')
    tmp_fz = exp10(tmp_fz);
  
  trans3d(psubwin, n, xm, ym, &tmp_fx, &tmp_fy, &tmp_fz);
  
  return 0;
}


double InvAxis(double min, double max, double u)
{
  return (u-min)/(max-min)*min + (u-max)/(min-max)*max;
}


int ReverseDataFor3DXonly(sciPointObj * psubwin, double * xvect, int n1)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  int cmp;

  if(ppsubwin->axes.reverse[0] == TRUE){
    /* agir sur x */
    if(ppsubwin->logflags[0]=='l'){
      for(cmp=0;cmp<n1;cmp++)
	{
	  xvect[cmp] = log10(xvect[cmp]);
	  xvect[cmp] = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xvect[cmp]);
	  xvect[cmp] = exp10(xvect[cmp]);
	}
    }
    else
      for(cmp=0;cmp<n1;cmp++) 
	xvect[cmp] =  InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xvect[cmp]);
  }
  
  return 0;
}

int ReverseDataFor3DYonly(sciPointObj * psubwin, double * yvect, int n1)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  int cmp;

  if(ppsubwin->axes.reverse[1] == TRUE){
    /* agir sur y */
    if(ppsubwin->logflags[1]=='l'){
      for(cmp=0;cmp<n1;cmp++)
	{
	  yvect[cmp] = log10(yvect[cmp]);
	  yvect[cmp] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],yvect[cmp]);
	  yvect[cmp] = exp10(yvect[cmp]);
	}
    }
    else
      for(cmp=0;cmp<n1;cmp++)
	yvect[cmp] =  InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],yvect[cmp]);
  }
    
  return 0;
}


int ReverseDataFor3DZonly(sciPointObj * psubwin, double * zvect, int n1)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  int cmp;

  if(zvect != NULL){
    if(ppsubwin->axes.reverse[2] == TRUE){
      /* agir sur z */
      if(ppsubwin->logflags[2]=='l'){
	for(cmp=0;cmp<n1;cmp++)
	  {
	    zvect[cmp] = log10(zvect[cmp]);
	    zvect[cmp] = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],zvect[cmp]);
	    zvect[cmp] = exp10(zvect[cmp]);
	  }
      }
      else
	for(cmp=0;cmp<n1;cmp++)
	  zvect[cmp] =  InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],zvect[cmp]);
    }
  }
  
  return 0;
}

int ReverseDataFor3D(sciPointObj * psubwin, double * xvect, double * yvect, double * zvect, int n1)
{
  
  ReverseDataFor3DXonly(psubwin, xvect, n1);
  ReverseDataFor3DYonly(psubwin, yvect, n1);
  ReverseDataFor3DZonly(psubwin, zvect, n1);
  
  return 0;
}


/**DrawAxesIfRequired
 * @memo Draws Axes (only the basic  graphicobject under subwindows) in its SubWindow or figure
 * if and only if pFIGURE_FEATURE(pobj)->auto_redraw == TRUE !!
 * Only used inside High Level functions calls (sucha as plot2d, plot3d...)
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, -1 if not
 */
void DrawAxesIfRequired(sciPointObj * pobj)
{
  sciPointObj * pfigure = sciGetParentFigure(pobj);
  
  if(pFIGURE_FEATURE(pfigure)->auto_redraw == TRUE && pFIGURE_FEATURE(pfigure)->visible == TRUE)
    DrawAxes(pobj);
  
}

/* Routine used inside Plo2dn.c, Champ.c, Gray.c... */
/* to force the drawing of the axes after a new object is created */
void DrawAxes(sciPointObj * pobj)
{
  sciPointObj * psubwin = sciGetParentSubwin(pobj);
  /*   sciPointObj * pfigure = sciGetParentFigure(pobj); */
  char STRFLAG[4];
  integer x[6], v, markidsizenew[2];
  double dv;

  if (!sciGetVisibility(pobj)) return;
  
  x[0] = sciGetForeground (psubwin);
  x[2] = sciGetLineWidth (psubwin);
  x[3] = sciGetLineStyle (psubwin);
  markidsizenew[0] = sciGetMarkStyle(psubwin);
  markidsizenew[1] = sciGetLineWidth (psubwin);
  x[4] = 0;
  v = 0;
  dv = 0;

  
  sciSetSelectedSubWin(psubwin); 
  
  set_scale ("tttftt", pSUBWIN_FEATURE (psubwin)->WRect, pSUBWIN_FEATURE (psubwin)->FRect,
	     NULL, pSUBWIN_FEATURE (psubwin)->logflags, 
	     pSUBWIN_FEATURE (psubwin)->ARect); 
     
  if (!pSUBWIN_FEATURE (psubwin)->is3d)   /* we are in 2D mode...*/
    {
      /* F.Leray 07.12.04 */
      /* TO CORRECT the bug 1115 : Big object (grayplots) could cover axes*/
      C2F (dr) ("xset","dashes",x,x,x+4,x+4,x+4,&v,&dv,&dv,&dv,&dv,5L,4096);
      C2F (dr) ("xset","foreground",x,x,x+4,x+4,x+4,&v,&dv,&dv,&dv,&dv,5L,4096);
      C2F (dr) ("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F (dr) ("xset","mark",&markidsizenew[0],&markidsizenew[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      rebuild_strflag(psubwin,STRFLAG);
      axis_draw2 (STRFLAG); /* Axes is rebuilt here to avoid being covered by the new created object */
    }
}


/* Used only when switching from a plot3d to a plot2d or 
 * when typing: a=gca();
 *              a.view='2d' and we were in 3d 
 * to have a good update of 3d graphics scale in 2D
 * whenever we are in auto_redraw OFF  */
void UpdateSubwinScale(sciPointObj * pobj)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);
  sciPointObj * currentsubwin = NULL;
  BOOL vis_save = FALSE;
  double xbox[8],ybox[8],zbox[8];
  static integer InsideU[4],InsideD[4];
  	
  currentsubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());

  sciSetSelectedSubWin(pobj); 
  
  /*   set_scale ("tttftt", pSUBWIN_FEATURE (pobj)->WRect, pSUBWIN_FEATURE (pobj)->FRect, */
  /* 	     NULL, pSUBWIN_FEATURE (pobj)->logflags,  */
  /* 	     pSUBWIN_FEATURE (pobj)->ARect);  */
  
  if (ppsubwin->is3d) 
    {  /* 3D Coordinates */ /* verifier si c'est encore utile SS */
      /*To have directly all the possible ISOVIEW Modes*/
      long isoflag = (long)(ppsubwin->axes.flag[1]+1)/2; 
      
      if(ppsubwin->isoview == TRUE) {
	if(isoflag ==2 || isoflag == 3){		}
	else {
	  if((ppsubwin->axes.flag[1] == 0)
	     || (ppsubwin->axes.flag[1] == 2))
	    /* The default isoview mode is type=4 3d isometric bounds 
	       derived from the data, to similarily type=2  */
	    ppsubwin->axes.flag[1] = 4; 
	  else if(ppsubwin->axes.flag[1] == 1)
	    ppsubwin->axes.flag[1] = 3;
	}
      }
      else {
	if((ppsubwin->axes.flag[1] == 3) 
	   || (ppsubwin->axes.flag[1] == 5))
	  ppsubwin->axes.flag[1] = 1; /* computed from ebox*/
	else if((ppsubwin->axes.flag[1] == 4) 
		|| (ppsubwin->axes.flag[1] == 6))
	  /* The default NON-isoview mode is 2 computed from data*/
	  ppsubwin->axes.flag[1] = 2; 
      }
      

      vis_save = sciGetVisibility(pobj);

      sciSetVisibility(pobj,FALSE);
      axis_3ddraw(pobj,xbox,ybox,zbox,InsideU,InsideD); /* TEST on sciGetVisibility inside */
      /* because axis_3ddraw displays 3d axes BUT ALSO compute + reset the 3d scale BEFORE !! */
      sciSetVisibility(pobj,vis_save);
    }
  else /* we are in 2D mode...*/
    {
      sci_update_frame_bounds_2d(pobj);
    }

  sciSetSelectedSubWin(currentsubwin); 
  
}



/**sciClip
 * @memo Set the clipping rectangle 
 * @author Djalel ABDEMOUCHE
 * 29/11/2002
 */
/* Note that clipping is not supported at all in 3D mode for now : */
/* This assertion includes axes (in 3D), surfaces and parametric curves objects. */

void
sciClip (sciPointObj *pobj)
{
  int x,y,w,h; 
  int value;
  double *clip_region = NULL;
  
  sciPointObj * psubwin = sciGetParentSubwin(pobj);
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  value = sciGetIsClipping(pobj);     /* clipping state */
  
  if(value == -1) return;
  if(ppsubwin->is3d == TRUE) return; /* no clipping in 3d */
  
  clip_region = sciGetClipping(pobj); /* clipping region */
  
  
  if(sciGetIsClipRegionValuated(pobj) == 0)
    value = 0; /* we use the 'clipgrf' value instead */
  
  if (value == 0){
    double clip[4];
    double tmpx, tmpy, tmpw, tmph;
    
    tmpw = fabs(ppsubwin->FRect[2] - ppsubwin->FRect[0]);
    tmph = fabs(ppsubwin->FRect[3] - ppsubwin->FRect[1]);
    
    tmpx = ppsubwin->FRect[0]; /* xmin */
    tmpy = ppsubwin->FRect[3]; /* ymax */
    
    clip[0] = tmpx;
    clip[1] = tmpy;
    clip[2] = tmpw;
    clip[3] = tmph;
    
    sciSetClipping(pobj,clip);

    frame_clip_on();
  }
  else if (value > 0)
    { 
      double tmpw, tmph;
      double tmpx, tmpy;


      tmpw = clip_region[2];
      tmph = clip_region[3];
      
      tmpx = clip_region[0];
      tmpy = clip_region[1];
      
      if(ppsubwin->axes.reverse[0] == TRUE)
	tmpx = tmpx + tmpw;
      
      if(ppsubwin->axes.reverse[1] == TRUE)
	tmpy = tmpy - tmph;
      
      
      x = XDouble2Pixel( tmpx);
      y = YDouble2Pixel( tmpy);
      w = WDouble2Pixel( tmpx, tmpw);
      h = HDouble2Pixel( tmpy, tmph);
      C2F(dr)("xset","clipping",&x, &y, &w, &h,PI0,PI0,PD0,PD0,PD0,PD0,4L,8L);
    }
}

/**sciClip
 * @memo Clear the clipping rectangle 
 * @author Djalel ABDEMOUCHE
 * 29/11/2002
 */
void
sciUnClip (sciPointObj * pobj)
{
/*   sciPointObj * psubwin = sciGetParentSubwin(pobj); */
  int value = sciGetIsClipping(pobj);     /* clipping state */
  
  /*  if (value > -1 && !pSUBWIN_FEATURE(psubwin)->is3d)*/
  if (value > -1)
    C2F(dr)("xset","clipoff",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,7L);
}


int Gen3DPoints(integer type,integer *polyx, integer *polyy, integer *fill, integer whiteid, double zmin, double zmax, double *x, double *y, double *z, integer i, integer j, integer jj1, integer *p, integer dc, integer fg, sciPointObj * psurface)
{
  sciPointObj *pobj;
  int facteur = 1;

  sciPointObj *psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  sciSurface * ppsurface = pSURFACE_FEATURE (psurface);
  
  pobj = sciGetSelectedSubWin (sciGetCurrentFigure ()); 
  if (trans3d(pobj ,1, &(polyx[  5*jj1]),&(polyy[  5*jj1]),&(x[i]),&(y[j]),&(z[i+(*p)*j]))==0) return 0; 
  if (trans3d(pobj ,1, &(polyx[1+  5*jj1]),&(polyy[1+  5*jj1]),&(x[i]),&(y[j+1]),&(z[i+(*p)*(j+1)]))==0) return 0; 
  if (trans3d(pobj ,1, &(polyx[2+  5*jj1]),&(polyy[2+  5*jj1]),&(x[i+1]),&(y[j+1]),&(z[(i+1)+(*p)*(j+1)]))==0) return 0; 
  if (trans3d(pobj ,1, &(polyx[3+  5*jj1]),&(polyy[3+  5*jj1]),&(x[i+1]),&(y[j]),&(z[(i+1)+(*p)*j]))==0) return 0;   
  if (trans3d(pobj ,1, &(polyx[4+  5*jj1]),&(polyy[4+  5*jj1]),&(x[i]),&(y[j]),&(z[i+(*p)*j]))==0) return 0; 
  
  
  if(ppsubwin->axes.reverse[0] == TRUE) facteur = -facteur;
  if(ppsubwin->axes.reverse[1] == TRUE) facteur = -facteur;
  if(ppsubwin->axes.reverse[2] == TRUE) facteur = -facteur;
  
  /* try */
  facteur = ppsurface->flag_x * ppsurface->flag_y * facteur;
  
  /* type == flagcolor and dc == color_mode */
  /* fg = hidden color */
  
  if ((((polyx[1+5*jj1]-polyx[0+5*jj1])*(polyy[2+5*jj1]-polyy[0+5*jj1])-
	(polyy[1+5*jj1]-polyy[0+5*jj1])*(polyx[2+5*jj1]-polyx[0+5*jj1]))*facteur <  0) && (fg >=0 )) 
    {
      
      /* ------------------- */
      /* Beneath the surface */
      /* ------------------- */

      if (type != 0) /* flagcolor = 1 case : special treatment compared to flagcolor = 0 */
	             /* don't know why... F.Leray */
	fill[jj1]= (dc < 0 ) ? -fg : fg ;
      else           /* flagcolor = 0 :  No shading at all, fixed facecolor. */
	fill[jj1]=  (dc != 0 ) ? fg : dc ;
    }
  else
    {

      /* ------------------- */
      /* Above the surface */
      /* ------------------- */

     if (type != 0)
	{  /* flagcolor = 1 :  Z-level flat shading. */
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



int Merge3dDimension(sciPointObj *pparent)
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
      if ( pPOLYLINE_FEATURE(psonstmp->pointobj)->n1 == 0 )
      {
        N = 0 ;
      }
      else if (pPOLYLINE_FEATURE (psonstmp->pointobj)->plot != 5)
      {/*polyline*/
        N = pPOLYLINE_FEATURE (psonstmp->pointobj)->n1-1 ;
	    if ((pPOLYLINE_FEATURE (psonstmp->pointobj)->plot != 2) && 
	         (sciGetIsMark((sciPointObj *)psonstmp->pointobj) == 1))
        {
	      N=N+1;
        }
      }
      else /* patch */
      {
        N = 1 ;
      }
      break;
    case  SCI_SEGS: 
      {
        sciSegs * ppSegs = pSEGS_FEATURE(psonstmp->pointobj) ;
        if ( ppSegs->ptype == 0 )
        { 
          N = ppSegs->Nbr1 / 2 ;
        }
        else
        {
          N = ppSegs->Nbr1 * ppSegs->Nbr2 ;
        }
      }
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

int ChildrenCounter(sciPointObj *pparent)
{
  int N,q=0;
  sciSons * psonstmp = sciGetSons (pparent);
  
  while (psonstmp != (sciSons *) NULL) {   
    switch (sciGetEntityType (psonstmp->pointobj)) {  
    case SCI_SURFACE:
      N=1;
      break;
    case  SCI_POLYLINE:
      N = 1; 
      break;
    case  SCI_SEGS: 
      N=1;
      break;
    case  SCI_RECTANGLE: 
      N = 1;
      break;
    case SCI_AGREG:
      N = ChildrenCounter(psonstmp->pointobj);
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
      if ( pPOLYLINE_FEATURE(psonstmp->pointobj)->n1 == 0 )
      {
        N = 0 ;
      }
      else if (pPOLYLINE_FEATURE (psonstmp->pointobj)->plot != 5)
      {/*polyline*/
        N = pPOLYLINE_FEATURE (psonstmp->pointobj)->n1-1 ;
	    if ((pPOLYLINE_FEATURE (psonstmp->pointobj)->plot != 2) && 
	         (sciGetIsMark((sciPointObj *)psonstmp->pointobj) == 1))
        {
	      N=N+1;
        }
      }
      else /* patch */
      {
        N = 1 ;
      }
      break;
    case  SCI_SEGS: 
      {
        sciSegs * ppSegs = pSEGS_FEATURE(psonstmp->pointobj) ;
        if ( ppSegs->ptype == 0 )
        { 
          N = ppSegs->Nbr1 / 2 ;
        }
        else
        {
          N = ppSegs->Nbr1 * ppSegs->Nbr2 ;
        }
      }
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
  if ((index_in_entity = (int *) MALLOC (q * sizeof (int))) == (int *)NULL) {
    Scistring("Merge3d : not enough memory to allocate \n");
    return;
  }
  if ((from_entity   = (long *) MALLOC (q * sizeof (long))) == (long *) NULL) {
    Scistring("Merge3d : not enough memory to allocate \n");
    FREE(index_in_entity);
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
    FREE(index_in_entity);
    FREE(from_entity);
    sciprint ("\r\n No merge supported");}
  else /* inform the subwindow to display Merge instead of individual children */
    pSUBWIN_FEATURE (psubwin)->facetmerge = TRUE;
  
}

/*------------------------------------------------------------------------------------------*/
/**
 * get the coordiantes of the vertices of a facet in a FAC3D object
 * @param[in/out] pobj       surface object of type Fac3d
 * @param         facetIndex index of the facet in the object
 * @param[out]    verticesX  list of X coordinates of the vertices
 * @param[out]    verticesY  list of Y coordinates of the vertices
 * @param[out]    verticesZ  list of Z coordinates of the vertices
 * @param[out]    zOriginal  pointer on the Z coordinate of the first vertex in the fac3d object
 */
void retrieveFacetVertices( sciPointObj *  pobj       ,
                            int            facetIndex ,
                            double         verticesX[],
                            double         verticesY[],
                            double         verticesZ[],
                            double      ** zOriginal   )
{
  int nbFacets = pSURFACE_FEATURE (pobj)->dimzx ;
  int i ;

  for ( i = 0 ; i < nbFacets ; i++ )
  {
    verticesX[i] = pSURFACE_FEATURE(pobj)->pvecx[facetIndex * nbFacets + i] ;
    verticesY[i] = pSURFACE_FEATURE(pobj)->pvecy[facetIndex * nbFacets + i] ;
    verticesZ[i] = pSURFACE_FEATURE(pobj)->pvecz[facetIndex * nbFacets + i] ;
  }
  
  ReverseDataFor3D( pobj, verticesX, verticesY, verticesZ, nbFacets ) ;

  *zOriginal = &(pSURFACE_FEATURE(pobj)->pvecz[facetIndex * nbFacets]) ;
}
/*------------------------------------------------------------------------------------------*/

void DrawMerge3d(sciPointObj *psubwin, sciPointObj *pmerge, int * DPI)
{
  int N,i,j,index,p,max_p,n1,npoly;
  double * dist;
  double X[5],Y[5],Z[5];
  double * Zoriginal = NULL; /* used to conserve Z wether or not z axis is reversed ! (see plo3dn.c) */
  double * x = NULL ;
  double * y = NULL ; 
  double * z = NULL ;
  sciPointObj *pobj; 
  int *locindex;
  int *polyx,*polyy,fill[20];/* here we suppose there is no more than 20 edge in a facet */
  int k1,iflag;
  int pstyle = 0;
  int whiteid,verbose=0,narg;
  static double zmin,zmax,xmoy,ymoy,zmoy,zl;
  int context[6];
  

  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  int u;
  /* change to static arrays : indeed, no need to recopy each time the entire data of a given object */
  double xtmp[2] ; 
  double ytmp[2] ;
  double ztmp[4] ;

  double * verticesX = NULL ; /* normally max size is 4 for facets (2 for lines and segs) but may be one day we will manage greater complex patchs (that is why the 10) */
  double * verticesY = NULL ;  /* no it can have any size, but display of facets with more than 4 edges might not be accurate */
  double * verticesZ = NULL ;
  
#ifdef WIN32 
  int hdcflag;
#endif

  if(sciGetEntityType (psubwin) != SCI_SUBWIN) return;
  N=pMERGE_FEATURE (pmerge)->N; /* total number of elements */
  
  if ((dist=(double *)MALLOC(N*sizeof(double)))==(double *) NULL) {
    Scistring("DrawMerge3d : MALLOC No more Place\n");
    return;
  }
  if ((locindex=(int *)MALLOC(N*sizeof(int)))==(int *) NULL) {
    Scistring("DrawMerge3d : MALLOC No more Place\n");
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
      if ( pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D )
      { /* x,y,Z */
	int l,k,n1,n2;

	n1= pSURFACE_FEATURE (pobj)->dimzx;
	n2= pSURFACE_FEATURE (pobj)->dimzy;
	l=(int)(index/(n1-1));
	k=index-l*(n1-1);

	xtmp[0] = pSURFACE_FEATURE (pobj)->pvecx[k];
	xtmp[1] = pSURFACE_FEATURE (pobj)->pvecx[k+1];
	
	/* I didn't use ReverseDataFor3D because dim n1 is not the same for x, y and z  */
	if(ppsubwin->axes.reverse[0] == TRUE){
	  /* agir sur x */
	  if(ppsubwin->logflags[0]=='l'){
	    for(u=0;u<2;u++){
	      xtmp[u] = log10(xtmp[u]);
	      xtmp[u] = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp[u]);
	      xtmp[u] = exp10(xtmp[u]);
	    }
	  }
	  else {
	    xtmp[0] = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp[0]);
	    xtmp[1] = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp[1]);
	  }
	}

	ytmp[0] = pSURFACE_FEATURE (pobj)->pvecy[l];
	ytmp[1] = pSURFACE_FEATURE (pobj)->pvecy[l+1];

	if(ppsubwin->axes.reverse[1] == TRUE){ 
	  /* agir sur y */
	  if(ppsubwin->logflags[1]=='l'){
	    for(u=0;u<2;u++){
	      ytmp[u] = log10(ytmp[u]);
	      ytmp[u] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp[u]);
	      ytmp[u] = exp10(ytmp[u]);
	    }
	  }
	  else{
	    ytmp[0] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp[0]);
	    ytmp[1] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp[1]);
	  }
	}

	ztmp[0] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1];
	ztmp[1] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1+n1];
	ztmp[2] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1+n1+1];
	ztmp[3] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1+1];


	if(ppsubwin->axes.reverse[2] == TRUE){ 
	  /* agir sur z */
	  if(ppsubwin->logflags[2]=='l'){
	    for(u=0;u<4;u++){
	      ztmp[u] = log10(ztmp[u]);
	      ztmp[u] = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp[u]);
	      ztmp[u] = exp10(ztmp[u]);
	    }
	  }
	  else{
	    for(u=0;u<4;u++)
	      ztmp[u] = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp[u]);
	  }
	}
  
	n2= pSURFACE_FEATURE (pobj)->dimzy;
	X[0]=X[1]=xtmp[0];
	X[2]=X[3]=xtmp[1];

	Z[0]=ztmp[0];
	Z[1]=ztmp[1];
	Z[2]=ztmp[2];
	Z[3]=ztmp[3];

	Y[0]=Y[3]=ytmp[0];
	Y[1]=Y[2]=ytmp[1];

	Zoriginal = &(pSURFACE_FEATURE (pobj)->pvecz[k+l*n1]);
	
	p=4;
	x=X;y=Y;z=Z;
	
      }
      else
      {
        /* facets */
        p = pSURFACE_FEATURE (pobj)->dimzx; /* number of edges in the facets */

        verticesX = MALLOC( p * sizeof(double) ) ;
        verticesY = MALLOC( p * sizeof(double) ) ;
        verticesZ = MALLOC( p * sizeof(double) ) ;

        if ( verticesX == NULL || verticesY == NULL || verticesZ == NULL )
        {
          FREE(verticesX) ;
          FREE(verticesY) ;
          FREE(verticesZ) ;
          sciprint( "sciDrawObj: Unable to allocate temporary vector, memory full.\n" ) ;
          return ;
        }

        retrieveFacetVertices( pobj, index, verticesX, verticesY, verticesZ, &Zoriginal ) ;
                            
	x = verticesX ; 
	y = verticesY ;
	z = verticesZ ;
	
      }
      break;
    case  SCI_POLYLINE:
    {
      sciPolyline * ppPolyLine = pPOLYLINE_FEATURE (pobj) ;
      n1 = ppPolyLine->n1 ;
      p  = 0 ;
      if ( sciGetIsMark(pobj) ) { p = 1 ; } /* F.Leray 20.01.05 A REVOIR ICI*/
      if ( sciGetIsLine(pobj) ) { p = 2 ; }
      
      if ( ppPolyLine->plot != 2 && sciGetIsMark(pobj) == 1 )
      {
        xtmp[0] = ppPolyLine->pvx[index];
        xtmp[1] = xtmp[0] ;
        
        ytmp[0] = ppPolyLine->pvy[index];
        ytmp[1] = ytmp[0] ; /* used by trans3d + drawing : case 0,1 and 4 */
       
        if( ppPolyLine->pvz != NULL )
        {
          ztmp[0] = ppPolyLine->pvz[index];
          ztmp[1] = ztmp[0];
        }
      }
      else
      {
        xtmp[0] = ppPolyLine->pvx[index];
        xtmp[1] = ppPolyLine->pvx[index+1];
        
        ytmp[0] = ppPolyLine->pvy[index];
        ytmp[1] = ppPolyLine->pvy[index+1]; /* used by trans3d + drawing : case 0,1 and 4 */
        if( ppPolyLine->pvz != NULL )
        {
          ztmp[0] = ppPolyLine->pvz[index];
          ztmp[1] = ppPolyLine->pvz[(index+1)%n1];
        }
      }
      
      if( ppPolyLine->pvz != NULL )
      {
	ReverseDataFor3D(psubwin,xtmp,ytmp,ztmp,2);
      }
      else
      {
	ReverseDataFor3D(psubwin,xtmp,ytmp,(double *) NULL,2);
      }
      
      switch ( ppPolyLine->plot )
      {
      case 0: case 1: case 4: /*linear interpolation */
	x=&(xtmp[0]);
	y=&(ytmp[0]);
	if ( ppPolyLine->pvz != (double *) NULL) 
	  z=&(ztmp[0]);
	else
	  z=(double *)NULL;
	break;
      case 2: /* staircase */ /* just for completion  */
	X[0]=xtmp[0];
	X[1]=xtmp[1];
	Y[0]=ytmp[0];
	Y[1]=ytmp[0];
	if (ppPolyLine->pvz != (double *) NULL) {
	  Z[0]=ztmp[0];
	  Z[1]=ztmp[0];
	  z=Z;
	}
	else 
	  z=(double *)NULL;
	x=X;y=Y;
	break;
      case 3 : /* vertical bar */ /* just for completion  */
	X[0]=xtmp[0];
	X[1]=xtmp[0];
	Y[0]=0.0;
	if(ppsubwin->logflags[1]=='l') /* when logscale on Y, special treatment because we can not have Y == 0 */
	  Y[0] = ppsubwin->FRect[1];
	Y[1]=ytmp[0];
	if ( ppPolyLine->pvz != (double *) NULL) {
	  Z[0]=ztmp[0];
	  Z[1]=ztmp[0];
	  z=Z;
	}
	else 
	  z=(double *)NULL;
	x=X;y=Y;
	break;
      case 5: /* patch */
	x=xtmp;
	y=ytmp;
	if ( ppPolyLine->pvz != (double *) NULL)
	  z=ztmp;
	else
	  z= (double *) NULL;
	break;
      }
    }
      break;
    case SCI_SEGS: 
      {
        sciSegs * ppSegs = pSEGS_FEATURE(pobj) ;
        p = 2;
      /***************/
        if ( ppSegs->ptype == 0 )
        {
          xtmp[0] = ppSegs->vx[2*index];
          xtmp[1] = ppSegs->vx[2*index+1];

          ytmp[0] = ppSegs->vy[2*index];
          ytmp[1] = ppSegs->vy[2*index+1];

          if( ppSegs->vz != NULL )
          {
            ztmp[0] = ppSegs->vz[2*index];
            ztmp[1] = ppSegs->vz[2*index+1];
          }

        }
        else
        {
          /* vx is size n1, vy n2 and vfx and vfy n1 x n2 */
          int column = index / ppSegs->Nbr1 ;
          int row    = index - column * ppSegs->Nbr1 ; 
          double maxGap = 0.707 * computeGridMinGap( ppSegs->vx, ppSegs->vy, ppSegs->Nbr1, ppSegs->Nbr2 ) ;

          xtmp[0] = ppSegs->vx[row] ;
          ytmp[0] = ppSegs->vy[column] ;

          if (ppSegs->typeofchamp == 0 )
          {
            double maxNorm = getLongestVector( ppSegs->vfx, ppSegs->vfy, ppSegs->Nbr1, ppSegs->Nbr2, 1.0, 1.0 ) ;
            xtmp[1] = ppSegs->vx[row]    + maxGap * ppSegs->vfx[index] / maxNorm ;
            ytmp[1] = ppSegs->vy[column] + maxGap * ppSegs->vfy[index] / maxNorm ;
          }
          else
          {
            /* colored */
            double norm = sqrt( ppSegs->vfx[index] * ppSegs->vfx[index] + ppSegs->vfy[index] * ppSegs->vfy[index] ) ;
            xtmp[1] = ppSegs->vx[row]    + maxGap * ppSegs->vfx[index] / norm ;
            ytmp[1] = ppSegs->vy[column] + maxGap * ppSegs->vfy[index] / norm ; ;
          }


          if( ppSegs->vz != NULL )
          {
            ztmp[0] = ppSegs->vz[row] ;
            ztmp[1] = ppSegs->vz[row] + ppSegs->vfz[index] ;
          }

        }

        if( ppSegs->vz != NULL )
        {
          ReverseDataFor3D(psubwin,xtmp,ytmp,ztmp,2);
        }
        else
        {
          ReverseDataFor3D(psubwin,xtmp,ytmp, NULL,2);
        }
        /**************/

        X[0]=xtmp[0];
        X[1]=xtmp[1];
        Y[0]=ytmp[0];
        Y[1]=ytmp[1];
        if ( ppSegs->vz != (double *) NULL) {
          Z[0]=ztmp[0];
          Z[1]=ztmp[1];
          z=Z;
        }
        else
        {
          z=(double *)NULL;
        }
        x=X;y=Y;
      }
      break;
    case  SCI_RECTANGLE: 
    {
      double rectx[4],recty[4],rectz[4];
      p = 5;
      
      rectx[0]= rectx[3] =pRECTANGLE_FEATURE (pobj)->x;
      rectx[1]= rectx[2] =pRECTANGLE_FEATURE (pobj)->x+pRECTANGLE_FEATURE (pobj)->width;   
      recty[0]= recty[1] =pRECTANGLE_FEATURE (pobj)->y;   
      recty[2]= recty[3] =pRECTANGLE_FEATURE (pobj)->y-pRECTANGLE_FEATURE (pobj)->height;
      rectz[0]= rectz[1]=rectz[2]= rectz[3]=pRECTANGLE_FEATURE (pobj)->z;
	
      ReverseDataFor3D(psubwin, rectx, recty, rectz, 4);
      
      X[0]=X[1]=X[4]=rectx[0];
      Y[0]=Y[3]=Y[4]=recty[0];
      X[2]=X[3]=rectx[1];
      Y[1]=Y[2]=recty[2];
      Z[0]=Z[1]=Z[2]=Z[3]=Z[4]=rectz[0];
      x=X;y=Y;z=Z; 
    }  
    break;
    default:
      p = 0;
    }
    /* each element is represented by its barycenter */
    xmoy=0.0;ymoy=0.0;zmoy=0.0;
    if (z != (double *)NULL) {
      for ( j= 0 ; j < p ; j++) {
	xmoy += x[j];  ymoy += y[j];  zmoy += z[j];
      }
    }
    else {
      for ( j= 0 ; j < p ; j++) {
	xmoy += x[j];  ymoy += y[j];
      }
    }
    /* Compute the distance from the observer */
    dist[i]=  TRZ(xmoy/p,ymoy/p,zmoy/p);
    max_p=Max(max_p,p);

  } /* END of FOR here F.Leray 01.12.04 */

  /* sort the distance in decreasing order */
  C2F(dsort)(dist,&N,locindex); 

  FREE(verticesX) ;
  FREE(verticesY) ;
  FREE(verticesZ) ;

  /* ========================================================================
   * draw each element in the order given by locindex
   * ========================================================================*/
  C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  zmin = pSUBWIN_FEATURE (psubwin)->SRect[4];
  zmax = pSUBWIN_FEATURE (psubwin)->SRect[5];
  if ((polyx=(int *)MALLOC((max_p+1)*sizeof(int)))==(int *) NULL) {
    FREE(dist);FREE(locindex);
    Scistring("DrawMerge3d : MALLOC No more Place\n");

    return;
  }
  if ((polyy=(int *)MALLOC((max_p+1)*sizeof(int)))==(int *) NULL) {
    FREE(dist);FREE(locindex);
    Scistring("DrawMerge3d : MALLOC No more Place\n");
    return;
  }
  npoly=1; 
  for ( i = N ; i>0 ; i--)
  { /* loop on elements */
    int j,nok;
    j=locindex[i-1]-1;
    index=pMERGE_FEATURE (pmerge)->index_in_entity[j];
    pobj=(sciPointObj *) sciGetPointerFromHandle (pMERGE_FEATURE (pmerge)->from_entity[j]);
    
    if ( !sciGetRealVisibility(pobj) )
    {
      continue ;
    }
    /* build the element coordinates */
    switch (sciGetEntityType (pobj)) {  
    case SCI_SURFACE:
      if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D) { /* x,y,Z */
        int l,k,n1,n2;
          
        n1= pSURFACE_FEATURE (pobj)->dimzx;
        n2= pSURFACE_FEATURE (pobj)->dimzy;
        l=(int)(index/(n1-1));
        k=index-l*(n1-1);

        xtmp[0] = pSURFACE_FEATURE (pobj)->pvecx[k];
        xtmp[1] = pSURFACE_FEATURE (pobj)->pvecx[k+1];
	  
        /* I didn't use ReverseDataFor3D because dim n1 is not the same for x, y and z  */
        if(ppsubwin->axes.reverse[0] == TRUE){
          /* agir sur x */
          if(ppsubwin->logflags[0]=='l'){
            for(u=0;u<2;u++){
              xtmp[u] = log10(xtmp[u]);
              xtmp[u] = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp[u]);
              xtmp[u] = exp10(xtmp[u]);
            }
          }
          else {
            xtmp[0] = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp[0]);
            xtmp[1] = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp[1]);
          }
        }

        ytmp[0] = pSURFACE_FEATURE (pobj)->pvecy[l];
        ytmp[1] = pSURFACE_FEATURE (pobj)->pvecy[l+1];

        if(ppsubwin->axes.reverse[1] == TRUE){ 
          /* agir sur y */
          if(ppsubwin->logflags[1]=='l'){
            for(u=0;u<2;u++){
              ytmp[u] = log10(ytmp[u]);
              ytmp[u] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp[u]);
              ytmp[u] = exp10(ytmp[u]);
            }
          }
          else{
            ytmp[0] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp[0]);
            ytmp[1] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp[1]);
          }
        }

        ztmp[0] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1];
        ztmp[1] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1+n1];
        ztmp[2] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1+n1+1];
        ztmp[3] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1+1];


        if(ppsubwin->axes.reverse[2] == TRUE){ 
          /* agir sur z */
          if(ppsubwin->logflags[2]=='l'){
            for(u=0;u<4;u++){
              ztmp[u] = log10(ztmp[u]);
              ztmp[u] = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp[u]);
              ztmp[u] = exp10(ztmp[u]);
            }
          }
          else{
            for(u=0;u<4;u++)
              ztmp[u] = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp[u]);
          }
        }
	  
        n2= pSURFACE_FEATURE (pobj)->dimzy;
        X[0]=X[1]=xtmp[0];
        X[2]=X[3]=xtmp[1];
        Z[0]=ztmp[0];
        Z[1]=ztmp[1];
        Z[2]=ztmp[2];
        Z[3]=ztmp[3];
	  
        Y[0]=Y[3]=ytmp[0];
        Y[1]=Y[2]=ytmp[1];
	  
        Zoriginal = &(pSURFACE_FEATURE (pobj)->pvecz[k+l*n1]);
	  
        p=4;
        x=X;y=Y;z=Z;
	  
      }
      else
      {
        /* facets */
        p = pSURFACE_FEATURE (pobj)->dimzx; /* number of edges in the facets */
          
        verticesX = MALLOC( p * sizeof(double) ) ;
        verticesY = MALLOC( p * sizeof(double) ) ;
        verticesZ = MALLOC( p * sizeof(double) ) ;
          
        if ( verticesX == NULL || verticesY == NULL || verticesZ == NULL )
        {
          FREE(verticesX) ;
          FREE(verticesY) ;
          FREE(verticesZ) ;
          sciprint( "sciDrawObj: Unable to allocate temporary vector, memory full.\n" ) ;
          return ;
        }
          
        retrieveFacetVertices( pobj, index, verticesX, verticesY, verticesZ, &Zoriginal ) ;
          
        x = verticesX ; 
        y = verticesY ;
        z = verticesZ ;
      }
      break;
    case  SCI_POLYLINE:
    {
      sciPolyline * ppPolyLine = pPOLYLINE_FEATURE( pobj ) ;
      n1 = ppPolyLine->n1 ;
      p=0;
      if ( sciGetIsMark( pobj ) ) { p=1 ; } /* F.Leray 20.01.05 A REVOIR ICI*/
      if ( sciGetIsLine( pobj ) ) { p=2 ; }
          
      if ( ppPolyLine->plot != 2 && sciGetIsMark(pobj) == 1 )
      {
        xtmp[0] = ppPolyLine->pvx[index];
        xtmp[1] = xtmp[0] ;
            
        ytmp[0] = ppPolyLine->pvy[index];
        ytmp[1] = ytmp[0] ; /* used by trans3d + drawing : case 0,1 and 4 */
        if(ppPolyLine->pvz != NULL)
        {
          ztmp[0] = ppPolyLine->pvz[index];
          ztmp[1] = ztmp[0] ;
        }
      }
      else
      {
        xtmp[0] = ppPolyLine->pvx[index];
        xtmp[1] = ppPolyLine->pvx[index+1];
            
        ytmp[0] = ppPolyLine->pvy[index];
        ytmp[1] = ppPolyLine->pvy[index+1]; /* used by trans3d + drawing : case 0,1 and 4 */

        if(ppPolyLine->pvz != NULL)
        {
          ztmp[0] = ppPolyLine->pvz[index];
          ztmp[1] = ppPolyLine->pvz[(index+1)%n1];
        }
      }
          
      if(ppPolyLine->pvz != NULL)
        ReverseDataFor3D(psubwin,xtmp,ytmp,ztmp,2);
      else
        ReverseDataFor3D(psubwin,xtmp,ytmp,(double *) NULL,2);
          
      switch (ppPolyLine->plot) {
      case 0: case 1: case 4: /*linear interpolation */
        x=&(xtmp[0]);
        y=&(ytmp[0]);
        if (ppPolyLine->pvz != (double *) NULL) 
          z=&(ztmp[0]);
        else
          z=(double *)NULL;
        break;
      case 2: /* staircase */ /* just for completion  */
        X[0]=xtmp[0];
        X[1]=xtmp[1];
        Y[0]=ytmp[0];
        Y[1]=ytmp[0];
        if (ppPolyLine->pvz != (double *) NULL) {
          Z[0]=ztmp[0];
          Z[1]=ztmp[0];
          z=Z;
        }
        else 
          z=(double *)NULL;
        x=X;y=Y;
        break;
      case 3 : /* vertical bar */ /* just for completion  */
        X[0]=xtmp[0];
        X[1]=xtmp[0];
        Y[0]=0.0;
        if(ppsubwin->logflags[1]=='l') /* when logscale on Y, special treatment because we can not have Y == 0 */
          Y[0] = ppsubwin->FRect[1];
        Y[1]=ytmp[0];
        if (ppPolyLine->pvz != (double *) NULL) {
          Z[0]=ztmp[0];
          Z[1]=ztmp[0];
          z=Z;
        }
        else 
          z=(double *)NULL;
        x=X;y=Y;
        break;
      case 5: /* patch*/
        x=xtmp;
        y=ytmp;
        if (ppPolyLine->pvz != (double *) NULL)
          z=ztmp;
        else
          z= (double *) NULL;
        break;
      }
    }
    break;
    case  SCI_SEGS: 
    {
      sciSegs * ppSegs = pSEGS_FEATURE(pobj) ;
      p = 2;
      /***************/

      if (ppSegs->ptype == 0) /* ptype == 0 F.Leray : This is NOT A champ */
      {  
        if (ppSegs->iflag == 1) {
          pstyle=sciGetGoodIndex(pobj, ppSegs->pstyle[index]);
        }
        else{
          pstyle=sciGetGoodIndex(pobj, ppSegs->pstyle[0]);
        }
      }
      else
      {
        pstyle = 0 ;
      }

      iflag = ppSegs->iflag;

      /***************/
      if ( ppSegs->ptype == 0 )
      {
        xtmp[0] = ppSegs->vx[2*index];
        xtmp[1] = ppSegs->vx[2*index+1];

        ytmp[0] = ppSegs->vy[2*index];
        ytmp[1] = ppSegs->vy[2*index+1];

        if( ppSegs->vz != NULL )
        {
          ztmp[0] = ppSegs->vz[2*index];
          ztmp[1] = ppSegs->vz[2*index+1];
        }

      }
      else
      {
        /* vx is size n1, vy n2 and vfx and vfy n1 x n2 */
        int column = index / ppSegs->Nbr1 ;
        int row    = index - column * ppSegs->Nbr1 ;
        double maxGap = 0.707 * computeGridMinGap( ppSegs->vx, ppSegs->vy, ppSegs->Nbr1, ppSegs->Nbr2 ) ;

        xtmp[0] = ppSegs->vx[row] ;
        ytmp[0] = ppSegs->vy[column] ;

        if (ppSegs->typeofchamp == 0 )
        {
          double maxNorm = getLongestVector( ppSegs->vfx, ppSegs->vfy, ppSegs->Nbr1, ppSegs->Nbr2, 1.0, 1.0 ) ;
          xtmp[1] = ppSegs->vx[row]    + maxGap * ppSegs->vfx[index] / maxNorm ;
          ytmp[1] = ppSegs->vy[column] + maxGap * ppSegs->vfy[index] / maxNorm ;
        }
        else
        {
          /* colored */
          double norm = sqrt( ppSegs->vfx[index] * ppSegs->vfx[index] + ppSegs->vfy[index] * ppSegs->vfy[index] ) ;
          xtmp[1] = ppSegs->vx[row]    + maxGap * ppSegs->vfx[index] / norm ;
          ytmp[1] = ppSegs->vy[column] + maxGap * ppSegs->vfy[index] / norm ; ;
        }


        if( ppSegs->vz != NULL )
        {
          ztmp[0] = ppSegs->vz[row] ;
          ztmp[1] = ppSegs->vz[row] + ppSegs->vfz[index] ;
        }
      }


      if(ppSegs->vz != NULL)
        ReverseDataFor3D(psubwin,xtmp,ytmp,ztmp,2);
      else
        ReverseDataFor3D(psubwin,xtmp,ytmp,(double *) NULL,2);

      /**************/

      X[0]=xtmp[0];
      X[1]=xtmp[1];
      Y[0]=ytmp[0];
      Y[1]=ytmp[1];
      if (ppSegs->vz != (double *) NULL) {
        Z[0]=ztmp[0];
        Z[1]=ztmp[1];
        z=Z;
      }
      else
        z=(double *)NULL;
      x=X;y=Y;
    }
    break;
    case  SCI_RECTANGLE: 
    {
      double rectx[4],recty[4],rectz[4];
      p = 5;
	
      pstyle=0; /* arevoir */
      iflag=0; /* arevoir */
      
      rectx[0]= rectx[3] =pRECTANGLE_FEATURE (pobj)->x;
      rectx[1]= rectx[2] =pRECTANGLE_FEATURE (pobj)->x+pRECTANGLE_FEATURE (pobj)->width;   
      recty[0]= recty[1] =pRECTANGLE_FEATURE (pobj)->y;   
      recty[2]= recty[3] =pRECTANGLE_FEATURE (pobj)->y-pRECTANGLE_FEATURE (pobj)->height;
      rectz[0]= rectz[1]=rectz[2]= rectz[3]=pRECTANGLE_FEATURE (pobj)->z;
      
      ReverseDataFor3D(psubwin, rectx, recty, rectz, 4);
      
      X[0]=X[1]=X[4]=rectx[0];
      Y[0]=Y[3]=Y[4]=recty[0];
      X[2]=X[3]=rectx[1];
      Y[1]=Y[2]=recty[2];
      Z[0]=Z[1]=Z[2]=Z[3]=Z[4]=rectz[0];
      x=X;y=Y;z=Z; 
    }
    break;
    default:
      p = 0;
    }
      
    nok = 0;

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

      if(nok == 1) continue;

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
      C2F (dr) ("xset", "thickness",  context+1, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 9L);
      C2F (dr) ("xset", "line style", context+2, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L); 
      C2F (dr) ("xset", "mark", context+4, context+5, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 4L, 4L);

      if (sciGetEntityType (pobj)==SCI_SURFACE) {
        int fg1  = pSURFACE_FEATURE (pobj)->hiddencolor;
        int flag = pSURFACE_FEATURE (pobj)->flag[0];
        int facteur = 1;
	  
        polyx[p]=polyx[0];
        polyy[p]=polyy[0];
        p++ ;
        /*close the facet*/
        /* beware we got twice the first point !!! */

        /* facteur is used below */
        if(ppsubwin->axes.reverse[0] == TRUE) facteur = -facteur;
        if(ppsubwin->axes.reverse[1] == TRUE) facteur = -facteur;
        if(ppsubwin->axes.reverse[2] == TRUE) facteur = -facteur;
	  
        if ((((polyx[1]-polyx[0])*(polyy[2]-polyy[0])-(polyy[1]-polyy[0])*(polyx[2]-polyx[0]))*facteur <  0) &&
            (fg1 >= 0)) { /* hidden face */

          if ( pSURFACE_FEATURE (pobj)->flagcolor != 0)
            fill[0] = (flag < 0 ) ? -fg1 : fg1 ;
          else
            fill[0] = (flag != 0 ) ? fg1 : flag ;
          if(sciGetIsLine(pobj)){
            C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 6L);
            C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 10L);
            C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&p,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
          if(sciGetIsMark(pobj))
            DrawMarks3D(pobj,5*npoly,polyx,polyy,DPI);
        }
        else {
          switch ( pSURFACE_FEATURE (pobj)->flagcolor) {
          case 0:
            fill[0]= flag ;
            if(sciGetIsLine(pobj)){
              C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 6L);
              C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 10L);
              C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&p, PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
            if(sciGetIsMark(pobj))
              DrawMarks3D(pobj,5*npoly,polyx,polyy,DPI);
            break;
          case 1:
            zl = 0.0 ;
            /* compute facet barycenter */
            for ( k1= 0 ; k1 < (p-1) ; k1++) { zl+= z[k1] ; }
            zl /= p - 1.0 ;

            fill[0] = inint( (whiteid-1) * (zl-zmin)/(zmax-zmin) ) + 1 ;

            if ( flag  < 0 ) fill[0]=-fill[0];
            if(sciGetIsLine(pobj)){
              C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 6L);
              C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 10L);
              C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&p ,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
            if(sciGetIsMark(pobj))
              DrawMarks3D(pobj,5*npoly,polyx,polyy,DPI);
            break;
          case 2:
            fill[0]= (int) pSURFACE_FEATURE (pobj)->color[index];
            if ( flag < 0 ) fill[0]=-fill[0];
            if(sciGetIsLine(pobj)){
              C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 6L);
              C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 10L);
              C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&p ,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
            if(sciGetIsMark(pobj))
              DrawMarks3D(pobj,5*npoly,polyx,polyy,DPI);
            break;
          case 3:
            p--;
            if ( (p) != 3 && (p) !=4 ) {
              Scistring("Interpolated shading is only allowed for polygons with 3 or 4 vertices\n");
              return;
            }
            else  {
              /* shade needs (int*) color */
              integer *cvect = NULL,ik;
              sciSurface * ppsurface = pSURFACE_FEATURE (pobj);
		
              if((cvect=MALLOC(ppsurface->nc*sizeof(integer)))==NULL){
                sciprint("Allocation failed in merge for color matrix\n");
                return;
              }    
		
              for(ik=0;ik<ppsurface->nc;ik++) 
                cvect[ik] = (int) ppsurface->color[ik];

              C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 6L);
              C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 10L);
			
              scilab_shade(polyx,polyy,&(cvect[p*index]),p,ppsurface->flag[0]);
              FREE(cvect); cvect = NULL;
              if (sciGetIsMark (pobj))
                DrawMarks3D (pobj, p,polyx,polyy,DPI);
            }
            break;
          case 4: /* new case for "flat" mode matlab compatibility */
            fill[0]= (int) pSURFACE_FEATURE (pobj)->color[index];
            if ( flag < 0 ) fill[0]=-fill[0];
            if(sciGetIsLine(pobj)){
              C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 6L);
              C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 10L);
              C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&p ,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
            if(sciGetIsMark(pobj))
              DrawMarks3D(pobj,5*npoly,polyx,polyy,DPI);
            break;
          }
        }
      } /* end SCI_SURFACE*/
      else if(sciGetEntityType (pobj)==SCI_SEGS)
      {
        /* PSEGS here ! */
        sciSegs * ppSegs = pSEGS_FEATURE(pobj) ;
        if ( sciGetIsMark(pobj) )
        {
          integer v;
          double dv=0;
          int x[4], markidsizenew[2];
	    
          x[0] = sciGetMarkForeground(pobj);
	    
          markidsizenew[0] = sciGetMarkStyle(pobj);
          markidsizenew[1] = sciGetMarkSize (pobj);
	    
          C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
                    &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
                    &dv, &dv, &dv, &dv, 5L, 4096);
	    
          C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
                    PD0, PD0, 0L, 0L);
          DrawNewMarks(pobj,p,polyx,polyy,DPI);
        }
	  
        if(sciGetIsLine(pobj)){
          C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, 
                    PD0, PD0, PD0, 5L, 6L);
          C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, 
                    PD0, PD0, PD0, 5L, 10L);
          C2F (dr) ("xset", "thickness",  context+1, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 9L);
          C2F (dr) ("xset", "line style", context+2, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L); 
          if ( ppSegs->ptype == 0 )
          {
            C2F(dr)("xsegs","v",polyx,polyy,&p,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
          else
          {
            // Champ
            int arrowSize = computeRealArrowSize( pobj, p, polyx, polyy ) ;
            if( ppSegs->typeofchamp == 0 )
            {
              int sflag = 0 ;
              C2F(dr)("xarrow","v",polyx,polyy,&p,&arrowSize,&pstyle,&sflag,PD0,PD0,PD0,PD0,0L,0L);
            }
            else
            {
              int sflag = 1 ; /* colored */
              int arrowColor = computeArrowColor( ppSegs->vfx, ppSegs->vfy, ppSegs->Nbr1, ppSegs->Nbr2, index ) ;
              C2F(dr)("xarrow","v",polyx,polyy,&p,&arrowSize,&arrowColor,&sflag,PD0,PD0,PD0,PD0,0L,0L);
            }
          } 
        }
      }
      else if(sciGetEntityType (pobj)==SCI_RECTANGLE) { /* RECTANGLE case here ! */
        if(sciGetIsFilled(pobj) == TRUE)
        {
          integer v;
          double dv=0;
          int x[4],close=1;
          char str[2] = "xv";
          x[0] = sciGetBackground(pobj);
	      
          C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
                    &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
                    &dv, &dv, &dv, &dv, 5L, 4096);
	      
          C2F (dr) ("xarea", str, &p, polyx, polyy, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
        }
	  
        if (sciGetIsMark(pobj) == TRUE){
          integer v;
          double dv=0;
          int x[4], markidsizenew[2];
	    
          x[0] = sciGetMarkForeground(pobj);
	    
          markidsizenew[0] = sciGetMarkStyle(pobj);
          markidsizenew[1] = sciGetMarkSize (pobj);
	    
          C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
                    &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
                    &dv, &dv, &dv, &dv, 5L, 4096);
	    
          C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
                    PD0, PD0, 0L, 0L);
          DrawNewMarks(pobj,p,polyx,polyy,DPI);
        }
	  
        if(sciGetIsLine(pobj)){
          C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, 
                    PD0, PD0, PD0, 5L, 6L);
          C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, 
                    PD0, PD0, PD0, 5L, 10L);
          C2F (dr) ("xset", "thickness",  context+1, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 9L);
          C2F (dr) ("xset", "line style", context+2, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L); 
          C2F(dr)("xsegs","v",polyx,polyy,&p,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
        }
      }
      else { /* POLYLINE case starts here ! */
        /* special case 5 */
        if(sciGetIsFilled(pobj) == TRUE && pPOLYLINE_FEATURE (pobj)->plot != 5) /* No filling if mode plot == 5 is selected */
        {
          integer v;
          double dv=0;
          int x[4],close=1;
          char str[2] = "xv";
          x[0] = sciGetBackground(pobj);
	      
          C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
                    &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
                    &dv, &dv, &dv, &dv, 5L, 4096);
	      
          C2F (dr) ("xarea", str, &p, polyx, polyy, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
        }
	  
        if (sciGetIsMark(pobj) == TRUE){
          integer v;
          double dv=0;
          int x[4], markidsizenew[2];
	    
          x[0] = sciGetMarkForeground(pobj);
	    
          markidsizenew[0] = sciGetMarkStyle(pobj);
          markidsizenew[1] = sciGetMarkSize (pobj);
	    
          C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
                    &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
                    &dv, &dv, &dv, &dv, 5L, 4096);
	    
          C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
                    PD0, PD0, 0L, 0L);
          DrawNewMarks(pobj,p,polyx,polyy,DPI);
        }
	  
        if(sciGetIsLine(pobj)){
          int un = 1, deux = 2;
          context[3] = 0;
	    
          C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, 
                    PD0, PD0, PD0, 5L, 6L);
          C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, 
                    PD0, PD0, PD0, 5L, 10L);
          C2F (dr) ("xset", "thickness",  context+1, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 9L);
          C2F (dr) ("xset", "line style", context+2, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L); 
          C2F (dr) ("xlines", "xv", &deux, polyx, polyy, &un, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
        }
      }
    }

#ifdef WIN32
    if ( hdcflag == 1) ReleaseWinHdc ();
#endif	  
	
  }
  FREE(dist);FREE(locindex);FREE(polyx);FREE(polyy);
  FREE(verticesX) ;
  FREE(verticesY) ;
  FREE(verticesZ) ;
}


/*-------------------------------------------------------------------------------------------*/
/* sciRefreshObj                                                                             */
/* draw an object but before select the rigth figure for display                             */
/*-------------------------------------------------------------------------------------------*/
int sciRefreshObj( sciPointObj * pobj )
{
  
  int parentId = sciGetNum( sciGetParentFigure( pobj ) ) ;
  int currentId             ;
  int status                ;
  int verboseGet        = 0 ;
  int iDontKnowWhatItIs     ;
  /* get current Id in ScilabXgc */
  C2F (dr) ("xget", "window",&verboseGet,&currentId,&iDontKnowWhatItIs,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  /* set the parent figure of the object as the current figure */
  if ( parentId != currentId )
  {
    C2F (dr) ("xset", "window",&parentId,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  
  /* draw the object */
  status = sciDrawObjIfRequired( pobj ) ;

  /* set back the values */
  if ( parentId != currentId )
  {
    C2F (dr) ("xset", "window",&currentId,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }

  return status ;
  
  
}



/**sciDrawObjIfRequired
 * @memo Draws Object (only the basic  graphicobject under subwindows) in its SubWindow or figure
 * if and only if pFIGURE_FEATURE(pobj)->auto_redraw == TRUE !!
 * Only used inside High Level functions calls (sucha as plot2d, plot3d...)
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, -1 if not
 */
int
sciDrawObjIfRequired (sciPointObj * pobj)
{
  sciPointObj * pfigure = sciGetParentFigure(pobj);

  if(pFIGURE_FEATURE(pfigure)->auto_redraw == TRUE && pFIGURE_FEATURE(pfigure)->visible == TRUE)
    sciDrawObj(pobj);

  return 0;
}

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
  integer *xm = NULL;
  integer *ym = NULL;
  integer *zm = NULL;
  integer n2 = 1, xtmp[4], ytmp[4], *pstyle = NULL/*,rect1[4]*/;
  integer closeflag = 0 ;
  double anglestr,w2,h2 ;
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
  double xbox[8],ybox[8],zbox[8];
  double *xzz = NULL;
  double *yzz = NULL;
  double *zzz = NULL;
  static integer InsideU[4],InsideD[4];
  	
  integer xxx[6];
  int fontstyle_zero = 0; /*To fill Sci_Axis for Axes objects */
  integer isoflag =0;     /*  for 3d isoview mode*/

  sciSurface * ppsurface = NULL; /* debug */
  sciAxes *paxes = (sciAxes *) NULL; /* debug */
  /*   sciPointObj * pfigure = NULL;  sciPointObj * psubwin = NULL;/\* debug *\/  */
  sciSubWindow * ppsubwin = NULL; /* debug */
  int i,j;
  /* variable pour le set_scale update_frame_bounds*/
  double subwin[4], framevalues[4];

  int nb_curves = 0, *curves_size = NULL, jk; /* for SCI_POLYLINE */
  double **xvect = (double **) NULL;
  double **yvect = (double **) NULL;
  double **zvect = (double **) NULL;
  int result_trans3d = 1;
  BOOL drawline = TRUE;
  int nb_curves_bar = 0;
  
  double bar_width = 0.;
  double * x_shift = NULL;
  double * y_shift = NULL;
  double * z_shift = NULL;
  double tempvect = 0.;

  /* get the number of curves/polylines created */
  /* by ONE call to plot2d (property pPOLYLINE_FEATURE ->n2) */
  /* to help the drawing of the bar (case 6 ONLY) in case POLYLINE */
  
  char STRFLAG[4];
  int DPI[2];

/*   int nb_vertices=0; */

  subwin[0]    = 0;
  subwin[1]    = 0;
  subwin[2]   = 1;
  subwin[3]    = 1;
  framevalues[0] = 0;
  framevalues[1] = 0;
  framevalues[2] = 1;
  framevalues[3] = 1;
  
  /* driver test */
  
  if((GetDriverId() != 0) && (xinitxend_flag == 1)){
    /*     printf("I DO NOTHING !!\n"); */
    return -1;
  }

  currentsubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());

  /* get the DPI of the current driver to draw new marks correctly */
  GetDPIFromDriver(DPI);

  switch (sciGetEntityType (pobj))
    {
    case SCI_FIGURE:
    {
      int curWinNum = 0 ;
      
      if( !pFIGURE_FEATURE(pobj)->auto_redraw ) { break ; }
     
      x[1] = sciGetBackground (pobj);x[4] = 0;
      /** xclear will properly upgrade background if necessary **/
#ifdef WIN32
      flag_DO = MaybeSetWinhdc();
#endif

      /* select the right figure for drawing to avoid displaying in a wrong window */
      C2F(dr)("xget","window",&verbose,&curWinNum,&narg,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
      C2F(dr)("xset","window",&(pFIGURE_FEATURE(pobj)->number),PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
      
      C2F(dr)("xset","pixmap",&(pFIGURE_FEATURE (pobj)->pixmap),PI0,PI0,PI0,PI0,PI0,PD0,
	      PD0,PD0,PD0,0L,0L);
      if (pFIGURE_FEATURE (pobj)->pixmap == 0 || GetDriverId() != 0){
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

      /* STOP HERE if figure is invisible: */
      if ( !sciGetVisibility(pobj) ) { break; }

      psonstmp = sciGetLastSons (pobj);
      while (psonstmp != (sciSons *) NULL) {
	if (sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN) sciDrawObj (psonstmp->pointobj);
       
	psonstmp = psonstmp->pprev;
      }

      /* return to the current window */
      C2F(dr)("xset","window",&curWinNum,PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
      break;
    }
    case SCI_SUBWIN: 
      if ( !sciGetVisibility(pobj) ) { break ; }

      ppsubwin = pSUBWIN_FEATURE (pobj);

      sciSetSelectedSubWin(pobj); 
     
      set_scale ("tttftt", pSUBWIN_FEATURE (pobj)->WRect, pSUBWIN_FEATURE (pobj)->FRect,
		 NULL, pSUBWIN_FEATURE (pobj)->logflags, 
		 pSUBWIN_FEATURE (pobj)->ARect); 

      if (pSUBWIN_FEATURE (pobj)->is3d) 
	{  
	  /* to avoid re-drawing on screen during computation of the vertices (at least)... */
	  /* see zoom_box function in Plo2dEch.c*/
	  if(GlobalFlag_Zoom3dOn == 1)
	    {
	      int un=1;
	      C2F(dr)("xset","pixmap",&un,PI0,PI0,PI0,PI0,PI0,PD0,
		      PD0,PD0,PD0,0L,0L);
	      C2F (dr) ("xset","wwpc", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0,0L, 0L);
	    }
	  
	  /* 3D Coordinates */ /* verifier si c'est encore utile SS */
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
	 
	  axis_3ddraw(pobj,xbox,ybox,zbox,InsideU,InsideD); /* TEST on sciGetVisibility inside : REMOVED F.Leray 21.01.05 */
	  /* because axis_3ddraw displays 3d axes BUT ALSO compute + reset the 3d scale BEFORE !! */
	  
          /* merge object is drawn first  since it might conceal other objects */
          /* which are not merged. */
          if ( pSUBWIN_FEATURE(pobj)->facetmerge )
          {
            sciDrawObj( sciGetMerge( pobj ) ) ;
          }

          psonstmp = sciGetLastSons (pobj);
	  while (psonstmp != (sciSons *) NULL)
          {
            if ( !(pSUBWIN_FEATURE(pobj)->facetmerge && sciIsMergeable(psonstmp->pointobj))  )
            {
              sciDrawObj(psonstmp->pointobj) ;
            }
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
	 
          sciUpdateScaleAngles( ppsubwin->theta, ppsubwin->alpha ) ; /* for merge objects */
	  sci_update_frame_bounds_2d(pobj);
	 
	  
	  /* Clipping only exists in 3d */
	  /* the global var. Cscale has just been updated */
	  /* therefore I can re-compute the clipping now (if it is "clipgrf") */
	  
	  sciClip(pobj); /* to update the clip_box if needed */
	  sciUnClip(pobj);
	  
	  DrawAxesBackground();

	  /* grid is behind other objetcs */
          drawAxesGrid( pobj ) ;
	   
          /* there is a bug here */
          /* We should make a check for merge objects here */
          /* But merge object has been only created only for 3d */
          /* so sometimes it works, sometime not */
          psonstmp = sciGetLastSons (pobj);
	  while ( psonstmp != NULL )
          {
            if ( !(ppsubwin->facetmerge && sciGetEntityType(psonstmp->pointobj) == SCI_SEGS) )
            {
              sciDrawObj( psonstmp->pointobj ) ;
            }
            
            psonstmp = psonstmp->pprev;
          }

	  x[0] = sciGetForeground (pobj);
	  x[2] = sciGetLineWidth (pobj);
	  x[3] = sciGetLineStyle (pobj);
	  markidsizenew[0] = sciGetMarkStyle(pobj);
	  markidsizenew[1] = sciGetLineWidth (pobj);x[4] = 0;v = 0;dv = 0;
	 
	  /* F.Leray 07.12.04 */
	  /* TO CORRECT the bug 1115 : Big object (grayplots) could cover axes*/
	  C2F (dr) ("xset","dashes",x,x,x+4,x+4,x+4,&v,&dv,&dv,&dv,&dv,5L,4096);
	  C2F (dr) ("xset","foreground",x,x,x+4,x+4,x+4,&v,&dv,&dv,&dv,&dv,5L,4096);
	  C2F (dr) ("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F (dr) ("xset","mark",&markidsizenew[0],&markidsizenew[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	 
	  /* 	 if (sciGetVisibility(pobj)) */
	  /* 	   { */
	  rebuild_strflag(pobj,STRFLAG);
	  axis_draw2 (STRFLAG);
	  
	  labels2D_draw(pobj); /* F.Leray 08.08.05 : labels' drawing at the end */
	  
#ifdef WIN32
	  if ( flag_DO == 1) ReleaseWinHdc();
#endif
	  wininfo("");  
	}
      break;                      
      /******************/
	  
    case SCI_AGREG:
    {
      BOOL isMerging = pSUBWIN_FEATURE (sciGetParentSubwin(pobj) )->facetmerge ;
      if (!sciGetVisibility(pobj)) break; /* RE-PUT F.Leray 21.01.05 */
            /* scan the hierarchie and call sciDrawObj */
      psonstmp = sciGetLastSons(pobj) ;
      while (psonstmp != (sciSons *) NULL)
      {
        /* draw only objects which are not already included in a merge objects */
        if ( !isMerging || !sciIsMergeable(psonstmp->pointobj) )
        {
          sciDrawObj (psonstmp->pointobj);
        }
        psonstmp = psonstmp->pprev;
      }
      break;
      /************ 30/04/2001 **************************************************/
    } 
    case SCI_LEGEND: 
      if (!sciGetVisibility(pobj)) break;
      else
	{
      		sciLegend * ppLegend = pLEGEND_FEATURE (pobj) ;
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
      	for (i = 0; i < ppLegend->nblegends; i++)
      	{
          if (sciGetIsMark(pLEGEND_FEATURE (pobj)->pptabofpointobj[i]))
          {
            pLEGEND_FEATURE (pobj)->pstyle[i] = 
              -sciGetMarkStyle (pLEGEND_FEATURE (pobj)->pptabofpointobj[i]);
          }
          else
          {
            ppLegend->pstyle[i] =  
              sciGetForeground( ppLegend->pptabofpointobj[i] );
          }
      	}
      	/*sciSetCurrentObj(pobj); F.Leray 25.03.04*/
      	Legends( ppLegend->pstyle, &(ppLegend->nblegends), sciGetText(pobj));
       
          
      	/* restore the graphic context */

      	C2F (dr1) ("xset", "dashes", &xold[0], &vold, &vold, &vold, &vold, &v,
			 &dv, &dv, &dv, &dv, 5L, 6L);
      	C2F (dr1) ("xset", "foreground", &xold[1], &vold, &vold, &vold, &vold,
			 &v, &dv, &dv, &dv, &dv, 5L, 10L);
#ifdef WIN32 
      	if ( flag_DO == 1) ReleaseWinHdc ();
#endif
 	}
      break; 

      /******************************** 22/05/2002 ***************************/    
    case SCI_FEC:
	{
	  int curLineStyle = 0 ;
      int lineStyle    = 1 ;
      int verbose      = 0 ;
		
	  if ( !sciGetVisibility(pobj) ) { break ; }
      
      n1=1;
	  if ((xm = MALLOC ((pFEC_FEATURE (pobj)->Nnode)*sizeof (integer))) == NULL) { return -1 ; }
	  if ((ym = MALLOC ((pFEC_FEATURE (pobj)->Nnode)*sizeof (integer))) == NULL) { return -1 ; }

      if( !pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d )
	{
	  for ( i =0 ; i < pFEC_FEATURE (pobj)->Nnode ; i++) {
	    xm[i]= XScale(pFEC_FEATURE (pobj)->pvecx[i]); 
	    ym[i]= YScale(pFEC_FEATURE (pobj)->pvecy[i]);} 
	}
      else /* 3D version */
	{
	  double * xvect = NULL;
	  double * yvect = NULL;
	  int n = pFEC_FEATURE (pobj)->Nnode;
	  

	  if ((xvect = MALLOC (n*sizeof (double))) == NULL) return -1;
	  if ((yvect = MALLOC (n*sizeof (double))) == NULL){
	    FREE(xvect); xvect = (double *) NULL; return -1;
	  }
	  
	  for(i=0;i<n;i++){
	    xvect[i] = pFEC_FEATURE (pobj)->pvecx[i];
	    yvect[i] = pFEC_FEATURE (pobj)->pvecy[i];
	  }
	  
	  ReverseDataFor3DXonly(sciGetParentSubwin(pobj),xvect,n);
	  ReverseDataFor3DYonly(sciGetParentSubwin(pobj),yvect,n);
	  
	  trans3d(sciGetParentSubwin(pobj),n,xm,ym,xvect,yvect,NULL);

	  FREE(xvect); xvect = (double *) NULL;
	  FREE(yvect); yvect = (double *) NULL;
	}

#ifdef WIN32
      flag_DO=MaybeSetWinhdc();
#endif
	  /* need to put line style to plain otherwise bug 1872 occurs */
      C2F(dr)("xget","line style",&verbose,&curLineStyle,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset", "line style", &lineStyle, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);

      newfec(xm,ym,pFEC_FEATURE (pobj)->pnoeud,pFEC_FEATURE (pobj)->pfun,
	     &pFEC_FEATURE (pobj)->Nnode,&pFEC_FEATURE (pobj)->Ntr,
	     pFEC_FEATURE (pobj)->zminmax,pFEC_FEATURE (pobj)->colminmax,
	     pFEC_FEATURE (pobj)->colout, pFEC_FEATURE (pobj)->with_mesh);

	  C2F(dr)("xset", "line style", &curLineStyle, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);

#ifdef WIN32
      if ( flag_DO == 1) ReleaseWinHdc();
#endif
	  
      /* FREE(xm);FREE(ym); */ /* SS 02/04 */
      FREE(xm); xm = (integer *) NULL;
      FREE(ym); ym = (integer *) NULL; /* et F.Leray 18.02.04*/

      break;      
      /******************************** 22/05/2002 ***************************/
	}
    case SCI_SEGS:
      if (!sciGetVisibility(pobj)) { break ; }
      
      sciClip(pobj);

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

      if ( pSEGS_FEATURE (pobj)->ptype == 0 ) /* ptype == 0 F.Leray : This is NOT A champ */
      {  
        
        n=pSEGS_FEATURE (pobj)->Nbr1;
	  
        if ((xm = MALLOC (n*sizeof (integer))) == NULL)	{ return -1 ; }
        if ((ym = MALLOC (n*sizeof (integer))) == NULL)	{ return -1 ; } /* F.Leray 18.02.04 Correction suivante:*/
	  
        if ((pstyle = MALLOC ((int)(n/2)*sizeof (integer))) == NULL) { return -1 ; } /* SS 19.04*/
        if (pSEGS_FEATURE (pobj)->iflag == 1)
        {
          for ( i =0 ; i <(int) (n/2) ; i++)
          {
            pstyle[i]=sciGetGoodIndex(pobj, pSEGS_FEATURE (pobj)->pstyle[i]);
          }
        }
        else
        {
          pstyle[0] = sciGetGoodIndex(pobj, pSEGS_FEATURE (pobj)->pstyle[0]) ;
        }
        if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
        {
          double * xvect = NULL;
          double * yvect = NULL;
          double * zvect = NULL;

          if ((xvect = MALLOC (n*sizeof (double))) == NULL) return -1;
          if ((yvect = MALLOC (n*sizeof (double))) == NULL) return -1;

          for( i = 0 ; i < n ; i++ )
          {
            xvect[i] = pSEGS_FEATURE (pobj)->vx[i];
            yvect[i] = pSEGS_FEATURE (pobj)->vy[i];
          }

          if ( pSEGS_FEATURE (pobj)->vz != NULL )
          {
            if ((zvect = MALLOC (n*sizeof (double))) == NULL) { return -1 ; }
            for( i = 0 ; i < n ; i++ )
            {
              zvect[i] = pSEGS_FEATURE (pobj)->vz[i];
            }
          }
          
          ReverseDataFor3D(sciGetParentSubwin(pobj),xvect,yvect,zvect,n);
	      
          trans3d(sciGetParentSubwin(pobj),n,xm,ym,xvect,yvect,zvect);
	      
          FREE(xvect); xvect = NULL;
          FREE(yvect); yvect = NULL;
          if ( zvect != NULL )
          {
            FREE( zvect ) ;
            zvect = NULL;
          }
        }
        else
        {
          for ( i =0 ; i < n ; i++)
          {
            xm[i] = XScale(pSEGS_FEATURE (pobj)->vx[i]) ; 
            ym[i] = YScale(pSEGS_FEATURE (pobj)->vy[i]) ;
          }
        }
#ifdef WIN32 
        flag_DO = MaybeSetWinhdc();
#endif

        if( sciGetIsMark(pobj) )
        {
          drawPolyMarks( pobj, n, xm, ym, DPI ) ;
        }

        if( sciGetIsLine( pobj ) )
        {
          x[0] = sciGetForeground(pobj);
	      
          C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,
                    &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
                    PD0, PD0, PD0, 0L, 0L);    
          C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
                    PD0, PD0, PD0, 0L, 0L);

          if ( pSEGS_FEATURE (pobj)->arrowsize == 0 )
          {
            /* only lines */
            C2F(dr)("xsegs","v",xm,ym,&n,pstyle,&pSEGS_FEATURE (pobj)->iflag,
                    PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
          else
          {
            int arrowSize = computeRealArrowSize( pobj, n, xm, ym ) ;      		    
            C2F(dr)("xarrow","v",xm,ym,&n,&arrowSize,pstyle,&pSEGS_FEATURE(pobj)->iflag,PD0,PD0,PD0,PD0,0L,0L);
            /* with C2F(dr)("xarrow",... did not work: why? What does (dr1) routine make more than (dr) in New Graphics mode ?? */
            /* Answer : dr deals with pixels value (data: xm and ym are integers!!) whereas dr1 deals with double value coming from the user */
            /* This is true for old and new graphics mode. */
          }

        }

        
#ifdef WIN32 
        if ( flag_DO == 1) ReleaseWinHdc ();
#endif 
        FREE(xm);         xm = (integer *) NULL;
        FREE(ym);         ym = (integer *) NULL; 
        FREE(pstyle); pstyle = (integer *) NULL; /* SS 19.04*/
      }
      else    /*ptype == 1*/ /* ptype == 1 F.Leray : This IS A champ */
      {
#ifdef WIN32 
        flag_DO = MaybeSetWinhdc();
#endif
        C2F(dr)("xget","use color",&verbose, &uc, &narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
        if (uc)
          C2F(dr)("xget","color",&verbose,xz,&narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
        else
          C2F(dr)("xget","line style",&verbose,xz,&narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
#ifdef WIN32 
        if ( flag_DO == 1) ReleaseWinHdc ();
#endif 

        n=2*(pSEGS_FEATURE (pobj)->Nbr1)*((pSEGS_FEATURE (pobj)->Nbr2));
	 

        xm = MALLOC (n*sizeof (integer)) ;
        ym = MALLOC (n*sizeof (integer)) ;

        zm = NULL;/* SS 02/04 */
        if ( xm == NULL || ym == NULL) 
        {
          sciprint("Running out of memory \n");
          return -1;
        }      
        if ( pSEGS_FEATURE (pobj)->typeofchamp == 1 )
        { /* champ1 has been called */
         
          zm = MALLOC (((int) (n/2))*sizeof (integer)) ;
	    
          if (  zm == NULL ) 
          {
            sciprint("Running out of memory \n");
            return -1;
          }      
        }
        
        sciChamp2DRealToPixel(xm,ym,zm,&na,&arssize,
                              pSEGS_FEATURE (pobj)->vx,pSEGS_FEATURE (pobj)->vy,pSEGS_FEATURE (pobj)->vfx,
                              pSEGS_FEATURE (pobj)->vfy,&(pSEGS_FEATURE (pobj)->Nbr1),
                              &(pSEGS_FEATURE (pobj)->Nbr2),&(pSEGS_FEATURE (pobj)->parfact),&(pSEGS_FEATURE (pobj)->typeofchamp),TRUE);
#ifdef WIN32 
        flag_DO = MaybeSetWinhdc();
#endif

        /** size of arrow **/
        /* a bit tricky */
	  
        /* test if we are in 3d HERE */
        if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
        {
          double * xvect = NULL;
          double * yvect = NULL;
          double * zvect = NULL;
	      
          if ((xvect = MALLOC (n*sizeof (double))) == NULL) return -1;
          if ((yvect = MALLOC (n*sizeof (double))) == NULL) return -1;
          if ((zvect = MALLOC (n*sizeof (double))) == NULL) return -1;
	      
          for(i=0;i<n;i++){
            xvect[i] = XPi2R(xm[i]);
            yvect[i] = YPi2R(ym[i]);
          }
	      
          
          /* F.Leray 06.12.04 */
          /* A REVOIR : ne marche pas en 3D */
          ReverseDataFor3D(sciGetParentSubwin(pobj),xvect,yvect,NULL,n);
	      
          trans3d(sciGetParentSubwin(pobj),n,xm,ym,xvect,yvect,NULL);
	      
          FREE(xvect); xvect = NULL;
          FREE(yvect); yvect = NULL;
          FREE(zvect); zvect = NULL;
        }

        if ( sciGetIsMark( pobj ) )
        {
          drawPolyMarks( pobj, n, xm, ym, DPI ) ;
        }

        if( sciGetIsLine( pobj ) )
        {
          int arrowSize =  computeRealArrowSize( pobj, na, xm, ym ) ;
          x[0] = sciGetForeground(pobj);

          C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,
                    &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
                    PD0, PD0, PD0, 0L, 0L);    
          C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
                    PD0, PD0, PD0, 0L, 0L);
          if( pSEGS_FEATURE(pobj)->typeofchamp == 0 )
          {
            C2F(dr)("xarrow","v",xm,ym,&na,&arrowSize,xz,(sflag=0,&sflag),&dv,&dv,&dv,&dv,0L,0L);
          }
          else
          {
            C2F(dr)("xarrow","v",xm,ym,&na,&arrowSize,zm,(sflag=1,&sflag),&dv,&dv,&dv,&dv,0L,0L);
          }
        }

        
#ifdef WIN32 
        if ( flag_DO == 1) ReleaseWinHdc ();
#endif 
        FREE(xm) ; xm = (integer *) NULL;
        FREE(ym) ; ym = (integer *) NULL;/* SS 02/04 */ /* et F.Leray 18.02.04*/
        if ( pSEGS_FEATURE (pobj)->typeofchamp == 1) 
        {
          FREE(zm); zm = (integer *) NULL;/* F.Leray 1802.04 modif ici*/
        }
      }  

      sciUnClip(pobj);
      break;
    case SCI_GRAYPLOT:
      if (!sciGetVisibility(pobj)) break;
      n1 = pGRAYPLOT_FEATURE (pobj)->nx;
      n2 = pGRAYPLOT_FEATURE (pobj)->ny;    
     
      switch (pGRAYPLOT_FEATURE (pobj)->type )
	{
	
	  
	case 0:  /* Grayplot case */
	  if(pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d == FALSE){

	    /* F.Leray 19.05.05 : Now I use only xliness (not xfrect) */
	    /* to better manage axes reversing */
	    if ((xm = MALLOC (n1*n2*sizeof (integer))) == NULL)	return -1;
	    if ((ym = MALLOC (n2*n1*sizeof (integer))) == NULL){
	      FREE(xm); xm = (integer *) NULL; return -1; 
	    }

	    for ( i =0 ; i < n1 ; i++)  /* on x*/
	      for ( j =0 ; j < n2 ; j++)  /* on y */
		{
		  xm[i+j*n1]= XScale(pGRAYPLOT_FEATURE (pobj)->pvecx[i]);
		  ym[j+i*n2]= YScale(pGRAYPLOT_FEATURE (pobj)->pvecy[j]);
		}
	    
#ifdef WIN32
	    flag_DO = MaybeSetWinhdc();
#endif
	    frame_clip_on();
  
	    if (strncmp(pGRAYPLOT_FEATURE (pobj)->datamapping,"scaled", 6) == 0)
	      GraySquareScaled(xm,ym,pGRAYPLOT_FEATURE (pobj)->pvecz,n1,n2); /* SS 03/01/03 */
	    else
	      GraySquareDirect(xm,ym,pGRAYPLOT_FEATURE (pobj)->pvecz,n1,n2);

	    frame_clip_off();
#ifdef WIN32
	    if ( flag_DO == 1) ReleaseWinHdc();
#endif
	    
	    /*  FREE(xm);FREE(ym); */ /* SS 03/01/03 */
	    FREE(xm); xm = (integer *) NULL; /* F.Leray c'est mieux.*/
	    FREE(ym); ym = (integer *) NULL;

	  }
	  else{
	    /*3D version */
	    double * xvect = NULL;
	    double * yvect = NULL;
	
	    if ((xvect = MALLOC (n1*sizeof (double))) == NULL) return -1;
	    if ((yvect = MALLOC (n2*sizeof (double))) == NULL){
	      FREE(xvect); xvect = (double *) NULL; return -1;
	    }
	    
	    for(i=0;i<n1;i++) xvect[i] = pGRAYPLOT_FEATURE (pobj)->pvecx[i];
	    for(i=0;i<n2;i++) yvect[i] = pGRAYPLOT_FEATURE (pobj)->pvecy[i];


	    if ((xm = MALLOC (n1*n2*sizeof (integer))) == NULL)	return -1;
	    if ((ym = MALLOC (n2*n1*sizeof (integer))) == NULL){
	      FREE(xm); xm = (integer *) NULL; return -1; 
	    }
	    
	    ReverseDataFor3DXonly(sciGetParentSubwin(pobj),xvect,n1);
	    ReverseDataFor3DYonly(sciGetParentSubwin(pobj),yvect,n2);

	    for ( i =0 ; i < n1 ; i++)  /* on x*/
	      for ( j =0 ; j < n2 ; j++)  /* on y */
		trans3d(sciGetParentSubwin(pobj),1,&xm[i+j*n1],&ym[j+i*n2],
			&xvect[i],&yvect[j],NULL);
	    
#ifdef WIN32
	    flag_DO = MaybeSetWinhdc();
#endif
	    frame_clip_on(); 
	    
	    /* draw the filled projected rectangle */
	    /*   for(i=0;i<(n1-1)*(n2-1);i++) */
	    /* 	      { */
	    for (i = 0 ; i < (n1)-1 ; i++)
	      for (j = 0 ; j < (n2)-1 ; j++)
		{
		  integer vertexx[5], vertexy[5];
		  int cinq = 5, un = 1;
		  double zmoy,zmax,zmin,zmaxmin;
		  integer verbose=0,whiteid,narg,fill[1],cpat,xz[2];
		  double *z = pGRAYPLOT_FEATURE (pobj)->pvecz;
		  zmin=Mini(z,(n1)*(n2));
		  zmax=Maxi(z,(n1)*(n2));
		  zmaxmin=zmax-zmin;
		  
		  if (zmaxmin <= SMDOUBLE) zmaxmin=SMDOUBLE;
		  C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  C2F(dr)("xget","pattern",&verbose,&cpat,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  C2F(dr)("xget","wdim",&verbose,xz,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  

		  if(strncmp(pGRAYPLOT_FEATURE (pobj)->datamapping,"scaled", 6) == 0)
		    {
		      /* color for current rectangle */
		      zmoy=1/4.0*(z[i+n1*j]+z[i+n1*(j+1)]+z[i+1+n1*j]+z[i+1+n1*(j+1)]);
		      
		      fill[0]=1 + inint((whiteid-1)*(zmoy-zmin)/(zmaxmin));  
		      
		      fill[0] = - fill[0]; /* not to have contour with foreground color around the rectangle */
		    }
		  else /* "direct" mode is used */
		    {
		      fill[0] = - (int) z[j+n1*i];
		    }
		  
		  vertexx[0] = xm[i+n1*j];
		  vertexx[1] = xm[i+n1*(j+1)];
		  vertexx[2] = xm[i+1+n1*(j+1)];
		  vertexx[3] = xm[i+1+n1*j];
		  vertexx[4] = xm[i+n1*j];
		  
		  vertexy[0] = ym[j+n2*i];
		  vertexy[1] = ym[j+1+n2*i];
		  vertexy[2] = ym[j+1+n2*(i+1)];
		  vertexy[3] = ym[j+n2*(i+1)];
		  vertexy[4] = ym[j+n2*i];
		  
		  C2F(dr)("xliness","str",vertexx,vertexy,fill,&un,&cinq,
			  PI0,PD0,PD0,PD0,PD0,0L,0L);
		}
	    
	    frame_clip_off();  
#ifdef WIN32
	    if ( flag_DO == 1) ReleaseWinHdc();
#endif
	    
	    FREE(xm); xm = (integer *) NULL;
	    FREE(ym); ym = (integer *) NULL;
	    FREE(xvect); xvect = (double *) NULL;
	    FREE(yvect); yvect = (double *) NULL;
	  }
	  break;
	case 1: /* Matplot case */
	  {
	    /* In this case (and inside Matplot1 too but Matplot and Matplot1 are almost the same), */
	    /* dim x = n2 and dim y = n1 (cf. scimatplot in matdes.c) */ /* F.Leray 20.05.05 */
	    sciPointObj * psubwin = sciGetParentSubwin(pobj);
	    sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
	    
	    if(pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d == FALSE){
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
	      if(ppsubwin->axes.reverse[0] == TRUE || ppsubwin->axes.reverse[1] == TRUE)
		GraySquare1_NGreverse(xm,ym,pGRAYPLOT_FEATURE (pobj)->pvecz,n1,n2,psubwin);  
	      else
		GraySquare1(xm,ym,pGRAYPLOT_FEATURE (pobj)->pvecz,n1,n2);  
	      frame_clip_off();  
	      /*	      C2F(dr)("xrect","v",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],
			      &Cscale.WIRect1[3],PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);*/
#ifdef WIN32
	      if ( flag_DO == 1) ReleaseWinHdc();
#endif
	     
	      /*  FREE(xm);FREE(ym); */ /* SS 03/01/03 */
	      FREE(xm); xm = (integer *) NULL; /* F.Leray c'est mieux.*/
	      FREE(ym); ym = (integer *) NULL;
	    }
	    else{
	      /* 3D version */
	      double * xvect = NULL;
	      double * yvect = NULL;
	   
	      /* Warning here (Matplot case) : n1 becomes n2 and vice versa */
	      if ((xvect = MALLOC (n2*sizeof (double))) == NULL) return -1;
	      if ((yvect = MALLOC (n1*sizeof (double))) == NULL){
		FREE(xvect); xvect = (double *) NULL; return -1;
	      }
	    
	      for(i=0;i<n2;i++) xvect[i] = i+0.5;
	      for(i=0;i<n1;i++) yvect[i] = n1-1-i+0.5;
	    

	      if ((xm = MALLOC (n2*n1*sizeof (integer))) == NULL)	return -1;
	      if ((ym = MALLOC (n1*n2*sizeof (integer))) == NULL){
		FREE(xm); xm = (integer *) NULL; return -1; 
	      }
	    
	      ReverseDataFor3DXonly(sciGetParentSubwin(pobj),xvect,n2);
	      ReverseDataFor3DYonly(sciGetParentSubwin(pobj),yvect,n1);

	    
	      for ( i =0 ; i < n2 ; i++)  /* on x*/
		for ( j =0 ; j < n1 ; j++)  /* on y */
		  trans3d(sciGetParentSubwin(pobj),1,&xm[i+j*n2],&ym[j+i*n1],
			  &xvect[i],&yvect[j],NULL);
	    
#ifdef WIN32
	      flag_DO = MaybeSetWinhdc();
#endif
	      frame_clip_on(); 
	    
	      /* draw the filled projected rectangle */
	      /*   for(i=0;i<(n1-1)*(n2-1);i++) */
	      /* 	      { */
	      for (i = 0 ; i < (n2)-1 ; i++)
		for (j = 0 ; j < (n1)-1 ; j++)
		  {
		    integer vertexx[5], vertexy[5];
		    int cinq = 5, un = 1;
		    integer fill;

		    fill = (int ) - pGRAYPLOT_FEATURE (pobj)->pvecz[(n1-1)*i+j];

		    vertexx[0] = xm[i+n2*j];
		    vertexx[1] = xm[i+n2*(j+1)];
		    vertexx[2] = xm[i+1+n2*(j+1)];
		    vertexx[3] = xm[i+1+n2*j];
		    vertexx[4] = xm[i+n2*j];
		  
		    vertexy[0] = ym[j+n1*i];
		    vertexy[1] = ym[j+1+n1*i];
		    vertexy[2] = ym[j+1+n1*(i+1)];
		    vertexy[3] = ym[j+n1*(i+1)];
		    vertexy[4] = ym[j+n1*i];
		    
		    C2F(dr)("xliness","str",vertexx,vertexy,&fill,&un,&cinq,
			    PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }
	    
	      frame_clip_off();  
#ifdef WIN32
	      if ( flag_DO == 1) ReleaseWinHdc();
#endif
	    
	      FREE(xm); xm = (integer *) NULL;
	      FREE(ym); ym = (integer *) NULL;
	      FREE(xvect); xvect = (double *) NULL;
	      FREE(yvect); yvect = (double *) NULL;
	    }
	  }
	  break;
	case 2: /* Matplot1 case */
	  /* In this case (and inside Matplot too but Matplot and Matplot1 are almost the same), */
	  /* dim x = n2 and dim y = n1 (cf. scimatplot in matdes.c) */ /* F.Leray 20.05.05 */
	  
	  if(pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d == FALSE){
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
	  }
	  else{
	    /* 3D version */
	    double * xvect = NULL;
	    double * yvect = NULL;
	   
	    /* Warning here (Matplot case) : n1 becomes n2 and vice versa */
	    if ((xvect = MALLOC (n2*sizeof (double))) == NULL) return -1;
	    if ((yvect = MALLOC (n1*sizeof (double))) == NULL){
	      FREE(xvect); xvect = (double *) NULL; return -1;
	    }
	   
	    xx[0]=pGRAYPLOT_FEATURE (pobj)->pvecx[0];
	    xx[1]=pGRAYPLOT_FEATURE (pobj)->pvecx[2];
	    yy[0]=pGRAYPLOT_FEATURE (pobj)->pvecx[1];
	    yy[1]=pGRAYPLOT_FEATURE (pobj)->pvecx[3];
	   
	   
	    /** Boundaries of the frame **/
	    for ( i =0 ; i < n2 ; i++)
	      xvect[i]= (( xx[1]*i + xx[0]*((n2-1)-i) )/(n2-1));
	   
	    for ( j =0 ; j < n1 ; j++)
	      yvect[j]= (( yy[0]*j + yy[1]*((n1-1)-j) )/ (n1-1)); 
	   
	   
	    if ((xm = MALLOC (n2*n1*sizeof (integer))) == NULL)	return -1;
	    if ((ym = MALLOC (n1*n2*sizeof (integer))) == NULL){
	      FREE(xm); xm = (integer *) NULL; return -1; 
	    }
	   
	    ReverseDataFor3DXonly(sciGetParentSubwin(pobj),xvect,n2);
	    ReverseDataFor3DYonly(sciGetParentSubwin(pobj),yvect,n1);
	   
	   
	    for ( i =0 ; i < n2 ; i++)  /* on x*/
	      for ( j =0 ; j < n1 ; j++)  /* on y */
		trans3d(sciGetParentSubwin(pobj),1,&xm[i+j*n2],&ym[j+i*n1],
			&xvect[i],&yvect[j],NULL);
	   
#ifdef WIN32
	    flag_DO = MaybeSetWinhdc();
#endif
	    frame_clip_on(); 
	   
	    /* draw the filled projected rectangle */
	    for (i = 0 ; i < (n2)-1 ; i++)
	      for (j = 0 ; j < (n1)-1 ; j++)
		{
		  integer vertexx[5], vertexy[5];
		  int cinq = 5, un = 1;
		  integer fill;

		  fill = (int) - pGRAYPLOT_FEATURE (pobj)->pvecz[(n1-1)*i+j];
		 
		  vertexx[0] = xm[i+n2*j];
		  vertexx[1] = xm[i+n2*(j+1)];
		  vertexx[2] = xm[i+1+n2*(j+1)];
		  vertexx[3] = xm[i+1+n2*j];
		  vertexx[4] = xm[i+n2*j];
		 
		  vertexy[0] = ym[j+n1*i];
		  vertexy[1] = ym[j+1+n1*i];
		  vertexy[2] = ym[j+1+n1*(i+1)];
		  vertexy[3] = ym[j+n1*(i+1)];
		  vertexy[4] = ym[j+n1*i];
		  
		  C2F(dr)("xliness","str",vertexx,vertexy,&fill,&un,&cinq,
			  PI0,PD0,PD0,PD0,PD0,0L,0L);
		}
	   
	    frame_clip_off();  
#ifdef WIN32
	    if ( flag_DO == 1) ReleaseWinHdc();
#endif
	    FREE(xm); xm = (integer *) NULL;
	    FREE(ym); ym = (integer *) NULL;
	    FREE(xvect); xvect = (double *) NULL;
	    FREE(yvect); yvect = (double *) NULL;
	  }
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
  
      x[4] = 0;
      v = 0;
      dv = 0;
      logflags[0]='g';
      logflags[1]= pSUBWIN_FEATURE(sciGetParentSubwin(pobj))->logflags[0]; /* F.Leray 26.10.04 Pb when logscale on and data is <= 0 for clipping */
      logflags[2]= pSUBWIN_FEATURE(sciGetParentSubwin(pobj))->logflags[1];
      logflags[3]='\0';
     

      x_shift = pPOLYLINE_FEATURE (pobj)->x_shift;
      y_shift = pPOLYLINE_FEATURE (pobj)->y_shift;
      z_shift = pPOLYLINE_FEATURE (pobj)->z_shift;
    

      /* //////////////////////////////////////////////////////////////// */
      BuildXYZvectForClipping_IfNanOrLogON(pobj,sciGetParentSubwin(pobj),&nb_curves, &xvect, &yvect, &zvect, &curves_size);
      /* //////////////////////////////////////////////////////////////// */
      
#ifdef WIN32 
      flag_DO = MaybeSetWinhdc();
#endif
      
      C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
		&dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
		&dv, &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);
      C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);
   
#ifdef WIN32 
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif  
      n1 = pPOLYLINE_FEATURE (pobj)->n1;
      /* n2 = 1; */ /* the number of curves is always 1 since we draw each one line at a given time */
      nb_curves_bar = pPOLYLINE_FEATURE (pobj)->n2;
      closeflag = pPOLYLINE_FEATURE (pobj)->closed; /* 0 or 1 */
      
      /***/
      sciClip(pobj);


#ifdef WIN32 
      flag_DO = MaybeSetWinhdc ();
#endif


      for(jk=0;jk<nb_curves;jk++)
	{
	  n1 = curves_size[jk];

	  if(n1==0) continue;
	  
	  if ((xm = MALLOC ((2*n1)*sizeof (integer))) == NULL)	return -1;
	  if ((ym = MALLOC ((2*n1)*sizeof (integer))) == NULL)	return -1;
	  if ((xzz = MALLOC ((2*n1)*sizeof (double))) == NULL)	return -1;
	  if ((yzz = MALLOC ((2*n1)*sizeof (double))) == NULL)	return -1;
	  if ((zzz = MALLOC ((2*n1)*sizeof (double))) == NULL)	return -1;
	  
	  /**DJ.Abdemouche 2003**/
	  switch (pPOLYLINE_FEATURE (pobj)->plot)
	    {
	    case 1:  /* case plot2d, xpoly */
	      if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d){
		drawline = TRUE;
		/* axes reverse is tested and xvect, yvect are changed if needed here */
		ReverseDataFor3D(sciGetParentSubwin(pobj),xvect[jk],yvect[jk],zvect[jk],n1);
		
		result_trans3d = trans3d(sciGetParentSubwin(pobj),n1,xm,ym,xvect[jk],yvect[jk],zvect[jk]);
	      }
	      else{
		drawline = TRUE;
		/* In 2d, the axes reverse is done inside XScale, YScale... routines. */
		C2F (echelle2d) (xvect[jk],yvect[jk], xm, ym, &n1, &un, "f2i",3L); 
	      }
	      break; 
	    case 2:
	      drawline = TRUE;
	      if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
		{
		  if(zvect[jk] == NULL){
		    FREE(zzz); zzz = (double *) NULL;
		  }
		  
		  ReverseDataFor3D(sciGetParentSubwin(pobj),xvect[jk],yvect[jk],zvect[jk],n1);
		  
		  Plo2dTo3d(2,&un,&n1,xvect[jk],yvect[jk],zvect[jk],xzz,yzz,zzz);
		  result_trans3d = trans3d(sciGetParentSubwin(pobj),n1*2,xm,ym,xzz,yzz,zzz);
		}
	      else
		{
		  Plo2d2RealToPixel(&un,&n1,xvect[jk],yvect[jk],xm,ym,logflags);
		}
	      n1=n1*2;
	      break;
	    case 3:  
	      drawline = FALSE;
	      if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
		{
		  if(zvect[jk] == NULL){
		    FREE(zzz); zzz = (double *) NULL;
		  }
		    
		  ReverseDataFor3D(sciGetParentSubwin(pobj),xvect[jk],yvect[jk],zvect[jk],n1);
		  
		  Plo2dTo3d(3,&un,&n1,xvect[jk],yvect[jk],zvect[jk],xzz,yzz,zzz);
		  result_trans3d = trans3d(sciGetParentSubwin(pobj),n1*2,xm,ym,xzz,yzz,zzz);
		}
	      else
		{
		  Plo2d3RealToPixel(&un,&n1,xvect[jk],yvect[jk],xm,ym,logflags);
		}
	      if(result_trans3d == 1)
		nn1= n1*2;
	      /* add mark support even for bar plot lines */
	      if (sciGetIsMark(pobj) == TRUE){
		int x[4], markidsizenew[2];
		x[0] = sciGetMarkForeground(pobj);
		
		markidsizenew[0] = sciGetMarkStyle(pobj);
		markidsizenew[1] = sciGetMarkSize (pobj);
		
		C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
			  &dv, &dv, &dv, 5L, 4096);
		C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
			  &dv, &dv, &dv, &dv, 5L, 4096);
		
		C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
			  PD0, PD0, 0L, 0L);   
		
		DrawNewMarks(pobj,nn1,&xm[0],&ym[0],DPI);
	      }
	      
	      if (sciGetIsLine(pobj) == TRUE){
		lstyle=sciGetForeground(pobj);
		iflag=0; 
		C2F(dr)("xsegs","v",&xm[0],&ym[0],&nn1,&lstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      }
	      /**DJ.Abdemouche 2003**/
	      n1=un;
	      break;
	    case 4:
	      drawline = FALSE;
	      if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
		{
		  if(zvect[jk] == NULL){
		    FREE(zzz); zzz = (double *) NULL;
		  }
		  
		  ReverseDataFor3D(sciGetParentSubwin(pobj),xvect[jk],yvect[jk],zvect[jk],n1);
		  
		  Plo2dTo3d(4,&un,&n1,xvect[jk],yvect[jk],zvect[jk],xzz,yzz,zzz);
		  
		  result_trans3d = trans3d(sciGetParentSubwin(pobj),n1*2,xm,ym,xzz,yzz,zzz);
		}
	      else
		{
		  Plo2d4RealToPixel(&un,&n1,xvect[jk],yvect[jk],xm,ym,logflags); 
		}
	      nn2=2*(n1)-1;
	      arsize1= Cscale.WIRect1[2]/70.0;arsize2= Cscale.WIRect1[3]/70.0;
	      arsize=  (arsize1 < arsize2) ? inint(10*arsize1) : inint(10*arsize2) ;
	      
	      if(result_trans3d == 1){
		integer lstyle=sciGetForeground(pobj) ,iflag=0;
		
		/* add mark support even for arrow line style */
		if (sciGetIsMark(pobj) == TRUE){
		  int x[4], markidsizenew[2];
		  x[0] = sciGetMarkForeground(pobj);
		  
		  markidsizenew[0] = sciGetMarkStyle(pobj);
		  markidsizenew[1] = sciGetMarkSize (pobj);
		  
		  C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
			    &dv, &dv, &dv, 5L, 4096);
		  C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
			    &dv, &dv, &dv, &dv, 5L, 4096);
		  
		  C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
			    PD0, PD0, 0L, 0L);   
		  
		  DrawNewMarks(pobj,nn2,&xm[0],&ym[0],DPI);
		}
		
		if (sciGetIsLine(pobj) == TRUE){
		  x[0] = sciGetForeground(pobj);
		  x[2] = sciGetLineWidth (pobj);
		  x[3] = sciGetLineStyle (pobj);
		  
		  C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,
			    &dv, &dv, &dv, 5L, 4096);
		  C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
				&dv, &dv, &dv, &dv, 5L, 4096);
		  C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
			    PD0, PD0, PD0, 0L, 0L);
		  C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
			    PD0, PD0, PD0, 0L, 0L);
		  
		  arsize = (integer) pPOLYLINE_FEATURE(pobj)->arsize_factor * arsize;

		  C2F(dr)("xarrow","v",&xm[0],&ym[0],&nn2,&arsize,&lstyle,&iflag,PD0,PD0,PD0,PD0,0L,0L);
		}
	      }
	      break;
	    case 5: /* case xfpoly */
	      drawline = TRUE;
	      if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d){
		ReverseDataFor3D(sciGetParentSubwin(pobj),xvect[jk],yvect[jk],zvect[jk],n1);
		result_trans3d = trans3d(sciGetParentSubwin(pobj),n1,xm,ym,xvect[jk],yvect[jk],zvect[jk]);
	      }
	      else
		C2F (echelle2d) (xvect[jk],yvect[jk], xm, ym, &n1, &un, "f2i",3L);
	      
	      sciClip(pobj);
	      
	      if(result_trans3d == 1)
		C2F (dr) ("xarea", str, &n1, xm, ym, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
	      
	      break;
	    case 6: /* 'Matlab' bar */
	      drawline = TRUE;
	      x[2] = sciGetLineWidth (pobj);
	      x[3] = sciGetLineStyle (pobj);
	      /* get the number of polylines sisters with bar property "on" */
	      
	      bar_width =  pPOLYLINE_FEATURE(pobj)->bar_width;
	      
	      if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
		{
		  double myX[4], myY[4], myZ[4];
		  int pix_X[4], pix_Y[4];
		  int quatre = 4;
		  
		  for(i=0;i<n1;i++)
		    {
		      myX[0] = myX[1] = xvect[jk][i] - bar_width/2;
		      myX[2] = myX[3] = xvect[jk][i] + bar_width/2;
		      if(y_shift == (double *) NULL)
			myY[0] = myY[3] =  0.;
		      else
			myY[0] = myY[3] =  y_shift[i];
		      myY[1] = myY[2] =  yvect[jk][i];
		      
		      if(zvect[jk] == (double *) NULL)
			myZ[0] = myZ[1] = myZ[2] = myZ[3] = 0.; /* cas log a revoir */
		      else		      
			myZ[0] = myZ[1] = myZ[2] = myZ[3] = zvect[jk][i];
		      
		      ReverseDataFor3D(sciGetParentSubwin(pobj),myX,myY,myZ,quatre);
		      result_trans3d = trans3d(sciGetParentSubwin(pobj),quatre,pix_X,pix_Y,myX,myY,myZ);
		      
		      x[0] = sciGetBackground(pobj);
		      
		      C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
				&dv, &dv, &dv, 5L, 4096);
		      C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
				&dv, &dv, &dv, &dv, 5L, 4096);
		      
		      C2F (dr) ("xarea", str, &quatre, pix_X, pix_Y, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
		      
		      x[0] = sciGetForeground(pobj);
		      
		      C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
				&dv, &dv, &dv, 5L, 4096);
		      C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
				&dv, &dv, &dv, &dv, 5L, 4096);
		      
		      C2F (dr) ("xlines", "xv", &quatre, pix_X, pix_Y, &un, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
		    }

		  ReverseDataFor3D(sciGetParentSubwin(pobj),xvect[jk],yvect[jk],zvect[jk],n1);
		  result_trans3d = trans3d(sciGetParentSubwin(pobj),n1,xm,ym,xvect[jk],yvect[jk],zvect[jk]);
		}
	      else
		{
		  double myX[4], myY[4];
		  int pix_X[4], pix_Y[4],i;
		  int quatre = 4;
		  
		  for(i=0;i<n1;i++)
		    {
		      myX[0] = myX[1] = xvect[jk][i] - bar_width/2;
		      myX[2] = myX[3] = xvect[jk][i] + bar_width/2;
		      
		      if(y_shift == (double *) NULL)
			myY[0] = myY[3] =  0.;
		      else
			myY[0] = myY[3] =  y_shift[i];
		      myY[1] = myY[2] =  yvect[jk][i];
		      
		      C2F (echelle2d) (myX,myY, pix_X, pix_Y, &quatre, &un, "f2i",3L);
		      
		      x[0] = sciGetBackground(pobj);
		      
		      C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
				&dv, &dv, &dv, 5L, 4096);
		      C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
				&dv, &dv, &dv, &dv, 5L, 4096);
		      
		      C2F (dr) ("xarea", str, &quatre, pix_X, pix_Y, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
		      
		      x[0] = sciGetForeground(pobj);
		      
		      C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
				&dv, &dv, &dv, 5L, 4096);
		      C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
				&dv, &dv, &dv, &dv, 5L, 4096);
		      
		      C2F (dr) ("xlines", "xv", &quatre, pix_X, pix_Y, &un, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
		    }
		  C2F (echelle2d) (xvect[jk],yvect[jk], xm, ym, &n1, &un, "f2i",3L); 
		}
	      break;
	    case 7: /* 'Matlab' barh */
	      drawline = TRUE;
	      x[2] = sciGetLineWidth (pobj);
	      x[3] = sciGetLineStyle (pobj);
	      /* get the number of polylines sisters with bar property "on" */
	      
	      bar_width =  pPOLYLINE_FEATURE(pobj)->bar_width;
	      
	      for(i=0;i<n1;i++)
		{
		  tempvect=xvect[jk][i];
		  xvect[jk][i]=yvect[jk][i];
		  yvect[jk][i]=tempvect;
		}

	      if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
		{
		  double myX[4], myY[4], myZ[4];
		  int pix_X[4], pix_Y[4];
		  int quatre = 4;
		  
		  for(i=0;i<n1;i++)
		    {
		      myY[0] = myY[1] = yvect[jk][i] + bar_width/2;
		      myY[2] = myY[3] = yvect[jk][i] - bar_width/2;
		      
		      if(y_shift == (double *) NULL)
			myX[0] = myX[3] =  0.;
		      else 
			myX[0] = myX[3] =  y_shift[i];
		      myX[1] = myX[2] =  xvect[jk][i];
		      
		      if(zvect[jk] == (double *) NULL)
			myZ[0] = myZ[1] = myZ[2] = myZ[3] = 0.; /* cas log a revoir */
		      else		      
			myZ[0] = myZ[1] = myZ[2] = myZ[3] = zvect[jk][i];
		      
		      ReverseDataFor3D(sciGetParentSubwin(pobj),myX,myY,myZ,quatre);
		      result_trans3d = trans3d(sciGetParentSubwin(pobj),quatre,pix_X,pix_Y,myX,myY,myZ);
		      
		      x[0] = sciGetBackground(pobj);
		      
		      C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
				&dv, &dv, &dv, 5L, 4096);
		      C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
				&dv, &dv, &dv, &dv, 5L, 4096);
		      
		      C2F (dr) ("xarea", str, &quatre, pix_X, pix_Y, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
		      
		      x[0] = sciGetForeground(pobj);
		      
		      C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
				&dv, &dv, &dv, 5L, 4096);
		      C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
				&dv, &dv, &dv, &dv, 5L, 4096);
		      
		      C2F (dr) ("xlines", "xv", &quatre, pix_X, pix_Y, &un, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
		    }

		  ReverseDataFor3D(sciGetParentSubwin(pobj),xvect[jk],yvect[jk],zvect[jk],n1);
		  result_trans3d = trans3d(sciGetParentSubwin(pobj),n1,xm,ym,xvect[jk],yvect[jk],zvect[jk]);
		}
	      else
		{
		  double myX[4], myY[4];
		  int pix_X[4], pix_Y[4],i;
		  int quatre = 4;
		  
		  for(i=0;i<n1;i++)
		    {
		      myY[0] = myY[1] = yvect[jk][i] + bar_width/2;
		      myY[2] = myY[3] = yvect[jk][i] - bar_width/2;
		      
		      if(y_shift == (double *) NULL)
			myX[0] = myX[3] =  0.;
		      else 
			myX[0] = myX[3] =  y_shift[i];
		      myX[1] = myX[2] =  xvect[jk][i];
		      
		      C2F (echelle2d) (myX,myY, pix_X, pix_Y, &quatre, &un, "f2i",3L);
		      
		      x[0] = sciGetBackground(pobj);
		      
		      C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
				&dv, &dv, &dv, 5L, 4096);
		      C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
				&dv, &dv, &dv, &dv, 5L, 4096);
		      
		      C2F (dr) ("xarea", str, &quatre, pix_X, pix_Y, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
		      
		      x[0] = sciGetForeground(pobj);
		      
		      C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
				&dv, &dv, &dv, 5L, 4096);
		      C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
				&dv, &dv, &dv, &dv, 5L, 4096);
		      
		      C2F (dr) ("xlines", "xv", &quatre, pix_X, pix_Y, &un, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
		    }
		  C2F (echelle2d) (xvect[jk],yvect[jk], xm, ym, &n1, &un, "f2i",3L); 
		}
	      break;
	    default:
	      sciprint ("This Polyline cannot be drawn !\n");
/* #ifdef WIN32  */
/* 	      if ( flag_DO == 1) ReleaseWinHdc (); */
/* #endif   */
	      break;     
	    }
	 
	  if(result_trans3d == 1 && drawline == TRUE)
	    {
  	      if(sciGetIsFilled(pobj) == TRUE && pPOLYLINE_FEATURE (pobj)->plot != 5) /* No filling if mode plot == 5 is selected */
		{
		  if(pPOLYLINE_FEATURE (pobj)->isinterpshaded == FALSE){
		    /* flat mode */
		    x[0] = sciGetBackground(pobj);
		    
		    C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
			      &dv, &dv, &dv, 5L, 4096);
		    C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
			      &dv, &dv, &dv, &dv, 5L, 4096);
		    
		    C2F (dr) ("xarea", str, &n1, xm, ym, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
		  }
		  else{
		    /* interp. shading */
		    int *vect = pPOLYLINE_FEATURE (pobj)->scvector;
		    int nn = pPOLYLINE_FEATURE (pobj)->n1;

		    scilab_shade (xm, ym, vect, nn, 0);
		  }
		}
	      
	      if (sciGetIsMark(pobj) == TRUE){
		int x[4], markidsizenew[2];
		x[0] = sciGetMarkForeground(pobj);
	       
		markidsizenew[0] = sciGetMarkStyle(pobj);
		markidsizenew[1] = sciGetMarkSize (pobj);
		
		C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
			  &dv, &dv, &dv, 5L, 4096);
		C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
			  &dv, &dv, &dv, &dv, 5L, 4096);
	       
		C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
			  PD0, PD0, 0L, 0L);   
		
		DrawNewMarks(pobj,n1,xm,ym,DPI);
	      }
	      
	      if (sciGetIsLine(pobj) == TRUE){
		
		x[0] = sciGetForeground(pobj);
		
		C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
			  &dv, &dv, &dv, 5L, 4096);
		C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
			  &dv, &dv, &dv, &dv, 5L, 4096);
		C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
			  PD0, PD0, PD0, 0L, 0L);
		C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
			  PD0, PD0, PD0, 0L, 0L);

		C2F (dr) ("xlines", "xv", &n1, xm, ym, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
	      }
	    }
	  
#ifdef WIN32 
	  if ( flag_DO == 1) ReleaseWinHdc ();
#endif  
	 
	  FREE(xzz); xzz = (double *) NULL;
	  FREE(yzz); yzz = (double *) NULL;
	  FREE(zzz); zzz = (double *) NULL;/* SS 02/04 */
	  
	  FREE (xm); xm = (integer *) NULL;
	  FREE (ym); ym = (integer *) NULL;
	  
	}
      

      for(i=0;i<nb_curves;i++){
	int nn;
	nn = curves_size[i];
	
	if(nn==0) continue;
	
	FREE(xvect[i]); xvect[i] = (double *) NULL;
	FREE(yvect[i]); yvect[i] = (double *) NULL;
	FREE(zvect[i]); zvect[i] = (double *) NULL;
      }
      FREE(xvect); xvect = (double **) NULL;
      FREE(yvect); yvect = (double **) NULL;
      /*   if(zvect != (double **) NULL) */
      FREE(zvect); zvect = (double **) NULL;
      
      FREE(curves_size); curves_size = NULL;

#ifdef WIN32 
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif  
      
      sciUnClip(pobj);
      
      
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

      x[2] = sciGetLineWidth (pobj);
      x[3] = sciGetLineStyle (pobj);
      x[4] = 0;
      v = 0;
      dv = 0;
#ifdef WIN32 
      flag_DO = MaybeSetWinhdc ();
#endif
    
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
	  
	  double xvect;
	  double yvect;
	  double zvect;
	  
	  xvect = pARC_FEATURE(pobj)->x;
	  yvect = pARC_FEATURE(pobj)->y;
	  zvect = pARC_FEATURE(pobj)->z;
	  
	  ReverseDataFor3D(sciGetParentSubwin(pobj),&xvect,&yvect,&zvect,un);
	  
	  trans3d(sciGetParentSubwin(pobj),un,&x1,&yy1,&xvect,&yvect,&zvect);
	}
      else
	{
	  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (sciGetParentSubwin(pobj));
	  double tmpx = pARC_FEATURE (pobj)->x;
	  double tmpy = pARC_FEATURE (pobj)->y;
	  
	  double tmpwidth = pARC_FEATURE (pobj)->width;
	  double tmpheight= pARC_FEATURE (pobj)->height;
	  
	  if(ppsubwin->axes.reverse[0] == TRUE){
	    tmpx= tmpx + tmpwidth;
	  }
	  
	  if(ppsubwin->axes.reverse[1] == TRUE){
	    tmpy = tmpy - tmpheight;
	  }
	  
	  x1  = XDouble2Pixel (tmpx);
	  yy1 = YDouble2Pixel (tmpy);
	}

      w2 = pARC_FEATURE (pobj)->width;
      h2 = pARC_FEATURE (pobj)->height; 
      /* Nouvelles fonctions de changement d'echelle pour les longueurs --> voir PloEch.h */ 
      
      w1 = WScale(w2);
      h1 = HScale(h2);
      
      angle1 = (integer) (pARC_FEATURE (pobj)->alphabegin);
      angle2 = (integer) (pARC_FEATURE (pobj)->alphaend); 

      if((pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->axes.reverse[0] == TRUE)
	 && (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->axes.reverse[1] == FALSE)){
	angle1 = 180*64 - (angle2+angle1);
      }
      else if((pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->axes.reverse[0] == TRUE)
	      && (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->axes.reverse[1] == TRUE)){
	angle1 = 180*64 + angle1;
      }
      else if((pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->axes.reverse[0] == FALSE)
	      && (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->axes.reverse[1] == TRUE)){
	angle1 = 360*64 - (angle2+angle1);
      }
     
#ifdef WIN32 
      flag_DO = MaybeSetWinhdc ();
#endif
      sciClip(pobj);
      
      if(sciGetIsFilled(pobj) == TRUE){
	x[0] = sciGetBackground(pobj);
	C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, 
		  &dv, &dv, &dv, &dv, 5L, 6L);
	C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4,&v, 
		  &dv, &dv, &dv, &dv, 5L, 10L );
	C2F (dr) ("xfarc", str, &x1, &yy1, &w1, &h1, &angle1, &angle2, PD0, PD0, PD0,PD0, 5L, 0L);
      }
      
      if(sciGetIsLine(pobj) == TRUE){
	x[0] = sciGetForeground(pobj);
	C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, 
		  &dv, &dv, &dv, &dv, 5L, 6L);
	C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4,&v, 
		  &dv, &dv, &dv, &dv, 5L, 10L );
	C2F (dr) ("xarc", str, &x1, &yy1, &w1, &h1, &angle1, &angle2, PD0, PD0, PD0,PD0, 5L, 0L);
      }
      
/*       if (pARC_FEATURE (pobj)->fill  <= 0) */
/* 	C2F (dr) ("xarc", str, &x1, &yy1, &w1, &h1, &angle1, &angle2, PD0, PD0, PD0,PD0, 5L, 0L); */
/*       else */
/* 	C2F (dr) ("xfarc", str, &x1, &yy1, &w1, &h1, &angle1, &angle2, PD0, PD0, PD0,PD0, 5L, 0L); */
      sciUnClip(pobj);
#ifdef WIN32 
      if ( flag_DO == 1)  ReleaseWinHdc ();
#endif
      break;
    case SCI_RECTANGLE:
    {
      sciRectangle * ppRect = pRECTANGLE_FEATURE (pobj) ;
      /* store the size of the rectangle */
      double ulPoint[3]  = { ppRect->x, ppRect->y, ppRect->z } ;
      double rectSize[2] = { ppRect->width, ppRect->height }   ;
      int rectEdgesX[4] ;
      int rectEdgesY[4] ;
      if (!sciGetVisibility(pobj)) break;
      n = 1;
      x[2] = sciGetLineWidth (pobj);
      x[3] = sciGetLineStyle (pobj);
      x[4] = 0;

      v = 0;
      dv = 0; 
#ifdef WIN32
      flag_DO = MaybeSetWinhdc ();
#endif

      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);    
      C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);

#ifdef WIN32 
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif 

      /* get the 4 vertices of the rectangle in pixels */
      rectangleDouble2Pixel( sciGetParentSubwin(pobj),
                             ulPoint                 ,
                             rectSize                ,
                             rectEdgesX              ,
                             rectEdgesY                ) ;

      /**DJ.Abdemouche 2003**/
      if ( !(pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d) )
      {
        int rectPixPosX   ;
        int rectPixPosY   ;
        int rectPixWidth  ;
        int rectPixHeight ;
        
        /* retrieve x,y,w,h */
        rectPixPosX   = rectEdgesX[0] ;
        rectPixPosY   = rectEdgesY[0] ;
        rectPixWidth  = rectEdgesX[2] - rectEdgesX[0] ;
        rectPixHeight = rectEdgesY[2] - rectEdgesY[0] ;

        if ( ppRect->strwidth == 0 )
        {
          ppRect->strwidth  = rectPixWidth  ;
          ppRect->strheight = rectPixHeight ;
          
        }
        wstr=ppRect->strwidth;
        hstr=ppRect->strheight;
	  
	  
#ifdef WIN32 
        flag_DO = MaybeSetWinhdc ();
#endif
        sciClip(pobj);

        if( sciGetIsFilled(pobj) )
        {
          x[0] = sciGetBackground(pobj);
          C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, 
                    &dv, &dv, &dv, &dv, 5L, 6L);
          C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4,&v, 
                    &dv, &dv, &dv, &dv, 5L, 10L );
          C2F(dr)("xfrect",str,&rectPixPosX,&rectPixPosY,&rectPixWidth,&rectPixHeight,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
        }
	  
        if( sciGetIsMark(pobj) )
        {
          x[0] = sciGetMarkForeground(pobj);
	       
          markidsizenew[0] =  sciGetMarkStyle(pobj);
          markidsizenew[1] =  sciGetMarkSize(pobj);
	      
          C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
                    &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
                    &dv, &dv, &dv, &dv, 5L, 4096);
	     
          C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
                    PD0, PD0, 0L, 0L);
	     
          n = 4;
          xtmp[0] = rectPixPosX ;
          xtmp[1] = rectPixPosX + rectPixWidth ;
          xtmp[2] = rectPixPosX + rectPixWidth ;
          xtmp[3] = rectPixPosX ;
          ytmp[0] = rectPixPosY ;
          ytmp[1] = rectPixPosY ;
          ytmp[2] = rectPixPosY + rectPixHeight ;
          ytmp[3] = rectPixPosY + rectPixHeight ;
	     
          DrawNewMarks(pobj,n,xtmp,ytmp,DPI);
        }

        if (sciGetIsLine(pobj))
        {
          x[0] = sciGetForeground(pobj);

          C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,
                    &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,
                    &dv, &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
                    PD0, PD0, PD0, 0L, 0L);    
          C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
                    PD0, PD0, PD0, 0L, 0L);
	      

          if (ppRect->str == 1)
          {
            rectPixPosY -= hstr;
            C2F(dr)("xrect",str,&rectPixPosX,&rectPixPosY,&wstr,&hstr,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
          else
          {
            C2F(dr)("xrect",str,&rectPixPosX,&rectPixPosY,&rectPixWidth,&rectPixHeight,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
        }
	  
        sciUnClip(pobj);
#ifdef WIN32
        if ( flag_DO == 1)  ReleaseWinHdc ();
#endif
      }
      else /* Rect. in 3D */
      { 
        int close=1;
        n=4;
                
#ifdef WIN32
        flag_DO = MaybeSetWinhdc ();
#endif
        sciClip(pobj);

	  
        if( sciGetIsFilled(pobj) )
        {
          x[0] = sciGetBackground(pobj);
          C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, 
                    &dv, &dv, &dv, &dv, 5L, 6L);
          C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4,&v, 
                    &dv, &dv, &dv, &dv, 5L, 10L );
          C2F (dr) ("xarea", str, &n, rectEdgesX, rectEdgesY, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
        }
	  
        if (sciGetIsMark(pobj)) 
        {
          x[0] = sciGetMarkForeground(pobj);
		 
          markidsizenew[0] =  sciGetMarkStyle(pobj);
          markidsizenew[1] =  sciGetMarkSize(pobj);

          C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
                    &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
                    &dv, &dv, &dv, &dv, 5L, 4096);
	     
          C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
                    PD0, PD0, 0L, 0L);
	     
          n=4;
	     
          DrawNewMarks(pobj,n,rectEdgesX,rectEdgesY,DPI);
        }

        if (sciGetIsLine(pobj)) 
        {
          x[0] = sciGetForeground(pobj);
          C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,
                    &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,
                    &dv, &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
                    PD0, PD0, PD0, 0L, 0L);    
          C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
                    PD0, PD0, PD0, 0L, 0L);
	      
          C2F (dr) ("xlines", "xv", &n, rectEdgesX, rectEdgesY, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
        }

        sciUnClip(pobj);
#ifdef WIN32
        if ( flag_DO == 1)  ReleaseWinHdc ();
#endif
      }
    }
    break;
    case SCI_TEXT:
      if (!sciGetVisibility(pobj))
      {
        break;
      }
      else
      {
        sciText * ppText =  pTEXT_FEATURE( pobj ) ;
        
        /*drawText( pobj ) ;*/
        n = 1;
 
        v = 0;
        dv = 0;

        flagx = 0;
#ifdef WIN32 
        flag_DO = MaybeSetWinhdc ();
#endif
        if ( ppText->isclip )
        {
          sciClip(pobj);
        }

        if (pTEXT_FEATURE (pobj)->fill==-1) {
          if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
          {
          
            double xvect;
            double yvect;
            double zvect;
          
            xvect = ppText->x;
            yvect = ppText->y;
            zvect = ppText->z;
          
            ReverseDataFor3D(sciGetParentSubwin(pobj),&xvect,&yvect,&zvect,n);
          
            trans3d(sciGetParentSubwin(pobj),n,&x1,&yy1,&xvect,&yvect,&zvect);
          }
          else 
          {
            x1  = XDouble2Pixel (ppText->x);
            yy1 = YDouble2Pixel (ppText->y);
          }
          anglestr = (sciGetFontOrientation (pobj)/10); 	
          /* *10 parce que l'angle est conserve en 1/10eme de degre*/
          
          /* wether or not we draw and/or fill the box */
          /* no need to compute anything if both line_mode */
          /* and fill mode are false */
          if(    sciGetIsBoxed (pobj)
              && ( sciGetIsFilled( pobj ) || sciGetIsLine( pobj ) ) )
          {   
            int font_[2], cur_font_[2];
            int rect1[4], verb=0;
            int xm[4], ym[4],n=4;
            double cosangle = cos((360-anglestr)*M_PI/180);
            double sinangle = sin((360-anglestr)*M_PI/180);
            int close=1;

            
            C2F(dr1)("xget","font",&verb,font_,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);
            
            cur_font_[0] = font_[0];
            cur_font_[1] = font_[1];
            
            font_[0] = sciGetFontStyle (pobj);
            font_[1] = sciGetFontDeciWidth (pobj)/100;
            
            C2F(dr1)("xset","font",&font_[0],&font_[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

            C2F(dr)("xstringl",ppText->ptextstring,
                    &x1,&yy1,rect1,&v,&v,&v,&dv,&dv,&dv,&dv,9L,pTEXT_FEATURE (pobj)->textlen);
	    
            
            xm[0] = x1;
            xm[1] = round(x1 + cosangle*rect1[2]);
            xm[2] = round(x1 + cosangle*rect1[2] + sinangle*(-rect1[3]));
            xm[3] = round(x1 + sinangle*(-rect1[3]));
            
            ym[0] = yy1;
            ym[1] = round(yy1 - sinangle*rect1[2]);
            ym[2] = round(yy1 - sinangle*rect1[2] + cosangle*(-rect1[3]));
            ym[3] = round(yy1 + cosangle*(-rect1[3]));
            
            
            /* draw the background */
            if ( sciGetIsFilled( pobj ) )
            {
              x[0] = sciGetBackground(pobj);
              C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
              C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
              
              C2F (dr) ("xarea", str, &n, xm, ym, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
            }

            /* draw the line around the box */
            if ( sciGetIsLine( pobj ) )
            {
              /* draw a rectangle around the text */
              x[0] = sciGetForeground(pobj);
              
              C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
              C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
              
              C2F (dr) ("xlines", "xv", &n, xm, ym, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
            }
	    
            C2F(dr1)("xset","font",&cur_font_[0],&cur_font_[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
          
#ifdef WIN32 
          flag_DO = MaybeSetWinhdc ();
#endif
          /* load the object foreground and dashes color */
          x[0] = sciGetFontForeground (pobj);/*la dash est de la meme couleur que le foreground*/
          x[2] = sciGetFontDeciWidth (pobj)/100;
          x[3] = 0;
          x[4] = sciGetFontStyle(pobj);
          
          C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
          C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
          C2F(dr)("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);
          
#ifdef WIN32 
          if ( flag_DO == 1) ReleaseWinHdc ();
#endif
          C2F(dr)("xstring",sciGetText (pobj),&x1,&yy1,PI0,&flagx,PI0,PI0,&anglestr, PD0,PD0,PD0,0L,0L);
        }
        else { /* SS for xstringb should be improved*/
          integer w1, h1;
          w1  = XDouble2Pixel (pTEXT_FEATURE (pobj)->wh[0]);
          h1 = YDouble2Pixel (pTEXT_FEATURE (pobj)->wh[1]);
          x[0] = sciGetFontForeground (pobj);/*la dash est de la meme couleur que le foreground*/
          x[2] = sciGetFontDeciWidth (pobj)/100;
          x[3] = 0;
          x[4] = sciGetFontStyle(pobj);
          
          C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
          C2F (dr) ("xset", "foreground", x, x, x+3, x+3, x+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
          C2F(dr)("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);

          C2F(dr1)("xstringb",sciGetText (pobj),&(ppText->fill),&v,&v,&v,&v,&v,
                   &(ppText->x),&(ppText->y),
                   &(ppText->wh[0]),&(ppText->wh[1]),9L,0L);
        } 
        if ( ppText->isclip )
        {
          sciUnClip(pobj);
        }
        
#ifdef WIN32 
        if ( flag_DO == 1) ReleaseWinHdc ();
#endif
      }
      break;
      
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
      sciClip(pobj);
     
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
      sciUnClip(pobj);   
			  
      break;
    case SCI_MERGE:
      if (!(pSUBWIN_FEATURE (sciGetParentSubwin(pobj) )->facetmerge)) break; 
     
      /*     if (!sciGetVisibility(pobj)) break; */
     
      DrawMerge3d(sciGetParentSubwin(pobj), pobj, DPI);  /* TEST on sciGetVisibility inside */
      break;
    case SCI_SURFACE:
     
      ppsurface = pSURFACE_FEATURE (pobj);

      if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj) )->facetmerge) break;  
      if (!sciGetVisibility(pobj)) break;
      itmp[0] = 0;		/* verbose*/
      itmp[1] = 0;		/* thickness value*/
      itmp[2] = 1;		/* narg*/
      C2F (dr) ("xget", "thickness", &itmp[0], &itmp[1], &itmp[2], PI0, PI0,PI0, PD0, PD0, PD0, PD0, 4L, 9L);
      C2F (dr) ("xget", "mark", &itmp[0], markidsizeold, &itmp[3], PI0, PI0, PI0,PD0, PD0, PD0, PD0, 4L, 4L);

      /* load the object foreground and dashes color */
/*       x[0] = sciGetForeground (pobj);	 */
      x[2] = sciGetLineWidth (pobj);
      x[3] = sciGetLineStyle (pobj);
      markidsizenew[0] = sciGetMarkStyle(pobj);
      markidsizenew[1] = sciGetLineWidth (pobj);
      x[4] = 0;v = 0;dv = 0; 
#ifdef WIN32
      flag_DO = MaybeSetWinhdc();
#endif
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
		      pSURFACE_FEATURE (pobj)->color,
		      &pSURFACE_FEATURE (pobj)->dimzx,&pSURFACE_FEATURE (pobj)->dimzy, DPI);

	  break;
	case SCI_PLOT3D:
	  C2F(plot3dn)(pobj,pSURFACE_FEATURE (pobj)->pvecx,pSURFACE_FEATURE (pobj)->pvecy,
		       pSURFACE_FEATURE (pobj)->pvecz,
		       &pSURFACE_FEATURE (pobj)->dimzx,&pSURFACE_FEATURE (pobj)->dimzy, DPI);
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


extern int DrawNewMarks(sciPointObj * pobj, int n1, int *xm, int *ym, int *DPI)
{
  int style = sciGetMarkStyle(pobj);
  double size = (double) sciGetMarkSize(pobj);
  int foreground = sciGetMarkForeground(pobj);
  int background = sciGetMarkBackground(pobj);
  int i;

  int x[4],v,closeflag=0;
  double dv;
  int flagx = 0;
  int old_foreground;

  int thick;
  int linestyle[6];
  int tabulated_marksize[] = {8,10,12,14,18,24};
  /*   int tabulated_marksize[] = {3,5,7,9,11,13}; */
  
  integer verbose = 0,old_thick, old_linestyle[6],narg = 0;
  int ixres = DPI[0]; /* only the x DPI is used here : */
    
  int pixel_offset = CheckPixelStatus();
  double size_plus_one = 0.;
  double size_minus_one = 0.;


  linestyle[0] = 1;
  
  if(sciGetMarkSizeUnit(pobj) == 2){ /* tabulated */ /* size is given relative to the array tabulated_marksize */
    if( (int) size <= 5){
      if(style == 0 && size == 0) 
	size = 1; /* Singularity : To allow the drawing of a single pixel in old mode (tabulated) */
      else
	size = tabulated_marksize[(int) size];
    }   
    else{
      size = tabulated_marksize[5];
    }
  }

  size_plus_one = size + 1.;
  size_minus_one = size - 1.;

  /* We get the DPIs value : only the x DPI is used here : */
  /* we assum we have a square pixel (i.e. xDPI == yDPI or almost) */
  size = size * (ixres/72.);
  size_minus_one = size_minus_one * (ixres/72.);
  size_plus_one  = size_plus_one  * (ixres/72.);

  C2F (dr) ("xget", "foreground", &flagx, &old_foreground, &v, &v, &v,
	    &v, &dv, &dv, &dv, &dv, 5L, 4096);
  
  C2F(dr)("xget","thickness",&verbose,&old_thick,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  
  C2F(dr)("xget","line style",&verbose,old_linestyle,&narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);

  for(i=1;i<6;i++) linestyle[i] = old_linestyle[i];

  /* My default for marks drawing */
  C2F (dr) ("xset", "line style", linestyle, PI0, PI0, PI0, PI0, PI0, PD0,
	    PD0, PD0, PD0, 0L, 0L);
  
  thick = sciGetLineWidth (pobj); /* to answer the request from newsgroup to have marks thickness == line thickness every time */
  if(thick < 1) thick = 1;
  
  C2F(dr)("xset","thickness",&thick,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  /* end of default*/

  if(size < 2){
    /* represents a . (pixel) with editable foreground */
    for(i=0;i<n1;i++)
      {
	int deux = 2;
	int xmasterix[2];
	int ymasterix[2];
	
	C2F (dr) ("xset", "dashes", &foreground, &foreground, x+4, x+4, x+4, &v, &dv,
		  &dv, &dv, &dv, 5L, 4096);
	C2F (dr) ("xset", "foreground", &foreground, &foreground, x+4, x+4, x+4, &v,
		  &dv, &dv, &dv, &dv, 5L, 4096);
	
	xmasterix[0] = xm[i];
	xmasterix[1] = xm[i] + pixel_offset;
	ymasterix[0] = 	ymasterix[1] = ym[i];
	C2F (dr) ("xlines", "xv", &deux, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
      }
    return 0;
  }
  
  foreground = sciGetMarkForeground(pobj);
  background = sciGetMarkBackground(pobj);

  switch(style){
  case 0:
    /* represents a simple full dot with editable foreground */
    for(i=0;i<n1;i++)
      DrawMark_FullDot(xm[i], ym[i], size_minus_one, foreground, foreground, pixel_offset);
    
    break;
  case 1:
    /* represents a plus with editable foreground */
    for(i=0;i<n1;i++)
      DrawMark_Plus(xm[i], ym[i], (int)(size/2), foreground, pixel_offset);
    
    break;
  case 2:
    /* represents a cross with editable foreground */
    for(i=0;i<n1;i++)
	DrawMark_Cross(xm[i], ym[i], (int)(size/2), foreground, pixel_offset);
    
    break;
  case 3:
    
    /* represents a circle AND a plus within this circle with editable foreground and background */
    for(i=0;i<n1;i++)
      {
	DrawMark_FullDot(xm[i], ym[i], size_minus_one, foreground, background, pixel_offset);
	DrawMark_Plus(xm[i], ym[i], (int)(size/2), foreground, pixel_offset);
      }
    break;
  case 4:
    /* represents a diamond with background == foreground  */
    for(i=0;i<n1;i++)
      DrawMark_FullDiamond(xm[i], ym[i], (int)(size/2), foreground, foreground);

    break;
  case 5:
    /* represents a diamond with both editable foreground and background  */
    for(i=0;i<n1;i++)
      DrawMark_FullDiamond(xm[i], ym[i], (int)(size/2), foreground, background);
    
    break;
  case 6:
    /* represents an upward-pointing triangle with both editable foreground and background  */
    for(i=0;i<n1;i++)
      DrawMark_FullTriangleUp(xm[i], ym[i], (int)size, foreground, background);
    
    break;
  case 7:
    /* represents a downward-pointing triangle with both editable foreground and background  */
    for(i=0;i<n1;i++)
      DrawMark_FullTriangleDown(xm[i], ym[i], (int)size, foreground, background);
    
    break;
  case 8:
    /* represents a diamond with a plus inside with both editable foreground and background  */
    for(i=0;i<n1;i++)
      {	
	DrawMark_FullDiamond(xm[i], ym[i], (int)(size/2), foreground, background);
	DrawMark_Plus(xm[i], ym[i], (int)(size/2), foreground, pixel_offset);
      }
    break;
  case 9:
    /* represents a circle with editable foreground and background */
    for(i=0;i<n1;i++)
      DrawMark_FullDot(xm[i], ym[i], size_minus_one, foreground, background, pixel_offset);

    break;
  case 10:
    /* Asterix case : we can modify only the foreground (background changes have no impact) */
    for(i=0;i<n1;i++)
      DrawMark_Asterisk(xm[i], ym[i],(int) (size/2), foreground, pixel_offset);
    
    break;
  case 11:
    /* represents a square with editable foreground and background */
    for(i=0;i<n1;i++)
      DrawMark_FullSquare(xm[i], ym[i],(int) size_minus_one, foreground, background, pixel_offset);

    break;
  case 12:
    /* represents a right-pointing triangle with both editable foreground and background  */
    for(i=0;i<n1;i++)
      DrawMark_FullTriangleRight(xm[i], ym[i],(int) size, foreground, background);
    
    break;
  case 13:
    /* represents a left-pointing triangle with both editable foreground and background  */
    for(i=0;i<n1;i++)
      DrawMark_FullTriangleLeft(xm[i], ym[i],(int) size, foreground, background);

    break;
  case 14:
    /* represents a pentagram with both editable foreground and background  */
    for(i=0;i<n1;i++)
      DrawMark_FullPentagram(xm[i], ym[i], (int)(size/2), foreground, background);

    break;
  default:
    break;
  }
 
  C2F (dr) ("xset", "line style", old_linestyle, PI0, PI0, PI0, PI0, PI0, PD0,
	    PD0, PD0, PD0, 0L, 0L);
  
  C2F(dr)("xset","thickness",&old_thick,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  
  C2F (dr) ("xset", "dashes", &old_foreground, &old_foreground, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &old_foreground, &old_foreground, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
  
  return 0; 
}


void DrawMarks3D(sciPointObj *pobj, int n1, int *xm, int *ym, int *DPI)
{
  integer v;
  double dv=0;
  int x[4], markidsizenew[2];

  if (sciGetIsMark(pobj) == TRUE){
    x[0] = sciGetMarkForeground(pobj);
    
    markidsizenew[0] = sciGetMarkStyle(pobj);
    markidsizenew[1] = sciGetMarkSize (pobj);
    
    C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, &dv,
	      &dv, &dv, &dv, 5L, 4096);
    C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4, &v,
	      &dv, &dv, &dv, &dv, 5L, 4096);
    
    C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
	      PD0, PD0, 0L, 0L);   
    
    DrawNewMarks(pobj,n1,xm,ym,DPI);
  }
}



/* F.Leray 25.02.05 */
/* This function is used to determine how the pixel data*/
/* is given to Xt method (like XDrawLines) */
/* 0 : pixel itself */
/* 1 : pixel's interval */
/* Explanation :*/
/* i.e.in X11 (same thing using Win32 driver), with x2=x1+1 and y1=y2, we have : */
/*   XDrawLine(x1,y1,x1,y1)  does nothing (no pixel lit) */
/*   XDrawLine(x1,y1,x2,y2)  ONE pixel lit */
/* BUT the corresponding function with driver Gtk, Gif (and others?) give us: */
/*   GTKDrawLine(x1,y1,x1,y1)  ONE pixel lit */
/*   GTKDrawLine(x1,y1,x2,y2)  TWO pixels lit */
/* for now (and only for marks), I make a trick using a pixel_offset set to 0 or 1 */
/* depending on the driver. */

/* slight modif from Bruno : if fact GetDriverId return also 0 */
/* if gtk is enable so I have added a call to withgtk to get the */
/* the good pixel_offset */
extern int C2F(withgtk)(int *rep);

int CheckPixelStatus(void)
{
  if(GetDriverId() == 0) /* X11 or Win32 driver or Gtk driver */
    { 
      int irep;
      C2F(withgtk)(&irep);
      if (irep)
	return 0;
      else
	return 1;
    }
  else
    return 0;
}



/* center : xmi,ymi */
/* diameter : size */
int DrawMark_FullDot(int xmi, int ymi, double size, int foreground, int background, int pixel_offset)
{
  int x1 = (int) (xmi - size/2);
  int yy1= (int) (ymi - size/2);
  int w1 = (int) size+pixel_offset;
  int h1 = (int) size+pixel_offset; 
  char str[2] = "xv";
  int x[4];
  integer v;
  double dv;

  integer angle1 = 0;
  integer angle2 = 64*360;

  C2F (dr) ("xset", "dashes", &background, &background, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
  
  C2F (dr) ("xfarc", str, &x1, &yy1, &w1, &h1, &angle1, &angle2, PD0, PD0, PD0,PD0, 5L, 0L);
  
  C2F (dr) ("xset", "dashes", &foreground, &foreground, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
	
  C2F (dr) ("xarc", str, &x1, &yy1, &w1, &h1, &angle1, &angle2, PD0, PD0, PD0,PD0, 5L, 0L);
  
  return 0;
}

/* center : xmi,ymi */
/* "+" half-size (minus centered pixel) : size */
int DrawMark_Plus(int xmi, int ymi, int size, int foreground, int pixel_offset)
{
  int deux = 2;
  int xmasterix[2];
  int ymasterix[2];
  int x[4],v,closeflag=0;
  double dv;
  
  C2F (dr) ("xset", "dashes", &foreground, &foreground, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
  
  /* the "+" */
  xmasterix[0] = xmi -((int) size) ;
  xmasterix[1] = xmi +(int) size + pixel_offset;
  ymasterix[0] = 	ymasterix[1] = ymi;
/*   C2F(dr)("xsegs","v", xmasterix, ymasterix, &deux,&foreground,&closeflag,PI0,PD0,PD0,PD0,PD0,0L,0L); */
  C2F (dr) ("xlines", "xv", &deux, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);

  
  xmasterix[0] = xmasterix[1] = xmi;
  ymasterix[0] = ymi - (int) size;
  ymasterix[1] = ymi + (int) size + pixel_offset;
/*   C2F(dr)("xsegs","v", xmasterix, ymasterix, &deux,&foreground,&closeflag,PI0,PD0,PD0,PD0,PD0,0L,0L); */
  C2F (dr) ("xlines", "xv", &deux, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
   
  return 0;
}

/* center : xmi,ymi */
/* "x" half-size (minus centered pixel) : size */
int DrawMark_Cross(int xmi, int ymi, int size, int foreground, int pixel_offset)
{
  int deux = 2;
  int xmasterix[2];
  int ymasterix[2];
  int x[4],v,closeflag=0;
  double dv;
  
  C2F (dr) ("xset", "dashes", &foreground, &foreground, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
  
  /* the "/" */
  xmasterix[0] = (int) (xmi - size);
  xmasterix[1] = (int) (xmi + size + pixel_offset);
  ymasterix[0] = (int) (ymi + size);
  ymasterix[1] = (int) (ymi - size - pixel_offset);

  C2F (dr) ("xlines", "xv", &deux, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  /* end */
	
  /* the "\" */
  xmasterix[0] = (int) (xmi - size);
  xmasterix[1] = (int) (xmi + size + pixel_offset);
  ymasterix[0] = (int) (ymi - size);
  ymasterix[1] = (int) (ymi + size + pixel_offset);
  C2F (dr) ("xlines", "xv", &deux, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  /* end */

  return 0;
}

/* center : xmi,ymi */
/* represents a diamond with both editable foreground and background  */
int DrawMark_FullDiamond(int xmi, int ymi, int size, int foreground, int background)
{
  int quatre = 4, un=1;
  int xmdiamond[4];
  int ymdiamond[4];
  int x[4],v;
  double dv;
	
  xmdiamond[0] = xmi - size;
  xmdiamond[1] = xmi;
  xmdiamond[2] = xmi + size;
  xmdiamond[3] = xmi;
	
  ymdiamond[0] = ymi;
  ymdiamond[1] = ymi + size;
  ymdiamond[2] = ymi;
  ymdiamond[3] = ymi - size;
  
  C2F (dr) ("xset", "dashes", &background, &background, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
	
  C2F (dr) ("xarea", "v", &quatre, xmdiamond, ymdiamond, &un, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);
  
  if(foreground != background){
    C2F (dr) ("xset", "dashes", &foreground, &foreground, x+4, x+4, x+4, &v, &dv,
	      &dv, &dv, &dv, 5L, 4096);
    C2F (dr) ("xset", "foreground", &foreground, &foreground, x+4, x+4, x+4, &v,
	      &dv, &dv, &dv, &dv, 5L, 4096);
    
    C2F (dr) ("xlines", "xv", &quatre, xmdiamond, ymdiamond, &un, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  }
  
  return 0;
}


int DrawMark_FullTriangleUp(int xmi, int ymi, int size, int foreground, int background)
{
  int trois = 3, un=1;
  int xmtriangle[3];
  int ymtriangle[3];
  int x[4],v;
  double dv;
   
  xmtriangle[0] = (int) (xmi - size/2);
  xmtriangle[1] = (int) (xmi + size/2);
  xmtriangle[2] = xmi;
	
  ymtriangle[0] = (int) (ymi + size/3);
  ymtriangle[1] = (int) (ymi + size/3);
  ymtriangle[2] = (int) (ymi - 2*size/3);

  C2F (dr) ("xset", "dashes", &background, &background, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
	
  C2F (dr) ("xarea", "v", &trois, xmtriangle, ymtriangle, &un, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);
	
  C2F (dr) ("xset", "dashes", &foreground, &foreground, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);

  if(foreground != background){
    C2F (dr) ("xset", "foreground", &foreground, &foreground, x+4, x+4, x+4, &v,
	      &dv, &dv, &dv, &dv, 5L, 4096);
    
    C2F (dr) ("xlines", "xv", &trois, xmtriangle, ymtriangle, &un, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  }
  
  return 0;
}
      

int DrawMark_FullTriangleDown(int xmi, int ymi, int size, int foreground, int background)
{
  int trois = 3, un=1;
  int xmtriangle[3];
  int ymtriangle[3];
  int x[4],v;
  double dv;
   	
  xmtriangle[0] = (int) (xmi - size/2);
  xmtriangle[1] = (int) (xmi + size/2);
  xmtriangle[2] = (int) (xmi);
  
  ymtriangle[0] = (int) (ymi - size/3);
  ymtriangle[1] = (int) (ymi - size/3);
  ymtriangle[2] = (int) (ymi + 2*size/3);
  
  C2F (dr) ("xset", "dashes", &background, &background, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
  
  C2F (dr) ("xarea", "v", &trois, xmtriangle, ymtriangle, &un, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);
  
  C2F (dr) ("xset", "dashes", &foreground, &foreground, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
  
  C2F (dr) ("xlines", "xv", &trois, xmtriangle, ymtriangle, &un, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  
  return 0;
}
      
int DrawMark_Asterisk(int xmi, int ymi, int size, int foreground, int pixel_offset)
{
  int deux = 2;
  int xmasterix[2];
  int ymasterix[2];
  int sizecross = (int)(0.8*size);
  int x[4],v,closeflag=0;
  double dv;
  
  C2F (dr) ("xset", "dashes", &foreground, &foreground, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
	
  /* the "+" */
  xmasterix[0] = xmi - size;
  xmasterix[1] = xmi + size +pixel_offset;
  ymasterix[0] = ymasterix[1] = ymi;
  C2F (dr) ("xlines", "xv", &deux, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
	
  xmasterix[0] = xmasterix[1] = xmi;
  ymasterix[0] = ymi - size;
  ymasterix[1] = ymi + size +pixel_offset;
  C2F (dr) ("xlines", "xv", &deux, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  /* end */

  /* the "/" */
  xmasterix[0] =(int)( xmi - ceil(sizecross));
  xmasterix[1] =(int)( xmi + ceil(sizecross)+pixel_offset);
  ymasterix[0] = (int)(ymi + ceil(sizecross));
  ymasterix[1] = (int)(ymi - ceil(sizecross)-pixel_offset);
  C2F (dr) ("xlines", "xv", &deux, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  /* end */
	
  /* the "\" */
  xmasterix[0] = (int)(xmi - ceil(sizecross));
  xmasterix[1] = (int)(xmi + ceil(sizecross)+pixel_offset);
  ymasterix[0] = (int)(ymi - ceil(sizecross));
  ymasterix[1] = (int)(ymi + ceil(sizecross)+pixel_offset);
  C2F (dr) ("xlines", "xv", &deux, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  /* end */

  return 0;
}


int DrawMark_FullSquare(int xmi, int ymi, int size, int foreground, int background, int pixel_offset)
{
  int x1 = xmi - size/2;
  int yy1= ymi - size/2;
  int w1 = size;
  int h1 = size;
  char str[2] = "xv";
  int x[4],v;
  double dv;
  
  C2F (dr) ("xset", "dashes", &background, &background, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
	
  C2F (dr) ("xfrect", str, &x1, &yy1, &w1, &h1, PI0, PI0, PD0, PD0, PD0,PD0, 5L, 0L);
	
  C2F (dr) ("xset", "dashes", &foreground, &foreground, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
	
  C2F (dr) ("xrect", str, &x1, &yy1, &w1, &h1, PI0, PI0, PD0, PD0, PD0,PD0, 5L, 0L);
	

  return 0;
}


int DrawMark_FullTriangleRight(int xmi, int ymi, int size, int foreground, int background)
{
  int trois = 3, un=1;
  int xmtriangle[3];
  int ymtriangle[3];
  int x[4],v;
  double dv;
  
  xmtriangle[0] = (int) (xmi - size/3);
  xmtriangle[1] = (int) (xmi - size/3);
  xmtriangle[2] = (int) (xmi + 2*size/3);
	
  ymtriangle[0] = (int) (ymi - size/2);
  ymtriangle[1] = (int) (ymi + size/2);
  ymtriangle[2] = ymi;
	
  C2F (dr) ("xset", "dashes", &background, &background, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
	
  C2F (dr) ("xarea", "v", &trois, xmtriangle, ymtriangle, &un, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);

  C2F (dr) ("xset", "dashes", &foreground, &foreground, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xlines", "xv", &trois, xmtriangle, ymtriangle, &un, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);

  return 0;
}


int DrawMark_FullTriangleLeft(int xmi, int ymi, int size, int foreground, int background)
{

  int trois = 3, un=1;
  int xmtriangle[3];
  int ymtriangle[3];
  int x[4],v;
  double dv;
  
  xmtriangle[0] = (int) (xmi + size/3);
  xmtriangle[1] = (int) (xmi + size/3);
  xmtriangle[2] = (int) (xmi - 2*size/3);
	
  ymtriangle[0] = (int) (ymi - size/2);
  ymtriangle[1] = (int) (ymi + size/2);
  ymtriangle[2] = ymi;

  C2F (dr) ("xset", "dashes", &background, &background, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
	
  C2F (dr) ("xarea", "v", &trois, xmtriangle, ymtriangle, &un, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);

  C2F (dr) ("xset", "dashes", &foreground, &foreground, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xlines", "xv", &trois, xmtriangle, ymtriangle, &un, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);

  
  return 0;
}


int DrawMark_FullPentagram(int xmi, int ymi, int size, int foreground, int background)
{
  int dix= 10, un=1;
  int xmpoints1[5],ympoints1[5];
  int xmpoints2[5],ympoints2[5];
  int xmpoints[10], ympoints[10];
  int x[4],v;
  double dv;
	
  double r2 = size*cos(72*M_PI/180)/cos(36*M_PI/180);

  xmpoints1[0] = (int)(xmi - size*cos(54*M_PI/180));
  ympoints1[0] = (int)(ymi + size*sin(54*M_PI/180));
	
  xmpoints1[1] = (int)(xmi + size*cos(54*M_PI/180));
  ympoints1[1] = (int)(ymi + size*sin(54*M_PI/180));
	
  xmpoints1[2] = (int)(xmi + (xmpoints1[1] - xmi) *cos(72*M_PI/180) + (ympoints1[1] - ymi) *sin(72*M_PI/180));
  ympoints1[2] = (int)(ymi - (xmpoints1[1] - xmi) *sin(72*M_PI/180) + (ympoints1[1] - ymi) *cos(72*M_PI/180));

  xmpoints1[3] = xmi;
  ympoints1[3] = ymi - size;

  xmpoints1[4] = (int)(xmi + (xmpoints1[0] - xmi) *cos(-72*M_PI/180) + (ympoints1[0] - ymi) *sin(-72*M_PI/180));
  ympoints1[4] = (int)(ymi - (xmpoints1[0] - xmi) *sin(-72*M_PI/180) + (ympoints1[0] - ymi) *cos(-72*M_PI/180));

	
  xmpoints2[0] = (int)( xmi - r2*sin(36*M_PI/180));
  ympoints2[0] = (int)(ymi - size*cos(72*M_PI/180));

  xmpoints2[1] = (int)(xmi + r2*sin(36*M_PI/180));
  ympoints2[1] = (int)(ymi - size*cos(72*M_PI/180));

  xmpoints2[2] = (int)(xmi + (xmpoints2[1] - xmi)*cos(-72*M_PI/180) + (ympoints2[1] - ymi) *sin(-72*M_PI/180));
  ympoints2[2] = (int)(ymi - (xmpoints2[1] - xmi)*sin(-72*M_PI/180) + (ympoints2[1] - ymi) *cos(-72*M_PI/180));

  xmpoints2[3] = xmi;
  ympoints2[3] = (int)(ymi + r2);

  xmpoints2[4] = (int)(xmi + (xmpoints2[0] - xmi)*cos(72*M_PI/180) + (ympoints2[0] - ymi) *sin(72*M_PI/180));
  ympoints2[4] = (int)(ymi - (xmpoints2[0] - xmi)*sin(72*M_PI/180) + (ympoints2[0] - ymi) *cos(72*M_PI/180));

	
  xmpoints[0] = xmpoints2[4];
  ympoints[0] = ympoints2[4];

  xmpoints[1] = xmpoints1[0];
  ympoints[1] = ympoints1[0];

  xmpoints[2] = xmpoints2[3];
  ympoints[2] = ympoints2[3];

  xmpoints[3] = xmpoints1[1];
  ympoints[3] = ympoints1[1];

  xmpoints[4] = xmpoints2[2];
  ympoints[4] = ympoints2[2];

  xmpoints[5] = xmpoints1[2];
  ympoints[5] = ympoints1[2];

  xmpoints[6] = xmpoints2[1];
  ympoints[6] = ympoints2[1];

  xmpoints[7] = xmpoints1[3];
  ympoints[7] = ympoints1[3];

  xmpoints[8] = xmpoints2[0];
  ympoints[8] = ympoints2[0];

  xmpoints[9] = xmpoints1[4];
  ympoints[9] = ympoints1[4];


  C2F (dr) ("xset", "dashes", &background, &background, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);
	
  C2F (dr) ("xarea", "v", &dix, xmpoints, ympoints, &un, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);
	
  C2F (dr) ("xset", "dashes", &foreground, &foreground, x+4, x+4, x+4, &v, &dv,
	    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, x+4, x+4, x+4, &v,
	    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xlines", "xv", &dix, xmpoints, ympoints, &un, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);

  return 0;
}


/* we draw marks in pixel */
/* Returns the DPI depending on the used driver */
int GetDPIFromDriver(int * DPI)
{
  int driver = GetDriverId(); /* return the first letter of the driver name (see XCall.c) */
  int succeed = 0;
  int ixres, iyres;
  
  switch(driver)
    {
    case 0: 
	  if(WindowsPrintScreen == 1)
	   {
         ixres = Printer_XRes;
         iyres = Printer_YRes;
		 succeed = 1;
	   }
	  else
	   {
        succeed = GetScreenDPI(&ixres,&iyres);
        if(succeed == -1){
	    /* gtk version <2 enabled */
	     ixres = (int)72.; /* default value*/
	     iyres = (int)72.; /* default value*/
        }
	   }
      break;
    case 1: /* Pos */
      /*       printf("DRIVERS POS enabled -- -- -- --\n"); */
      /* when using Pos driver, the output file is 6000x4240 pixels */
      /* computed DPI: height : 6000/(30cm/2.54) = 508 ; width: 4240/(21.20/2.54) = 508 */
      ixres = (int)(524.*1.5);
      iyres = (int)(524.*1.5);
      break;
    case 2: /* Fig. */
      /*       printf("DRIVERS FIG enabled -- -- -- --\n"); */
      /* when using Pos driver, the output file is 6000x4240 pixels */
      /* computed DPI: height : 9600/(8inches) = 1200 ; width: 6784/(5.7inches) = 1190 */
      ixres = (int)1200.;
      iyres = (int)1190.;
      break;
    case 3: /* Gif & PPM driver */ /* NOT SURE: may be 72. avery time... */
    default:
      succeed = GetScreenDPI(&ixres,&iyres);
      if(succeed == -1){
	/* gtk version <2 enabled */
	ixres = (int)72.; /* default value*/	
	iyres = (int)72.; /* default value*/
      }
      break;
    }

  DPI[0] = ixres;
  DPI[1] = iyres;

  return 0;
}



/* 1----------4 */
/* |          | */
/* |          | */
/* |          | */
/* 2----------3 */

int CheckDisplay(double fact_h, double fact_w, char logflag, char *foo,int *posi,int *fontid,int *old_rect) 
{
  int rect[4],i;
  int point[4][2];
  int logrect[4], XX, YY;
  
  if(old_rect[0] == 0 && old_rect[1] == 0 && old_rect[2] == 0 && old_rect[3] == 0)
    return 1;
  
  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
  
  /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
  C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
  
  if(logflag == 'n')
    {
      C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      rect[3] = (integer)(fact_h* rect[3]); /* added the 01.06.05 */
      rect[2] = (integer)(fact_w* rect[2]);
   }
  else
    {
      int smallersize = fontid[1]-2;
      int rect10[4];
      int posi10[2];
      
      posi10[0] = posi[0] - logrect[2];
      posi10[1] = posi[1] + logrect[3];
      
      C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      
      posi[0] = rect10[0] + rect10[2];
      posi[1] = (int) (rect10[1] - rect10[3]*.1);
      
      C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      
      rect[2] = (integer)(fact_w*(rect[2] + rect10[2]));
      rect[3] = (integer)(fact_h*(rect[3] + rect10[3] + (int) (rect10[3]*.1))); /* added the 01.06.05 */
/*       rect[3] = rect[3] + rect10[3] + (int) (rect10[3]*.1); /\* added the 01.06.05 *\/ */
      rect[0] = rect10[0];
      rect[1] = rect[1];

      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  
  
  point[0][0] = rect[0]; /* upper left point */
  point[0][1] = rect[1];
  
  point[1][0] = rect[0]; /* lower left point */
  point[1][1] = rect[1]+rect[3];
  
  point[2][0] = rect[0]+rect[2]; /* lower right point */
  point[2][1] = rect[1]+rect[3];
  
  point[3][0] = rect[0]+rect[2]; /* upper right point */
  point[3][1] = rect[1];
  
  for(i=0;i<4;i++)
    if(IsInsideRectangle(old_rect,point[i]) == 0)
      return 0; /* If one inside the old_rect, DO NOT display the graduation ! */


  return 1;
}



/* The unit is the pixel */
/* return 0 if the point is inside the rect */
/* 1 if it is actually outside the rect */
/* (rect[0],rect[1]) : upper left point of the bounding box  in pixel */
/* (rect[2],rect[3]) : width and height in pixel */
/* point[0] : x component */
/* point[1] : y component */
int IsInsideRectangle(int * rect, int *point)
{

  if((point[0] >= rect[0] && point[0] <= rect[0]+rect[2]) &&
     (point[1] >= rect[1] && point[1] <= rect[1]+rect[3]))
    return 0;
  
  return 1;
}



int AdaptGraduationsOnYBottomLeft(int iof, int x, int y, int size, integer *Ticsdir, int *fontid, sciPointObj * psubwin, double yminval, double ymaxval, double fx, double fy, double fz)
{
  int i;
  int nbtics, nbsubtics;
  int lastyindex;
  int old_rect[4];
  int nb_grads_max = 0;
  int vx[2], vy[2];
  int xm, ym;
  double grads_tmp[20];
  char c_format[5];
  integer barlengthx = 0,barlengthy = 0;
  integer rect[4],posi[2]; 
  integer textcolor = -1;
  int logrect[4],XX,YY;
  int pas;
  double fact_h = 1.5, fact_w = 1.5;
  int compteur = 0;

  int possible_pas, possible_compteur;
  
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  for(i=0;i<4;i++) old_rect[i] = 0; /* Init. old_rect to force first grad. to be displayed */
  
  lastyindex = ppsubwin->axes.nygrads - 1;
  
  if(lastyindex == 0)
    ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.ygrads[0]));
  else
    ChoixFormatE(c_format,
		 ppsubwin->axes.ygrads[0],
		 ppsubwin->axes.ygrads[lastyindex],
		 ((ppsubwin->axes.ygrads[lastyindex])-(ppsubwin->axes.ygrads[0]))/(lastyindex));
  
  nbtics = ppsubwin->axes.nygrads;
  nbsubtics = ppsubwin->axes.nbsubtics[1];
  
  
  for(i=0;i<nbtics;i++) grads_tmp[i] = ppsubwin->axes.ygrads[i];

  for(i=0;i<nbtics;i++)
    {
      char foo[256]; 
      double ytmp = ppsubwin->axes.ygrads[i];
		  
/*       if(ytmp<yminval || ytmp>ymaxval)  */
/* 	{ */
/* 	  /\*   sciprint("je rejete la valeur: %lf\n\n",xtmp); *\/ */
/* 	  continue; /\* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon *\/ */
/* 	  /\* donc autant ne pas aller plus loin dans l'algo... *\/ */
/* 	} */
		  
      sprintf(foo,c_format,ytmp);

      /***************************************************************/
      /************************* COMMON PART *************************/
      /***************************************************************/
      if(ppsubwin->axes.reverse[1] == TRUE)
	ytmp = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp);
		  
      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz);
		  
      vx[0]=xm;vy[0]=ym; 
		  
      barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
      barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
      C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      

/*       if (IsDownAxes(psubwin)){ */
/* 	vx[1]=vx[0]; */
/* 	vy[1]=vy[0]+iof/2; */
/* 	posi[0] = inint(xm-rect[2]/2);  */
/* 	posi[1]=inint( vy[0] + iof + rect[3]);} */
/*       else{ */
/* 	vx[1]=vx[0]+barlengthx; */
/* 	vy[1]=vy[0]+barlengthy; */
/* 	posi[0] = inint( xm+2*barlengthx); */
/* 	posi[1]=inint( ym + 2*barlengthy + rect[3]);} */
		      
      
      if (IsDownAxes(psubwin)){
	vx[1]=vx[0];
	vy[1]=vy[0]+iof/2;
	posi[0] = inint(xm-rect[2]/2); 
	posi[1]=inint( vy[0] + iof + rect[3]);}
      else{
	vx[1]=vx[0]+barlengthx;
	vy[1]=vy[0]+barlengthy;
/* 	posi[0] = inint( xm+2*barlengthx-rect[2]/2);  */
	posi[0] = inint( xm+2*barlengthx-rect[2]); 
	posi[1]=inint( ym + 2*barlengthy + rect[3]);}
      
      /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
      C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
      
      
      if(CheckDisplay(fact_h, fact_w, ppsubwin->logflags[1],foo,posi,fontid,old_rect) == 0)
	continue; /*  graduation too close, DO NOT display the graduation ! */
      
      
      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if ( ppsubwin->logflags[1] == 'l' )
	{
	  int smallersize = fontid[1]-2;
	  int old_rect10[4];
	  int posi10[2];

	  posi10[0] = posi[0] - logrect[2];
	  posi10[1] = posi[1] + logrect[3];
	  
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  posi[0] = old_rect10[0] + old_rect10[2];
	  posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);
	  
	  C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  /* update old_rect */
	  old_rect[2] = (int)(fact_w*(old_rect[2] + (double)old_rect10[2]));
	  old_rect[3] = (int)(fact_h*(old_rect[3] + old_rect10[3] + (int) (old_rect10[3]*.1)));
	  old_rect[0] = old_rect10[0];
	  old_rect[1] = old_rect[1];
	  
	  nb_grads_max++;
	  
	  /* put back the current fontid */
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      else{
	/* update old_rect */
	C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	old_rect[3] = (int)(fact_h* old_rect[3]);
	old_rect[2] = (int)(fact_w* old_rect[2]);
	nb_grads_max++;
      }
    }
  
  if(ppsubwin->logflags[1] != 'l')
    if(nb_grads_max == 1) /* only one grad. can be displayed : we choose to display the average value (max+min)/2 */
      {
	ppsubwin->axes.ygrads[0] = (grads_tmp[0]+grads_tmp[lastyindex])/2.;
	ppsubwin->axes.nygrads = 1;
        
        if ( !ppsubwin->flagNax )
        {
          /* don't change the number if it aldready has been specified */
          ppsubwin->axes.nbsubtics[1] = 1 ;
        }
	return 0;
      }
  
  
  pas = nbtics - 2; /* pas == grads number - 2 */
  
  possible_pas = -99;
  possible_compteur = -99;
  
  while(pas > 0)
    {
      int tmp = 0;
      compteur = 0;
      for(;;)
	{
	  tmp = tmp + pas;
	  compteur++;
	  
	  if((tmp == (nbtics - 1)) && (compteur < nb_grads_max)){
	    possible_pas = pas;
	    possible_compteur = ++compteur;
	    break;
	  }
	  
	  if(tmp > (nbtics - 1))
	    break;
	}
      
      pas--;
    }
  
  
  if(possible_compteur != -99){
    compteur = possible_compteur;
    pas = possible_pas;
    
    for(i=0;i<compteur;i++)
      ppsubwin->axes.ygrads[i] = grads_tmp[i*pas];
    
    ppsubwin->axes.nygrads = compteur;
    
    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nygrads,
                                                           ppsubwin->logflags[1],
							   ppsubwin->axes.ygrads,
                                                           ppsubwin->axes.nbsubtics[1]); 
  }
  else{
    ppsubwin->axes.ygrads[0] = grads_tmp[0];
    ppsubwin->axes.ygrads[1] = grads_tmp[lastyindex];
    ppsubwin->axes.nygrads = 2;
    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nygrads,
                                                           ppsubwin->logflags[1],
							   ppsubwin->axes.ygrads,
                                                           ppsubwin->axes.nbsubtics[1]); 
    
    if(ppsubwin->logflags[1] == 'n'){
      if(nb_grads_max > 4) {
	/* we could display at least 4 graduations but we did not find a proper interval... */
	/* To avoid to display only the min and max (see above), we add 2 newly created grads by interpolating between min and max */
	double pas = (grads_tmp[lastyindex]-grads_tmp[0])/3;
	for(i=0;i<3;i++){
	  ppsubwin->axes.ygrads[i] = grads_tmp[0] + i*pas;
	}
	
	ppsubwin->axes.ygrads[3] = grads_tmp[lastyindex]; /* exact max */
	ppsubwin->axes.nygrads = 4;
        /* Nb of subtics computation and storage */
	ppsubwin->axes.nbsubtics[1] = ComputeNbSubTicsFor3dUse(psubwin,
                                                               ppsubwin->axes.nygrads,
                                                               'n',
							       ppsubwin->axes.ygrads,
                                                               ppsubwin->axes.nbsubtics[1]); 
      }
    }
  }
  
  return 0;
}


int AdaptGraduationsOnXBottomLeft(int iof, int x, int y, int size, integer *Ticsdir, int *fontid, sciPointObj * psubwin, double xminval, double xmaxval, double fx, double fy, double fz)
{
  int i;
  int nbtics, nbsubtics;
  int lastxindex;
  int old_rect[4];
  int nb_grads_max = 0;
  int vx[2], vy[2];
  int xm, ym;
  double grads_tmp[20];
  char c_format[5];
  integer barlengthx = 0,barlengthy = 0;
  integer rect[4],posi[2]; 
  integer textcolor = -1;
  int logrect[4],XX,YY;
  int pas;
  double fact_h = 1.5, fact_w = 1.5;
  int compteur = 0;

  int possible_pas, possible_compteur;
  
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  for(i=0;i<4;i++) old_rect[i] = 0; /* Init. old_rect to force first grad. to be displayed */
  
  lastxindex = ppsubwin->axes.nxgrads - 1;
  
  if(lastxindex == 0)
    ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.xgrads[0]));
  else
    ChoixFormatE(c_format,
		 ppsubwin->axes.xgrads[0],
		 ppsubwin->axes.xgrads[lastxindex],
		 ((ppsubwin->axes.xgrads[lastxindex])-(ppsubwin->axes.xgrads[0]))/(lastxindex));
  
  nbtics = ppsubwin->axes.nxgrads;
  nbsubtics = ppsubwin->axes.nbsubtics[0];
  
  
  for(i=0;i<nbtics;i++) grads_tmp[i] = ppsubwin->axes.xgrads[i];

  for(i=0;i<nbtics;i++)
    {
      char foo[256]; 
      double xtmp = ppsubwin->axes.xgrads[i];
		  
/*       if(xtmp<xminval || xtmp>xmaxval)  */
/* 	{ */
/* 	  /\*   sciprint("je rejete la valeur: %lf\n\n",xtmp); *\/ */
/* 	  continue; /\* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon *\/ */
/* 	  /\* donc autant ne pas aller plus loin dans l'algo... *\/ */
/* 	} */
		  
      sprintf(foo,c_format,xtmp);

      /***************************************************************/
      /************************* COMMON PART *************************/
      /***************************************************************/
      if(ppsubwin->axes.reverse[0] == TRUE)
	xtmp = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp);
		  
      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz);
		  
      vx[0]=xm;vy[0]=ym; 
		  
      barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
      barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
      C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      

/*       if (IsDownAxes(psubwin)){ */
/* 	vx[1]=vx[0]; */
/* 	vy[1]=vy[0]+iof/2; */
/* 	posi[0] = inint(xm-rect[2]/2);  */
/* 	posi[1]=inint( vy[0] + iof + rect[3]);} */
/*       else{ */
/* 	vx[1]=vx[0]+barlengthx; */
/* 	vy[1]=vy[0]+barlengthy; */
/* 	posi[0] = inint( xm+2*barlengthx); */
/* 	posi[1]=inint( ym + 2*barlengthy + rect[3]);} */
		      
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
      
      /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
      C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
      
      
      if(CheckDisplay(fact_h, fact_w, ppsubwin->logflags[0],foo,posi,fontid,old_rect) == 0)
	continue; /*  graduation too close, DO NOT display the graduation ! */
      
      
      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if ( ppsubwin->logflags[0] == 'l' )
	{
	  int smallersize = fontid[1]-2;
	  int old_rect10[4];
	  int posi10[2];

	  posi10[0] = posi[0] - logrect[2];
	  posi10[1] = posi[1] + logrect[3];
	  
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  posi[0] = old_rect10[0] + old_rect10[2];
	  posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);
	  
	  C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  /* update old_rect */
	  old_rect[2] = (int)(fact_w*(old_rect[2] + old_rect10[2]));
	  old_rect[3] = (int)(fact_h*(old_rect[3] + old_rect10[3] + (int) (old_rect10[3]*.1)));
	  old_rect[0] = old_rect10[0];
	  old_rect[1] = old_rect[1];
	  
	  nb_grads_max++;
	  
	  /* put back the current fontid */
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      else{
	/* update old_rect */
	C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	old_rect[3] = (int)(fact_h* old_rect[3]);
	old_rect[2] = (int)(fact_w* old_rect[2]);
	nb_grads_max++;
      }
    }
  
  if(ppsubwin->logflags[0] != 'l')
    if(nb_grads_max == 1) /* only one grad. can be displayed : we choose to display the average value (max+min)/2 */
      {
	ppsubwin->axes.xgrads[0] = (grads_tmp[0]+grads_tmp[lastxindex])/2.;
	ppsubwin->axes.nxgrads = 1;
        if ( !ppsubwin->flagNax )
        {
          ppsubwin->axes.nbsubtics[0] = 1;
        }
	return 0;
      }
  
  
  pas = nbtics - 2; /* pas == grads number - 2 */
  
  possible_pas = -99;
  possible_compteur = -99;
  
  while(pas > 0)
    {
      int tmp = 0;
      compteur = 0;
      for(;;)
	{
	  tmp = tmp + pas;
	  compteur++;
	  
	  if((tmp == (nbtics - 1)) && (compteur < nb_grads_max)){
	    possible_pas = pas;
	    possible_compteur = ++compteur;
	    break;
	  }
	  
	  if(tmp > (nbtics - 1))
	    break;
	}
      
      pas--;
    }
  
  
  if(possible_compteur != -99){
    compteur = possible_compteur;
    pas = possible_pas;
    
    for(i=0;i<compteur;i++)
      ppsubwin->axes.xgrads[i] = grads_tmp[i*pas];
    
    ppsubwin->axes.nxgrads = compteur;
    
    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nxgrads,
                                                           ppsubwin->logflags[0],
							   ppsubwin->axes.xgrads,
                                                           ppsubwin->axes.nbsubtics[0]); 
  }
  else{
    ppsubwin->axes.xgrads[0] = grads_tmp[0];
    ppsubwin->axes.xgrads[1] = grads_tmp[lastxindex];
    ppsubwin->axes.nxgrads = 2;
    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nxgrads,
                                                           ppsubwin->logflags[0],
							   ppsubwin->axes.xgrads,
                                                           ppsubwin->axes.nbsubtics[0]); 
    
    if(ppsubwin->logflags[0] == 'n'){
      if(nb_grads_max > 4) {
	/* we could display at least 4 graduations but we did not find a proper interval... */
	/* To avoid to display only the min and max (see above), we add 2 newly created grads by interpolating between min and max */
	double pas = (grads_tmp[lastxindex]-grads_tmp[0])/3;
	for(i=0;i<3;i++){
	  ppsubwin->axes.xgrads[i] = grads_tmp[0] + i*pas;
	}
	
	ppsubwin->axes.xgrads[3] = grads_tmp[lastxindex]; /* exact max */
	ppsubwin->axes.nxgrads = 4;
        
        /* Nb of subtics computation and storage */
        /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
	ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin,
                                                               ppsubwin->axes.nxgrads,
                                                               'n',
							       ppsubwin->axes.xgrads,
                                                               ppsubwin->axes.nbsubtics[0]); 
      }
    }
  }
  
  return 0;
}



int AdaptGraduationsOnYBottomRight(int iof, int x, int y, int size, integer *Ticsdir, int *fontid, sciPointObj * psubwin, double yminval, double ymaxval, double fx, double fy, double fz)
{
  int i;
  int nbtics, nbsubtics;
  int lastyindex;
  int old_rect[4];
  int nb_grads_max = 0;
  int vx[2], vy[2];
  int xm, ym;
  double grads_tmp[20];
  char c_format[5];
  integer barlengthx = 0,barlengthy = 0;
  integer rect[4],posi[2]; 
  integer textcolor = -1;
  int logrect[4],XX,YY;
  int pas;
  double fact_h = 1.5, fact_w = 1.5;
  int compteur = 0;

  int possible_pas, possible_compteur;
  
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  for(i=0;i<4;i++) old_rect[i] = 0; /* Init. old_rect to force first grad. to be displayed */
  
  lastyindex = ppsubwin->axes.nygrads - 1;
  
  if(lastyindex == 0)
    ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.ygrads[0]));
  else
    ChoixFormatE(c_format,
		 ppsubwin->axes.ygrads[0],
		 ppsubwin->axes.ygrads[lastyindex],
		 ((ppsubwin->axes.ygrads[lastyindex])-(ppsubwin->axes.ygrads[0]))/(lastyindex));
  
  nbtics = ppsubwin->axes.nygrads;
  nbsubtics = ppsubwin->axes.nbsubtics[1];
  
  
  for(i=0;i<nbtics;i++) grads_tmp[i] = ppsubwin->axes.ygrads[i];

  for(i=0;i<nbtics;i++)
    {
      char foo[256]; 
      double ytmp = ppsubwin->axes.ygrads[i];
		  
/*       if(ytmp<yminval || ytmp>ymaxval)  */
/* 	{ */
/* 	  /\*   sciprint("je rejete la valeur: %lf\n\n",xtmp); *\/ */
/* 	  continue; /\* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon *\/ */
/* 	  /\* donc autant ne pas aller plus loin dans l'algo... *\/ */
/* 	} */
		  
      sprintf(foo,c_format,ytmp);

      /***************************************************************/
      /************************* COMMON PART *************************/
      /***************************************************************/
      if(ppsubwin->axes.reverse[1] == TRUE)
	ytmp = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp);
		  
      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz);
		  
      vx[0]=xm;vy[0]=ym; 
		  
      barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
      barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
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
		      
      
      /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
      C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
      
      
      if(CheckDisplay(fact_h, fact_w, ppsubwin->logflags[1],foo,posi,fontid,old_rect) == 0)
	continue; /*  graduation too close, DO NOT display the graduation ! */
      
      
      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if ( ppsubwin->logflags[1] == 'l' )
	{
	  int smallersize = fontid[1]-2;
	  int old_rect10[4];
	  int posi10[2];

	  posi10[0] = posi[0] - logrect[2];
	  posi10[1] = posi[1] + logrect[3];
	  
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  posi[0] = old_rect10[0] + old_rect10[2];
	  posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);
	  
	  C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  /* update old_rect */
	  old_rect[2] = (int)(fact_w*(old_rect[2] + old_rect10[2]));
	  old_rect[3] = (int)(fact_h*(old_rect[3] + old_rect10[3] + (int) (old_rect10[3]*.1)));
	  old_rect[0] = old_rect10[0];
	  old_rect[1] = old_rect[1];
	  
	  nb_grads_max++;
	  
	  /* put back the current fontid */
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      else{
	/* update old_rect */
	C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	old_rect[3] = (int)(fact_h* old_rect[3]);
	old_rect[2] = (int)(fact_w* old_rect[2]);
	nb_grads_max++;
      }
    }
  
  if(ppsubwin->logflags[1] != 'l')
    if(nb_grads_max == 1) /* only one grad. can be displayed : we choose to display the average value (max+min)/2 */
      {
	ppsubwin->axes.ygrads[0] = (grads_tmp[0]+grads_tmp[lastyindex])/2.;
	ppsubwin->axes.nygrads = 1;
        if ( !ppsubwin->flagNax )
        {
          ppsubwin->axes.nbsubtics[1] = 1;
        }
	return 0;
      }
  
  
  pas = nbtics - 2; /* pas == grads number - 2 */
  
  possible_pas = -99;
  possible_compteur = -99;
  
  while(pas > 0)
    {
      int tmp = 0;
      compteur = 0;
      for(;;)
	{
	  tmp = tmp + pas;
	  compteur++;
	  
	  if((tmp == (nbtics - 1)) && (compteur < nb_grads_max)){
	    possible_pas = pas;
	    possible_compteur = ++compteur;
	    break;
	  }
	  
	  if(tmp > (nbtics - 1))
	    break;
	}
      
      pas--;
    }
  
  
  if(possible_compteur != -99){
    compteur = possible_compteur;
    pas = possible_pas;
    
    for(i=0;i<compteur;i++)
      ppsubwin->axes.ygrads[i] = grads_tmp[i*pas];
    
    ppsubwin->axes.nygrads = compteur;
    
    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nygrads,
                                                           ppsubwin->logflags[1],
							   ppsubwin->axes.ygrads,
                                                           ppsubwin->axes.nbsubtics[1]);
  }
  else{
    ppsubwin->axes.ygrads[0] = grads_tmp[0];
    ppsubwin->axes.ygrads[1] = grads_tmp[lastyindex];
    ppsubwin->axes.nygrads = 2;

    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nygrads,
                                                           ppsubwin->logflags[1],
							   ppsubwin->axes.ygrads,
                                                           ppsubwin->axes.nbsubtics[1]);
    
    if(ppsubwin->logflags[1] == 'n'){
      if(nb_grads_max > 4) {
	/* we could display at least 4 graduations but we did not find a proper interval... */
	/* To avoid to display only the min and max (see above), we add 2 newly created grads by interpolating between min and max */
	double pas = (grads_tmp[lastyindex]-grads_tmp[0])/3;
	for(i=0;i<3;i++){
	  ppsubwin->axes.ygrads[i] = grads_tmp[0] + i*pas;
	}
	
	ppsubwin->axes.ygrads[3] = grads_tmp[lastyindex]; /* exact max */
	ppsubwin->axes.nygrads = 4;
        /* Nb of subtics computation and storage */
        /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
	ppsubwin->axes.nbsubtics[1] = ComputeNbSubTicsFor3dUse(psubwin,
                                                               ppsubwin->axes.nygrads,
                                                               'n',
							       ppsubwin->axes.ygrads,
                                                               ppsubwin->axes.nbsubtics[1]);
      }
    }
  }
  
  return 0;
}


int AdaptGraduationsOnXBottomRight(int iof, int x, int y, int size, integer *Ticsdir, int *fontid, sciPointObj * psubwin, double xminval, double xmaxval, double fx, double fy, double fz)
{
  int i;
  int nbtics, nbsubtics;
  int lastxindex;
  int old_rect[4];
  int nb_grads_max = 0;
  int vx[2], vy[2];
  int xm, ym;
  double grads_tmp[20];
  char c_format[5];
  integer barlengthx = 0,barlengthy = 0;
  integer rect[4],posi[2]; 
  integer textcolor = -1;
  int logrect[4],XX,YY;
  int pas;
  double fact_h = 1.5, fact_w = 1.5;
  int compteur = 0;

  int possible_pas, possible_compteur;
  
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  for(i=0;i<4;i++) old_rect[i] = 0; /* Init. old_rect to force first grad. to be displayed */
  
  lastxindex = ppsubwin->axes.nxgrads - 1;
  
  if(lastxindex == 0)
    ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.xgrads[0]));
  else
    ChoixFormatE(c_format,
		 ppsubwin->axes.xgrads[0],
		 ppsubwin->axes.xgrads[lastxindex],
		 ((ppsubwin->axes.xgrads[lastxindex])-(ppsubwin->axes.xgrads[0]))/(lastxindex));
  
  nbtics = ppsubwin->axes.nxgrads;
  nbsubtics = ppsubwin->axes.nbsubtics[0];
  
  
  for(i=0;i<nbtics;i++) grads_tmp[i] = ppsubwin->axes.xgrads[i];

  for(i=0;i<nbtics;i++)
    {
      char foo[256]; 
      double xtmp = ppsubwin->axes.xgrads[i];
		  
/*       if(xtmp<xminval || xtmp>xmaxval)  */
/* 	{ */
/* 	  /\*   sciprint("je rejete la valeur: %lf\n\n",xtmp); *\/ */
/* 	  continue; /\* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon *\/ */
/* 	  /\* donc autant ne pas aller plus loin dans l'algo... *\/ */
/* 	} */
		  
      sprintf(foo,c_format,xtmp);

      /***************************************************************/
      /************************* COMMON PART *************************/
      /***************************************************************/
      if(ppsubwin->axes.reverse[0] == TRUE)
	xtmp = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp);
		  
      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz);
		  
      vx[0]=xm;vy[0]=ym; 
		  
      barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
      barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
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
		      
      
      /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
      C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
      
      
      if(CheckDisplay(fact_h, fact_w, ppsubwin->logflags[0],foo,posi,fontid,old_rect) == 0)
	continue; /*  graduation too close, DO NOT display the graduation ! */
      
      
      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if ( ppsubwin->logflags[0] == 'l' )
	{
	  int smallersize = fontid[1]-2;
	  int old_rect10[4];
	  int posi10[2];

	  posi10[0] = posi[0] - logrect[2];
	  posi10[1] = posi[1] + logrect[3];
	  
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  posi[0] = old_rect10[0] + old_rect10[2];
	  posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);
	  
	  C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  /* update old_rect */
	  old_rect[2] = (int)(fact_w*(old_rect[2] + old_rect10[2]));
	  old_rect[3] = (int)(fact_h*(old_rect[3] + old_rect10[3] + (int) (old_rect10[3]*.1)));
	  old_rect[0] = old_rect10[0];
	  old_rect[1] = old_rect[1];
	  
	  nb_grads_max++;
	  
	  /* put back the current fontid */
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      else{
	/* update old_rect */
	C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	old_rect[3] = (int)(fact_h* old_rect[3]);
	old_rect[2] = (int)(fact_w* old_rect[2]);
	nb_grads_max++;
      }
    }
  
  if(ppsubwin->logflags[0] != 'l')
    if(nb_grads_max == 1) /* only one grad. can be displayed : we choose to display the average value (max+min)/2 */
      {
	ppsubwin->axes.xgrads[0] = (grads_tmp[0]+grads_tmp[lastxindex])/2.;
	ppsubwin->axes.nxgrads = 1;
        if ( !ppsubwin->flagNax )
        {
          ppsubwin->axes.nbsubtics[0] = 1;
        }
	return 0;
      }
  
  
  pas = nbtics - 2; /* pas == grads number - 2 */
  
  possible_pas = -99;
  possible_compteur = -99;
  
  while(pas > 0)
    {
      int tmp = 0;
      compteur = 0;
      for(;;)
	{
	  tmp = tmp + pas;
	  compteur++;
	  
	  if((tmp == (nbtics - 1)) && (compteur < nb_grads_max)){
	    possible_pas = pas;
	    possible_compteur = ++compteur;
	    break;
	  }
	  
	  if(tmp > (nbtics - 1))
	    break;
	}
      
      pas--;
    }
  
  
  if(possible_compteur != -99){
    compteur = possible_compteur;
    pas = possible_pas;
    
    for(i=0;i<compteur;i++)
      ppsubwin->axes.xgrads[i] = grads_tmp[i*pas];
    
    ppsubwin->axes.nxgrads = compteur;
    
    /* Nb of subtics computation and storage */
    /* ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin,ppsubwin->axes.nxgrads,ppsubwin->logflags[0], */
/* 							   ppsubwin->axes.xgrads,Max((int) abs((13-compteur)/2),2)); */
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nxgrads,
                                                           ppsubwin->logflags[0],
 							   ppsubwin->axes.xgrads,
                                                           ppsubwin->axes.nbsubtics[0]);
  }
  else{
    ppsubwin->axes.xgrads[0] = grads_tmp[0];
    ppsubwin->axes.xgrads[1] = grads_tmp[lastxindex];
    ppsubwin->axes.nxgrads = 2;
    /* /\* Nb of subtics computation and storage *\/ */
/*     ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin,ppsubwin->axes.nxgrads,ppsubwin->logflags[0], */
/* 							   ppsubwin->axes.xgrads,Max((int) abs((13-compteur)/2),2));  */
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nxgrads,
                                                           ppsubwin->logflags[0],
 							   ppsubwin->axes.xgrads,
                                                           ppsubwin->axes.nbsubtics[0]);
    if(ppsubwin->logflags[0] == 'n'){
      if(nb_grads_max > 4) {
	/* we could display at least 4 graduations but we did not find a proper interval... */
	/* To avoid to display only the min and max (see above), we add 2 newly created grads by interpolating between min and max */
	double pas = (grads_tmp[lastxindex]-grads_tmp[0])/3;
	for(i=0;i<3;i++){
	  ppsubwin->axes.xgrads[i] = grads_tmp[0] + i*pas;
	}
	
	ppsubwin->axes.xgrads[3] = grads_tmp[lastxindex]; /* exact max */
	ppsubwin->axes.nxgrads = 4;
        /* Nb of subtics computation and storage */
	/* ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin, */
/*                                                                ppsubwin->axes.nxgrads, */
/*                                                                'n', */
/* 							       ppsubwin->axes.xgrads, */
/*                                                                Max((int) abs((13-compteur)/2),2) ); */
        ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin,
                                                               ppsubwin->axes.nxgrads,
                                                               'n',
							       ppsubwin->axes.xgrads,
                                                               ppsubwin->axes.nbsubtics[0] );
      }
    }
  }
  
  return 0;
}

int AdaptGraduationsOnZ(int x, int y, int size, integer *Ticsdir, int *fontid, sciPointObj * psubwin, double zminval, double zmaxval, double fx, double fy, double fz)
{
  int i;
  int nbtics, nbsubtics;
  int lastzindex;
  int old_rect[4];
  int nb_grads_max = 0;
  int vx[2], vy[2];
  int xm, ym;
  double grads_tmp[20];
  char c_format[5];
  integer barlengthx = 0,barlengthy = 0;
  integer rect[4],posi[2]; 
  integer textcolor = -1;
  int logrect[4],XX,YY;
  int pas;
  double fact_h = 1.5, fact_w = 1.;
  int compteur;

  int possible_pas, possible_compteur;
  
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  for(i=0;i<4;i++) old_rect[i] = 0; /* Init. old_rect to force first grad. to be displayed */
  


  lastzindex = ppsubwin->axes.nzgrads - 1;
  
  if(lastzindex == 0)
    ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.zgrads[0]));
  else
    ChoixFormatE(c_format,
		 ppsubwin->axes.zgrads[0],
		 ppsubwin->axes.zgrads[lastzindex],
		 ((ppsubwin->axes.zgrads[lastzindex])-(ppsubwin->axes.zgrads[0]))/(lastzindex));
  
  nbtics = ppsubwin->axes.nzgrads;
  nbsubtics = ppsubwin->axes.nbsubtics[2];


  for(i=0;i<nbtics;i++) grads_tmp[i] = ppsubwin->axes.zgrads[i];
  
  for(i=0;i<nbtics;i++)
    {
      char foo[256];
      double ztmp = ppsubwin->axes.zgrads[i];
      
/*       if(ztmp<zminval || ztmp>zmaxval) */
/* 	{ */
/* 	  /\*   sciprint("je rejete la valeur: %lf\n\n",xtmp); *\/ */
/* 	  continue; /\* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon *\/ */
/* 	  /\* donc autant ne pas aller plus loin dans l'algo... *\/ */
/* 	} */
      
      /*       sprintf(foo,c_format,ztmp); */

      sprintf(foo,c_format,ztmp);
      
      /***************************************************************/
      /************************* COMMON PART *************************/
      /***************************************************************/
      if(ppsubwin->axes.reverse[2] == TRUE)
	ztmp = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp);
      
      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&ztmp);
            
      
      vx[0]=xm;vy[0]=ym;
      
      barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
      barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
      vx[1]=vx[0]+barlengthx;
      vy[1]=vy[0]+barlengthy;
      
      /* foo is set above with sprintf(foo,c_format,xtmp); */
  	      
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
      C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      posi[0] = inint( xm+2*barlengthx - rect[2]); 
      posi[1]=inint( ym + 2*barlengthy + rect[3]/2);


      /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
      C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
      
      
      if(CheckDisplay(fact_h, fact_w, ppsubwin->logflags[2],foo,posi,fontid,old_rect) == 0)
	continue; /*  graduation too close, DO NOT display the graduation ! */
	      
      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if ( ppsubwin->logflags[2] == 'l' )
	{
	  int smallersize = fontid[1]-2;
	  int old_rect10[4];
	  int posi10[2];
	  
	  posi10[0] = posi[0] - logrect[2];
	  posi10[1] = posi[1] + logrect[3];
	  
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  posi[0] = old_rect10[0] + old_rect10[2];
	  posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);
	  
	  C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  /* update old_rect */
	  old_rect[2] = (int)(fact_w*(old_rect[2] + old_rect10[2]));
	  old_rect[3] = (int)(fact_h*(old_rect[3] + old_rect10[3] + (int) (old_rect10[3]*.1)));
	  old_rect[0] = old_rect10[0];
	  old_rect[1] = old_rect[1];
	  
	  nb_grads_max++;
	  
	  
	  /* put back the current fontid */
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      else{
	/* update old_rect */
	C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	old_rect[3] = (int)(fact_h* old_rect[3]);
	old_rect[2] = (int)(fact_w* old_rect[2]);
	nb_grads_max++;
      }
    }
  
  
  if(ppsubwin->logflags[2] != 'l')
    if(nb_grads_max == 1) /* only one grad. can be displayed : we choose to display the average value (max+min)/2 */
      {
	ppsubwin->axes.zgrads[0] = (grads_tmp[0]+grads_tmp[lastzindex])/2.;
	ppsubwin->axes.nzgrads = 1;
        if ( !ppsubwin->flagNax )
        {
          ppsubwin->axes.nbsubtics[2] = 1;
        }
	return 0;
      }
  
  pas = nbtics - 2; /* pas == grads number - 2 */

  possible_pas = -99;
  possible_compteur = -99;

  while(pas > 0)
    {
      int tmp = 0;
      compteur = 0;
      for(;;)
	{
	  tmp = tmp + pas;
	  compteur++;
	  
	  if((tmp == (nbtics - 1)) && (compteur < nb_grads_max)){
	    possible_pas = pas;
	    possible_compteur = ++compteur;
	    break;
	  }
	  
	  if(tmp > (nbtics - 1))
	    break;
	}
      
      pas--;
    }
  
  if(possible_compteur != -99){
    compteur = possible_compteur;
    pas = possible_pas;
    
    for(i=0;i<compteur;i++)
      ppsubwin->axes.zgrads[i] = grads_tmp[i*pas];
    
    ppsubwin->axes.nzgrads = compteur;
    
    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[2] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nzgrads,
                                                           ppsubwin->logflags[2],
							   ppsubwin->axes.zgrads,
                                                           ppsubwin->axes.nbsubtics[2]); 
  }
  else{
    ppsubwin->axes.zgrads[0] = grads_tmp[0];
    ppsubwin->axes.zgrads[1] = grads_tmp[lastzindex];
    ppsubwin->axes.nzgrads = 2;
    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[2] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nzgrads,
                                                           ppsubwin->logflags[2],
							   ppsubwin->axes.zgrads,
                                                           ppsubwin->axes.nbsubtics[2]); 
  }
  
  return 0;
}



int ComputeNbSubTicsFor3dUse(sciPointObj * pobj, int nbtics, char logflag, double * grads, int nbsubtics_input)
{
  int ticsval[] =    {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
  int subticsval[] = {6,4,4,3,3,3,2,2,2 ,2 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1};
  int i;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);


  if(logflag =='l'){
    if( (grads[1]-grads[0]) == 1 ) /* intervalle de type ...10^n 10^(n+1)...*/
      {
	return 9; /* 9 subtics to have a pretty tics/grid in log.*/
      }
    else
      {
	return 1; /* no subtics at all (1 but draw on a tics place) */
      }
  }
  else
  {
    if(ppsubwin->flagNax == FALSE) /* if auto subtics mode == ON */
    { 
      for(i=0;i<19;i++)
      {
        if(nbtics == ticsval[i])
        {
          return subticsval[i];
        }
      }
    }
    else /* if auto subtics mode == OFF already computed in Plo2dn.c, Champ.c routines... */
    {  /* or given via a.subtics=[nbsubtics_on_x, nbsubtics_on_y, nbsubtics_on_z] command */
      return nbsubtics_input;
    }
  }
  
  return -1;
}


int ChooseFormatForOneGrad(char *c_format, double *grad)
{
  int compteur = 0;
  double epsilon = 1e-16;
  
  if((double)(abs((int)*grad)) < epsilon){ /* case where grad is strictly equal to 0. */
    *grad = 0.;
    strcpy(c_format,"%d");
    return 0;
  }
  
  while((int) (*grad) == 0){
    compteur++;
    *grad = 10*(*grad);
    
    if(compteur > 100){
      Scierror(999,"Error in function ChooseFormatForOneGrad\n");
      return -1;
    }
  }
  
  sprintf(c_format,"%%.%df",compteur);
  
  return 0;
}


/* Vertices * Tete = NULL; */

int Store3DPixelValues(sciPointObj * pobj, int xm, int ym, double x, double y, double z)
{

  sciPointObj * psubwin = sciGetParentSubwin(pobj);
  sciSubWindow *ppsubwin = pSUBWIN_FEATURE(psubwin);

  if(GlobalFlag_Zoom3dOn == 1){
    Vertices * pCurrent = ppsubwin->vertices_list;
    
    if (pCurrent != NULL) {
      
      pCurrent = pHead;
      
      if(( pCurrent->pNext = (Vertices*) MALLOC(sizeof(Vertices))) == NULL){
	printf("Allocation failed for vertices when zoom called\n");
	return -1;
      }
      
      /*       pCurrent->pNext = NewVertices; */
      
      pCurrent->pNext->pNext = NULL;
      
      pCurrent->pNext->value_xm = xm;
      pCurrent->pNext->value_ym = ym;
      pCurrent->pNext->value_x = x;
      pCurrent->pNext->value_y = y;
      pCurrent->pNext->value_z = z;

      pHead = pCurrent->pNext;

    }
    else
      { /* first element is created */
	if((ppsubwin->vertices_list = (Vertices*) MALLOC(sizeof(Vertices))) == NULL){
	  printf("Allocation failed for vertices when zoom called\n");
	  return -1;
	}
	
	ppsubwin->vertices_list->value_xm = xm;
	ppsubwin->vertices_list->value_ym = ym;
	ppsubwin->vertices_list->value_x = x;
	ppsubwin->vertices_list->value_y = y;
	ppsubwin->vertices_list->value_z = z;
	
	ppsubwin->vertices_list->pNext = NULL;

	pHead = ppsubwin->vertices_list;

      }
    index_vertex++;
  }
  
  return 0;
}


int SetMinMaxVertices(Vertices *vertices_list, double *xmin, double *ymin, double *zmin,double *xmax, double *ymax, double *zmax)
{ 
  Vertices * pCurrent = vertices_list;

  *xmin = *xmax = vertices_list->value_x;
  *ymin = *ymax = vertices_list->value_y;
  *zmin = *zmax = vertices_list->value_z;
  
  while (pCurrent != NULL) {
    if(pCurrent->value_x >  *xmin) *xmin = pCurrent->value_x;
    if(pCurrent->value_y >  *ymin) *ymin = pCurrent->value_y;
    if(pCurrent->value_z >  *zmin) *zmin = pCurrent->value_z;
    
    if(pCurrent->value_x <  *xmax) *xmax = pCurrent->value_x;
    if(pCurrent->value_y <  *ymax) *ymax = pCurrent->value_y;
    if(pCurrent->value_z <  *zmax) *zmax = pCurrent->value_z;
    
    pCurrent = pCurrent->pNext;
  }
  
  return 0;
}

int GetVerticesAt(Vertices *vertices_list, int *xm, int *ym, double *x, double *y, double *z)
{
  Vertices * pCurrent = vertices_list;
/*   int i; */

 /*  for(i=0;i<index;i++){ */
/*     pCurrent = pCurrent->pNext; */
/*     if(pCurrent == NULL){ */
/*       /\*       printf("pCurrent est nul et i vaut: %d\n",i); *\/ */
/*       return -1; */
/*     } */
/*   } */
  
  if(pHead2 == (Vertices *) NULL)
    pHead2 = pCurrent;
  
  *xm = pHead2->value_xm;
  *ym = pHead2->value_ym;
  *x  = pHead2->value_x;
  *y  = pHead2->value_y;
  *z  = pHead2->value_z;
  
  pHead2 = pHead2->pNext;
  
  return 0;
}     


int RemoveNext(Vertices *pCurrent)
{
  Vertices * DeletedElement = pCurrent->pNext;
  Vertices * pNextNext =  pCurrent->pNext->pNext;

  FREE(DeletedElement); DeletedElement = (Vertices *) NULL;
  
  pCurrent->pNext = pNextNext;

  return 0;
}

int FreeVertices(sciPointObj * psubwin)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  Vertices * pCurrent = ppsubwin->vertices_list;
  
  if (pCurrent != NULL) {
    
    /*pCurrent = Tete; */
    while (pCurrent->pNext != NULL){
      RemoveNext(pCurrent);
    }
    FREE(ppsubwin->vertices_list); ppsubwin->vertices_list = (Vertices *) NULL;
  }
  
  pHead = (Vertices *) NULL;
  pHead2 = (Vertices *) NULL;

  return 0;
}

/*------------------------------------------------------------------------------------------*/
void rectangleDouble2Pixel( sciPointObj * parentSubWin ,
                            double        ulPoint[3]   ,
                            double        userSize[2]  ,
                            int           edgesX[4]    ,
                            int           edgesY[4]     )
{
  sciSubWindow * ppSubWin = pSUBWIN_FEATURE( parentSubWin  ) ;

  if ( ppSubWin->is3d )
  {
    double rectx[4],recty[4],rectz[4];
    int n = 4 ;
    rectx[0] = rectx[3] = ulPoint[0] ;
    rectx[1] = rectx[2] = ulPoint[0] + userSize[0] ;   
    recty[0] = recty[1] = ulPoint[1] ;
    recty[2] = recty[3] = ulPoint[1] - userSize[1];
    rectz[0] = rectz[1] = rectz[2]= rectz[3] = ulPoint[2] ;
    
    ReverseDataFor3D( parentSubWin, rectx, recty, rectz, n);
    
    trans3d( parentSubWin,n,edgesX,edgesY,rectx,recty,rectz) ;
  }
  else
  {
    /* 2D mode */
    /* position of the upper left point of the rectangle with reverse axes. */
    double realPoint[2] ;
    int ulPointPix[2] ;
    int sizePix[2] ;
    
    if ( ppSubWin->axes.reverse[0] )
    {
      realPoint[0] = ulPoint[0] + userSize[0] ;
    }
    else
    {
      realPoint[0] = ulPoint[0] ;
    }
    
    if ( ppSubWin->axes.reverse[1] )
    {
      realPoint[1] = ulPoint[1] - userSize[1] ;
    }
    else
    {
      realPoint[1] = ulPoint[1] ;
    }
	  
    ulPointPix[0] = XDouble2Pixel( realPoint[0] ) ; 
    ulPointPix[1] = YDouble2Pixel( realPoint[1] ) ;
	
    /* Nouvelles fonctions de changement d'echelle pour les longueurs --> voir PloEch.h */
    sizePix[0] = WDouble2Pixel( ulPoint[0], userSize[0] ) ;
    /* for y we take the length from the bottom left corner */
    sizePix[1] = HDouble2Pixel( ulPoint[1] - userSize[1], userSize[1] ) ;

    edgesX[0] = ulPointPix[0] ;
    edgesX[1] = ulPointPix[0] + sizePix[0]  ;
    edgesX[2] = ulPointPix[0] + sizePix[0]  ;
    edgesX[3] = ulPointPix[0] ;

    edgesY[0] = ulPointPix[1] ;
    edgesY[1] = ulPointPix[1] ;
    edgesY[2] = ulPointPix[1] + sizePix[1]  ;
    edgesY[3] = ulPointPix[1] + sizePix[1]  ;

  }
}
/*------------------------------------------------------------------------------------------*/
/**
 * draw the marks at the vertices of a polyline.
 * @param[in] pObj Object on which the marks will be drawn. Might be a polyline, a segs or
 *                 rectangle object.
 * @param[in] nbMarks Number of marks to draw.
 * @param[in] xCoord X coordinates of the marks in pixels.
 * @param[in] yCoord Y coordinates of the marks in pixels.
 * @param     DPI    ???
 */
void drawPolyMarks( sciPointObj * pObj, int nbMarks, int xCoord[], int yCoord[], int * DPI )
{
  int foreground = sciGetMarkForeground( pObj ) ;
  int v     = 0   ;
  double dv = 0.0 ;
  int markIdSize[2] ;

  markIdSize[0] = sciGetMarkStyle( pObj ) ;
  markIdSize[1] = sciGetMarkSize(  pObj ) ;
  
  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
            &dv, &dv, &dv, 5L, 4096);     
  C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
            &dv, &dv, &dv, &dv, 5L, 4096);
  
  C2F (dr) ("xset", "mark", &markIdSize[0], &markIdSize[1], PI0, PI0, PI0, PI0, PD0, PD0,
            PD0, PD0, 0L, 0L);

  DrawNewMarks( pObj, nbMarks, xCoord, yCoord, DPI ) ;
}
/*------------------------------------------------------------------------------------------*/
/**
 * Compute the arrow size which must be used in (dr)(xarrows).
 * @param[in] pSegs object of which we are displaying arrows.
 * @param[in] X coordinates in pixels of the segments
 * @param[in] Y coordinates in pixels of the segments
 * @return    Size to use in xarrows.
 */
int computeRealArrowSize( sciPointObj * pSegs, int nbSegs, int xCoord[], int yCoord[] )
{
  sciSegs * ppSegs = pSEGS_FEATURE( pSegs ) ;
  double arrowSize = ppSegs->arrowsize ;
  int one = 1 ;

  if ( ppSegs->ptype == 0 )
  {
    if ( arrowSize > 0 )
    {
      int iAs2 = 0 ;
      int iAs  = 0 ;
      C2F(echelle2dl)( &arrowSize, &arrowSize, &iAs, &iAs2, &one, &one, "f2i" ) ;
      return iAs / 2 ;
    }
    else
    {
      /* taken from Xcall1.c */
      int i ;
      double length = 0.0 ;
      for ( i = 0 ; i < nbSegs / 2 ; i++ )
      {
        double dx ;
        double dy ;
        dx=( xCoord[2*i+1] - xCoord[2*i] ) ;
        dy=( yCoord[2*i+1] - yCoord[2*i] ) ;
        length += sqrt( dx * dx + dy * dy ) ;
      }
      if ( nbSegs != 0 )
      {
        length /= nbSegs / 2 ;
      }
      return round( -arrowSize * length ) ;
    }
  }
  else
  {
    if ( ppSegs->arrowsize >= 0 )
    {
      double arsize1 = ((double) Cscale.WIRect1[2])/(5*( ppSegs->Nbr1));
      double arsize2 = ((double) Cscale.WIRect1[3])/(5*( ppSegs->Nbr2));
      double arsize  =  (arsize1 < arsize2) ? inint(arsize1*10.0) : inint(arsize2*10.0) ;
      return (int)((arsize)*(ppSegs->arrowsize));
    }
  }

  return 0 ;

}
/*------------------------------------------------------------------------------------------*/
/* update the Csacle value from new viewing angles */
void sciUpdateScaleAngles( double theta, double alpha )
{
  double cost = 0.5 ;
  double sint = 0.5 ;
  double cosa = 0.5 ;
  double sina = 0.5 ;

  cost = cos( DEG2RAD(theta) ) ;
  cosa = cos( DEG2RAD(alpha) ) ;
  sint = sin( DEG2RAD(theta) ) ;
  sina = sin( DEG2RAD(alpha) ) ;
  
  Cscale.m[0][0]= -sint        ;
  Cscale.m[0][1]=  cost        ;
  Cscale.m[0][2]=  0           ;
  Cscale.m[1][0]= -cost * cosa ;
  Cscale.m[1][1]= -sint * cosa ;
  Cscale.m[1][2]=  sina        ;
  Cscale.m[2][0]=  cost * sina ;
  Cscale.m[2][1]=  sint * sina ;
  Cscale.m[2][2]=  cosa        ;

}
/*------------------------------------------------------------------------------------------*/

#undef round
