/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 INRIA
 --------------------------------------------------------------------------*/

#include <string.h> /* in case of dbmalloc use */
#include <stdio.h>
#include <math.h>
#include "Math.h"
#include "PloEch.h"

#define spINSIDE_SPARSE
#if defined(THINK_C) || defined (__MWERKS__)
#include ":sparse:spConfig.h" 
#else
#include "../sparse/spConfig.h"
#endif

#include "SetProperty.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "DrawObjects.h"
#include "BuildObjects.h"

#if WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

extern void initsubwin();
void compute_data_bounds(int cflag, char dataflag,double *x,double *y,integer n1,integer n2,double *drect);
extern double  sciFindLogMinSPos(double *x, int n);
void compute_data_bounds2(int cflag,char dataflag, char * logflags, double *x,double  *y, integer n1,integer n2, double *drect);
BOOL update_specification_bounds(sciPointObj *psubwin, double *rect,int flag);
int re_index_brect(double * brect, double * drect);
extern BOOL strflag2axes_properties(sciPointObj * psubwin, char * strflag);
extern char ** FreeUserLabels(char ** u_xlabels, int *u_nxgrads);
extern double * FreeUserGrads(double * u_xgrads);
extern double * AllocUserGrads(double * u_xgrads, int nb);
extern int CopyUserGrads(double *u_xgrad_SRC, double *u_xgrad_DEST, int dim);
extern char ** AllocAndSetUserLabels(char ** u_xlabels, double * u_xgrads, int u_nxgrads, char logflag);
extern char ** AllocAndSetUserLabelsFromMdl(char ** u_xlabels, char ** u_xlabels_MDL, int u_nxgrads);
extern int CreatePrettyGradsFromNax(sciPointObj * psubwin,int * Nax);
extern int GraduateWithNax(sciSubWindow * ppsubwin,double *min,double *max,int nbtics, double * grads);
int ChooseGoodFormat(char * c_format,char logflag, double *_grads,int n_grads);
/*--------------------------------------------------------------------
 *  plot2dn(ptype,Logflags,x,y,n1,n2,style,strflag,legend,brect,aaint,lstr1,lstr2)
 *  
 *  Draw *n1 curves of *n2 points each
 *
 *  ptype is an integer which gives the polyline drawind mode (0,1,2,3,4)
 *
 *  Logflags is a two character string
 *
 *  (x[i+(*n2)*j] ,y[i+(*n2)*j]) Double values giving the point
 *  position of point i of curve j (i=0,*n2-1 j=0,*n1-1)
 *
 *  style[*n1]-> give the style to use for each curve 
 *     if style is positive --> a mark is used (mark id = style[i])
 *     if style is strictly negative --> a dashed line is used 
 *        (dash id = abs(style[i])
 *     if there's only one curve, style can be of type style[0]=style,
 *     style[1]=pos ( pos in [1,6]) 
 *     pos give the legend position (1 to 6) (this can be iteresting
 *     if you want to superpose curves with different legends by 
 *     calling plot2d more than one time.
 *
 *  strflag[3] is a string
 *  
 *     if strflag[0] == '1' then legends are added 
 *        legend = "leg1@leg2@....@legn"; gives the legend for each curve
 *      else no legend
 *
 *     if strflag[1] == '1' then  the values of brect are used to fix 
 *        the drawing boundaries :  brect[]= <xmin,ymin,xmax,ymax>;
 *      if strflag[1] == '2' then the values  are computed from data
 *      else if strflag[1]=='0' the previous values 
 *                (previous call or defaut values) are used 
 *
 *     if  strflag[2] == '1' ->then an axis is added
 *        the number of intervals 
 *        is specified by the vector aaint[4] of integers 
 *         <aaint[0],aaint[1]> specifies the x-axis number of  points 
 *         <aaint[2],aaint[3]> same for y-axis
 *     if  strflag[2] == '2' -> no axis, only a box around the curves
 *     else no box and no axis 

 * lstr* : unused ( but used by Fortran ) 
 *--------------------------------------------------------------------------*/
  
