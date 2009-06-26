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
/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "machine.h"
#include "MALLOC.h"
#include "intcscicos.h"
#include "scoMisc.h"
#undef Top
#include "stack-c.h"
#define Top C2F(vstk).top
#include "BuildObjects.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"
#include "HandleManagement.h" /* sciGetPointerFromHandle */
#include "cvstr.h"
#include "sciprint.h"
#include "libinter.h"
#include "scicos-def.h"
#include "Scierror.h"
#include "tree.h"
#include "scicos.h"
#include "import.h" /* getscicosvarsfromimport */
#include "BOOL.h"
#include "il_state.h"
#include "il_sim.h"
/*--------------------------------------------------------------------------*/ 
extern int *listentry(int *header, int i); /* mexlib.h */
extern void str2sci(char** x,int n,int m); /* core/src/c/str2sci.h */
extern void C2F(itosci)();
extern int C2F(scierr)();
extern void C2F(vvtosci)();
extern int C2F(mktlist)(); 
extern int C2F(namstr)();
extern int C2F(funnum)(char *fname);
/*--------------------------------------------------------------------------*/ 
/* variable defined in scicos.c */
extern CURBLK_struct C2F(curblk);
extern COSIM_struct C2F(cosim);
extern COSERR_struct coserr;    /* declaration of coserr -valable partout- */
/*--------------------------------------------------------------------------*/ 
/* fonction pour recuperer le nombre du champs a partir de son nom */
int MlistGetFieldNumber(int *ptr, const char *string)
{
  int nf, longueur, istart, k, ilocal, retval;
  int *headerstr;
  static char str[24];
 
  headerstr = listentry(ptr,1);
  nf=headerstr[1]*headerstr[2]-1;  /* number of fields */
  retval=-1;
  for (k=0; k<nf; k++) {
    longueur=Min(headerstr[6+k]-headerstr[5+k],24);  /* size of kth fieldname */
    istart=5+nf+headerstr[5+k];    /* start of kth fieldname code */
    /*    istart=8+headerstr[4+nf+k]; */
    C2F(cvstr)(&longueur, &headerstr[istart], str, (ilocal=1, &ilocal),longueur);
    str[longueur]='\0';
    if (strcmp(string, str) == 0) {
      retval=k+2;
      break;}
  }
  return retval;
}
/*--------------------------------------------------------------------------*/ 
int intendscicosim(char *fname,unsigned long fname_len)
     /* termine la simulation */
{
  int isrun = C2F(cosim).isrun;

  CheckRhs(-1,0);
  if (!isrun) {
    Scierror(999,"%s: scicosim is not running.\n",fname);
  }
  else {
    end_scicos_sim();
  }
  return 0;
}
/*--------------------------------------------------------------------------*/ 

