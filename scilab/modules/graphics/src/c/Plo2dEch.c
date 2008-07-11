/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2000 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2002 - INRIA - Serge Steer
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
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
 --------------------------------------------------------------------------*/

#include <string.h> /* in case of dbmalloc use */
#include <stdio.h>
#include <math.h>
#include "math_graphics.h" 
#include "PloEch.h"

#include "GetProperty.h"
#include "SetProperty.h"
#include "BuildObjects.h"
#include "Interaction.h"
#include "DrawObjects.h"
#include "Plo2dEch.h"
#include "PloEch.h"
#include "Vertices.h"
#include "GraphicZoom.h"
#include "axesScale.h"
#include "sciprint.h"
#include "Format.h"
#include "scirun.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"
#include "dynamic_menus.h" /* StoreCommand */
#include "Axes.h"
#include "GraphicSynchronizerInterface.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"

extern void xgetmouse2(char *fname, char *str, integer *ibutton, integer *iflag, integer *x1, integer *yy1, integer *x6, integer *x7, double *x, double *y, double *dx3, double *dx4, integer lx0, integer lx1);
extern void xclick_2(char *fname, char *str, integer *ibutton, integer *iflag, integer *istr, integer *x1, integer *yy1, integer *x7, double *x, double *y, double *dx3, double *dx4, integer lx0, integer lx1);

/*----------------------------------------------
 * A List for storing Window scaling information 
 *----------------------------------------------*/

static void scale_copy (WCScaleList *s1, WCScaleList *s2);
static integer curwin (void);
static void set_scale_win (ScaleList **listptr, integer i, WCScaleList *s);
static WCScaleList *new_wcscale ( WCScaleList *val);
static WCScaleList *check_subwin_wcscale (WCScaleList *listptr, double *);
static int same_subwin (double lsubwin_rect[4],double subwin_rect[4]);
static void set_window_scale (integer i,WCScaleList  *scale);
int C2F(setscale2d)(double WRect[4],double FRect[4], char * logscale, integer l1) ;
void get_margin_in_pixel(integer Margin[]) ;
int C2F(xechelle2d)(double x[], integer x1[], integer *  n1,char dir[],integer lstr) ;
int C2F(yechelle2d)(double y[], integer yy1[], integer * n2,char dir[], integer lstr) ;
int zoom_get_rectangle(double *bbox,int *x_pixel, int *y_pixel) ;
void Gr_Rescale(char *logFlags, double *FRectI, integer *Xdec, integer *Ydec, integer *xnax, integer *ynax) ;

/* The scale List : one for each graphic window */

static ScaleList *The_List = NULL;

/* Current Scale */

WCScaleList  Cscale = 
  { 
    0,
    {600,400},
    {0.0,0.0,1.0,1.0},
    {0.0,0.0,1.0,1.0},
    {1/8.0,1/8.0,1/8.0,1/8.0},
    {0.0,1.0,0.0,10},
    {0.0,1.0,0.0,10},
    75.0,53.0,450.0,318.0,
    {'n','n'},
    {75,53,450,318},
    {2,10,2,10},
    {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}},
    {0.0,1.0,0.0,1.0,0.0,1.0},
    35.0,45.0,
    1,                 /* added by es */
    (WCScaleList *) 0, /*unused */
    (WCScaleList *) 0  /*unused */  
  };

/** default values **/

WCScaleList  Defscale = 
  { 
    0,
    {600,400},
    {0.0,0.0,1.0,1.0},
    {0.0,0.0,1.0,1.0},
    {1/8.0,1/8.0,1/8.0,1/8.0},
    {0.0,1.0,0.0,10},
    {0.0,1.0,0.0,10},
    75.0,53.0,450.0,318.0,
    {'n','n'},
    {75,53,450,318},
    {2,10,2,10},
    {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}},
    {0.0,1.0,0.0,1.0,0.0,1.0},
    35.0,45.0,
    1,                 /* added by es */
    (WCScaleList *) 0, /*unused */
    (WCScaleList *) 0 /*unused */  
  };

/*----------------------------------------------------------
 * Back to defaults values : fills current scale (Scale)
 * and curwin() scale with default scale.
 *----------------------------------------------------------*/

void Cscale2default( void )
{
  scale_copy(&Cscale,&Defscale);  set_window_scale(curwin(),&Cscale);
}

void set_window_scale_with_default( int i ) { set_window_scale(i,&Defscale);} 

/*------------------------------------------------------------
 * void get_window_scale(i,subwin)
 *   if subwin == NULL : search for existing scale values of window i.
 *   if subwin != NULL : search for existing scale values of subwindow subwin of window i.
 *   If no scale are found we do nothing and return 0
 *   else the scale is copied into current scale Cscale ans we return 1.
 *   
 *-------------------------------------------------------------*/

static int get_scale_win (ScaleList *listptr, integer wi, double *subwin);

int get_window_scale( integer i, double * subwin )
{ 
  return get_scale_win(The_List,Max(0L,i),subwin);
}

static int get_scale_win(ScaleList *listptr,integer wi,double *subwin)
{
  if (listptr != (ScaleList  *) NULL)
    { 
      if ((listptr->Win) == wi)
	{ 
	  if ( subwin == NULL) 
	    {
	      scale_copy(&Cscale,listptr->scales);
	      return 1;
	    }
	  else
	    {
	      WCScaleList *loc = check_subwin_wcscale(listptr->scales,subwin);
	      if ( loc != NULL) 
		{
		  scale_copy(&Cscale,loc);
		  return 1;
		}
	      else 
		{
		  return 0;
		}
	    }
	}
      else 
	return get_scale_win(listptr->next,wi,subwin);
    }
  return 0;
}

/*------------------------------------------------------------
 * void set_window_scale(i,scale)
 * add the current scale at the begining of window i scale list 
 * (which is also modified) making Cscale the current scale of window i 
 *-------------------------------------------------------------*/

static void set_window_scale(integer i,WCScaleList  *scale)
{ 
  set_scale_win(&The_List,Max(0L,i),scale);
}

static void set_scale_win(ScaleList **listptr, integer i, WCScaleList *scale)
{
  if ( *listptr == (ScaleList  *) NULL)
    {
      /* window i does not exist add an entry for it */
      if ((*listptr = (ScaleList *) MALLOC( sizeof (ScaleList)))==0)
	{
		sciprint(_("%s: No more memory.\n"),"set_scale_win");
	  return;
	}
      else 
	{ 
	  (*listptr)->Win  = i;
	  (*listptr)->next = (ScaleList *) NULL ;
	  (*listptr)->scales = new_wcscale(scale);
	  if ( (*listptr)->scales == 0) 
	    {
	      *listptr = 0;
		  sciprint(_("%s: No more memory.\n"),"set_scale_win");
	    }
	  return ;
	}
    }
  if ( (*listptr)->Win == i) 
    { 
      /* try to find a scale for Cscale.subwin_rect for window i */
      WCScaleList *loc = check_subwin_wcscale((*listptr)->scales,scale->subwin_rect); 
      if ( loc != NULL) 
	{
	  /* subwin exists we update it */
	  scale_copy(loc,scale);
	  /* we move loc to the top of the list */
	  if ( loc != (*listptr)->scales )
	    {
	      (loc->prev)->next = loc->next ;
	      if ( loc->next != NULL) (loc->next)->prev = loc->prev ;
	      loc->next = (*listptr)->scales;
	      loc->next->prev = loc ;
	      (*listptr)->scales = loc;
	    }
	}
      else 
	{
	  /* subwin does not exists we add it a the begining of the list */
	  if (( loc = new_wcscale(scale))== NULL)
	    {
		  sciprint(_("%s: No more memory.\n"),"set_scale_win");
	      return ;
	    }
	  else 
	    {
	      loc->next = (*listptr)->scales;
	      if (loc->next != NULL) loc->next->prev = loc;
	      (*listptr)->scales = loc;
	    }
	}
    }
  else 
    set_scale_win( &((*listptr)->next),i,scale);
}
 
static WCScaleList *new_wcscale(WCScaleList *val)
{
  WCScaleList *new ;
  if ((new = (WCScaleList *) MALLOC( sizeof (WCScaleList))) == NULL) 
    return NULL;
  new->next = (WCScaleList *) 0;
  new->prev = (WCScaleList *) 0;
  scale_copy(new,val);
  return new;
}

static WCScaleList *check_subwin_wcscale(WCScaleList *listptr,double subwin_rect[4])
{
  if ( listptr == (WCScaleList  *) NULL)  return NULL;
  if ( same_subwin( listptr->subwin_rect,subwin_rect)) 
    return listptr;
  else 
    return check_subwin_wcscale(listptr->next,subwin_rect);
}

static int same_subwin(double lsubwin_rect[4],double subwin_rect[4])
{
  if ( Abs(lsubwin_rect[0] - subwin_rect[0]) < 1.e-8
       && Abs(lsubwin_rect[1] - subwin_rect[1]) < 1.e-8
       && Abs(lsubwin_rect[2] - subwin_rect[2]) < 1.e-8
       && Abs(lsubwin_rect[3] - subwin_rect[3]) < 1.e-8 ) 
    return 1;
  else 
    return 0;
}

/*------------------------------------------------------------
 * del_window_scale(i)
 * delete the scales associated to window i 
 *-------------------------------------------------------------*/

static void DeleteWCScale (WCScaleList *l);