int plot2dn(integer ptype,char *logflags,double *x,double *y,integer *n1,integer *n2,integer *style,char *strflag,char *legend,double *brect,integer *aaint,BOOL flagNax, integer lstr1,integer lstr2)
{
  int closeflag = 0;
  int jj = 0;
  sciPointObj **pptabofpointobj;
  sciPointObj  *psubwin;
  long hdl;
  long *hdltab;
  int cmpt=0,ok/*,i*/;
  int with_leg;
  double drect[6];
  char dataflag/*,frameflag*/;
  sciSubWindow * ppsubwin = NULL;
  BOOL bounds_changed = FALSE;
  BOOL axes_properties_changed = FALSE;

  psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
  ppsubwin = pSUBWIN_FEATURE(psubwin);
  ok=0;  
  if (sciGetSurface(psubwin) != (sciPointObj *) NULL)   ok=1;

  if (!(sciGetGraphicMode (psubwin)->addplot)) { 
    sciXbasc(); 
    initsubwin(); 	/* Pb here Re-init for the psubwin does not work properly F.Leray 24.02.04*/
    sciRedrawFigure();
    psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ()); 
  } 
  
  
  if (sciGetSurface(psubwin) == (sciPointObj *) NULL) /* F.Leray 18.05.04 */
    {
      pSUBWIN_FEATURE (psubwin)->is3d = FALSE;
      pSUBWIN_FEATURE (psubwin)->project[2]= 0;
    }
  else
    {
      pSUBWIN_FEATURE (psubwin)->theta_kp=pSUBWIN_FEATURE (psubwin)->theta;
      pSUBWIN_FEATURE (psubwin)->alpha_kp=pSUBWIN_FEATURE (psubwin)->alpha;
    }
  
  pSUBWIN_FEATURE (psubwin)->alpha  = 0.0;
  pSUBWIN_FEATURE (psubwin)->theta  = 270.0;
  
  if (sciGetSurface(psubwin) != (sciPointObj *) NULL){
    if(sciGetCurrentScilabXgc () != (struct BCG *) NULL)
      UpdateSubwinScale(psubwin);
    pSUBWIN_FEATURE (psubwin)->is3d = FALSE;
  }


  /* Force psubwin->axes.aaint to those given by argument aaint*/
  /* F.Leray 07.10.04 REMOVE AAINT*/
 /*  for (i=0;i<4;i++) pSUBWIN_FEATURE(psubwin)->axes.aaint[i] = aaint[i];  */

  

  /* Force psubwin->logflags to those given by argument*/
  if (pSUBWIN_FEATURE(psubwin)->FirstPlot == TRUE){
    pSUBWIN_FEATURE (psubwin)->logflags[0]=logflags[1];
    pSUBWIN_FEATURE (psubwin)->logflags[1]=logflags[2];
  }

  /* Force "cligrf" clipping */
  sciSetIsClipping (psubwin,0); 

  /* Force  axes_visible property */
  /*pSUBWIN_FEATURE (psubwin)->isaxes  = TRUE;*/ /* WHY ??? */

  if (sciGetGraphicMode (psubwin)->autoscaling) {
    /* compute and merge new specified bounds with psubwin->Srect */
    switch (strflag[1])  {
    case '0': 
      /* do not change psubwin->Srect */
      break;
    case '1' : case '3' : case '5' : case '7':
      /* Force psubwin->Srect=brect */
      re_index_brect(brect, drect);
      break;
    case '2' : case '4' : case '6' : case '8': case '9':
      /* Force psubwin->Srect to the x and y bounds */
      if ( (int)strlen(logflags) < 1) dataflag='g' ; else dataflag=logflags[0];
      compute_data_bounds2(0,dataflag,pSUBWIN_FEATURE (psubwin)->logflags,x,y,*n1,*n2,drect);
      break;
    }
    if (!pSUBWIN_FEATURE(psubwin)->FirstPlot && 
	(strflag[1] == '5' || strflag[1] == '7' || strflag[1] == '8' || strflag[1] == '9')) { /* merge psubwin->Srect and drect */
      
      drect[0] = Min(pSUBWIN_FEATURE(psubwin)->SRect[0],drect[0]); /*xmin*/
      drect[2] = Min(pSUBWIN_FEATURE(psubwin)->SRect[2],drect[2]); /*ymin*/
      drect[1] = Max(pSUBWIN_FEATURE(psubwin)->SRect[1],drect[1]); /*xmax*/
      drect[3] = Max(pSUBWIN_FEATURE(psubwin)->SRect[3],drect[3]); /*ymax*/
      
    }
    if (strflag[1] != '0')
      bounds_changed = update_specification_bounds(psubwin, drect,2);
  } 
  
  if(pSUBWIN_FEATURE (psubwin)->FirstPlot == TRUE) bounds_changed = TRUE;
  
  axes_properties_changed = strflag2axes_properties(psubwin, strflag);
     
  pSUBWIN_FEATURE (psubwin)->FirstPlot = FALSE; /* just after strflag2axes_properties */
   
  with_leg= (strflag[0] == '1');
  pSUBWIN_FEATURE (psubwin)->with_leg = with_leg;

  /* F.Leray 07.10.04 : trigger algo to init. manual graduation u_xgrads and 
     u_ygrads if nax (in matdes.c which is == aaint HERE) was specified */
  
  ppsubwin->flagNax = flagNax; /* store new value for flagNax */

  if(ppsubwin->flagNax == TRUE){
    if(ppsubwin->logflags[0] == 'n' && ppsubwin->logflags[1] == 'n')
      {
	ppsubwin->axes.auto_ticks[0] = FALSE; /* x and y graduations are imposed by Nax */
	ppsubwin->axes.auto_ticks[1] = FALSE;
	
	CreatePrettyGradsFromNax(psubwin,aaint);
      }
    else{
      sciprint("Warning : Nax does not work with logarithmic scaling\n");}
  }
  
  if(bounds_changed == TRUE || axes_properties_changed == TRUE)
    sciDrawObj(sciGetCurrentFigure());
  /*     EraseAndOrRedraw(psubwin); /\* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 *\/ */
  
  /*---- Drawing the curves and the legends ----*/
  if ( *n1 != 0 ) {
    frame_clip_on ();
    if ((hdltab = MALLOC ((*n1+2) * sizeof (long))) == NULL) {
      sciprint ("Running out of memory for plot2d\n");
      return 0;   
    }
    if (with_leg) {
      /* pptabofpointobj allocated for legends */
      if ((pptabofpointobj = MALLOC((*n1)*sizeof(sciPointObj*))) == NULL) {
        sciprint ("Running out of memory for plot2d\n");
        FREE(hdltab);
        return 0;
      }
    }
    for (jj = 0;jj < *n1; jj++) {/*A.Djalel 3D axes*/
      sciPointObj * pobj = NULL;
      if (style[jj] > 0) { 
	sciSetCurrentObj (ConstructPolyline
			  ((sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure()),&(x[jj*(*n2)]),
			   &(y[jj*(*n2)]),PD0,closeflag,*n2,*n1,ptype,
			   &style[jj],NULL,NULL,NULL,NULL,TRUE,FALSE,FALSE,FALSE));
      }
      else {
	int minusstyle = -style[jj];
	sciSetCurrentObj (ConstructPolyline
			  ((sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure()),&(x[jj*(*n2)]),
			   &(y[jj*(*n2)]),PD0,closeflag,*n2,*n1,ptype,
			   NULL,NULL,&minusstyle,NULL,NULL,FALSE,FALSE,TRUE,FALSE));
      }
      pobj = sciGetCurrentObj();
      
      if (with_leg) pptabofpointobj[jj] = pobj;
      sciDrawObjIfRequired(pobj);
      
      hdl=sciGetHandle(pobj);
      hdltab[cmpt]=hdl;
      cmpt++;
    }
    
    DrawAxesIfRequired(sciGetCurrentObj ()); /* force axes redrawing once is sufficient (F.Leray 10.01.05) */
    
    frame_clip_off ();

    /*---- Drawing the Legends ----*/
    if (with_leg) {
      sciSetCurrentObj (ConstructLegend
                        ((sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure()),
			 legend, strlen(legend), *n1, style, pptabofpointobj)); 
      hdl=sciGetHandle(sciGetCurrentObj ());   
      hdltab[cmpt]=hdl;
      cmpt++;
      Legends (style, n1, legend);  
      FREE(pptabofpointobj);
    }

    /*---- construct Compound ----*/
    sciSetCurrentObj(ConstructCompound (hdltab, cmpt)); 
    FREE(hdltab);
    if (ok==1) {
      Merge3d(psubwin);
      sciDrawObj(sciGetCurrentFigure ());
      /*       sciDrawObj(sciGetSelectedSubWin (sciGetCurrentFigure ())); */
    }
    return(0);
  }
  

  /*  sciDrawObj(sciGetCurrentFigure ());*/
  return(0);
}




