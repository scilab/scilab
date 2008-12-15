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
#include <stdio.h>
#include "math_graphics.h"
#include "sciprint.h"
#include "machine.h"
extern int C2F(getgeom)(double *);

/*----------------------------------------------------
 * erase a rectangle 
 *----------------------------------------------------*/ 
/*
extern int C2F(dr1)( char x0[],char x1[],int *x2,int *x3,int *x4,int *x5,int *x6,
			 int *x7,double *dx1,double *dx2,double *dx3,double *dx4,
			 int lx0, int lx1) ;

extern int C2F(dr)(char x0[],char x1[],int *x2,int *x3,int *x4,int *x5,int *x6,
			int *x7,double *dx1,double *dx2,double *dx3,double *dx4,
			int lx0, int lx1) ;
*/
static void block_draw_rect_1(double r[],double percent)
{
/*
  int verbose=0;
  static int in=6;
  double w = r[2]*percent;
  double x = r[0];
*/
  /* disabled */
  /* TO DO : new graphics version */

  /*C2F(dr)("xget","alufunction",&verbose,&alumode,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if ( alumode != in ) C2F(dr1)("xset","alufunction",&in,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr1)("xfrect","v",PI0,PI0,PI0,PI0,PI0,PI0,&x,r+1,&w,r+3,0L,0L);
  if ( alumode != in ) C2F(dr1)("xset","alufunction",&alumode,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  */
}

static void block_draw_rect_2(double r[],double percent)
{
/*
  int verbose=0;*/
  /* int flag =0;  double ang=0.0;  char foo[24]; */
/*static int in=6;
  double x = r[0]+ r[2]*percent-2;
  double w = 4;*/
  /* disabled */
  /* TO DO : new graphics version */
  /*C2F(dr)("xget","alufunction",&verbose,&alumode,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if ( alumode != in ) C2F(dr1)("xset","alufunction",&in,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr1)("xfrect","v",PI0,PI0,PI0,PI0,PI0,PI0,&x,r+1,&w,r+3,0L,0L);
  /* sprintf(foo,"%5.3f",percent);
     C2F(dr1)("xstring",foo,PI0,PI0,PI0,&flag,PI0,PI0,r,r+1,&ang,PD0,0L,0L);
  */

  /* disabled */
  /* TO DO : new graphics version */
  /*if ( alumode != in ) C2F(dr1)("xset","alufunction",&alumode,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);*/
}


static void block_draw_rect_3(double r[],double percent)
{
	/*
  int verbose=0;
  int flag =0;  double ang=0.0;
  static int in=6;
  double x = r[0] + (1.0/10)*r[2];
  double y = r[1] - r[3] + 2 ;
  */
  /* disabled */
  /* TO DO : new graphics version */
  /*C2F(dr)("xget","alufunction",&verbose,&alumode,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if ( alumode != in ) C2F(dr1)("xset","alufunction",&in,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  sprintf(foo,"%5.3f",percent);
  C2F(dr1)("xstring",foo,PI0,PI0,PI0,&flag,PI0,PI0,&x,&y,&ang,PD0,0L,0L);
  if ( alumode != in ) C2F(dr1)("xset","alufunction",&alumode,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  */
}




/*----------------------------------------------------
 * switch to X11 driver  
 *----------------------------------------------------*/ 

#define REMOVE_REC_DRIVER() /*char old_rec[4]; int rem_flag ; rem_flag = scig_driverX11(old_rec); */
#define RESTORE_DRIVER() /*if (rem_flag == 1) C2F(SetDriver)(old_rec,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);*/

static int scig_driverX11(char *old) 
{
	/* disabled */
	/* TO DO : new graphics version */
  /*GetDriver1(old,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);*/
  if ( old[0] == 'R' )
    {
		/* disabled */
		/* TO DO : new graphics version */
      /*C2F(SetDriver)("X11",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);*/
      return 1;
    }
  return 0;
}

/*----------------------------------------------------
 * changes the current window to win 
 * returns the old current window number 
 *----------------------------------------------------*/ 

static int set_block_win( int win)
{
  static int cur = 0;
  /* TO DO : rewrite with new graphics functions */
  /*
  C2F(dr)("xget","window",&verbose,&cur,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if ( cur != win ) 
    C2F(dr)("xset","window",&win,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  */
  return cur ;
}

/*----------------------------------------------------
 * changes the current color to color 
 * returns the old color
 *----------------------------------------------------*/ 


static int set_slider_color( int color)
{
  static int verbose = 0, cur=0,narg;
  /*
  C2F(dr)("xget","color",&verbose,&cur,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if ( cur != color ) 
    C2F(dr)("xset","color",&color,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  */

  return cur ;
}

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
    sciprint("Slider t=%10.3f, flag=%d \r\n",*t,*flag);

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
	  REMOVE_REC_DRIVER();
	  curcolor=set_slider_color(ipar[1]);
	  switch (ipar[0]) 
	    {
	    case 1 : 
	      block_draw_rect_1(z+2,z[0]);
	      block_draw_rect_1(z+2,percent);
	      break;
	    case 2 :
	      block_draw_rect_2(z+2,z[0]);
	      block_draw_rect_2(z+2,percent);
	      break;
	    case 3 :
	      block_draw_rect_3(z+2,z[0]);
	      block_draw_rect_3(z+2,percent);
	      break;
	    }
	  curcolor=set_slider_color(curcolor);
	  z[0] = percent;
	  RESTORE_DRIVER();
	}
    }
    set_block_win(cur);
    break;
  case 4 : 
    /* initial case */ 
    z[0]= 0.0;
    C2F(getgeom)(z+1);
    z[2] = z[2]+ t3d +th ; 
    z[4] -= t3d + 2*th  ;
    z[3] = z[3] + z[5] ;
    z[5] -= t3d  ;
    wid= (int) z[1];
    if( wid < 0) return;
    cur = set_block_win(wid) ; 
    {
      REMOVE_REC_DRIVER();
      curcolor=set_slider_color(ipar[1]);
	  /*
      C2F(dr1)("xclea","v",PI0,PI0,PI0,PI0,PI0,PI0,z+2,z+3,z+4,z+5,0L,0L);
	  */
      switch (ipar[0]) 
	{
	case 1 : 
	  block_draw_rect_1(z+2,z[0]);
	  break;
	case 2 :
	  block_draw_rect_2(z+2,z[0]);
	  break;
	case 3 :
	  block_draw_rect_3(z+2,z[0]);
	  break;

	}
      curcolor=set_slider_color(curcolor);
      RESTORE_DRIVER();
    }
    set_block_win(cur);
    break;
  }
}

