/*
 * nsp version
 *  Jean-Philippe Chancelier 
 *  Copyright Enpc 
 */

#include "nsp/machine.h"
#include "nsp/object.h"
#include "scicos_block.h"

NspMatrix *scicos_itosci(const int x[],int mx,int nx) 
{
  int i;
  NspMatrix *M;
  if ((M = nsp_matrix_create(NVOID,'r',mx,nx)) == NULLMAT) return NULLMAT; 
  for ( i = 0 ; i < M->mn; i++) M->R[i]=(double) x[i];
  return M;
}

NspMatrix *scicos_dtosci(const double x[],int mx,int nx) 
{
  int i;
  NspMatrix *M;
  if ((M = nsp_matrix_create(NVOID,'r',mx,nx)) == NULLMAT) return NULLMAT; 
  for ( i = 0 ; i < M->mn; i++) M->R[i]= x[i];
  return M;
}


NspSMatrix *scicos_str2sci(const char *x[],int n,int m)
{
  NspSMatrix *S;
  if ((S=nsp_smatrix_create_from_array(n*m,x))== NULLSMAT) return  NULLSMAT;
  /* reshape */
  S->m = m;
  S->n = n;
  return S;
}

void scicos_scitovv(double x[],int nx, NspObject *Ob )
{
  NspMatrix *M= ((NspMatrix *) Ob);
  int i; 
  for ( i= 0 ; i < Min(nx,M->mn) ; i++) x[i]=M->R[i];
}

NspMatrix *scicos_vvtosci(const double x[],int nx) 
{
  int i;
  NspMatrix *M;
  if ((M = nsp_matrix_create(NVOID,'r',1,nx)) == NULLMAT) return NULLMAT; 
  for ( i = 0 ; i < M->mn; i++) M->R[i]= x[i];
  return M;
}

int scicos_scitod(double x[],int mx,int nx, NspObject *Ob)
{
  NspMatrix *M= ((NspMatrix *) Ob);
  int i;
  if ( M->m != mx || M->n != nx ) 
    {
      Scierror("Expecting a (%d,%d) matrix\n",mx,nx);
      return FAIL;
    }
  for ( i = 0 ; i < M->mn; i++) x[i]= M->R[i];
  return OK;
}


int scicos_list_to_vars(double *outptr[],int nout,int outsz[],NspObject *Ob)
{
  int k; 
  NspList *L= (NspList *) Ob;
  for ( k = nout-1; k >=0 ; k--) 
    {
      NspObject *elt = nsp_list_get_element(L,k+1);
      if ( elt == NULL) return FAIL;
      if ( scicos_scitod(outptr[k],outsz[k],1,elt)== FAIL) return FAIL;
    }
  return OK;
}

NspObject *scicos_vars_to_list(double *inptr[],int nin,int insz[])
{
  int k;
  NspObject *Ob;
  if ((Ob = (NspObject *) nsp_list_create("L",NULL) ) == NULL) return NULL;
  for( k=0 ; k < nin ; k++) 
    {
      NspObject *elt;
      if ((elt = (NspObject *) scicos_dtosci(inptr[k],insz[k],1))== NULL)
	{
	  nsp_list_destroy((NspList *) Ob);
	  return NULL;
	}
      if ( nsp_list_insert((NspList *) Ob,elt,k+1) == FAIL)
	{
	  nsp_list_destroy((NspList *) Ob);
	  return NULL;
	}
    }
  return Ob;
}