/* Given two set of coordinates x and y this routine computes the corresponding 
 *  data bounds rectangle drect=[xmin,ymin,xmax,ymax] taking into account the logflag
 *  -> means we have to find among the data the min > 0.
 */
void compute_data_bounds2(int cflag,char dataflag, char * logflags, double *x,double  *y, integer n1,integer n2, double *drect)
{
  int size_x,size_y;
  double xd[2];
  double *x1;
  switch ( dataflag ) {
  case 'e' : 
    xd[0] = 1.0; xd[1] = (double)n2;
    x1 = xd;size_x = (n2 != 0) ? 2 : 0 ;
    break; 
  case 'o' : 
    x1 = x;size_x = n2;
    break;
  case 'g' : 
  default  : 
    x1 = x;size_x = (cflag == 1) ? n1 : (n1*n2) ;
    break; 
  }

  if (size_x != 0) {
    if(logflags[0] != 'l'){
      drect[0] =  Mini(x1, size_x); 
      drect[1] =  Maxi(x1,size_x); 
    }
    else { /* log. case */
      drect[0] =  sciFindLogMinSPos(x1,size_x); 
      drect[1] =  Maxi(x1,size_x); 
    }
    
  }
  else {
    if(logflags[0] != 'l'){
      drect[0] = 0.0;
      drect[1] = 10.0;
    }
    else{/* log. case */
      drect[0] = 1.0;
      drect[1] = 10.0;
    }
  }

  size_y = (cflag == 1) ? n2 : (n1*n2) ;
  if (size_y != 0) {
    if(logflags[1] != 'l'){
      drect[2] =  Mini(y, size_y); 
      drect[3] =  Maxi(y,size_y); 
    }
    else{/* log. case */
      drect[2] =  sciFindLogMinSPos(y,size_y); 
      drect[3] =  Maxi(y,size_y); 
    }

  }
  else {
    if(logflags[1] != 'l'){
      drect[2] = 0.0;
      drect[3] = 10.0;
    }
    else{/* log. case */
      drect[2] = 1.0;
      drect[3] = 10.0;
    }
  }
  /* back to default values for  x=[] and y = [] */
  if ( drect[2] == LARGEST_REAL ) { drect[2] = 0.0; drect[3] = 10.0 ;} 
  if ( drect[0] == LARGEST_REAL ) { drect[0] = 0.0; drect[1] = 10.0 ;} 

}




