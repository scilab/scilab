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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
#ifndef __IMPORT_H__
#define __IMPORT_H__

#include "machine.h"

#include "dynlib_scicos.h"

#include "scicos_block4.h"

typedef struct
{
	int lnk;
	int pos;
} outtb_el;

typedef struct {
	double* x;            /* 1  - continuous state */
	int *nx;          /* 2  - length of continuous state  */
	int * xptr;        /* 3  - vector of pointers on block states */
	int * zcptr;       /* 4  - vector of pointers on block zero crosses */
	double* z;            /* 5  - discrete state */
	int *nz;          /* 6  - length of discrete state */
	int* zptr;        /* 7  - vector of pointers on block states */
	int *noz;         /* 8  - scalar number of object discrete states */
	void **oz;            /* 9  - pointer array of object discrete states */
	int *ozsz;        /* 10 - vector of dimension of object discrete states */
	int *oztyp;       /* 11 - vector of type of object discrete states */
	int *ozptr;       /* 12 - vector of pointers on object discrete states */
	double* g;            /* 13 - zero crossing state */
	int *ng;          /* 14 - length of zero crossing register */
	int* mod;         /* 15 - modes of the blocks */
	int *nmod;        /* 16 - length of mod register */
	int* modptr;      /* 17 - vector of pointers */
	int* iz;          /* 18 - vectors of labels */
	int* izptr;       /* 19 - vector of pointers on block labels */
	int* inpptr;      /* 20 - vector of pointers on block inputs */
	int* inplnk;      /* 21 - vector of pointers on input link */
	int* outptr;      /* 22 - vector of pointers on block outputs */
	int* outlnk;      /* 23 - vector of pointers on output link */
	double * rpar;        /* 24 - vector of real parameters */
	int* rpptr;       /* 25 - vector of pointers on block real parameters */
	int* ipar;        /* 26 - vector of integer parameters */
	int* ipptr;       /* 27 - vector of pointers on block integer parameters */
	void **opar;          /* 28 - pointer array of object parameters */
	int *oparsz;      /* 29 - vector of dimension of object parameters */
	int *opartyp;     /* 30 - vector of type of object parameters */
	int *opptr;       /* 31 - vector of pointers on object parameters */
	int *nblk;        /* 32 - number of  blocks */
	void **outtbptr;      /* 33 - vector of pointer of object of outtb */
	int *outtbsz;     /* 34 - vector of size of object of outtb */
	int *outtbtyp;    /* 35 - vector of type of object of outtb */
	int *nlnk;        /* 36 - number of object of outtb */
	int* subs;        /* 37 -  */
	int *nsubs;       /* 38 -  */
	double* tevts;        /* 39 -  */
	int* evtspt;      /* 40 -  */
	int *nevts;       /* 41 -  */
	int *pointi;      /* 42 -  */
	int *iord;        /* 43 -  */
	int *niord;       /* 44 -  */
	int *oord;        /* 45 -  */
	int *noord;       /* 46 -  */
	int *zord;        /* 47 -  */
	int *nzord;       /* 48 -  */
	int *funptr;      /* 49 - block indexes */
	int *funtyp;      /* 50 - block types */
	int *ztyp;        /* 51 - block types */
	int *cord;        /* 52 - block types */
	int *ncord;       /* 53 - */
	int *ordclk;      /* 54 - block types */
	int *nordclk;     /* 55 - size of ordclk */
	int *clkptr;      /* 56 - block types */
	int *ordptr;      /* 57 - block types */
	int *nordptr;     /* 58 - */
	int *critev;      /* 59 - block types */
	int *iwa;         /* 60 - integer work space of size nevts */
	int *mask;        /* 61 - integer work space ng for mask */
	scicos_block *blocks; /* 62 - scicos_block structure ptr */
	double *t0;           /* 63 - scalar initial time of simulation  */
	double *tf;           /* 64 - scalar final time of simulation    */
	double *Atol;         /* 65 - scalar absolute tolerance of solvers of diff. equa. */
	double *rtol;         /* 66 - scalar relative tolerance of solvers of diff. equa. */
	double *ttol;         /* 67 - scalar time tolerance of solvers of diff. equa. */
	double *deltat;       /* 68 - deltat */
	double *hmax;         /* 69 - hmax */
	outtb_el *outtb_elem; /* 70 - table (of size 2) coding each indexes of element of matrix in a same array */
	int *nelem;       /* 71 - total unit element */
	int *xprop;       /* 72 - xproperties (of size nx) */
	double *xd;           /* 73 - derivative of continuous state */
} ScicosImport;

SCICOS_IMPEXP ScicosImport* getscicosimportptr(void);

SCICOS_IMPEXP int getscicosvarsfromimport(char *what,void **v,int *nv,int *mv);

SCICOS_IMPEXP void C2F(getouttb)(int *nsize,int *nvec,double *outtc);

SCICOS_IMPEXP void C2F(getlabel)(int *kfun, char *label,int *n);

SCICOS_IMPEXP int C2F(getscilabel)(int *kfun, int label[], int *n);

#endif /* __IMPORT_H__ */
