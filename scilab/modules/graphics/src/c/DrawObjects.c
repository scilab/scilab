/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE,   INRIA 2001-2002
 *    Djalel ABDEMOUCHE,   INRIA 2002-2004
 *    Fabrice Leray,       INRIA 2004-2006
 *    Jean-Baptiste Silvy, INRIA 2006-xxxx
 *    Comment:
 *    This file contains all functions used to Draw the content of a window.
 *    The main functions is sciDrawObj that draws the objects recursively.
 ------------------------------------------------------------------------/-*/
#include <math.h>

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "Graphics.h" 
#include "DrawObjects.h"
#include "GetProperty.h"
#include "bcg.h"
#include "SetProperty.h"
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "ObjectStructure.h"
#include "Axes.h"
#include "Xcall1.h"

#include "math_graphics.h"

#include "MALLOC.h" /* MALLOC */

#ifdef WITH_TK
#include "../../../tclsci/includes/GedManagement.h"
#endif

#if _MSC_VER
extern HDC TryToGetDC(HWND hWnd);
#endif

#define		round(a)	(int)(((a)<0.0)?(a)-.5:(a)+.5)

extern int GetScreenDPI(int *ixres, int *iyres);
int WindowsPrintScreen = 0;
int Printer_XRes = 0;
int Printer_YRes = 0;

extern double C2F(dsort)();/*DJ.A merge*/ 
extern int scilab_shade(integer *polyx, integer *polyy, integer *fill, integer polysize, integer flag);
extern void xstringb_angle (char *string, integer x, integer y, integer w, integer h, double angle);
extern void xstringb_bbox (char *string, integer x, integer y, integer w, integer h, double angle, int *bbox);
extern int GlobalFlag_Zoom3dOn;

extern int index_vertex;
int Store3DPixelValues(sciPointObj * pobj, int xm, int ym, double x, double y, double z);
static Vertices * pHead = (Vertices *) NULL;
static Vertices * pHead2 = (Vertices *) NULL;

int xinitxend_flag = 0 ;

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


#ifdef _MSC_VER
static int flag_DO; /* F.Leray 16.02.04 flag global pour la fonction recursive DrawObj*/
#endif


/**sciGetDC
 * Returns the HDC of the figure window (not the parent)
 */
#ifdef _MSC_VER
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


#ifdef WITH_TK
            /* close ged to prevent errors when using it */
            sciDestroyGed( sciGetNum( mafigure ) ) ;
#endif

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