void del_window_scale( integer i )
{ 
  ScaleList *loc, *loc1;
  /* check head of The_List */
  if ( The_List == NULL) return ;
  if ( The_List->Win == i) 
    {
      loc1= The_List ; 
      The_List = The_List->next;
      DeleteWCScale(loc1->scales);
      FREE(loc1);
      return;
    }
  loc1= The_List;
  loc = The_List->next;
  while ( loc != NULL) 
    {
      if ( loc->Win == i ) 
	{
	  loc1->next = loc->next;
	  DeleteWCScale(loc->scales);
	  FREE(loc);
	  return ;
	}
      else
	{
	  loc1 = loc;
	  loc  = loc->next;
	}
    }
}

static void DeleteWCScale(WCScaleList *l) 
{
  if ( l != NULL) 
    {
      DeleteWCScale(l->next);
      FREE(l);
    }
}

/*-------------------------------------------
 * static void scale_copy(s1,s2) : s1=s2 
 * (only necessary for non ansi C compilers) 
 *-------------------------------------------*/

static void scale_copy( WCScaleList * s1, WCScaleList * s2 ) 
{
  int i,j;
  s1->flag=s2->flag;
  s1->wdim[0]=  s2->wdim[0];
  s1->wdim[1]=  s2->wdim[1];
  for ( i = 0 ; i < 4 ; i++ ) 
  {
    s1->subwin_rect[i]=s2->subwin_rect[i];
    s1->frect[i]=s2->frect[i];
    s1->WIRect1[i]=s2->WIRect1[i];
    s1->Waaint1[i]=s2->Waaint1[i];
    s1->xtics[i]=s2->xtics[i];
    s1->ytics[i]=s2->ytics[i];
    s1->axis[i]=s2->axis[i];
  }
  for ( i = 0 ; i < 3 ; i++ )
  {
    for ( j = 0 ; j < 3 ; j++ )
    {
      s1->m[i][j] = s2->m[i][j] ;
    }
  }
  for (i=0; i< 6; i++ )
  {
    s1->bbox1[i] = s2->bbox1[i] ;
  }
  s1->Wxofset1=s2->Wxofset1;
  s1->Wyofset1=s2->Wyofset1;
  s1->Wscx1=s2->Wscx1;
  s1->Wscy1=s2->Wscy1;
  s1->logflag[0] = s2->logflag[0];
  s1->logflag[1] = s2->logflag[1];
}

/*-------------------------------------------
 * return current window : ok if driver is Rec
 *-------------------------------------------*/

static integer curwin( void )
{
  return sciGetNum(sciGetCurrentFigure());
}

/*-------------------------------------------
 * show the recorded scales 
 *-------------------------------------------*/

static void show_scales (ScaleList *listptr);

void ShowScales( void )
{ 
  sciprint("-----------scales-------------\n");
  show_scales(The_List);
  sciprint("----------current scale-------\n");
  sciprint("\tsubwin=[%5.2f,%5.2f,%5.2f,%5.2f], flag=%d\n",
	   Cscale.subwin_rect[0],Cscale.subwin_rect[1],Cscale.subwin_rect[2],Cscale.subwin_rect[3],
	   Cscale.flag);
  sciprint("-----------end----------------\n");
}

static void show_scales( ScaleList *listptr)
{
  if (listptr != (ScaleList  *) NULL)
    { 
      WCScaleList *loc = listptr->scales;
      sciprint("Window %d\n",listptr->Win);
      while ( loc != NULL) 
	{
	  sciprint("\tsubwin=[%5.2f,%5.2f,%5.2f,%5.2f], flag=%d\n",
		   loc->subwin_rect[0],loc->subwin_rect[1],loc->subwin_rect[2],loc->subwin_rect[3],
		   loc->flag);
	  loc = loc->next ;
	}
      show_scales(listptr->next);
    }
}


/*-------------------------------------------
 * setscale2d 
 * uses WRect,FRect,logscale to update 
 * current scale (Cscale) 
 *  WRect gives the subwindow to use 
 *  FRect gives the bounds 
 *  WRect=[<x-upperleft>,<y-upperleft>,largeur,hauteur]
 *    example WRect=[0,0,1.0,1.0] we use all the window 
 *            WRect=[0.5,0.5,0.5,0.5] we use the down right 
 *            quarter of the window 
 *-------------------------------------------*/

int C2F(setscale2d)(double WRect[4],double FRect[4], char * logscale, integer l1)
{
  static integer aaint[]={2,10,2,10};
  if (logscale[0]=='l') 
    {
      FRect[0]=log10(FRect[0]);
      FRect[2]=log10(FRect[2]);
    }
  if (logscale[1]=='l') 
    {
      FRect[1]=log10(FRect[1]);
      FRect[3]=log10(FRect[3]);
    }
  set_scale("tttftf",WRect,FRect,aaint,logscale,NULL);
  return(0);
}


/*-------------------------------------------
 * setscale2d 
 * uses WRect,ARect,FRect,logscale to update 
 * current scale (Cscale) 
 *  WRect gives the subwindow to use 
 *  ARect gives the axis rectangle 
 *  FRect gives the bounds 
 *  WRect=[<x-upperleft>,<y-upperleft>,largeur,hauteur]
 *    example WRect=[0,0,1.0,1.0] we use all the window 
 *            WRect=[0.5,0.5,0.5,0.5] we use the down right 
 *            quarter of the window
 *  logscale : gives xy log axis flags 
 *  each argument can be a null pointer if they are 
 *  not to be changed from their current value 
 * 
 *  Each window can have a set of scales : one for each specified 
 *  subwindow. This routine must take care of properly 
 *  switching from one scale to an other.
 *  
 *-------------------------------------------*/

int C2F(Nsetscale2d)( double    WRect[4],
                      double    ARect[4],
                      double    FRect[4],
                      char    * logscale,
                      integer   l1       )
{
  /* if some arguments are null pointer we set them to 
   * the corresponding Cscale value. 
   * this is only important for StoreNEch which do not work with null arguments 
   */ 
  /* char flag[] = "tfffff";*/ /* flag to specify which arguments have changed*/
  /* 14/03/2002*/
  sciPointObj * masousfen = NULL;
  sciPointObj * currentFigure = sciGetCurrentFigure();
	
  char flag[7];
  strcpy(flag,"tfffff");  
 
  /** voir aussi si xsetech (frect) **/ 
  if ( WRect != NULL) 
    {
      /* Ajout djalel */
      startFigureDataWriting(currentFigure);
      if (( masousfen = sciIsExistingSubWin (WRect)) != (sciPointObj *)NULL) {
        sciSetSelectedSubWin(masousfen);
      }
      else if ((masousfen = ConstructSubWin (sciGetCurrentFigure())) != NULL)
      {
        /* F.Leray Adding here 26.03.04*/
        sciSetCurrentObj(masousfen);
        sciSetSelectedSubWin(masousfen);
        pSUBWIN_FEATURE (masousfen)->WRect[0]   = WRect[0];
        pSUBWIN_FEATURE (masousfen)->WRect[1]   = WRect[1];
        pSUBWIN_FEATURE (masousfen)->WRect[2]   = WRect[2];
        pSUBWIN_FEATURE (masousfen)->WRect[3]   = WRect[3];
      }
      endFigureDataWriting(currentFigure);
      
      /* a subwindow is specified */
      flag[1]='t';
      if (! same_subwin( WRect, Cscale.subwin_rect))
	{
	  /* we are using a new subwin we keep current state 
	   * in scale list 
	   */
	  set_window_scale(curwin(),&Cscale);
	  /* now we try to extract a previous scale with WRect as subwin 
	   * which becomes the new Cscale if it is found 
	   */
	  if ( get_window_scale(curwin(),WRect) == 0 ) 
	    {
	      /* this is the first time WRect is used : we reset Cscale flag
	       * Note also that if FRect is also specified in this call 
	       * Cscale.flag will be set to 1 bellow 
	       */
	      Cscale.flag = 0;
	    }
	}
    }
  else
  {
    WRect = Cscale.subwin_rect; 
  }

  if (FRect != NULL)
  {
    startFigureDataWriting(currentFigure);
    masousfen = sciGetCurrentSubWin();
    pSUBWIN_FEATURE (masousfen)->SRect[0] = FRect[0];
    pSUBWIN_FEATURE (masousfen)->SRect[2] = FRect[1];
    pSUBWIN_FEATURE (masousfen)->SRect[1] = FRect[2];
    pSUBWIN_FEATURE (masousfen)->SRect[3] = FRect[3];
    endFigureDataWriting(currentFigure);
  }

  if ( FRect != NULL)
  {
    flag[2]='t';
  }
  else
  {
    FRect = Cscale.frect;
  }

  if (ARect != NULL)
  {
    startFigureDataWriting(currentFigure);
    masousfen=sciGetCurrentSubWin();
    pSUBWIN_FEATURE (masousfen)->ARect[0] = ARect[0];
    pSUBWIN_FEATURE (masousfen)->ARect[1] = ARect[1];
    pSUBWIN_FEATURE (masousfen)->ARect[2] = ARect[2];
    pSUBWIN_FEATURE (masousfen)->ARect[3] = ARect[3];
    endFigureDataWriting(currentFigure);
  }

  if ( ARect != NULL) flag[5]='t'; else ARect = Cscale.axis;
  if ( logscale != NULL) flag[4] ='t'; else logscale = Cscale.logflag;
  if ( flag[4] == 't' && flag[2] == 't' ) 
    {
      if (logscale[0]=='l') 
	{
	  if ( FRect[0] <= 0 || FRect[2] <= 0 ) 
	    {
	      sciprint(_("Warning: negative boundaries on %s scale with a log scale\n"),"x");
	      FRect[0]=1.e-8;FRect[2]=1.e+8;
	    } 
	  FRect[0]=log10(FRect[0]);
	  FRect[2]=log10(FRect[2]);
	}
      if (logscale[1]=='l') 
	{
	  if ( FRect[1] <= 0 || FRect[3] <= 0 ) 
	    {
	      sciprint(_("Warning: negative boundaries on %s scale with a log scale\n"),"y");
	      FRect[1]=1.e-8;FRect[3]=1.e+8;
	    } 
	  FRect[1]=log10(FRect[1]);
	  FRect[3]=log10(FRect[3]);
	}
    }
  set_scale(flag,WRect,FRect,NULL,logscale,ARect); 

  if (masousfen != NULL)
  {
    // subwindow has changed, redraw it
    sciDrawObj(masousfen);
  }

  return(0);
}

