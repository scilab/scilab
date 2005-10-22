/* Copyright INRIA */
#include <string.h>

#include "../machine.h"
#include "../os_specific/sci_mem_alloc.h"  /* malloc */
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
extern void str2sci(char** x,int n,int m);
extern int *listentry(int *header, int i);

void 
sciblk4(Blocks,flag)

     scicos_block *Blocks; 
     integer flag;

{
  int k,j,i,topsave;
  double *u,*y;
  int one=1,*header,ne1,ne3,ne8;
  int nu,ny,l5,l,moinsun=-1;
  int mlhs=1,mrhs=2;
  int n27=31,zero=0;
  int *le1,*le2,ne2,*le3,*le4,ne4,ne7,*le33,*le5,*le6,ne6,*le7,*le8;
  double *le22,*le44,*le111,*le333,*le55,*le222,*le444,*le66,*le666,*le77,*le88;
  char *str[]={ "scicos_block","nevprt","funpt","type",
		"scsptr","nz","z","nx","x","xd","res","nin",
		"insz","inptr","nout","outsz","outptr","nevout",
		"evout","nrpar","rpar","nipar","ipar","ng","g",
		"ztyp","jroot","label","work","nmode","mode"};
  
  char **str1;
  

  topsave=Top;
  str2sci(str,1,31);

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
  if(Blocks[0].scsptr>0){
    C2F(vvtosci)(Blocks[0].z,&Blocks[0].nz);
    if (C2F(scierr)()!=0) goto err; 
  }  else{
    C2F(dtosci)(Blocks[0].z,&Blocks[0].nz,&one);
    if (C2F(scierr)()!=0) goto err; 
  }
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
  
  C2F(itosci)(Blocks[0].outsz,&Blocks[0].nout,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(itosci)(&Blocks[0].nout,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  for (k=0;k<Blocks[0].nout;k++) {
    y=(double *)Blocks[0].outptr[k];
    ny=Blocks[0].outsz[k];
    C2F(dtosci)(y,&ny,&one);
    if (C2F(scierr)()!=0) goto err;
  }
  C2F(mklist)(&Blocks[0].nout);
  
  C2F(itosci)(&Blocks[0].nevout,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(dtosci)(Blocks[0].evout,&Blocks[0].nevout,&one);
  if (C2F(scierr)()!=0) goto err; 
  C2F(itosci)(&Blocks[0].nrpar,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  if(Blocks[0].scsptr>0){
    C2F(vvtosci)(Blocks[0].rpar,&Blocks[0].nrpar);
    if (C2F(scierr)()!=0) goto err; 
  }  else{
    C2F(dtosci)(Blocks[0].rpar,&Blocks[0].nrpar,&one);
    if (C2F(scierr)()!=0) goto err; 
  }
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
  C2F(itosci)(Blocks[0].jroot,&Blocks[0].ng,&one);
  if (C2F(scierr)()!=0) goto err;
  
  if ((str1=MALLOC(sizeof(char*))) ==NULL )  return ;
  if ((str1[0]=MALLOC(sizeof(char)*(strlen(Blocks[0].label)+1))) ==NULL )  return ;
  (str1[0])[strlen(Blocks[0].label)]='\0';
  strncpy(str1[0],Blocks[0].label,strlen(Blocks[0].label));
  str2sci(str1,1,1);
  FREE(str1[0]);
  FREE(str1);
  if (C2F(scierr)()!=0) goto err; 
  
  C2F(vvtosci)(*Blocks[0].work,&zero);
  if (C2F(scierr)()!=0) goto err; 
  C2F(itosci)(&Blocks[0].nmode,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  C2F(itosci)(Blocks[0].mode,&Blocks[0].nmode,&one);  
  if (C2F(scierr)()!=0) goto err;

  C2F(mktlist)(&n27);
  if (C2F(scierr)()!=0) goto err; 
  
  C2F(itosci)(&flag,&one,&one);
  if (C2F(scierr)()!=0) goto err;
  

  C2F(scifunc)(&mlhs,&mrhs);

  if (C2F(scierr)()!=0) goto err;
  
  l5=Top;
  l = *Lstk(l5);
  header = (int *) stk(l);
 
  switch (flag) {
  case 1 :
    /* y computation */
    {
      /* 16ieme element de la tlist y */
      if (Blocks[0].nout!=0) {
	le3=(int*) listentry(header,17);
	for (j=1; j <=le3[1] ; j++)
	  {
	    le33=(int*) listentry(le3,j); 
	    le333=(double *) (le33+4);
	    if (le33){
	      ne3=le33[1];
	    }
	    else{ne3=0;}
	    C2F(unsfdcopy)(&ne3,le333,&moinsun,Blocks[0].outptr[j-1],&moinsun);
	  }
      }
    }
    break;
  case 0 :
    /*  x'  computation */
    /* 9 ieme element de la tlist xd */
    if (Blocks[0].nx != 0){
      le4=(int*) listentry(header,10);
      le44=(double *) listentry(header,10);
      ne4=le4[1];
      le444=((double *) (le4+4));
      C2F(unsfdcopy)(&ne4,le444,&moinsun,Blocks[0].xd,&moinsun);
      /* 10 ieme element de la tlist res */
      le6=(int*) listentry(header,11);
      le66=(double *) listentry(header,11);
      ne6=le6[1];
      le666=((double *) (le6+4));
      C2F(unsfdcopy)(&ne6,le666,&moinsun,Blocks[0].res,&moinsun);
    }
    break;
  case 2 :
    {
      /* 6ieme element de la tlist z */
      if (Blocks[0].nz != 0){
	le1=(int*) listentry(header,7);
	if(Blocks[0].scsptr>0){
	  le111=(double*) listentry(header,7);
	  ne1=header[7+2]-header[7+1];
	} else{
	  ne1=le1[1];
	  le111=((double *) (le1+4));
	}
	C2F(unsfdcopy)(&ne1,le111,&moinsun,Blocks[0].z,&moinsun);
      }
      /* 8 ieme element de la tlist x */
      if (Blocks[0].nx != 0){
	le2=(int*) listentry(header,9);
	le22=(double *) listentry(header,9);
	ne2=le2[1];
	le222=((double *) (le2+4));
	C2F(unsfdcopy)(&ne2,le222,&moinsun,Blocks[0].x,&moinsun);
	/* 9 ieme element de la tlist xd */
      	le4=(int*) listentry(header,10);
	le44=(double *) listentry(header,10);
	ne4=le4[1];
	le444=((double *) (le4+4));
	C2F(unsfdcopy)(&ne4,le444,&moinsun,Blocks[0].xd,&moinsun);
      }
      /* 30 ieme element de la tlist mode */
      le7=(int*) listentry(header,31);
      le77=(double*) (le7+4);
      ne7=le7[1];
      for (i=0; i<ne7; i++){
	Blocks[0].mode[i]=(int)le77[i];
      }   
    }
    break;
  case 3 :
    le5=(int*) listentry(header,19);
    le55=(double*) (le5+4);
    for (j=0; j<Blocks[0].nevout;j++){
      Blocks[0].evout[j]=le55[j];
    }
    break;
  case 4 :
    if (Blocks[0].nz != 0){
      le1=(int*) listentry(header,7);
      if(Blocks[0].scsptr>0){
	le111=(double*) listentry(header,7);
	ne1=header[7+2]-header[7+1];
      } else{
	ne1=le1[1];
	le111=((double *) (le1+4));
      }
      C2F(unsfdcopy)(&ne1,le111,&moinsun,Blocks[0].z,&moinsun);
    }
    /* 8ieme element de la tlist x */
    if (Blocks[0].nx != 0){
      le2=(int*) listentry(header,9);
      le22=(double *) listentry(header,9);
      ne2=le2[1];
      le222=((double *) (le2+4));
      C2F(unsfdcopy)(&ne2,le222,&moinsun,Blocks[0].x,&moinsun);
      /* 9 ieme element de la tlist xd */
      le4=(int*) listentry(header,10);
      le44=(double *) listentry(header,10);
      ne4=le4[1];
      le444=((double *) (le4+4));
      C2F(unsfdcopy)(&ne4,le444,&moinsun,Blocks[0].xd,&moinsun);
    }
    break;
  case 5 :
    if (Blocks[0].nz != 0){
      le1=(int*) listentry(header,7);
      if(Blocks[0].scsptr>0){
	le111=(double*) listentry(header,7);
	ne1=header[7+2]-header[7+1];
      } else{
	ne1=le1[1];
	le111=((double *) (le1+4));
      }
      C2F(unsfdcopy)(&ne1,le111,&moinsun,Blocks[0].z,&moinsun);
    }
    /* 8ieme element de la tlist x */
    if (Blocks[0].nx != 0){
      le2=(int*) listentry(header,9);
      le22=(double *) listentry(header,9);
      ne2=le2[1];
      le222=((double *) (le2+4));
      C2F(unsfdcopy)(&ne2,le222,&moinsun,Blocks[0].x,&moinsun);
      /* 9 ieme element de la tlist xd */
      le4=(int*) listentry(header,10);
      le44=(double *) listentry(header,10);
      ne4=le4[1];
      le444=((double *) (le4+4));
      C2F(unsfdcopy)(&ne4,le444,&moinsun,Blocks[0].xd,&moinsun);
    }
    break;
  case 6 :
    /* 6ieme element de la tlist z */
    if (Blocks[0].nz != 0){
      le1=(int*) listentry(header,7);
      if(Blocks[0].scsptr>0){
	le111=(double*) listentry(header,7);
	ne1=header[7+2]-header[7+1];
	} else{
	  ne1=le1[1];
	  le111=((double *) (le1+4));
	}
      C2F(unsfdcopy)(&ne1,le111,&moinsun,Blocks[0].z,&moinsun);
    }
    /* 8ieme element de la tlist x */
    if (Blocks[0].nx != 0){
      le2=(int*) listentry(header,9);
      le22=(double *) listentry(header,9);
      ne2=le2[1];
      le222=((double *) (le2+4));
      C2F(unsfdcopy)(&ne2,le222,&moinsun,Blocks[0].x,&moinsun);
      /* 9 ieme element de la tlist xd */
      le4=(int*) listentry(header,10);
      le44=(double *) listentry(header,10);
      ne4=le4[1];
      le444=((double *) (le4+4));
      C2F(unsfdcopy)(&ne4,le444,&moinsun,Blocks[0].xd,&moinsun);
    }
    /* 16ieme element de la tlist y */
    if (Blocks[0].nout!=0) {
      le3=(int*) listentry(header,17);
      for (j=1; j <= Blocks[0].nout  ; j++)
	{
	  le33=(int*) listentry(le3,j); 
	  le333=(double *) (le33+4);
	  if (le33){
	    ne3=le33[1];
	  }
	  else{ne3=0;}
	  C2F(unsfdcopy)(&ne3,le333,&moinsun,Blocks[0].outptr[j-1],&moinsun);
	}
    }
    
    break;
  case 7 :
    /* 9 ieme element de la tlist xd */
    if (Blocks[0].nx != 0){
      le4=(int*) listentry(header,10);
      le44=(double *) listentry(header,10);
      ne4=le4[1];
      le444=((double *) (le4+4));
      C2F(unsfdcopy)(&ne4,le444,&moinsun,Blocks[0].xd,&moinsun);
    }
    /* 30 ieme element de la tlist mode */
    le7=(int*) listentry(header,31);
    le77=(double*) (le7+4);
    ne7=le7[1];
    for (i=0; i<ne7; i++){
      Blocks[0].mode[i]=(int)le77[i];
    }     
    break;
  case 9 :
    /* 24 ieme element de la tlist g */
    le8=(int*) listentry(header,25);
    le88=(double*) (le8+4);
    ne8=le8[1];
    C2F(unsfdcopy)(&ne8,le88,&moinsun,Blocks[0].g,&moinsun);
    /* 30 ieme element de la tlist mode */
    le7=(int*) listentry(header,31);
    le77=(double*) (le7+4);
    ne7=le7[1];
    for (i=0; i<ne7; i++){
      Blocks[0].mode[i]=(int)le77[i];
    }     
    break;
  }
  /* Top=Top-1; */
  Top=topsave;
  return;
 err: 
  Top=topsave;
  set_block_error(-1);
}