/* Given two set of coordinates x and y this routine computes the corresponding 
 *  data bounds rectangle drect=[xmin,ymin,xmax,ymax] 
 */
void compute_data_bounds(int cflag, char dataflag,double *x,double *y,integer n1,integer n2,double *drect)
{
  int size_x,size_y;
  double xd[2];
  double *x1;
  switch ( dataflag ) {
  case 'e' : 
    xd[0] = 1.0; xd[1] = (double)n2;
    x1 = xd;size_x = (n2 != 0) ? 2 : 0 ;
    break; 
  case 'o' : 
    x1 = x;size_x = n2;
    break;
  case 'g' : 
  default  : 
    x1 = x;size_x = (cflag == 1) ? n1 : (n1*n2) ;
    break; 
  }

  if (size_x != 0) {
    drect[0] =  Mini(x1, size_x); 
    drect[1] =  Maxi(x1,size_x); 
  }
  else {
    drect[0] = 0.0;
    drect[1] = 10.0;
  }

  size_y = (cflag == 1) ? n2 : (n1*n2) ;
  if (size_y != 0) {
    drect[2] =  Mini(y, size_y); 
    drect[3] =  Maxi(y,size_y); 
  }
  else {
    drect[2] = 0.0;
    drect[3] = 10.0;
  }
  /* back to default values for  x=[] and y = [] */
  if ( drect[2] == LARGEST_REAL ) { drect[2] = 0.0; drect[3] = 10.0 ;} 
  if ( drect[0] == LARGEST_REAL ) { drect[0] = 0.0; drect[1] = 10.0 ;} 

}
BOOL update_specification_bounds(psubwin, rect,flag)
     sciPointObj  *psubwin;
     double *rect;
     int flag;
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  BOOL haschanged = FALSE;
  
  ppsubwin->SRect[0] = rect[0];
  ppsubwin->SRect[1] = rect[1];
  ppsubwin->SRect[2] = rect[2];
  ppsubwin->SRect[3] = rect[3];
  if (flag==3) {
    ppsubwin->SRect[4] = rect[4];
    ppsubwin->SRect[5] = rect[5];
  }
  
  if(flag != 3)
    haschanged = sci_update_frame_bounds_2d(psubwin);
  else
    haschanged = sci_update_frame_bounds_3d(psubwin);
  
  
  return haschanged;
}