/* used to send values to Scilab */
int getscale2d( double WRect[4], double FRect[4], char * logscale, double ARect[4] )
{
  integer i;
  static double ten=10.0;
  logscale[0] = Cscale.logflag[0];
  logscale[1] = Cscale.logflag[1];
  for ( i=0; i < 4 ; i++) 
    {
      WRect[i]=Cscale.subwin_rect[i];
      FRect[i]=Cscale.frect[i];
      ARect[i]=Cscale.axis[i];
    }
  if (logscale[0]=='l') 
    { 
      FRect[0]=pow(ten,FRect[0]);
      FRect[2]=pow(ten,FRect[2]);
    }
  if (logscale[1]=='l') 
    {
      FRect[1]=pow(ten,FRect[1]);
      FRect[3]=pow(ten,FRect[3]);
    }
  return(0);
}

void get_frame_in_pixel(integer WIRect[])
{
  /* ajout bruno */
  WIRect[0] = Cscale.WIRect1[0];
  WIRect[1] = Cscale.WIRect1[1];
  WIRect[2] = Cscale.WIRect1[0] +  Cscale.WIRect1[2];
  WIRect[3] = Cscale.WIRect1[1] +  Cscale.WIRect1[3];
}

void get_margin_in_pixel(integer Margin[])
{
  /* added by bruno 
   *       Margin[0]: left margin 
   *       Margin[1]: right margin
   *       Margin[2]: up margin
   *       Margin[3]: down margin
   */
  double coef_w = 1.0 - Cscale.axis[0] - Cscale.axis[1];
  double coef_h = 1.0 - Cscale.axis[2] - Cscale.axis[3];
  Margin[0] = (integer) ( Cscale.axis[0]/coef_w * (double) Cscale.WIRect1[2]);
  Margin[1] = (integer) ( Cscale.axis[1]/coef_w * (double) Cscale.WIRect1[2]);
  Margin[2] = (integer) ( Cscale.axis[2]/coef_h * (double) Cscale.WIRect1[3]);
  Margin[3] = (integer) ( Cscale.axis[3]/coef_h * (double) Cscale.WIRect1[3]);
}

/*-------------------------------------------
 * changes selected items in the current scale 
 * flag gives which component must be used for 
 *      upgrading or setting the current scale 
 * flag[0]   : used for window dim upgrade 
 * flag[1:5] : subwin,frame_values,aaint,logflag,axis_values
 * Result: Cscale is changed 
 * Warning : frame_values[i] must be log10(val[i]) 
 *           when using log scales 
 *-------------------------------------------*/

void set_scale( char    flag[6]        ,
                double  subwin[4]      ,
                double  frame_values[4],
                integer aaint[4]       ,
                char    logflag[3]     ,
                double  axis_values[4]  )
     /* flag[i] = 't' or 'f' */
     /* subwindow specification */ /* <=> WRect*/
     /* [xmin,ymin,xmax,ymax] */
     /* [xint,x_subint,y_int,y_subint]*/
     /* [xlogflag,ylogflag,zlogflag (NOT USED HERE)] */
     /* [mfact_xl, mfact_xr,mfact_yu,mfact_yd]; */
{
  char wdim_changed= 'f',subwin_changed='f';
  char frame_values_changed='f',aaint_changed='f';
  char logflag_changed='f';
  char axis_changed = 'f';
  integer wdim[2];
  int i;
  if ( flag[0] == 't'  ) 
    {
      wdim[0] = sciGetWindowWidth(sciGetCurrentFigure());
      wdim[1] = sciGetWindowHeight(sciGetCurrentFigure());
      if ( Cscale.wdim[0] != wdim[0] || Cscale.wdim[1] != wdim[1]) 
	{ 
	  Cscale.wdim[0] = wdim[0]; Cscale.wdim[1] = wdim[1]; 
	  wdim_changed='t';
	}
    }
  if ( flag[1] == 't' ) 
    {
      for (i=0; i < 4 ; i++ ) 
	if ( subwin[i] != Cscale.subwin_rect[i]) { subwin_changed='t' ; break;}
    }
  if ( flag[2] == 't' ) 
    {
      for (i=0; i < 4 ; i++ ) 
	if ( frame_values[i] != Cscale.frect[i]) { frame_values_changed='t' ; break;}
      /* if no scales were present and the values given are the same as the 
       * default frect values we must register that we are setting a scale 
       */
      if ( Cscale.flag == 0) frame_values_changed='t' ;
    }
  if ( flag[3] == 't' ) 
    {
      for (i=0; i < 4 ; i++ ) 
	if ( aaint[i] != Cscale.Waaint1[i]) { aaint_changed='t' ; break;}
    }
  if ( flag[4] == 't' ) 
    {
      for (i=0; i < 2 ; i++ ) 
	if ( logflag[i] != Cscale.logflag[i]) { logflag_changed='t' ; break;}
    }
  if ( flag[5] == 't' ) 
    {
      for (i=0; i < 4 ; i++ ) 
	if ( axis_values[i] != Cscale.axis[i]) { axis_changed='t' ; break;}
    }
  if ( axis_changed == 't') 
    {
      for (i=0; i < 4 ; i++ ) Cscale.axis[i] = axis_values[i];
    }

  if ( subwin_changed == 't' ) 
    {
      for (i=0; i < 4 ; i++ ) Cscale.subwin_rect[i] = subwin[i];
    }
  if ( frame_values_changed == 't' ) 
    {
      for (i=0; i < 4 ; i++) Cscale.frect[i]=frame_values[i]; 
      /* the scale is no more a default scale */
      Cscale.flag = 1;
    }
  if ( wdim_changed == 't' || subwin_changed == 't' || frame_values_changed == 't' 
       ||  axis_changed == 't' )
    {
      /* Upgrading constants for 2D transformation */
      double scx,scy,xoff,yoff,val;

      sciPointObj *pobj = sciGetCurrentSubWin();
      sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);

      scx =  ((double) Cscale.wdim[0]*Cscale.subwin_rect[2]-Cscale.wdim[0]*Cscale.subwin_rect[2]
	      *(Cscale.axis[0]+Cscale.axis[1]));
      scy =  ((double) Cscale.wdim[1]*Cscale.subwin_rect[3]-Cscale.wdim[1]*Cscale.subwin_rect[3]
	      *(Cscale.axis[2]+Cscale.axis[3]));
      xoff= ((double) Cscale.wdim[0]*Cscale.subwin_rect[2])*(Cscale.axis[0]);
      yoff= ((double) Cscale.wdim[1]*Cscale.subwin_rect[3])*(Cscale.axis[2]);
      
      Cscale.Wxofset1= xoff+Cscale.subwin_rect[0]*((double)Cscale.wdim[0]);
      Cscale.Wyofset1= yoff+Cscale.subwin_rect[1]*((double)Cscale.wdim[1]);
      Cscale.Wscx1   = scx;
      Cscale.Wscy1   = scy;

      val = Abs(Cscale.frect[0]- Cscale.frect[2]);
      Cscale.Wscx1 = (val <=SMDOUBLE) ? Cscale.Wscx1/SMDOUBLE : Cscale.Wscx1/val; 
      val = Abs(Cscale.frect[1]- Cscale.frect[3]);
      Cscale.Wscy1 = (val <=SMDOUBLE) ? Cscale.Wscy1/SMDOUBLE : Cscale.Wscy1/val;

      /*	  F.Leray 12.10.04 : MODIF named scale_modification*/
      if(ppsubwin->axes.reverse[0]==TRUE && ppsubwin->is3d == FALSE)
      {
        Cscale.WIRect1[0] = XScale( Cscale.frect[2]);
        Cscale.WIRect1[2] = Abs(XScale( Cscale.frect[2]) -  XScale( Cscale.frect[0]));
      }
      else
      {
        Cscale.WIRect1[0] = XScale( Cscale.frect[0]);
        Cscale.WIRect1[2] = Abs(XScale( Cscale.frect[2]) -  XScale( Cscale.frect[0]));
      }

      if(ppsubwin->axes.reverse[1]==TRUE && ppsubwin->is3d == FALSE)
      {
        Cscale.WIRect1[1] = YScale( Cscale.frect[1]);
        Cscale.WIRect1[3] = Abs(YScale( Cscale.frect[3]) -  YScale( Cscale.frect[1]));
      }
      else
      {
        Cscale.WIRect1[1] = YScale( Cscale.frect[3]);
        Cscale.WIRect1[3] = Abs(YScale( Cscale.frect[3]) -  YScale( Cscale.frect[1]));
      }
    }
  if (  aaint_changed== 't' ) 
    {
      for (i=0; i < 4 ; i++) Cscale.Waaint1[i]=aaint[i];
    }
  if ( logflag_changed== 't' ) 
    {
      Cscale.logflag[0]=logflag[0];      Cscale.logflag[1]=logflag[1];
    }
  if (  aaint_changed== 't' || frame_values_changed == 't')
    {
      Cscale.xtics[0] = Cscale.frect[0];
      Cscale.xtics[1] = Cscale.frect[2];
      Cscale.ytics[0] = Cscale.frect[1];
      Cscale.ytics[1] = Cscale.frect[3];
      Cscale.xtics[2] = 0.0;
      Cscale.ytics[2] = 0.0;
      Cscale.xtics[3] = Cscale.Waaint1[1];
      Cscale.ytics[3] = Cscale.Waaint1[3];
    }
 
    set_window_scale(curwin(),&Cscale);

}

