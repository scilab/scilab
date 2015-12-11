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

typedef struct
{
    double* x;            /* continuous state */
    int *nx;          /* length of continuous state  */
    int * xptr;        /* vector of pointers on block states */
    int * zcptr;       /* vector of pointers on block zero crosses */
    double* z;            /* discrete state */
    int *nz;          /* length of discrete state */
    int* zptr;        /* vector of pointers on block states */
    int *noz;         /* scalar number of object discrete states */
    void **oz;            /* pointer array of object discrete states */
    int *ozsz;        /* vector of dimension of object discrete states */
    int *oztyp;       /* vector of type of object discrete states */
    int *ozptr;       /* vector of pointers on object discrete states */
    double* g;            /* zero crossing state */
    int *ng;          /* length of zero crossing register */
    int* mod;         /* modes of the blocks */
    int *nmod;        /* length of mod register */
    int* modptr;      /* vector of pointers */
    char** iz;          /* vectors of labels */
    int* izptr;       /* vector of pointers on block labels */
    char** uid;         /* vectors of uids */
    int* uidptr;      /* vector of pointers on block uids */
    int* inpptr;      /* vector of pointers on block inputs */
    int* inplnk;      /* vector of pointers on input link */
    int* outptr;      /* vector of pointers on block outputs */
    int* outlnk;      /* vector of pointers on output link */
    double * rpar;        /* vector of real parameters */
    int* rpptr;       /* vector of pointers on block real parameters */
    int* ipar;        /* vector of integer parameters */
    int* ipptr;       /* vector of pointers on block integer parameters */
    void **opar;          /* pointer array of object parameters */
    int *oparsz;      /* vector of dimension of object parameters */
    int *opartyp;     /* vector of type of object parameters */
    int *opptr;       /* vector of pointers on object parameters */
    int *nblk;        /* number of  blocks */
    void **outtbptr;      /* vector of pointer of object of outtb */
    int *outtbsz;     /* vector of size of object of outtb */
    int *outtbtyp;    /* vector of type of object of outtb */
    int *nlnk;        /* number of object of outtb */
    int* subs;        /*  */
    int *nsubs;       /*  */
    double* tevts;        /*  */
    int* evtspt;      /*  */
    int *nevts;       /*  */
    int *pointi;      /*  */
    int *iord;        /*  */
    int *niord;       /*  */
    int *oord;        /*  */
    int *noord;       /*  */
    int *zord;        /*  */
    int *nzord;       /*  */
    int *funptr;      /* block indexes */
    int *funtyp;      /* block types */
    int *ztyp;        /* block types */
    int *cord;        /* block types */
    int *ncord;       /* */
    int *ordclk;      /* block types */
    int *nordclk;     /* size of ordclk */
    int *clkptr;      /* block types */
    int *ordptr;      /* block types */
    int *nordptr;     /* */
    int *critev;      /* block types */
    int *iwa;         /* integer work space of size nevts */
    int *mask;        /* integer work space ng for mask */
    scicos_block *blocks; /* scicos_block structure ptr */
    double *t0;           /* scalar initial time of simulation  */
    double *tf;           /* scalar final time of simulation    */
    double *Atol;         /* scalar absolute tolerance of solvers of diff. equa. */
    double *rtol;         /* scalar relative tolerance of solvers of diff. equa. */
    double *ttol;         /* scalar time tolerance of solvers of diff. equa. */
    double *deltat;       /* deltat */
    double *hmax;         /* hmax */
    outtb_el *outtb_elem; /* table (of size 2) coding each indexes of element of matrix in a same array */
    int *nelem;       /* total unit element */
    int *xprop;       /* xproperties (of size nx) */
    double *xd;           /* derivative of continuous state */
} ScicosImport;

SCICOS_IMPEXP ScicosImport* getscicosimportptr(void);

SCICOS_IMPEXP int getscicosvarsfromimport(char *what, void **v, int *nv, int *mv);

SCICOS_IMPEXP void C2F(getouttb)(int *nsize, int *nvec, double *outtc);

SCICOS_IMPEXP void C2F(getlabel)(int *kfun, char *label, int *n);

SCICOS_IMPEXP int getscilabel(int *kfun, char *label, int *n);

#endif /* __IMPORT_H__ */