/* F.Leray */
/* brect must have the same format as drect i.e.: [xmin,xmax,ymin,ymax] */
/* brect = INPUT ; drect = OUTPUT (warning drect is dim 6) */
int re_index_brect(double * brect, double * drect)
{
  drect[0] = brect[0];
  drect[1] = brect[2];
  drect[2] = brect[1];
  drect[3] = brect[3];
/*  drect[4] = brect[4];
    drect[5] = brect[5];*/
  
  return 0;
}
/* F.Leray 07.05.04 */
/* Dispatch info contained in strflag to all the flag available in
   sciSubwin object (tight_limits, isoview, isaxes...) */
BOOL strflag2axes_properties(sciPointObj * psubwin, char * strflag)
{
  BOOL haschanged = FALSE;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);

  /* F.Leray 07.05.04 */
  /* strflag[1] Isoview & tight_limits flags management*/
  switch (strflag[1])  {
  case '0': case '9':
    /* no changes */
    break;
  case '1' : case '2' : case '7' : case '8' :
    if(ppsubwin->tight_limits != TRUE){
      ppsubwin->tight_limits = TRUE;
      haschanged = TRUE;
    }
    /*pSUBWIN_FEATURE (psubwin)->isoview      = FALSE; */
    break;
  case '3' : case '4' :
    /*pSUBWIN_FEATURE (psubwin)->tight_limits = TRUE;*/
    if(ppsubwin->isoview != TRUE){
      ppsubwin->isoview = TRUE;
      haschanged = TRUE;
    }
    break;
  case '5' : case '6' :
    if(ppsubwin->tight_limits != FALSE){
      ppsubwin->tight_limits = FALSE; /* pretty axes */
      haschanged = TRUE;
    }
    /*pSUBWIN_FEATURE (psubwin)->isoview      = FALSE;*/
    break;
  }
      
  /* F.Leray 07.05.04 */
  /* strflag[2] */
  switch (strflag[2])  {
  case '0': 
    if(ppsubwin->FirstPlot == TRUE){
      /*       ppsubwin->isaxes = FALSE; */
      ppsubwin->axes.axes_visible[0] = FALSE;
      ppsubwin->axes.axes_visible[1] = FALSE;
      ppsubwin->axes.axes_visible[2] = FALSE; /* also trigger z axis */
      ppsubwin->axes.rect = 0;
      haschanged = TRUE;
    }
    /*else no changes : the isaxes properties is driven by the previous plot */
    break;
  case '1' : 
 /*    ppsubwin->isaxes = TRUE; */
    if(ppsubwin->axes.axes_visible[0] != TRUE ||
       ppsubwin->axes.axes_visible[1] != TRUE ||
       ppsubwin->axes.axes_visible[2] != TRUE ||
       ppsubwin->axes.ydir != 'l'){
      ppsubwin->axes.axes_visible[0] = TRUE;
      ppsubwin->axes.axes_visible[1] = TRUE;
      ppsubwin->axes.axes_visible[2] = TRUE; /* also trigger z axis */
      ppsubwin->axes.ydir ='l';
      ppsubwin->axes.rect = 1;
      haschanged = TRUE;
    }
    break;
  case '2' : 
  /*   ppsubwin->isaxes = TRUE; */
    if(ppsubwin->axes.axes_visible[0] != TRUE ||
       ppsubwin->axes.axes_visible[1] != TRUE ||
       ppsubwin->axes.axes_visible[2] != TRUE){
      ppsubwin->axes.axes_visible[0] = FALSE;
      ppsubwin->axes.axes_visible[1] = FALSE;
      ppsubwin->axes.axes_visible[2] = FALSE; /* also trigger z axis */
      ppsubwin->axes.rect = 1;
      haschanged = TRUE;
    }
    break;
  case '3' : 
    /*    ppsubwin->isaxes = TRUE; */
    if(ppsubwin->axes.axes_visible[0] != TRUE ||
       ppsubwin->axes.axes_visible[1] != TRUE ||
       ppsubwin->axes.axes_visible[2] != TRUE ||
       ppsubwin->axes.ydir != 'r'){
      ppsubwin->axes.axes_visible[0] = TRUE;
      ppsubwin->axes.axes_visible[1] = TRUE;
      ppsubwin->axes.axes_visible[2] = TRUE; /* also trigger z axis */
      ppsubwin->axes.ydir ='r';
      haschanged = TRUE;
    }
    break;
  case '4' :
    /*     ppsubwin->isaxes = TRUE; */
    if(ppsubwin->axes.axes_visible[0] != TRUE ||
       ppsubwin->axes.axes_visible[1] != TRUE ||
       ppsubwin->axes.axes_visible[2] != TRUE){
      ppsubwin->axes.axes_visible[0] = TRUE;
      ppsubwin->axes.axes_visible[1] = TRUE;
      ppsubwin->axes.axes_visible[2] = TRUE; /* also trigger z axis */
      ppsubwin->axes.rect = 1;
      /* Case not implemented yet : axes are drawn centred in the middle of the frame box. */
      haschanged = TRUE;
    }
    break;
  case '5' :
 /*    ppsubwin->isaxes = TRUE; */
    if(ppsubwin->axes.axes_visible[0] != TRUE ||
       ppsubwin->axes.axes_visible[1] != TRUE ||
       ppsubwin->axes.axes_visible[2] != TRUE ||
       ppsubwin->axes.xdir != 'c' ||
       ppsubwin->axes.ydir != 'c'){
      ppsubwin->axes.axes_visible[0] = TRUE;
      ppsubwin->axes.axes_visible[1] = TRUE;
      ppsubwin->axes.axes_visible[2] = TRUE; /* also trigger z axis */
      ppsubwin->axes.xdir ='c';
      ppsubwin->axes.ydir ='c';
      ppsubwin->axes.rect = 1;
      haschanged = TRUE;
    }
    break;
  case '9' :
    /*     ppsubwin->isaxes = TRUE; */
    if(ppsubwin->axes.axes_visible[0] != TRUE ||
       ppsubwin->axes.axes_visible[1] != TRUE ||
       ppsubwin->axes.axes_visible[2] != TRUE){
      ppsubwin->axes.axes_visible[0] = TRUE;
      ppsubwin->axes.axes_visible[1] = TRUE;
      ppsubwin->axes.axes_visible[2] = TRUE; /* also trigger z axis */
      ppsubwin->axes.rect = 1;
      haschanged = TRUE;
    }
  }
  return haschanged;
}