/*--------------------------------------------------------------------
 * Get the current window dimensions.
 *--------------------------------------------------------------------*/

void get_cwindow_dims(int wdims[2])
{
  wdims[0] = sciGetWindowWidth(sciGetCurrentFigure());
  wdims[1] = sciGetWindowHeight(sciGetCurrentFigure());
}

/* for x only */

int C2F(xechelle2d)(double x[], integer x1[], integer *  n1,char dir[],integer lstr)
{
  integer i;
  if (strcmp("f2i",dir)==0) 
  {

    if (Cscale.logflag[0] == 'n') 
      for ( i=0 ; i < (*n1) ; i++) x1[i]= XScale(x[i]);
    else 
      for ( i=0 ; i < (*n1) ; i++) x1[i]= XLogScale(x[i]);
  }
  else if (strcmp("i2f",dir)==0) 
  {
    if (Cscale.logflag[0] == 'n') 
      for ( i=0 ; i < (*n1) ; i++) x[i]= XPi2R( x1[i] );
    else 
      for ( i=0 ; i < (*n1) ; i++) x[i]= exp10(XPi2R( x1[i]));
  }
  else 
    sciprint(_("%s: Wrong dir %s argument.\n"),"echelle2d",dir);

  return(0);
}

/* for y only */

int C2F(yechelle2d)(double y[], integer yy1[], integer * n2,char dir[], integer lstr)
{
  integer i;
  if (strcmp("f2i",dir)==0) 
  {
    if (Cscale.logflag[1] == 'n') 
      for ( i=0 ; i < (*n2) ; i++) yy1[i]= YScale(y[i]);
    else 
      for ( i=0 ; i < (*n2) ; i++) yy1[i]= YLogScale(y[i]);
  }
  else if (strcmp("i2f",dir)==0) 
  {
    if (Cscale.logflag[1] == 'n') 
      for ( i=0 ; i < (*n2) ; i++)  y[i]= YPi2R( yy1[i] );
    else 
      for ( i=0 ; i < (*n2) ; i++)  y[i]= exp10(YPi2R( yy1[i]));
  }
  else 
    sciprint(_("%s: Wrong dir %s argument.\n"),"echelle2d",dir); 

  return(0);
}

/*--------------------------------------------------------------------
 * Convert pixel<->double using current scale 
 * 
 *  C2F(echelle2d)(x,y,x1,y1,n1,n2,rect,dir)
 *    x,y,x1,y1 of size [n1*n2] 
 *    dir     : "f2i" -> double to integer (you give x and y and get x1,y1)
 *            : "i2f" -> integer to double (you give x1 and y1 and get x,y)
 *    lstr    : unused (Fortran/C) 
 * --------------------------------------------------------------------------*/

int C2F(echelle2d)( double    x[]  ,
                    double    y[]  ,
                    integer   x1[] ,
                    integer   yy1[],
                    integer * n1   ,
                    integer * n2   ,
                    char      dir[],
                    integer   lstr )

{
  int n=(*n1)*(*n2);
  C2F(xechelle2d)(x,x1,&n,dir,lstr);
  C2F(yechelle2d)(y,yy1,&n,dir,lstr);
  return(0);
}



/*--------------------------------------------------------------------
 * void C2F(echelle2dl)(x, y, x1, yy1, n1, n2,  dir)
 * like echelle2d but for length convertion 
 * Note that it cannot work in logarithmic scale 
 *--------------------------------------------------------------------*/

void C2F(echelle2dl)( double    x[]  ,
                      double    y[]  ,
                      integer   x1[] ,
                      integer   yy1[],
                      integer * n1   ,
                      integer * n2   ,
                      char    * dir   )
{
  integer i;
  if (strcmp("f2i",dir)==0) 
    {
      for ( i=0 ; i < (*n1)*(*n2) ; i++)
	{
	  x1[i]=inint( Cscale.Wscx1*( x[i]));
	  yy1[i]=inint( Cscale.Wscy1*( y[i]));
	}
    }
  else if (strcmp("i2f",dir)==0) 
    {
      for ( i=0 ; i < (*n1)*(*n2) ; i++)
	{
	  x[i]=x1[i]/Cscale.Wscx1;
	  y[i]=yy1[i]/Cscale.Wscy1;
	}
    }
  else 
    sciprint(_("%s: Wrong dir %s argument.\n"),"echelle2d",dir);
}

/** meme chose mais pour transformer des ellipses **/

void C2F(ellipse2d)( double x[], integer x1[], integer * n, char * dir)
{
  integer i;
  if (strcmp("f2i",dir)==0) 
    {
      /** double to integer (pixel) direction **/
      for ( i=0 ; i < (*n) ; i=i+6)
	{
	  x1[i  ]= XScale(x[i]);
	  x1[i+1]= YScale(x[i+1]);
	  x1[i+2]= inint( Cscale.Wscx1*( x[i+2]));
	  x1[i+3]= inint( Cscale.Wscy1*( x[i+3]));
	  x1[i+4]= inint( x[i+4]);
	  x1[i+5]= inint( x[i+5]);
	}
    }
  else if (strcmp("i2f",dir)==0) 
    {
      for ( i=0 ; i < (*n) ; i=i+6)
	{
	  x[i]=   XPi2R(x1[i]); 
	  x[i+1]= YPi2R( x1[i+1] ); 
	  x[i+2]= x1[i+2]/Cscale.Wscx1;
	  x[i+3]= x1[i+3]/Cscale.Wscy1;
	  x[i+4]= x1[i+4];
	  x[i+5]= x1[i+5];
	}
    }
  else 
    sciprint(_("%s: Wrong dir %s argument.\n"),"echelle2d",dir);
}

/** meme chose mais pour transformer des rectangles **/

void C2F(rect2d)( double x[], integer x1[], integer * n, char * dir)
{
  integer i;
  if (strcmp("f2i",dir)==0) 
    {
      /** double to integer (pixel) direction **/
      for ( i=0 ; i < (*n) ; i= i+4)
	{
	  if ( Cscale.logflag[0] == 'n' ) 
	    {
	      x1[i]=  XScale(x[i]);
	      x1[i+2]=inint( Cscale.Wscx1*( x[i+2]));
	    }
	  else 
	    {
	      x1[i]= XLogScale(x[i]);
	      x1[i+2]=inint( Cscale.Wscx1*(log10((x[i]+x[i+2])/x[i])));
	    } 
	  if ( Cscale.logflag[1] == 'n' ) 
	    {
	      x1[i+1]= YScale(x[i+1]);
	      x1[i+3]=inint( Cscale.Wscy1*( x[i+3]));
	    }
	  else 
	    {
	      x1[i+1]= YLogScale(x[i+1]);
	      x1[i+3]=inint( Cscale.Wscy1*(log10(x[i+1]/(x[i+1]-x[i+3]))));
	    }
	}
    } 
  else if (strcmp("i2f",dir)==0) 
    {
      for ( i=0 ; i < (*n) ; i=i+4)
	{
	  if ( Cscale.logflag[0] == 'n' ) 
	    {
	      x[i]= XPi2R(x1[i] );
	      x[i+2]=x1[i+2]/Cscale.Wscx1;
	    }
	  else
	    {
	      x[i]=exp10( XPi2R(x1[i]));
	      x[i+2]=exp10(XPi2R( x1[i]+x1[i+2] ));
	      x[i+2] -= x[i];
	    }
	  if ( Cscale.logflag[1] == 'n' ) 
	    {
	      x[i+1]= YPi2R( x1[i+1]);
	      x[i+3]=x1[i+3]/Cscale.Wscy1;
	    }
	  else
	    {
	      x[i+1]=exp10( YPi2R( x1[i+1]));
	      x[i+3]=exp10( YPi2R( x1[i+3]+x1[i+1])); 
	      x[i+2] -= x[i+1];
	    }
	}
    }
  else 
    sciprint(_("%s: Wrong dir %s argument.\n"),"echelle2d",dir);
}

 
/** meme chose mais pour axis **/

