/* Copyright INRIA */
#include "../machine.h"
#include <string.h>
#include "import.h"

extern  int C2F(cvstr)  __PARAMS((integer *,integer *,char *,integer *,unsigned long int));

extern struct {
  integer kfun;
} C2F(curblk);



ScicosImport  scicos_imp={
(double *) NULL,   /* x */
(integer *) NULL,  /* xptr */
(integer *) NULL,  /* zcptr */
(double *) NULL,   /* z */
(integer *) NULL,  /* zptr */
(integer *) NULL,   /* mod */
(integer *) NULL,  /* modptr */
(integer *) NULL,  /* iz */
(integer *) NULL,  /* izptr */

(integer *) NULL,  /* inpptr */
(integer *) NULL,  /* inplnk */
(integer *) NULL,  /* outptr */
(integer *) NULL,  /* outlnk */
(integer *) NULL,  /* lnkptr */
0,                 /* nlnkptr */
(double *) NULL,   /* rpar */
(integer *) NULL,  /* rpptr */
(integer *) NULL,  /* ipar */
(integer *) NULL,  /* ipptr */
0,                 /* nblk */
(double *) NULL,   /* outtb */
0,                 /* nout */
(integer *) NULL,  /* subs */
0,                 /* nsubs */
(double *) NULL,   /* tevts */
(integer *) NULL,  /* evtspt */
0,                 /* nevts */
0,                 /* pointi */
(integer *) NULL,  /* oord */
(integer *) NULL,  /* zord */
(integer *) NULL,  /* funptr */
(integer *) NULL,   /* funtyp */
(integer *) NULL,   /* ztyp */
(integer *) NULL,   /* cord */
(integer *) NULL,   /* ordclk */
(integer *) NULL,   /* clkptr */
(integer *) NULL,   /* ordptr */
(integer *) NULL,   /* critev */
(integer *) NULL,    /* iwa */
};

void  
C2F(makescicosimport)(x,xptr,zcptr,z,zptr,mod,modptr,iz,izptr,
     inpptr,inplnk,outptr,outlnk,lnkptr,nlnkptr,
     rpar,rpptr,ipar,ipptr,nblk,outtb,nout,subs,nsubs,
     tevts,evtspt,nevts,pointi,oord,zord,
     funptr,funtyp,ztyp,cord,ordclk,clkptr,ordptr,critev,iwa)
     
double *x ,*z,*outtb,*rpar,*tevts;
integer *xptr,*zcptr,*zptr,*iz,*izptr,*inpptr,*inplnk,*outptr,*outlnk,*lnkptr;
integer *nlnkptr,*rpptr,*ipar,*ipptr,*nblk,*nout,*subs,*nsubs;
integer *evtspt,*nevts,*pointi,*oord,*zord,*funptr,*funtyp,*ztyp,*cord,*ordclk;
integer *clkptr,*ordptr,*critev, *iwa, *mod,*modptr;
     
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

