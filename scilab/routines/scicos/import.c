/* Copyright INRIA */


#include "nsp/math.h"
#include "nsp/machine.h"
#include <string.h>
#include "import.h"

extern struct {
  int kfun;
} C2F(curblk);


ScicosImport  scicos_imp={
  (double *) NULL,   /* x */
  (int *) NULL,  /* xptr */
  (int *) NULL,  /* zcptr */
  (double *) NULL,   /* z */
  (int *) NULL,  /* zptr */
  (int *) NULL,   /* mod */
  (int *) NULL,  /* modptr */
  (char **) NULL,  /* iz XXX jpc */
  (char **) NULL,  /* izptr XXX jpc*/
  (int *) NULL,  /* inpptr */
  (int *) NULL,  /* inplnk */
  (int *) NULL,  /* outptr */
  (int *) NULL,  /* outlnk */
  (int *) NULL,  /* lnkptr */
  0,                 /* nlnkptr */
  (double *) NULL,   /* rpar */
  (int *) NULL,  /* rpptr */
  (int *) NULL,  /* ipar */
  (int *) NULL,  /* ipptr */
  0,                 /* nblk */
  (double *) NULL,   /* outtb */
  0,                 /* nout */
  (int *) NULL,  /* subs */
  0,                 /* nsubs */
  (double *) NULL,   /* tevts */
  (int *) NULL,  /* evtspt */
  0,                 /* nevts */
  0,                 /* pointi */
  (int *) NULL,  /* oord */
  (int *) NULL,  /* zord */
  (int *) NULL,  /* funptr */
  (int *) NULL,   /* funtyp */
  (int *) NULL,   /* ztyp */
  (int *) NULL,   /* cord */
  (int *) NULL,   /* ordclk */
  (int *) NULL,   /* clkptr */
  (int *) NULL,   /* ordptr */
  (int *) NULL,   /* critev */
  (int *) NULL,    /* iwa */
};

void  scicos_makescicosimport(double *x, int *xptr, int *zcptr, double *z, int *zptr, int *mod, int *modptr, char **iz, char **izptr, int *inpptr, int *inplnk, int *outptr, int *outlnk, int *lnkptr, int *nlnkptr, double *rpar, int *rpptr, int *ipar, int *ipptr, int *nblk, double *outtb, int *nout, int *subs, int *nsubs, double *tevts, int *evtspt, int *nevts, int *pointi, int *oord, int *zord, int *funptr, int *funtyp, int *ztyp, int *cord, int *ordclk, int *clkptr, int *ordptr, int *critev, int *iwa)
{
    scicos_imp.x=x;
    scicos_imp.xptr=xptr;
    scicos_imp.zcptr=zcptr;
    scicos_imp.z=z;
    scicos_imp.zptr=zptr;
    scicos_imp.mod=mod;
    scicos_imp.modptr=modptr;
    scicos_imp.iz=iz;
    scicos_imp.izptr=izptr;

    scicos_imp.inpptr=inpptr;
    scicos_imp.inplnk=inplnk;
    scicos_imp.outptr=outptr;
    scicos_imp.outlnk=outlnk;
    scicos_imp.lnkptr=lnkptr;
    scicos_imp.nlnkptr=*nlnkptr;

    scicos_imp.rpar=rpar;
    scicos_imp.rpptr=rpptr;
    scicos_imp.ipar=ipar;
    scicos_imp.ipptr=ipptr;
    scicos_imp.nblk=*nblk;
    scicos_imp.outtb=outtb;
    scicos_imp.nout=*nout;

    scicos_imp.subs=subs;
    scicos_imp.nsubs=*nsubs;

    scicos_imp.tevts=tevts;
    scicos_imp.evtspt=evtspt;
    scicos_imp.nevts=*nevts;
    scicos_imp.pointi=*pointi;

    scicos_imp.oord=oord;
    scicos_imp.zord=zord;

    scicos_imp.funptr=funptr;
    scicos_imp.funtyp=funtyp;

    scicos_imp.ztyp=ztyp;
    scicos_imp.cord=cord;
    scicos_imp.ordclk=ordclk;
    scicos_imp.clkptr=clkptr;
    scicos_imp.ordptr=ordptr;
    scicos_imp.critev=critev;
    scicos_imp.iwa=iwa;
}

void scicos_clearscicosimport(void)
{
  scicos_imp.x=(double *) NULL;
  scicos_imp.xptr=(int *) NULL;
  scicos_imp.zcptr=(int *) NULL;
  scicos_imp.z=(double *) NULL;
  scicos_imp.zptr=(int *) NULL;
  scicos_imp.mod=(int *) NULL;
  scicos_imp.modptr=(int *) NULL;
  scicos_imp.iz= NULL;
  scicos_imp.izptr= NULL;
  scicos_imp.inpptr=(int *) NULL;
  scicos_imp.inplnk=(int *) NULL;
  scicos_imp.outptr=(int *) NULL;
  scicos_imp.outlnk=(int *) NULL;
  scicos_imp.lnkptr=(int *) NULL;
  scicos_imp.nlnkptr=0;
  scicos_imp.rpar=(double *) NULL;
  scicos_imp.rpptr=(int *) NULL;
  scicos_imp.ipar=(int *) NULL;
  scicos_imp.ipptr=(int *) NULL;
  scicos_imp.nblk=0;
  scicos_imp.outtb=(double *) NULL;
  scicos_imp.nout=0;
  scicos_imp.subs=(int *) NULL;
  scicos_imp.nsubs=0;
  scicos_imp.tevts=(double *) NULL;
  scicos_imp.evtspt=(int *) NULL;
  scicos_imp.nevts=0;
  scicos_imp.pointi=0;
  scicos_imp.oord=(int *) NULL;
  scicos_imp.zord=(int *) NULL;
  scicos_imp.funptr=(int *) NULL;
  scicos_imp.funtyp=(int *) NULL;
  scicos_imp.ztyp=(int *) NULL;
  scicos_imp.cord=(int *) NULL;
  scicos_imp.ordclk=(int *) NULL;
  scicos_imp.clkptr=(int *) NULL;
  scicos_imp.ordptr=(int *) NULL;
  scicos_imp.critev=(int *) NULL;
  scicos_imp.iwa=(int *) NULL;
}

