/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "math_graphics.h"
#include "sciprint.h"
#include "machine.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
// fix for release 5.3.2
//extern int C2F(getgeom)(double *);
/*--------------------------------------------------------------------------*/
/*----------------------------------------------------
 * changes the current window to win 
 * returns the old current window number 
 *----------------------------------------------------*/ 
static int set_block_win( int win)
{
  static int cur = 0;
  return cur ;
}
/*--------------------------------------------------------------------------*/
/*----------------------------------------------------
 * changes the current color to color 
 * returns the old color
 *----------------------------------------------------*/ 
static int set_slider_color( int color)
{
  static int cur = 0;
  return cur ;
}
/*--------------------------------------------------------------------------*/
/*----------------------------------------------------
 * follow an input value with a graphic slider 
 * 
 *  rpar = [min-range, max-range] 
 *  ipar = [type (1,2),color]
 *  z = [val,window,x,y,w,h] 
 *----------------------------------------------------*/

void slider(int *flag, int *nevprt, double *t, double *xd,
	    double *x, int *nx, double *z, int *nz,
	    double *tvec, int *ntvec, double *rpar,
	    int *nrpar, int *ipar, int *nipar,
	    double * u, int *nu, double * y, int *ny) 
{
  
  int wid, idb = 0 ; /* XXX remettre idb avec son common */
  int cur; 
  static double th=2 ; /* border thickness */
  static double t3d = 4.0 ; /* 3d look thickness */
  int curcolor;
  if ( idb == 1 ) 
    sciprint(_("Slider t=%10.3f, flag=%d \n"),*t,*flag);

  switch ( *flag ) {
  case 2 : 
    /* standard case */ 
    wid= (int) z[1];
    if( wid < 0) return;
    cur = set_block_win(wid) ; 
    {
      double val = Min(rpar[1],Max(rpar[0],u[0]));
      double percent = (val - rpar[0])/(rpar[1]-rpar[0]);
      if ( Abs(z[0] - percent) > 0.01 ) /* a mettre en parametre XXXXX */
	{
	  curcolor=set_slider_color(ipar[1]);
	  switch (ipar[0]) 
	    {
	    case 1 : 
//	      block_draw_rect_1(z+2,z[0]);
//	      block_draw_rect_1(z+2,percent);
	      break;
	    case 2 :
//	      block_draw_rect_2(z+2,z[0]);
//	      block_draw_rect_2(z+2,percent);
	      break;
	    case 3 :
//	      block_draw_rect_3(z+2,z[0]);
//	      block_draw_rect_3(z+2,percent);
	      break;
	    }
	  curcolor=set_slider_color(curcolor);
	  z[0] = percent;
	}
    }
    set_block_win(cur);
    break;
  case 4 : 
    /* initial case */ 
    z[0]= 0.0;
// fix for release 5.3.2
    z[1] = 100.0;
//    C2F(getgeom)(z+1);
    z[2] = z[2]+ t3d +th ; 
    z[4] -= t3d + 2*th  ;
    z[3] = z[3] + z[5] ;
    z[5] -= t3d  ;
    wid= (int) z[1];
    if( wid < 0) return;
    cur = set_block_win(wid) ; 
    {
      curcolor=set_slider_color(ipar[1]);
	  /*
      C2F(dr1)("xclea","v",PI0,PI0,PI0,PI0,PI0,PI0,z+2,z+3,z+4,z+5,0L,0L);
	  */
      switch (ipar[0]) 
	{
	case 1 : 
//	  block_draw_rect_1(z+2,z[0]);
	  break;
	case 2 :
//	  block_draw_rect_2(z+2,z[0]);
	  break;
	case 3 :
//	  block_draw_rect_3(z+2,z[0]);
	  break;

	}
      curcolor=set_slider_color(curcolor);
    }
    set_block_win(cur);
    break;
  }
}
/*--------------------------------------------------------------------------*/