void
C2F(clearscicosimport)()
{
    scicos_imp.x=(double *) NULL;
    scicos_imp.xptr=(integer *) NULL;
    scicos_imp.zcptr=(integer *) NULL;
    scicos_imp.z=(double *) NULL;
    scicos_imp.zptr=(integer *) NULL;
    scicos_imp.mod=(integer *) NULL;
    scicos_imp.modptr=(integer *) NULL;
    scicos_imp.iz=(integer *) NULL;
    scicos_imp.izptr=(integer *) NULL;

    scicos_imp.inpptr=(integer *) NULL;
    scicos_imp.inplnk=(integer *) NULL;
    scicos_imp.outptr=(integer *) NULL;
    scicos_imp.outlnk=(integer *) NULL;
    scicos_imp.lnkptr=(integer *) NULL;
    scicos_imp.nlnkptr=0;

    scicos_imp.rpar=(double *) NULL;
    scicos_imp.rpptr=(integer *) NULL;
    scicos_imp.ipar=(integer *) NULL;
    scicos_imp.ipptr=(integer *) NULL;
    scicos_imp.nblk=0;
    scicos_imp.outtb=(double *) NULL;
    scicos_imp.nout=0;

    scicos_imp.subs=(integer *) NULL;
    scicos_imp.nsubs=0;
    scicos_imp.tevts=(double *) NULL;
    scicos_imp.evtspt=(integer *) NULL;
    scicos_imp.nevts=0;
    scicos_imp.pointi=0;

    scicos_imp.oord=(integer *) NULL;
    scicos_imp.zord=(integer *) NULL;

    scicos_imp.funptr=(integer *) NULL;
    scicos_imp.funtyp=(integer *) NULL;

    scicos_imp.ztyp=(integer *) NULL;
    scicos_imp.cord=(integer *) NULL;
    scicos_imp.ordclk=(integer *) NULL;
    scicos_imp.clkptr=(integer *) NULL;
    scicos_imp.ordptr=(integer *) NULL;
    scicos_imp.critev=(integer *) NULL;

    scicos_imp.iwa=(integer *) NULL;
}

integer  
C2F(getscicosvars)(what,v,nv,type)
integer *what;  /* data structure selection */
double **v;    /* Pointer to the beginning of the imported data */
integer *nv;   /* size of the imported data */
integer *type ;/* type of the imported data 0:integer,1:double */
{
    int nblk;

    if (scicos_imp.x==(double *)NULL){
	*v=(void *) NULL;
	return(2); /* undefined import table scicos is not running */
    }
    nblk=scicos_imp.nblk;
    /* imported from */
    switch (*what) {
    case 1 :			/* continuous state */
	*nv=(integer) (scicos_imp.xptr[nblk]-scicos_imp.xptr[0]);
        *v=(void *)(scicos_imp.x);
	*type=1;
	break;
    case 2 :			/* continuous state splitting array*/
	*nv=(integer)(nblk+1);
	*v=(void *) (scicos_imp.xptr);
	*type=0;
	break;
    case 3 :			/* continuous state splitting array*/
	*nv=(integer)(nblk+1);
	*v=(void *) (scicos_imp.zcptr);
	*type=0;
	break;
    case 4 :			/* discrete state */
	*nv=(integer)(scicos_imp.zptr[nblk]-scicos_imp.zptr[0]);
	*v=(void *) (scicos_imp.z);
	*type=1;
	break;
    case 5 :			/* discrete  state splitting array*/
	*nv=(integer)(nblk+1);
	*v=(void *) (scicos_imp.zptr);
	*type=0;
	break;
    case 6 :			/* rpar */
	*nv=(integer)(scicos_imp.rpptr[nblk]-scicos_imp.rpptr[0]);
	*v=(void *) (scicos_imp.rpar);
	*type=1;
	break;
    case 7 :			/* rpar  splitting array*/
	*nv=(integer)(nblk+1);
	*v=(void *) (scicos_imp.rpptr);
	*type=0;
	break;
    case 8 :			/* ipar */
	*nv=(integer)(scicos_imp.ipptr[nblk]-scicos_imp.ipptr[0]);
	*v=(void *) (scicos_imp.ipar);
	*type=0;
	break;
    case 9 :			/* ipar  splitting array*/
	*nv=(integer)(nblk+1);
	*v=(void *) (scicos_imp.ipptr);
	*type=0;
	break;
    case 10 :			/* outtb */
	*nv=(integer)(scicos_imp.nout);
	*v=(void *) (scicos_imp.outtb);
	*type=1;
	break;
    case 11 :                   /* inpptr */
	*nv=(integer)(nblk+1);
	*v=(void *) (scicos_imp.inpptr); 
	*type=0;
	break;
    case 12 :                   /* outptr */
	*nv=(integer)(nblk+1);
	*v=(void *) (scicos_imp.outptr); 
	*type=0;
	break;
    case 13 :                   /* inplnk */
	*nv=(integer)(scicos_imp.inpptr[nblk]-scicos_imp.inpptr[0]); 
	*v=(void *) (scicos_imp.inplnk); 
	*type=0;
	break;
    case 14 :                   /* outlnk */
	*nv=(integer)(scicos_imp.outptr[nblk]-scicos_imp.outptr[0]); 
	*v=(void *) (scicos_imp.outlnk); 
	*type=0;
	break;
    case 15 :                   /* lnkptr */
	*nv=(integer)(scicos_imp.nlnkptr);
	*v=(void *) (scicos_imp.lnkptr); 
	*type=0;
	break;
    }
    return(0);
}
void 
C2F(getlabel)(kfun,label,n)
integer *n, *kfun;  /* length of the label 
                       as input n gives the max length expected*/
