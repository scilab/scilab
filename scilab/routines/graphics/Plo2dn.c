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
#include "Entities.h"
extern void initsubwin();
void compute_data_bounds(int cflag,char dataflag,double *x,double *y,int n1,int n2,double *drect);
extern double  sciFindLogMinSPos(double *x, int n);
void compute_data_bounds2(int cflag,char dataflag,char *logflags,double *x,double *y,int n1,int n2,double *drect);
void update_specification_bounds(sciPointObj *psubwin, double *rect,int flag);
int re_index_brect(double * brect, double * drect);
extern void strflag2axes_properties(sciPointObj * psubwin, char * strflag);

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
  
int plot2dn(integer ptype,char *logflags,double *x,double *y,integer *n1,integer *n2,integer *style,char *strflag,char *legend,double *brect,integer *aaint,integer lstr1,integer lstr2)
{
  int closeflag = 0;
  int jj = 0;
  sciPointObj **pptabofpointobj;
  sciPointObj  *psubwin;
  long hdl;
  long *hdltab;
  int cmpt=0,i,ok;
  int with_leg;
  double drect[6];
  char dataflag/*,frameflag*/;
  sciSubWindow * ppsubwin = NULL;


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
      sciRedrawFigure(); 
    pSUBWIN_FEATURE (psubwin)->is3d = FALSE;
  }


  /* Force psubwin->axes.aaint to those given by argument aaint*/
  for (i=0;i<4;i++) pSUBWIN_FEATURE(psubwin)->axes.aaint[i] = aaint[i]; 

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
    if (strflag[1] != '0') update_specification_bounds(psubwin, drect,2);

  } 

  pSUBWIN_FEATURE (psubwin)->FirstPlot = FALSE;
  strflag2axes_properties(psubwin, strflag);

  with_leg= (strflag[0] == '1');
  pSUBWIN_FEATURE (psubwin)->with_leg = with_leg;

  sciDrawObj(sciGetSelectedSubWin (sciGetCurrentFigure ()));/* ???? */ 
  
  /*---- Drawing the curves and the legengs ----*/
  if ( *n1 != 0 ) {
    frame_clip_on ();
    if ((hdltab = malloc ((*n1+2) * sizeof (long))) == NULL) {
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
      sciSetCurrentObj (ConstructPolyline
                        ((sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure()),&(x[jj*(*n2)]),
                        &(y[jj*(*n2)]),PD0,closeflag,*n2,1,ptype)); 
      if (style[jj] > 0) { 
        sciSetIsMark(sciGetCurrentObj(), FALSE);   
        sciSetForeground (sciGetCurrentObj(), style[jj]);
      }
      else {
	sciSetIsMark(sciGetCurrentObj(),  (style[jj] <= 0 ? TRUE : FALSE));
	sciSetMarkStyle (sciGetCurrentObj(),-(style[jj]));
      } 
      if (with_leg) pptabofpointobj[jj] = sciGetCurrentObj();
      sciDrawObj(sciGetCurrentObj ()); 
      hdl=sciGetHandle(sciGetCurrentObj ());   
      hdltab[cmpt]=hdl;
      cmpt++;
    }
    frame_clip_off ();

    /*---- Drawing the Legends ----*/
    if (with_leg) {
      sciSetCurrentObj (ConstructLegend
                        ((sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure
()),
                         legend, strlen(legend), *n1, style, pptabofpointobj)); 
      hdl=sciGetHandle(sciGetCurrentObj ());   
      hdltab[cmpt]=hdl;
      cmpt++;
      Legends (style, n1, legend);  
      FREE(pptabofpointobj);
    }

    /*---- construct agregation ----*/
    sciSetCurrentObj(ConstructAgregation (hdltab, cmpt)); 
    FREE(hdltab);
    if (ok==1) Merge3d(psubwin);
    sciDrawObj(sciGetCurrentFigure ()); /*A.Djalel 3D axes*/
    return(0);
  }
  sciDrawObj(sciGetCurrentFigure ());
  return(0);
}




