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
#ifndef __SCICOS_H 
#define __SCICOS_H 

#include "machine.h"
#include "scicos_block4.h"

/* maximum value for sum of number of inputs and outputs ports of a given 
block of type 2 */
#define SZ_SIZE 60
/* maximum value for sum of number of inputs and outputs of a given block 
of type 0 */
#define TB_SIZE 500

typedef void (*voidf)();

/* Blocks prototypes */
#define I int
#define IP int*
#define DP double*
#define DPP double**
#define DB scicos_block*
#define F scicos_flag
#define FP scicos_flag*

/*                    flag  nclock ntvec  rpar  nrpar ipar  nipar  u  nu */
#define ARGS_scicosm1 IP,    IP,    IP,    DP,   IP,  IP,   IP,   DP, IP

/* flag  nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar  intabl  ni  outabl no */
#define ARGS_scicos0 FP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DP,IP,DP,IP

/*       flag   nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar  intabl  .... */
#define ARGS_scicos FP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP

/*        flag   nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar   args_in sz_in, n_in  args_out sz_out, n_out  */
#define ARGS_scicos2 FP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DPP,IP,IP,DPP,IP,IP

/*        flag   nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar   args_in sz_in, n_in  args_out sz_out, n_out g ng */
#define ARGS_scicos2z FP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DPP,IP,IP,DPP,IP,IP,DP,IP

/*       flag   nclock t    residual xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar  intabl  .... */

#define ARGS_scicosi FP,IP,DP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP

/*        flag   nclockf t   residual xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar   args_in sz_in, n_in  args_out sz_out, n_out  */
#define ARGS_scicosi2 FP,IP,DP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DPP,IP,IP,DPP,IP,IP

/*        flag   nclockf t   residual xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar   args_in sz_in, n_in  args_out sz_out, n_out g ng */
#define ARGS_scicosi2z FP,IP,DP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DPP,IP,IP,DPP,IP,IP,DP,IP

/*         block     flag*/
#define ARGS_scicos4 DB, F


typedef void (*ScicosFm1)(ARGS_scicosm1);
typedef void (*ScicosF0)(ARGS_scicos0);
typedef void (*ScicosF)(ARGS_scicos);
typedef void (*ScicosF2)(ARGS_scicos2);
typedef void (*ScicosF2z)(ARGS_scicos2z);
typedef void (*ScicosFi)(ARGS_scicosi);
typedef void (*ScicosFi2)(ARGS_scicosi2);
typedef void (*ScicosFi2z)(ARGS_scicosi2z);
typedef void (*ScicosF4)(ARGS_scicos4);

typedef  struct  {
  char *name;
  ScicosF fonc;
} OpTab ;

/**
*
*/
int C2F(scicos)();

/**
*
*/
void callf(double *t, scicos_block *block, scicos_flag *flag);

/**
*
*/
int get_phase_simulation(void);

/**
*
*/
void set_block_error(int err);

/**
*
*/
double get_scicos_time(void);

/**
*
*/
void set_pointer_xproperty(int* pointer);

/**
*
*/
void do_cold_restart(void);

/**
*
*/
int get_block_number(void);

/**
*
*/
void end_scicos_sim(void);

/**
*
*/
void Coserror(char *fmt, ...);

#endif 