void C2F(axis2d)( double  * alpha     ,
                  double  * initpoint ,
                  double  * size      ,
                  integer * initpoint1,
                  double  * size1       )
{
  double sina ,cosa;
  double xx,yy,scl;
  /* pour eviter des problemes numerique quand Cscale.scx1 ou Cscale.scy1 sont 
   *  tres petits et cosal ou sinal aussi 
   */
  if ( Abs(*alpha) == 90 ) 
    {
      scl=Cscale.Wscy1;
    }
  else 
    {
      if (Abs(*alpha) == 0) 
	{
	  scl=Cscale.Wscx1;
	}
      else 
	{
	  sina= sin(*alpha * M_PI/180.0);
	  cosa= cos(*alpha * M_PI/180.0);
	  xx= cosa*Cscale.Wscx1; xx *= xx;
	  yy= sina*Cscale.Wscy1; yy *= yy;
	  scl= sqrt(xx+yy);
	}
    }
  size1[0] = size[0]*scl;
  size1[1]=  size[1]*scl;
  size1[2]=  size[2];
  initpoint1[0]= XScale(initpoint[0]);
  initpoint1[1]= YScale(initpoint[1]);
}

/** Changement interactif d'echelle **/

extern int EchCheckSCPlots();

/** get a rectangle interactively **/

int zoom_get_rectangle(double *bbox,int *x_pixel, int *y_pixel)
{
  x_pixel[0] = 0;
  x_pixel[1] = 0;
  y_pixel[0] = 0;
  y_pixel[1] = 0;
  bbox[0] = 0.0;
  bbox[1] = 0.0;
  bbox[2] = 0.0;
  bbox[3] = 0.0;

  return 0;
}



int zoom_box(double *bbox,int *x_pixel, int *y_pixel)
{
  integer min,max,puiss,deux=2,dix=10,box[4],box1[4],section[4];

  double fmin,fmax,lmin,lmax;
  sciPointObj *psousfen,*tmpsousfen;
  sciPointObj * pfigure = NULL;
  sciSons *psonstmp;
  sciSubWindow * ppsubwin = NULL; /* debug */

  box[0]= Min(XDouble2Pixel(bbox[0]),XDouble2Pixel(bbox[2]));
  box[2]= Max(XDouble2Pixel(bbox[0]),XDouble2Pixel(bbox[2]));
  box[1]= Min(YDouble2Pixel(bbox[1]),YDouble2Pixel(bbox[3]));
  box[3]= Max(YDouble2Pixel(bbox[1]),YDouble2Pixel(bbox[3]));

  pfigure = sciGetCurrentFigure();
  tmpsousfen= sciGetCurrentSubWin();
  psonstmp = sciGetSons (sciGetCurrentFigure());
  while (psonstmp != (sciSons *) NULL)
  {
    if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
    {
      psousfen= (sciPointObj *)psonstmp->pointobj;
      if ( pSUBWIN_FEATURE (psousfen)->is3d == TRUE) {
        double xmin, ymin;
        double xmax, ymax;
        double zmin, zmax;
        sciSons *psonstmp2 = (sciSons *) NULL;
        double epsilon = 1e-16;

        int box3d[4];
        int section3d[4];

        sciSetSelectedSubWin(psousfen);
        ppsubwin = pSUBWIN_FEATURE (psousfen);

        box3d[0] = x_pixel[0];
        box3d[2] = x_pixel[1];
        box3d[1] = y_pixel[0];
        box3d[3] = y_pixel[1];

        box1[0]= Cscale.WIRect1[0];
        box1[2]= Cscale.WIRect1[2]+Cscale.WIRect1[0];
        box1[1]= Cscale.WIRect1[1];
        box1[3]= Cscale.WIRect1[3]+Cscale.WIRect1[1];

        if (sciIsAreaZoom(box3d,box1,section3d))
        {
          int tmp[4],i;

          for(i=0;i<4;i++) tmp[i] = section3d[i];

          section3d[0] = Min(tmp[0],tmp[2]);
          section3d[2] = Max(tmp[0],tmp[2]);
          section3d[1] = Min(tmp[1],tmp[3]);
          section3d[3] = Max(tmp[1],tmp[3]);

          /* determine how many vertices we will have to draw */
          /* this is used for 3d zoom (to know size of the global vector) */
          setVertexIndex( 0 ) ;

          /* this flag is used inside trans3d called many times by sciDrawObj */
          setZoom3dStatus( TRUE );

          psonstmp2 = sciGetLastSons (psousfen);

          sciDrawObj(psousfen); /* see GlobalFlag_Zoom3dOn impact flag in sciDrawObj & trans3d functions */

          setZoom3dStatus( FALSE );

          SetMinMaxVertices(pSUBWIN_FEATURE(psousfen)->vertices_list, &xmin, &ymin, &zmin, &xmax, &ymax, &zmax);


          for( i = 0 ; i < getVertexIndex() ; i++ )
          {
            int xp,yp;
            double x,y,z;

            if(GetVerticesAt(pSUBWIN_FEATURE(psousfen)->vertices_list, &xp, &yp, &x, &y, &z)==-1) 
              break; /* all the vertices have been scanned inside the current psubwin */

            if(xp >= section3d[0] && xp <= section3d[2] && yp >= section3d[1] && yp <= section3d[3])
            {
              /* 			  printf("ICI => xp = %d \t yp = %d et x = %lf \t y = %lf \t z = %lf\n",xp,yp,x,y,z); */
              if(x < xmin) xmin=x; if(x > xmax) xmax=x;
              if(y < ymin) ymin=y; if(y > ymax) ymax=y;
              if(z < zmin) zmin=z; if(z > zmax) zmax=z;
            }
          }

          if(ppsubwin->axes.reverse[0] == TRUE) {
            double tmp2;
            tmp2 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xmin);
            xmin = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xmax);
            xmax = tmp2;
          }

          if(ppsubwin->axes.reverse[1] == TRUE) {
            double tmp2;
            tmp2 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ymin);
            ymin = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ymax);
            ymax = tmp2;
          }

          if(ppsubwin->axes.reverse[2] == TRUE) {
            double tmp2;
            tmp2 = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],zmin);
            zmin = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],zmax);
            zmax = tmp2;
          }

          FreeVertices(psousfen);

          /* case where no vertex has been selected : */
          /* no zoom is performed */
          if(xmin > xmax || ymin > ymax || zmin > zmax)
          {
            xmin = ppsubwin->SRect[0];
            xmax = ppsubwin->SRect[1];

            ymin = ppsubwin->SRect[2];
            ymax = ppsubwin->SRect[3];

            zmin = ppsubwin->SRect[4];
            zmax = ppsubwin->SRect[5];
          }

          if (!(sciGetZooming(psousfen)))
            sciSetZooming(psousfen, 1);


          /* case where a 3d rotated plot2d is zoomed */
          if(fabs(zmin) < epsilon && fabs(zmax) < epsilon){
            zmin = pSUBWIN_FEATURE (psousfen)->SRect[4];
            zmax = pSUBWIN_FEATURE (psousfen)->SRect[5];
          }

          pSUBWIN_FEATURE (psousfen)->ZRect[0] = xmin;
          pSUBWIN_FEATURE (psousfen)->ZRect[2] = xmax;

          pSUBWIN_FEATURE (psousfen)->ZRect[1] = ymin;
          pSUBWIN_FEATURE (psousfen)->ZRect[3] = ymax;

          pSUBWIN_FEATURE (psousfen)->ZRect[4] = zmin;
          pSUBWIN_FEATURE (psousfen)->ZRect[5] = zmax;

        }
      }
      sciSetSelectedSubWin(psousfen);
      ppsubwin = pSUBWIN_FEATURE (psousfen);

      if ( pSUBWIN_FEATURE (psousfen)->is3d == FALSE) {

        box1[0]= Cscale.WIRect1[0];
        box1[2]= Cscale.WIRect1[2]+Cscale.WIRect1[0];
        box1[1]= Cscale.WIRect1[1];
        box1[3]= Cscale.WIRect1[3]+Cscale.WIRect1[1];

        if (sciIsAreaZoom(box,box1,section))
        {
          bbox[0]= Min(XPixel2Double(section[0]),XPixel2Double(section[2]));
          bbox[2]= Max(XPixel2Double(section[0]),XPixel2Double(section[2]));
          bbox[1]= Min(YPixel2Double(section[1]),YPixel2Double(section[3]));
          bbox[3]= Max(YPixel2Double(section[1]),YPixel2Double(section[3]));

          if (!(sciGetZooming(psousfen)))
            sciSetZooming(psousfen, 1);

          /** regraduation de l'axe des axes ***/
          fmin=  bbox[0];
          fmax=  bbox[2];
          if(pSUBWIN_FEATURE (psousfen)->logflags[0] == 'n') {
            C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
            pSUBWIN_FEATURE(psousfen)->axes.xlim[2]=puiss;
            pSUBWIN_FEATURE (psousfen)->ZRect[0]=lmin;
            pSUBWIN_FEATURE (psousfen)->ZRect[2]=lmax;}
          else {
            pSUBWIN_FEATURE(psousfen)->axes.xlim[2]=0;
            pSUBWIN_FEATURE (psousfen)->ZRect[0]=fmin;
            pSUBWIN_FEATURE (psousfen)->ZRect[2]=fmax;
          }


          fmin= bbox[1]; 
          fmax= bbox[3];
          if(pSUBWIN_FEATURE (psousfen)->logflags[1] == 'n') {
            C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
            pSUBWIN_FEATURE(psousfen)->axes.ylim[2]=puiss;
            pSUBWIN_FEATURE (psousfen)->ZRect[1]=lmin;
            pSUBWIN_FEATURE (psousfen)->ZRect[3]=lmax;}
          else {
            pSUBWIN_FEATURE(psousfen)->axes.ylim[2]=0;
            pSUBWIN_FEATURE (psousfen)->ZRect[1]=fmin;
            pSUBWIN_FEATURE (psousfen)->ZRect[3]=fmax;}
        }
      }
    }
    psonstmp = psonstmp->pnext;
  }
  sciSetSelectedSubWin(tmpsousfen);
  sciDrawObj(sciGetCurrentFigure());

  return 0;
}