int CreatePrettyGradsFromNax(sciPointObj * psubwin,int * Nax)
{
  double xmin = 0, xmax = 0;
  double ymin = 0, ymax = 0;
  int nbtics_x = Nax[1];
  int nbtics_y = Nax[3];
  int nbsubtics_x = Nax[0];
  int nbsubtics_y = Nax[2];

  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  
  if(sciGetZooming(psubwin) == TRUE) {
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
  
  /* x graduations */
  ppsubwin->axes.u_xgrads  = FreeUserGrads (ppsubwin->axes.u_xgrads);
  ppsubwin->axes.u_xlabels = FreeUserLabels(ppsubwin->axes.u_xlabels,
					    &ppsubwin->axes.u_nxgrads);
  
  ppsubwin->axes.u_nxgrads = nbtics_x;
  ppsubwin->axes.u_xgrads = AllocUserGrads(ppsubwin->axes.u_xgrads, nbtics_x);
  
/*   GraduateWithNax(ppsubwin,xmin,xmax,xoutmin,xoutmax,nbtics_x,nbsubtics_x,  */
/* 		  ppsubwin->axes.u_xgrads, ppsubwin->axes.u_nxgrads); */
  
  GraduateWithNax(ppsubwin,&xmin,&xmax,nbtics_x,ppsubwin->axes.u_xgrads);
  
  ppsubwin->axes.u_xlabels = AllocAndSetUserLabels(ppsubwin->axes.u_xlabels, 
						   ppsubwin->axes.u_xgrads, 
						   ppsubwin->axes.u_nxgrads, 
						   ppsubwin->logflags[0]);
  
  /* y graduations */
  ppsubwin->axes.u_ygrads  = FreeUserGrads (ppsubwin->axes.u_ygrads);
  ppsubwin->axes.u_ylabels = FreeUserLabels(ppsubwin->axes.u_ylabels,
					    &ppsubwin->axes.u_nygrads);
  
  ppsubwin->axes.u_nygrads = nbtics_y;
  ppsubwin->axes.u_ygrads = AllocUserGrads(ppsubwin->axes.u_ygrads, nbtics_y);
  
  /*   GraduateWithNax(ppsubwin,ymin,ymax,youtmin,youtmax,nbtics_y,nbsubtics_y,  */
/* 		  ppsubwin->axes.u_ygrads, ppsubwin->axes.u_nygrads); */
  
  GraduateWithNax(ppsubwin,&ymin,&ymax,nbtics_y,ppsubwin->axes.u_ygrads);
  
  ppsubwin->axes.u_ylabels = AllocAndSetUserLabels(ppsubwin->axes.u_ylabels, 
						   ppsubwin->axes.u_ygrads, 
						   ppsubwin->axes.u_nygrads, 
						   ppsubwin->logflags[1]);
    
  /* Subtics storage here */
  ppsubwin->axes.nbsubtics[0] = nbsubtics_x +1;
  ppsubwin->axes.nbsubtics[1] = nbsubtics_y +1;

  return 0;
}

int GraduateWithNax(sciSubWindow * ppsubwin,double *min,double *max,int nbtics, double * grads)
{
  int i;
  double pas;
  
  if(nbtics == 1){
    pas = 0.;
    grads[0] = (*min);
  }
  else{
    pas = (*max - *min) / (nbtics -1);
    
    for(i=0;i<nbtics;i++) 
      grads[i] = (*min) + pas*i;
  }

  return 0;
}

char ** FreeUserLabels(char ** u_xlabels, int *u_nxgrads)
{
  int i;
  
  if(u_xlabels != NULL){
    for(i=0;i<(*u_nxgrads);i++)
      {FREE(u_xlabels[i]); u_xlabels[i] = (char *) NULL;}
  }
  
  u_xlabels = (char **) NULL;
    
  *u_nxgrads = 0;
  
  return u_xlabels;
}


double * FreeUserGrads(double * u_xgrads)
{
  FREE(u_xgrads); u_xgrads = NULL;
  return u_xgrads;
}

double * AllocUserGrads(double * u_xgrads, int nb)
{
  
  if(nb == 0)
    return (double *) NULL;
  
  if(u_xgrads != NULL)
    {
      sciprint("Impossible: u_xgrads must be freed before re-allocating");
      return (double *) NULL;
    }
  
  if((u_xgrads=(double *)MALLOC(nb*sizeof(double)))==NULL){
    sciprint("No more place for allocating user grads using Nax");
    return (double *) NULL;
  }
    
  return u_xgrads;

}

int CopyUserGrads(double *u_xgrad_SRC, double *u_xgrad_DEST, int dim)
{
  int i;

  if(u_xgrad_SRC == NULL)
    return 0;

  for(i=0;i<dim;i++)
    u_xgrad_DEST[i] = u_xgrad_SRC[i];
  
  return 0;
}



char ** AllocAndSetUserLabels(char ** u_xlabels, double * u_xgrads, int u_nxgrads, char logflag)
{
  int i;
  char c_format[5]; 
  int nbtics = u_nxgrads;

  if(u_xgrads == NULL)
    return (char **) NULL;
  
  if(u_xlabels != NULL)
    {
      sciprint("Impossible: u_xlabels must be freed before re-allocating");
      return (char **) NULL;
    }
  

  if((u_xlabels=(char **)MALLOC(u_nxgrads*sizeof(char *)))==NULL){
    sciprint("No more place for allocating user labels using Nax");
    return (char **) NULL;
  }

  ChooseGoodFormat(c_format,logflag,u_xgrads,u_nxgrads);
  
  for(i=0;i<nbtics;i++)
    {  
      char foo[100];
      
      sprintf(foo,c_format, u_xgrads[i]);
      
      if((u_xlabels[i]=(char *)MALLOC((strlen(foo)+1)*sizeof(char )))==NULL){
	sciprint("No more place for allocating u_xlabels");
	return (char **) NULL;
      }
      
      strcpy(u_xlabels[i],foo);
    }
  
  return u_xlabels;
}



char ** AllocAndSetUserLabelsFromMdl(char ** u_xlabels, char ** u_xlabels_MDL, int u_nxgrads)
{
  int i;
  int nbtics = u_nxgrads;

  if(u_nxgrads == 0)
    return (char **) NULL;
  
  if(u_xlabels != NULL)
    {
      sciprint("Impossible: u_xlabels must be freed before re-allocating");
      return (char **) NULL;
    }
  
  if((u_xlabels=(char **)MALLOC(u_nxgrads*sizeof(char *)))==NULL){
    sciprint("No more place for allocating user labels using Nax");
    return (char **) NULL;
  }

  
  for(i=0;i<nbtics;i++)
    {  
      if((u_xlabels[i]=(char *)MALLOC((strlen(u_xlabels_MDL[i])+1)*sizeof(char )))==NULL){
	sciprint("No more place for allocating u_xlabels");
	return (char **) NULL;
      }
      
      strcpy(u_xlabels[i],u_xlabels_MDL[i]);
    }
  
  return u_xlabels;
}


