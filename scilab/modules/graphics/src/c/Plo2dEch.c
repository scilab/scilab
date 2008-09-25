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
#include "PloEch.h"
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

extern void xgetmouse2(char *fname, char *str, int *ibutton, int *iflag, int *x1, int *yy1, int *x6, int *x7, double *x, double *y, double *dx3, double *dx4, int lx0, int lx1);
extern void xclick_2(char *fname, char *str, int *ibutton, int *iflag, int *istr, int *x1, int *yy1, int *x7, double *x, double *y, double *dx3, double *dx4, int lx0, int lx1);

/*----------------------------------------------
 * A List for storing Window scaling information 
 *----------------------------------------------*/

static void scale_copy (WCScaleList *s1, WCScaleList *s2);
static int curwin (void);
static void set_scale_win (ScaleList **listptr, int i, WCScaleList *s);
static WCScaleList *new_wcscale ( WCScaleList *val);
static WCScaleList *check_subwin_wcscale (WCScaleList *listptr, double *);
static int same_subwin (double lsubwin_rect[4],double subwin_rect[4]);
static void set_window_scale (int i,WCScaleList  *scale);

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



/*------------------------------------------------------------
 * void get_window_scale(i,subwin)
 *   if subwin == NULL : search for existing scale values of window i.
 *   if subwin != NULL : search for existing scale values of subwindow subwin of window i.
 *   If no scale are found we do nothing and return 0
 *   else the scale is copied into current scale Cscale ans we return 1.
 *   
 *-------------------------------------------------------------*/

static int get_scale_win (ScaleList *listptr, int wi, double *subwin);

int get_window_scale( int i, double * subwin )
{ 
  return get_scale_win(The_List,Max(0L,i),subwin);
}

static int get_scale_win(ScaleList *listptr,int wi,double *subwin)
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

static void set_window_scale(int i,WCScaleList  *scale)
{ 
  set_scale_win(&The_List,Max(0L,i),scale);
}

static void set_scale_win(ScaleList **listptr, int i, WCScaleList *scale)
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

static int curwin( void )
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
                      int   l1       )
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
  int i;
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
                int aaint[4]       ,
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
  int wdim[2];
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


extern void unzoom()
{
  /** 17/09/2002 ***/
  double fmin,fmax,lmin,lmax;
  int min,max,puiss,deux=2,dix=10;
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