int zoom()
{
  double bbox[4];
  int x_pixel[2], y_pixel[2];

  if ( zoom_get_rectangle(bbox,x_pixel,y_pixel) == 1 ) { return 1 ; }
  
  zoom_box(bbox,x_pixel,y_pixel);
  return 0;
}


extern void unzoom()
{
  /** 17/09/2002 ***/
  double fmin,fmax,lmin,lmax;
  integer min,max,puiss,deux=2,dix=10;
  sciPointObj *psousfen;
  sciSons *psonstmp;

  /***** 02/10/2002 ****/
  psonstmp = sciGetSons(sciGetCurrentFigure());
  while (psonstmp != (sciSons *) NULL)
  {
    if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
    {
      psousfen= (sciPointObj *)psonstmp->pointobj;
      if (sciGetZooming(psousfen))
      {
        sciSetZooming(psousfen, 0);

        pSUBWIN_FEATURE (psousfen)->ZRect[0]   = pSUBWIN_FEATURE (psousfen)->SRect[0];
        pSUBWIN_FEATURE (psousfen)->ZRect[1]   = pSUBWIN_FEATURE (psousfen)->SRect[1];
        pSUBWIN_FEATURE (psousfen)->ZRect[2]   = pSUBWIN_FEATURE (psousfen)->SRect[2];
        pSUBWIN_FEATURE (psousfen)->ZRect[3]   = pSUBWIN_FEATURE (psousfen)->SRect[3];

        /*}  SS: moved below because if sciGetZooming(psousfen)==0 
        ZRect is undefined -> code may enter in infinite recursion loop to compute graduation
        and there is no use to regraduate */

        /** regraduation de l'axe des axes ***/
        fmin= pSUBWIN_FEATURE (psousfen)->ZRect[0];
        fmax= pSUBWIN_FEATURE (psousfen)->ZRect[2];
        C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
        pSUBWIN_FEATURE(psousfen)->axes.xlim[2]=puiss;

        fmin= pSUBWIN_FEATURE (psousfen)->ZRect[1];
        fmax= pSUBWIN_FEATURE (psousfen)->ZRect[3];
        C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
        pSUBWIN_FEATURE(psousfen)->axes.ylim[2]=puiss;
        /*****/
      }
    }
    psonstmp = psonstmp->pnext;
  }

  sciDrawObj(sciGetCurrentFigure());
}

extern void unzoom_one_axes(sciPointObj *psousfen)
{
  double fmin,fmax,lmin,lmax;
  integer min,max,puiss,deux=2,dix=10;

  if (sciGetEntityType (psousfen) == SCI_SUBWIN) {
    if (sciGetZooming(psousfen)) 	{
      sciSetZooming(psousfen, 0);

      pSUBWIN_FEATURE (psousfen)->ZRect[0]   = pSUBWIN_FEATURE (psousfen)->SRect[0];
      pSUBWIN_FEATURE (psousfen)->ZRect[1]   = pSUBWIN_FEATURE (psousfen)->SRect[1];
      pSUBWIN_FEATURE (psousfen)->ZRect[2]   = pSUBWIN_FEATURE (psousfen)->SRect[2];
      pSUBWIN_FEATURE (psousfen)->ZRect[3]   = pSUBWIN_FEATURE (psousfen)->SRect[3];

    }

    /** regraduation de l'axe des axes ***/
    fmin= pSUBWIN_FEATURE (psousfen)->ZRect[0];
    fmax= pSUBWIN_FEATURE (psousfen)->ZRect[2];
    C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
    pSUBWIN_FEATURE(psousfen)->axes.xlim[2]=puiss;

    fmin= pSUBWIN_FEATURE (psousfen)->ZRect[1];
    fmax= pSUBWIN_FEATURE (psousfen)->ZRect[3];
    C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
    pSUBWIN_FEATURE(psousfen)->axes.ylim[2]=puiss;

    sciDrawObj(sciGetCurrentFigure());
  }
}

/* 
 *  FRectI=[xmin,ymin,xmax,ymax] est transforme de 
 *  facon a avoir une graduation simple et reguliere 
 *  Xdec,Ydec,xnax,ynax
 *  caracterisant cette graduation 
 *  (voir les fonctions qui suivent )
 */

void Gr_Rescale(char *logFlags, double *FRectI, integer *Xdec, integer *Ydec, integer *xnax, integer *ynax)
{
  double FRectO[4];
  sciPointObj *psubwin; 
  int i;

  /** 18/06/2002 **/
    psubwin = sciGetCurrentSubWin();
    for (i=0;i<4 ; i++)
      pSUBWIN_FEATURE (psubwin)->axes.limits[i+1]=FRectI[i];
     
    
  if (logFlags[0] == 'n') 
    { 
      if (!pSUBWIN_FEATURE (psubwin)->tight_limits )
	{
          C2F(graduate)(FRectI,FRectI+2,FRectO,FRectO+2,xnax,xnax+1,Xdec,Xdec+1,Xdec+2);
	  FRectI[0]=FRectO[0];FRectI[2]=FRectO[2];
	}
        
      else
	{
	  C2F(graduate)(FRectI,FRectI+2,FRectO,FRectO+2,xnax,xnax+1,Xdec,Xdec+1,Xdec+2);
	}
    }
  else
    {
      Xdec[0]=inint(FRectI[0]);
      Xdec[1]=inint(FRectI[2]);
      Xdec[2]=0;
    }
  if (logFlags[1] == 'n') 
    {
      if ( !pSUBWIN_FEATURE (psubwin)->tight_limits)
	{
	  C2F(graduate)(FRectI+1,FRectI+3,FRectO+1,FRectO+3,ynax,ynax+1,Ydec,Ydec+1,Ydec+2);
	  FRectI[1]=FRectO[1];FRectI[3]=FRectO[3];
	} 
      else
	{
	  C2F(graduate)(FRectI+1,FRectI+3,FRectO+1,FRectO+3,ynax,ynax+1,Ydec,Ydec+1,Ydec+2);
	}
    }
  else
    {
      Ydec[0]=inint(FRectI[1]);Ydec[1]=inint(FRectI[3]);Ydec[2]=0;
    }


}



int XScale(double x)
{
  /*F.Leray 12.10.04 : MODIF named scale_modification*/

  sciPointObj *pobj = sciGetCurrentSubWin();
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);

  if(ppsubwin->axes.reverse[0]==TRUE && ppsubwin->is3d == FALSE)
  {

    /* 2147483647 == 2^31 - 1 == INT32MAX */
    /*return inint( Max( Min(Cscale.Wscx1*(Cscale.frect[2] - (x)) + Cscale.Wxofset1, INT_MAX ), INT_MIN ) ) ;*/
    double dRes = Cscale.Wscx1 * (Cscale.frect[2] - (x)) + Cscale.Wxofset1 ;
    return FLOAT_2_INT( dRes ) ;
  }
  else
  {
    /*return inint( Max( Min(Cscale.Wscx1*((x) -Cscale.frect[0]) + Cscale.Wxofset1, INT_MAX ), INT_MIN ) );*/
    double dRes = Cscale.Wscx1 * ((x) -Cscale.frect[0]) + Cscale.Wxofset1 ;
    return FLOAT_2_INT( dRes ) ;
  }
  
}




int XLogScale(double x)
{
  /*F.Leray 12.10.04 : MODIF named scale_modification*/

  sciPointObj *pobj = sciGetCurrentSubWin();
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);

  if(ppsubwin->axes.reverse[0]==TRUE && ppsubwin->is3d == FALSE){
    return inint( Cscale.Wscx1*(Cscale.frect[2] - log10(x)) + Cscale.Wxofset1);
  }else{
    return inint( Cscale.Wscx1*(log10(x) -Cscale.frect[0]) + Cscale.Wxofset1);
  }
}




int YScale(double y)
{
  /*F.Leray 12.10.04 : MODIF named scale_modification*/

  sciPointObj *pobj = sciGetCurrentSubWin();
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);

  if( ppsubwin->axes.reverse[1] && !ppsubwin->is3d )
  {
    double dRes = Cscale.Wscy1 * ( y - Cscale.frect[1] ) + Cscale.Wyofset1 ;
    return FLOAT_2_INT( dRes ) ;
  }
  else
  {
    double dRes = Cscale.Wscy1 * ( -y + Cscale.frect[3] ) + Cscale.Wyofset1 ;
    return FLOAT_2_INT( dRes ) ;
  }
}