void  sciblk2(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,rpar,nrpar,
	      ipar,nipar,inptr,insz,nin,outptr,outsz,nout)
     int *flag,*nevprt,*nx,*nz,*ntvec,*nrpar,ipar[],*nipar,insz[],*nin,outsz[],*nout;
     double x[],xd[],z[],tvec[],rpar[];
     double *inptr[],*outptr[],*t;
{
  int mlhs=5,mrhs=8;
  NspObject * Args[8]; 
  NspObject * Ret[5];

  /* FIXME: give names to all */
  if ((Args[0]= (NspObject *) scicos_itosci(flag,1,1)) == NULL) goto err;
  if ((Args[1]= (NspObject *) scicos_itosci(nevprt,1,1)) == NULL) goto err;
  if ((Args[2]= (NspObject *) scicos_dtosci(t,1,1)) == NULL) goto err;
  if ((Args[3]= (NspObject *) scicos_dtosci(x,*nx,1)) == NULL) goto err;
  if ((Args[4]= (NspObject *) scicos_vvtosci(z,*nz)) == NULL) goto err;
  if ((Args[5]= (NspObject *) scicos_vvtosci(rpar,*nrpar)) == NULL) goto err; 
  if ((Args[6]= (NspObject *) scicos_itosci(ipar,*nipar,1)) == NULL) goto err;
  if ((Args[7]= (NspObject *) nsp_list_create("L",NULL) ) == NULL) goto err;
  if ((Args[8]= scicos_vars_to_list(inptr,*nin,insz))==NULLOBJ) goto err;

  if ( scicos_scifunc(Args,mlhs,mrhs) == FAIL) goto err;
  switch (*flag) 
    {
    case 1 :
      scicos_scitovv(z,*nz,Ret[2]);
      scicos_scitod(x,*nx,1,Ret[1]);
      if (*nout != 0 ) 
	{
	  if ( scicos_list_to_vars(outptr,*nout,outsz,Ret[0])==FAIL) goto err;
	}
      break;
    case 0 :
      /*     [y,x,z,tvec,xd]=func(flag,nevprt,t,x,z,rpar,ipar,u) */

      /*  x'  computation */
      scicos_scitod(xd,*nx,1,Ret[4]);
      break;
    case 2 :
      scicos_scitovv(z,*nz,Ret[2]);
      scicos_scitod(x,*nx,1,Ret[1]);
      break;
    case 3 :
      scicos_scitod(tvec,*ntvec,1,Ret[3]);
      break;
    case 4 :
    case 5 :
      scicos_scitovv(z,*nz,Ret[2]);
      scicos_scitod(x,*nx,1,Ret[1]);
      break;
    case 6 :
      scicos_scitovv(z,*nz,Ret[2]);
      scicos_scitod(x,*nx,1,Ret[1]);
      if ( *nout !=0 ) 
	{
	  if ( scicos_list_to_vars(outptr,*nout,outsz,Ret[0])==FAIL) goto err;
	}
      break;
    }
  return;
 err: 
    *flag=-1;
}


