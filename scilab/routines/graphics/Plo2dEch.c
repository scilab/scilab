/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

#include <string.h> /* in case of dbmalloc use */
#include <stdio.h>
#include <math.h>
#include "Math.h" 
#include "Graphics.h"
#include "PloEch.h"
#include "Entities.h"

/*----------------------------------------------
 * A List for storing Window scaling information 
 *----------------------------------------------*/

static void scale_copy __PARAMS((WCScaleList *s1, WCScaleList *s2));
static integer curwin __PARAMS((void));
static void zoom_rect __PARAMS((double,double,double,double));
static void set_scale_win __PARAMS((ScaleList **listptr, integer i, WCScaleList *s));
static WCScaleList *new_wcscale __PARAMS(( WCScaleList *val));
static WCScaleList *check_subwin_wcscale __PARAMS((WCScaleList *listptr, double *));
static int same_subwin __PARAMS((double lsubwin_rect[4],double subwin_rect[4]));
static void set_window_scale __PARAMS((integer i,WCScaleList  *scale));

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

extern void Cscale2default()
{
  scale_copy(&Cscale,&Defscale);  set_window_scale(curwin(),&Cscale);
}

void set_window_scale_with_default(i) int i; { set_window_scale(i,&Defscale);} 

/*------------------------------------------------------------
 * void get_window_scale(i,subwin)
 *   if subwin == NULL : search for existing scale values of window i.
 *   if subwin != NULL : search for existing scale values of subwindow subwin of window i.
 *   If no scale are found we do nothing and return 0
 *   else the scale is copied into current scale Cscale ans we return 1.
 *   
 *-------------------------------------------------------------*/

static int get_scale_win __PARAMS((ScaleList *listptr, integer wi, double *subwin));

int get_window_scale(i,subwin)
     integer i;
     double *subwin;
{ 
  return get_scale_win(The_List,Max(0L,i),subwin);
}

static int get_scale_win(listptr, wi,subwin)
     ScaleList *listptr;
     integer wi;
     double *subwin;
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

static void set_window_scale(i,scale)
     integer i;
     WCScaleList  *scale;
{ 
  set_scale_win(&The_List,Max(0L,i),scale);
}