/*--------------------------------------------------------------------------*/ 
int intduplicate(char *fname, unsigned long fname_len)

     /* v=duplicate(u,count) 
      * returns v=[u(1)*ones(count(1),1);
      *            u(2)*ones(count(2),1);
      *            ...
      */
{
  int m1,m2,m3,n1,n2,n3,l1,l2,l3,n;
  CheckRhs(2,2);
  CheckLhs(1,1);
  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  n=m1*n1;
  if (n==0) {
    m3=0;
    CreateVar(3, "d", &m3, &m3, &l3);
    LhsVar(1) = 3;
    return 0;
  }
  if (n!=m2*n2) 
    {
      Scierror(999,"%s: 1st and 2nd argument must have equal size\n",fname);
      return 0;
    }
  comp_size(stk(l2),&n3,n);
  m3=1;
  CreateVar(3, "d", &n3, &m3, &l3);
  duplicata(&n,stk(l1),stk(l2),stk(l3),&n3);
  LhsVar(1) = 3;
  return 0;
}
/*--------------------------------------------------------------------------*/ 
int intdiffobjs(char *fname, unsigned long fname_len)
     /*   diffobjs(A,B) returns 0 if A==B and 1 if A and B differ */
{
  int un,l3,k;
  int size1;int size2;
  int *header1;int *header2;
  CheckRhs(2,2);
  CheckLhs(1,1);
  header1 = GetData(1);
  header2 = GetData(2);
  CreateVar(3,"d",(un=1,&un),(un=1,&un),&l3);
  LhsVar(1) = 3;
  size1=2*(*Lstk(Top-Rhs+2)-*Lstk(Top-Rhs+1));
  size2=2*(*Lstk(Top-Rhs+3)-*Lstk(Top-Rhs+2));

  if (size1 != size2) {
    *stk(l3)=1;
    return 0;
  }
  for (k=0; k<size1; k++) {
    if (header1[k] != header2[k]) {
      *stk(l3)=1;
      return 0;
    }
    *stk(l3)=0;

  }
  return 0;
}
/*--------------------------------------------------------------------------*/ 
int inttree2(char *fname, unsigned long fname_len)
     /* [ord,ok]=ctree2(vec,outoin,outoinptr,dep_u,dep_uptr) */
{
  int un=1,ipvec,nvec,mvec,noin,moin,ipoin,noinr,moinr,ipoinr;
  int ndep,mdep,ipdep,ndepuptr,mdepuptr,ipdepuptr,ipord,ipok,n,nord;

  CheckRhs(5,5);
  CheckLhs(2,2);

  GetRhsVar(1,"i",&nvec,&mvec,&ipvec);
  GetRhsVar(2,"i",&noin,&moin,&ipoin);
  GetRhsVar(3,"i",&noinr,&moinr,&ipoinr);
  GetRhsVar(4,"i",&ndep,&mdep,&ipdep);
  GetRhsVar(5,"i",&ndepuptr,&mdepuptr,&ipdepuptr);
  n=nvec*mvec;
  CreateVar(6,"i",&n,&un,&ipord);
  CreateVar(7,"i",&un,&un,&ipok);

  ctree2(istk(ipvec),n,istk(ipdep),istk(ipdepuptr),istk(ipoin),istk(ipoinr),
	 istk(ipord),&nord,istk(ipok));
  *istk(iadr(C2F(intersci).iwhere[5])+1)=nord;

  LhsVar(1)=6;
  LhsVar(2)=7;

  return 0;
}
/*--------------------------------------------------------------------------*/ 
int inttree3(char *fname, unsigned long fname_len)
     /* [r2,ok2]=ctree3(vec,dd,dep_uptr,typ_l,bexe,boptr,blnk,blptr)*/
{
  int un=1,ipvec,nvec,mvec,ntyp,mtyp,iptyp,nbex,mbex,ipbex;
  int ndep,mdep,ipdep,ndepuptr,mdepuptr,ipdepuptr,ipord,ipok,n,nord;
  int nbop,mbop,ipbop,nbln,mbln,ipbln,nblr,mblr,ipblr;
  
  CheckRhs(8,8);
  CheckLhs(2,2);

  GetRhsVar(1,"i",&nvec,&mvec,&ipvec);
  GetRhsVar(2,"i",&ndep,&mdep,&ipdep);
  GetRhsVar(3,"i",&ndepuptr,&mdepuptr,&ipdepuptr);
  GetRhsVar(4,"i",&ntyp,&mtyp,&iptyp);
  GetRhsVar(5,"i",&nbex,&mbex,&ipbex);
  GetRhsVar(6,"i",&nbop,&mbop,&ipbop);
  GetRhsVar(7,"i",&nbln,&mbln,&ipbln);
  GetRhsVar(8,"i",&nblr,&mblr,&ipblr);

  n=nvec*mvec;
  CreateVar(9,"i",&n,&un,&ipord);
  CreateVar(10,"i",&un,&un,&ipok);

  ctree3(istk(ipvec),n,istk(ipdep),istk(ipdepuptr),istk(iptyp),istk(ipbex),
	 istk(ipbop),istk(ipbln),istk(ipblr),istk(ipord),&nord,istk(ipok));
  *istk(iadr(C2F(intersci).iwhere[8])+1)=nord;

  LhsVar(1)=9;
  LhsVar(2)=10;

  return 0;
}
/*--------------------------------------------------------------------------*/ 
int inttree4(char *fname,unsigned long fname_len)
     /* [r1,r2]=ctree4(vec,outoin,outoinptr,nd,ddd) */
{
  int un=1,ipvec,nvec,mvec,noin,moin,ipoin,noinr,moinr,ipoinr;
  int nnd,mnd,ipnd,ntyp,mtyp,iptyp,ipr1,ipr2,n,nr,nn;

  CheckRhs(5,5);
  CheckLhs(2,2);

  GetRhsVar(1,"i",&nvec,&mvec,&ipvec);
  GetRhsVar(2,"i",&noin,&moin,&ipoin);
  GetRhsVar(3,"i",&noinr,&moinr,&ipoinr);
  GetRhsVar(4,"i",&nnd,&mnd,&ipnd);
  GetRhsVar(5,"i",&ntyp,&mtyp,&iptyp);
  n=nvec*mvec;
  nn=nnd*mnd;
  CreateVar(6,"i",&un,&nn,&ipr1);
  CreateVar(7,"i",&un,&nn,&ipr2);

  ctree4(istk(ipvec),n,istk(ipnd),mnd,istk(iptyp),istk(ipoin),
	 istk(ipoinr),istk(ipr1),istk(ipr2),&nr);

  LhsVar(1)=6;
  LhsVar(2)=7;
  /*      nbcols(6)=nr */
  *istk(iadr(C2F(intersci).iwhere[5])+2)=nr;
  /*      nbcols(7)=nr */
  *istk(iadr(C2F(intersci).iwhere[6])+2)=nr;
  return 0;
}
/*--------------------------------------------------------------------------*/ 
int intxproperty(char *fname,unsigned long fname_len)
     /* renvoi le type d'equation get_pointer_xproperty() 
      *	(-1: algebriques, +1 differentielles) */
{
  int un;
  int* pointer_xproperty;
  int n_pointer_xproperty;
  int isrun = C2F(cosim).isrun;

  if (!isrun) {
    Scierror(999,"%s: scicosim is not running.\n",fname);
  }
  else {
    CheckRhs(-1,0);
    CheckLhs(1,1);
	pointer_xproperty = get_pointer_xproperty();
	n_pointer_xproperty = get_npointer_xproperty();
    CreateVarFromPtr(1,"i",&n_pointer_xproperty,(un=1,&un),&pointer_xproperty);
    LhsVar(1)=1;
  }
  return 0;
}
/*--------------------------------------------------------------------------*/ 
int intphasesim(char *fname, unsigned long fname_len)
     /* renvoi la phase de simulation phase=get_phase_simulation() */
{ 
  int un,l1;
  int isrun = C2F(cosim).isrun;

  if (!isrun) {
    Scierror(999,"%s: scicosim is not running.\n",fname);
  }
  else {
    CheckRhs(-1,0);
    CheckLhs(1,1);
    CreateVar(1,"i",(un=1,&un),(un=1,&un),&l1);
    *istk(l1)=get_phase_simulation();
    LhsVar(1)=1;
  }
  return 0;
}
/*--------------------------------------------------------------------------*/ 
int intsetxproperty(char *fname, unsigned long fname_len)
     /* renvoi le type d'equation get_pointer_xproperty() 
      *	(-1: algebriques, +1 differentielles) */
{
  int un,l1,m1;
  int isrun = C2F(cosim).isrun;

  if (!isrun) {
    Scierror(999,"%s: scicosim is not running.\n",fname);
  }
  else {
    CheckRhs(1,1);
    GetRhsVar(1,"i",&m1,(un=1,&un),&l1);
    set_pointer_xproperty(istk(l1));
    LhsVar(1)=0;
  }
  return 0;
}
/*--------------------------------------------------------------------------*/ 
int intsetblockerror(char *fname, unsigned long fname_len)
     /* renvoi une erreur */
{
  int un,l1;
  int isrun = C2F(cosim).isrun;

  CheckRhs(1,1);

  if (!isrun) {
    Scierror(999,"%s: scicosim is not running.\n",fname);
  }
  else {
    GetRhsVar(1,"i",(un=1,&un),(un=1,&un),&l1);
    set_block_error(*istk(l1));
    LhsVar(1)=0;
  }
  return 0;
}
/*--------------------------------------------------------------------------*/ 
void  duplicata(int *n,double *v,double *w,double *ww,int *nw)
{
  int i,j,k;
  k=0;
  for (i=0;i<*n;i++) {
    for (j=0;j<(int) w[i];j++) {
      ww[k]=v[i];
      k=k+1;
    }
  }
  *nw=k;
}
/*--------------------------------------------------------------------------*/ 
void  comp_size(double *v,int *nw,int n)
{
  int i;
  *nw=0;
  for (i=0;i<n;i++) {
    if (v[i]>0) *nw=*nw+(int) v[i];
  }
}
/*--------------------------------------------------------------------------*/ 
/*--------------------------------------------------------------------------*/ 