char *label;    
{
    int k;
    int job=1;

    k= *kfun;
    if (*n>(integer)(scicos_imp.izptr[k]-scicos_imp.izptr[k-1])){
      *n=(integer)(scicos_imp.izptr[k]-scicos_imp.izptr[k-1]);
    }
    if (*n>0 )
	F2C(cvstr)(n,&(scicos_imp.iz[scicos_imp.izptr[k-1]-1]),label,&job,*n);
}
void 
C2F(getblockbylabel)(kfun,label,n)
integer *n, *kfun;  /* length of the label */
char **label;    
{
    int k,i,i0,nblk,n1;
    int job=0;
    int lab[40];

    nblk=(integer)(scicos_imp.nblk);
    F2C(cvstr)(n,lab,*label,&job,*n);

    *kfun=0;
    for (k=0;k<nblk;k++) {
      n1=(integer)(scicos_imp.izptr[k]-scicos_imp.izptr[k-1]);
      if (n1==*n) {
	i0=scicos_imp.izptr[k-1]-1;
	i=0;
	while ((lab[i]==scicos_imp.iz[i0+i])&(i<n1)) i++;
	if (i==n1) {
	  *kfun=k+1;
	  return;
	}
      }
    }
}
integer
C2F(getsciblockbylabel)(kfun,label,n)
integer *n, *kfun;  /* length of the label */
integer label[];    
{
    int k,i,i0,nblk,n1;
    if (scicos_imp.x==(double *)NULL){
	return(2); /* undefined import table scicos is not running */
    }
    nblk=(integer)(scicos_imp.nblk);

    *kfun=0;
    for (k=0;k<nblk;k++) {
      n1=(integer)(scicos_imp.izptr[k]-scicos_imp.izptr[k-1]);
      if (n1==*n) {
	i0=scicos_imp.izptr[k-1]-1;
	i=0;
	while ((label[i]==scicos_imp.iz[i0+i])&(i<n1)) i++;
	if (i==n1) {
	  *kfun=k+1;
	  return 0;
	}
      }
    }
    return 0;
}

integer
C2F(getscilabel)(kfun,label,n)
integer *n, *kfun;  /* length of the label */
integer label[];    
{
    int k,i;
    integer *u,*y;

    if (scicos_imp.x==(double *)NULL){
	return(2); /* undefined import table scicos is not running */
    }
    k= *kfun;
    *n=(integer)(scicos_imp.izptr[k]-scicos_imp.izptr[k-1]);
    if (*n>0 ) {
	u=(integer *)&(scicos_imp.iz[scicos_imp.izptr[k-1]-1]);
	y=label;
	for (i=0;i<*n;i++)
	    *(y++)=*(u++);  
	}
    return(0);
}

integer C2F(getcurblock)()
{
return(C2F(curblk).kfun);
    }

void 
C2F(getouttb)(nsize,nvec,outtc)
integer *nsize,*nvec;
double *outtc;
{
  integer i;
  for (i=0;i<*nsize;i++)
    outtc[i]=(double)scicos_imp.outtb[nvec[i]-1];  
}
