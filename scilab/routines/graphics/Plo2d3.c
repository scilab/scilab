/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "Math.h"
#include "PloEch.h"

#include "GetProperty.h"

extern int version_flag();

extern void Plo2d3RealToPixel __PARAMS((integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf));

/*--------------------------------------------------------------------
  C2F(plot2d3)(xf,x,y,n1,n2,style,strflag,legend,brect,aaint)

  used to plot only vertical bars form (x_i,0) to (x_i,y_i)
  the arguments are similar to those of plot2d 
  the only difference is that style must have positive values 
  which are considered as dash-styles 
--------------------------------------------------------------------------*/
  
int C2F(plot2d3)(char *xf, double *x, double *y, integer *n1, integer *n2, integer *style, char *strflag, char *legend, double *brect, integer *aaint, integer l1, integer l2, integer l3)
{
  int n;
  integer *xm,*ym;
  integer j;
  /** Attention : 2*(*n2) **/
  integer nn2=2*(*n2);
  if ( CheckxfParam(xf)== 1) return(0);

  /** Boundaries of the frame **/
  update_frame_bounds(0,xf,x,y,n1,n2,aaint,strflag,brect);

  /* Storing values if using the Record driver */
  if (GetDriver()=='R') 
    StorePlot("plot2d3",xf,x,y,n1,n2,style,strflag,legend,brect,aaint);

  /* Allocation */
  n = (*n1)*nn2 ; 
  if ( n != 0 ) 
    {
      xm = graphic_alloc(0,n,sizeof(int));
      ym = graphic_alloc(1,n,sizeof(int));
      if ( xm == 0 || ym == 0) 
	{
	  sciprint("Running out of memory \n");
	  return 0;
	}      

      /** Real to Pixel values **/
      Plo2d3RealToPixel(n1,n2,x,y,xm,ym,xf);
      
      /** Draw Axis or only rectangle **/
    }
  axis_draw(strflag);
  if ( n != 0 ) 
    {
      /** Drawing the curves **/
      frame_clip_on();
      /** to get the default dash **/
      for ( j = 0 ; j < (*n1) ; j++)
	{
	  integer lstyle,iflag=0;
	  /** style must be negative **/
	  lstyle = (style[j] < 0) ?  -style[j] : style[j];
	  C2F(dr)("xsegs","v",&xm[2*(*n2)*j],&ym[2*(*n2)*j],&nn2
		  ,&lstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      frame_clip_off();
      /** Drawing the Legends **/
      if ((int)strlen(strflag) >=1  && strflag[0] == '1')
	Legends(style,n1,legend);
    }
  return(0);
}


extern void Plo2d3RealToPixel(integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf)
{
  integer i,j;
  /** Computing y-values **/
  double y_zero = 0.;

  if(version_flag()==0)
    {
      sciPointObj *  psubwin =  sciGetSelectedSubWin (sciGetCurrentFigure ());
      sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);

      if(ppsubwin->logflags[1]=='l')
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