int YLogScale(double y)
{
  /*F.Leray 12.10.04 : MODIF named scale_modification*/
  sciPointObj *pobj = sciGetCurrentSubWin();
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);

  if(ppsubwin->axes.reverse[1]==TRUE && ppsubwin->is3d == FALSE){
    return inint( Cscale.Wscy1*(log10(y)-Cscale.frect[1]) + Cscale.Wyofset1);
  }else{
    return inint( Cscale.Wscy1*(-log10(y)+Cscale.frect[3]) + Cscale.Wyofset1);
  }

}



double XPi2R(int x)
{
  /*F.Leray 12.10.04 : MODIF named scale_modification*/

  sciPointObj *pobj = sciGetCurrentSubWin();
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);

  if(ppsubwin->axes.reverse[0]==TRUE && ppsubwin->is3d == FALSE){
    return Cscale.frect[2] - (1.0/Cscale.Wscx1)*((x) - Cscale.Wxofset1);
  }else{
    return Cscale.frect[0] + (1.0/Cscale.Wscx1)*((x) - Cscale.Wxofset1);
  }
}


double YPi2R(int y)
{
  /*F.Leray 12.10.04 : MODIF named scale_modification*/
  sciPointObj *pobj = sciGetCurrentSubWin();
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);

  if(ppsubwin->axes.reverse[1]==TRUE && ppsubwin->is3d == FALSE){
    return Cscale.frect[1] + (1.0/Cscale.Wscy1)*((y) - Cscale.Wyofset1);
  }else{
    return Cscale.frect[3] - (1.0/Cscale.Wscy1)*((y) - Cscale.Wyofset1);
  }
}

double XDPi2R( double x )
{
  /*F.Leray 12.10.04 : MODIF named scale_modification*/
  sciPointObj *pobj = sciGetCurrentSubWin();
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);

  if(ppsubwin->axes.reverse[0]==TRUE && ppsubwin->is3d == FALSE){
    return Cscale.frect[2] - (1.0/Cscale.Wscx1)*((x) - Cscale.Wxofset1);
  }else{
    return Cscale.frect[0] + (1.0/Cscale.Wscx1)*((x) - Cscale.Wxofset1);
  }

}


double YDPi2R( double y )
{
  /*F.Leray 12.10.04 : MODIF named scale_modification*/
  sciPointObj *pobj = sciGetCurrentSubWin();
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);

  if(ppsubwin->axes.reverse[1]==TRUE && ppsubwin->is3d == FALSE){
    return Cscale.frect[1] + (1.0/Cscale.Wscy1)*((y) - Cscale.Wyofset1);
  }else{
    return Cscale.frect[3] - (1.0/Cscale.Wscy1)*((y) - Cscale.Wyofset1);
  }
}

double Zoom3d_XPi2R(int x)
{
  /*F.Leray 12.10.04 : MODIF named scale_modification*/
  sciPointObj *pobj = sciGetCurrentSubWin();
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);

  if(ppsubwin->axes.reverse[0]==TRUE){ /* difference is HERE */
    return Cscale.frect[2] - (1.0/Cscale.Wscx1)*((x) - Cscale.Wxofset1);
  }else{
    return Cscale.frect[0] + (1.0/Cscale.Wscx1)*((x) - Cscale.Wxofset1);
  }
}


double Zoom3d_YPi2R(int y)
{
  /*F.Leray 12.10.04 : MODIF named scale_modification*/

  sciPointObj *pobj = sciGetCurrentSubWin();
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);

  if(ppsubwin->axes.reverse[1]==TRUE) { /* difference is HERE */
    return Cscale.frect[1] + (1.0/Cscale.Wscy1)*((y) - Cscale.Wyofset1);
  }else{
    return Cscale.frect[3] - (1.0/Cscale.Wscy1)*((y) - Cscale.Wyofset1);
  }
}