/* Given two set of coordinates x and y this routine computes the corresponding 
 *  data bounds rectangle drect=[xmin,ymin,xmax,ymax] taking into account the logflag
 *  -> means we have to find among the data the min > 0.
 */
void compute_data_bounds2(cflag,dataflag,logflags,x,y,n1,n2,drect)
     int cflag;/* cflag is non zero when called by contour */
     char dataflag; /* 'e','o' or 'g'*/
     char * logflags; /* for log. case x/ymin control */
     integer n1, n2;
     double *x, *y; 
     double *drect;
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
void compute_data_bounds(cflag,dataflag,x,y,n1,n2,drect)
     int cflag;/* cflag is non zero when called by contour */
     char dataflag; /* 'e','o' or 'g'*/
     integer n1, n2;
     double *x, *y; 
     double *drect;
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
void update_specification_bounds(psubwin, rect,flag)
     sciPointObj  *psubwin;
     double *rect;
     int flag;
{
    pSUBWIN_FEATURE (psubwin)->SRect[0]=rect[0];
    pSUBWIN_FEATURE (psubwin)->SRect[1]=rect[1];
    pSUBWIN_FEATURE (psubwin)->SRect[2]=rect[2];
    pSUBWIN_FEATURE (psubwin)->SRect[3]=rect[3];
    if (flag==3) {
      pSUBWIN_FEATURE (psubwin)->SRect[4]=rect[4];
      pSUBWIN_FEATURE (psubwin)->SRect[5]=rect[5];
    }
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
void strflag2axes_properties(sciPointObj * psubwin, char * strflag)
{
      
  /* F.Leray 07.05.04 */
  /* strflag[1] Isoview & tight_limits flags management*/
  switch (strflag[1])  {
  case '0': case '9':
    /* no changes */
    break;
  case '1' : case '2' : case '7' : case '8' :
    pSUBWIN_FEATURE (psubwin)->tight_limits = TRUE;
    /*pSUBWIN_FEATURE (psubwin)->isoview      = FALSE; */
    break;
  case '3' : case '4' :
    /*pSUBWIN_FEATURE (psubwin)->tight_limits = TRUE;*/
    pSUBWIN_FEATURE (psubwin)->isoview      = TRUE;
    break;
  case '5' : case '6' :
    pSUBWIN_FEATURE (psubwin)->tight_limits = FALSE; /* pretty axes */
    /*pSUBWIN_FEATURE (psubwin)->isoview      = FALSE;*/
    break;
  }
      
  /* F.Leray 07.05.04 */
  /* strflag[2] */
  switch (strflag[2])  {
  case '0': 
    if( pSUBWIN_FEATURE (psubwin)->FirstPlot == TRUE)
      pSUBWIN_FEATURE (psubwin)->isaxes = FALSE;
    /*else no changes : the isaxes properties is driven by the previous plot */
   
    break;
  case '1' : 
    pSUBWIN_FEATURE (psubwin)->isaxes = TRUE;
    pSUBWIN_FEATURE (psubwin)->axes.ydir ='l';
    break;
  case '2' : 
    pSUBWIN_FEATURE (psubwin)->isaxes = TRUE;
    /* Case not implemented yet : the plot is surrounded by a box without tics. */
    break;
  case '3' : 
    pSUBWIN_FEATURE (psubwin)->isaxes = TRUE;
    pSUBWIN_FEATURE (psubwin)->axes.ydir ='r';
    break;
  case '4' :
    pSUBWIN_FEATURE (psubwin)->isaxes = TRUE;
    /* Case not implemented yet : axes are drawn centred in the middle of the frame box. */
    break;
  case '5' :
    pSUBWIN_FEATURE (psubwin)->isaxes = TRUE;
    pSUBWIN_FEATURE (psubwin)->axes.xdir ='c';
    pSUBWIN_FEATURE (psubwin)->axes.ydir ='c';
    break;
  case '9' :
    pSUBWIN_FEATURE (psubwin)->isaxes = TRUE;
  }
}

