/* Copyright INRIA */
#include <string.h>
#include "../machine.h"
#include "../stack-c.h"
#include "scicos_block.h"

extern integer C2F(scierr)();
extern void C2F(itosci)();
extern void C2F(dtosci)();
extern void C2F(vvtosci)();
extern void C2F(scitovv)();
extern void C2F(skipvars)();
extern void C2F(scitod)();
extern void C2F(list2vars)();
extern void C2F(ltopadj)();
extern int C2F(mktlist)();
extern void C2F(scifunc)();
extern int C2F(mklist)();
extern void C2F(str2sci)(char** x,int n,int m);

void 
sciblk4(Blocks,flag)

scicos_block *Blocks; 
integer flag;

{
  int k,j;
  double *u;
  int one=1,*header,ne1;
  int nu,l5,l,moinsun=-1;
  int mlhs=1,mrhs=2;
  int n27=30,zero=0;
  int *le1,*le2,ne2,*le3,*le4,ne4,*le33;
  double *le22,*le44,*le111,*le333;
  char *str[]={ "scicos_block","time","nevprt","funpt","type",
		"scsptr","nz","z","nx","x","xd","res","nin",
		"insz","inptr","nout","outsz","outptr","nevout",
		"evout","nrpar","rpar","nipar","ipar","ng","g",
		"ztyp","jroot","label","work"};
  
  char **str1;
  
  str2sci(str,1,30);

  C2F(dtosci)(&Blocks[0].time,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(itosci)(&Blocks[0].nevprt,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(itosci)(&Blocks[0].funpt,&zero,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(itosci)(&Blocks[0].type,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(itosci)(&Blocks[0].scsptr,&zero,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(itosci)(&Blocks[0].nz,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(vvtosci)(Blocks[0].z,&Blocks[0].nz);
  if (C2F(scierr)()!=0) goto err; 
  C2F(itosci)(&Blocks[0].nx,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(dtosci)(Blocks[0].x,&Blocks[0].nx,&one);
  if (C2F(scierr)()!=0) goto err; 
  C2F(dtosci)(Blocks[0].xd,&Blocks[0].nx,&one);
  if (C2F(scierr)()!=0) goto err; 
  C2F(dtosci)(Blocks[0].res,&Blocks[0].nx,&one);
  if (C2F(scierr)()!=0) goto err; 
  C2F(itosci)(&Blocks[0].nin,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(itosci)(Blocks[0].insz,&Blocks[0].nin,&one);
  if (C2F(scierr)()!=0) goto err;
  for (k=0;k<Blocks[0].nin;k++) {
    u=(double *)Blocks[0].inptr[k];
    nu=Blocks[0].insz[k];
    C2F(dtosci)(u,&nu,&one);
    if (C2F(scierr)()!=0) goto err;
  }
  C2F(mklist)(&Blocks[0].nin);
  
  C2F(itosci)(&Blocks[0].nout,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(itosci)(Blocks[0].outsz,&Blocks[0].nout,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(dtosci)(*Blocks[0].outptr,&Blocks[0].nout,&one);
  if (C2F(scierr)()!=0) goto err; 
  C2F(itosci)(&Blocks[0].nevout,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(dtosci)(Blocks[0].evout,&Blocks[0].nevout,&one);
  if (C2F(scierr)()!=0) goto err; 
  C2F(itosci)(&Blocks[0].nrpar,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(vvtosci)(Blocks[0].rpar,&Blocks[0].nrpar);
  if (C2F(scierr)()!=0) goto err; 
  C2F(itosci)(&Blocks[0].nipar,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(itosci)(Blocks[0].ipar,&Blocks[0].nipar,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(itosci)(&Blocks[0].ng,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(dtosci)(Blocks[0].g,&Blocks[0].ng,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(itosci)(&Blocks[0].ztyp,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(dtosci)(Blocks[0].jroot,&Blocks[0].ng,&one);
  if (C2F(scierr)()!=0) goto err;
  
  if ((str1=malloc(sizeof(char*))) ==NULL )  return 0;
  if ((str1[0]=malloc(sizeof(char)*(strlen(Blocks[0].label)+1))) ==NULL )  return 0;
  (str1[0])[strlen(Blocks[0].label)]='\0';
  strncpy(str1[0],Blocks[0].label,strlen(Blocks[0].label));
  str2sci(str1,1,1);
  free(str1[0]);
  free(str1);
  if (C2F(scierr)()!=0) goto err; 

  C2F(vvtosci)(*Blocks[0].work,&zero);
  if (C2F(scierr)()!=0) goto err; 

  C2F(mktlist)(&n27);
  if (C2F(scierr)()!=0) goto err; 
  
  C2F(itosci)(&flag,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  
  C2F(scifunc)(&mlhs,&mrhs);
  if (C2F(scierr)()!=0) goto err;
  
  l5=Top;
  l = *lstk(l5);
  header = (int *) stk(l);
 
  switch (flag) {
  case 1 :
    /* y computation */
    {
      /* 17ieme element de la tlist y */
      if (Blocks[0].nout!=0) {
	le3=(int*) listentry(header,18);
	for (j=1; j <=le3[1] ; j++)
	  {
	    le33=(int*) listentry(le3,j); 
	    le333=(double *) (le33+4);
	    *Blocks[0].outptr[j-1]=le333[0];
	  }
      }
    }
    break;
  case 0 :
    /*  x'  computation */
    {
      /* 10 ieme element de la tlist xd */
      if (Blocks[0].nx != 0){
	le4=(int*) listentry(header,11);
	le44=(double *) listentry(header,11);
	ne4=header[11+2]-header[11+1];
	C2F(unsfdcopy)(&ne4,le44,&moinsun,Blocks[0].xd,&moinsun);
      }
      break;
    }
  case 2 :
    {
      /* 7ieme element de la tlist z */
      le1=(int*) listentry(header,8);
      le111=(double*) listentry(header,8);
      ne1=header[8+2]-header[8+1];
      C2F(unsfdcopy)(&ne1,le111,&moinsun,Blocks[0].z,&moinsun);
      /* 9ieme element de la tlist x */
      if (Blocks[0].nx != 0){
	le2=(int*) listentry(header,10);
	le22=(double *) listentry(header,10);
	ne2=header[10+2]-header[10+1];
	C2F(unsfdcopy)(&ne2,le22,&moinsun,Blocks[0].x,&moinsun);
      }
    }
    break;
  case 3 :
    /*skip=5;
      C2F(skipvars)(&skip);*/
    /*C2F(scitod)(tvec,&Blocks.nevout,&one);*/
    /*skip=2;
      C2F(skipvars)(&skip);*/
    break;
  case 4 :
    le1=(int*) listentry(header,8);
    le111=(double*) listentry(header,8);
    ne1=header[8+2]-header[8+1];
    C2F(unsfdcopy)(&ne1,le111,&moinsun,Blocks[0].z,&moinsun);
    /* 9ieme element de la tlist x */
    if (Blocks[0].nx != 0){
      le2=(int*) listentry(header,10);
      le22=(double *) listentry(header,10);
      ne2=header[10+2]-header[10+1];
      C2F(unsfdcopy)(&ne2,le22,&moinsun,Blocks[0].x,&moinsun);
    }
    break;
  case 5 :
    le1=(int*) listentry(header,8);
    le111=(double*) listentry(header,8);
    ne1=header[8+2]-header[8+1];
    C2F(unsfdcopy)(&ne1,le111,&moinsun,Blocks[0].z,&moinsun);
    /* 9ieme element de la tlist x */
    if (Blocks[0].nx != 0){
      le2=(int*) listentry(header,10);
      le22=(double *) listentry(header,10);
      ne2=header[10+2]-header[10+1];
      C2F(unsfdcopy)(&ne2,le22,&moinsun,Blocks[0].x,&moinsun);
    }
    break;
  case 6 :
    /* 7ieme element de la tlist z */
    le1=(int*) listentry(header,8);
    le111=(double*) listentry(header,8);
    ne1=header[8+2]-header[8+1];
    C2F(unsfdcopy)(&ne1,le111,&moinsun,Blocks[0].z,&moinsun);
    /* 9ieme element de la tlist x */
    if (Blocks[0].nx != 0){
      le2=(int*) listentry(header,10);
      le22=(double *) listentry(header,10);
      ne2=header[10+2]-header[10+1];
      C2F(unsfdcopy)(&ne2,le22,&moinsun,Blocks[0].x,&moinsun);
    }
    /* 17ieme element de la tlist y */
    if (Blocks[0].nout!=0) {
      le3=(int*) listentry(header,18);
      for (j=1; j <= Blocks[0].nout  ; j++)
	{
	  le33=(int*) listentry(le3,j); 
	  le333=(double *) (le33+4);
	  *Blocks[0].outptr[j-1]=le333[0];
	}
    }
    
    break;
  }
  Top=Top-1;
  return;
 err: 
  flag=-1;
}