void scizoom( double bbox[4], sciPointObj * pobj )
{
  sciPointObj *psousfen;
  double fmin,fmax,lmin,lmax;
  integer min,max,puiss,deux=2,dix=10;
  psousfen= pobj; /* ??? */

  if ( !( sciGetZooming(pobj) ) )
  {
    sciSetZooming(psousfen, TRUE);
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
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
int PixelHeight2d( sciPointObj * parentSubWin, double posY, double height )
{
  if ( pSUBWIN_FEATURE( parentSubWin )->logflags[1] == 'l' )
  {
    return HLogScale( posY, height ) ;
  }

  return HScale( height ) ;

}
/*--------------------------------------------------------------------------*/
int PixelWidth2d( sciPointObj * parentSubWin, double posX, double width )
{
  if ( pSUBWIN_FEATURE( parentSubWin )->logflags[0] == 'l' )
  {
    return WLogScale( posX, width ) ;
  }

  return WScale( width ) ;

}
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
void Plo2d2RealToPixel(integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf)
{
  integer i,j;
  /** Computing y-values **/
  if ((int)strlen(xf) >= 3 && xf[2]=='l')	  
  {
    for ( i=0 ; i < (*n2) ; i++)
      for (j=0 ; j< (*n1) ; j++)
        ym[2*i+1+2*(*n2)*j]= ym[2*i+2*(*n2)*j]= YLogScale(y[i+(*n2)*j]);
  }
  else 
  {
    for ( i=0 ; i < (*n2) ; i++)
      for (j=0 ; j< (*n1) ; j++)
        ym[2*i+1+2*(*n2)*j]= ym[2*i+2*(*n2)*j]= YScale(y[i+(*n2)*j]);
  }

  /** Computing x-values **/
  switch (xf[0])
  {
  case 'e' :
    /** No X-value given by the user **/
    if ((int)strlen(xf) >= 2 && xf[1]=='l')
      for (j=0 ; j< (*n1) ; j++)
      {
        for ( i=1 ; i < (*n2) ; i++)
        {
          xm[2*i+2*(*n2)*j]= XLogScale(i+1.0);
          xm[2*i-1+2*(*n2)*j]=xm[2*i+2*(*n2)*j];

        }
        xm[2*(*n2)*j]= XScale(0);
        xm[2*(*n2)-1+ 2*(*n2)*j]= xm[2*(*n2-1)+ 2*(*n2)*j];
      }
    else 
      for (j=0 ; j< (*n1) ; j++)
      {
        for ( i=1 ; i < (*n2) ; i++)
        {
          xm[2*i+2*(*n2)*j]= XScale((i+1.0));
          xm[2*i-1+2*(*n2)*j]=xm[2*i+2*(*n2)*j];

        }
        xm[2*(*n2)*j]= XScale(1.0);
        xm[2*(*n2)-1+ 2*(*n2)*j]=	 xm[2*(*n2-1)+ 2*(*n2)*j];
      }
      break ;
  case 'o' :
    if ((int)strlen(xf) >= 2 && xf[1]=='l')
      for (j=0 ; j< (*n1) ; j++)
      {
        for ( i=1 ; i < (*n2) ; i++)
        {

          xm[2*i+2*(*n2)*j]= XLogScale(x[i]);
          xm[2*i-1+2*(*n2)*j]=xm[2*i+2*(*n2)*j];
        }
        xm[2*(*n2)*j]= XLogScale(x[0]);
        xm[2*(*n2)-1+ 2*(*n2)*j]= xm[2*(*n2-1)+ 2*(*n2)*j];
      }
    else 
      for (j=0 ; j< (*n1) ; j++)
      {
        for ( i=1 ; i < (*n2) ; i++)
        {
          xm[2*i+2*(*n2)*j]= XScale(x[i]);
          xm[2*i-1+2*(*n2)*j]=xm[2*i+2*(*n2)*j];

        }
        xm[2*(*n2)*j]= XScale(x[0]);
        xm[2*(*n2)-1+ 2*(*n2)*j]= xm[2*(*n2-1)+ 2*(*n2)*j];
      }
      break;
  case 'g' :
  default:
    if ((int)strlen(xf) >= 2 && xf[1]=='l')
      for (j=0 ; j< (*n1) ; j++)
      {
        for ( i=1 ; i < (*n2) ; i++)
        {
          xm[2*i+2*(*n2)*j]= XLogScale(x[i+(*n2)*j]);
          xm[2*i-1+2*(*n2)*j]=xm[2*i+2*(*n2)*j];

        }
        xm[2*(*n2)*j]= XLogScale(x[(*n2)*j]);
        xm[2*(*n2)-1+ 2*(*n2)*j]= xm[2*(*n2-1)+ 2*(*n2)*j];
      }
    else 
      for (j=0 ; j< (*n1) ; j++)
      {
        for ( i=1 ; i < (*n2) ; i++)
        {
          xm[2*i+2*(*n2)*j]= XScale(x[i+(*n2)*j]);
          xm[2*i-1+2*(*n2)*j]=xm[2*i+2*(*n2)*j];

        }
        xm[2*(*n2)*j]= XScale(x[(*n2)*j]);
        xm[2*(*n2)-1+ 2*(*n2)*j]= xm[2*(*n2-1)+ 2*(*n2)*j];
      }
      break;
  }
}
/*--------------------------------------------------------------------------*/
void Plo2d3RealToPixel(integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf)
{
  integer i,j;
  /** Computing y-values **/
  double y_zero = 0.;
  sciPointObj *  psubwin =  sciGetCurrentSubWin();
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);

  if(ppsubwin->logflags[1]=='l')
  {
    y_zero = ppsubwin->FRect[1];
  }



  if ((int)strlen(xf) >= 3 && xf[2]=='l')	  
  {
    for ( i=0 ; i < (*n2) ; i++)
      for (j=0 ; j< (*n1) ; j++)
      {
        ym[2*i+1+2*(*n2)*j]= YScale(y_zero);
        ym[2*i+2*(*n2)*j]= YLogScale(y[i+(*n2)*j]);
      }
  }
  else 
  {
    for ( i=0 ; i < (*n2) ; i++)
      for (j=0 ; j< (*n1) ; j++)
      {
        ym[2*i+1+2*(*n2)*j]= YScale(y_zero);
        ym[2*i+2*(*n2)*j]= YScale(y[i+(*n2)*j]);
      }
  }

  /** Computing x-values **/
  switch (xf[0])
  {
  case 'e' :
    /** No X-value given by the user **/
    if ((int)strlen(xf) >= 2 && xf[1]=='l')
      for (j=0 ; j< (*n1) ; j++)
      {
        for ( i=0 ; i < (*n2) ; i++)
        {
          xm[2*i+2*(*n2)*j]= XLogScale(i+1.0);
          xm[2*i+1+2*(*n2)*j]=xm[2*i+2*(*n2)*j];

        }
      }
    else 
      for (j=0 ; j< (*n1) ; j++)
      {
        for ( i=0 ; i < (*n2) ; i++)
        {
          xm[2*i+2*(*n2)*j]= XScale((i+1.0));
          xm[2*i+1+2*(*n2)*j]=xm[2*i+2*(*n2)*j];

        }
      }
      break ;
  case 'o' :
    if ((int)strlen(xf) >= 2 && xf[1]=='l')
      for (j=0 ; j< (*n1) ; j++)
      {
        for ( i=0 ; i < (*n2) ; i++)
        {

          xm[2*i+2*(*n2)*j]= XLogScale(x[i]);
          xm[2*i+1+2*(*n2)*j]=xm[2*i+2*(*n2)*j];
        }
      }
    else 
      for (j=0 ; j< (*n1) ; j++)
      {
        for ( i=0 ; i < (*n2) ; i++)
        {
          xm[2*i+2*(*n2)*j]= XScale(x[i]);
          xm[2*i+1+2*(*n2)*j]=xm[2*i+2*(*n2)*j];

        }
      }
      break;
  case 'g' :
  default:
    if ((int)strlen(xf) >= 2 && xf[1]=='l')
      for (j=0 ; j< (*n1) ; j++)
      {
        for ( i=0 ; i < (*n2) ; i++)
        {
          xm[2*i+2*(*n2)*j]= XLogScale(x[i+(*n2)*j]);
          xm[2*i+1+2*(*n2)*j]=xm[2*i+2*(*n2)*j];

        }
      }
    else 
      for (j=0 ; j< (*n1) ; j++)
      {
        for ( i=0 ; i < (*n2) ; i++)
        {
          xm[2*i+2*(*n2)*j]= XScale(x[i+(*n2)*j]);
          xm[2*i+1+2*(*n2)*j]=xm[2*i+2*(*n2)*j];
        }
      }
      break;
  }
}
/*--------------------------------------------------------------------------*/
void Plo2d4RealToPixel(integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf)
{
  integer i,j;
  /** Computing y-values **/
  if ((int)strlen(xf) >= 3 && xf[2]=='l')	  
  {
    for ( i=0 ; i < (*n2) ; i++)
      for (j=0 ; j< (*n1) ; j++)
        ym[2*i+2*(*n2)*j]= YLogScale(y[i+(*n2)*j]);
    for ( i=0 ; i < (*n2)-1 ; i++)
      for (j=0 ; j< (*n1) ; j++)
        ym[2*i+1+2*(*n2)*j]=	  ym[2*i+2+2*(*n2)*j];
  }
  else 
  {
    for ( i=0 ; i < (*n2) ; i++)
      for (j=0 ; j< (*n1) ; j++)
        ym[2*i+2*(*n2)*j]= YScale(y[i+(*n2)*j]);
    for ( i=0 ; i < (*n2)-1 ; i++)
      for (j=0 ; j< (*n1) ; j++)
        ym[2*i+1+2*(*n2)*j]=	  ym[2*i+2+2*(*n2)*j];

  }

  /** Computing x-values **/
  switch (xf[0])
  {
  case 'e' :
    /** No X-value given by the user **/
    if ((int)strlen(xf) >= 2 && xf[1]=='l')
    {
      for (j=0 ; j< (*n1) ; j++)
        for ( i=0 ; i < (*n2) ; i++)
          xm[2*i+2*(*n2)*j]= XLogScale(i+1.0);
      for (j=0 ; j< (*n1) ; j++)
        for ( i=0 ; i < (*n2)-1 ; i++)
          xm[2*i+1+2*(*n2)*j]=	   xm[2*i+2+2*(*n2)*j];
    }
    else 
    {
      for (j=0 ; j< (*n1) ; j++)
        for ( i=0 ; i < (*n2) ; i++)
          xm[2*i+2*(*n2)*j]= XScale((i+1.0));
      for (j=0 ; j< (*n1) ; j++)
        for ( i=0 ; i < (*n2)-1 ; i++)
          xm[2*i+1+2*(*n2)*j]=	   xm[2*i+2+2*(*n2)*j];
    }
    break ;
  case 'o' :
    if ((int)strlen(xf) >= 2 && xf[1]=='l')
    {
      for (j=0 ; j< (*n1) ; j++)
        for ( i=0 ; i < (*n2) ; i++)
          xm[2*i+2*(*n2)*j]= XLogScale(x[i]);
      for (j=0 ; j< (*n1) ; j++)
        for ( i=0 ; i < (*n2)-1 ; i++)
          xm[2*i+1+2*(*n2)*j]=	   xm[2*i+2+2*(*n2)*j];
    }
    else 

    {
      for (j=0 ; j< (*n1) ; j++)
        for ( i=0 ; i < (*n2) ; i++)
          xm[2*i+2*(*n2)*j]= XScale(x[i]);
      for (j=0 ; j< (*n1) ; j++)
        for ( i=0 ; i < (*n2)-1 ; i++)
          xm[2*i+1+2*(*n2)*j]=	   xm[2*i+2+2*(*n2)*j];

    }
    break;
  case 'g' :
  default:
    if ((int)strlen(xf) >= 2 && xf[1]=='l')
    {
      for (j=0 ; j< (*n1) ; j++)
        for ( i=0 ; i < (*n2) ; i++)
          xm[2*i+2*(*n2)*j]= XLogScale(x[i+(*n2)*j]);
      for (j=0 ; j< (*n1) ; j++)
        for ( i=0 ; i < (*n2)-1 ; i++)
          xm[2*i+1+2*(*n2)*j]=	   xm[2*i+2+2*(*n2)*j];

    }
    else 
    {
      for (j=0 ; j< (*n1) ; j++)
        for ( i=0 ; i < (*n2) ; i++)
          xm[2*i+2*(*n2)*j]= XScale(x[i+(*n2)*j]);
      for (j=0 ; j< (*n1) ; j++)
        for ( i=0 ; i < (*n2)-1 ; i++)
          xm[2*i+1+2*(*n2)*j]=	xm[2*i+2+2*(*n2)*j];

    }
    break;
  }
}
/*--------------------------------------------------------------------------*/
/**
 * Interface to function xchange "f2i". Convert user 2d coordinates
 * to pixel ones.
 * @param rect [x,y,w,h] of current subwin
 */
void convertUserCoordToPixelCoords(const double xCoords[], const double yCoords[],
                                   int xPixCoords[], int yPixCoords[], int nbCoords,
                                   int rect[4])
{ 
  /* coordinates transformation */
  int i;
  sciPointObj * selectedSubwin = sciGetCurrentSubWin();
  updateSubwinScale(selectedSubwin);
  for (i = 0; i < nbCoords; i++)
  {
    // specify a default value for Z
    double curCoords[3] = {xCoords[i], yCoords[i], 0.0};
    int curPixCoords[2];
    sciGet2dViewPixelCoordinates(selectedSubwin, curCoords, curPixCoords);
    xPixCoords[i] = curPixCoords[0];
    yPixCoords[i] = curPixCoords[1];
  }

  /* get viewing area */
  sciGetViewingArea(selectedSubwin, &rect[0], &rect[1], &rect[2], &rect[3]);
 
}
/*--------------------------------------------------------------------------*/
/**
* Interface to function xchange "i2f". Convert pixel coordinates
* to user 2d coordinates.
* @param rect [x,y,w,h] of current subwin
*/
void convertPixelCoordsToUserCoords(const int xPixCoords[], const int yPixCoords[],
                                    double xUserCoords[], double yUserCoords[], int nbCoords,
                                    int rect[4])
{ 
  /* coordinates transformation */
  int i;
  sciPointObj * selectedSubwin = sciGetCurrentSubWin();
  updateSubwinScale(selectedSubwin);
  for (i = 0; i < nbCoords; i++)
  {
    // specify a default value for Z
    int curPixCoords[2] = {xPixCoords[i], yPixCoords[i]};
    double curCoords[2];
    sciGet2dViewCoordFromPixel(selectedSubwin, curPixCoords, curCoords);
    xUserCoords[i] = curCoords[0];
    yUserCoords[i] = curCoords[1];
  }

  /* get viewing area */
  sciGetViewingArea(selectedSubwin, &rect[0], &rect[1], &rect[2], &rect[3]);

}
/*--------------------------------------------------------------------------*/