static void set_scale_win(listptr, i,scale)
     ScaleList **listptr;
     integer i;
     WCScaleList *scale;
{
  if ( *listptr == (ScaleList  *) NULL)
    {
      /* window i does not exist add an entry for it */
      if ((*listptr = (ScaleList *) MALLOC( sizeof (ScaleList)))==0)
	{
	  Scistring("AddWindowScale_ memory exhausted\n");
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
	      Scistring("AddWindowScale_ memory exhausted\n");
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
	      Scistring("AddWindowScale_ memory exhausted\n");
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
 
static WCScaleList *new_wcscale(val) 
     WCScaleList *val;
{
  WCScaleList *new ;
  if ((new = (WCScaleList *) MALLOC( sizeof (WCScaleList))) == NULL) 
    return NULL;
  new->next = (WCScaleList *) 0;
  new->prev = (WCScaleList *) 0;
  scale_copy(new,val);
  return new;
}

static WCScaleList *check_subwin_wcscale(listptr,subwin_rect)
     WCScaleList *listptr;
     double subwin_rect[4];
{
  if ( listptr == (WCScaleList  *) NULL)  return NULL;
  if ( same_subwin( listptr->subwin_rect,subwin_rect)) 
    return listptr;
  else 
    return check_subwin_wcscale(listptr->next,subwin_rect);
}

static int same_subwin( lsubwin_rect,subwin_rect)
     double lsubwin_rect[4],subwin_rect[4];
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

static void DeleteWCScale __PARAMS((WCScaleList *l));

void del_window_scale(i)
     integer i;
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

static void DeleteWCScale(l) 
     WCScaleList *l;
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

static void scale_copy(s1,s2) 
     WCScaleList *s1,*s2;
{
  int i,j;
  s1->flag=s2->flag;
  s1->wdim[0]=  s2->wdim[0];
  s1->wdim[1]=  s2->wdim[1];
  for (i=0; i< 4; i++) 
    {
      s1->subwin_rect[i]=s2->subwin_rect[i];
      s1->frect[i]=s2->frect[i];
      s1->WIRect1[i]=s2->WIRect1[i];
      s1->Waaint1[i]=s2->Waaint1[i];
      s1->xtics[i]=s2->xtics[i];
      s1->ytics[i]=s2->ytics[i];
      s1->axis[i]=s2->axis[i];
    }
  for (i=0; i< 3; i++) 
    for (j=0; i< 3; i++) 
      s1->m[i][j]=s2->m[i][j];
  for (i=0; i< 6; i++) 
    s1->bbox1[i]=s2->bbox1[i] ;
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

static integer curwin()
{
  integer verbose=0,narg,winnum;
  C2F(dr)("xget","window",&verbose,&winnum,&narg ,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  return(winnum);
}

/*-------------------------------------------
 * show the recorded scales 
 *-------------------------------------------*/

static void show_scales __PARAMS((ScaleList *listptr));

void ShowScales()
{ 
  sciprint("-----------scales-------------\r\n");
  show_scales(The_List);
  sciprint("----------current scale-------\r\n");
  sciprint("\tsubwin=[%5.2f,%5.2f,%5.2f,%5.2f], flag=%d\r\n",
	   Cscale.subwin_rect[0],Cscale.subwin_rect[1],Cscale.subwin_rect[2],Cscale.subwin_rect[3],
	   Cscale.flag);
  sciprint("-----------end----------------\r\n");
}

static void show_scales(listptr)
     ScaleList *listptr;
{
  if (listptr != (ScaleList  *) NULL)
    { 
      WCScaleList *loc = listptr->scales;
      sciprint("Window %d \r\n",listptr->Win);
      while ( loc != NULL) 
	{
	  sciprint("\tsubwin=[%5.2f,%5.2f,%5.2f,%5.2f], flag=%d\r\n",
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

int C2F(setscale2d)(WRect,FRect,logscale,l1)
     double FRect[4], WRect[4];
     char *logscale;
     integer l1;
{
  static integer aaint[]={2,10,2,10};
  if (GetDriver()=='R') StoreEch("scale",WRect,FRect,logscale);
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

int C2F(Nsetscale2d)(WRect,ARect,FRect,logscale,l1)
     double FRect[4], WRect[4],ARect[4];
     char *logscale;
     integer l1;
{
  /* if some arguments are null pointer we set them to 
   * the corresponding Cscale value. 
   * this is only important for StoreNEch which do not work with null arguments 
   */ 
  /* char flag[] = "tfffff";*/ /* flag to specify which arguments have changed*/
  /* 14/03/2002*/
  sciPointObj *masousfen;
	
  char flag[7];
  strcpy(flag,"tfffff");  
 
  /** voir aussi si xsetech (frect) **/ 
  if ( WRect != NULL) 
    {
      /* Ajout djalel */
      if (version_flag() == 0)
        {
	  if (( masousfen = sciIsExistingSubWin (WRect)) != (sciPointObj *)NULL)
	    sciSetSelectedSubWin(masousfen);
	  else if ((masousfen = ConstructSubWin (sciGetCurrentFigure(), 0)) != NULL)
	    {
	    sciSetSelectedSubWin(masousfen);
            pSUBWIN_FEATURE (masousfen)->WRect[0]   = WRect[0];
	    pSUBWIN_FEATURE (masousfen)->WRect[1]   = WRect[1];
	    pSUBWIN_FEATURE (masousfen)->WRect[2]   = WRect[2];
            pSUBWIN_FEATURE (masousfen)->WRect[3]   = WRect[3];
            }
	} 
      /** */
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
  else WRect = Cscale.subwin_rect; 
  if (version_flag() == 0)
    if (FRect != NULL)
	    { masousfen=sciGetSelectedSubWin(sciGetCurrentFigure());
	      pSUBWIN_FEATURE (masousfen)->FRect[0]   = FRect[0];
	      pSUBWIN_FEATURE (masousfen)->FRect[1]   = FRect[1];
	      pSUBWIN_FEATURE (masousfen)->FRect[2]   = FRect[2];
	      pSUBWIN_FEATURE (masousfen)->FRect[3]   = FRect[3];
	    }
  if ( FRect != NULL) flag[2]='t'; else FRect = Cscale.frect;
  if ( ARect != NULL) flag[5]='t'; else ARect = Cscale.axis;
  if ( logscale != NULL) flag[4] ='t'; else logscale = Cscale.logflag;
  if ( flag[4] == 't' && flag[2] == 't' ) 
    {
      if (logscale[0]=='l') 
	{
	  if ( FRect[0] <= 0 || FRect[2] <= 0 ) 
	    {
	      Scistring("Warning: negative boundaries on x scale with a log scale \n");
	      FRect[0]=1.e-8;FRect[2]=1.e+8;
	    } 
	  FRect[0]=log10(FRect[0]);
	  FRect[2]=log10(FRect[2]);
	}
      if (logscale[1]=='l') 
	{
	  if ( FRect[1] <= 0 || FRect[3] <= 0 ) 
	    {
	      Scistring("Warning: negative boundaries on y scale with a log scale \n");
	      FRect[1]=1.e-8;FRect[3]=1.e+8;
	    } 
	  FRect[1]=log10(FRect[1]);
	  FRect[3]=log10(FRect[3]);
	}
    }
  if ((GetDriver()=='R') && (version_flag() != 0)) StoreNEch("nscale",flag,WRect,ARect,FRect,logscale);
  set_scale(flag,WRect,FRect,NULL,logscale,ARect);  
  return(0);
}

/* used to send values to Scilab */

int getscale2d(WRect,FRect,logscale,ARect)
     double FRect[4],WRect[4],ARect[4];
     char *logscale;
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

void set_scale(flag,subwin,frame_values,aaint,logflag,axis_values)
     char flag[6];            /* flag[i] = 't' or 'f' */
     double  subwin[4];       /* subwindow specification */
     double  frame_values[4]; /* [xmin,ymin,xmax,ymax] */
     integer aaint[4];        /* [xint,x_subint,y_int,y_subint]*/
     char logflag[2];         /* [xlogflag,ylogflag] */
     double axis_values[4];   /* [mfact_xl, mfact_xr,mfact_yu,mfact_yd]; */
{
  char c;
  char wdim_changed= 'f',subwin_changed='f';
  char frame_values_changed='f',aaint_changed='f';
  char logflag_changed='f';
  char axis_changed = 'f';
  integer  verbose=0,narg,wdim[2];
  int i;
  if ( flag[0] == 't'  ) 
    {
      C2F(dr)("xget","wdim",&verbose,wdim,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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

      Cscale.WIRect1[0] = XScale( Cscale.frect[0]);
      Cscale.WIRect1[1] = YScale( Cscale.frect[3]);
      Cscale.WIRect1[2] = Abs(XScale( Cscale.frect[2]) -  XScale( Cscale.frect[0]));
      Cscale.WIRect1[3] = Abs(YScale( Cscale.frect[3]) -  YScale( Cscale.frect[1]));

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
 
  /** Cscale changes are copied int current window scale */
  if ( (c=GetDriver()) == 'X' ||  c == 'R' || c == 'I' || c == 'G' || c == 'W' )
    {
      set_window_scale(curwin(),&Cscale);
    }  
}

/*--------------------------------------------------------------------
 * Get the current window dimensions.
 *--------------------------------------------------------------------*/

void get_cwindow_dims(wdims)
     int wdims[2];
{
  int verbose=0,narg;
  C2F(dr)("xget","wdim",&verbose,wdims,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}

/*--------------------------------------------------------------------
 * use current scale to set the clipping rectangle 
 *--------------------------------------------------------------------*/

void frame_clip_on()
{
  C2F(dr)("xset","clipping",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],
	  &Cscale.WIRect1[3],PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}

void frame_clip_off()
{
  C2F(dr)("xset","clipoff",PI0,PI0,PI0,PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}

/*--------------------------------------------------------------------
 * Convert pixel<->double using current scale 
 * 
 *  C2F(echelle2d)(x,y,x1,y1,n1,n2,rect,dir)
 *    x,y,x1,y1 of size [n1*n2] 
 *    rect[4] : the frame boundaries in pixel are returned in rect 
 *    dir     : "f2i" -> double to integer (you give x and y and get x1,y1)
 *            : "i2f" -> integer to double (you give x1 and y1 and get x,y)
 *    lstr    : unused (Fortran/C) 
 * --------------------------------------------------------------------------*/

int C2F(echelle2d)(x,y,x1,yy1,n1,n2,dir,lstr)
     double x[],y[];
     integer x1[],yy1[],*n1,*n2,lstr;
     char dir[];
{
  int n=(*n1)*(*n2);
  C2F(xechelle2d)(x,x1,&n,dir,lstr);
  C2F(yechelle2d)(y,yy1,&n,dir,lstr);
  return(0);
}

/* for x only */

int C2F(xechelle2d)(x,x1,n1,dir,lstr)
     double x[];
     integer x1[],*n1;
     char dir[];
     integer lstr;
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
   sciprint(" Wrong dir %s argument in echelle2d\r\n",dir);
  
  return(0);
}

/* for y only */

int C2F(yechelle2d)(y,yy1,n2,dir,lstr)
     double y[];
     integer yy1[],*n2;
     char dir[];
     integer lstr;
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
    sciprint(" Wrong dir %s argument in echelle2d\r\n",dir); 
  
  return(0);
}

/*--------------------------------------------------------------------
 * void C2F(echelle2dl)(x, y, x1, yy1, n1, n2,  dir)
 * like echelle2d but for length convertion 
 * Note that it cannot work in logarithmic scale 
 *--------------------------------------------------------------------*/

void C2F(echelle2dl)(x, y, x1, yy1, n1, n2,  dir)
     double  x[],y[];
     integer x1[],yy1[],*n1,*n2;
     char *dir;
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
    sciprint(" Wrong dir %s argument in echelle2d\r\n",dir);
}

/** meme chose mais pour transformer des ellipses **/

void C2F(ellipse2d)(x, x1, n, dir)
     double x[];
     integer x1[],*n;
     char *dir;
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
    sciprint(" Wrong dir %s argument in echelle2d\r\n",dir);
}

/** meme chose mais pour transformer des rectangles **/

void C2F(rect2d)(x, x1, n, dir)
     double x[];
     integer x1[],*n;
     char *dir;
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
    sciprint(" Wrong dir %s argument in echelle2d\r\n",dir);
}

 
/** meme chose mais pour axis **/

void C2F(axis2d)(alpha, initpoint, size, initpoint1, size1)
     double *alpha;
     double *initpoint;
     double *size;
     integer *initpoint1;
     double *size1;
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

void zoom_get_rectangle(bbox)
     double bbox[4];
{
  /* Using the mouse to get the new rectangle to fix boundaries */
  integer th,th1=1;
  integer pixmode,alumode,color,style[10],fg,verbose=0,narg;
  integer ibutton,in,iwait=0,istr=0, noir=33;
  integer modes[2];
  double x0,yy0,x,y,xl,yl;

  modes[0]=1;modes[1]=0; /* for xgemouse only get mouse mouvement*/ 

  C2F(dr)("xget","pixmap",&verbose,&pixmode,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","alufunction",&verbose,&alumode,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","thickness",&verbose,&th,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","color",&verbose,&color,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","line style",&verbose,style,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","foreground",&verbose,&fg,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

#ifdef WIN32
  SetWinhdc();
  SciMouseCapture();
#endif 
  C2F(SetDriver)("X11",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  C2F(dr)("xset","thickness",&th1,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","line style",(in=1,&in),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","color",&fg,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
 
  /** XXXXXX : a regler pour Win32 in = 6 **/
  C2F(dr1)("xset","alufunction",(in=6,&in),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr1)("xclick","one",&ibutton,&iwait,&istr,PI0,PI0,PI0,&x0,&yy0,PD0,PD0,0L,0L);
  x=x0;y=yy0;
  ibutton=-1;
  while ( ibutton == -1 ) 
    {
      /* dessin d'un rectangle */
      C2F (dr) ("xset", "color",&noir,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0, PD0,0L,0L);
      zoom_rect(x0,yy0,x,y);
      if ( pixmode == 1) C2F(dr1)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr1)("xgetmouse","one",&ibutton,&iwait,PI0,PI0,modes,PI0,&xl, &yl,PD0,PD0,0L,0L);
      /* effacement du rectangle */
      zoom_rect(x0,yy0,x,y);
      if ( pixmode == 1) C2F(dr1)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      x=xl;y=yl;
    }   
#ifndef WIN32
  /** XXXX */
  C2F(dr1)("xset","alufunction",(in=3,&in),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
#endif
  /* Back to the default driver which must be Rec and redraw the recorded
   * graphics with the new scales 
   */
  bbox[0]=Min(x0,x);
  bbox[1]=Min(yy0,y);
  bbox[2]=Max(x0,x);
  bbox[3]=Max(yy0,y);
  C2F(dr1)("xset","alufunction",&alumode,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","thickness",&th,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","line style",style,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","color",&color,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

#ifdef WIN32
  ReleaseWinHdc();
  SciMouseRelease();
#endif 

}



void zoom()
{
  char driver[4];
  integer aaint[4],flag[2]; /* ansi : ={1,0};*/
  integer verbose=0,narg,ww; 
  /** 01/07/2002 ***/
  sciPointObj *psousfen,*tmpsousfen;
  sciSons *psonstmp;  

  double fmin,fmax,lmin,lmax;  
  integer min,max,puiss,deux=2,dix=10,box1[4],section[4];
 
 
  flag[0] =1 ; flag[1]=0;
  C2F(dr1)("xget","window",&verbose,&ww,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  GetDriver1(driver,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  if (strcmp("Rec",driver) != 0) 
    {
      Scistring("\n Use the Rec driver to zoom " );
      return;
    }
  else 
    {
      double bbox[4];
 
      zoom_get_rectangle(bbox);   
      C2F(SetDriver)(driver,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      C2F(dr)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
      if (version_flag() == 0){  
	/***** 02/10/2002 ****/
        integer box[4];
       
        box[0]= Min(XDouble2Pixel(bbox[0]),XDouble2Pixel(bbox[2])); 
        box[2]= Max(XDouble2Pixel(bbox[0]),XDouble2Pixel(bbox[2]));
        box[1]= Min(YDouble2Pixel(bbox[1]),YDouble2Pixel(bbox[3])); 
        box[3]= Max(YDouble2Pixel(bbox[1]),YDouble2Pixel(bbox[3]));
       
        
        tmpsousfen= (sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure());
        psonstmp = sciGetSons (sciGetCurrentFigure());
        while (psonstmp != (sciSons *) NULL)	
          {  
           if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
             {
                psousfen= (sciPointObj *)psonstmp->pointobj;
                 sciSetSelectedSubWin(psousfen);
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
		 }
	     }     
           psonstmp = psonstmp->pnext;
          }
          sciSetSelectedSubWin(tmpsousfen);
          sciSetReplay(1);
	  sciDrawObj(sciGetCurrentFigure());
	  sciSetReplay(0);
         }
        else 
           Tape_ReplayNewScale(" ",&ww,flag,PI0,aaint,PI0,PI0,bbox,PD0,PD0,PD0);
	  
    }
}


extern void unzoom()
{
  char driver[4];
  integer ww,verbose=0,narg; 
  /** 17/09/2002 ***/  
  double fmin,fmax,lmin,lmax;  
  integer min,max,puiss,deux=2,dix=10;
  sciPointObj *psousfen;
  sciSons *psonstmp;  

  
  GetDriver1(driver,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  if (strcmp("Rec",driver) != 0) 
    {
      Scistring("\n Use the Rec driver to unzoom " );
      return;
    }
  else 
    {
      C2F(dr1)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr1)("xget","window",&verbose,&ww,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if (version_flag() == 0){ 
         /***** 02/10/2002 ****/
        psonstmp = sciGetSons (sciGetCurrentFigure());
        while (psonstmp != (sciSons *) NULL)	
          {  
           if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
             {
                psousfen= (sciPointObj *)psonstmp->pointobj;
        	if (sciGetZooming(psousfen))
	            {
		      sciSetZooming(psousfen, 0);
		      pSUBWIN_FEATURE (psousfen)->FRect[0]   = pSUBWIN_FEATURE (psousfen)->FRect_kp[0];
		      pSUBWIN_FEATURE (psousfen)->FRect[1]   = pSUBWIN_FEATURE (psousfen)->FRect_kp[1];
		      pSUBWIN_FEATURE (psousfen)->FRect[2]   = pSUBWIN_FEATURE (psousfen)->FRect_kp[2];
		      pSUBWIN_FEATURE (psousfen)->FRect[3]   = pSUBWIN_FEATURE (psousfen)->FRect_kp[3];
		    }
	  
		/** regraduation de l'axe des axes ***/
		fmin= pSUBWIN_FEATURE (psousfen)->FRect[0];
		fmax= pSUBWIN_FEATURE (psousfen)->FRect[2]; 
		C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
		pSUBWIN_FEATURE(psousfen)->axes.xlim[2]=puiss; 
          
		fmin= pSUBWIN_FEATURE (psousfen)->FRect[1];
		fmax= pSUBWIN_FEATURE (psousfen)->FRect[3]; 
		C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
		pSUBWIN_FEATURE(psousfen)->axes.ylim[2]=puiss;
		/*****/  
               }     
              psonstmp = psonstmp->pnext;
            } 
        
          sciSetReplay(1);
	  sciDrawObj(sciGetCurrentFigure());
	  sciSetReplay(0);
    }
   else
    Tape_ReplayUndoScale("v",&ww);
    }
}

/**
  Win32, warning when using xor mode
  colors are changed and black is turned to white
  so we must use an other pattern than the black one
  inside dbox
  **/

static void zoom_rect(x0, yy0, x, y)
     double x0;
     double yy0;
     double x;
     double y;
{
  double xi,yi,w,h;
#ifdef WIN32
  integer verbose=0,pat,pat1=3,narg;
  C2F(dr)("xget","pattern",&verbose,&pat,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","pattern",&pat1,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
#endif
  xi=Min(x0,x);
  w=Abs(x0-x);
  yi=Max(yy0,y);
  h=Abs(yy0-y);
  C2F(dr1)("xrect","v",PI0,PI0,PI0,PI0,PI0,PI0,&xi,&yi,&w,&h,0L,0L);
#ifdef WIN32
  C2F(dr)("xset","pattern",&pat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
#endif
}

/* 
 *  FRectI=[xmin,ymin,xmax,ymax] est transforme de 
 *  facon a avoir une graduation simple et reguliere 
 *  Xdec,Ydec,xnax,ynax
 *  caracterisant cette graduation 
 *  (voir les fonctions qui suivent )
 */

void Gr_Rescale(logf, FRectI, Xdec, Ydec, xnax, ynax)
     char *logf;
     double *FRectI;
     integer *Xdec;
     integer *Ydec;
     integer *xnax;
     integer *ynax;
{
  double FRectO[4];
  sciPointObj *psubwin; 
  int i;

  /** 18/06/2002 **/
  if (version_flag() == 0){
      psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
      for (i=0;i<4 ; i++)
	pSUBWIN_FEATURE (psubwin)->axes.limits[i+1]=FRectI[i];}
     
    
  if (logf[0] == 'n') 
    { 
      if ((version_flag() != 0) || ( pSUBWIN_FEATURE (psubwin)->axes.limits[0] !=1))
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
  if (logf[1] == 'n') 
    {
     if ((version_flag() != 0) || ( pSUBWIN_FEATURE (psubwin)->axes.limits[0] !=1))
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

