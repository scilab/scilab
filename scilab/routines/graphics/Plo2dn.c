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
  int cmpt=0;
  int with_leg;

 
  if (!(sciGetGraphicMode (sciGetSelectedSubWin (sciGetCurrentFigure ())))->addplot) { 
    sciXbasc(); 
    initsubwin();
    sciRedrawFigure();
  } 
  
  /*---- Boundaries of the frame ----*/
  psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ()); 
  pSUBWIN_FEATURE (psubwin)->logflags[0]=logflags[1];
  pSUBWIN_FEATURE (psubwin)->logflags[1]=logflags[2];

  if (sciGetGraphicMode (psubwin)->autoscaling)
     update_frame_bounds(0,logflags,x,y,n1,n2,aaint,strflag,brect); 
    

  with_leg= ((int)strlen(strflag) >=1  && strflag[0] == '1');



  /*---- Drawing the axes ----*/
  /** Check if an other axis exist in the selected subwindow **/

  sciSetIsClipping (psubwin,0); 

  if (strflag[1]!='0')
    strncpy(pSUBWIN_FEATURE (psubwin)->strflag, strflag, strlen(strflag));
  pSUBWIN_FEATURE (psubwin)->isaxes  = TRUE;

  sciDrawObj(sciGetSelectedSubWin (sciGetCurrentFigure ()));


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
    for (jj = 0;jj < *n1; jj++) {
      sciSetCurrentObj (ConstructPolyline
                        ((sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure()),&(x[jj*(*n2)]),
                        &(y[jj*(*n2)]),closeflag,*n2,1,ptype)); 
      if (style[jj] > 0) { 
        sciSetIsMark(sciGetCurrentObj(), FALSE);   
        sciSetForeground (sciGetCurrentObj(), style[jj]);
      }
      else {
        sciSetIsMark(sciGetCurrentObj(),  (style[jj] < 0 ? TRUE : FALSE));
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
    return(0);
  }
  return(0);
}