void 
sciblk2i(flag,nevprt,t,residual,xd,x,nx,z,nz,tvec,ntvec,rpar,nrpar,
	 ipar,nipar,inptr,insz,nin,outptr,outsz,nout)
     int *flag,*nevprt,*nx,*nz,*ntvec,*nrpar,ipar[],*nipar,insz[],*nin,outsz[],*nout;
     double residual[],x[],xd[],z[],tvec[],rpar[];
     double *inptr[],*outptr[],*t;
{
  int mlhs=6,mrhs=9;
  /*
  [y,  x,  z,  tvec,xd]=func(flag,nevprt,t,xd,x,z,rpar,ipar,u)
  [y,  x,  z,  tvec,res]=func(flag,nevprt,t,xd,x,z,rpar,ipar,u)
  */
  NspObject * Args[9]; 
  NspObject * Ret[5]; /* FIXME */

  /* FIXME: give names to all */
  if ((Args[0]= (NspObject *) scicos_itosci(flag,1,1)) == NULL) goto err;
  if ((Args[1]= (NspObject *) scicos_itosci(nevprt,1,1)) == NULL) goto err;
  if ((Args[2]= (NspObject *) scicos_dtosci(t,1,1)) == NULL) goto err;
  if ((Args[3]= (NspObject *) scicos_dtosci(xd,*nx,1)) == NULL) goto err;
  if ((Args[4]= (NspObject *) scicos_dtosci(x,*nx,1)) == NULL) goto err;
  if ((Args[5]= (NspObject *) scicos_vvtosci(z,*nz)) == NULL) goto err;
  if ((Args[6]= (NspObject *) scicos_vvtosci(rpar,*nrpar)) == NULL) goto err; 
  if ((Args[7]= (NspObject *) scicos_itosci(ipar,*nipar,1)) == NULL) goto err;
  if ((Args[8]= (NspObject *) nsp_list_create("L",NULL) ) == NULL) goto err;
  if ((Args[9]= scicos_vars_to_list(inptr,*nin,insz))==NULLOBJ) goto err;
  if ( scicos_scifunc(Args,mlhs,mrhs) == FAIL) goto err;
  switch (*flag) 
    {
    case 1 :
      /* y  computation */
      /* int Ret[0] */
      if ( *nout != 0 ) 
	{
	  if ( scicos_list_to_vars(outptr,*nout,outsz,Ret[0])==FAIL) goto err;
	}
      break;
    case 0 :
      /*  residual  computation */
      scicos_scitod(residual,*nx,1,Ret[4]);
      break;
    case 2 : 
      /* continuous and discrete state jump */
      scicos_scitod(xd,*nx,1,Ret[4]);
      scicos_scitovv(z,*nz,Ret[2]);
      scicos_scitod(x,*nx,1,Ret[1]);
      break;
    case 3 :
      /* output event */
      scicos_scitod(tvec,*ntvec,1,Ret[3]);
      break;
    case 4 :
      scicos_scitod(xd,*nx,1,Ret[4]);
      scicos_scitovv(z,*nz,Ret[2]);
      scicos_scitod(x,*nx,1,Ret[1]);
      break;
    case 5 :
      scicos_scitod(xd,*nx,1,Ret[4]);
      scicos_scitovv(z,*nz,Ret[2]);
      scicos_scitod(x,*nx,1,Ret[1]);
      break;
    case 6 :
      scicos_scitod(xd,*nx,1,Ret[4]);
      scicos_scitovv(z,*nz,Ret[2]);
      scicos_scitod(x,*nx,1,Ret[1]);
      if (*nout !=0) 
	{
	  if ( scicos_list_to_vars(outptr,*nout,outsz,Ret[0])==FAIL) goto err;
	}
      break;
    }
  return;
 err: 
  *flag=-1;
}



