#ifndef __IMPORT_H__
#define __IMPORT_H__

/* Copyright INRIA */

typedef struct {
  double* x;      /* continuous  state */
  int* xptr;  /* vector of pointers on block states */
  int* zcptr;  /* vector of pointers on block zero crosses */
  double* z;      /* discrete state */
  int* zptr;  /* vector of pointers on block states */
  int* mod;      /* modes of the blocks */
  int* modptr;  /* vector of pointers */
  char** iz;      /* unused */
  char** izptr;  /* vector of pointers on iz */
  int* inpptr; /* vector of pointers on block inputs */
  int* inplnk;
  int* outptr;/* vector of pointers on block outputs */
  int* outlnk;
  int* lnkptr;
  int nlnkptr; /* size of lnkptr */
  double * rpar;  /* vector of real parameters */
  int* rpptr; /* vector of pointers on block real parameters */
  int* ipar;  /* vector of int parameters */
  int* ipptr; /* vector of pointers on block int parameters */
  int nblk;   /* number of  blocks */
  double * outtb; /* vector of outputs*/
  int nout;   /* size of outtb */
  int* subs;  /* import structure */
  int nsubs;  /* number of imported data */
  double* tevts;
  int* evtspt;
  int nevts;
  int pointi;
  int *oord;
  int *zord;
  int *funptr; /* block indexes */
  int *funtyp; /* block types */
  int *ztyp; /* block types */
  int *cord; /* block types */
  int *ordclk; /* block types */
  int *clkptr; /* block types */
  int *ordptr; /* block types */
  int *critev; /* block types */
  int *iwa; /* int work space 2*nblk */
  int *mask; /* int work space ng for mask */
} ScicosImport;

#endif /* __IMPORT_H__ */
