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

extern void Plo2d1RealToPixel __PARAMS((integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf));

/*--------------------------------------------------------------------
  C2F(plot2d1)(xf,x,y,n1,n2,style,strflag,legend,brect,aint)
  
  similar to plot2d plus one additionnal argument xf 
  and a special treatment for x 

  there's a splecial treatment for x 
  if xf[0]='e' for empty : x can point to nothing, the x -values 
  are assumed to be x[i+(*n2)*j]= i 
  if xf[0]='o' for one   : all the curves have the same x values 
     x is of size *n2  xx[i+(*n2)*j] = x[i];
  if xf[0]='g' for general : x is of size (*n2)*(n1);

  xf[1]='l' or 'n' LogAxis or standard on X
  xf[2]='l' or 'n' LogAxis or standard on Y
--------------------------------------------------------------------------*/
  
int C2F(plot2d1)(char *xf, double *x, double *y, integer *n1, integer *n2, integer *style, char *strflag, char *legend, double *brect, integer *aaint, integer l1, integer l2, integer l3)
{
  int n;
  integer *xm,*ym, nn2=(*n2);
  if ( CheckxfParam(xf)== 1) return(0);

  /** Boundaries of the frame **/
  update_frame_bounds(0,xf,x,y,n1,n2,aaint,strflag,brect);

  /* Storing values if using the Record driver */
  if (GetDriver()=='R') 
    StorePlot("plot2d1",xf,x,y,n1,n2,style,strflag,legend,brect,aaint);

  /** Allocation **/

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
      Plo2d1RealToPixel(n1,n2,x,y,xm,ym,xf);
    }
  /** Drawing axes **/
  axis_draw(strflag);
  /** Drawing the curves **/
  if ( n != 0 ) 
    {
      frame_clip_on();
      C2F(dr)("xpolys","v",xm,ym,style,n1,&nn2 ,PI0,PD0,PD0,PD0,PD0,0L,0L);
      frame_clip_off();
      /** Drawing the Legends **/
      if ((int)strlen(strflag) >=1  && strflag[0] == '1')
	Legends(style,n1,legend);
    }
 return(0);

}

extern void Plo2d1RealToPixel(integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf)
{
  integer i,j;
  /** Computing y-values **/
  if ((int)strlen(xf) >= 3 && xf[2]=='l')	  
    {
      for ( i=0 ; i < (*n2) ; i++)
	for (j=0 ; j< (*n1) ; j++)
	  ym[i+(*n2)*j]= YLogScale(y[i+(*n2)*j]);
    }
  else 
    {
      for ( i=0 ; i < (*n2) ; i++)
	for (j=0 ; j< (*n1) ; j++)
	  ym[i+(*n2)*j]= YScale(y[i+(*n2)*j]);
    }
  
  /** Computing x-values **/
  switch (xf[0])
    {
    case 'e' :
      /** No X-value given by the user **/
      if ((int)strlen(xf) >= 2 && xf[1]=='l')
	for ( i=0 ; i < (*n2) ; i++)
	  for (j=0 ; j< (*n1) ; j++)
	    xm[i+(*n2)*j]= XLogScale(i+1.0);
      else 
	for ( i=0 ; i < (*n2) ; i++)
	  for (j=0 ; j< (*n1) ; j++)
	    xm[i+(*n2)*j]= XScale(i+1.0);
      break ;
    case 'o' :
      if ((int)strlen(xf) >= 2 && xf[1]=='l')
	for ( i=0 ; i < (*n2) ; i++)
	  for (j=0 ; j< (*n1) ; j++)
	    xm[i+(*n2)*j]= XLogScale(x[i]);
      else 
	for ( i=0 ; i < (*n2) ; i++)
	  for (j=0 ; j< (*n1) ; j++)
	    xm[i+(*n2)*j]= XScale(x[i]);
      break;
    case 'g' :
    default:
      if ((int)strlen(xf) >= 2 && xf[1]=='l')
	for ( i=0 ; i < (*n2) ; i++)
	  for (j=0 ; j< (*n1) ; j++)
	    xm[i+(*n2)*j]= XLogScale(x[i+(*n2)*j]);
      else 
	for ( i=0 ; i < (*n2) ; i++)
	  for (j=0 ; j< (*n1) ; j++)
	    xm[i+(*n2)*j]= XScale(x[i+(*n2)*j]);
      break;
    }
}


int CheckxfParam(char *xf)
{
  if ( strlen(xf) < 3 ) 
    {
      sciprint("Error : first argument must be a string of length 3");
      return(1);
    }
  if ( xf[0] != 'g' && xf[0] != 'e' && xf[0] != 'o' ) 
    {
      sciprint("Error : wrong first character in string \"%s\"\n",xf);
      return(1) ;
    }
  if ( xf[1] != 'l' && xf[1] != 'n' ) 
    {
      sciprint("Error : wrong second character in string \"%s\"\n",xf);
      return(1) ;
    }
  if ( xf[2] != 'l' && xf[2] != 'n' ) 
    {
      sciprint("Error : wrong third character in string \"%s\"\n",xf);
      return(1) ;
    }
  return(0);
}

