#ifndef __IMPORT_H__
#define __IMPORT_H__
#include "scicos_block4.h"
/* Copyright INRIA */
typedef struct
{
  int lnk;
  int pos;
} outtb_el;

typedef struct {
  double* x;            /* 1  - continuous state */
  integer *nx;          /* 2  - length of continuous sate  */
  integer* xptr;        /* 3  - vector of pointers on block states */
  integer* zcptr;       /* 4  - vector of pointers on block zero crosses */
  double* z;            /* 5  - discrete state */
  integer *nz;          /* 6  - length of discrete state */
  integer* zptr;        /* 7  - vector of pointers on block states */
  double* g;            /* 8  - zero crossing state */
  integer *ng;          /* 9  - length of zero crossing register */
  integer* mod;         /* 10 - modes of the blocks */
  integer *nmod;        /* 11 - length of mod register */
  integer* modptr;      /* 12 - vector of pointers */
  integer* iz;          /* 13 - vectors of labels */
  integer* izptr;       /* 14 - vector of pointers on block labels */
  integer* inpptr;      /* 15 - vector of pointers on block inputs */
  integer* inplnk;      /* 16 - vector of pointers on input link */
  integer* outptr;      /* 17 - vector of pointers on block outputs */
  integer* outlnk;      /* 18 - vector of pointers on output link */
  double * rpar;        /* 19 - vector of real parameters */
  integer* rpptr;       /* 20 - vector of pointers on block real parameters */
  integer* ipar;        /* 21 - vector of integer parameters */
  integer* ipptr;       /* 22 - vector of pointers on block integer parameters */
  integer *nblk;        /* 23 - number of  blocks */
  void **outtbptr;      /* 24 - vector of pointer of object of outtb*/
  integer *outtbsz;     /* 25 - vector of size of object of outtb*/
  integer *outtbtyp;    /* 26 - vector of type of object of outtb*/
  integer *nlnk;        /* 27 - number of object of outtb*/
  integer* subs;        /* 28 -  */
  integer *nsubs;       /* 29 -  */
  double* tevts;        /* 30 -  */
  integer* evtspt;      /* 31 -  */
  integer *nevts;       /* 32 -  */
  integer *pointi;      /* 33 -  */
  integer *iord;        /* 34 -  */
  integer *niord;       /* 35 -  */
  integer *oord;        /* 36 -  */
  integer *noord;       /* 37 -  */
  integer *zord;        /* 38 -  */
  integer *nzord;       /* 39 -  */
  integer *funptr;      /* 40 - block indexes */
  integer *funtyp;      /* 41 - block types */
  integer *ztyp;        /* 42 - block types */
  integer *cord;        /* 43 - block types */
  integer *ncord;       /* 44 - */
  integer *ordclk;      /* 45 - block types */
  integer *clkptr;      /* 46 - block types */
  integer *ordptr;      /* 47 - block types */
  integer *nordptr;     /* 48 - */
  integer *critev;      /* 49 - block types */
  integer *iwa;         /* 50 - integer work space of size nevts */
  integer *mask;        /* 51 - integer work space ng for mask */
  scicos_block *blocks; /* 52 - scicos_block structure ptr */
  double *t0;           /* 53 - t0       */
  double *tf;           /* 54 - tf       */
  double *Atol;         /* 55 - Atol     */
  double *rtol;         /* 56 - rtol     */
  double *ttol;         /* 57 - ttol     */
  double *deltat;       /* 58 - deltat   */
  double *hmax;         /* 59 - hmax     */
  outtb_el *outtb_elem;
  integer *nelem;
} ScicosImport;

#endif /* __IMPORT_H__ */