/* 
 * data structure selection 
 * Pointer to the beginning of the imported data 
 * nv,   size of the imported data 
 * type  type of the imported data 0:int,1:double 
 */

int  scicos_getscicosvars(int *what, double **v, int *nv, int *type)
{
  int nblk;
  if (scicos_imp.x == NULL)
    {
      *v= NULL;
      return(2); /* undefined import table scicos is not running */
    }
  nblk=scicos_imp.nblk;
  /* imported from */
  switch (*what) 
    {
    case 1 :			/* continuous state */
      *nv=(int) (scicos_imp.xptr[nblk]-scicos_imp.xptr[0]);
      *v=(void *)(scicos_imp.x);
      *type=1;
      break;
    case 2 :			/* continuous state splitting array*/
      *nv=(int)(nblk+1);
      *v=(void *) (scicos_imp.xptr);
      *type=0;
      break;
    case 3 :			/* continuous state splitting array*/
      *nv=(int)(nblk+1);
      *v=(void *) (scicos_imp.zcptr);
      *type=0;
      break;
    case 4 :			/* discrete state */
      *nv=(int)(scicos_imp.zptr[nblk]-scicos_imp.zptr[0]);
      *v=(void *) (scicos_imp.z);
      *type=1;
      break;
    case 5 :			/* discrete  state splitting array*/
      *nv=(int)(nblk+1);
      *v=(void *) (scicos_imp.zptr);
      *type=0;
      break;
    case 6 :			/* rpar */
      *nv=(int)(scicos_imp.rpptr[nblk]-scicos_imp.rpptr[0]);
      *v=(void *) (scicos_imp.rpar);
      *type=1;
      break;
    case 7 :			/* rpar  splitting array*/
      *nv=(int)(nblk+1);
      *v=(void *) (scicos_imp.rpptr);
      *type=0;
      break;
    case 8 :			/* ipar */
      *nv=(int)(scicos_imp.ipptr[nblk]-scicos_imp.ipptr[0]);
      *v=(void *) (scicos_imp.ipar);
      *type=0;
      break;
    case 9 :			/* ipar  splitting array*/
      *nv=(int)(nblk+1);
      *v=(void *) (scicos_imp.ipptr);
      *type=0;
      break;
    case 10 :			/* outtb */
      *nv=(int)(scicos_imp.nout);
      *v=(void *) (scicos_imp.outtb);
      *type=1;
      break;
    case 11 :                   /* inpptr */
      *nv=(int)(nblk+1);
      *v=(void *) (scicos_imp.inpptr); 
      *type=0;
      break;
    case 12 :                   /* outptr */
      *nv=(int)(nblk+1);
      *v=(void *) (scicos_imp.outptr); 
      *type=0;
      break;
    case 13 :                   /* inplnk */
      *nv=(int)(scicos_imp.inpptr[nblk]-scicos_imp.inpptr[0]); 
      *v=(void *) (scicos_imp.inplnk); 
      *type=0;
      break;
    case 14 :                   /* outlnk */
      *nv=(int)(scicos_imp.outptr[nblk]-scicos_imp.outptr[0]); 
      *v=(void *) (scicos_imp.outlnk); 
      *type=0;
      break;
    case 15 :                   /* lnkptr */
      *nv=(int)(scicos_imp.nlnkptr);
      *v=(void *) (scicos_imp.lnkptr); 
      *type=0;
      break;
    }
  return(0);
}


void  scicos_getlabel(int *kfun,char **label)
{
  *label = scicos_imp.izptr[*kfun];
}


int scicos_getblockbylabel(int *kfun,const char *label) 
{
  int k,nblk;
  nblk=(int)(scicos_imp.nblk);
  *kfun=0;
  for ( k=0 ; k < nblk ; k++) 
    {
      /* FIXME: j'y comprends rien 
      n1=(int)(scicos_imp.izptr[k]-scicos_imp.izptr[k-1]);
      if (n1==*n) {
	i0=scicos_imp.izptr[k-1]-1;
	i=0;
	while ((lab[i]==scicos_imp.iz[i0+i])&(i<n1)) i++;
	if (i==n1) {
	  *kfun=k+1;
	  return;
	  }
	}
      */
    }
  return FAIL;
}

int scicos_getsciblockbylabel(int *kfun,const char *label) 
{
  int k,nblk;
  if (scicos_imp.x==(double *)NULL){
    return FAIL;
  }
  nblk=(int)(scicos_imp.nblk);
  *kfun=0;
  for( k=0 ; k < nblk ; k++) 
    {
      if (strcmp(scicos_imp.izptr[k],label)==0) 
	{
	  *kfun=k+1;
	  return OK;
	  break;
	}
    }
  return FAIL;
}

int scicos_getscilabel(int *kfun,char **label)
{
  if (scicos_imp.x==(double *)NULL) 
    {
      return FAIL; /* undefined import table scicos is not running */
    }
  *label = scicos_imp.izptr[*kfun];
  return OK;
}

int scicos_getcurblock(void)
{
  return(C2F(curblk).kfun);
}


void scicos_getouttb(int *nsize,int *nvec, double *outtc)
{
  int i;
  for (i=0;i<*nsize;i++)
    outtc[i]=(double)scicos_imp.outtb[nvec[i]-1];  
}
