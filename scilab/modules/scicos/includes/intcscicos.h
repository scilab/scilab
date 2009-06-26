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
#ifndef __INTCSCICOS_H__
#define __INTCSCICOS_H__

#include "scicos_block4.h"

/* functions to split (gateways) */
int intendscicosim(char *fname,unsigned long fname_len);

int intduplicate(char *fname, unsigned long fname_len);
int intdiffobjs(char *fname, unsigned long fname_len);
int inttree2(char *fname, unsigned long fname_len);
int inttree3(char *fname, unsigned long fname_len);
int inttree4(char *fname,unsigned long fname_len);
int intxproperty(char *fname,unsigned long fname_len);
int intphasesim(char *fname, unsigned long fname_len);
int intsetxproperty(char *fname, unsigned long fname_len);
int intsetblockerror(char *fname, unsigned long fname_len);
int intscicosimc(char *fname,unsigned long fname_len);




int MlistGetFieldNumber(int *ptr, const char *string);
void  duplicata(int *n,double *v,double *w,double *ww,int *nw);
void  comp_size(double *v,int *nw,int n);




#endif /* __INTCSCICOS_H__ */