void sciblk4(scicos_block *Blocks, int flag)
{
  int mlhs=1,mrhs=2;
  NspObject *Ob;
  NspHash *H;
  NspObject * Args[31]; 
  NspObject * Ret[5];
  int p = 0;
  /* this are the tlist names */
  char *str[]={ "scicos_block","nevprt","funpt","type",
		"scsptr","nz","z","nx","x","xd","res","nin",
		"insz","inptr","nout","outsz","outptr","nevout",
		"evout","nrpar","rpar","nipar","ipar","ng","g",
		"ztyp","jroot","label","work","nmode","mode"};
  if ((Args[p++]= (NspObject *)  scicos_itosci(&Blocks->nevprt,1,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(Blocks->funpt,0,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(&Blocks->type,1,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(&Blocks->scsptr,0,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(&Blocks->nz,1,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_vvtosci(Blocks->z,Blocks->nz))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(&Blocks->nx,1,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_dtosci(Blocks->x,Blocks->nx,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_dtosci(Blocks->xd,Blocks->nx,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_dtosci(Blocks->res,Blocks->nx,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(&Blocks->nin,1,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(Blocks->insz,Blocks->nin,1))== NULL) goto err;
  if ((Args[p++]= scicos_vars_to_list(Blocks->inptr,Blocks->nin,Blocks->insz))==NULLOBJ) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(Blocks->outsz,Blocks->nout,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(&Blocks->nout,1,1))== NULL) goto err;
  if ((Args[p++]= scicos_vars_to_list(Blocks->outptr,Blocks->nout,Blocks->outsz))==NULLOBJ) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(&Blocks->nevout,1,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_dtosci(Blocks->evout,Blocks->nevout,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(&Blocks->nrpar,1,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_vvtosci(Blocks->rpar,Blocks->nrpar))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(&Blocks->nipar,1,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(Blocks->ipar,Blocks->nipar,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(&Blocks->ng,1,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_dtosci(Blocks->g,Blocks->ng,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(&Blocks->ztyp,1,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(Blocks->jroot,Blocks->ng,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_str2sci(Blocks->label,1,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_vvtosci(Blocks->work,0))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(Blocks->nmode,1,1))== NULL) goto err;
  if ((Args[p++]= (NspObject *)  scicos_itosci(Blocks->mode,Blocks->nmode,1))== NULL) goto err; 
  if ((Args[p++]= (NspObject *)  scicos_itosci(Blocks->mode,Blocks->nmode,1))== NULL) goto err; 

  if ( scicos_scifunc(Args,mlhs,mrhs) == FAIL) goto err;
  H=(NspHash *) Ret[0];
  switch (flag) {
  case 1 :
    /* y computation */
    if (Blocks->nout!=0) 
      {
	/* 16ieme element de la tlist y */
	if ( nsp_hash_find(H,"y",&Ob) == FAIL) goto err;
	if ( scicos_list_to_vars(Blocks->outptr,Blocks->nout,Blocks->outsz,Ob)==FAIL) goto err;
      }
    break;
  case 0 :
    /*  x'  computation */
    /* 9 ieme element de la tlist xd */
    if (Blocks->nx != 0)
      {
	if ( nsp_hash_find(H,"xd",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->xd,Blocks->nx,1,Ob);
	/* 10 ieme element de la tlist res */
	if ( nsp_hash_find(H,"res",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->res,Blocks->nx,1,Ob);
      }
      break;
  case 2 :
    /* 6ieme element de la tlist z */
    if (Blocks->nz != 0) 
      {
	if ( nsp_hash_find(H,"z",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->z,Blocks->nz,1,Ob);
      }
    /* 8 ieme element de la tlist x */
    if (Blocks->nx != 0)
      {
	if ( nsp_hash_find(H,"x",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->x,Blocks->nx,1,Ob);
	/* 9 ieme element de la tlist xd */
	if ( nsp_hash_find(H,"xd",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->xd,Blocks->nx,1,Ob);
      }
    if ( nsp_hash_find(H,"mode",&Ob) == FAIL) goto err;
    scicos_scitod(Blocks->mode,Blocks->nmode,1,Ob);
    break;
  case 3 :
    if ( nsp_hash_find(H,"evout",&Ob) == FAIL) goto err;
    scicos_scitod(Blocks->evout,Blocks->nevout,1,Ob);
    break;
  case 4 :
    if (Blocks->nz != 0) 
      {
	if ( nsp_hash_find(H,"z",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->z,Blocks->nz,1,Ob);
      }
    if (Blocks->nx != 0) 
      {
	/* 8ieme element de la tlist x */
	if ( nsp_hash_find(H,"x",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->x,Blocks->nx,1,Ob);
	/* 9 ieme element de la tlist xd */
	if ( nsp_hash_find(H,"xd",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->xd,Blocks->nx,1,Ob);
    }
    break;
  case 5 :
    if (Blocks->nz != 0) 
      {
	if ( nsp_hash_find(H,"z",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->z,Blocks->nz,1,Ob);
      }
    if (Blocks->nx != 0) 
      {
	/* 8ieme element de la tlist x */
	if ( nsp_hash_find(H,"x",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->x,Blocks->nx,1,Ob);
	/* 9 ieme element de la tlist xd */
	if ( nsp_hash_find(H,"xd",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->xd,Blocks->nx,1,Ob);
      }
    break;
  case 6 :
    if (Blocks->nz != 0) 
      {
	/* 6ieme element de la tlist z */
	if ( nsp_hash_find(H,"z",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->z,Blocks->nz,1,Ob);
      }
    if (Blocks->nx != 0) 
      {
	/* 8ieme element de la tlist x */
	if ( nsp_hash_find(H,"x",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->x,Blocks->nx,1,Ob);
	/* 9 ieme element de la tlist xd */
	if ( nsp_hash_find(H,"xd",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->xd,Blocks->nx,1,Ob);
      }
    /* 16ieme element de la tlist y */
    if (Blocks->nout!=0) 
      {
	/* 16ieme element de la tlist y */
	if ( nsp_hash_find(H,"y",&Ob) == FAIL) goto err;
	if ( scicos_list_to_vars(Blocks->outptr,Blocks->nout,Blocks->outsz,Ob)==FAIL) goto err;
      }
    break;
  case 7 :
    if (Blocks->nx != 0)
      {
	/* 9 ieme element de la tlist xd */
	if ( nsp_hash_find(H,"xd",&Ob) == FAIL) goto err;
	scicos_scitod(Blocks->xd,Blocks->nx,1,Ob);
      }
    /* 30 ieme element de la tlist mode */
    if ( nsp_hash_find(H,"mode",&Ob) == FAIL) goto err;
    scicos_scitod(Blocks->mode,Blocks->nmode,1,Ob);
    break;
  case 9 :
    /* 24 ieme element de la tlist g */
    if ( nsp_hash_find(H,"g",&Ob) == FAIL) goto err;
    scicos_scitod(Blocks->g,Blocks->ng,1,Ob);
    /* 30 ieme element de la tlist mode */
    if ( nsp_hash_find(H,"mode",&Ob) == FAIL) goto err;
    scicos_scitod(Blocks->mode,Blocks->nmode,1,Ob);
    break;
  }
  return;
 err: 
  flag=-1;
}





void scicos_sciblk(int *flag, int *nevprt, double *t, double *xd, double *x, int *nx,
		   double *z, int *nz, double *tvec, int *ntvec, double *rpar, int *nrpar,
		   int *ipar, int *nipar, double *u, int *nu, double *y, int *ny)
{
  int mlhs= 5 , mrhs= 8;
  /*     Copyright INRIA */
  /*     routine used to evaluate a block defined by a scilab function */
  /*     scilab function syntax must be */
  /*     [y,x,z,tvec,xd]=func(flag,nevprt,t,x,z,rpar,ipar,u) */
  /*     with */
  /*        t      scalar current time */
  /*        x      column vector continuous state */
  /*        z      column vector discrete state */
  /*        u      column vector block input */
  /*        nevprt int */
  /*        flag   int */
  /*        y      column vector block output */
  /*        xd     column vector block state derivative */
  NspObject * Args[8]; 
  NspObject * Ret[5];
  /* FIXME: give names to all */
  if ((Args[0]= (NspObject *) scicos_itosci(flag,1,1)) == NULL) goto err;
  if ((Args[1]= (NspObject *) scicos_itosci(nevprt,1,1)) == NULL) goto err;
  if ((Args[2]= (NspObject *) scicos_dtosci(t,1,1)) == NULL) goto err;
  if ((Args[3]= (NspObject *) scicos_dtosci(x,*nx,1)) == NULL) goto err;
  if ((Args[4]= (NspObject *) scicos_vvtosci(z,*nz)) == NULL) goto err;
  if ((Args[5]= (NspObject *) scicos_vvtosci(rpar,*nrpar)) == NULL) goto err; 
  if ((Args[6]= (NspObject *) scicos_itosci(ipar,*nipar,1)) == NULL) goto err;
  if ((Args[8]= (NspObject *) scicos_dtosci(u,*nu,1)) == NULL) goto err;
  /*     macro execution */
  if ( scicos_scifunc(Args,mlhs,mrhs) == FAIL) goto err;
  /*     transfer output variables to fortran */
  switch (*flag) 
    {
      /*     [y,x,z,tvec,xd]=func(flag,nevprt,t,x,z,rpar,ipar,u) */
    case 1: 
      /* y or z computation */
      scicos_scitod(z, *nz, 1,Ret[2]);
      scicos_scitod(x, *nx, 1,Ret[1]);
      scicos_scitod(y, *ny, 1,Ret[0]);
      break;
    case 0:
      scicos_scitod(xd, *nx, 1,Ret[4]);
      break;
    case 2: 
      /*  x'  computation */
      scicos_scitod(z, *nz, 1,Ret[2]);
      scicos_scitod(x, *nx, 1,Ret[1]);
      break;
    case 3:
      scicos_scitod(tvec, *ntvec, 1,Ret[3]);
      break;
    case 4: 
    case 5:
      scicos_scitod(z, *nz, 1,Ret[2]);
      scicos_scitod(x, *nx, 1,Ret[1]);
      break;
    case 6: 
      scicos_scitod(z, *nz, 1,Ret[2]);
      scicos_scitod(x, *nx, 1,Ret[1]);
      scicos_scitod(y, *ny, 1,Ret[0]);
      break;
    }
  return ;
 err: 
    *flag=-1;
    return;
} 