/* update the scale and retrieve the bounding box of the axis */
void updateScale3d( sciPointObj * pobj    ,
                    double        dbox[6] ,
                    double        xbox[6] ,
                    double        ybox[6] ,
                    double        zbox[6]  )
{
  int    flag     ;
  int    ib       ;
  double FRect[4] ;
  double WRect[4] ; 
  double ARect[4] ;
  double xmmin    ;
  double xmmax    ;
  double ymmin    ;
  double ymmax    ;
  double wmax     ;
  double hmax     ;
  char   logf[2]  ;
  integer aaint[] = {2,10,2,10} ;
  sciSubWindow * ppsubwin =  pSUBWIN_FEATURE (pobj);

  sci_update_frame_bounds_3d( pobj ) ;
       
  dbox[0] =  ppsubwin->FRect[0] ; /*xmin*/
  dbox[1] =  ppsubwin->FRect[2] ; /*xmax*/
  dbox[2] =  ppsubwin->FRect[1] ; /*ymin*/
  dbox[3] =  ppsubwin->FRect[3] ; /*ymax*/
  dbox[4] =  ppsubwin->FRect[4] ; /*zmin*/
  dbox[5] =  ppsubwin->FRect[5] ; /*zmax*/

  if( ppsubwin->cube_scaling )
  {
    dbox[0] =  0.; 
    dbox[1] =  1.;
    dbox[2] =  0.;
    dbox[3] =  1.;
    dbox[4] =  0.;
    dbox[5] =  1.;
  }


  /** changement de coordonnees 3d */
  flag = ( ppsubwin->axes.flag[1] + 1 ) / 2 ; /* F.Leray Adding HERE 19.04.04 */

  Cscale.alpha = ppsubwin->alpha ;
  Cscale.theta = ppsubwin->theta ;
      
  /* update Cscale.m from the new viewing angles */
  sciUpdateScaleAngles( ppsubwin->theta, ppsubwin->alpha ) ;
      
  for ( ib = 0 ; ib < 6 ; ib++ ) 
  { 
    if ( flag == 0 )
    { 
      dbox[ib] = Cscale.bbox1[ib];
    }
    else
    { 
      Cscale.bbox1[ib] = dbox[ib];
    }
  }

  /* get the vertices of the axes box */
  sciGetAxisBox( dbox, xbox, ybox, zbox ) ;
      
  /** Calcul des echelles en fonction de la taille du dessin **/
  if ( flag == 1 || flag == 3 ) /* ALL the expanded cases : flag[1] = 1 or 2 or 5 or 6 */
  {
    xmmin=  (double) Mini(xbox,8L);
    xmmax= (double) Maxi(xbox,8L); /* search for x Min/Max on all the edges (there are 8 edges that compose the box) F.Leray 13.10.04 */
    ymmax=  (double) - Mini(ybox,8L); /* same thing on ybox vector ( 1) why - (minus) ? see 2) )*/
    ymmin=  (double) - Maxi(ybox,8L);
  }
  if ( flag == 2 || flag == 3 ) /* ALL the isometric cases : flag[1] = 3 or 4 or 5 or 6 */
  {
    int verbose = 0 ;
    int narg        ;
    int wdim[2]     ;
    /* get current window size */
    C2F(dr)("xget","wdim",&verbose,wdim,&narg, PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    getscale2d(WRect,FRect,logf,ARect);
    wmax=linint((double)wdim[0] * WRect[2]);
    hmax=linint((double)wdim[1] * WRect[3]); 
  }
  if ( flag == 2 ) /* the "NON expanded isometric" cases : flag[1] = 3 or 4 */
  {
    double R  ;
    double xo ;
    double yo ;
    double zo ;
    double dx ;
    double dy ;
    double dz ;
    
    /* radius and center of the sphere circumscribing the box */
    dx = dbox[1]-dbox[0];
    dy = dbox[3]-dbox[2];
    dz = dbox[5]-dbox[4];
    
    R  = (double) sqrt(dx*dx + dy*dy + dz*dz)/2;
    xo = (double) (xbox[0]+xbox[6])/2 ;
    yo = (double) (ybox[0]+ybox[6])/2 ;
    zo = (double) (zbox[0]+zbox[6])/2 ;
    
    xmmin=  (double)  xo - R ;
    xmmax=  (double)  xo + R ;
    ymmax=  (double) -yo + R ;
    ymmin=  (double) -yo - R ;
  }
  if (flag==2 || flag==3)
  {
    double hx ;
    double hy  ;
    hx=xmmax-xmmin;
    hy=ymmax-ymmin;
    if ( hx/(double)wmax  < hy/(double)hmax ) 
    {
      double hx1 ;
      hx1=wmax*hy/hmax;
      xmmin=xmmin-(hx1-hx)/2.0;
      xmmax=xmmax+(hx1-hx)/2.0;
    }
    else 
    {
      double hy1 ;
      hy1=hmax*hx/wmax;
      ymmin=ymmin-(hy1-hy)/2.0;
      ymmax=ymmax+(hy1-hy)/2.0;
    }
  }
  if (flag != 0 ) /* != using current 3D scale */
  {
    /* FRect = [Xmin,Ymin,Xmax,Ymax] */
    FRect[0] =  xmmin ;
    FRect[1] = -ymmax ;
    FRect[2] =  xmmax ;
    FRect[3] = -ymmin ; /* 2) ... (why - (minus) ? )*/
    set_scale("tftttf",NULL,FRect,aaint,"nn",NULL);
    Cscale.metric3d=flag; 
  }
}

/**axis_3ddraw 10/2003
 * @author Djalel Abdemouche
 * Should be in Axes.c file
 */
void axis_3ddraw(sciPointObj *pobj, double *xbox, double *ybox, double *zbox, integer *InsideU, integer *InsideD) 
{
  double dbox[6];
  integer flag,i,p,n,pat,hiddencolor, x[5]; /* F. Leray : redimmensionnment (+1) du tableau x[4];*/
  static double Alpha, Teta;
  int verbose=0,narg;
  integer ixbox[8],iybox[8],xind[8],dash[6];
  integer background,zero=0, color_old; /* Adding color_old 04.03.04*/
  sciSubWindow * ppsubwin =  pSUBWIN_FEATURE (pobj);
  EAxesBoxType subWinBoxType = sciGetBoxType( pobj ) ;

  /* Initialisation phase for x (to detect bug): x set to -1000 F.Leray 05.03.04*/
  for(i=0;i<5;i++) x[i] = -1000;

  if(sciGetEntityType (pobj) == SCI_SUBWIN)
    {  
      
      updateScale3d( pobj, dbox, xbox, ybox, zbox ) ;
      /* Until here we have computed + reset the 3d scale*/
      Teta  = ppsubwin->theta  ;
      Alpha =  ppsubwin->alpha ;
      if( Abs( Teta ) < 0.1 )
      {
	/* to avoid bug at limit when theta == 0 */
	/* I recompute temp value xyzbox with theta == 0.1 */
	/* to have a correct xind, InsideU et InsideD */
	ComputeCorrectXindAndInsideUD( Teta, Alpha, dbox, xind, InsideU, InsideD ) ;
      }
      else
      {
        sciAxesVerticesIndices( InsideU, InsideD, xbox, ybox, xind ) ;
      }

      /* BUG Event A ESSAYER SOUS WINDOWS !! */
      /* sous X11, un event X11 declenche par sciprint -> puis scig_resize fait que l'on */
      /* lance axis_3ddraw 2 fois de suite SANS QUE le premier est le temps de */
      /* terminer completement !!*/
      /* pour le voir decommenter la ligne ci-dessous et les lignes de printf : */
      /* DEBUT DE axis_3ddraw */
      /* DEBUT DE Axes3dStrings2 */
	    
      /* 	    for(i=0;i<50000;i++) sciprint("i= %d \n", i); */
	    

      /* 	    printf("INFO HERE \n"); */
      /* 	    sciprint("alpha = %lf \t theta = %lf",Alpha,Teta); */
      /* 	    sciprint("xind vaut:\n"); */
      /* 	    for(i=0;i<8;i++) sciprint("xind[%d] = %d\n",i,xind[i]); */
      /* 	    sciprint("InsideD & InsideU vallent:\n"); */
      /* 	    for(i=0;i<4;i++) sciprint("InsideD[%d] = %d \t InsideU[%d]=%d \n",i,InsideD[i],i,InsideU[i]); */
      /* 	    sciprint("fin \n\n"); */

      /* F.Leray 26.10.04 Debug*/
      /* Same thing as above but using printf to avoid sciprint */
      /*   printf("INFO HERE \n"); */
      /*       printf("alpha = %lf \t theta = %lf",Alpha,Teta); */
      /*       printf("xind vaut:\n"); */
      /*       for(i=0;i<8;i++) printf("xind[%d] = %d\n",i,xind[i]); */
      /*       printf("InsideD & InsideU vallent:\n"); */
      /*       for(i=0;i<4;i++) printf("InsideD[%d] = %d \t InsideU[%d]=%d \n",i,InsideD[i],i,InsideU[i]); */
      /*       printf("fin \n\n"); */



      /* F.Leray Rajout 02.04.04 :*/
      background=sciGetBackground(pobj);
	  
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

      flag = ppsubwin->axes.flag[2]; /* box drawing */
      
      if ( subWinBoxType != BT_OFF )
	{ 
	  x[2] = sciGetLineWidth (pobj);
          x[3] = sciGetLineStyle (pobj);
	  C2F (dr) ("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          
	  hiddencolor = sciSetGoodIndex( pobj, ppsubwin->axes.hiddenAxisColor ) ;
	  
	  if (zbox[InsideU[0]] > zbox[InsideD[0]])
	    DrawAxis(xbox,ybox,InsideD,hiddencolor);
	  else
	    {
	      DrawAxis(xbox,ybox,InsideU,hiddencolor); 	
	    }
	  if (Ishidden(pobj))
	    ppsubwin->hiddenstate=(InsideU[0] % 4);
	  else
	    ppsubwin->hiddenstate=(InsideD[0] % 4);
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

      p = 7 ;
      n = 1 ;
      for ( i = 0 ; i < p ; i++ )
      {
        ixbox[i] = XScale( xbox[ xind[i] ] ) ;
        iybox[i] = YScale( ybox[ xind[i] ] ) ;
      }
      ixbox[p-1]=ixbox[0];iybox[p-1]=iybox[0]; 

      if ( subWinBoxType == BT_BACK_HALF || subWinBoxType == BT_ON )
      {
        C2F(dr)("xpolys","v",ixbox,iybox,x,&n,&p,PI0,PD0,PD0,PD0,PD0,0L,0L);
        
      }
      Axes3dStrings2(ixbox,iybox,xind) ;
      
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
  {
    if(pSUBWIN_FEATURE (pobj)->axes.rect == BT_ON)
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
      {
        DrawAxis(xbox,ybox,InsideU,x[0]);
      }
      else
      {
        DrawAxis(xbox,ybox,InsideD,x[0]);
      }
    }
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


/**Axes3dStrings2
 * @author F.Leray 18.10.04
 * Should be in Axes.c file
 */
int Axes3dStrings2(integer *ixbox, integer *iybox, integer *xind)
{
  integer verbose=0,narg,xz[2],fontid[2],fontsize_kp,color_kp,size;
  integer iof,barlengthx = 0,barlengthy = 0, posi[2]; 
  integer rect[4],flag=0,x=0,y=0;
  double ang=0.0, bbox[6];
  int fontsize=-1,textcolor=-1,ticscolor=-1;
  int fontstyle=0; /* F.Leray 08.04.04 */
  sciPointObj *psubwin = NULL;
  sciSubWindow * ppsubwin = NULL;
  int ns=2,iflag=0,gstyle,gridStyle=2,dash[6];
  double xx[4],yy[4],zz[4],vxx1,vyy1,vzz1;
  integer i,xm,ym,vx[2],vy[2],xg[2],yg[2],j;
  integer fontid_old[2], textcolor_old;

  int lastzindex = 0, lastxindex = 0, lastyindex = 0;
  double xminval, yminval, zminval, xmaxval, ymaxval, zmaxval;
  int nbtics = 0;
  int nbsubtics = 0;

  int logrect[4], XX = 0, YY = 0; /* see below */ /* ah ouais ou ca ? */
  double angle=0.0;
  
  int constOffset[2] ; /* displacment of labels from the axes segments. */
  int maxTicksLabelSize[2] ; /* for each axis the maximum size of ticks label */


  psubwin= (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());
  ppsubwin = pSUBWIN_FEATURE (psubwin);

  /** le cot\'e gauche ( c'est tjrs un axe des Z **/
  xz[0] = Cscale.WIRect1[2] ;
  xz[1] = Cscale.WIRect1[2] ;
  iof = (xz[0]+xz[1])/50;

  ticscolor = sciGetForeground(psubwin);
  textcolor=sciGetFontForeground(psubwin);

  fontsize=sciGetFontDeciWidth(psubwin)/100;
  fontstyle=sciGetFontStyle(psubwin);
  
  if(sciGetEntityType (psubwin) != SCI_SUBWIN) { 
    sciprint("Impossible case\n");
    return 0;
  }
  

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
  rect[0]= Cscale.WIRect1[0] ;
  rect[1]= Cscale.WIRect1[1] ;
  rect[2]= Cscale.WIRect1[2] ;
  rect[3]= Cscale.WIRect1[3]/6 ;
  textcolor_old = textcolor;
  fontid_old[0] = fontid[0];
  fontid_old[1] = fontid[1];

  constOffset[0] = Cscale.WIRect1[2] / 50 + 1 ;
  constOffset[1] = Cscale.WIRect1[3] / 25 + 1 ;
  
  if( sciGetVisibility(ppsubwin->mon_title) )
  {
    /* get the pointer on the title */
    sciLabel * ppLabel = pLABEL_FEATURE( ppsubwin->mon_title ) ;
    
    /* get position and orientation of the title */
    if ( ppLabel->auto_rotation )
    {
      sciSetFontOrientation( ppsubwin->mon_title, 0 ) ;
    }
    
    if ( ppLabel->auto_position )
    {
      /* same as in 2d */
      int segmentStart[2] = { rect[0] + rect[2], rect[1] } ;
      int segmentEnd[2]   = { rect[0]          , rect[1] } ;
      computeLabelAutoPos( ppsubwin->mon_title, segmentStart, segmentEnd, constOffset ) ;
    }
    /* draw the label */
    drawText( ppLabel->text ) ;
  }
  
  textcolor = textcolor_old;
  fontid[0] = fontid_old[0];
  fontid[1] = fontid_old[1];

  size = xz[0]>=xz[1] ? (integer) (xz[1]/50.0) : (integer) (xz[0]/50.0); 
  
  /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
  C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	

  /********************************************/
  /* draw the axis line if axes_visible is on */
  /********************************************/
  {
    int x[5] ;
    int two = 2 ;
    int one = 1 ;
    int verbose = 0 ;
    int dash[6] ;
    int pat ;
    int narg ;
    x[0] = sciGetForeground (psubwin);
    x[2] = sciGetLineWidth (psubwin) ;
    x[3] = sciGetLineStyle (psubwin);
    x[4] = 0;
    C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
    C2F(dr)("xget","pattern",&verbose,&pat,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4,PI0,PD0,PD0,PD0,PD0, 5L, 6L);
    C2F (dr) ("xset","foreground",x,x,x+4,x+4,x+4,PI0,PD0,PD0,PD0,PD0,5L,4096); /* F.Leray 05.03.04 Useless too*/
    C2F (dr) ("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F (dr) ("xset", "line style", x+3,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    if ( ppsubwin->axes.axes_visible[2] )
    {
      /* draw the line of the axis like in 2d */
      /* the two bounds are (ixbox[2],iybox[2]) and (ixbox[2],iybox[2]) */
      C2F(dr)("xpolys","v",&(ixbox[2]),&(iybox[2]),x,&one,&two, PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
    if ( ( xind[4]+xind[5] == 3) || ( xind[4]+xind[5] == 11 ) )
    {
      /* i copy this test but I don't know what it mean... jb Silvy 03/2006 */
      if ( ppsubwin->axes.axes_visible[1] )
      {
        /* draw the line of the axis like in 2d */
        /* the two bounds are (ixbox[3],iybox[3]) and (ixbox[3],iybox[3]) */
        C2F(dr)("xpolys","v",&(ixbox[3]),&(iybox[3]),x,&one,&two, PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
      if ( ppsubwin->axes.axes_visible[0] )
      {
        /* draw the line of the axis like in 2d */
        /* the two bounds are (ixbox[4],iybox[4]) and (ixbox[4],iybox[4]) */
        C2F(dr)("xpolys","v",&(ixbox[4]),&(iybox[4]),x,&one,&two, PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
    }
    else
    {
      if ( ppsubwin->axes.axes_visible[1] )
      {
        /* draw the line of the axis like in 2d */
        /* the two bounds are (ixbox[3],iybox[3]) and (ixbox[3],iybox[3]) */
        C2F(dr)("xpolys","v",&(ixbox[4]),&(iybox[4]),x,&one,&two, PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
      if ( ppsubwin->axes.axes_visible[0] )
      {
        /* draw the line of the axis like in 2d */
        /* the two bounds are (ixbox[4],iybox[4]) and (ixbox[4],iybox[4]) */
        C2F(dr)("xpolys","v",&(ixbox[3]),&(iybox[3]),x,&one,&two, PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
    }
    C2F(dr)("xset","pattern",&pat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }  

  /********************/
  /*** le z scaling ***/ /* DISPLAY Z graduations */
  /********************/


  if (pSUBWIN_FEATURE (psubwin)->project[2]==1)
    {
      double fx,fy,fz; 
      char c_format[5] ;

      integer Ticsdir[2];
      Ticsdir[0]=ixbox[3]-ixbox[4]; /* <=> en pixel direction/vecteur non norme(e)s des tics en x */
      Ticsdir[1]=iybox[3]-iybox[4]; /* <=> idem pour y */
      /* NB: for a default plot3d (simply calling plot3d in siclab console) */
      /* Ticsdir[0] & Ticsdir[1] are negative : we do min - max here... */

      BBoxToval(&fx,&fy,&fz,xind[3],bbox); /* xind[3] <=> en bas a gauche <=> zmin */
      x=ixbox[2]-(xz[0]+xz[1])/20;
      y=(iybox[3]+iybox[2])/2;
  
      /*       NumberFormat(str,((integer) zz[0]),((integer) zz[2])); */
      
      
      if( !ppsubwin->axes.auto_ticks[2] )
	{
	  /* we display the z tics specified by the user*/
	  nbtics = ppsubwin->axes.u_nzgrads;
	  nbsubtics = ppsubwin->axes.nbsubtics[2];

          maxTicksLabelSize[0] = 0 ;
          maxTicksLabelSize[1] = 0 ;
	  
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
	      
	      if( ppsubwin->axes.reverse[2] )
              {
		ztmp = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp);
              }
              
	      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&ztmp);
	      

	      vx[0]=xm;vy[0]=ym;

	      barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
	      barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
	      vx[1]=vx[0]+barlengthx;
	      vy[1]=vy[0]+barlengthy;
		  
	      /* foo is set above with sprintf(foo,c_format,xtmp); */
		  
	      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
	      
              /* get the size of ticks label */
              C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
              maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;


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
	      if ( ppsubwin->grid[2] > -1 && ppsubwin->axes.axes_visible[2] )
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
			  C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
			  C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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

          maxTicksLabelSize[0] = 0 ;
          maxTicksLabelSize[1] = 0 ;
	  
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
	      
              /* get the size of ticks label */
              C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
              maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;

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
	      if ( ppsubwin->grid[2] > -1 && ppsubwin->axes.axes_visible[2] )
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
			  C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
			  C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
  
  if ( sciGetVisibility(ppsubwin->mon_z_label) )
    {
      /* draw z label */
      sciLabel * ppLabel = pLABEL_FEATURE(ppsubwin->mon_z_label) ;
            
      if ( ppLabel->auto_rotation )
      {
        /* mult by 10 because */
        sciSetFontOrientation(ppsubwin->mon_z_label, 270 * 10 ) ;
      }
      
      if( ppLabel->auto_position )
      {
        
        int segmentStart[2] = { ixbox[2], iybox[2] } ;
        int segmentEnd[2]   = { ixbox[3], iybox[3] } ;
        
        /* add the bar size and ticks label size to the offset */
        int offset[2] = { constOffset[0] + maxTicksLabelSize[0] + abs( barlengthx ), 
                          constOffset[1] + maxTicksLabelSize[1] + abs( barlengthy )  } ;
        computeLabelAutoPos( ppsubwin->mon_z_label, segmentStart, segmentEnd, offset ) ;
      }
      /* a trick to force the display with 2d scale */
      drawText( ppLabel->text ) ;
      
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
  	  
	  if( !ppsubwin->axes.auto_ticks[0] )
	    {
	      /* we display the x tics specified by the user*/
	      nbtics = ppsubwin->axes.u_nxgrads;
	      nbsubtics = ppsubwin->axes.nbsubtics[0];
	      
              maxTicksLabelSize[0] = 0 ;
              maxTicksLabelSize[1] = 0 ;

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

                  /* get the size of the icks label */
		  C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                  maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
                  maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;
                  
                  
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
		      
		  if( ppsubwin->axes.axes_visible[0] )
                  {    
		    C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
		    C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
		    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }
		  /* grid to put here */
		  if ( ppsubwin->grid[0] > -1 && ppsubwin->axes.axes_visible[0] )
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
			      C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
			      C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
              {
		ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.xgrads[0]));
              }
	      else
              {
		ChoixFormatE(c_format,
			     ppsubwin->axes.xgrads[0],
			     ppsubwin->axes.xgrads[lastxindex],
			     ((ppsubwin->axes.xgrads[lastxindex])-(ppsubwin->axes.xgrads[0]))/(lastxindex));
              }
              
	      nbtics = ppsubwin->axes.nxgrads;
	      nbsubtics = ppsubwin->axes.nbsubtics[0];
	      	     
              maxTicksLabelSize[0] = 0 ;
              maxTicksLabelSize[1] = 0 ;
 
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

                  if( ppsubwin->axes.reverse[0] )
                  {
		    xtmp = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp);
		  }
                  
		  ComputeGoodTrans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz);
		  
		  vx[0]=xm;vy[0]=ym; 
		  
		  barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
		  barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

		  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */

                  /* get the size of ticks label */
		  C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                  maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
                  maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;

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
		      
		  if( ppsubwin->axes.axes_visible[0] )
                  {
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
                    {
		      C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
                    }	    
		    C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
		    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }
		  /* grid to put here */
		  if ( ppsubwin->grid[0] > -1 && ppsubwin->axes.axes_visible[0]  )
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
			  
                      for(j=0;j<nbsubtics;j++)
                      {
                        tmp_log_grads[j] = log10( tmp[0] + (j) * pas ) ;
                      }
			  
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
                        C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
                        C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
			      
			      if( ppsubwin->axes.axes_visible[0] )
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
      if ( sciGetVisibility(ppsubwin->mon_x_label) )
      {
        sciLabel * ppLabel = pLABEL_FEATURE( ppsubwin->mon_x_label ) ;
        if( ppLabel->auto_rotation )
        {
          sciSetFontOrientation(ppsubwin->mon_x_label, 0) ;
        }
        
	  
        if( ppLabel->auto_position )
        {

          int segmentStart[2] = { ixbox[4], iybox[4] } ;
          int segmentEnd[2]   = { ixbox[5], iybox[5] } ;
          
          /* add the bar size and ticks label size to the offset */
          int offset[2] = { constOffset[0] + maxTicksLabelSize[0] + abs( barlengthx ), 
                            constOffset[1] + maxTicksLabelSize[1] + abs( barlengthy )  } ;
          computeLabelAutoPos( ppsubwin->mon_x_label, segmentStart, segmentEnd, offset ) ;
        }
        /* a trick to force the display with 2d scale */
        drawText( ppLabel->text ) ;
        
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

              maxTicksLabelSize[0] = 0 ;
              maxTicksLabelSize[1] = 0 ;
	      
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

                  /* get the size of the ticks */
		  C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                  maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
                  maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;
                  

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
		  if ( ppsubwin->grid[1] > -1 && ppsubwin->axes.axes_visible[1] )
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
			      C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
			      C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
	     
              maxTicksLabelSize[0] = 0 ;
              maxTicksLabelSize[1] = 0 ;
 
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
                  maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
                  maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;

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
		  
		  if ( ppsubwin->grid[1] > -1 && ppsubwin->axes.axes_visible[1] )
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
			      C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
			      C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
      if ( sciGetVisibility(ppsubwin->mon_y_label) )
      {
        sciLabel * ppLabel = pLABEL_FEATURE( ppsubwin->mon_y_label ) ;
        if( ppLabel->auto_rotation )
        {
          sciSetFontOrientation( ppsubwin->mon_y_label, 0 ) ;
        }
          
        if( ppLabel->auto_position )
        {
            
          int segmentStart[2] = { ixbox[4], iybox[4] } ;
          int segmentEnd[2]   = { ixbox[5], iybox[5] } ;
            
          /* add the bar size and ticks label size to the offset */
          int offset[2] = { constOffset[0] + maxTicksLabelSize[0] + abs( barlengthx ), 
                            constOffset[1] + maxTicksLabelSize[1] + abs( barlengthy )  } ;
          computeLabelAutoPos( ppsubwin->mon_y_label, segmentStart, segmentEnd, offset ) ;
        }
        /* a trick to force the display with 2d scale */
        drawText( ppLabel->text ) ;
        
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
	      
              maxTicksLabelSize[0] = 0 ;
              maxTicksLabelSize[1] = 0 ;

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
                  maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
                  maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;
                  

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
		  if ( ppsubwin->grid[0] > -1 && ppsubwin->axes.axes_visible[0]  )
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
			      C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
			      C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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

              maxTicksLabelSize[0] = 0 ;
              maxTicksLabelSize[1] = 0 ;
	           
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
		  maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
                  maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;

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
		  if ( ppsubwin->grid[0] > -1 && ppsubwin->axes.axes_visible[0] )
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
			      
			      			      
			      xg[0]= xm;  yg[0]= ym;  
			      if (Ishidden(psubwin))
				{ xg[1]= xm; yg[1]= iybox[0] -iybox[5]+ym; }
			      else
				{xg[1]= ixbox[1] - ixbox[3] +xm; yg[1]= iybox[5] - iybox[4] +ym; } 
			      C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			      C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
			      C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			      xg[0]= xg[1]; yg[0]= yg[1];
			      xg[1] = ixbox[1] - ixbox[3] +xm; yg[1]=  iybox[0] - iybox[4] +ym;
			      C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
			      C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
			      
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
			      C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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

      if ( sciGetVisibility(ppsubwin->mon_x_label) )
      { 
	  
          sciLabel * ppLabel = pLABEL_FEATURE(ppsubwin->mon_x_label) ;
          if( ppLabel->auto_rotation )
          {
	    sciSetFontOrientation(ppsubwin->mon_x_label, 0 ) ;
	  }
	  

	   if( ppLabel->auto_position )
           {
             
             int segmentStart[2] = { ixbox[3], iybox[3] } ;
             int segmentEnd[2]   = { ixbox[4], iybox[4] } ;
            
             /* add the bar size and ticks label size to the offset */
             int offset[2] = { constOffset[0] + maxTicksLabelSize[0] + abs( barlengthx ), 
                               constOffset[1] + maxTicksLabelSize[1] + abs( barlengthy )  } ;
             computeLabelAutoPos( ppsubwin->mon_x_label, segmentStart, segmentEnd, offset ) ;
           }
           /* a trick to force the display with 2d scale */
           drawText( ppLabel->text ) ;
           
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
	      
              maxTicksLabelSize[0] = 0 ;
              maxTicksLabelSize[1] = 0 ;

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
		  maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
                  maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;
                  
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
		  if ( ppsubwin->grid[1] > -1 && ppsubwin->axes.axes_visible[1] )
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
			      C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
			      C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
	      
              maxTicksLabelSize[0] = 0 ;
              maxTicksLabelSize[1] = 0 ;

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
		  maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
                  maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;
                  
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
		  if ( ppsubwin->grid[1] > -1 && ppsubwin->axes.axes_visible[1] )
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
			      C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
			      C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
      
      if ( sciGetVisibility(ppsubwin->mon_y_label) )
      {  
        sciLabel * ppLabel =  pLABEL_FEATURE(ppsubwin->mon_y_label) ;
        if( ppLabel->auto_rotation )
        {
          sciSetFontOrientation(ppsubwin->mon_y_label, 0 ) ;
        }
                
        if( ppLabel->auto_position )
        {
             
          int segmentStart[2] = { ixbox[3], iybox[3] } ;
          int segmentEnd[2]   = { ixbox[4], iybox[4] } ;
          
          /* add the bar size and ticks label size to the offset */
          int offset[2] = { constOffset[0] + maxTicksLabelSize[0] + abs( barlengthx ), 
                            constOffset[1] + maxTicksLabelSize[1] + abs( barlengthy )  } ;
          computeLabelAutoPos( ppsubwin->mon_y_label, segmentStart, segmentEnd, offset ) ;
        }
        /* a trick to force the display with 2d scale */
        drawText( ppLabel->text ) ;
        
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


    
  if (sciGetEntityType(pobj) == SCI_SUBWIN){

    cube_scaling = pSUBWIN_FEATURE(pobj)->cube_scaling;

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

/* compute the graduation of the segment [minVal,maxVal] knowing the number of ticks */
void GradFixedlog( double minVal, double maxVal, double * ticks, int nbGrads )
{
  int initSize ;
  int i ;

  /* intialize the array as usual */
  GradLog( minVal, maxVal, ticks, &initSize, FALSE ) ;

  if ( initSize > nbGrads )
  {
    /* we create a smaller vector from a bigger one */
    int nbRemove = initSize - nbGrads ;
    
    BOOL * removedTicks ;
    if( ( removedTicks = MALLOC( initSize * sizeof(BOOL) ) ) == NULL )
    {
      return ;
    }

    for ( i = 0 ; i < initSize ; i++ )
    {
      removedTicks[i] = FALSE ;
    }

    /* we now remove the nbremove indexes : round( ( 0.5 + i ) * size / nbRemove ) */
    /* i=0..nbReg-1 should do the thing */
    for ( i = 0 ; i < nbRemove ; i++ )
    {
      int remIndex = 1 + (int) round(  i  * ((double) initSize - 2 ) / ( (double) nbRemove ) ) ;
      removedTicks[remIndex] = TRUE ;
    }

    removeBadTicks( ticks, removedTicks, &initSize ) ;

    FREE( removedTicks ) ;

  }

}


/* compute the automatic graduation of the segment [_min,_max] and store it in _grads */
/* the number of graduation may be fixed if compNgrads is TRUE or automaticaly computed */
/* otherwise. */
int GradLog( double   _min   ,
             double   _max   ,
             double * _grads ,
             int    * n_grads,
             int      compNgrads )
{
  int i;
  int log_min, log_max;
  int size;
  
  if ( compNgrads )
  {
    GradFixedlog( _min, _max, _grads, *n_grads ) ;
    return 0 ;
  }

  log_max =  (int) ceil(_max);
  log_min =  (int) floor(_min);
  

  size = log_max - log_min +1;
  /*  tab=(int *)MALLOC(size*sizeof(int)); */
  
  /*   for(i=0;i<size;i++) tab[i]=log_min+i; */

  *n_grads = 0 ;

  if(size<=7)    {
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
      {
	for(i=0;i<=(int )(size/pas);i++)
	  {
	    _grads[i] = log_min+(i*pas);
	    
	    *n_grads = (*n_grads) + 1;
	    /* 	    sciprint("Juste en sortie, _grads[%d] = %lf\n",i, _grads[i]); */
	  }
      }
    }
  
  return 0;
}

/**
 * get the exponent used for log axis from given data bounds
 * @return 0 if OK, -1 if negative bounds.
 */
int sciGetLogExponent( double minBound, double maxBound, double * expMin, double * expMax )
{
  if ( minBound > 0 )
  {
    *expMin = floor( log10( minBound ) ) ;
    *expMax = ceil(  log10( maxBound ) ) ;
    return 0 ;
  }
  *expMax = 1.0 ;
  *expMin = 0.0 ;
  return -1 ;
}

/* get the displayed bounds of an axis */
void sciGetDisplayedBounds( sciPointObj * pSubWin,
                            double      * xmin   ,
                            double      * xmax   ,
                            double      * ymin   ,
                            double      * ymax   ,
                            double      * zmin   ,
                            double      * zmax    )
{
  sciSubWindow * ppsubwin =  pSUBWIN_FEATURE ( pSubWin ) ;
    /*****************************************************************
     * get initial bounds
   *****************************************************************/
  if( sciGetZooming( pSubWin ) )
  {
    *xmin = ppsubwin->ZRect[0] ; 
    *ymin = ppsubwin->ZRect[1] ; 
    *xmax = ppsubwin->ZRect[2] ;
    *ymax = ppsubwin->ZRect[3] ;
    *zmin = ppsubwin->ZRect[4] ;
    *zmax = ppsubwin->ZRect[5] ;
  }
  else
  {
    *xmin = ppsubwin->SRect[0] ;
    *ymin = ppsubwin->SRect[2] ;
    *xmax = ppsubwin->SRect[1] ;
    *ymax = ppsubwin->SRect[3] ;
    *zmin = ppsubwin->SRect[4] ;
    *zmax = ppsubwin->SRect[5] ;
  }
  
  
   
  /*****************************************************************
   * modify  bounds and aaint  if using log scaling X axis
   *****************************************************************/
  if ( ppsubwin->logflags[0] == 'l' )
  {
    if ( sciGetLogExponent( *xmin, *xmax, xmin, xmax ) != 0 )
    {
      Scistring("Warning: Can't use Log on X-axis xmin is negative. \n");
    }
  }

  /*****************************************************************
   * modify  bounds and aaint  if using log scaling Y axis
   *****************************************************************/
  if ( ppsubwin->logflags[1] == 'l' )
  {
    if ( sciGetLogExponent( *ymin, *ymax, ymin, ymax ) != 0 )
    {
      Scistring("Warning: Can't use Log on Y-axis ymin is negative. \n");
    }
  }

  /*****************************************************************
   * modify  bounds and aaint  if using log scaling Z axis
   *****************************************************************/
  if ( ppsubwin->logflags[2] == 'l' )
  {
    if ( sciGetLogExponent( *zmin, *zmax, zmin, zmax ) != 0 )
    {
      Scistring("Warning: Can't use Log on Z-axis zmin is negative. \n");
    }
  }

}

/* F.Leray au 13.10.04 completly review for new axes graduations */
/*** F.Leray 02.04.04 */
/* FUNCTION FOR 2D UPDATE ONLY !!!!! <=> beginning of axis_3ddraw (in 2d HERE of course! ) */
/* Copy on update_frame_bounds */
BOOL sci_update_frame_bounds_2d(sciPointObj *pobj)
{
  double xmax, xmin, ymin, ymax, zmin, zmax ;
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
  

  sciGetDisplayedBounds( pobj, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax ) ;


  /* _grad Init. to 0. */
  for(i=0;i<20;i++) 
    {
      ppsubwin->axes.xgrads[i] = 0.;
      ppsubwin->axes.ygrads[i] = 0.;
    }
   
 
  if ( ppsubwin->logflags[0]=='n') { /* x-axis */
    TheTicks(&xmin, &xmax, &(ppsubwin->axes.xgrads[0]), &ppsubwin->axes.nxgrads, FALSE); 
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'n',NULL,ppsubwin->axes.nbsubtics[0]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
  }
  else{ /* log. case */
    GradLog(xmin,xmax,ppsubwin->axes.xgrads,&ppsubwin->axes.nxgrads, FALSE );
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'l',ppsubwin->axes.xgrads,0);
  }
   
  if ( ppsubwin->logflags[1]=='n') { /* y-axis */
    TheTicks(&ymin, &ymax, &(ppsubwin->axes.ygrads[0]), &ppsubwin->axes.nygrads, FALSE);
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'n',NULL, ppsubwin->axes.nbsubtics[1]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
  }
  else{ /* log. case */
    GradLog(ymin,ymax,ppsubwin->axes.ygrads,&ppsubwin->axes.nygrads, FALSE );
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
      TheTicks(&xmin, &xmax, &(ppsubwin->axes.xgrads[0]), &ppsubwin->axes.nxgrads, FALSE);
      ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'n',NULL, ppsubwin->axes.nbsubtics[0]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    }
    else{ /* log. case */
      GradLog(xmin,xmax,ppsubwin->axes.xgrads,&ppsubwin->axes.nxgrads, FALSE);
      ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'l',ppsubwin->axes.xgrads,0);
    }

 

    if ( ppsubwin->logflags[1]=='n') { /* y-axis */
      TheTicks(&ymin, &ymax, &(ppsubwin->axes.ygrads[0]), &ppsubwin->axes.nygrads, FALSE);
      ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'n',NULL, ppsubwin->axes.nbsubtics[1]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    }
    else{ /* log. case */
      GradLog(ymin,ymax,ppsubwin->axes.ygrads,&ppsubwin->axes.nygrads, FALSE );
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

 /*  if(sciGetZooming(pobj) == TRUE) { /\* a revoir *\/ */
/*     xmin= ppsubwin->ZRect[0];  */
/*     ymin= ppsubwin->ZRect[1];  */
/*     xmax= ppsubwin->ZRect[2]; */
/*     ymax= ppsubwin->ZRect[3]; */
/*     zmin= ppsubwin->ZRect[4]; */
/*     zmax= ppsubwin->ZRect[5]; */
/*   } */
/*   else { */
/*     xmin=ppsubwin->SRect[0]; */
/*     xmax=ppsubwin->SRect[1]; */
/*     ymin=ppsubwin->SRect[2]; */
/*     ymax=ppsubwin->SRect[3]; */
/*     zmin=ppsubwin->SRect[4]; */
/*     zmax=ppsubwin->SRect[5]; */
/*   } */

 /*  ppsubwin->axes.limits[1]=xmin; /\* set a quoi?? au final ca saute normalement...*\/ */
/*   ppsubwin->axes.limits[3]=xmax; /\* F.Leray 19.10.04 *\/ */
/*   ppsubwin->axes.limits[2]=ymin; */
/*   ppsubwin->axes.limits[4]=ymax; */
/*   ppsubwin->axes.limits[5]=zmin; */
/*   ppsubwin->axes.limits[6]=zmax; */


  sciGetDisplayedBounds( pobj, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax ) ;

  /* /\***************************************************************** */
/*    * modify  bounds and aaint  if using log scaling X axis */
/*    *****************************************************************\/ */
/*   if ( ppsubwin->logflags[0]=='l') { */
/*     if ( xmin >  0) { */
/*       xmax=ceil(log10(xmax));  xmin=floor(log10(xmin)); */
/*     } */
/*     else { */
/*       Scistring("Warning: Can't use Log on X-axis xmin is negative \n"); */
/*       xmax= 1; xmin= 0; */
/*     } */
/*   } */
  
/*   /\***************************************************************** */
/*    * modify  bounds and aaint  if using log scaling Y axis */
/*    *****************************************************************\/ */
/*   if ( ppsubwin->logflags[1]=='l') { */
/*     if ( ymin > 0 ) { */
/*       ymax= ceil(log10(ymax)); ymin= floor(log10(ymin)); */
/*     } */
/*     else { */
/*       Scistring(" Can't use Log on Y-axis ymin is negative \n"); */
/*       ymax= 1; ymin= 0; */
/*     } */
/*   } */
  

/*   /\***************************************************************** */
/*    * modify  bounds and aaint  if using log scaling Y axis */
/*    *****************************************************************\/ */
/*   if ( ppsubwin->logflags[2]=='l') { */
/*     if ( zmin > 0 ) { */
/*       zmax= ceil(log10(zmax)); zmin= floor(log10(zmin)); */
/*     } */
/*     else { */
/*       Scistring(" Can't use Log on Z-axis zmin is negative \n"); */
/*       zmax= 1; zmin= 0; */
/*     } */
/*   } */
  

  /* _grad Init. to 0. */
  for(i=0;i<20;i++) 
    {
      ppsubwin->axes.xgrads[i] = 0.;
      ppsubwin->axes.ygrads[i] = 0.;
      ppsubwin->axes.zgrads[i] = 0.;
    }
   
  
  if ( ppsubwin->logflags[0]=='n') { /* x-axis */
    TheTicks(&xmin, &xmax, &(ppsubwin->axes.xgrads[0]), &ppsubwin->axes.nxgrads, FALSE);
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'n',NULL,ppsubwin->axes.nbsubtics[0]); /* Nb of subtics computation and storage */
  }
  else{ /* log. case */
    GradLog(xmin,xmax,ppsubwin->axes.xgrads,&ppsubwin->axes.nxgrads, FALSE );
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'l',ppsubwin->axes.xgrads,0);
  }
  
  if ( ppsubwin->logflags[1]=='n') { /* y-axis */
    TheTicks(&ymin, &ymax, &(ppsubwin->axes.ygrads[0]), &ppsubwin->axes.nygrads, FALSE);
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'n',NULL, ppsubwin->axes.nbsubtics[1]); /* Nb of subtics computation and storage */
  }
  else{ /* log. case */
    GradLog(ymin,ymax,ppsubwin->axes.ygrads,&ppsubwin->axes.nygrads, FALSE );
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'l',ppsubwin->axes.ygrads,0);
  }
  
  if ( ppsubwin->logflags[2]=='n') { /* z-axis */
    TheTicks(&zmin, &zmax, &(ppsubwin->axes.zgrads[0]), &ppsubwin->axes.nzgrads, FALSE);
    ppsubwin->axes.nbsubtics[2] = ComputeNbSubTics(pobj,ppsubwin->axes.nzgrads,'n',NULL, ppsubwin->axes.nbsubtics[2]); /* Nb of subtics computation and storage */
  }
  else{ /* log. case */
    GradLog(zmin,zmax,ppsubwin->axes.zgrads,&ppsubwin->axes.nzgrads, FALSE );
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
   
  set_scale("tftftf",NULL,ppsubwin->FRect,NULL,ppsubwin->logflags,NULL); 
     
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
      if(ppsubwin->axes.rect == BT_ON)
	STRFLAG[2] = '2';
      else
	STRFLAG[2] = '0';
    }
  
  STRFLAG[3] = '\0';
}


/*----------------------------------------------------------------------------------*/
/* draw the title and the two labels of a 2d axis                                 */
/*----------------------------------------------------------------------------------*/
int labels2D_draw( sciPointObj * psubwin )
{
/* Rewritten by jb Silvy 06/2006 */
  
  integer rect1[4] ;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  int offset[2] ; /* vertical and horizontal offsets to move labels a little from axes */

  if ( !sciGetVisibility(psubwin) )
  {
    return 0 ;
  }
  
  /* get the size of the current subwin in pixels */
  rect1[0]= Cscale.WIRect1[0] ; /* upper left point of the axes x coordinate. */
  rect1[1]= Cscale.WIRect1[1] ; /* upper left point of the axes y coordinate. */
  rect1[2]= Cscale.WIRect1[2] ; /* width of the axes */
  rect1[3]= Cscale.WIRect1[3] ; /* height of the axes */

  /* the little displacment of the labels from the axes box */
  offset[0] = rect1[2] / 50 + 1 ;
  offset[1] = rect1[3] / 25 + 1 ;
  

  /*********/
  /* TITLE */
  /*********/
  
  if ( sciGetVisibility(ppsubwin->mon_title) )
  {
    /* get the pointer on the title */
    sciLabel * ppLabel = pLABEL_FEATURE( ppsubwin->mon_title ) ;
    
    /* get position and orientation of the title */
    if ( ppLabel->auto_rotation )
    {
      sciSetFontOrientation( ppsubwin->mon_title, 0 ) ;
    }
    
    
    if ( ppLabel->auto_position )
    {
      int segmentStart[2] = { rect1[0] + rect1[2], rect1[1] } ;
      int segmentEnd[2]   = { rect1[0]           , rect1[1] } ;
      computeLabelAutoPos( ppsubwin->mon_title, segmentStart, segmentEnd, offset ) ;
    }
    /* draw the label */
    drawText( ppLabel->text ) ;

  }
  
  /***********/
  /* x label */
  /***********/
  
  if( sciGetVisibility(ppsubwin->mon_x_label) )
  {
    /* get the pointer on the title */
    sciLabel * ppLabel = pLABEL_FEATURE( ppsubwin->mon_x_label ) ;
    
    if( ppLabel->auto_rotation )
    {
      sciSetFontOrientation(ppsubwin->mon_x_label, 0 ) ;
    }

    
    if( ppLabel->auto_position )
    {
      int segmentStart[2] ;
      int segmentEnd[2]   ;

      if(ppsubwin->axes.xdir == 'u')
      {
        segmentStart[0] = rect1[0] + rect1[2] ;
        segmentEnd[0]   = rect1[0] ;

        /* we add the size of the numbers to the height */
        segmentStart[1] = ppsubwin->XGradMostOnTop ;
        segmentEnd[1]   = ppsubwin->XGradMostOnTop ;
      }
      else
      {
        segmentStart[0] = rect1[0] ;
        segmentEnd[0]   = rect1[0] + rect1[2] ;
        
        segmentStart[1] = ppsubwin->XGradMostOnBottom ;
        segmentEnd[1]   = ppsubwin->XGradMostOnBottom ;
      }
      computeLabelAutoPos( ppsubwin->mon_x_label, segmentStart, segmentEnd, offset ) ;
    }

    drawText( ppLabel->text ) ;
    
  }
  
  
  /***********/
  /* y label */
  /***********/
  

  if ( sciGetVisibility(ppsubwin->mon_y_label) )
  {
    sciLabel * ppLabel = pLABEL_FEATURE( ppsubwin->mon_y_label ) ;

    if( ppLabel->auto_rotation )
    {
      sciInitFontOrientation( ppsubwin->mon_y_label, 270 * 10 ) ;
    }
    
    if( ppLabel->auto_position )
    {
      int segmentStart[2] ;
      int segmentEnd[2]   ;

      if(ppsubwin->axes.ydir == 'r')
      {

         /* we add the size of the numbers to the height */
        segmentStart[0] = ppsubwin->YGradMostOnRight ;
        segmentEnd[0]   = ppsubwin->YGradMostOnRight ;

        segmentStart[1] = rect1[1] + rect1[3] ;
        segmentEnd[1]   = rect1[1] ;
      }
      else
      {
        segmentStart[0] = ppsubwin->YGradMostOnLeft ;
        segmentEnd[0]   = ppsubwin->YGradMostOnLeft ;
        
        segmentStart[1] = rect1[1] ;
        segmentEnd[1]   = rect1[1] + rect1[3] ;
      }
      computeLabelAutoPos( ppsubwin->mon_y_label, segmentStart, segmentEnd, offset ) ;
    }
    drawText( ppLabel->text ) ;
  }
  
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

int  ComputeCorrectXindAndInsideUD(double Teta,double Alpha, double *dbox, integer *xind, integer *InsideU, integer *InsideD)
{
  double xbox[8], ybox[8], zbox[8];

  Teta = 0.1; /* Little offset to compute correct values for xind,  InsideU and InsideD */

  /* update Cscale.m from the new viewing angles */
  sciUpdateScaleAngles( Teta, Alpha ) ;
 
  sciGetAxisBox( dbox, xbox, ybox, zbox ) ;
 
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
  

  /* we search for values <= 0 if log_flag */
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
      
    if(pppolyline->pvz != NULL && ppsubwin->logflags[2] == 'l') 
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
  
  /*  if(ppsubwin->axes.reverse[0] == TRUE) */
  /*     tmp_fx = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],tmp_fx); */
  
  /*   if(ppsubwin->axes.reverse[1] == TRUE) */
  /*     tmp_fy = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],tmp_fy); */
  
  /*   if(ppsubwin->axes.reverse[2] == TRUE) */
  /*     tmp_fz = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],tmp_fz); */
  
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
 * Draws Axes (only the basic  graphicobject under subwindows) in its SubWindow or figure
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
 * Set the clipping rectangle 
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
 * Clear the clipping rectangle 
 * @author Djalel ABDEMOUCHE
 * 29/11/2002
 */
void
sciUnClip (sciPointObj * pobj)
{
  int value = sciGetIsClipping(pobj);     /* clipping state */
  
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
      {
        sciPolyline * ppPolyLine = pPOLYLINE_FEATURE (psonstmp->pointobj) ;
        if ( ppPolyLine->plot != 5) 
        {/*polyline*/
          N = ppPolyLine->n1 - 1 ;
          if ( (ppPolyLine->plot != 2) && 
              (sciGetIsMark(psonstmp->pointobj) == 1))
          {
            N++ ;
          }
        }
        else /* patch */
        {
          N = 1;
        }
      }
      break;
    case  SCI_SEGS: 
      N=pSEGS_FEATURE (psonstmp->pointobj)->Nbr1 / 2 ;
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
      if (pPOLYLINE_FEATURE (psonstmp->pointobj)->plot != 5) {/*polyline*/
	N = pPOLYLINE_FEATURE (psonstmp->pointobj)->n1-1;
	if ((pPOLYLINE_FEATURE (psonstmp->pointobj)->plot != 2) && 
	    (sciGetIsMark((sciPointObj *)psonstmp->pointobj) == 1))
	  N=N+1;
      }
      else /* patch */
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

void DrawMerge3d(sciPointObj *psubwin, sciPointObj *pmerge, int * DPI)
{
  int N,i,j,index,p,max_p,n1,npoly;
  double * dist;
  double X[5],Y[5],Z[5];
  double * Zoriginal = NULL; /* used to conserve Z wether or not z axis is reversed ! (see plo3dn.c) */
  double *x,*y,*z;
  sciPointObj *pobj; 
  int *locindex;
  int *polyx,*polyy,fill[20];/* here we suppose there is no more than 20 edge in a facet */
  int k1,iflag;
  int pstyle = 0;
  int whiteid,verbose=0,narg;
  static double zmin,zmax,xmoy,ymoy,zmoy,zl;
  int context[6];
  

  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  sciMerge     * ppMerge  = pMERGE_FEATURE (pmerge) ;
  int u;
  /* change to static arrays : indeed, no need to recopy each time the entire data of a given object */
  double xtmp[10]; /* normally max size is 4 for facets (2 for lines and segs) but may be one day we will manage greater complex patchs (that is why the 10) */
  double ytmp[10];
  double ztmp[10];
  
#ifdef _MSC_VER 
  int hdcflag;
#endif

  if(sciGetEntityType (psubwin) != SCI_SUBWIN) return;
  N = ppMerge->N ; /* total number of elements */
  
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
    pobj=(sciPointObj *) sciGetPointerFromHandle (ppMerge->from_entity[i]);
    index = ppMerge->index_in_entity[i];

    /*compute element coordinates */
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
      else{ /* facets */
	p=pSURFACE_FEATURE (pobj)->dimzx;

	for(u=0;u<4;u++){
	  xtmp[u] = pSURFACE_FEATURE (pobj)->pvecx[index*p+u];
	  ytmp[u] = pSURFACE_FEATURE (pobj)->pvecy[index*p+u];
	  ztmp[u] = pSURFACE_FEATURE (pobj)->pvecz[index*p+u];
	}
	
	ReverseDataFor3D(psubwin,xtmp,ytmp,ztmp,4);
	
	x=&(xtmp[0]);
	y=&(ytmp[0]);
	z=&(ztmp[0]);
	
	Zoriginal = &(pSURFACE_FEATURE (pobj)->pvecz[index*p]);

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
      }
      else
      {
        xtmp[0] = ppPolyLine->pvx[index];
        xtmp[1] = ppPolyLine->pvx[index+1];
        
        ytmp[0] = ppPolyLine->pvy[index];
        ytmp[1] = ppPolyLine->pvy[index+1]; /* used by trans3d + drawing : case 0,1 and 4 */
      }
      
      if( ppPolyLine->pvz != NULL )
      {
	  ztmp[0] = ppPolyLine->pvz[index];
	  ztmp[1] = ppPolyLine->pvz[index+1];
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
    case  SCI_SEGS: 
      p = 2;
      /***************/

      xtmp[0] =  pSEGS_FEATURE (pobj)->vx[2*index];
      xtmp[1] =  pSEGS_FEATURE (pobj)->vx[2*index+1];

      ytmp[0] =  pSEGS_FEATURE (pobj)->vy[2*index];
      ytmp[1] =  pSEGS_FEATURE (pobj)->vy[2*index+1];
      
      
      if(pSEGS_FEATURE (pobj)->vz != NULL){
	ztmp[0] = pSEGS_FEATURE (pobj)->vz[2*index];
	ztmp[1] = pSEGS_FEATURE (pobj)->vz[2*index+1];
      }
      
      if(pSEGS_FEATURE (pobj)->vz != NULL)
	ReverseDataFor3D(psubwin,xtmp,ytmp,ztmp,2);
      else
	ReverseDataFor3D(psubwin,xtmp,ytmp,(double *) NULL,2);
 	
      /**************/

      X[0]=xtmp[0];
      X[1]=xtmp[1];
      Y[0]=ytmp[0];
      Y[1]=ytmp[1];
      if (pSEGS_FEATURE (pobj)->vz != (double *) NULL) {
	Z[0]=ztmp[0];
	Z[1]=ztmp[1];
	z=Z;
      }
      else
	z=(double *)NULL;
      x=X;y=Y;
      
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
  for ( i = N ; i>0 ; i--) { /* loop on elements */
    int j,nok;
    j=locindex[i-1]-1;
    index= ppMerge->index_in_entity[j];
    pobj=(sciPointObj *) sciGetPointerFromHandle (ppMerge->from_entity[j]);
    /*     if (sciGetVisibility (pobj)) { */
    if (sciGetVisibility(pobj)) {
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
	else{ /* facets */
	  p=pSURFACE_FEATURE (pobj)->dimzx;
	  
	  for(u=0;u<4;u++){
	    xtmp[u] = pSURFACE_FEATURE (pobj)->pvecx[index*p+u];
	    ytmp[u] = pSURFACE_FEATURE (pobj)->pvecy[index*p+u];
	    ztmp[u] = pSURFACE_FEATURE (pobj)->pvecz[index*p+u];
	  }
	  
	  ReverseDataFor3D(psubwin,xtmp,ytmp,ztmp,4);
	  
	  x=&(xtmp[0]);
	  y=&(ytmp[0]);
	  z=&(ztmp[0]);
	  
	  Zoriginal = &(pSURFACE_FEATURE (pobj)->pvecz[index*p]);
	}
	break;
      case  SCI_POLYLINE:
        {
          sciPolyline * ppPolyLine = pPOLYLINE_FEATURE( pobj ) ;
          p=0;
          if ( sciGetIsMark( pobj ) ) { p=1 ; } /* F.Leray 20.01.05 A REVOIR ICI*/
          if ( sciGetIsLine( pobj ) ) { p=2 ; }
          
          /* check if we want to draw segments or marks */
          if ( ppPolyLine->plot != 2 && sciGetIsMark(pobj) == 1 )
          {
            xtmp[0] = ppPolyLine->pvx[index];
            xtmp[1] = xtmp[0] ;
            
            ytmp[0] = ppPolyLine->pvy[index];
            ytmp[1] = ytmp[0] ; /* used by trans3d + drawing : case 0,1 and 4 */
          }
          else
          {
            xtmp[0] = ppPolyLine->pvx[index];
            xtmp[1] = ppPolyLine->pvx[index+1];
            
            ytmp[0] = ppPolyLine->pvy[index];
            ytmp[1] = ppPolyLine->pvy[index+1]; /* used by trans3d + drawing : case 0,1 and 4 */
          }
                   
          if(ppPolyLine->pvz != NULL){
            ztmp[0] = ppPolyLine->pvz[index];
            ztmp[1] = ppPolyLine->pvz[index+1];
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
	p = 2;
	/***************/
	
	if (pSEGS_FEATURE (pobj)->ptype == 0) /* ptype == 0 F.Leray : This is NOT A champ */
	  {  
	    if (pSEGS_FEATURE (pobj)->iflag == 1) {
	      pstyle=sciGetGoodIndex(pobj, pSEGS_FEATURE (pobj)->pstyle[index]);
	    }
	    else{
	      pstyle=sciGetGoodIndex(pobj, pSEGS_FEATURE (pobj)->pstyle[0]);
	    }
	  }
	else
	  {
	    pstyle=sciGetGoodIndex(pobj, pSEGS_FEATURE (pobj)->pstyle[0]);
	  }
	
	iflag = pSEGS_FEATURE (pobj)->iflag;
	
	xtmp[0] =  pSEGS_FEATURE (pobj)->vx[2*index];
	xtmp[1] =  pSEGS_FEATURE (pobj)->vx[2*index+1];
	
	ytmp[0] =  pSEGS_FEATURE (pobj)->vy[2*index];
	ytmp[1] =  pSEGS_FEATURE (pobj)->vy[2*index+1];
	
	
	if(pSEGS_FEATURE (pobj)->vz != NULL){
	  ztmp[0] = pSEGS_FEATURE (pobj)->vz[2*index];
	ztmp[1] = pSEGS_FEATURE (pobj)->vz[2*index+1];
	}
	
      
	if(pSEGS_FEATURE (pobj)->vz != NULL)
	  ReverseDataFor3D(psubwin,xtmp,ytmp,ztmp,2);
	else
	  ReverseDataFor3D(psubwin,xtmp,ytmp,(double *) NULL,2);
	
	/**************/

	X[0]=xtmp[0];
	X[1]=xtmp[1];
	Y[0]=ytmp[0];
	Y[1]=ytmp[1];
	if (pSEGS_FEATURE (pobj)->vz != (double *) NULL) {
	  Z[0]=ztmp[0];
	  Z[1]=ztmp[1];
	  z=Z;
	}
	else
	  z=(double *)NULL;
	x=X;y=Y;
	
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
#ifdef _MSC_VER
	hdcflag=MaybeSetWinhdc();
#endif
	C2F (dr) ("xset", "thickness",  context+1, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 9L);
	C2F (dr) ("xset", "line style", context+2, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L); 
	C2F (dr) ("xset", "mark", context+4, context+5, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 4L, 4L);
/* #ifdef _MSC_VER */
/* 	if ( hdcflag == 1) ReleaseWinHdc (); */
/* #endif	   */

	if (sciGetEntityType (pobj)==SCI_SURFACE) {
	  int fg1  = pSURFACE_FEATURE (pobj)->hiddencolor;
	  int flag = pSURFACE_FEATURE (pobj)->flag[0];
	  int facteur = 1;
	  
	  polyx[p]=polyx[0];polyy[p]=polyy[0];p++; /*close the facet*/

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
	      zl=0;
	      /* 	      for ( k1= 0 ; k1 < p ; k1++) zl+= z[k1]; */
	      for ( k1= 0 ; k1 < p ; k1++) zl+= Zoriginal[k1]; /* F.Leray 01.12.04 : DO NOT REPLACE z by ztmp here : zmin & zmax are computed to work with z ! */
	      fill[0]=inint((whiteid-1)*((zl/p)-zmin)/(zmax-zmin))+1;

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
	else if(sciGetEntityType (pobj)==SCI_SEGS) { /* PSEGS here ! */
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
	else if(sciGetEntityType (pobj)==SCI_RECTANGLE) { /* RECTANGLE case here ! */
	  if( sciGetIsFilled(pobj) )
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
	    /*C2F(dr)("xsegs","v",polyx,polyy,&p,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L); */
	    C2F (dr) ("xlines", "xv", &deux, polyx, polyy, &un, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
	  }
	  /* 	  else {/\*patch*\/ */
	  /* 	    int close=1; */
	  /* 	    C2F (dr) ("xarea", "v", &p, polyx, polyy, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L); */
	  /* 	  } */
	}
      }
    }

#ifdef _MSC_VER
	if ( hdcflag == 1) ReleaseWinHdc ();
#endif	  
	
  }
  FREE(dist);FREE(locindex);FREE(polyx);FREE(polyy);
}

/*------------------------------------------------------------------------------------------*/
/**
 * draw the figure number numFigure.
 */
void sciDrawFigure( int numFigure )
{
  int curFigure = sciGetNumFigure( sciGetCurrentFigure() ) ;
  sciSetUsedWindow( numFigure ) ;
  sciDrawObj( sciGetCurrentFigure() ) ;
  sciSetUsedWindow( curFigure ) ;
}
/*------------------------------------------------------------------------------------------*/

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
 * Draws Object (only the basic  graphicobject under subwindows) in its SubWindow or figure
 * if and only if pFIGURE_FEATURE(pobj)->auto_redraw == TRUE !!
 * Only used inside High Level functions calls (sucha as plot2d, plot3d...)
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, -1 if not
 */
int
sciDrawObjIfRequired (sciPointObj * pobj)
{
  sciPointObj * pfigure = sciGetParentFigure(pobj);

  if( pFIGURE_FEATURE(pfigure)->auto_redraw && pFIGURE_FEATURE(pfigure)->visible )
  {
    sciDrawObj(pobj);
  }

  return 0;
}

/**sciDrawObj
 * Draws Object (only the basic  graphicobject under subwindows) in its SubWindow or figure
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
  integer closeflag = 0,ias,ias1;
  double w2,h2,as;
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
      sciFigure * ppFigure = pFIGURE_FEATURE(pobj) ;
      int curWinNum = 0 ;
      if( !ppFigure->auto_redraw )
      {
        break ;
      }
      
      x[1] = sciGetBackground (pobj) ;
      x[4] = 0 ;
      /** xclear will properly upgrade background if necessary **/
#ifdef _MSC_VER
      flag_DO = MaybeSetWinhdc() ;
#endif
      
      /* select the right figure for drawing to avoid displaying in a wrong window */
      C2F(dr)("xget","window",&verbose,&curWinNum,&narg,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
      C2F(dr)("xset","window",&(ppFigure->number),PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
      
      C2F(dr)("xset","pixmap",&(ppFigure->pixmap),PI0,PI0,PI0,PI0,PI0,PD0,
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
#ifdef _MSC_VER
      if ( flag_DO == 1) ReleaseWinHdc();
#endif

      /* STOP HERE if figure is invisible: */
      if (sciGetVisibility(pobj) == FALSE) break;

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
      if (sciGetVisibility(pobj) == FALSE) break;

      ppsubwin = pSUBWIN_FEATURE (pobj);

      sciSetSelectedSubWin(pobj); 
     
      set_scale ("tttftt", ppsubwin->WRect, ppsubwin->FRect,
		 NULL, ppsubwin->logflags, 
		 ppsubwin->ARect); 

      if (ppsubwin->is3d) 
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
	  isoflag = (long)(ppsubwin->axes.flag[1]+1)/2; 
	 
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
	 
	  axis_3ddraw(pobj,xbox,ybox,zbox,InsideU,InsideD); /* TEST on sciGetVisibility inside : REMOVED F.Leray 21.01.05 */
	  /* because axis_3ddraw displays 3d axes BUT ALSO compute + reset the 3d scale BEFORE !! */
	  
	  psonstmp = sciGetLastSons (pobj);
	  while (psonstmp != (sciSons *) NULL) {
	    sciDrawObj (psonstmp->pointobj);
	    psonstmp = psonstmp->pprev;
	  }

	  triedre(pobj,xbox,ybox,zbox,InsideU,InsideD);
	 
	  wininfo("alpha=%.1f,theta=%.1f",ppsubwin->alpha,ppsubwin->theta); 
	}/***/
      else /* we are in 2D mode...*/
	{
	  x[0] = sciGetForeground (pobj);
	  x[2] = sciGetLineWidth (pobj);
	  x[3] = sciGetLineStyle (pobj);
	  markidsizenew[0] = sciGetMarkStyle(pobj);
	  markidsizenew[1] = sciGetLineWidth (pobj);x[4] = 0;v = 0;dv = 0;
	 
#ifdef _MSC_VER
	  flag_DO=MaybeSetWinhdc();
#endif
	  C2F (dr) ("xset","dashes",x,x,x+4,x+4,x+4,&v,&dv,&dv,&dv,&dv,5L,4096);
	  C2F (dr) ("xset","foreground",x,x,x+4,x+4,x+4,&v,&dv,&dv,&dv,&dv,5L,4096);
	  C2F (dr) ("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F (dr) ("xset","mark",&markidsizenew[0],&markidsizenew[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	 
	  sci_update_frame_bounds_2d(pobj);
	 
	  
	  /* Clipping only exists in 2d */
	  /* the global var. Cscale has just been updated */
	  /* therefore I can re-compute the clipping now (if it is "clipgrf") */
	  
	  sciClip(pobj); /* to update the clip_box if needed */
	  sciUnClip(pobj);
	  
	  /* 	 if (sciGetVisibility(pobj)) */
	  DrawAxesBackground();
	   
	  /** walk subtree **/
	  psonstmp = sciGetLastSons (pobj);
	  while (psonstmp != (sciSons *) NULL) {
	    sciDrawObj (psonstmp->pointobj);
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
	  
#ifdef _MSC_VER
	  if ( flag_DO == 1) ReleaseWinHdc();
#endif
	  wininfo("");  
	}
      break;                      
      /******************/
	  
    case SCI_AGREG: 
      if (!sciGetVisibility(pobj)) break; /* RE-PUT F.Leray 21.01.05 */
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
      
#ifdef _MSC_VER 
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
      	Legends( ppLegend->pstyle, &(ppLegend->nblegends), getStrMatElement(sciGetText(pobj),0,0));
       
          
      	/* restore the graphic context */

      	C2F (dr1) ("xset", "dashes", &xold[0], &vold, &vold, &vold, &vold, &v,
			 &dv, &dv, &dv, &dv, 5L, 6L);
      	C2F (dr1) ("xset", "foreground", &xold[1], &vold, &vold, &vold, &vold,
			 &v, &dv, &dv, &dv, &dv, 5L, 10L);
#ifdef _MSC_VER 
      	if ( flag_DO == 1) ReleaseWinHdc ();
#endif
 	}
      break; 

      /******************************** 22/05/2002 ***************************/    
    case SCI_FEC:  
      if (!sciGetVisibility(pobj)) break;
      
      n1=1;
      if ((xm = MALLOC ((pFEC_FEATURE (pobj)->Nnode)*sizeof (integer))) == NULL)	return -1;
      if ((ym = MALLOC ((pFEC_FEATURE (pobj)->Nnode)*sizeof (integer))) == NULL)	return -1;

      if(pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d == FALSE)
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

#ifdef _MSC_VER
      flag_DO=MaybeSetWinhdc();
#endif   
      newfec(xm,ym,pFEC_FEATURE (pobj)->pnoeud,pFEC_FEATURE (pobj)->pfun,
	     &pFEC_FEATURE (pobj)->Nnode,&pFEC_FEATURE (pobj)->Ntr,
	     pFEC_FEATURE (pobj)->zminmax,pFEC_FEATURE (pobj)->colminmax,
	     pFEC_FEATURE (pobj)->colout, pFEC_FEATURE (pobj)->with_mesh);
#ifdef _MSC_VER
      if ( flag_DO == 1) ReleaseWinHdc();
#endif
	  
      /* FREE(xm);FREE(ym); */ /* SS 02/04 */
      FREE(xm); xm = (integer *) NULL;
      FREE(ym); ym = (integer *) NULL; /* et F.Leray 18.02.04*/

      break;      
      /******************************** 22/05/2002 ***************************/    
    case SCI_SEGS:
      if (!sciGetVisibility(pobj)) break;
      
      sciClip(pobj);

      /* load the object foreground and dashes color */
      x[0] = sciGetForeground(pobj); /* Adding F.leray 27.04.04 */
      x[2] = sciGetLineWidth (pobj);
      x[3] = sciGetLineStyle (pobj);
      x[4] = 0;
      markidsizenew[0] =  sciGetMarkStyle(pobj);;
      markidsizenew[1] =  sciGetLineWidth (pobj);;

#ifdef _MSC_VER 
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
#ifdef _MSC_VER 
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif 

      if (pSEGS_FEATURE (pobj)->ptype == 0) /* ptype == 0 F.Leray : This is NOT A champ */
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
	  if ((pstyle = MALLOC ((int)(n/2)*sizeof (integer))) == NULL)	return -1; /* SS 19.04*/
	  if (pSEGS_FEATURE (pobj)->iflag == 1) {
	    for ( i =0 ; i <(int) (n/2) ; i++) {
	      pstyle[i]=sciGetGoodIndex(pobj, pSEGS_FEATURE (pobj)->pstyle[i]);
	    }
	  }
	  else
	    pstyle[0]=sciGetGoodIndex(pobj, pSEGS_FEATURE (pobj)->pstyle[0]);
	  if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
	    {
	      double * xvect = NULL;
	      double * yvect = NULL;
	      double * zvect = NULL;

	      if ((xvect = MALLOC (n*sizeof (double))) == NULL) return -1;
	      if ((yvect = MALLOC (n*sizeof (double))) == NULL) return -1;
	      if ((zvect = MALLOC (n*sizeof (double))) == NULL) return -1;

	      for(i=0;i<n;i++){
		xvect[i] = pSEGS_FEATURE (pobj)->vx[i];
		yvect[i] = pSEGS_FEATURE (pobj)->vy[i];

		if(pSEGS_FEATURE (pobj)->vz!= NULL)
		  zvect[i] = pSEGS_FEATURE (pobj)->vz[i];
		else
		  zvect = (double *) NULL;
	      }
	      
	      ReverseDataFor3D(sciGetParentSubwin(pobj),xvect,yvect,zvect,n);
	      
	      trans3d(sciGetParentSubwin(pobj),n,xm,ym,xvect,yvect,zvect);
	      
	      FREE(xvect); xvect = NULL;
	      FREE(yvect); yvect = NULL;
	      FREE(zvect); zvect = NULL;
	    }
	  else
	    {
	      for ( i =0 ; i <n ; i++) {
		xm[i]= XScale(pSEGS_FEATURE (pobj)->vx[i]); 
		ym[i]= YScale(pSEGS_FEATURE (pobj)->vy[i]);}
	    } 
#ifdef _MSC_VER 
	  flag_DO = MaybeSetWinhdc();
#endif
	  if (pSEGS_FEATURE (pobj)->arrowsize == 0){
	    if(sciGetIsMark(pobj))
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
		DrawNewMarks(pobj,n,xm,ym,DPI);
	      }	   
	    
	    if(sciGetIsLine(pobj)){
	      
	      x[0] = sciGetForeground(pobj);
	      
	      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,
			&dv, &dv, &dv, 5L, 4096);
	      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
			PD0, PD0, PD0, 0L, 0L);    
	      C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
			PD0, PD0, PD0, 0L, 0L);
	      C2F(dr)("xsegs","v",xm,ym,&n,pstyle,&pSEGS_FEATURE (pobj)->iflag,
		      PI0,PD0,PD0,PD0,PD0,0L,0L);
	    }
	  }
	  else{ 
	    if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->is3d)
	      {  
		as=pSEGS_FEATURE (pobj)->arrowsize;
		C2F(echelle2dl)(&as,&as,&ias,&ias1,&un,&un,"f2i"); 
	/* 	if (pSUBWIN_FEATURE (sciGetParentSubwin(pobj))->isoview) */
/* 		  ias=ias*10; */
/* 		else  */
		  ias =ias/2;
		
		if(sciGetIsMark(pobj))
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
		    DrawNewMarks(pobj,n,xm,ym,DPI);
		  }
		
		if(sciGetIsLine(pobj)){

		  x[0] = sciGetForeground(pobj);
		  
		  C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,
			    &dv, &dv, &dv, 5L, 4096);
		  C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
			    PD0, PD0, PD0, 0L, 0L);    
		  C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
			    PD0, PD0, PD0, 0L, 0L);
	
		  C2F(dr)("xarrow","v",xm,ym,&n,&ias,pstyle,&pSEGS_FEATURE (pobj)->iflag,PD0,PD0,PD0,PD0,0L,0L);
		}
	      }
	    else
	      {
		  as=pSEGS_FEATURE (pobj)->arrowsize;
		  C2F(echelle2dl)(&as,&as,&ias,&ias1,&un,&un,"f2i"); 
		  ias = ias/2;
		  
		if(sciGetIsMark(pobj))
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
		    
		    DrawNewMarks(pobj,n,xm,ym,DPI);
		  }
		
		if ( sciGetIsLine( pobj ) )
		{
		    x[0] = sciGetForeground(pobj);
		    
		    C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,
			      &dv, &dv, &dv, 5L, 4096);
		    C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
			      PD0, PD0, PD0, 0L, 0L);    
		    C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
			      PD0, PD0, PD0, 0L, 0L);
		    
		    C2F(dr)("xarrow","v",xm,ym,&n,&ias,pstyle,&pSEGS_FEATURE (pobj)->iflag,PD0,PD0,PD0,PD0,0L,0L);
		}
	
		/* with C2F(dr)("xarrow",... did not work: why? What does (dr1) routine make more than (dr) in New Graphics mode ?? */
		/* Answer : dr deals with pixels value (data: xm and ym are integers!!) whereas dr1 deals with double value coming from the user */
		/* This is true for old and new graphics mode. */
	      } /***/
	  }
#ifdef _MSC_VER 
	  if ( flag_DO == 1) ReleaseWinHdc ();
#endif 
	  FREE(xm);         xm = (integer *) NULL;
	  FREE(ym);         ym = (integer *) NULL; 
	  FREE(pstyle); pstyle = (integer *) NULL; /* SS 19.04*/
	}
      else    /*ptype == 1*/ /* ptype == 1 F.Leray : This IS A champ */
        {
#ifdef _MSC_VER 
	  flag_DO = MaybeSetWinhdc();
#endif
	  /*verbose = 1; */ /* debug F.Leray release mode */
	  C2F(dr)("xget","use color",&verbose, &uc, &narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
	  if (uc)
	    C2F(dr)("xget","color",&verbose,xz,&narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
	  else
	    C2F(dr)("xget","line style",&verbose,xz,&narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
#ifdef _MSC_VER 
	  if ( flag_DO == 1) ReleaseWinHdc ();
#endif 

	  n=2*(pSEGS_FEATURE (pobj)->Nbr1)*((pSEGS_FEATURE (pobj)->Nbr2));
	 

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
	  if ( pSEGS_FEATURE (pobj)->typeofchamp == 1) { /* champ1 has been called */
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

	  sciChamp2DRealToPixel(xm,ym,zm,&na,&arssize,
			     pSEGS_FEATURE (pobj)->vx,pSEGS_FEATURE (pobj)->vy,pSEGS_FEATURE (pobj)->vfx,
			     pSEGS_FEATURE (pobj)->vfy,&(pSEGS_FEATURE (pobj)->Nbr1),
			     &(pSEGS_FEATURE (pobj)->Nbr2),&(pSEGS_FEATURE (pobj)->parfact),&(pSEGS_FEATURE (pobj)->typeofchamp));
#ifdef _MSC_VER 
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
	      
	      /*      for(i=0;i<n;i++) sciprint("xm[%d] =%d\n",i,xm[i]); */
	      /* 	      for(i=0;i<n;i++) sciprint("ym[%d] =%d\n",i,ym[i]); */


	      /* 	      for(i=0;i<n;i++) sciprint("xvect[%d] =%lf\n",i,xvect[i]); */
	      /* 	      for(i=0;i<n;i++) sciprint("yvect[%d] =%lf\n",i,yvect[i]); */
	      /* 	      sciprint("na = %d\n",na); */
	      /* 	      sciprint("\n"); */

	      /* F.Leray 06.12.04 */
	      /* A REVOIR : ne marche pas en 3D */


	      ReverseDataFor3D(sciGetParentSubwin(pobj),xvect,yvect,NULL,n);
	      
	      trans3d(sciGetParentSubwin(pobj),n,xm,ym,xvect,yvect,NULL);
	      
	      FREE(xvect); xvect = NULL;
	      FREE(yvect); yvect = NULL;
	      FREE(zvect); zvect = NULL;
	    }

	  if (pSEGS_FEATURE (pobj)->typeofchamp == 0){
	    if(sciGetIsMark(pobj))
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
		
		DrawNewMarks(pobj,n,xm,ym,DPI);
	      }

	    if(sciGetIsLine(pobj)){

	      x[0] = sciGetForeground(pobj);

	      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,
			&dv, &dv, &dv, 5L, 4096);
	      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
			PD0, PD0, PD0, 0L, 0L);    
	      C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
			PD0, PD0, PD0, 0L, 0L);
	      C2F(dr)("xarrow","v",xm,ym,&na,&arssize,xz,(sflag=0,&sflag),&dv,&dv,&dv,&dv,0L,0L);
	    }
	  }
	  else{
	    if(sciGetIsMark(pobj))
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
		
		DrawNewMarks(pobj,n,xm,ym,DPI);
	      }
	    
	    if(sciGetIsLine(pobj)){
	      
	      x[0] = sciGetForeground(pobj);
	      
	      C2F (dr) ("xset", "dashes", x, x, x+3, x+3, x+3, &v, &dv,
			&dv, &dv, &dv, 5L, 4096);
	      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
			PD0, PD0, PD0, 0L, 0L);    
	      C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
			PD0, PD0, PD0, 0L, 0L);
	      C2F(dr)("xarrow","v",xm,ym,&na,&arssize,zm,(sflag=1,&sflag),&dv,&dv,&dv,&dv,0L,0L);
	    }
	  }
#ifdef _MSC_VER 
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
	    
#ifdef _MSC_VER
	    flag_DO = MaybeSetWinhdc();
#endif
	    frame_clip_on();
  
	    if (strncmp(pGRAYPLOT_FEATURE (pobj)->datamapping,"scaled", 6) == 0)
	      GraySquareScaled(xm,ym,pGRAYPLOT_FEATURE (pobj)->pvecz,n1,n2); /* SS 03/01/03 */
	    else
	      GraySquareDirect(xm,ym,pGRAYPLOT_FEATURE (pobj)->pvecz,n1,n2);

	    frame_clip_off();
#ifdef _MSC_VER
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
	    
#ifdef _MSC_VER
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
#ifdef _MSC_VER
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
#ifdef _MSC_VER
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
#ifdef _MSC_VER
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
	    
#ifdef _MSC_VER
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
#ifdef _MSC_VER
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
#ifdef _MSC_VER
	    flag_DO = MaybeSetWinhdc();
#endif

	    frame_clip_on(); 
	    GraySquare1(xm,ym,pGRAYPLOT_FEATURE (pobj)->pvecz,n1,n2); 
	    frame_clip_off();
#ifdef _MSC_VER
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
	   
#ifdef _MSC_VER
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
#ifdef _MSC_VER
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
      
#ifdef _MSC_VER 
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
   
#ifdef _MSC_VER 
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif  
      n1 = pPOLYLINE_FEATURE (pobj)->n1;
      /* n2 = 1; */ /* the number of curves is always 1 since we draw each one line at a given time */
      nb_curves_bar = pPOLYLINE_FEATURE (pobj)->n2;
      closeflag = pPOLYLINE_FEATURE (pobj)->closed; /* 0 or 1 */
      
      /***/
      sciClip(pobj);


#ifdef _MSC_VER 
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
                      
		      /* encapsulate this for a clip check on each segment */
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
/* #ifdef _MSC_VER  */
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
	      
	      if ( sciGetIsMark(pobj) )
              {
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
	      
	      if ( sciGetIsLine(pobj) )
              {
                
		x[0] = sciGetForeground( pobj ) ;
                x[2] = sciGetLineWidth(  pobj ) ;
                x[3] = sciGetLineStyle( pobj ) ;
		
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
	  
#ifdef _MSC_VER
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

#ifdef _MSC_VER 
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
#ifdef _MSC_VER 
      flag_DO = MaybeSetWinhdc ();
#endif
    
      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, 
		PD0, PD0, PD0, PD0, 4L, 9L);   
      C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, 
		PD0, PD0, PD0, PD0, 4L, 10L);
      
#ifdef _MSC_VER 
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
     
#ifdef _MSC_VER 
      flag_DO = MaybeSetWinhdc ();
#endif
      sciClip(pobj);
      
      if(sciGetIsFilled(pobj) == TRUE){
	x[0] = sciGetBackground(pobj);
	C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4, &v, 
		  &dv, &dv, &dv, &dv, 5L, 6L);
	C2F (dr) ("xset", "foreground", x, x, x+4, x+4, x+4,&v, 
		  &dv, &dv, &dv, &dv, 5L, 10L );
	C2F (dr) ("xfarc", str, &x1, &yy1, &w1, &h1, &angle1, &angle2, PD0, PD0, PD0,PD0, 0L, 0L);
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
#ifdef _MSC_VER 
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
#ifdef _MSC_VER 
      flag_DO = MaybeSetWinhdc ();
#endif

      C2F (dr) ("xset", "thickness", x+2, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);    
      C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0,
		PD0, PD0, PD0, 0L, 0L);

#ifdef _MSC_VER 
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
	  
	  
#ifdef _MSC_VER 
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
#ifdef _MSC_VER
        if ( flag_DO == 1)  ReleaseWinHdc ();
#endif
      }
      else /* Rect. in 3D */
      { 
        int close=1;
        n=4;
                
#ifdef _MSC_VER
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
#ifdef _MSC_VER 
        if ( flag_DO == 1)  ReleaseWinHdc ();
#endif
      }
    }
    break;
    case SCI_TEXT:
      drawText( pobj ) ;
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
#ifdef _MSC_VER 
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
#ifdef _MSC_VER 
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
#ifdef _MSC_VER
      flag_DO = MaybeSetWinhdc();
#endif
      C2F (dr) ("xset", "thickness",  x+2, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 9L);
      C2F (dr) ("xset", "line style", x+3, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L); /*D.A 17/12/2003*/
      C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 4L, 4L);
#ifdef _MSC_VER
      if ( flag_DO == 1) ReleaseWinHdc ();
#endif
      
      
      n=1;               
#ifdef _MSC_VER
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
#ifdef _MSC_VER
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
/* BUT the corresponding function with driver Gif (and others?) give us: */
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
	ixres = (int)72.; /* default value*/	
	iyres = (int)72.; /* default value*/
      }
      break;
    }

  DPI[0] = ixres;
  DPI[1] = iyres;

  return 0;
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

/*-------------------------------------------------------------------------------------*/
/* compute the 8 vertices (xbox[i],ybox[i],zbox[i]) of an axis aligned box knowing */
/* xmin, xmax, ymin, ymax, zmin and zmax in limits */
void sciGetAxisBox( double limits[6], double xbox[8], double ybox[8], double zbox[8] )
{
  xbox[0]=TRX(limits[0],limits[2],limits[4]); /* transfo. 3D of [xmin,ymin,zmin] */
  ybox[0]=TRY(limits[0],limits[2],limits[4]); /* into [xbox[0],ybox[0],zbox[0] ] */
  zbox[0]=TRZ(limits[0],limits[2],limits[4]); /*                                 */ 
  
  xbox[1]=TRX(limits[0],limits[3],limits[4]); /* transfo. 3D of [xmin,ymax,zmin] */
  ybox[1]=TRY(limits[0],limits[3],limits[4]); /* into [xbox[1],ybox[1],zbox[1] ] */
  zbox[1]=TRZ(limits[0],limits[3],limits[4]); /*                                 */
  
  xbox[2]=TRX(limits[1],limits[3],limits[4]); /* transfo. 3D of [xmax,ymax,zmin] */
  ybox[2]=TRY(limits[1],limits[3],limits[4]); /* into [xbox[2],ybox[2],zbox[2] ] */
  zbox[2]=TRZ(limits[1],limits[3],limits[4]); /*                                 */
  
  xbox[3]=TRX(limits[1],limits[2],limits[4]); /* transfo. 3D of [xmax,ymin,zmin] */
  ybox[3]=TRY(limits[1],limits[2],limits[4]); /* into [xbox[3],ybox[3],zbox[3] ] */
  zbox[3]=TRZ(limits[1],limits[2],limits[4]); /*                                 */
  
  xbox[4]=TRX(limits[0],limits[2],limits[5]); /* transfo. 3D of [xmin,ymin,zmax] */
  ybox[4]=TRY(limits[0],limits[2],limits[5]); /* into [xbox[4],ybox[4],zbox[4] ] */
  zbox[4]=TRZ(limits[0],limits[2],limits[5]); /*                                 */
  
  xbox[5]=TRX(limits[0],limits[3],limits[5]); /* transfo. 3D of [xmin,ymax,zmax] */
  ybox[5]=TRY(limits[0],limits[3],limits[5]); /* into [xbox[5],ybox[5],zbox[5] ] */
  zbox[5]=TRZ(limits[0],limits[3],limits[5]); /*                                 */
  
  xbox[6]=TRX(limits[1],limits[3],limits[5]); /* transfo. 3D of [xmax,ymax,zmax] */
  ybox[6]=TRY(limits[1],limits[3],limits[5]); /* into [xbox[6],ybox[6],zbox[6] ] */
  zbox[6]=TRZ(limits[1],limits[3],limits[5]); /*                                 */
  
  xbox[7]=TRX(limits[1],limits[2],limits[5]); /* transfo. 3D of [xmax,ymin,zmax] */
  ybox[7]=TRY(limits[1],limits[2],limits[5]); /* into [xbox[7],ybox[7],zbox[7] ] */
  zbox[7]=TRZ(limits[1],limits[2],limits[5]); /*                                 */
}
/*-------------------------------------------------------------------------------------*/
/* update the Csacle value from new viewing angles */
void sciUpdateScaleAngles( double theta, double alpha )
{
  double cost = 0.5 ;
  double sint = 0.5 ;
  double cosa = 0.5 ;
  double sina = 0.5 ;

  cost=cos( theta * PI_OVER_180 ) ;
  cosa=cos( alpha * PI_OVER_180 ) ;
  sint=sin( theta * PI_OVER_180 ) ;
  sina=sin( alpha * PI_OVER_180 ) ;
  
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

/*-------------------------------------------------------------------------------------*/
/* compute the drawing order of the axes box vertices int xind */
void sciAxesVerticesIndices( integer insideU[4],
                             integer insideD[4],
                             double  xbox[8]   ,
                             double  ybox[8]   ,
                             integer xind[8]    )
{
  int    i      ;
  int    ind    ;
  int    ind2   ;
  int    ind3   ;
  double xmaxi  ;
  int    tmpind ;

  /* F.Leray 23.02.04 Mise a 0 du tableau xind pour corriger bug*/
  /* dans le cas ind < 3 ET ybox[tmpind] < ybox[tmpind]*/
  for( i = 0 ; i < 8 ; i++ )
  { 
    xind[i] = 0 ;
  }

  /* indices */
  /* determine the indices for the 3d represention */
  xmaxi = ( (double) Maxi(xbox,8L) ) ;
  ind = -1 ;
  MaxiInd( xbox, 8L, &ind, xmaxi ) ;
  if ( ind > 3 )
  {
    xind[0]=ind;
  }
  tmpind = ind ;  
  MaxiInd(xbox,8L,&ind,xmaxi);
  if ( ind > 3 )
  {
    xind[0] = ind ;
  }
  if (ybox[tmpind] > ybox[ind] )
  {
    xind[0] = tmpind ;
  }
  
  if (ind < 0 || ind > 8) 
  {
    Scistring("xind out of bounds");
    xind[0]=0;
  }
  Nextind(xind[0],&ind2,&ind3);
  if (ybox[ind2] > ybox[ind3]) 
  {
    xind[1]=ind2;insideU[0]=ind3;
  }
  else 
  {
    xind[1]=ind3;insideU[0]=ind2;
  }
  Nextind(ind2,&ind2,&ind3); insideU[1]=xind[0];
  insideU[2]=ind2; 
  if ( insideU[0] > 3 )
  {
    insideU[3] = insideU[0] - 4 ;
  }
  else
  {
    insideU[3] = insideU[0] + 4 ;
  }
  xind[2]=ind2;
  /* le pointeger en bas qui correspond */	  
  if ( ind2 > 3 )
  {
    xind[3] = ind2 - 4 ;
  }
  else
  {
    xind[3] = ind2 + 4 ;
  }
  Nextind(xind[3],&ind2,&ind3);
  if (ybox[ind2] < ybox[ind3]) 
  {
    xind[4]=ind2;insideD[0]=ind3;
  }
  else  
  {
    xind[4]=ind3;insideD[0]=ind2;
  }
  Nextind(ind2,&ind2,&ind3);
  insideD[1]=xind[3];
  insideD[2]=ind2;
  if ( insideD[0] > 3 )
  {
    insideD[3]=insideD[0] - 4 ;
  }
  else
  {
    insideD[3]=insideD[0] + 4 ;
  }
  xind[5]=ind2;
         
}
/*-------------------------------------------------------------------------------------*/
/**
 * Convert a length from user coord. to pixels in 2D. This function is similar to
 *         WDouble2Pixel, but also use the logmode in 3D.
 * @param parentSubWin Axes in which the length is computed.
 * @param posX         X coordinate of where the distance is transformed
 *                     (only used in log mode).
 * @param height       converted width in user coord.
 * @return the corresponding length in pixels.
 */
int PixelWidth2d( sciPointObj * parentSubWin, double posX, double width )
{
  if ( pSUBWIN_FEATURE( parentSubWin )->logflags[0] == 'l' )
  {
    return WLogScale( posX, width ) ;
  }

  return WScale( width ) ;
  
}
/*-------------------------------------------------------------------------------------*/
/**
 * Convert a length from user coord. to pixels in 2D. This function is similar to
 *         HDouble2Pixel, but also use the logmode in 3D.
 * @param parentSubWin Axes in which the length is computed.
 * @param posY         Y coordinate of where the distance is transformed
 *                     (only used in log mode).
 * @param height       converted height in user coord.
 * @return the corresponding length in pixels.
 */
int PixelHeight2d( sciPointObj * parentSubWin, double posY, double height )
{
  if ( pSUBWIN_FEATURE( parentSubWin )->logflags[1] == 'l' )
  {
    return HLogScale( posY, height ) ;
  }

  return HScale( height ) ;
  
}
/*-------------------------------------------------------------------------------------*/
/**
 * convert the coordinates of a 2d or 3d point to its pixel coordinates.
 * @param pSubWin SubWindow in which the point is, might be either in 2d or 3d.
 * @param coord3d Coordinates of the point in the axes. If the subwin is in 2d,
 *        only the first two coordinates are used.
 */
void getPixelCoordinates( sciPointObj * pSubWin, double coord3d[3], int pixCoord[2] )
{
  
    if ( pSUBWIN_FEATURE (pSubWin)->is3d )
    {
      int n = 1 ;
      
      double xvect = coord3d[0] ;
      double yvect = coord3d[1] ;
      double zvect = coord3d[2] ;
      
      ReverseDataFor3D(pSubWin,&xvect,&yvect,&zvect,n);
      trans3d(pSubWin,n,&(pixCoord[0]),&(pixCoord[1]),&xvect,&yvect,&zvect ) ;
    }
    else 
    {
      pixCoord[0] = XDouble2Pixel( coord3d[0] ) ;
      pixCoord[1] = YDouble2Pixel( coord3d[1] ) ;
    }
}
/*-------------------------------------------------------------------------------------*/
/* getStringBox                                                                        */
/* compute the four corners of a text                                                  */
/* works only in 2D */
void getStringBox( char   ** text         ,
                   double    textPos[2]   ,
                   int       textDim[2]   ,
                   BOOL      autoSize     ,
                   double    userSize[2]  ,
                   double    angle        ,
                   int       fontId       ,
                   int       fontSize     ,
                   double    corners[4][2] )
{
  int pos[2] ;
  int textSize[2] = {0,0} ;
  int corn[4][2] ;
  StringMatrix * strings = newCompleteMatrix( (void **) text, textDim[0], textDim[1] ) ;

  /* take coordinates in pixels */
  pos[0] = XDouble2Pixel( textPos[0] ) ;
  pos[1] = YDouble2Pixel( textPos[1] ) ;

  if ( !autoSize )
  {
    textSize[0] = WDouble2Pixel( textPos[0], userSize[0] ) ;
    textSize[1] = HDouble2Pixel( textPos[1], userSize[1] ) ;
  }

  /* NULL because we don't need the position of each string */
  getStringsPositions( strings, &fontId, &fontSize, pos, autoSize, textSize, FALSE, angle, NULL, corn ) ;

  /* take everything back to user coordinates */
  /* to retrieve exactly the first corner as in stringl we take the input */
  corners[0][0] = XDPixel2Double( corn[0][0] ) ;
  corners[0][1] = YDPixel2Double( corn[0][1] ) ;
  
  corners[1][0] = XDPixel2Double( corn[1][0] ) ;
  corners[1][1] = YDPixel2Double( corn[1][1] ) ;
  
  corners[2][0] = XDPixel2Double( corn[2][0] ) ;
  corners[2][1] = YDPixel2Double( corn[2][1] ) ;
  
  corners[3][0] = XDPixel2Double( corn[3][0] ) ;
  corners[3][1] = YDPixel2Double( corn[3][1] ) ;

  /* we don't need the matrix anymore, but the text is needed */
  desallocateMatrix( strings ) ;

}
/*-------------------------------------------------------------------------------------*/
/**
 * Compute the bounding box of a text object.
 * @param pText   Pointer on the object to surround.
 * @param cornPix Bounding box in pixel coordinates. If NULL, not computed.
 * @param corners Bounding box in user coordinates. If NULL, not computed.
 */
void getTextBoundingBox( sciPointObj * pText        ,
                         int           cornPix[4][2],
                         double        corners[4][2] )
{
  int           fontId   = sciGetFontStyle( pText ) ;
  int           fontSize = sciGetFontDeciWidth( pText ) / 100 ;
  double        angle    = DEG2RAD( sciGetFontOrientation (pText) / 10 ) ; 
  int           position[2] ;
  double        textPos[3]  ;
  double        userSize[2] ;
  int           textSize[2] ;
  sciText     * ppText   = pTEXT_FEATURE( pText ) ;
  sciPointObj * parentSW = sciGetParentSubwin( pText ) ;

  /* transform the position in pixels */
  /* we don't take the axes reverse into account. This has obviously no meaning for text.*/
  textPos[0] = ppText->x ;
  textPos[1] = ppText->y ;
  textPos[2] = ppText->z ;
  if ( sciGetIs3d( pText ) )
  {
    /* normal case */
    getPixelCoordinates( parentSW, textPos, position ) ;
  }
  else
  {
    /* special for label, use of the 2d scale */
    position[0] = XDouble2Pixel( textPos[0] ) ;
    position[1] = YDouble2Pixel( textPos[1] ) ;
  }
  
  sciGetUserSize( pText, &(userSize[0]), &(userSize[1]) ) ;
  
  /* We take the size in 2d. */
  textSize[0] = PixelWidth2d(  parentSW, ppText->x, userSize[0] ) ;
  textSize[1] = PixelHeight2d( parentSW, ppText->y, userSize[1] ) ;

  if ( cornPix == NULL )
  {
    int corn[4][2] ;
    /* NULL because we don't need the position of each string */
    getStringsPositions( sciGetText( pText )     ,
                         &fontId                 ,
                         &fontSize               ,
                         position                ,
                         sciGetAutoSize( pText ) ,
                         textSize                ,
                         sciGetCenterPos( pText ),
                         angle                   ,
                         NULL                    ,
                         corn                     ) ;
    if ( corners != NULL )
    {
      /* take everything back to user coordinates */
      /* to retrieve exactly the first corner as in stringl we take the input */
      corners[0][0] = XDPixel2Double( corn[0][0] ) ;
      corners[0][1] = YDPixel2Double( corn[0][1] ) ;
      
      corners[1][0] = XDPixel2Double( corn[1][0] ) ;
      corners[1][1] = YDPixel2Double( corn[1][1] ) ;
      
      corners[2][0] = XDPixel2Double( corn[2][0] ) ;
      corners[2][1] = YDPixel2Double( corn[2][1] ) ;
      
      corners[3][0] = XDPixel2Double( corn[3][0] ) ;
      corners[3][1] = YDPixel2Double( corn[3][1] ) ;
    }
  }
  else
  {
    /* NULL because we don't need the position of each string */
    getStringsPositions( sciGetText( pText )     ,
                         &fontId                 ,
                         &fontSize               ,
                         position                ,
                         sciGetAutoSize( pText ) ,
                         textSize                ,
                         sciGetCenterPos( pText ),
                         angle                   ,
                         NULL                    ,
                         cornPix                  ) ;
    
    if ( corners != NULL )
    {
      /* take everything back to user coordinates */
      /* to retrieve exactly the first corner as in stringl we take the input */
      corners[0][0] = XDPixel2Double( cornPix[0][0] ) ;
      corners[0][1] = YDPixel2Double( cornPix[0][1] ) ;
      
      corners[1][0] = XDPixel2Double( cornPix[1][0] ) ;
      corners[1][1] = YDPixel2Double( cornPix[1][1] ) ;
      
      corners[2][0] = XDPixel2Double( cornPix[2][0] ) ;
      corners[2][1] = YDPixel2Double( cornPix[2][1] ) ;
      
      corners[3][0] = XDPixel2Double( cornPix[3][0] ) ;
      corners[3][1] = YDPixel2Double( cornPix[3][1] ) ;
    }
  }

}
/*-------------------------------------------------------------------------------------*/
/**
 * Compute the axis aligned bounding box of a text object.
 * @param pText   Pointer on the object to surround.
 * @param rectPix Bounding box (x,y,w,h) in pixel coordinates. If NULL, not computed.
 *                (x,y) is the coordinates of the bottom left vertice.
 * @param conrPix The four corners of the real bounding box. If NULL, not computed.
 */
void getTextAabb( sciPointObj * pText        ,
                  int           rectPix[4]   ,
                  int           cornPix[4][2] )
{
  if ( cornPix == NULL )
  {
    int cornPix[4][2] ;
    
    getTextBoundingBox( pText, cornPix, NULL ) ;
    
    if ( rectPix != NULL )
    {
      rectPix[0] = Min( cornPix[0][0], Min( cornPix[1][0], Min( cornPix[2][0], cornPix[3][0] ) ) ) ;
      /* in pixel the bottom left point has the maximal y value */
      rectPix[1] = Max( cornPix[0][1], Max( cornPix[1][1], Max( cornPix[2][1], cornPix[3][1] ) ) ) ;
      rectPix[2] = Max( abs( cornPix[2][0] - cornPix[0][0] ), abs( cornPix[3][0] - cornPix[1][0] ) ) ;
      rectPix[3] = Max( abs( cornPix[2][1] - cornPix[0][1] ), abs( cornPix[3][1] - cornPix[1][1] ) ) ;
    }
  }
  else
  {
     getTextBoundingBox( pText, cornPix, NULL ) ;

     if ( rectPix != NULL )
     {
       rectPix[0] = Min( cornPix[0][0], Min( cornPix[1][0], Min( cornPix[2][0], cornPix[3][0] ) ) ) ;
       /* in pixel the bottom left point has the maximal y value */
       rectPix[1] = Max( cornPix[0][1], Max( cornPix[1][1], Max( cornPix[2][1], cornPix[3][1] ) ) ) ;
       rectPix[2] = Max( abs( cornPix[2][0] - cornPix[0][0] ), abs( cornPix[3][0] - cornPix[1][0] ) ) ;
       rectPix[3] = Max( abs( cornPix[2][1] - cornPix[0][1] ), abs( cornPix[3][1] - cornPix[1][1] ) ) ;
     }
  }

}
/*-------------------------------------------------------------------------------------*/
/**
 * return the bounding rectangle of a string.
 * @param string bounded string
 * @param center Position where the string ought to be displayed.
 * @param rect   vector containing the upper left point and the width and height of the
 *               bounding box : [x,y,w,h].
 */
void getStringBbox( char * string, int center[2], int rect[4] )
{
  C2F(dr)("xstringl",string,&(center[0]),&(center[1]),rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,9L,bsiz);
}
/*-------------------------------------------------------------------------------------*/
/**
 * returns the minimal space between too consecutive strings horizontally.
 */
int getStringHorizontalSpace( void )
{
  int rect[4] ;
  int pos[2] = {0,0} ;
  /* return the width of a space */
  getStringBbox( " ", pos, rect ) ;
  return rect[2] ;
}
/*-------------------------------------------------------------------------------------*/
/**
 * returns the minimal space between too consecutive strings vertically.
 */
int getStringVerticalSpace( void )
{
  int rect[4] ;
  int pos[2] = {0,0} ;
  getStringBbox( " ", pos, rect ) ;
  return rect[3] ;
}
/*-------------------------------------------------------------------------------------*/
/**
 * Compute an array of cells in which a set of strings can be displayed. The size of the
 * array is automatically computed using the current font.
 * This routine does not compute any rotation.
 * @param strMat Set of strings.
 * @param textPos Position of the upper left point of the string array.
 * @param stringPosition matrix containing the corners of the cells. Element (i,j) of this
 *                       matrix correspond to the upper-left point of the string (i,j).
 *                       It size must be equal to the size of strMat plus one in each
 *                       dimension. If NULL, not computed.
 * @param boundingBox position of the four corners of the bounding box surrounding the
 *                    array of cells.
 */
void getStringsRect( StringMatrix  * strMat            ,
                     int             textPos[2]        ,
                     Vect2iMatrix  * stringPosition    ,
                     int             boundingBox[4][2]  )
{
  /* the space size between two consecutive strings in a row */
  int hSpace = getStringHorizontalSpace() ;
  int vSpace = getStringVerticalSpace()   ;
  int * rowHeight ; /* vectors containing height and width of column of the array*/
  int * colWidth  ;

  int i ;
  int j ;
  int nbRow = getMatNbRow( strMat ) ;
  int nbCol = getMatNbCol( strMat ) ;
  int rect[4] ;
  int curHeight = 0 ; /* height in pixels of the current row */
  int curWidth  = 0 ; /* lenght of the longest row in pixels */


  /* initialize arrays */
  rowHeight = MALLOC( (nbRow + 1) * sizeof( int ) ) ;
  colWidth  = MALLOC( (nbCol + 1) * sizeof( int ) ) ;

  /* the tricky part */

  /* we first compute the width of each row  of the array */
  /* The width is given by the tallest string of each row */

  rowHeight[nbRow] = textPos[1] ;

  /* begin with the lower left string which is at position textPos */
  for ( i = nbRow - 1 ; i >= 0 ; i-- )
  {
    for ( j = 0 ; j < nbCol ; j++ )
    {
      getStringBbox( getStrMatElement( strMat, i, j ), textPos, rect ) ;
      curHeight = Max( curHeight, rect[3] ) ;
    }
    /* the height of the current column is the max of the height its tallest string + vSpace */
    rowHeight[i] = rowHeight[i+1] - curHeight - vSpace ;
    curHeight = 0 ;
  }

  /* same for columns */
  colWidth[0] = textPos[0] ;

  for ( j = 0 ; j < nbCol ; j++ )
  {
    for ( i = nbRow - 1 ; i >= 0 ; i-- )
    {
      getStringBbox( getStrMatElement( strMat, i, j ), textPos, rect ) ;
      curWidth = Max( curWidth, rect[2] ) ;
    }
    colWidth[j+1] = colWidth[j] + curWidth + hSpace ;
    curWidth = 0 ;
  }

  /* now fill the matrix */
  if ( stringPosition != NULL )
  {
    for ( i = 0 ; i <= nbRow ; i++ )
    {
      for ( j = 0 ; j <= nbCol ; j++ )
      {
        int intersect[2] = { colWidth[j], rowHeight[i] } ;
        copyVect2iMatElement( stringPosition, i, j, intersect ) ;
      }
    }
  }
  
  /* get the bounding box */
  boundingBox[0][0] = textPos[0] ;
  boundingBox[0][1] = textPos[1] ;
  
  boundingBox[1][0] = boundingBox[0][0] ;
  boundingBox[1][1] = rowHeight[0] ;
  
  boundingBox[2][0] = colWidth[nbCol] ;
  boundingBox[2][1] = boundingBox[1][1] ;

  boundingBox[3][0] = boundingBox[2][0] ;
  boundingBox[3][1] = boundingBox[0][1] ;

  FREE( rowHeight ) ;
  FREE( colWidth  ) ;
  
}
/*-------------------------------------------------------------------------------------*/
/**
 * Check if a string can be displayed in a cell with a certain width anf height.
 */
BOOL isFittingInCell( char * string, int cellWidth, int cellHeight )
{
  int rect[4] ;
  int pos[2] = {0,0} ;

  /* get the size of the string */
  getStringBbox( string, pos, rect ) ;

  if ( rect[2] < cellWidth && rect[3] < cellHeight )
  {
    return TRUE ;
  }
  return FALSE ;
}
/*-------------------------------------------------------------------------------------*/
/**
 * get the largest font which can be used to fit the strings in the array.
 * @param strMat Set of strings which try to fit in stringPosition.
 * @param position of the cells in which the strings need to fit.
 */
int computeSuitableFont( StringMatrix  * strMat, Vect2iMatrix  * stringPosition )
{
  int nbRow = getMatNbRow( strMat ) ;
  int nbCol = getMatNbCol( strMat ) ;
  int largestFont  ;
  int smallestFont ;
  int i ;
  int j ; 
  integer curFont[2]  ;
  integer verbose = 0 ;
  integer v ;

   C2F(dr)("xget","font",&verbose,curFont,&v,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,5L) ;
  
  /* the largest font is the minimum of all largest font which can be used for each cells */
  /* initializeit with the largest font the driver can display */
  C2F(dr)( "xfontmxs","", &smallestFont, &largestFont, PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L ) ;

  /* set the largest font */
  C2F(dr)("xset","font",&(curFont[0]),&largestFont,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  for ( i = 0 ; i < nbRow ; i++ )
  {
    for ( j = 0 ; j < nbCol ; j++ )
    {
      int  * blCorner = getVect2iMatElement( stringPosition, i    , j     ) ;
      int  * urCorner = getVect2iMatElement( stringPosition, i + 1, j + 1 ) ;
      char * string   = getStrMatElement( strMat, i, j ) ;
      int width  = urCorner[0] - blCorner[0] ;
      int height = urCorner[1] - blCorner[1] ;
      while ( !isFittingInCell( string, width, height ) )
      {
        largestFont-- ;
        if ( largestFont == smallestFont )
        {
          /* we reach the minimum size, no need to continue */
          C2F(dr)("xset","font",&curFont[0],&curFont[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L) ; 
          return smallestFont ;
        }
        /* set the new font */
        C2F(dr)("xset","font",&(curFont[0]),&largestFont,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
    }
  }
  
  /* return to the previous font */
  C2F(dr)("xset","font",&curFont[0],&curFont[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
  return largestFont ;

}
/*-------------------------------------------------------------------------------------*/
/**
 * Compute an array of cells in which a set of strings can be displayed when the size
 * (width and height) of the array are specified by user. This routine does not compute
 * any rotation.
 * @param strMat Set of strings.
 * @param textPos Position of the upper left point of the string array.
 * @param stringPosition matrix containing the corners of the cells. Element (i,j) of this
 *                       matrix correspond to the upper-left point of the string (i,j).
 *                       It size must be equal to the size of strMat plus one in each
 *                       dimension. If NULL, not computed.
 * @param boundingBox position of the four corners of the bounding box surrounding the
 *                    array of cells.
 * @param userSize width and height of the array of cells.
 * @param newFontSize font size which would fit the array best using the strings in strMat.
 */
void getStringsRectSized( StringMatrix  * strMat           ,
                          int             textPos[2]       ,
                          Vect2iMatrix  * stringPosition   ,
                          int             boundingBox[4][2],
                          int             userSize[2]      ,
                          int           * newFontSize       )
{
  integer curFont[2]  ;
  integer verbose = 0 ;
  integer v           ;
  integer fontSizeOne = 1 ;
  double homothFactors[2] ;
  
  /* first get the automatic array with font 1 */

  /* get the current font */
  /* we need to change the defaut font before using xstringl */
  C2F(dr)("xget","font",&verbose,curFont,&v,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,5L);

  /* set the new font */
  C2F(dr)("xset","font",&(curFont[0]),&fontSizeOne,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  
  /* get the displaying array */
  getStringsRect( strMat, textPos, stringPosition, boundingBox ) ;

  /* get the homothety factors */
  /* horizontal : userWidth / bbWidth */
  homothFactors[0] = ((double) userSize[0]) / ( boundingBox[2][0] - boundingBox[0][0] ) ;
  homothFactors[1] = ((double) userSize[1]) / ( boundingBox[0][1] - boundingBox[2][1] ) ;

  /* apply homothety on the matrix and boundingbox */
  iHomothety2D( boundingBox[1], boundingBox[0], homothFactors, boundingBox[1] ) ;
  iHomothety2D( boundingBox[2], boundingBox[0], homothFactors, boundingBox[2] ) ;
  iHomothety2D( boundingBox[3], boundingBox[0], homothFactors, boundingBox[3] ) ;

  /* we must tkae the min from both direction to avoid strings to go over the lines. */
  if ( stringPosition != NULL )
  {
    homothVect2iMatrix( stringPosition, boundingBox[0], homothFactors ) ;
    *newFontSize = computeSuitableFont( strMat, stringPosition ) ;
  }
  
  /* return to the previous font */
  C2F(dr)("xset","font",&curFont[0],&curFont[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

}
/*-------------------------------------------------------------------------------------*/
/**
 * Rotate four points around the first one.
 * @param boundingBox the four 2D points.
 * @param angle rotation angle in radian.
 */
void rotateBoundingBox( int boundingBox[4][2], int center[2], double angle )
{
  if ( Abs( angle ) > EPSILON )
  {
    double cosAngle = cos( angle ) ;
    double sinAngle = sin( angle ) ;
    iRotate2Dim( boundingBox[0], center, cosAngle, sinAngle, boundingBox[0] ) ;
    iRotate2Dim( boundingBox[1], center, cosAngle, sinAngle, boundingBox[1] ) ;
    iRotate2Dim( boundingBox[2], center, cosAngle, sinAngle, boundingBox[2] ) ;
    iRotate2Dim( boundingBox[3], center, cosAngle, sinAngle, boundingBox[3] ) ;
  }
}
/*-------------------------------------------------------------------------------------*/
/**
 * Translae four points around the first one.
 * @param boundingBox the four 2D points.
 * @param angle rotation angle in radian.
 */
void translateBoundingBox( int boundingBox[4][2], int trans[2] )
{
  iTranslate2D( boundingBox[0], trans, boundingBox[0] ) ;
  iTranslate2D( boundingBox[1], trans, boundingBox[1] ) ;
  iTranslate2D( boundingBox[2], trans, boundingBox[2] ) ;
  iTranslate2D( boundingBox[3], trans, boundingBox[3] ) ;
    
}
/*-------------------------------------------------------------------------------------*/
/**
 * Get the translation which should be applied to the text depending on the meaning of
 * its position.
 * @param centeredPos specify the meaning of the position of the text. If FALSE, the
 *                    position corresponds to the position of the lower left point. So
 *                    nothing needs to be changed. If TRUE, the center is the position
 *                    corresponds to the center of the text bounding box and then a
 *                    translation must be performed.
 * @param textSize width and height wanted for the text.
 * @param bbox The four corners of the computed boundign box of the text. The first corner
 *             should correspond to the position of the text.
 * @param trans Translation to apply to the text.
 */
void getStringPositionTranslation( BOOL centeredPos, int textSize[2], int bbox[4][2], int trans[2] )
{
  if ( centeredPos )
  {
    trans[0] = ( textSize[0] + bbox[0][0] - bbox[2][0] ) / 2 ;
    trans[1] = ( -textSize[1] + bbox[0][1] - bbox[2][1] ) / 2 ;
  }
  else
  {
    trans[0] = 0 ;
    trans[1] = 0 ;
  }
}
/*-------------------------------------------------------------------------------------*/
/**
 * Compute an array of cells in which each string will fit. The bounding box of the
 * finally displayed string is also computed. The generated array of cells can the be used
 * for the display with the drawStringsInPosition routine.
 * @param strMat    Set of string.
 * @param fontId    specify the fontId of the strings.
 * @param fontSize  if autoSize is on then specify the size of the strings. Otherwise,
 *                  this returns the size which should be used to display the strings in
 *                  the cell array.
 * @param textPos   Position of the upper left point of the string array and center
 *                  of rotation.
 * @param autoSize  If TRUE, the size of the text is displayed usign the specified font.
 *                  If flase the width and height of the text are specified by the user.
 * @param textSize  if autoSize is false, specify the size of the rectangle in which the
 *                  text must fit.
 * @param angle     Angle for the rotation of the strings.
 * @param centerPos Specify where is the rotation center of the string. If TRUE in the middle
 *                  of the array, otherwise in the bottom-left.
 * @param stringPosition matrix containing the corners of the cells. Element (i,j) of this
 *                       matrix correspond to the upper-left point of the string (i,j).
 *                       It size must be equal to the size of strMat plus one in each
 *                       dimension. If NULL, not computed.
 * @param boundingBox position of the four corners of the bounding box surrounding the
 *                    array of cells. The points are given clockwise.
 */
void getStringsPositions( StringMatrix  * strMat        ,
                          int           * fontId        ,
                          int           * fontSize      ,
                          int             textPos[2]    ,
                          BOOL            autoSize      ,
                          int             textSize[2]   ,
                          BOOL            centerPos     ,
                          double          angle         ,
                          Vect2iMatrix  * stringPosition,
                          int             boundingBox[4][2] )
{
  integer curFont[2]  ;
  integer verbose = 0 ;
  integer v           ;
  int trans[2] ;

  if ( autoSize )
  {
    /* get the current font */
    /* we need to change the defaut font before using xstringl */
    C2F(dr)("xget","font",&verbose,curFont,&v,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,5L);

    /* set the new font */
    C2F(dr)("xset","font",fontId,fontSize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

    /* compute the stringPosition and bounding box in pixels without rotation */
    getStringsRect( strMat, textPos, stringPosition, boundingBox ) ;

    /* return to the previous font */
    C2F(dr)("xset","font",&curFont[0],&curFont[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  else
  {
    getStringsRectSized( strMat, textPos, stringPosition, boundingBox, textSize, fontSize ) ;
  }

  /* for now we have computes the matrix as if its position was corresponding to its */
  /* lower-left vertice. */
  /* we must now translate the points depending on where is really the center relatively */
  /* to the array. */
  getStringPositionTranslation( centerPos, textSize, boundingBox, trans ) ;
  if ( stringPosition != NULL )
  {
    translateVect2iMatrix( stringPosition, trans ) ;
  }
  translateBoundingBox( boundingBox, trans ) ;

  /* then turn everything, we need to turn them in pixels because of logarithmic scale */
  if ( Abs( angle ) > EPSILON )
  {
    if ( stringPosition != NULL )
    {
      rotateVect2iMatrix( stringPosition, textPos, angle ) ;
    }
    rotateBoundingBox( boundingBox, textPos, angle ) ;
  }

}
/*-------------------------------------------------------------------------------------*/
/**
 * Inside a cell, return the margin to be used for the display of a string inside a cell.
 * @param stringSize width and height of the string wich will be displayed.
 * @param blCorner   the bottom left corner of the cell. The cell must not be turned
 *                   (ie axes aligned).
 * @param urCorner   the upper right corner of the cell.
 * @param align      specify the position of the string inside the cell.
 * @param lMargin    left margin margin (horizontal).
 * @param bMargin    bottom mergin (vertical).
 */
void getStringMargins( int                stringSize[2],
                       int                blCorner[2]  ,
                       int                urCorner[2]  ,
                       sciTextAlignment   align        ,
                       int              * lMargin      ,
                       int              * bMargin       )
{
  int cellWidth  = urCorner[0] - blCorner[0] ;
  int cellHeight = urCorner[1] - blCorner[1] ;

  switch( align )
  {
  case ALIGN_LEFT:
    *lMargin = 0 ;
    break ;
  case ALIGN_RIGHT:
    *lMargin = cellWidth - stringSize[0] ;
    break ;
  case ALIGN_CENTER:
    /* same margin for each side */
    *lMargin = ( cellWidth - stringSize[0] ) / 2 ;
    break ;
  default:
    sciprint("error unhandled alignment\n");
    *lMargin = -1 ;
    *bMargin = -1 ;
    break ;
  }
  *bMargin = ( cellHeight - stringSize[1] ) / 2 ;
  
}
/*-------------------------------------------------------------------------------------*/
/**
 * Draw a string inside a box. The box can be rotated.
 * @param string displayed string
 * @param blCorner  Bottom left corner of the string.
 * @param urCorner  Upper right corner of the string.
 * @param rotCenter Rotation center.
 * @param angle     Rotation angle in radian.
 * @param align     Position of the string inside the box.
 */
void drawStringInCell( char             * string      ,
                       int                blCorner[2] ,
                       int                urCorner[2] ,
                       int                rotCenter[2],
                       double             angle       ,
                       sciTextAlignment   align        )
{
  int rect[4] ;
  int leftMargin ;
  int bottomMargin ;
  int pos[2] ;
  int flag = 0 ;
  double angleStr = RAD2DEG( angle ) ;
  /* get the size of the string */
  getStringBbox( string, blCorner, rect ) ;
  
  getStringMargins( &(rect[2]), blCorner, urCorner, align, &leftMargin, &bottomMargin ) ;

  /* get the position unturned */
  pos[0] = blCorner[0] + leftMargin   ;
  pos[1] = blCorner[1] + bottomMargin + rect[3] ;

  /* then turn it around the center */
  if ( Abs( angle ) > EPSILON )
  {
    iRotate2D( pos, rotCenter, angle, pos ) ;
  }
  /* then display it */
  C2F(dr)("xstring",string,&(pos[0]),&(pos[1]),PI0,&flag,PI0,PI0,&angleStr, PD0,PD0,PD0,0L,0L);
}                       

/*-------------------------------------------------------------------------------------*/
/**
 * Given a matrix of strings and an array of cells positions, draw each strings in
 * the corresponding cell. The array of cells must not been turned, this function
 * manage the rotation itself. The alignment is also respected.
 * @param strings     set of strings to display.
 * @param cellsArray  position of the cells in which the strings are dispalyed.
 * @param center      position of the center of the string array around which the rotation
 *                    is done.
 * @param angle       rotation angle in radian of the displayed matrix around its
 *                    lower left point.
 * @param align       position of strings inside their cells.
 */
void drawStringsInPosition( StringMatrix     * strings    ,
                            Vect2iMatrix     * cellsArray ,
                            int                center[2]  ,
                            double             angle      ,
                            sciTextAlignment   align       )
{
  int nbRow = getMatNbRow( strings ) ;
  int nbCol = getMatNbCol( strings ) ;
  int i ;
  int j ;
  
  for ( i = 0 ; i < nbRow ; i++ )
  {
    for ( j = 0 ; j < nbCol ; j++ )
    {
      /* get the two extreme corners of the cell in which the text will be drawn */
      int  * blCorner = getVect2iMatElement( cellsArray, i    , j     ) ;
      int  * urCorner = getVect2iMatElement( cellsArray, i + 1, j + 1 ) ;
      char * string   = getStrMatElement( strings, i, j ) ;
      drawStringInCell( string, blCorner, urCorner, center, angle, align ) ;
    }
  }
}
/*-------------------------------------------------------------------------------------*/
/**
 * Routine which draw a text object
 * @param pObj the pointer on the text object
 */
void drawText( sciPointObj * pObj )
{
  int    v  = 0  ;
  double dv = 0. ;
  double anglestr = 0. ;
  int textProperties[6] ;
  int verb=0;
  int font_[2], cur_font_[2] ;
  int position[2] ;
  int textDim[2] ;
  double userSize[2] ;
  int    textSize[2] ;
  double textPos[3] ;
  Vect2iMatrix * bboxes ;
  int globalBbox[4][2] ;
  sciText * ppText =  pTEXT_FEATURE( pObj ) ;
  sciPointObj * parentSW = sciGetParentSubwin(pObj) ;
  
  if (!sciGetVisibility(pObj))
  {
    return ;
  }
  
  
  
#ifdef _MSC_VER
  flag_DO = MaybeSetWinhdc ();
#endif
  if ( ppText->isclip )
  {
    sciClip(pObj);
  }

  /* transform the position in pixels */
  /* we don't take the axes reverse into account. This has obviously no meaning for text.*/
  textPos[0] = ppText->x ;
  textPos[1] = ppText->y ;
  textPos[2] = ppText->z ;
  
  if ( sciGetIs3d( pObj ) )
  {
    /* normal case */
    getPixelCoordinates( parentSW, textPos, position ) ;
  }
  else
  {
    /* for labels, the text is displayed using 2d scale. */
    position[0] = XDouble2Pixel( textPos[0] ) ;
    position[1] = YDouble2Pixel( textPos[1] ) ;
  }


  sciGetUserSize( pObj, &(userSize[0]), &(userSize[1]) ) ;
  
  
  /* We take the size in 2d. */
  textSize[0] = PixelWidth2d(  parentSW, ppText->x, userSize[0] ) ;
  textSize[1] = PixelHeight2d( parentSW, ppText->y, userSize[1] ) ;
  
  /* *10 parce que l'angle est conserve en 1/10eme de degre*/
  /* si c'est pas nimp tout ca.... */
  anglestr = (sciGetFontOrientation (pObj)/10); 	
  
  /* set the font */
  C2F(dr)("xget","font",&verb,cur_font_,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);
  
  font_[0] = sciGetFontStyle (pObj);
  font_[1] = sciGetFontDeciWidth (pObj)/100;
  
  C2F(dr)("xset","font",&font_[0],&font_[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  
  /* get the bounding box of the text matrix */
  /* the matrix with the bounding box of each strings in the text object */
  sciGetTextSize( pObj, &(textDim[0]), &(textDim[1]) ) ;
  bboxes = newMatrix( textDim[0] + 1, textDim[1] + 1 ) ;

  
  /* we get the array not turned because the display will turn everything by itself */
  /* However, the bounding box needs to be turned after */
  getStringsPositions( sciGetText( pObj )     ,
                       &font_[0]              ,
                       &font_[1]              ,
                       position               ,
                       sciGetAutoSize( pObj ) ,
                       textSize               ,
                       sciGetCenterPos( pObj ),
                       0.0                    ,
                       bboxes                 ,
                       globalBbox              ) ;
  
  /* font might have changed */
  C2F(dr)("xset","font",&font_[0],&font_[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    
  /* wether or not we draw and/or fill the box */
  /* no need to compute anything if both line_mode */
  /* and fill mode are false */
  /* Of course, the background should be drawn first */
  if(    sciGetIsBoxed (pObj)
         && ( sciGetIsFilled( pObj ) || sciGetIsLine( pObj ) ) )
  {
    int xm[4], ym[4],n=4;
    int close=1;
      
    /* we need to rotate the bounding box */
    rotateBoundingBox( globalBbox, position,  DEG2RAD(anglestr) ) ;

    xm[0] = globalBbox[0][0] ;
    xm[1] = globalBbox[1][0] ;
    xm[2] = globalBbox[2][0] ;
    xm[3] = globalBbox[3][0] ;

    ym[0] = globalBbox[0][1] ;
    ym[1] = globalBbox[1][1] ;
    ym[2] = globalBbox[2][1] ;
    ym[3] = globalBbox[3][1] ;

      
            
    /* draw the background */
    if ( sciGetIsFilled( pObj ) )
    {
      textProperties[0] = sciGetBackground( pObj ) ;
      C2F (dr) ("xset", "dashes", textProperties, textProperties, textProperties+3, textProperties+3, textProperties+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", textProperties, textProperties, textProperties+3, textProperties+3, textProperties+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
              
      C2F (dr) ("xarea", "xv", &n, xm, ym, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen("xv"));
    }

    /* draw the line around the box */
    if ( sciGetIsLine( pObj ) )
    {
      textProperties[0] = sciGetForeground( pObj ) ;
      /* draw a rectangle around the text */
      C2F (dr) ("xset", "dashes", textProperties, textProperties, textProperties+3, textProperties+3, textProperties+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", textProperties, textProperties, textProperties+3, textProperties+3, textProperties+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
              
      C2F (dr) ("xlines", "xv", &n, xm, ym, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
    }
  }
          
#ifdef _MSC_VER
  flag_DO = MaybeSetWinhdc ();
#endif
  /* load the object foreground and dashes color */
  textProperties[0] = sciGetFontForeground (pObj);/*la dash est de la meme couleur que le foreground*/
  textProperties[2] = sciGetFontDeciWidth (pObj)/100;
  textProperties[3] = 0 ;
  textProperties[4] = sciGetFontStyle(pObj);
  textProperties[5] = 0 ;
          
  C2F (dr) ("xset", "dashes", textProperties, textProperties, textProperties+3, textProperties+3, textProperties+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
  C2F (dr) ("xset", "foreground", textProperties, textProperties, textProperties+3, textProperties+3, textProperties+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
  /* C2F(dr)("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L); */
          
#ifdef _MSC_VER
  if ( flag_DO == 1) ReleaseWinHdc ();
#endif
    
  drawStringsInPosition( sciGetText( pObj ), bboxes,position, DEG2RAD(anglestr), sciGetAlignment( pObj ) ) ;
  /* C2F(dr)("xstring",getStrMatElement(sciGetText(pObj),0,0),&x1,&yy1,PI0,&flagx,PI0,PI0,&anglestr, PD0,PD0,PD0,0L,0L); */


  C2F(dr)("xset","font",&cur_font_[0],&cur_font_[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    
  deleteMatrix( bboxes ) ;

  if ( ppText->isclip )
  {
    sciUnClip(pObj);
  }
        
#ifdef _MSC_VER
  if ( flag_DO == 1) ReleaseWinHdc ();
#endif

}
/*------------------------------------------------------------------------------------------*/
/**
 * Give the position of the 4 corners of a rectangle in pixels given the position of its
 * upper left point in user coordiantes and its size. It works in every axes mode
 * (2D/3D, log, reverse).
 * @param parentSubWin Subwindow in which the rectangle is drawn.
 * @param ulPoint      Coordinates of the upper left point of the rectangle in user coord.
 * @param userSize     Width and height of the rectangle in user coord.
 * @param edgesX       X coordinates of the resulting positions.
 * @param edgesY       Y coordinates of the resulting positions.
 */
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
 * Update the coordinates of a label to put it in the middle of a segment (normally an axis)
 * but with a little orthogonal offset.
 * Let Ri i=0:3 be the four corners of the label boudinbg box. The algorithm will put the
 * center of the bounding box C on the middle of the segment I. Then we compute for each corner
 * the needed translation to put them at a distance given by offset. The translation is done
 * along an axis orthogonal to the segment. Finally we transalte the box from I with the longest
 * translation, so each corner is at least at the wanted distance.
 * @param pLabel pointer to the label to place.
 * @param axisStart Starting point of the segment.
 * @param axisEnd   The other end of the segment. The order of the two points determine the
 *                  side in which the label will be placed.
 * @param offsets   The label is not put exactly on the middle of the segment. It is moved
 *                  from a little offset orthogonally from the segment.
 */
void computeLabelAutoPos( sciPointObj * pLabel, int axisStart[2], int axisEnd[2], int offsets[2] )
{
  /* computation are done in double to avoid loss of data */
  double centerDir[2]  ; /* give the direction of the line on which is the center. */
                         /* it is orthogonal with the axis direction */
  double axisMiddle[2] ; /* middle of the axis = ( axisStart + axisEnd ) / 2 */
  int    bbox[4][2]    ; /* real bounding box of the label */
  double corners[4][2] ; /* the four corners of the boundign box */
  double centerDist    ; /* final distance between the center and the axis */
  int wantedBlPoint[2] ; /* the position we want for the bottom left point of the axis */
  double distance      ;
  double rectCenter[2] ;

  if ( axisStart[0] == axisEnd[0] && axisStart[1] == axisEnd[1] )
  {
    /* the axis is invisible. Its label is not displayed also. */
    return ;
  }

  /* normalized vector orthogonal to the axe */
  /* its direction (+ or -) determines on which side will be displayed the axis */
  centerDir[0] = axisStart[1] - axisEnd[1]   ;
  centerDir[1] = axisEnd[0]   - axisStart[0] ;
  normalize2d( centerDir ) ;

  /* the distance between the aabb and the axis is offsets[0] if the axis is vertical, */
  /* offest[1] if horizontal and something in between otherwise */
  distance = Abs( (double)offsets[0] * centerDir[0] ) + Abs( (double)offsets[1] * centerDir[1] ) ;

  axisMiddle[0] = ( axisStart[0] + axisEnd[0] ) / 2.0 ;
  axisMiddle[1] = ( axisStart[1] + axisEnd[1] ) / 2.0 ;
  
  /* get the size of the label axis aligned bouding box. */
  getTextBoundingBox( pLABEL_FEATURE(pLabel)->text, bbox, NULL ) ;

  /* get the center of the rectangle */
  rectCenter[0] = ( bbox[0][0] + bbox[1][0] + bbox[2][0] + bbox[3][0] ) / 4.0 ;
  rectCenter[1] = ( bbox[0][1] + bbox[1][1] + bbox[2][1] + bbox[3][1] ) / 4.0 ;

  /* We suppose the box is centered on I and we compute IRi, i=0:3 */
  corners[0][0] = bbox[0][0] - rectCenter[0] ;
  corners[0][1] = bbox[0][1] - rectCenter[1] ;
  corners[1][0] = bbox[1][0] - rectCenter[0] ;
  corners[1][1] = bbox[1][1] - rectCenter[1] ;
  corners[2][0] = bbox[2][0] - rectCenter[0] ;
  corners[2][1] = bbox[2][1] - rectCenter[1] ;
  corners[3][0] = bbox[3][0] - rectCenter[0] ;
  corners[3][1] = bbox[3][1] - rectCenter[1] ;

  /* compute IRi.v, i=0:3 to get the current distance between Ri and the axis. */
  /* Then for each Ri we can compute the need displacment along the centerDir to push it */
  /* at distance dist of the axis. Finally, the needed displacement is the maximum of the 4. */
  centerDist = distance - DOT_PROD_2D( corners[0], centerDir ) ;
  centerDist = Max( centerDist, distance - DOT_PROD_2D( corners[1], centerDir ) ) ;
  centerDist = Max( centerDist, distance - DOT_PROD_2D( corners[2], centerDir ) ) ;
  centerDist = Max( centerDist, distance - DOT_PROD_2D( corners[3], centerDir ) ) ;

  /* the wanted center is C = I + a.v */
  wantedBlPoint[0] = round( axisMiddle[0] + centerDist * centerDir[0] + corners[0][0] ) ;
  wantedBlPoint[1] = round( axisMiddle[1] + centerDist * centerDir[1] + corners[0][1] ) ;

  sciSetPosition( pLabel,
                  XPixel2Double( wantedBlPoint[0] ),
                  YPixel2Double( wantedBlPoint[1] ) ) ;

}
/*------------------------------------------------------------------------------------------*/

#undef round
