#ifndef __IMPORT_H__
#define __IMPORT_H__

/* Copyright INRIA */
typedef struct {
  double* x;      /* continuous  state */
  integer* xptr;  /* vector of pointers on block states */
  integer* zcptr;  /* vector of pointers on block zero crosses */
  double* z;      /* discrete state */
  integer* zptr;  /* vector of pointers on block states */
  integer* mod;      /* modes of the blocks */
  integer* modptr;  /* vector of pointers */
  integer* iz;      /* unused */
  integer* izptr;  /* vector of pointers on iz */
  integer* inpptr; /* vector of pointers on block inputs */
  integer* inplnk;
  integer* outptr;/* vector of pointers on block outputs */
  integer* outlnk;
  integer* lnkptr;
  integer nlnkptr; /* size of lnkptr */
  double * rpar;  /* vector of real parameters */
  integer* rpptr; /* vector of pointers on block real parameters */
  integer* ipar;  /* vector of integer parameters */
  integer* ipptr; /* vector of pointers on block integer parameters */
  integer nblk;   /* number of  blocks */
  double * outtb; /* vector of outputs*/
  integer nout;   /* size of outtb */
  integer* subs;  /* import structure */
  integer nsubs;  /* number of imported data */
  double* tevts;
  integer* evtspt;
  integer nevts;
  integer pointi;
  integer *oord;
  integer *zord;
  integer *funptr; /* block indexes */
  integer *funtyp; /* block types */
  integer *ztyp; /* block types */
  integer *cord; /* block types */
  integer *ordclk; /* block types */
  integer *clkptr; /* block types */
  integer *ordptr; /* block types */
  integer *critev; /* block types */
  integer *iwa; /* integer work space 2*nblk */
  integer *mask; /* integer work space ng for mask */
} ScicosImport;

#endif /* __IMPORT_H__ */
