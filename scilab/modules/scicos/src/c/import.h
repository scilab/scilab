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
#ifndef __IMPORT_H__
#define __IMPORT_H__
#include "scicos_block4.h"
#include "machine.h"

/* Copyright INRIA */
typedef struct
{
  int lnk;
  int pos;
} outtb_el;

typedef struct {
  double* x;            /* 1  - continuous state */
  integer *nx;          /* 2  - length of continuous state  */
  integer* xptr;        /* 3  - vector of pointers on block states */
  integer* zcptr;       /* 4  - vector of pointers on block zero crosses */
  double* z;            /* 5  - discrete state */
  integer *nz;          /* 6  - length of discrete state */
  integer* zptr;        /* 7  - vector of pointers on block states */
  integer *noz;         /* 8  - scalar number of object discrete states */
  void **oz;            /* 9  - pointer array of object discrete states */
  integer *ozsz;        /* 10 - vector of dimension of object discrete states */
  integer *oztyp;       /* 11 - vector of type of object discrete states */
  integer *ozptr;       /* 12 - vector of pointers on object discrete states */
  double* g;            /* 13 - zero crossing state */
  integer *ng;          /* 14 - length of zero crossing register */
  integer* mod;         /* 15 - modes of the blocks */
  integer *nmod;        /* 16 - length of mod register */
  integer* modptr;      /* 17 - vector of pointers */
  integer* iz;          /* 18 - vectors of labels */
  integer* izptr;       /* 19 - vector of pointers on block labels */
  integer* inpptr;      /* 20 - vector of pointers on block inputs */
  integer* inplnk;      /* 21 - vector of pointers on input link */
  integer* outptr;      /* 22 - vector of pointers on block outputs */
  integer* outlnk;      /* 23 - vector of pointers on output link */
  double * rpar;        /* 24 - vector of real parameters */
  integer* rpptr;       /* 25 - vector of pointers on block real parameters */
  integer* ipar;        /* 26 - vector of integer parameters */
  integer* ipptr;       /* 27 - vector of pointers on block integer parameters */
  void **opar;          /* 28 - pointer array of object parameters */
  integer *oparsz;      /* 29 - vector of dimension of object parameters */
  integer *opartyp;     /* 30 - vector of type of object parameters */
  integer *opptr;       /* 31 - vector of pointers on object parameters */
  integer *nblk;        /* 32 - number of  blocks */
  void **outtbptr;      /* 33 - vector of pointer of object of outtb */
  integer *outtbsz;     /* 34 - vector of size of object of outtb */
  integer *outtbtyp;    /* 35 - vector of type of object of outtb */
  integer *nlnk;        /* 36 - number of object of outtb */
  integer* subs;        /* 37 -  */
  integer *nsubs;       /* 38 -  */
  double* tevts;        /* 39 -  */
  integer* evtspt;      /* 40 -  */
  integer *nevts;       /* 41 -  */
  integer *pointi;      /* 42 -  */
  integer *iord;        /* 43 -  */
  integer *niord;       /* 44 -  */
  integer *oord;        /* 45 -  */
  integer *noord;       /* 46 -  */
  integer *zord;        /* 47 -  */
  integer *nzord;       /* 48 -  */
  integer *funptr;      /* 49 - block indexes */
  integer *funtyp;      /* 50 - block types */
  integer *ztyp;        /* 51 - block types */
  integer *cord;        /* 52 - block types */
  integer *ncord;       /* 53 - */
  integer *ordclk;      /* 54 - block types */
  integer *clkptr;      /* 55 - block types */
  integer *ordptr;      /* 56 - block types */
  integer *nordptr;     /* 57 - */
  integer *critev;      /* 58 - block types */
  integer *iwa;         /* 59 - integer work space of size nevts */
  integer *mask;        /* 60 - integer work space ng for mask */
  scicos_block *blocks; /* 61 - scicos_block structure ptr */
  double *t0;           /* 62 - scalar initial time of simulation  */
  double *tf;           /* 63 - scalar final time of simulation    */
  double *Atol;         /* 64 - scalar absolute tolerance of solvers of diff. equa. */
  double *rtol;         /* 65 - scalar relative tolerance of solvers of diff. equa. */
  double *ttol;         /* 66 - scalar time tolerance of solvers of diff. equa. */
  double *deltat;       /* 67 - deltat */
  double *hmax;         /* 68 - hmax */
  outtb_el *outtb_elem; /* 69 - table (of size 2) coding each indexes of element of matrix in a same array */
  integer *nelem;       /* 70 - total unit element */
} ScicosImport;

#endif /* __IMPORT_H__ */
