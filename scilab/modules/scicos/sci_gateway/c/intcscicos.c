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
#include <string.h>
#include <stdio.h>
#include <math.h>
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

/* TO BE REMOVED */
#include "BOOL.h"
#define TRUE_ TRUE
#define FALSE_ FALSE

/*to be removed*/
extern void Objstring(char *fname,
                unsigned long fname_len,
                int str,
                double x,
                double y,
                double *angle,
                double *box,
                double *wh,
                long * hdl,
                int fill,
                int *foreground,
                int *background,
                BOOL isboxed,
                BOOL isline,
                BOOL isfilled);

/* variable defined in scicos.c */
extern CURBLK_struct C2F(curblk);
struct {int isrun;} C2F(cosim); /* declaration of cosim -valable partout- */

/* TO DO new graphics function */
/*
extern void updateScaleIfRequired(sciPointObj * pSubWin);
*/
/*
 * int MlistGetFieldNumber(int *ptr, const char *string)
 * int intendscicosim(fname,fname_len)
 * int inttimescicos(fname,fname_len)
 * int intduplicate(fname,fname_len)
 * int intdiffobjs(fname,fname_len)
 * int inttree2(fname,fname_len)
 * int inttree3(fname,fname_len)
 * int inttree4 (char *fname,unsigned long fname_len)
 * int intxproperty(fname,fname_len)
 * int intphasesim(fname,fname_len)
 * int intsetxproperty(fname,fname_len)
 * int intsetblockerror(fname,fname_len)
 * void duplicata(n,v,w,ww,nw)
 * void comp_size(v,nw,n)
 * int intscicosimc(fname,fname_len)
 * int CopyVarFromlistentry(int lw, int *header, int i)
 * int var2sci(void *x,int n,int m,int typ_var)
 * int createblklist(scicos_block *Blocks, int *ierr,int flag_imp,int kfun)
 * int intgetscicosvarsc(fname,fname_len)
 * int intcurblkc(fname,fname_len)
 * int intbuildouttb(fname,fname_len)
 * int intpermutobj_c(fname,fname_len)
 */

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

int intendscicosim(fname,fname_len)
     /* termine la simulation */
     char *fname;
     unsigned long fname_len;
{
  int isrun = C2F(cosim).isrun;

  CheckRhs(-1,0);
  if (!isrun) {
    Scierror(999,"%s: scicosim is not running. \r\n",fname);
  }
  else {
    end_scicos_sim();
  }
  return 0;
}

int inttimescicos(fname,fname_len)
     /* renvoi le temps de simulation t=get_scicos_time() */
     char *fname;
     unsigned long fname_len;
{
  int un,l1;
  CheckRhs(-1,0);
  CheckLhs(1,1);
  CreateVar(1,"d",(un=1,&un),(un=1,&un),&l1);
  *stk(l1)=get_scicos_time();
  LhsVar(1)=1;
  return 0;
}

int intduplicate(fname,fname_len)

     /* v=duplicate(u,count) 
      * returns v=[u(1)*ones(count(1),1);
      *            u(2)*ones(count(2),1);
      *            ...
      */

     char *fname;
     unsigned long fname_len;
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
      Scierror(999,"%s: 1st and 2nd argument must have equal size \r\n",fname);
      return 0;
    }
  comp_size(stk(l2),&n3,n);
  m3=1;
  CreateVar(3, "d", &n3, &m3, &l3);
  duplicata(&n,stk(l1),stk(l2),stk(l3),&n3);
  LhsVar(1) = 3;
  return 0;
}

int intdiffobjs(fname,fname_len)
     /*   diffobjs(A,B) returns 0 if A==B and 1 if A and B differ */
     char *fname;
     unsigned long fname_len;
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

int inttree2(fname,fname_len)
     /* [ord,ok]=ctree2(vec,outoin,outoinptr,dep_u,dep_uptr) */
     char *fname;
     unsigned long fname_len;
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

int inttree3(fname,fname_len)
     /* [r2,ok2]=ctree3(vec,dd,dep_uptr,typ_l,bexe,boptr,blnk,blptr)*/
     char *fname;
     unsigned long fname_len;
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

int intxproperty(fname,fname_len)
     /* renvoi le type d'equation get_pointer_xproperty() 
      *	(-1: algebriques, +1 differentielles) */
     char *fname;
     unsigned long fname_len;
{
  int un;
  extern int* pointer_xproperty;
  extern int n_pointer_xproperty;
  int isrun = C2F(cosim).isrun;

  if (!isrun) {
    Scierror(999,"%s: scicosim is not running. \r\n",fname);
  }
  else {
    CheckRhs(-1,0);
    CheckLhs(1,1);
    CreateVarFromPtr(1,"i",&n_pointer_xproperty,(un=1,&un),&pointer_xproperty);
    LhsVar(1)=1;
  }
  return 0;
}

int intphasesim(fname,fname_len)
     /* renvoi la phase de simulation phase=get_phase_simulation() */
     char *fname;
     unsigned long fname_len;
{ 
  int un,l1;
  int isrun = C2F(cosim).isrun;

  if (!isrun) {
    Scierror(999,"%s: scicosim is not running. \r\n",fname);
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

int intsetxproperty(fname,fname_len)
     /* renvoi le type d'equation get_pointer_xproperty() 
      *	(-1: algebriques, +1 differentielles) */
     char *fname;
     unsigned long fname_len;
{
  int un,l1,m1;
  int isrun = C2F(cosim).isrun;

  if (!isrun) {
    Scierror(999,"%s: scicosim is not running. \r\n",fname);
  }
  else {
    CheckRhs(1,1);
    GetRhsVar(1,"i",&m1,(un=1,&un),&l1);
    set_pointer_xproperty(istk(l1));
    LhsVar(1)=0;
  }
  return 0;
}

int intsetblockerror(fname,fname_len)
     /* renvoi une erreur */
     char *fname;
     unsigned long fname_len;
{
  int un,l1;
  int isrun = C2F(cosim).isrun;

  CheckRhs(1,1);

  if (!isrun) {
    Scierror(999,"%s: scicosim is not running. \r\n",fname);
  }
  else {
    GetRhsVar(1,"i",(un=1,&un),(un=1,&un),&l1);
    set_block_error(*istk(l1));
    LhsVar(1)=0;
  }
  return 0;
}

void  duplicata(n,v,w,ww,nw)
     double *v,*w,*ww;
     int *n,*nw;
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

void  comp_size(v,nw,n)
     double *v;
     int *nw,n;
{
  int i;
  *nw=0;
  for (i=0;i<n;i++) {
    if (v[i]>0) *nw=*nw+(int) v[i];
  }
}

/* intsicosimc scicosim interface routine.
 *
 * [state,t] = scicosim(state,tcur,tf,sim,str,tol)
 *
 * rhs 1 state : Tlist
 *        - 1  : state(1)     : !xcs  x  z  oz iz  tevts  evtspt  pointi  outtb  !
 *        - 2  : state.x      : column vector of real
 *        - 3  : state.z      : column vector of real
 *        - 4  : state.oz     : list of scilab object
 *        - 5  : state.iz     : column vector of real (empty object with flag "finish")
 *        - 6  : state.tevts  : column vector of real
 *        - 7  : state.evtspt : column vector of real
 *        - 8  : state.pointi : real scalar
 *        - 9  : state.outtb  : list of scilab object
 * rhs 2 tcur  : real scalar
 * rhs 3 tf    : real scalar
 * rhs 4 sim   : Tlist
 *        - 1  : sim(1) : !scs    funs    xptr    zptr    ozptr   zcptr   inpptr
 *                         outptr inplnk  outlnk  rpar    rpptr   ipar    ipptr
 *                         opar   opptr   clkptr  ordptr  execlk  ordclk  cord
 *                         oord   zord    critev  nb      ztyp    nblk    ndcblk
 *                         subscr funtyp  iord    labels  modptr  !
 *        - 2  : sim.funs   : list of strings and/or scilab function
 *        - 3  : sim.xptr   : column vector of real
 *        - 4  : sim.zptr   : column vector of real
 *        - 5  : sim.ozptr  : column vector of real
 *        - 6  : sim.zcptr  : column vector of real
 *        - 7  : sim.inpptr : column vector of real
 *        - 8  : sim.outptr : column vector of real
 *        - 9  : sim.inplnk : column vector of real
 *        - 10 : sim.outlnk : column vector of real
 *        - 11 : sim.rpar   : column vector of real
 *        - 12 : sim.rpptr  : column vector of real
 *        - 13 : sim.ipar   : column vector of real
 *        - 14 : sim.ipptr  : column vector of real
 *        - 15 : sim.opar   : list of scilab object
 *        - 16 : sim.opptr  : column vector of real
 *        - 17 : sim.clkptr : column vector of real
 *        - 18 : sim.ordptr : column vector of real
 *        - 19 : sim.execlk : matrix of real
 *        - 20 : sim.ordclk : matrix of real
 *        - 21 : sim.cord   : matrix of real
 *        - 22 : sim.oord   : matrix of real
 *        - 23 : sim.zord   : column vector ? of real
 *        - 24 : sim.critev : column vector of real
 *        - 25 : sim.nb     : real scalar
 *        - 26 : sim.ztyp   : column vector of real
 *        - 27 : sim.nblk   : real scalar
 *        - 28 : sim.ndcblk : real scalar
 *        - 29 : sim.subscr : column vector of real
 *        - 30 : sim.funtyp : column vector of real
 *        - 31 : sim.iord   : column vector of real
 *        - 32 : sim.labels : column vector of strings
 *        - 33 : sim.modptr : column vector of real
 *
 * rhs 5 str   : string flag : 'start','run','finish','linear'
 * rhs 6 tol   : real vector of size (7,1) minimum (4,1)
 *               [atol rtol ttol [deltat realtimescale solver hmax]]'
 *
 * 16/03/06, A.Layec : Rewritten from original fortran
 *                     source code intsscicos in intcos.f.
 *
 * 29/03/06, Alan    : Improvement in accordance to c_pass2
 *                     (int32 parameters).
 *
 * 31/05/06, Alan    : Add global variable int *il_state_save
 *                     and int *il_sim_save in intcscicos.h to store
 *                     stack address of list %cpr.state and %cpr.sim
 *                     (to use with get/setscicosvars).
 *
 * 14/06/06, Alan    : Save common intersci before calling scicos
 *                     (to disable scilab crash with scifunc.f).
 *
 * 13/11/06, Alan    : Get back to double parameters for sim and state
 *                     (for better compatibility with scilab-4.x families).
 *                     Remove il_sim_save global variable.
 *
 * 15/12/06, Alan    : Warnings compilation removed.
 *                     This can crash scilab/scicos.
 *                     Please report.
 *
 * xx/02/07, Alan    : Add opar/odstate : scilab lists of arbitrary object
 *                     parameters/states.
 *
 */

#define freeparam \
     FREE(l_sim_iord);\
     FREE(l_sim_funtyp);\
     FREE(l_sim_subscr);\
     FREE(l_sim_ndcblk);\
     FREE(l_sim_nblk);\
     FREE(l_sim_ztyp);\
     FREE(l_sim_nb);\
     FREE(l_sim_critev);\
     FREE(l_sim_zord);\
     FREE(l_sim_oord);\
     FREE(l_sim_cord);\
     FREE(l_sim_ordclk);\
     FREE(l_sim_execlk);\
     FREE(l_sim_ordptr);\
     FREE(l_sim_clkptr);\
     FREE(l_sim_rpptr);\
     FREE(l_sim_ipar);\
     FREE(l_sim_ipptr);\
     FREE(l_sim_opptr);\
     FREE(l_sim_outlnk);\
     FREE(l_sim_inplnk);\
     FREE(l_sim_outptr);\
     FREE(l_sim_inpptr);\
     FREE(l_sim_zcptr);\
     FREE(l_sim_ozptr);\
     FREE(l_sim_zptr);\
     FREE(l_sim_xptr);\
     FREE(l_sim_modptr);\
     FREE(l_state_evtspt);\
     FREE(l_pointi)

/* prototype */
int intscicosimc(char *fname,unsigned long fname_len)
{
 /*********************************************
  * external structure and function declaration
  *********************************************/
 /*declaration of funnum (in scicos.c)*/
 extern int C2F(funnum)(char *fname);

 /************************************
  * variables and constants d?inition
  ************************************/
 static int id[nsiz];

 /*declaration of static structure*/
 static struct {int idb;} C2F(dbcos);   /*declaration of dbcos*/

 typedef struct inter_s_
 {
   int iwhere,nbrows,nbcols,itflag,ntypes,lad,ladc,lhsvar;
 } intersci_state ;

 typedef struct inter_l
 {
  intersci_state *state ;
  int nbvars;
 } intersci_list ;
 intersci_list *loc;
 intersci_state *new ;

 /* declaration of outtb_elem */
 outtb_el *outtb_elem=NULL;
 static int nelem;

 /*auxilary variables for dimension and address*/
 static int m1;                  /*state*/
 static int *il_state;
 static int m1e2,n1e2;           /*state.x*/
 static int *il_state_x;
 static double *l_state_x;
 static int m1e3,n1e3;           /*state.z*/
 static int *il_state_z;
 static double *l_state_z;
 static int *il_state_oz;        /*state.oz*/
 static int noz;
 static void **oz;
 static int *ozsz;
 static int *oztyp;
 static int m1e5,n1e5;           /*state.iz*/
 static int *il_state_iz;
 static void **l_state_iz;
 static int m1e6,n1e6;           /*state.tevts*/
 static int *il_state_tevts;
 static double *l_state_tevts;
 static int m1e7,n1e7;           /*state.evtspt*/
 static int *il_state_evtspt;
 static int *l_state_evtspt;
 static int m1e8,n1e8;           /*state.pointi*/
 static int *il_pointi;
 static int *l_pointi;
 static int *il_state_outtb;     /*state.outtb*/
 static int nlnk;
 static void **outtbptr;
 static int *outtbsz;
 static int *outtbtyp;

 static int m2,n2;               /*tcur*/
 static int *il_tcur;
 static double *l_tcur;

 static int m3,n3;               /*tf*/
 static int *il_tf;
 static double *l_tf;

 static int m4,n4,l4,il4;        /*sim*/
 static int *il_sim;
 static int l4e2,il4e2;          /*sim.funs*/
 static int *il_sim_fun;
 static int nblk;
 static int m_xptr,n_xptr;       /*sim.xptr*/
 static int *il_sim_xptr;
 static int *l_sim_xptr;
 static int m_zptr,n_zptr;       /*sim.zptr*/
 static int *il_sim_zptr;
 static int *l_sim_zptr;
 static int m_ozptr,n_ozptr;     /*sim.ozptr*/
 static int *il_sim_ozptr;
 static int *l_sim_ozptr;

 static int m_zcptr,n_zcptr;     /*sim.zcptr*/
 static int *il_sim_zcptr;
 static int *l_sim_zcptr;
 static int m_inpptr,n_inpptr;   /*sim.inpptr*/
 static int *il_sim_inpptr;
 static int *l_sim_inpptr;
 static int m_outptr,n_outptr;   /*sim.outptr*/
 static int *il_sim_outptr;
 static int *l_sim_outptr;
 static int m_inplnk,n_inplnk;   /*sim.inplnk*/
 static int *il_sim_inplnk;
 static int *l_sim_inplnk;
 static int m_outlnk,n_outlnk;   /*sim.outlnk*/
 static int *il_sim_outlnk;
 static int *l_sim_outlnk;
 static int m_rpar,n_rpar;       /*sim.rpar*/
 static int *il_sim_rpar;
 static double *l_sim_rpar;
 static int m_rpptr,n_rpptr;     /*sim.rpptr*/
 static int *il_sim_rpptr;
 static int *l_sim_rpptr;
 static int m_ipar,n_ipar;       /*sim.ipar*/
 static int *il_sim_ipar;
 static int *l_sim_ipar;
 static int m_ipptr,n_ipptr;     /*sim.ipptr*/
 static int *il_sim_ipptr;
 static int *l_sim_ipptr;
 static int *il_sim_opar;        /*sim.opar*/
 static int nopar;
 static void **opar;
 static int *oparsz;
 static int *opartyp;
 static int m_opptr,n_opptr;     /*sim.opptr*/
 static int *il_sim_opptr;
 static int *l_sim_opptr;

 static int m_clkptr,n_clkptr;   /*sim.clkptr*/
 static int *il_sim_clkptr;
 static int *l_sim_clkptr;
 static int m_ordptr,n_ordptr;   /*sim.ordptr*/
 static int *il_sim_ordptr;
 static int *l_sim_ordptr;
 static int m_execlk,n_execlk;   /*sim.execlk*/
 static int *il_sim_execlk;
 static int *l_sim_execlk;
 static int m_ordclk,n_ordclk;   /*sim.ordclk*/
 static int *il_sim_ordclk;
 static int *l_sim_ordclk;
 static int m_cord,n_cord;       /*sim.cord*/
 static int *il_sim_cord;
 static int *l_sim_cord;
 static int m_oord,n_oord;       /*sim.oord*/
 static int *il_sim_oord;
 static int *l_sim_oord;
 static int m_zord,n_zord;       /*sim.zord*/
 static int *il_sim_zord;
 static int *l_sim_zord;
 static int m_critev,n_critev;   /*sim.critev*/
 static int *il_sim_critev;
 static int *l_sim_critev;
 static int m_nb,n_nb;           /*sim.nb*/
 static int *il_sim_nb;
 static int *l_sim_nb;
 static int m_ztyp,n_ztyp;       /*sim.ztyp*/
 static int *il_sim_ztyp;
 static int *l_sim_ztyp;
 static int m_nblk,n_nblk;       /*sim.nblk*/
 static int *il_sim_nblk;
 static int *l_sim_nblk;
 static int m_ndcblk,n_ndcblk;   /*sim.ndcblk*/
 static int *il_sim_ndcblk;
 static int *l_sim_ndcblk;
 static int m_subscr,n_subscr;   /*sim.subscr*/
 static int *il_sim_subscr;
 static int *l_sim_subscr;
 static int m_funtyp,n_funtyp;   /*sim.funtyp*/
 static int *il_sim_funtyp;
 static int *l_sim_funtyp;
 static int m_iord,n_iord;       /*sim.iord*/
 static int *il_sim_iord;
 static int *l_sim_iord;
 static int m_lab,n_lab;         /*sim.labels*/
 static int *il_sim_lab;
 static int *il_sim_labptr;
 static int *l_sim_lab;
 static int m_modptr,n_modptr;   /*sim.modptr*/
 static int *il_sim_modptr;
 static int *l_sim_modptr;

 static int m5,n5;               /*str*/
 static int *il_str;
 static int *l_str;
 static int flag;

 static int m6,n6;               /*tol*/
 static int *il_tol;
 static double *l_tol;

 /*auxilary variables*/
 static double simpar[7];
 static int solver;
 static int *lfunpt;     /*for function table*/
 static int lf,ilf,ifun; /*for function table*/
 static int ierr,istate; /*error variable of scicos.c*/

 /*local variable*/
 int *subheader; /*pointer to get address of a subvariable in a list*/
 int i,j,k;        /*local counter variable*/
 int sz_str;     /*local variable to store size of string*/
 int err_check;  /*local variable for cross var. checking dimension*/

 /*definition of min/max input/output argument*/
 static int minlhs=1, maxlhs=2, minrhs=6, maxrhs=6;

 /*************************************
  * Check number of inputs and outputs
  *************************************/
 CheckRhs(minrhs,maxrhs);
 CheckLhs(minlhs,maxlhs);

   /****************
    * state (rhs 1)
    ****************/
    il_state = (int *) GetData(1);

    il_state_save = il_state; /* make a copy of il_state in a global variabe */

    if(il_state[0]!=16) /*Check if state is a tlist*/
    {
     Scierror(56,"%s : First argument must be a Tlist.\n",fname);
     C2F(iop).err=1;
     return 0;
    }
    m1 = il_state[1];

    /*2 : state.x      */
    il_state_x = (int *) (listentry(il_state,2));
    l_state_x  = (double *) (il_state_x+4);
    m1e2 = il_state_x[1];
    n1e2 = il_state_x[2];

    /*3 : state.z      */
    il_state_z = (int *) (listentry(il_state,3));
    l_state_z  = (double *) (il_state_z+4);
    m1e3 = il_state_z[1];
    n1e3 = il_state_z[2];

    /*4 : state.oz     */
    il_state_oz = (int *) (listentry(il_state,4));
    if(il_state_oz[0]!=15) /*check if il_state_oz is a list*/
    {
     Scierror(56,"%s : oz element of state must be a list.\n",fname);
     C2F(iop).err=4;
     return 0;
    }
    noz = il_state_oz[1];

    /*5 : state.iz     */
    il_state_iz = (int *) (listentry(il_state,5));
    l_state_iz  = (void **) (il_state_iz+4);
    m1e5 = il_state_iz[1];
    n1e5 = il_state_iz[2];

    /*6 : state.tevts  */
    il_state_tevts = (int *) (listentry(il_state,6));
    l_state_tevts  = (double *) (il_state_tevts+4);
    m1e6 = il_state_tevts[1];
    n1e6 = il_state_tevts[2];

    /*7 : state.evtspt */
    il_state_evtspt = (int *) (listentry(il_state,7));
    m1e7 = il_state_evtspt[1];
    n1e7 = il_state_evtspt[2];
    if ((m1e7*n1e7)==0) l_state_evtspt=NULL;
    else
    {
     if ((l_state_evtspt=(int *) MALLOC((m1e7*n1e7)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      return 0;
     }
     else
     {
      for(i=0;i<(m1e7*n1e7);i++) l_state_evtspt[i]= (int) ((double *)(il_state_evtspt+4))[i];
     }
    }

    /*8 : state.pointi */
    il_pointi = (int *) (listentry(il_state,8));
    m1e8 = il_pointi[1];
    n1e8 = il_pointi[2];
    if ((l_pointi=(int *) MALLOC((m1e8*n1e8)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_state_evtspt);
     return 0;
    }
    else
    {
     for(i=0;i<(m1e8*n1e8);i++) l_pointi[i]= (int) ((double *)(il_pointi+4))[i];
    }

    /*9 : state.outtb  */
    il_state_outtb = (int *) (listentry(il_state,9));
    if(il_state_outtb[0]!=15) /*check if il_state_outtb is a list*/
    {
     Scierror(56,"%s : outtb element of state must be a list.\n",fname);
     FREE(l_state_evtspt); FREE(l_pointi);
     C2F(iop).err=4;
     return 0;
    }
    nlnk = il_state_outtb[1]; /*nlnk is the dimension of the list state_louttb*/

 /***************
  * tcur (rhs 2)
  ***************/
 il_tcur = (int *) GetData(2);
 if(il_tcur[0]!=1) /*Check if tcur is a real or complex matrix*/
 {
  Scierror(53,"%s : Second argument must be a scalar.\n",fname);
  FREE(l_state_evtspt); FREE(l_pointi);
  C2F(iop).err=2;
  return 0;
 }
 l_tcur = (double *) (il_tcur+4);
 m2 = il_tcur[1];
 n2 = il_tcur[2];
 CheckScalar(2,m2,n2);
 CheckDims(2,m2,n2,1,1);

 /*************
  * tf (rhs 3)
  *************/
 il_tf = (int *) GetData(3);
 if(il_tf[0]!=1) /*Check if tf is a real or complex matrix*/
 {
  Scierror(53,"%s : Third argument must be a scalar.\n",fname);
  FREE(l_state_evtspt); FREE(l_pointi);
  C2F(iop).err=3;
  return 0;
 }
 l_tf = (double *) (il_tf+4);
 m3 = il_tf[1];
 n3 = il_tf[2];
 CheckScalar(3,m3,n3);
 CheckDims(3,m3,n3,1,1);

    /*************
    * sim (rhs 4)
    *************/
    il_sim = (int *) GetData(4);

    il_sim_save = il_sim;

    if(il_sim[0]!=16)  /*Check if sim is a tlist*/
    {
     Scierror(56,"%s : Fourth argument must be a Tlist.\n",fname);
     FREE(l_state_evtspt); FREE(l_pointi);
     C2F(iop).err=4;
     return 0;
    }
    m4 = il_sim[1];
    n4 = il_sim[2];

    /*2  : sim.funs*/
    il_sim_fun = (int *) (listentry(il_sim,2));
    if(il_sim_fun[0]!=15) /*check if sim.funs is a list*/
    {
     Scierror(56,"%s : Second element of sim must be a list.\n",fname);
     FREE(l_state_evtspt); FREE(l_pointi);
     C2F(iop).err=4;
     return 0;
    }
    nblk = il_sim_fun[1]; /*nblk is the dimension of the list sim.funs*/

    /*3  : sim.xptr   */
    il_sim_xptr = (int *) (listentry(il_sim,3));
    m_xptr = il_sim_xptr[1];
    n_xptr = il_sim_xptr[2];
    if (m_xptr*n_xptr==0) l_sim_xptr=NULL;
    else
    {
     if ((l_sim_xptr=(int *) MALLOC((m_xptr*n_xptr)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_xptr*n_xptr);i++) l_sim_xptr[i]= (int) ((double *)(il_sim_xptr+4))[i];
     }
    }

    /*4  : sim.zptr   */
    il_sim_zptr = (int *) (listentry(il_sim,4));
    m_zptr = il_sim_zptr[1];
    n_zptr = il_sim_zptr[2];
    if (m_zptr*n_zptr==0) l_sim_zptr=NULL;
    else
    {
     if ((l_sim_zptr=(int *) MALLOC((m_zptr*n_zptr)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_zptr*n_zptr);i++) l_sim_zptr[i]= (int) ((double *)(il_sim_zptr+4))[i];
     }
    }

    /*5  : sim.ozptr   */
    il_sim_ozptr = (int *) (listentry(il_sim,5));
    m_ozptr = il_sim_ozptr[1];
    n_ozptr = il_sim_ozptr[2];
    if (m_ozptr*n_ozptr==0) l_sim_ozptr=NULL;
    else
    {
     if ((l_sim_ozptr=(int *) MALLOC((m_ozptr*n_ozptr)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_ozptr*n_ozptr);i++) l_sim_ozptr[i]= (int) ((double *)(il_sim_ozptr+4))[i];
     }
    }

    /*6  : sim.zcptr  */
    il_sim_zcptr = (int *) (listentry(il_sim,6));
    m_zcptr = il_sim_zcptr[1];
    n_zcptr = il_sim_zcptr[2];
    if (m_zcptr*n_zcptr==0) l_sim_zcptr=NULL;
    else
    {
     if ((l_sim_zcptr=(int *) MALLOC((m_zcptr*n_zcptr)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_ozptr); FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_zcptr*n_zcptr);i++) l_sim_zcptr[i]= (int) ((double *)(il_sim_zcptr+4))[i];
     }
    }

    /*7  : sim.inpptr */
    il_sim_inpptr = (int *) (listentry(il_sim,7));
    m_inpptr = il_sim_inpptr[1];
    n_inpptr = il_sim_inpptr[2];
    if (m_inpptr*n_inpptr==0) l_sim_inpptr=NULL;
    else
    {
     if ((l_sim_inpptr=(int *) MALLOC((m_inpptr*n_inpptr)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_ozptr);
      FREE(l_sim_zcptr); FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_inpptr*n_inpptr);i++) l_sim_inpptr[i]= (int) ((double *)(il_sim_inpptr+4))[i];
     }
    }

    /*8  : sim.outptr */
    il_sim_outptr = (int *) (listentry(il_sim,8));
    m_outptr = il_sim_outptr[1];
    n_outptr = il_sim_outptr[2];
    if (m_outptr*n_outptr==0) l_sim_outptr=NULL;
    else
    {
     if ((l_sim_outptr=(int *) MALLOC((m_outptr*n_outptr)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_inpptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zcptr); FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_outptr*n_outptr);i++) l_sim_outptr[i]= (int) ((double *)(il_sim_outptr+4))[i];
     }
    }

    /*9  : sim.inplnk */
    il_sim_inplnk = (int *) (listentry(il_sim,9));
    m_inplnk = il_sim_inplnk[1];
    n_inplnk = il_sim_inplnk[2];
    if (m_inplnk*n_inplnk==0) l_sim_inplnk=NULL;
    else
    {
     if ((l_sim_inplnk=(int *) MALLOC((m_inplnk*n_inplnk)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_inplnk*n_inplnk);i++) l_sim_inplnk[i]= (int) ((double *)(il_sim_inplnk+4))[i];
     }
    }

    /*10  : sim.outlnk */
    il_sim_outlnk = (int *) (listentry(il_sim,10));
    m_outlnk = il_sim_outlnk[1];
    n_outlnk = il_sim_outlnk[2];
    if (m_outlnk*n_outlnk==0) l_sim_outlnk=NULL;
    else
    {
     if ((l_sim_outlnk=(int *) MALLOC((m_outlnk*n_outlnk)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_outlnk*n_outlnk);i++) l_sim_outlnk[i]= (int) ((double *)(il_sim_outlnk+4))[i];
     }
    }

    /*11 : sim.rpar   */
    il_sim_rpar = (int *) (listentry(il_sim,11));
    m_rpar = il_sim_rpar[1];
    n_rpar = il_sim_rpar[2];
    if (m_rpar*n_rpar==0) l_sim_rpar=NULL;
    else
    {
     l_sim_rpar = (double *) (il_sim_rpar+4);
    }

    /*12 : sim.rpptr  */
    il_sim_rpptr = (int *) (listentry(il_sim,12));
    m_rpptr = il_sim_rpptr[1];
    n_rpptr = il_sim_rpptr[2];
    if (m_rpptr*n_rpptr==0) l_sim_rpptr=NULL;
    else
    {
     if ((l_sim_rpptr=(int *) MALLOC((m_rpptr*n_rpptr)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_rpptr*n_rpptr);i++) l_sim_rpptr[i]= (int) ((double *)(il_sim_rpptr+4))[i];
     }
    }

    /*13 : sim.ipar   */
    il_sim_ipar = (int *) (listentry(il_sim,13));
    m_ipar = il_sim_ipar[1];
    n_ipar = il_sim_ipar[2];
    if (m_ipar*n_ipar==0) l_sim_ipar=NULL;
    else
    {
     if ((l_sim_ipar=(int *) MALLOC((m_ipar*n_ipar)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_ipar*n_ipar);i++) l_sim_ipar[i]= (int) ((double *)(il_sim_ipar+4))[i];
     }
    }

    /*14 : sim.ipptr  */
    il_sim_ipptr = (int *) (listentry(il_sim,14));
    m_ipptr = il_sim_ipptr[1];
    n_ipptr = il_sim_ipptr[2];
    if (m_ipptr*n_ipptr==0) l_sim_ipptr=NULL;
    else
    {
     if ((l_sim_ipptr=(int *) MALLOC((m_ipptr*n_ipptr)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_ipptr*n_ipptr);i++) l_sim_ipptr[i]= (int) ((double *)(il_sim_ipptr+4))[i];
     }
    }

    /*15 : sim.opar   */
    il_sim_opar = (int *) (listentry(il_sim,15));
    if(il_sim_opar[0]!=15)  /*Check if sim.opar is a list*/
    {
     Scierror(56,"%s : sim.opar must be a list.\n",fname);
     FREE(l_sim_ipptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_ozptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr);
     FREE(l_state_evtspt); FREE(l_pointi);
     C2F(iop).err=4;
     return 0;
    }
    nopar = il_sim_opar[1];

    /*16 : sim.opptr  */
    il_sim_opptr = (int *) (listentry(il_sim,16));
    m_opptr = il_sim_opptr[1];
    n_opptr = il_sim_opptr[2];
    if (m_opptr*n_opptr==0) l_sim_opptr=NULL;
    else
    {
     if ((l_sim_opptr=(int *) MALLOC((m_opptr*n_opptr)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_ipptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_opptr*n_opptr);i++) l_sim_opptr[i]= (int) ((double *)(il_sim_opptr+4))[i];
     }
    }

    /*17 : sim.clkptr */
    il_sim_clkptr = (int *) (listentry(il_sim,17));
    m_clkptr = il_sim_clkptr[1];
    n_clkptr = il_sim_clkptr[2];
    if (m_clkptr*n_clkptr==0) l_sim_clkptr=NULL;
    else
    {
     if ((l_sim_clkptr=(int *) MALLOC((m_clkptr*n_clkptr)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_clkptr);
      FREE(l_sim_opptr);
      FREE(l_sim_ipptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_clkptr*n_clkptr);i++) l_sim_clkptr[i]= (int) ((double *)(il_sim_clkptr+4))[i];
     }
    }

    /*18 : sim.ordptr */
    il_sim_ordptr = (int *) (listentry(il_sim,18));
    m_ordptr = il_sim_ordptr[1];
    n_ordptr = il_sim_ordptr[2];
    if ((l_sim_ordptr=(int *) MALLOC((m_ordptr*n_ordptr)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_clkptr);
     FREE(l_sim_opptr);
     FREE(l_sim_ipptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_ozptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr);
     FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m_ordptr*n_ordptr);i++) l_sim_ordptr[i]= (int) ((double *)(il_sim_ordptr+4))[i];
    }

    /*19 : sim.execlk */
    il_sim_execlk = (int *) (listentry(il_sim,19));
    m_execlk = il_sim_execlk[1];
    n_execlk = il_sim_execlk[2];
    if (m_execlk*n_execlk==0) l_sim_execlk=NULL;
    else
    {
     if ((l_sim_execlk=(int *) MALLOC((m_execlk*n_execlk)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_ordptr); FREE(l_sim_clkptr);
      FREE(l_sim_opptr);
      FREE(l_sim_ipptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_execlk*n_execlk);i++) l_sim_execlk[i]= (int) ((double *)(il_sim_execlk+4))[i];
     }
    }

    /*20 : sim.ordclk */
    il_sim_ordclk = (int *) (listentry(il_sim,20));
    m_ordclk = il_sim_ordclk[1];
    n_ordclk = il_sim_ordclk[2];
    if (m_ordclk*n_ordclk==0) l_sim_ordclk=NULL;
    else
    {
     if ((l_sim_ordclk=(int *) MALLOC((m_ordclk*n_ordclk)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
      FREE(l_sim_opptr);
      FREE(l_sim_ipptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_ordclk*n_ordclk);i++) l_sim_ordclk[i]= (int) ((double *)(il_sim_ordclk+4))[i];
     }
    }

    /*21 : sim.cord   */
    il_sim_cord = (int *) (listentry(il_sim,21));
    m_cord = il_sim_cord[1];
    n_cord = il_sim_cord[2];
    if (m_cord*n_cord==0) l_sim_cord=NULL;
    else
    {
     if ((l_sim_cord=(int *) MALLOC((m_cord*n_cord)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
      FREE(l_sim_opptr);
      FREE(l_sim_ipptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_cord*n_cord);i++) l_sim_cord[i]= (int) ((double *)(il_sim_cord+4))[i];
     }
    }

    /*22 : sim.oord   */
    il_sim_oord = (int *) (listentry(il_sim,22));
    m_oord = il_sim_oord[1];
    n_oord = il_sim_oord[2];
    if (m_oord*n_oord==0) l_sim_oord=NULL;
    else
    {
     if ((l_sim_oord=(int *) MALLOC((m_oord*n_oord)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_cord);
      FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
      FREE(l_sim_opptr);
      FREE(l_sim_ipptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_oord*n_oord);i++) l_sim_oord[i]= (int) ((double *)(il_sim_oord+4))[i];
     }
    }

    /*23 : sim.zord   */
    il_sim_zord = (int *) (listentry(il_sim,23));
    m_zord = il_sim_zord[1];
    n_zord = il_sim_zord[2];
    if (m_zord*n_zord==0) l_sim_zord=NULL;
    else
    {
     if ((l_sim_zord=(int *) MALLOC((m_zord*n_zord)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_oord); FREE(l_sim_cord);
      FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
      FREE(l_sim_opptr);
      FREE(l_sim_ipptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_zord*n_zord);i++) l_sim_zord[i]= (int) ((double *)(il_sim_zord+4))[i];
     }
    }

    /*24 : sim.critev */
    il_sim_critev = (int *) (listentry(il_sim,24));
    m_critev = il_sim_critev[1];
    n_critev = il_sim_critev[2];
    if (m_critev*n_critev==0) l_sim_critev=NULL;
    else
    {
     if ((l_sim_critev=(int *) MALLOC((m_critev*n_critev)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
      FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
      FREE(l_sim_opptr);
      FREE(l_sim_ipptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_critev*n_critev);i++) l_sim_critev[i]= (int) ((double *)(il_sim_critev+4))[i];
     }
    }

    /*25 : sim.nb     */
    il_sim_nb = (int *) (listentry(il_sim,25));
    m_nb = il_sim_nb[1];
    n_nb = il_sim_nb[2];
    if ((l_sim_nb=(int *) MALLOC((m_nb*n_nb)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_critev);
     FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
     FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
     FREE(l_sim_opptr);
     FREE(l_sim_ipptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_ozptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr);
     FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m_nb*n_nb);i++) l_sim_nb[i]= (int) ((double *)(il_sim_nb+4))[i];
    }
    if (l_sim_nb[0]!=nblk) /*value of nb must be equal to nblk*/
    {
     Scierror(42,"%s : Incompatible sim.nb RHS parameter.\n",fname);
     return 0;
    }

    /*26 : sim.ztyp   */
    il_sim_ztyp = (int *) (listentry(il_sim,26));
    m_ztyp = il_sim_ztyp[1];
    n_ztyp = il_sim_ztyp[2];
    if ((l_sim_ztyp=(int *) MALLOC((m_ztyp*n_ztyp)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_nb); FREE(l_sim_critev);
     FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
     FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
     FREE(l_sim_opptr);
     FREE(l_sim_ipptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_ozptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr);
     FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m_ztyp*n_ztyp);i++) l_sim_ztyp[i]= (int) ((double *)(il_sim_ztyp+4))[i];
    }

    /*27 : sim.nblk   */
    il_sim_nblk = (int *) (listentry(il_sim,27));
    m_nblk = il_sim_nblk[1];
    n_nblk = il_sim_nblk[2];
    if ((l_sim_nblk=(int *) MALLOC((m_nblk*n_nblk)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
     FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
     FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
     FREE(l_sim_opptr);
     FREE(l_sim_ipptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_ozptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr);
     FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m_nblk*n_nblk);i++) l_sim_nblk[i]= (int) ((double *)(il_sim_ztyp+4))[i];
    }

    /*28 : sim.ndcblk */
    il_sim_ndcblk = (int *) (listentry(il_sim,28));
    m_ndcblk = il_sim_ndcblk[1];
    n_ndcblk = il_sim_ndcblk[2];
    if ((l_sim_ndcblk=(int *) MALLOC((m_ndcblk*n_ndcblk)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_nblk); FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
     FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
     FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
     FREE(l_sim_opptr);
     FREE(l_sim_ipptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_ozptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr);
     FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m_ndcblk*n_ndcblk);i++) l_sim_ndcblk[i]= (int) ((double *)(il_sim_ndcblk+4))[i];
    }

    /*29 : sim.subscr */
    il_sim_subscr = (int *) (listentry(il_sim,29));
    m_subscr = il_sim_subscr[1];
    n_subscr = il_sim_subscr[2];
    if (m_subscr*n_subscr==0) l_sim_subscr=NULL;
    else
    {
     if ((l_sim_subscr=(int *) MALLOC((m_subscr*n_subscr)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_ndcblk);
      FREE(l_sim_nblk); FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
      FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
      FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
      FREE(l_sim_opptr);
      FREE(l_sim_ipptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_subscr*n_subscr);i++) l_sim_subscr[i]= (int) ((double *)(il_sim_subscr+4))[i];
     }
    }

    /*30 : sim.funtyp */
    il_sim_funtyp = (int *) (listentry(il_sim,30));
    m_funtyp = il_sim_funtyp[1];
    n_funtyp = il_sim_funtyp[2];
    if ((l_sim_funtyp=(int *) MALLOC((m_funtyp*n_funtyp)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_subscr); FREE(l_sim_ndcblk);
     FREE(l_sim_nblk); FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
     FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
     FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
     FREE(l_sim_opptr);
     FREE(l_sim_ipptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_ozptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr);
     FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m_funtyp*n_funtyp);i++) l_sim_funtyp[i]= (int) ((double *)(il_sim_funtyp+4))[i];
    }

    /*31 : sim.iord   */
    il_sim_iord = (int *) (listentry(il_sim,31));
    m_iord = il_sim_iord[1];
    n_iord = il_sim_iord[2];
    if (m_iord*n_iord==0) l_sim_iord=NULL;
    else
    {
     if ((l_sim_iord=(int *) MALLOC((m_iord*n_iord)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_funtyp); FREE(l_sim_subscr); FREE(l_sim_ndcblk);
      FREE(l_sim_nblk); FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
      FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
      FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
      FREE(l_sim_opptr);
      FREE(l_sim_ipptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_ozptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr);
      FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m_iord*n_iord);i++) l_sim_iord[i]= (int) ((double *)(il_sim_iord+4))[i];
     }
    }

    /*32 : sim.labels */
    il_sim_lab = (int *) (listentry(il_sim,32));
    m_lab = il_sim_lab[1];
    n_lab = il_sim_lab[2];
    il_sim_labptr = &il_sim_lab[4];  /*get address-1 of first pointer in labels*/
    l_sim_lab = (int *) (il_sim_lab+m_lab+5); /*get address of first string in labels*/

    /*33 : sim.modptr */
    il_sim_modptr = (int *) (listentry(il_sim,33));
    m_modptr = il_sim_modptr[1];
    n_modptr = il_sim_modptr[2];
    if ((l_sim_modptr=(int *) MALLOC((m_modptr*n_modptr)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_iord); FREE(l_sim_funtyp); FREE(l_sim_subscr); FREE(l_sim_ndcblk);
     FREE(l_sim_nblk); FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
     FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
     FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
     FREE(l_sim_opptr);
     FREE(l_sim_ipptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_ozptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr);
     FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m_modptr*n_modptr);i++) l_sim_modptr[i]= (int) ((double *)(il_sim_modptr+4))[i];
    }

 /*************
  * str (rhs 5)
  *************/
 il_str = (int *) GetData(5);
 m5 = il_str[1];
 n5 = il_str[2];
 l_str = (int *) (il_str+6);
 CheckDims(5,m5,n5,m5,1);

 sz_str=il_str[5]-1; /*store the length of str*/
 C2F(cha1).buf[0]=' ';
 C2F(cvstr)(&sz_str,&l_str[0],&C2F(cha1).buf[0],(j=1,&j),sz_str); /*codetoascii*/
 C2F(cha1).buf[sz_str]='\0';
 if (strcmp(C2F(cha1).buf,"start") == 0) flag=1;
 else if (strcmp(C2F(cha1).buf,"run") == 0) flag=2;
 else if (strcmp(C2F(cha1).buf,"finish") == 0) flag=3;
 else if (strcmp(C2F(cha1).buf,"linear") == 0) flag=4;
 else
 {
  Scierror(44,"%s : Fifth argument is incorrect.\n",fname);
  freeparam;
  C2F(iop).err=5;
  return 0;
 }

 /*************
  * tol (rhs 6)
  *************/
 il_tol = (int *) GetData(6);
 l_tol = (double *) (il_tol+4);
 m6 = il_tol[1];
 n6 = il_tol[2];
 m6 = m6*n6;
 if (m6<4) /*Check if tol has a minimun of four elements*/
 {
  Scierror(89,"%s : Sixth argument must have at least four elements.\n",fname);
  freeparam;
  C2F(iop).err=6;
  return 0;
 }
 else if(m6>7) /*Check if tol has a maximum of seven elements*/
 {
  Scierror(89,"%s : Sixth argument must have a maximum of seven elements.\n",fname);
  freeparam;
  C2F(iop).err=6;
  return 0;
 }

 /******************
  * set simpar array
  ******************/
 if (m6==4) {for(i=0;i<4;i++) simpar[i]=l_tol[i];simpar[4]=0;simpar[5]=0;simpar[6]=0;}
 else if (m6==5) {for(i=0;i<5;i++) simpar[i]=l_tol[i];simpar[5]=0;simpar[6]=0;}
 else if (m6==6) {for(i=0;i<6;i++) simpar[i]=l_tol[i];simpar[6]=0;}
 else for(i=0;i<7;i++) simpar[i]=l_tol[i];
 solver=(int)simpar[5]; /*set solver variable*/

 /******************************
  * cross variable size checking
  ******************************/
 err_check = 0;
 if (m1e6!=m1e7) err_check=1;             /*tevts vs evtspt*/
 else if (m_xptr!=m_zptr) err_check=2;    /*xptr vs zptr*/
 else if (m_xptr!=m_ozptr) err_check=3;   /*xptr vs ozptr*/
 else if (m_xptr!=m_zcptr) err_check=4;   /*xptr vs zcptr*/
 else if (m_xptr!=m_inpptr) err_check=5;  /*xptr vs inpptr*/
 else if (m_xptr!=m_outptr) err_check=6;  /*xptr vs outptr*/
 else if (m_xptr!=m_rpptr) err_check=7;   /*xptr vs rpptr*/
 else if (m_xptr!=m_ipptr) err_check=8;   /*xptr vs ipptr*/
 else if (m_xptr!=m_opptr) err_check=8;   /*xptr vs opptr*/
 else if (m_xptr!=m_clkptr) err_check=10; /*xptr vs clkptr*/
 else if ((n_ordclk!=2)&(m_ordclk!=0)) err_check=11; /*sim.ordclk*/
 else if ((n_cord!=2)&(m_cord!=0)) err_check=12;     /*sim.cord*/
 else if ((n_oord!=2)&(m_oord!=0)) err_check=13;     /*sim.oord*/
 else if ((n_zord!=2)&(m_zord!=0)) err_check=14;     /*sim.zord*/
 else if ((n_iord!=2)&(m_iord!=0)) err_check=15;     /*sim.iord*/
 if (err_check!=0)
 {
  /* please write an error table here  */
  Scierror(42,"%s : error in cross variable size checking : %d\n",\
           fname,err_check);
  freeparam;
  return 0;
 }

 /*******************************
  * set function table for blocks
  *******************************/
 il4 = iadr(*Lstk(Top-Rhs+4));  /*sim*/
 l4 = sadr(il4+m4+3);
 il4e2 = iadr(l4+*istk(il4+3)-1);
 l4e2 = sadr(il4e2+nblk+3);
 lf = l4e2;  /*first element of the list sim.funs*/
 /*define new variable lfunpt*/
 if ((lfunpt=(int *) MALLOC(nblk*sizeof(int))) ==NULL )
 {
  Scierror(999,"%s : Memory allocation error.\n",fname);
  freeparam;
  return 0;
 }

 /*for each block*/
 for (i=0;i<nblk;i++)
 {
   ilf=iadr(lf); /*should be removed later*/
   subheader=(int *)(listentry(il_sim_fun,i+1));
   /*Block is defined by a scilab function*/
   if ((subheader[0]==11)|(subheader[0]==13)) lfunpt[i]=-lf;
   /*Block is defined by a function described by a characater strings*/
   else if(subheader[0]==10)
   {
    sz_str=subheader[5]-1; /*store the length of function name*/
    C2F(cha1).buf[0]=' ';
    C2F(cvstr)(&sz_str,&subheader[6],&C2F(cha1).buf[0],(j=1,&j),sz_str); /*codetoascii*/
    C2F(cha1).buf[sz_str]='\0';
    ifun=C2F(funnum)(C2F(cha1).buf); /*search associated function number of function name*/
    /*Block is defined by a C or Fortran function*/
    if (ifun>0) lfunpt[i]=ifun;
    /*Block is defined by a predefined scilab function*/
    else 
    {
      C2F(namstr)(id,&subheader[6],&sz_str,(j=0,&j));
      C2F(com).fin=0;
      C2F(funs)(id);
      if ((C2F(com).fun==-1)|(C2F(com).fun==-2)) lfunpt[i]=-*Lstk(C2F(com).fin);
      else
      {
       C2F(curblk).kfun=i+1;
       Scierror(888,"%s : unknown block : %s\n",fname,C2F(cha1).buf);
       FREE(lfunpt);
       freeparam;
       return 0;
      }
    }
   }
   else
   {
    C2F(iop).err=4;
    Scierror(44,"%s : error\n",fname);
    FREE(lfunpt);
    freeparam;
    return 0;
   }
   lf=lf+*istk(il4e2+3+i)-*istk(il4e2+i+2);
 }

 /**********************
  * set oz, ozsz, oztyp
  **********************/
  if (noz==0)
  {
   oz=NULL;
   ozsz=NULL;
   oztyp=NULL;
  }
  else
  {
   /*Allocation of oz*/
   if ((oz=(void **) MALLOC((noz)*sizeof(void *))) ==NULL )
   {
    Scierror(999,"%s : Memory allocation error.\n",fname);
    FREE(lfunpt);
    freeparam;
    return 0;
   }
   /*Allocation of ozsz*/
   if ((ozsz=(int *) MALLOC(2*(noz)*sizeof(int))) ==NULL )
   {
    Scierror(999,"%s : Memory allocation error.\n",fname);
    FREE(oz);
    FREE(lfunpt);
    freeparam;
    return 0;
   }
   /*Allocation of oztyp*/
   if ((oztyp=(int *) MALLOC((noz)*sizeof(int))) ==NULL )
   {
    Scierror(999,"%s : Memory allocation error.\n",fname);
    FREE(oz); FREE(ozsz);
    FREE(lfunpt);
    freeparam;
    return 0;
   }

   /*set vectors of oz*/
   for(j=0;j<noz;j++)
   {
    subheader=(int *)(listentry(il_state_oz,j+1));

    switch (subheader[0]) /*store type and address*/
    {
     /*matrix of double*/
     case 1  : switch (subheader[3])
               {
                case 0  : oztyp[j]=SCSREAL_N;  /*double real matrix*/
                          ozsz[j]=subheader[1];
                          ozsz[j+noz]=subheader[2];
                          oz[j]=(SCSREAL_COP *)(subheader+4);
                          break;

                case 1  : oztyp[j]=SCSCOMPLEX_N;  /*double complex matrix*/
                          ozsz[j]=subheader[1];
                          ozsz[j+noz]=subheader[2];
                          oz[j]=(SCSCOMPLEX_COP *)(subheader+4);
                          break;

                default : oztyp[j]=SCSUNKNOW_N;
                          ozsz[j]=il_state_oz[3+j]-il_state_oz[2+j];
                          ozsz[j+noz]=1;
                          oz[j]=(SCSUNKNOW_COP *)subheader;
                          break;
               }
               break;

     /*matrix of integers*/
     case 8  : switch (subheader[3])
               {
                case 1  : oztyp[j]=SCSINT8_N;  /*int8*/
                          ozsz[j]=subheader[1];
                          ozsz[j+noz]=subheader[2];
                          oz[j]=(SCSINT8_COP *)(subheader+4);
                          break;

                case 2  : oztyp[j]=SCSINT16_N;  /*int16*/
                          ozsz[j]=subheader[1];
                          ozsz[j+noz]=subheader[2];
                          oz[j]=(SCSINT16_COP *)(subheader+4);
                          break;

                case 4  : oztyp[j]=SCSINT32_N;  /*int32*/
                          ozsz[j]=subheader[1];
                          ozsz[j+noz]=subheader[2];
                          oz[j]=(SCSINT32_COP *)(subheader+4);
                          break;

                case 11 : oztyp[j]=SCSUINT8_N; /*uint8*/
                          ozsz[j]=subheader[1];
                          ozsz[j+noz]=subheader[2];
                          oz[j]=(SCSUINT8_COP *)(subheader+4);
                          break;

                case 12 : oztyp[j]=SCSUINT16_N; /*uint16*/
                          ozsz[j]=subheader[1];
                          ozsz[j+noz]=subheader[2];
                          oz[j]=(SCSUINT16_COP *)(subheader+4);
                          break;

                case 14 : oztyp[j]=SCSUINT32_N; /*uint32*/
                          ozsz[j]=subheader[1];
                          ozsz[j+noz]=subheader[2];
                          oz[j]=(SCSUINT32_COP *)(subheader+4);
                          break;

                default : oztyp[j]=SCSUNKNOW_N;
                          ozsz[j]=il_state_oz[3+j]-il_state_oz[2+j];
                          ozsz[j+noz]=1;
                          oz[j]=(SCSUNKNOW_COP *)subheader;
                          break;
               }
               break;

     default : oztyp[j]=SCSUNKNOW_N;
               ozsz[j]=il_state_oz[3+j]-il_state_oz[2+j];
               ozsz[j+noz]=1;
               oz[j]=(SCSUNKNOW_COP *)subheader;
               break;
    }
   }
  }

 /****************************
  * set opar, oparsz, opartyp
  ****************************/
  if (nopar==0)
  {
   opar=NULL;
   oparsz=NULL;
   opartyp=NULL;
  }
  else
  {
   /*Allocation of opar*/
   if ((opar=(void **) MALLOC((nopar)*sizeof(void *))) ==NULL )
   {
    Scierror(999,"%s : Memory allocation error.\n",fname);
    FREE(oz); FREE(ozsz); FREE(oztyp);
    FREE(lfunpt);
    freeparam;
    return 0;
   }
   /*Allocation of oparsz*/
   if ((oparsz=(int *) MALLOC(2*(nopar)*sizeof(int))) ==NULL )
   {
    Scierror(999,"%s : Memory allocation error.\n",fname);
    FREE(opar);
    FREE(oz); FREE(ozsz); FREE(oztyp);
    FREE(lfunpt);
    freeparam;
    return 0;
   }
   /*Allocation of opartyp*/
   if ((opartyp=(int *) MALLOC((nopar)*sizeof(int))) ==NULL )
   {
    Scierror(999,"%s : Memory allocation error.\n",fname);
    FREE(opar); FREE(oparsz);
    FREE(oz); FREE(ozsz); FREE(oztyp);
    FREE(lfunpt);
    freeparam;
    return 0;
   }

   /*set vectors of opar*/
   for(j=0;j<nopar;j++)
   {
    subheader=(int *)(listentry(il_sim_opar,j+1));

    switch (subheader[0]) /*store type and address*/
    {
     /*matrix of double*/
     case 1  : switch (subheader[3])
               {
                case 0  : opartyp[j]=SCSREAL_N;  /*double real matrix*/
                          oparsz[j]=subheader[1];
                          oparsz[j+nopar]=subheader[2];
                          opar[j]=(SCSREAL_COP *)(subheader+4);
                          break;

                case 1  : opartyp[j]=SCSCOMPLEX_N;  /*double complex matrix*/
                          oparsz[j]=subheader[1];
                          oparsz[j+nopar]=subheader[2];
                          opar[j]=(SCSCOMPLEX_COP *)(subheader+4);
                          break;

                default : opartyp[j]=SCSUNKNOW_N;
                          oparsz[j]=il_sim_opar[3+j]-il_sim_opar[2+j];
                          oparsz[j+nopar]=1;
                          opar[j]=(SCSUNKNOW_COP *)subheader;
                          break;
               }
               break;

     /*matrix of integers*/
     case 8  : switch (subheader[3])
               {
                case 1  : opartyp[j]=SCSINT8_N;  /*int8*/
                          oparsz[j]=subheader[1];
                          oparsz[j+nopar]=subheader[2];
                          opar[j]=(SCSINT8_COP *)(subheader+4);
                          break;

                case 2  : opartyp[j]=SCSINT16_N;  /*int16*/
                          oparsz[j]=subheader[1];
                          oparsz[j+nopar]=subheader[2];
                          opar[j]=(SCSINT16_COP *)(subheader+4);
                          break;

                case 4  : opartyp[j]=SCSINT32_N;  /*int32*/
                          oparsz[j]=subheader[1];
                          oparsz[j+nopar]=subheader[2];
                          opar[j]=(SCSINT32_COP *)(subheader+4);
                          break;

                case 11 : opartyp[j]=SCSUINT8_N; /*uint8*/
                          oparsz[j]=subheader[1];
                          oparsz[j+nopar]=subheader[2];
                          opar[j]=(SCSUINT8_COP *)(subheader+4);
                          break;

                case 12 : opartyp[j]=SCSUINT16_N; /*uint16*/
                          oparsz[j]=subheader[1];
                          oparsz[j+nopar]=subheader[2];
                          opar[j]=(SCSUINT16_COP *)(subheader+4);
                          break;

                case 14 : opartyp[j]=SCSUINT32_N; /*uint32*/
                          oparsz[j]=subheader[1];
                          oparsz[j+nopar]=subheader[2];
                          opar[j]=(SCSUINT32_COP *)(subheader+4);
                          break;

                default : opartyp[j]=SCSUNKNOW_N;
                          oparsz[j]=il_sim_opar[3+j]-il_sim_opar[2+j];
                          oparsz[j+nopar]=1;
                          opar[j]=(SCSUNKNOW_COP *)subheader;
                          break;
               }
               break;

     default : opartyp[j]=SCSUNKNOW_N;
               oparsz[j]=il_sim_opar[3+j]-il_sim_opar[2+j];
               oparsz[j+nopar]=1;
               opar[j]=(SCSUNKNOW_COP *)subheader;
               break;
    }
   }
  }

 /*******************************
  * set outtbptr,outtbsz,outtbtyp
  *******************************/
 if (nlnk==0)
 {
  outtbptr=NULL;
  outtbsz=NULL;
  outtbtyp=NULL;
  outtb_elem=NULL;
  nelem=0;
 }
 else
 {
  /*Allocation of outtbptr*/
  if ((outtbptr=(void **) MALLOC(nlnk*sizeof(void *)))==NULL )
  {
   Scierror(999,"%s : Memory allocation error.\n",fname);
   FREE(opar); FREE(oparsz); FREE(opartyp);
   FREE(oz); FREE(ozsz); FREE(oztyp);
   FREE(lfunpt);
   freeparam;
   return 0;
  }
  /*Allocation of outtbsz*/
  if ((outtbsz=(int *) MALLOC(nlnk*2*sizeof(int)))==NULL )
  {
   Scierror(999,"%s : Memory allocation error.\n",fname);
   FREE(outtbptr);
   FREE(opar); FREE(oparsz); FREE(opartyp);
   FREE(oz); FREE(ozsz); FREE(oztyp);
   FREE(lfunpt);
   freeparam;
   return 0;
  }
  /*Allocation of outtbtyp*/
  if ((outtbtyp=(int *) MALLOC(nlnk*sizeof(int)))==NULL )
  {
   Scierror(999,"%s : Memory allocation error.\n",fname);
   FREE(outtbsz); FREE(outtbptr);
   FREE(opar); FREE(oparsz); FREE(opartyp);
   FREE(oz); FREE(ozsz); FREE(oztyp);
   FREE(lfunpt);
   freeparam;
   return 0;
  }

  /*initalize nelem*/
  nelem=0;

  /*set vectors of outtb*/
  for (j=0;j<nlnk;j++) /*for each link*/
  {
   subheader=(int *)(listentry(il_state_outtb,j+1)); /*get header of outtbl(j+1)*/
   outtbsz[j]=subheader[1]; /*store dimensions*/
   outtbsz[j+nlnk]=subheader[2];

   switch (subheader[0]) /*store type and address*/
   {
    /*matrix of double*/
    case 1  : switch (subheader[3])
              {
               case 0  : outtbtyp[j]=SCSREAL_N;  /*double real matrix*/
                         outtbptr[j]=(SCSREAL_COP *)(subheader+4);
                         break;

               case 1  : outtbtyp[j]=SCSCOMPLEX_N;  /*double complex matrix*/
                         outtbptr[j]=(SCSCOMPLEX_COP *)(subheader+4);
                         break;

               default : Scierror(888,\
                                 "%s : error. Type %d of double scalar matrix not yet supported "
                                 "for outtb.\n",\
                                 fname,subheader[3]);
                         FREE(outtbptr); FREE(outtbtyp); FREE(outtbsz);
                         FREE(opar); FREE(oparsz); FREE(opartyp);
                         FREE(oz); FREE(ozsz); FREE(oztyp);
                         FREE(lfunpt);
                         freeparam;
                         FREE(outtb_elem);
                         break;
              }
              break;

    /*matrix of integers*/
    case 8  : switch (subheader[3])
              {
               case 1  : outtbtyp[j]=SCSINT8_N;  /*int8*/
                         outtbptr[j]=(SCSINT8_COP *)(subheader+4);
                         break;

               case 2  : outtbtyp[j]=SCSINT16_N;  /*int16*/
                         outtbptr[j]=(SCSINT16_COP *)(subheader+4);
                         break;

               case 4  : outtbtyp[j]=SCSINT32_N;  /*int32*/
                         outtbptr[j]=(SCSINT32_COP *)(subheader+4);
                         break;

               case 11 : outtbtyp[j]=SCSUINT8_N; /*uint8*/
                         outtbptr[j]=(SCSUINT8_COP *)(subheader+4);
                         break;

               case 12 : outtbtyp[j]=SCSUINT16_N; /*uint16*/
                         outtbptr[j]=(SCSUINT16_COP *)(subheader+4);
                         break;

               case 14 : outtbtyp[j]=SCSUINT32_N; /*uint32*/
                         outtbptr[j]=(SCSUINT32_COP *)(subheader+4);
                         break;

               default : Scierror(888,\
                                 "%s : error. Type %d of int scalar matrix not yet supported "
                                 "for outtb.\n",\
                                 fname,subheader[3]);
                         FREE(outtbptr); FREE(outtbtyp); FREE(outtbsz);
                         FREE(opar); FREE(oparsz); FREE(opartyp);
                         FREE(oz); FREE(ozsz); FREE(oztyp);
                         FREE(lfunpt);
                         freeparam;
                         FREE(outtb_elem);
                         break;
              }
              break;

    default : Scierror(888,"%s : error. Type %d not yet supported for outtb.\n",fname,subheader[0]);
              FREE(outtbptr); FREE(outtbtyp); FREE(outtbsz);
              FREE(opar); FREE(oparsz); FREE(opartyp);
              FREE(oz); FREE(ozsz); FREE(oztyp);
              FREE(lfunpt);
              freeparam;
              FREE(outtb_elem);
              return 0;
              break;
   }

   /* store lnk and pos in outtb_elem */
   k=nelem;
   nelem+=outtbsz[j]*outtbsz[j+nlnk];
   if ((outtb_elem=(outtb_el *) REALLOC(outtb_elem,nelem*sizeof(outtb_el)))==NULL)
   {
    Scierror(999,"%s : No more free memory.\n",fname);
    FREE(outtbptr); FREE(outtbtyp); FREE(outtbsz);
    FREE(opar); FREE(oparsz); FREE(opartyp);
    FREE(oz); FREE(ozsz); FREE(oztyp);
    FREE(lfunpt);
    freeparam;
    FREE(outtb_elem);
    return 0;
   }
   for (i=0;i<outtbsz[j]*outtbsz[j+nlnk];i++)
   {
    outtb_elem[k+i].lnk=j;
    outtb_elem[k+i].pos=i;
   }
  }
 }

 /********************************
  * save intersci common
  * see intersci_push in stack2.c
  ********************************/
  Nbvars = Rhs; /*because of the use of getdata*/
  new = MALLOC(Rhs*sizeof(intersci_state));
  if (new == NULL)
  {
   Scierror(999,"%s : Memory allocation error.\n",fname);
   FREE(outtbptr); FREE(outtbtyp); FREE(outtbsz);
   FREE(opar); FREE(oparsz); FREE(opartyp);
   FREE(oz); FREE(ozsz); FREE(oztyp);
   FREE(lfunpt);
   FREE(outtb_elem);
   freeparam;
   return 0;
  }
  loc = MALLOC(sizeof(intersci_list));
  if (loc == NULL)
  {
   Scierror(999,"%s : Memory allocation error.\n",fname);
   FREE(outtbptr); FREE(outtbtyp); FREE(outtbsz);
   FREE(opar); FREE(oparsz); FREE(opartyp);
   FREE(oz); FREE(ozsz); FREE(oztyp);
   FREE(lfunpt);
   FREE(outtb_elem);
   FREE(new);
   freeparam;
   return 0;
  }
  loc->state = new; 
  loc->nbvars = Nbvars;
  for (i=0;i<Rhs;i++)
  {
   loc->state[i].iwhere = C2F(intersci).iwhere[i];
   loc->state[i].ntypes = C2F(intersci).ntypes[i];
   loc->state[i].lad    = C2F(intersci).lad[i];
   loc->state[i].lhsvar = C2F(intersci).lhsvar[i];
  }

 /************************
  * call scicos simulator
  ************************/
 if (C2F(iop).ddt!=0) C2F(dbcos).idb=1;   /*debug mode if ddt=0*/
 C2F(cosim).isrun=1;  /*set isrun=1 to say that we enter in the simulator*/

 /* Calling sequence :
  * int C2F(scicos)(double *x_in, int *xptr_in, double *z__,
  *                 void **work,int *zptr,int *modptr_in,
  *                 void **oz,int *ozsz,int *oztyp,int *ozptr,
  *                 int *iz,int *izptr,double *t0_in,
  *                 double *tf_in,double *tevts_in,int *evtspt_in,
  *                 int *nevts,int *pointi_in,void **outtbptr_in,
  *                 int *outtbsz_in,int *outtbtyp_in,
  *                 outtb_el *outtb_elem_in,int *nelem1,int *nlnk1,
  *                 int *funptr,int *funtyp_in,int *inpptr_in,
  *                 int *outptr_in, int *inplnk_in,int *outlnk_in,
  *                 double *rpar,int *rpptr,int *ipar,int *ipptr,
  *                 void **opar,int *oparsz,int *opartyp,int *opptr,
  *                 int *clkptr_in,int *ordptr_in,int *nordptr1,
  *                 int *ordclk_in,int *cord_in,int *ncord1,
  *                 int *iord_in,int *niord1,int *oord_in,
  *                 int *noord1,int *zord_in,int *nzord1,
  *                 int *critev_in,int *nblk1,int *ztyp,
  *                 int *zcptr_in,int *subscr,int *nsubs,
  *                 double *simpar,int *flag__,int *ierr_out)
  */

C2F(scicos)(l_state_x,l_sim_xptr,l_state_z,
            l_state_iz,l_sim_zptr,l_sim_modptr,
            oz,ozsz,oztyp,l_sim_ozptr,
            l_sim_lab,il_sim_labptr,l_tcur,
            l_tf,l_state_tevts,l_state_evtspt,
            &m1e5,l_pointi,outtbptr,
            outtbsz,outtbtyp,
            outtb_elem,&nelem,&nlnk,
            lfunpt,l_sim_funtyp,l_sim_inpptr,
            l_sim_outptr,l_sim_inplnk,l_sim_outlnk,
            l_sim_rpar,l_sim_rpptr,l_sim_ipar,l_sim_ipptr,
            opar,oparsz,opartyp,l_sim_opptr,
            l_sim_clkptr,l_sim_ordptr,&m_ordptr,
            l_sim_ordclk,l_sim_cord,&m_cord,
            l_sim_iord,&m_iord,l_sim_oord,
            &m_oord,l_sim_zord, &m_zord,
            l_sim_critev,&nblk,l_sim_ztyp,
            l_sim_zcptr,l_sim_subscr,&m_subscr,
            simpar,&flag,&ierr);

 C2F(dbcos).idb=0;  /*return in normal mode*/
 C2F(cosim).isrun=0;  /*return in normal mode*/

 /******************************
  * retrieve intersci common
  * see intersci_pop in stack2.c
  ******************************/
 Nbvars = loc->nbvars;
 for (i=0;i<Rhs;i++)
 {
  C2F(intersci).iwhere[i] = loc->state[i].iwhere ;
  C2F(intersci).ntypes[i] = loc->state[i].ntypes ;
  C2F(intersci).lad[i]    = loc->state[i].lad    ;
  C2F(intersci).lhsvar[i] = loc->state[i].lhsvar ;
 }
 FREE(loc->state);
 FREE(loc);

 /**********************
  * Free allocated array
  **********************/
 FREE(outtbptr); FREE(outtbtyp); FREE(outtbsz);
 FREE(opar); FREE(oparsz); FREE(opartyp);
 FREE(oz); FREE(ozsz); FREE(oztyp);
 FREE(lfunpt);
 FREE(outtb_elem);

 /*************************************
  * switch to appropriate message error
  *************************************/
 if (ierr>0)
 {
  switch (ierr)
  {
   case 1  : strcpy(C2F(cha1).buf,"scheduling problem");
             C2F(curblk).kfun=0;
             break;

   case 2  : strcpy(C2F(cha1).buf,"input to zero-crossing stuck on zero");
             C2F(curblk).kfun=0;
             break;

   case 3  : strcpy(C2F(cha1).buf,"event conflict");
             C2F(curblk).kfun=0;
             break;

   case 4  : strcpy(C2F(cha1).buf,"algrebraic loop detected");
             C2F(curblk).kfun=0;
             break;

   case 5  : strcpy(C2F(cha1).buf,"cannot allocate memory");
             C2F(curblk).kfun=0;
             break;

   case 6  : sprintf(C2F(cha1).buf,"the block %d has been called with input out of its domain",C2F(curblk).kfun);
             break;

   case 7  : strcpy(C2F(cha1).buf,"singularity in a block");
             break;

   case 8  : strcpy(C2F(cha1).buf,"block produces an internal error");
             break;

   case 20  : strcpy(C2F(cha1).buf,"initial conditions not converging");
              break;

   case 21  : sprintf(C2F(cha1).buf, "cannot allocate memory in block=%d",C2F(curblk).kfun);
              break;

   case 22  : strcpy(C2F(cha1).buf,"sliding mode condition, cannot integrate");
               break;

   case 23  : strcpy(C2F(cha1).buf,"Cannot find the initial mode, maybe there is a sliding mode condition");
               break;

	      /* IDA error messages*/
   case 201  : strcpy(C2F(cha1).buf,"IDA_MEM_NULL:The argument ida_mem was NULL");
               break;

   case 202  : strcpy(C2F(cha1).buf,"IDA_ILL_INPUT: One of the input arguments was illegal. This errors message may be returned if the linear solver function initialization (called by the user after calling IDACreate) failed to set the linear solver-specific lsolve field in ida_mem.");
               break;

   case 203  : strcpy(C2F(cha1).buf,"IDA_NO_MALLOC: indicating that ida_mem was not allocated.");
               break;
  
   case 204  : strcpy(C2F(cha1).buf,"IDA_TOO_MUCH_WORK: The solver took mxstep internal steps but could not reach tout. The default value for mxstep is MXSTEP_DEFAULT = 500.");
               break;
   
   case 205  : strcpy(C2F(cha1).buf,"IDA_TOO_MUCH_ACC: The solver could not satisfy the accuracy demanded by the user for some internal step.");
               break;

   case 206  : strcpy(C2F(cha1).buf,"IDA_ERR_FAIL: Error test failures occurred too many times (=MXETF = 10) during one internal step or occurred with |h|=h_min");
               break;

   case 207  : strcpy(C2F(cha1).buf,"IDA_CONV_FAIL: IDACalcIC->Failed to get convergence of the Newton iterations. or IDA_solve->Convergence test failures occurred too many times (= MXNCF = 10) during one internal step. ");
               break;
 
   case 208  : strcpy(C2F(cha1).buf,"IDA_LINIT_FAIL: The linear solver''s initialization routine failed.");
               break;
 
   case 209  : strcpy(C2F(cha1).buf,"IDA_LSETUP_FAIL: The linear solver''s setup routine had a non-recoverable error.");
               break;

   case 210  : strcpy(C2F(cha1).buf,"IDA_LSOLVE_FAIL: The linear solver''s solve routine had a non-recoverable error.");
               break;
  
   case 211  : strcpy(C2F(cha1).buf,"IDA_RES_FAIL: The user''s residual routine returned a non-recoverable error flag.");
               break;
 
   case 212  : strcpy(C2F(cha1).buf,"IDA_CONSTR_FAIL: IDACalcIC was unable to find a solution satisfying the inequality constraints.");
               break;
 
   case 213  : strcpy(C2F(cha1).buf,"IDA_REP_RES_ERR: The user''s residual function repeatedly returned a recoverable error flag, but the solver was unable to recover.");
               break;

   case 214  : strcpy(C2F(cha1).buf,"IDA_MEM_FAIL: A memory allocation failed.");
               break;

   case 215  : strcpy(C2F(cha1).buf,"IDA_BAD_T: t is not in the interval [tn-hu,tn].");
               break;
  
   case 216  : strcpy(C2F(cha1).buf,"IDA_BAD_EWT: Some component of the error weight vector is zero (illegal), either for the input value of y0 or a corrected value.");
               break;

   case 217  : strcpy(C2F(cha1).buf,"IDA_FIRST_RES_FAIL: The user''s residual routine returned  a recoverable error flag on the first call, but IDACalcIC was unable to recover. .");
               break;
  
   case 218  : strcpy(C2F(cha1).buf,"IDA_LINESEARCH_FAIL: The Linesearch algorithm failed to find a solution with a step larger than steptol in weighted RMS norm.");
               break;

   case 219  : strcpy(C2F(cha1).buf,"IDA_NO_RECOVERY: The user''s residual routine, or the linear solver''s setup or solve routine had a  recoverable error, but IDACalcIC was unable to recover.");
               break;

   case 220  : strcpy(C2F(cha1).buf,"IDA_RTFUNC_FAIL: The root founding function failed.");
               break;

   case 228  : strcpy(C2F(cha1).buf,"IDA_YOUT_NULL: ''yout'' = NULL illegal.");
               break;
   case 229  : strcpy(C2F(cha1).buf,"IDA_TRET_NULL: ''tret'' = NULL illegal.");
               break;
   case 230  : strcpy(C2F(cha1).buf,"IDA_BAD_ITASK: Illegal value for itask.");
               break;
   case 231  : strcpy(C2F(cha1).buf,"IDA_NO_ESTOP: itask = IDA_NORMAL_TSTOP or itask = IDA_ONE_STEP_TSTOP but tstop was not set");
               break;
   case 232  : strcpy(C2F(cha1).buf,"IDA_BAD_H0: h0 and tout - t0 inconsistent.");
               break;
   case 233  : strcpy(C2F(cha1).buf,"IDA_BAD_TSTOP: tstop is behind current simulation time in the direction of integration.");
               break;
   case 234  : strcpy(C2F(cha1).buf,"IDA_BAD_INIT_ROOT: Root found at and very near initial t.");
               break;
   case 235  : strcpy(C2F(cha1).buf,"IDA_NO_EFUN: itol = IDA_WF but no EwtSet function was provided.");
               break;
   case 236  : strcpy(C2F(cha1).buf,"IDA_EWT_FAIL: The user-provide EwtSet function failed.");
               break;
   case 237  : strcpy(C2F(cha1).buf,"-----------------:)-----------"); 
               break;
   case 238  : strcpy(C2F(cha1).buf,"IDA_LSOLVE_NULL: The linear solver''s solve routine is NULL.");
               break;
   case 239  : strcpy(C2F(cha1).buf,"IDA_NULL_Y0: y0 = NULL illegal.");
               break;
   case 240  : strcpy(C2F(cha1).buf,"IDA_BAD_ITOL:Illegal value for itol. The legal values are IDA_SS, IDA_SV, and IDA_WF.");
               break;
   case 241  : strcpy(C2F(cha1).buf,"IDA_NULL_F: user supplied ODE routine is (NULL) illegal.");
               break;
   case 242  : strcpy(C2F(cha1).buf,"IDA_BAD_NVECTOR: A required vector operation is not implemented.");
               break;
   case 243  : strcpy(C2F(cha1).buf,"IDA_NULL_ABSTOL: absolute tolerances is = NULL illegal.");
               break;
   case 244  : strcpy(C2F(cha1).buf,"IDA_BAD_RELTOL: relative tolerances is reltol < 0 illegal.");
               break;
   case 245  : strcpy(C2F(cha1).buf,"IDA_BAD_ABSTOL: abstol has negative component(s) (illegal).");
               break;
   case 246  : strcpy(C2F(cha1).buf,"IDA_NULL_G: user supplied zero-crossing routine is (NULL) illegal..");
               break;
   case 247  : strcpy(C2F(cha1).buf,"IDA_BAD_TOUT: Trouble interpolating. ''tout'' too far back in direction of integration");
               break;
   case 248  : strcpy(C2F(cha1).buf,"IDA_YP0_NULL: the derivative yp0 = NULL is illegal.");
               break;
   case 249  : strcpy(C2F(cha1).buf,"IDA_RES_NULL: th returned residual res = NULL is illegal.");
               break;
   case 250  : strcpy(C2F(cha1).buf,"IDA_YRET_NULL: yret = NULL illegal..");
               break;
   case 251  : strcpy(C2F(cha1).buf,"IDA_YPRET_NULL: yret = NULL illegal..");
               break;
   case 252  : strcpy(C2F(cha1).buf,"IDA_BAD_HINIT: yret = NULL illegal..");
               break;
   case 253  : strcpy(C2F(cha1).buf,"IDA_MISSING_ID :id = NULL (''id'' defines algebraic and differential states) but suppressalg option on.");
               break;
   case 254  : strcpy(C2F(cha1).buf,"IDA_Y0_FAIL_CONSTR: y0 fails to satisfy constraints.");
               break;
   case 255  : strcpy(C2F(cha1).buf,"IDA_TOO_CLOSE: ''tout'' too close to ''t0'' to start integration.");
               break;
   case 256  : strcpy(C2F(cha1).buf,"IDA_CLOSE_ROOTS: Root found at and very near starting time.");
               break;

	      /* CVODE error messages*/
   case 301  : strcpy(C2F(cha1).buf,"CV_TOO_MUCH_WORK: The solver took mxstep internal steps but could not reach ''tout''. The default value for mxstep is MXSTEP_DEFAULT = 500.");
               break;

   case 302  : strcpy(C2F(cha1).buf,"CV_TOO_MUCH_ACC: The solver could not satisfy the accuracy demanded by the user for some internal step");
               break;
 
   case 303  : strcpy(C2F(cha1).buf,"CV_ERR_FAILURE: Error test failures occurred too many times (=MXETF = 7) during one internal step or occurred with |h|=h_min ");
               break;

   case 304  : strcpy(C2F(cha1).buf,"CV_CONV_FAILURE: Convergence test failures occurred too many times (= MXNCF = 10) during one internal time step or occurred with |h| = hmin.");
               break;

   case 305  : strcpy(C2F(cha1).buf,"CV_LINIT_FAIL: The linear solver''s initialization function failed.");
               break;
  
   case 306  : strcpy(C2F(cha1).buf,"CV_LSETUP_FAIL: The linear solver''s setup routine failed in an unrecoverable manner.");
               break;
 
   case 307  : strcpy(C2F(cha1).buf,"CV_LSOLVE_FAIL: The linear solver''s solve routine failed in an unrecoverable manner.");
               break;
  
   case 308  : strcpy(C2F(cha1).buf,"CV_RHSFUNC_FAIL: The right-hand side function (user supplied ODE) failed in an unrecoverable manner");
               break;

   case 309  : strcpy(C2F(cha1).buf,"CV_FIRST_RHSFUNC_ERR: The right-hand side function (user supplied ODE) had a recoverable error at th efirst call");
               break;
 
   case 310  : strcpy(C2F(cha1).buf,"CV_REPTD_RHSFUNC_ERR: Convergence tests occurred too many times due to repeated recoverable errors in the right-hand side function (user supplied ODE). This error may be raised due to repeated  recoverable errors during the estimation of an initial step size.");
               break;
  
   case 311  : strcpy(C2F(cha1).buf,"CV_UNREC_RHSFUNC_ERR: The right-hand side function (user supplied ODE) had a recoverable error, but no recovery was possible.");
               break;
   case 312  : strcpy(C2F(cha1).buf,"CV_RTFUNC_FAIL: The rootfinding routine failed in an unrecoverable manner.");
               break;
   case 320  : strcpy(C2F(cha1).buf,"CV_MEM_FAIL: a memory allocation failed, including an attempt to increase maxord");
               break;
   case 321  : strcpy(C2F(cha1).buf,"CV_MEM_NULL: the cvode memory was NULL");
               break;
   case 322  : strcpy(C2F(cha1).buf,"CV_ILL_INPUT: indicating an input argument was illegal. This include the situation where a component of the error weight vector becomes negative during internal time-stepping. This also includes if the linear solver function initialization (called by the user after calling CVodeCreat) failed to set the linear solver-specific ''lsolve'' field in cvode_mem. This error happens if number of root functions is positive but the return surface value is NULL. ");
               break;
   case 323  : strcpy(C2F(cha1).buf,"CV_NO_MALLOC: indicating that cvode_mem has not been allocated (i.e., CVodeMalloc has not been called).");
               break;
   case 324  : strcpy(C2F(cha1).buf,"CV_BAD_K: k (the order of the derivative of y to be computed) is not in the range 0, 1, ..., qu, where qu is the order last used");
               break;
   case 325  : strcpy(C2F(cha1).buf,"CV_BAD_T: t is not in the interval [tn-hu,tn].");
               break;
   case 326  : strcpy(C2F(cha1).buf,"CV_BAD_DKY:  The dky argument was NULL. dky is the output derivative vector [((d/dy)^k)y](t).");
               break;
   case 327  : strcpy(C2F(cha1).buf,"CV_TOO_CLOSE: ''tout'' too close to ''t0'' to start integration.");
               break;
   case 328  : strcpy(C2F(cha1).buf,"CV_YOUT_NULL: ''yout'' = NULL illegal.");
               break;
   case 329  : strcpy(C2F(cha1).buf,"CV_TRET_NULL: ''tret'' = NULL illegal.");
               break;
   case 330  : strcpy(C2F(cha1).buf,"CV_BAD_ITASK: Illegal value for itask.");
               break;
   case 331  : strcpy(C2F(cha1).buf,"CV_NO_ESTOP: itask = CV_NORMAL_TSTOP or itask = CV_ONE_STEP_TSTOP but tstop was not set");
               break;
   case 332  : strcpy(C2F(cha1).buf,"CV_BAD_H0: h0 and tout - t0 inconsistent.");
               break;
   case 333  : strcpy(C2F(cha1).buf,"CV_BAD_TSTOP: tstop is behind current simulation time in the direction of integration.");
               break;
   case 334  : strcpy(C2F(cha1).buf,"CV_BAD_INIT_ROOT: Root found at and very near initial t.");
               break;
   case 335  : strcpy(C2F(cha1).buf,"CV_NO_EFUN: itol = CV_WF but no EwtSet function was provided.");
               break;
   case 336  : strcpy(C2F(cha1).buf,"CV_EWT_FAIL: The user-provide EwtSet function failed.");
               break;
   case 337  : strcpy(C2F(cha1).buf,"CV_BAD_EWT: Initial ewt has component(s) equal to zero (illegal).");
               break;
   case 338  : strcpy(C2F(cha1).buf,"CV_LSOLVE_NULL: The linear solver''s solve routine is NULL.");
               break;
   case 339  : strcpy(C2F(cha1).buf,"CV_NULL_Y0: y0 = NULL illegal.");
               break;
   case 340  : strcpy(C2F(cha1).buf,"CV_BAD_ITOL:Illegal value for itol. The legal values are CV_SS, CV_SV, and CV_WF.");
               break;
   case 341  : strcpy(C2F(cha1).buf,"CV_NULL_F: user supplied ODE routine is (NULL) illegal.");
               break;
   case 342  : strcpy(C2F(cha1).buf,"CV_BAD_NVECTOR: A required vector operation is not implemented.");
               break;
   case 343  : strcpy(C2F(cha1).buf,"CV_NULL_ABSTOL: absolute tolerances is = NULL illegal.");
               break;
   case 344  : strcpy(C2F(cha1).buf,"CV_BAD_RELTOL: relative tolerances is reltol < 0 illegal.");
               break;
   case 345  : strcpy(C2F(cha1).buf,"CV_BAD_ABSTOL: abstol has negative component(s) (illegal).");
               break;
   case 346  : strcpy(C2F(cha1).buf,"CV_NULL_G: user supplied zero-crossing routine is (NULL) illegal..");
               break;
   case 347  : strcpy(C2F(cha1).buf,"CV_BAD_TOUT: Trouble interpolating. ''tout'' too far back in direction of integration");
               break;
   case 348  : strcpy(C2F(cha1).buf,"CV_CLOSE_ROOTS: Root found at and very near starting time.");
               break;

  default  : if(ierr>=1000)
                strcpy(C2F(cha1).buf,"unknown or erroneous block");
              else if (ierr>=100)
              {
               istate=-(ierr-100);
               sprintf(C2F(cha1).buf, "integration problem istate=%d",istate);
               C2F(curblk).kfun=0;
              }
              else
              {
               strcpy(C2F(cha1).buf,"scicos unexpected error,please report...");
               C2F(curblk).kfun=0;
              }
              break;
  }
  if (! (C2F(errgst).err1>0||C2F(iop).err>0))
  {
   Scierror(888,"%s\n",C2F(cha1).buf);
   /*C2F(curblk).kfun=0;*/
   C2F(com).fun=0; /*set common fun=0 (this disable bug in debug mode)*/
   freeparam;
   return 0;
  }
 }

 if (C2F(iop).err>0) 
 {
  freeparam;
  return 0;
 }

 /*C2F(curblk).kfun=0;*/
 C2F(com).fun=0;

 /*********************
  * return Lsh variable
  *********************/
 /*copy int parameters of state in double parameters*/
 for(i=0;i<(m1e7*n1e7);i++) ((double *)(il_state_evtspt+4))[i] = (double) l_state_evtspt[i];
 for(i=0;i<(m1e8*n1e8);i++) ((double *)(il_pointi+4))[i] = (double) l_pointi[i];
 /*set lsh var*/
 if (Lhs>=1) LhsVar(1) = 1; /*return state in LhsVar(1)*/
 if (Lhs==2) LhsVar(2) = 2; /*return tcur in LhsVar(2)*/

 /* end */
 freeparam;
 return 0;
 }

/*-----------------------------------------------------------------
 * CopyVarFromlistentry
 *    Copy a Scilab object in a list to the variable position  lw
 *
 * Calling sequence :
 *  int CopyVarFromlistentry(int lw, int *header, int i)
 *
 * Input parameters : lw : integer, the free position
 *                    header : int pointer, a pointer of a list.
 *                    i : integer, give the number of the element to copy
 *
 * Output : FALSE if failed, TRUE else.
 *
 * Examples of use 
 *
 * 1 -  put the third element of a list given in position lw=1 
 *      to position lw=2 :
 *
 *  int *il_list;
 *  il_list = (int *) Getdata(1);
 *  CopyVarFromlistentry(2, il_list, 3)
 *
 * 2 - put the second element of a list stored in the fourth element 
 *     of a list in position lw=1 to position lw=3 :
 *
 *  int *il_list;
 *  il_list = (int *) Getdata(1);
 *  int *il_sublist;
 *  il_sublist = (int *) listentry(il_list,4);
 *  CopyVarFromlistentry(3, il_sublist, 2)
 *----------------------------------------------------------------*/

int CopyVarFromlistentry(int lw, int *header, int i)
{
   /* Local variable definition*/
   int ret,un=1;
   double *l;
   int n;

   /* Test if we receive a NULL ptr header */
   if (header==NULL) return FALSE_;

   /* Get the start address of the i element of the input list*/
   if ((l = (double *) listentry(header,i))==NULL) return FALSE_;

   /* Compute the length of the i element in double word */
   n = header[i+2]-header[i+1];

   /* Create empty data of a size n*sizeof(double) at the position lw */
   if ((ret=C2F(createdata)(&lw, n*sizeof(double)))==FALSE_) return ret;

   /* Copy the element i to position lw*/
   C2F(unsfdcopy)(&n,l,&un,stk(*Lstk(lw + Top - Rhs)),&un);
   return TRUE_;
}

/* var2sci function to convert an array of scicos
 * blocks to scilab object in the Top+1 position
 * in the stack.
 *
 * Input parameters :
 * *x       : void ptr, scicos blocks array to store
 *            in the Top+1 position in the stack.
 * n        : integer, number of rows.
 * m        : integer, number of columns.
 * typ_var  : integer, type of scicos data :
 *            SCSREAL    : double real
 *            SCSCOMPLEX : double complex
 *            SCSINT     : int
 *            SCSINT8    : int8
 *            SCSINT16   : int16
 *            SCSINT32   : int32
 *            SCSUINT    : uint
 *            SCSUINT8   : uint8
 *            SCSUINT16  : uint16
 *            SCSUINT32  : uint32
 *            SCSUNKNOW  : Unknow type
 *
 * Output parameters : int (<1000), error flag
 *                     (0 if no error)
 *
 * 07/06/06, Alan    : initial version.
 *
 * 23/06/06, Alan    : moved in intcscicos.c to do
 *                     the connection with getscicosvars("blocks")
 *
 * 09/02/07, Alan    : add unknown type of objects
 *
 */

/* prototype */
int var2sci(void *x,int n,int m,int typ_var)
{
  /************************************
   * variables and constants d?inition
   ************************************/
  /*counter and address variable declaration*/
  int nm,il,l,lw,j,i,err;

  /*define all type of accepted ptr */
  SCSREAL_COP *x_d,*ptr_d;
  SCSINT8_COP *x_c,*ptr_c;
  SCSUINT8_COP *x_uc,*ptr_uc;
  SCSINT16_COP *x_s,*ptr_s;
  SCSUINT16_COP *x_us,*ptr_us;
  SCSINT_COP *x_i,*ptr_i;
  SCSUINT_COP *x_ui,*ptr_ui;
  SCSINT32_COP *x_l,*ptr_l;
  SCSUINT32_COP *x_ul,*ptr_ul;

  /* Check if the stack is not full */
  if (Top >= Bot)
  {
   err = 1;
   return err;
  }
  else
  {
   Top = Top + 1;
   il = iadr(*Lstk(Top));
   l = sadr(il+4);
  }

  /* set number of double needed to store data */
  if (typ_var==SCSREAL_N)         nm=n*m;   /*double real matrix*/
  else if (typ_var==SCSCOMPLEX_N) nm=n*m*2; /*double real matrix*/
  else if (typ_var==SCSINT_N)     nm=(int)(ceil((n*m)/2)+1); /*int*/
  else if (typ_var==SCSINT8_N)    nm=(int)(ceil((n*m)/8)+1); /*int8*/
  else if (typ_var==SCSINT16_N)   nm=(int)(ceil((n*m)/4)+1); /*int16*/
  else if (typ_var==SCSINT32_N)   nm=(int)(ceil((n*m)/2)+1); /*int32*/
  else if (typ_var==SCSUINT_N)    nm=(int)(ceil((n*m)/2)+1); /*uint*/
  else if (typ_var==SCSUINT8_N)   nm=(int)(ceil((n*m)/8)+1); /*uint8*/
  else if (typ_var==SCSUINT16_N)  nm=(int)(ceil((n*m)/4)+1); /*uint16*/
  else if (typ_var==SCSUINT32_N)  nm=(int)(ceil((n*m)/2)+1); /*uint32*/
  else if (typ_var==SCSUNKNOW_N)  nm=n*m; /*arbitrary scilab object*/
  else nm=n*m; /*double real matrix*/

  /*check if there is free space for new data*/
  err = l + nm - *Lstk(Bot);
  if (err > 0)
  {
   err = 2;
   return err;
  }

  /**************************
   * store data on the stack
   *************************/
  switch (typ_var) /*for each type of data*/
  {
   case SCSREAL_N    : /* set header */
                     *istk(il) = 1; /*double real matrix*/
                     *istk(il+1) = n;
                     *istk(il+2) = m;
                     *istk(il+3) = 0;
                     x_d = (SCSREAL_COP *) x;
                     ptr_d = (SCSREAL_COP *) stk(l);
                     for (j=0;j<m*n;j++) ptr_d[j] = x_d[j];
                     break;

   case SCSCOMPLEX_N : /* set header */
                     *istk(il) = 1; /*double complex matrix*/
                     *istk(il+1) = n;
                     *istk(il+2) = m;
                     *istk(il+3) = 1;
                     x_d = (SCSCOMPLEX_COP *) x;
                     ptr_d = (SCSCOMPLEX_COP *) stk(l);
                     for (j=0;j<2*m*n;j++) ptr_d[j] = x_d[j];
                     break;

   case SCSINT_N     : /* set header */
                     *istk(il) = 8; /*int*/
                     *istk(il+1) = n;
                     *istk(il+2) = m;
                     *istk(il+3) = 4;
                     x_i = (SCSINT_COP *) x;
                     for (j=0;j<m*n;j++)
                     {
                      ptr_i = (SCSINT_COP *) istk(il+4);
                      ptr_i[j] = x_i[j];
                     }
                     break;

   case SCSINT8_N    : /* set header */
                     *istk(il) = 8; /*int8*/
                     *istk(il+1) = n;
                     *istk(il+2) = m;
                     *istk(il+3) = 1;
                     x_c = (SCSINT8_COP *) x;
                     for (j=0;j<m*n;j++)
                     {
                      ptr_c = (SCSINT8_COP *) istk(il+4);
                      ptr_c[j] = x_c[j];
                     }
                     break;

   case SCSINT16_N   : /* set header */
                     *istk(il) = 8; /*int16*/
                     *istk(il+1) = n;
                     *istk(il+2) = m;
                     *istk(il+3) = 2;
                     x_s = (SCSINT16_COP *) x;
                     for (j=0;j<m*n;j++)
                     {
                      ptr_s = (SCSINT16_COP *) istk(il+4);
                      ptr_s[j] = x_s[j];
                     }
                     break;

   case SCSINT32_N   : /* set header */
                     *istk(il) = 8; /*int32*/
                     *istk(il+1) = n;
                     *istk(il+2) = m;
                     *istk(il+3) = 4;
                     x_l = (SCSINT32_COP *) x;
                     for (j=0;j<m*n;j++)
                     {
                      ptr_l = (SCSINT32_COP *) istk(il+4);
                      ptr_l[j] = x_l[j];
                     }
                     break;

   case SCSUINT_N   : /* set header */
                     *istk(il) = 8; /*uint*/
                     *istk(il+1) = n;
                     *istk(il+2) = m;
                     *istk(il+3) = 14;
                     x_ui = (SCSUINT_COP *) x;
                     for (j=0;j<m*n;j++)
                     {
                      ptr_ui = (SCSUINT_COP *) istk(il+4);
                      ptr_ui[j] = x_ui[j];
                     }
                     break;

   case SCSUINT8_N   : /* set header */
                     *istk(il) = 8; /*uint8*/
                     *istk(il+1) = n;
                     *istk(il+2) = m;
                     *istk(il+3) = 11;
                     x_uc = (SCSUINT8_COP *) x;
                     for (j=0;j<m*n;j++)
                     {
                      ptr_uc = (SCSUINT8_COP *) istk(il+4);
                      ptr_uc[j] = x_uc[j];
                     }
                     break;

   case SCSUINT16_N  : /* set header */
                     *istk(il) = 8; /*uint16*/
                     *istk(il+1) = n;
                     *istk(il+2) = m;
                     *istk(il+3) = 12;
                     x_us = (SCSUINT16_COP *) x;
                     for (j=0;j<m*n;j++)
                     {
                      ptr_us = (SCSUINT16_COP *) istk(il+4);
                      ptr_us[j] = x_us[j];
                     }
                     break;

   case SCSUINT32_N  : /* set header */
                      *istk(il) = 8; /*uint32*/
                      *istk(il+1) = n;
                      *istk(il+2) = m;
                      *istk(il+3) = 14;
                      x_ul = (SCSUINT32_COP *) x;
                      for (j=0;j<m*n;j++)
                      {
                       ptr_ul = (SCSUINT32_COP *) istk(il+4);
                       ptr_ul[j] = x_ul[j];
                      }
                      break;

   case SCSUNKNOW_N  : lw=Top;
                       x_d = (double *) x;
                       C2F(unsfdcopy)(&nm,x_d,(j=1,&j),stk(*Lstk(Top)),(i=1,&i));
                       break;

   default         : /* set header */
                     *istk(il) = 1; /* double by default */
                     *istk(il+1) = n;
                     *istk(il+2) = m;
                     *istk(il+3) = 0;
                     x_d = (double *) x;
                     for (j=0;j<m*n;j++)
                     {
                      ptr_d = (double *) stk(il+4);
                      ptr_d[j] = x_d[j];
                     }
                     break;
  }

  /* set value in lstk */
  *Lstk(Top+1) = l + nm;

  /*return error flag = 0 */
  err = 0;
  return 0;
}

/* createblklist : function to create a Typed List
 *                 of a scicos_block structure
 *                 at the top+1 postion of the stack
 *
 * needs/depends : var2sci, C2F(mklist), C2F(mtklist),
 *                 vvtosci, itosci, string.h, C2F(scierr), str2sci
 *
 * input argument : Blocks :scicos_block  ptr on a scicos_block structure
 *                  ierr : int ptr, an error flag
 *                  flag_imp : if flag_imp>=0 then use
 *                             import structure for x, xd and g.
 *                             In this case flag_imp is the block number.
 *                  kfun : the current indexe of the block in the compiled structure
 *
 * output argument : return 0 if failed, 1 else.
 *
 *
 * 23/06/06, Alan : extracted from sciblk4 to be used
 *                  in intgetscicosvars
 *
 * 26/06/06, Alan : Add flag_imp in input arguments.
 *                  This is done to disable scilab crash with getscicosvars("blocks")
 *                  because when calling at the beginning of the simulation, x, xd and
 *                  g are not yet informed for all blocks with nx!=0 and ng!=0.
 *                  (They are not yet called with callf in scicos.c)
 *
 * 09/02/07, Alan : Add kfun flag and oz/opar
 *
 */

/*prototype*/
int createblklist(scicos_block *Blocks, int *ierr, int flag_imp, int kfun)
{
  /*local variable declaration*/
  int j,k;
  int nu,mu,ny,my;
  int u_typ,y_typ;
  int oz_typ,opar_typ;

  /*variable used when imp_flag>=0*/
  int nv,mv;          /* length of data                                        */
  int nblk,ng;        /* to store number of blocks and number of zero cross.   */
  void *ptr;          /* ptr for data comming from import structure            */
  int *ptr_int;       /* ptr to store ptr on int                           */
  double *ptr_double; /* ptr to store ptr on double                            */
  int *xptr, *zcptr;  /* to retrieve xptr by import and zcptr of scicos_blocks */
  double *x,*xd,*g;   /* ptr for x, xd and g for scicos_blocks              */
  int *funtyp;        /* ptr for funtyp */

  /* set length of block list -please update me-                           */
  static int nblklst=39;
  /* set string of first element of scilab Blocks tlist -please update me- */
  static char *str_blklst[]={ "scicos_block", "nevprt"  , "funpt" , "type"  ,
                              "scsptr"      , "nz"      , "z"     , "noz"   ,
                              "ozsz"        , "oztyp"   , "oz"    , "nx"    ,
                              "x"           , "xd"      , "res"   , "nin"   ,
                              "insz"        , "inptr"   , "nout"  , "outsz" ,
                              "outptr"      , "nevout"  , "evout" , "nrpar" ,
                              "rpar"        , "nipar"   , "ipar"  , "nopar" ,
                              "oparsz"      , "opartyp" , "opar"  , "ng"    ,
                              "g"           , "ztyp"    , "jroot" , "label" ,
                              "work"        , "nmode"   , "mode"};

  /* char ptr for str2sci - see below - */
  char **str1;

  /*retrieve funtyp by import structure*/
  strcpy(C2F(cha1).buf,"funtyp");
  *ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
  if (*ierr==0) return 0;
  funtyp = (int *) ptr;

  /* set nblk, x, xd ptr coming from import strucuture,
   * if flag_imp >=0
   */
  if (flag_imp>=0)
  {
   /*retrieve nblk by import structure*/
   strcpy(C2F(cha1).buf,"nblk");
   *ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
   if (*ierr==0) return 0;
   ptr_int = (int *) ptr;
   nblk = *ptr_int;

   /* retrieve ng by import structure */
   strcpy(C2F(cha1).buf,"ng");
   *ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
   ptr_int = (int *) ptr; /* cast void* ptr to int* ptr */
   ng = *ptr_int;

   /*retrieve xptr by import structure*/
   strcpy(C2F(cha1).buf,"xptr");
   *ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
   if (*ierr==0) return 0;
   ptr_int = (int *) ptr;
   xptr = ptr_int;

   /*retrieve zcptr by import structure*/
   strcpy(C2F(cha1).buf,"zcptr");
   *ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
   ptr_int = (int *) ptr;
   zcptr = ptr_int;

   /*retrieve x and xd by import structure*/
   strcpy(C2F(cha1).buf,"x");
   *ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
   if (*ierr==0) return 0;
   ptr_double = (double *) ptr;
   x = ptr_double;
   xd = &x[xptr[nblk]-1];

   /*retrieve g by import structure*/
   strcpy(C2F(cha1).buf,"g");
   *ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
   ptr_double = (double *) ptr;
   g = ptr_double;
  }

  /****************************
   * create scilab tlist Blocks
   ****************************/
  /* 1 - scicos_block */
  str2sci(str_blklst,1,nblklst);

  /* 2 - nevprt */
  C2F(itosci)(&Blocks[0].nevprt,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 3 - funpt */
  C2F(itosci)(&Blocks[0].funpt,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 4 - type */
  C2F(itosci)(&Blocks[0].type,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 5 - scsptr */
  C2F(itosci)(&Blocks[0].scsptr,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 6 - nz */
  C2F(itosci)(&Blocks[0].nz,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 7 - z */
  if(Blocks[0].scsptr>0)
  {
   C2F(vvtosci)(Blocks[0].z,&Blocks[0].nz);
   if (C2F(scierr)()!=0) return 0;
  }
  else
  {
   *ierr=var2sci(Blocks[0].z,Blocks[0].nz,1,SCSREAL_N);
   if (*ierr!=0) return 0; 
  }

  /* 8 - noz */
  C2F(itosci)(&Blocks[0].noz,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 9 - ozsz */
  C2F(itosci)(Blocks[0].ozsz,(j=Blocks[0].noz,&j),(k=2,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 10 - oztyp */
  C2F(itosci)(Blocks[0].oztyp,(j=Blocks[0].noz,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 11 - oz */
  for (k=0;k<Blocks[0].noz;k++)
  {
   nu=Blocks[0].ozsz[k]; /* retrieve number of rows */
   mu=Blocks[0].ozsz[Blocks[0].noz+k]; /* retrieve number of cols */
   oz_typ=Blocks[0].oztyp[k]; /* retrieve type */
   *ierr=var2sci(Blocks[0].ozptr[k],nu,mu,oz_typ);
   if (*ierr!=0) return 0;
  }
  /* if C blocks or null size then concatenate in a single list */
  if ((funtyp[kfun-1]==4) || (Blocks[0].noz==0))
  {
   C2F(mklist)(&Blocks[0].noz); /*create oz list*/
  }

  /* 12 - nx */
  C2F(itosci)(&Blocks[0].nx,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 13 - x */
  if (flag_imp>=0) *ierr=var2sci(&x[xptr[flag_imp]-1],Blocks[0].nx,1,SCSREAL_N);
  else *ierr=var2sci(Blocks[0].x,Blocks[0].nx,1,SCSREAL_N);
  if (*ierr!=0) return 0; 

  /* 14 - xd */
  if (flag_imp>=0) *ierr=var2sci(&xd[xptr[flag_imp]-1],Blocks[0].nx,1,SCSREAL_N);
  else *ierr=var2sci(Blocks[0].xd,Blocks[0].nx,1,SCSREAL_N);
  if (*ierr!=0) return 0; 

  /* 15 - res */
  *ierr=var2sci(Blocks[0].res,Blocks[0].nx,1,SCSREAL_N);
  if (*ierr!=0) return 0;

  /* 16 - nin */
  C2F(itosci)(&Blocks[0].nin,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 17 - insz */
  C2F(itosci)(Blocks[0].insz,(j=3*Blocks[0].nin,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 18 - inptr */
  for (k=0;k<Blocks[0].nin;k++) 
  {
   nu=Blocks[0].insz[k]; /* retrieve number of rows */
   mu=Blocks[0].insz[Blocks[0].nin+k]; /* retrieve number of cols */
   u_typ=Blocks[0].insz[2*Blocks[0].nin+k]; /* retrieve type */
   *ierr=var2sci(Blocks[0].inptr[k],nu,mu,u_typ);
   if (*ierr!=0) return 0;
  }
  C2F(mklist)(&Blocks[0].nin); /*create inptr list*/

  /* 19 - nout */
  C2F(itosci)(&Blocks[0].nout,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 20 - outsz */
  C2F(itosci)(Blocks[0].outsz,(j=3*Blocks[0].nout,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 21 - outptr */
  for (k=0;k<Blocks[0].nout;k++) 
  {
   ny=Blocks[0].outsz[k]; /* retrieve number of rows */
   my=Blocks[0].outsz[Blocks[0].nout+k]; /* retrieve number of cols */
   y_typ=Blocks[0].outsz[2*Blocks[0].nout+k]; /* retrieve type */
   *ierr=var2sci(Blocks[0].outptr[k],ny,my,y_typ);
   if (*ierr!=0) return 0;
  }
  C2F(mklist)(&Blocks[0].nout); /*create outptr list*/

  /* 22 - nevout */
  C2F(itosci)(&Blocks[0].nevout,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 23 - evout */
  *ierr=var2sci(Blocks[0].evout,Blocks[0].nevout,1,SCSREAL_N);
   if (*ierr!=0) return 0;

  /* 24 - nrpar */
  C2F(itosci)(&Blocks[0].nrpar,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 25 - rpar */
  if(Blocks[0].scsptr>0)
  {
   C2F(vvtosci)(Blocks[0].rpar,&Blocks[0].nrpar);
   if (C2F(scierr)()!=0) return 0;
  }
  else
  {
   *ierr=var2sci(Blocks[0].rpar,Blocks[0].nrpar,1,SCSREAL_N);
   if (*ierr!=0) return 0;
  }

  /* 26 - nipar */
  C2F(itosci)(&Blocks[0].nipar,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 27 - ipar */
  C2F(itosci)(Blocks[0].ipar,(j=Blocks[0].nipar,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 28 - nopar */
  C2F(itosci)(&Blocks[0].nopar,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 29 - oparsz */
  C2F(itosci)(Blocks[0].oparsz,(j=Blocks[0].nopar,&j),(k=2,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 30 - opartyp */
  C2F(itosci)(Blocks[0].opartyp,(j=Blocks[0].nopar,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 31 - opar */
  for (k=0;k<Blocks[0].nopar;k++)
  {
   nu=Blocks[0].oparsz[k]; /* retrieve number of rows */
   mu=Blocks[0].oparsz[Blocks[0].nopar+k]; /* retrieve number of cols */
   opar_typ=Blocks[0].opartyp[k]; /* retrieve type */
   *ierr=var2sci(Blocks[0].oparptr[k],nu,mu,opar_typ);
   if (*ierr!=0) return 0;
  }
  /* if C blocks or null size then concatenate in a single list */
  if ((funtyp[kfun-1]==4) || (Blocks[0].nopar==0))
  {
   C2F(mklist)(&Blocks[0].nopar); /*create opar list*/
  }

  /* 32 - ng */
  C2F(itosci)(&Blocks[0].ng,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 33 - g */
  if (flag_imp>=0) *ierr=var2sci(&g[zcptr[flag_imp]-1],Blocks[0].ng,1,SCSREAL_N);
  else *ierr=var2sci(Blocks[0].g,Blocks[0].ng,1,SCSREAL_N);
  if (*ierr!=0) return 0;

  /* 34 - ztyp */
  C2F(itosci)(&Blocks[0].ztyp,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 35 - jroot */
  C2F(itosci)(Blocks[0].jroot,(j=Blocks[0].ng,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 36 - label */
  if ((str1=MALLOC(sizeof(char*))) ==NULL )  return 0;
  if ((str1[0]=MALLOC(sizeof(char)*(strlen(Blocks[0].label)+1))) ==NULL )  return 0;
  (str1[0])[strlen(Blocks[0].label)]='\0';
  strncpy(str1[0],Blocks[0].label,strlen(Blocks[0].label));
  str2sci(str1,1,1);
  FREE(str1[0]);
  FREE(str1);
  if (C2F(scierr)()!=0) return 0; 

  /* 37 - work*/
  C2F(vvtosci)(*Blocks[0].work,(k=0,&k));
  if (C2F(scierr)()!=0) return 0; 

  /* 38 - nmode*/
  C2F(itosci)(&Blocks[0].nmode,(j=1,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  /* 39 - mode */
  C2F(itosci)(Blocks[0].mode,(j=Blocks[0].nmode,&j),(k=1,&k));
  if (C2F(scierr)()!=0) return 0;

  C2F(mktlist)(&nblklst); /*create Blocks list*/
  if (C2F(scierr)()!=0) return 0;

  /*return 1 if succeeded */
  return 1;
}

/* intgetscicosvarsc getscicosvars interface routine
 * retrieve some informations during simulation.
 *
 * [myvar]=getscicosvars(str)
 *
 * rhs 1  : str : a character string matrix with choice,
 *               - 'x' to retrieve continuous state
 *               - 'xptr' to retrieve ptr of continuous state
 *               - 'z' to retrieve discrete state
 *               - 'zptr' to retrieve ptr of discrete state
 *               - 'rpar' to retrieve real parameters
 *               - 'rpptr' to retrieve ptr of real parameters
 *               - 'ipar' to retrieve int parameters
 *               - 'ipptr' to retrieve  ptr of int parameters
 *               - 'outtb' to retrieve output register (list of scilb object)
 *               - 'inpptr' to retrieve number of input ports
 *               - 'outptr' to retrieve number of output ports
 *               - 'inplnk' to retrieve link number of input ports
 *               - 'outlnk' to retrieve link number of output ports
 *               ...... -see below-
 *
 * lhs 1  : myvar : matrix of int32 or double, or list or a Tlist
 *
 * 31/05/06, Alan : Rewritten from original fortran
 * source code intgetscicosvars in coselm.f.
 *
 * 22/06/06, Alan : Allow multiple string in rhs(1).
 *                  Create Tlist for Lhs(1).
 *
 * 23/06/06, Alan : Create blocks list for Lhs(1).
 *
 * 13/11/06, Alan : Remove il_sim_save global variable (all in sim
 *                  come from import struct now)
 *                  evtspt & pointi of state come from import struct
 *
 * 09/02/07, Alan : Update with oz/opar and restore il_sim_save only for opar
 *
 */

int intgetscicosvarsc(char *fname,unsigned long fname_len)
{
  /************************************
   * variables and constants d?inition
   ************************************/
  /* auxilary variables for dimension and address */
  int m1,n1;     /* dimension of input character string               */
  int *il_str;   /* address of the description of the input parameter */
  int *l_str;    /* address of the data of the input parameter        */
  int l_tmp;     /* temp variables to store address                   */
  int *il_tmp;

  /* definition of min/max input/output argument */
  static int minrhs=0, maxrhs=1;

  /* auxilary variables */
  int nv,mv;                /* length of data                                      */
  void *ptr;                /* ptr for data comming from import structure          */
  int *ptr_int;             /* ptr to store ptr on int                         */
  double *ptr_dd;           /* ptr to store ptr on double                          */
  scicos_block *ptr_scsblk; /* ptr to store ptr of scicos_block structure          */
  outtb_el *ptr_elem;       /* ptr to store ptr of outtb_el structure              */
  int nblk,ng;              /* to store number of blocks and number of zero cross. */
  int *xptr, *zcptr;        /* to store xptr and zcptr of scicos_blocks            */
  double *x,*xd,*g;         /* ptr for x, xd and g for scicos_blocks               */

  int ierr;       /* error flag                                */
  int errc;       /* error flag for ceateblklst                */
  int ptr_pos;    /* to store position in scilab string object */
  int Topsave;    /* to save the Top position                  */

  int sz_str;     /* local variabe to store size of string */
  int i,j,k;      /* local counter variable                */

  /* number of entries -please update me-                        */
  static int nentries=69;
  /* define accepted entries of getscicosvars -please update me- */
  static char *entry[]={"x"       , "nx"       , "xptr"   , "zcptr"      , "z"        ,
                        "nz"      , "zptr"     , "noz"    , "oz"         , "ozsz"     ,
                        "oztyp"   , "ozptr"    , "rpar"   , "rpptr"      , "ipar"     ,
                        "ipptr"   , "opar"     , "oparsz" , "opartyp"    , "opptr"    ,
                        "outtb"   , "inpptr"   , "outptr" , "inplnk"     , "outlnk"   ,
                        "subs"    , "tevts"    , "evtspt" , "pointi"     , "iord"     ,
                        "oord"    , "zord"     , "funtyp" , "ztyp"       , "cord"     ,
                        "ordclk"  , "clkptr"   , "ordptr" , "critev"     , "mod"      ,
                        "nmod"    , "iz"       , "nblk"   , "izptr"      , "outtbptr" ,
                        "outtbsz" , "outtbtyp" , "nlnk"   , "nsubs"      , "nevts"    ,
                        "niord"   , "noord"    , "nzord"  , "funptr"     , "ncord"    ,
                        "nordptr" , "iwa"      , "blocks" , "ng"         , "g"        ,
                        "t0"      , "tf"       , "Atol"   , "rtol"       , "ttol"     ,
                        "deltat"  , "hmax"     , "nelem"  , "outtb_elem"};

  char **dyn_char; /* for allocation of first entry in tlist */

  /****************************************
   * Check number of inputs and outputs Rhs
   ****************************************/
  CheckRhs(minrhs,maxrhs);

  /* Display usage of getscicosvars function if Rhs==0 */
  if (Rhs==0)
  {
   sciprint("\ngetscicosvars : utility function to retrieve\n"
            "                scicos arrays during simulation.\n\n"
            "Usage : [myvar]=getscicosvars([\"str1\";\"str2\";...]);\n\n"
            "- myvar : an int32 or double matrix or a Tlist.\n"
            "- [\"str1\";\"str2\",...] is a string matrix\n"
            "  that must be informed with the following values :\n");

   /* display allowed entries */
   i=0;
   for (j=0;j<nentries;j++)
   {
    if (j==nentries-1) sciprint("\"%s\" ",entry[j]);
    else sciprint("\"%s\", ",entry[j]);
    i++;
    if (i==6) {sciprint("\n");i=0;}
   }
   sciprint("\n");
   return 0;
  }

  /*******************
   * Check str (rhs 1)
   *******************/
  il_str = (int *) GetData(1); /* get ptr of int header of rsh 1 */
  if(il_str[0]!=10) /* Check if input argument is a character string matrix */
  {
   Scierror(55,"%s : First argument must be a string.\n",fname);
   C2F(iop).err=1;
   return 0;
  }

  /*retrieve dimension of input string matrix*/
  m1 = il_str[1]; /* number of row    */
  n1 = il_str[2]; /* number of column */

  /*Create header of Tlist*/
  if ((dyn_char=MALLOC((1+m1*n1)*sizeof(char *)))==NULL)
  {
   Scierror(999,"%s : No more memory.\n",fname);
   return 0;
  }

  /* Type of list is scicosvar */
  if ((dyn_char[0]=MALLOC((strlen("scicosvar")+1)*sizeof(char)))==NULL)
  {
   FREE(dyn_char);
   Scierror(999,"%s : No more memory.\n",fname);
   return 0;
  }
  else strcpy(dyn_char[0],"scicosvar");

  /*Check string matrix  */
  for (j=0;j<m1*n1;j++)
  {
    sz_str = il_str[5+j]-il_str[4+j]; /* store the length of str */
    /*get current position in the istk*/
    if (j==0)
      ptr_pos=5+m1*n1; 
    else
      ptr_pos += il_str[5+j-1]-il_str[4+j-1];
    l_str = &il_str[ptr_pos]; /* get ptr of rsh 1 */
    /* codetoascii convertion */
    C2F(cha1).buf[0]=' ';
    C2F(cvstr)(&sz_str,&l_str[0],&C2F(cha1).buf[0],(i=1,&i),sz_str);
    C2F(cha1).buf[sz_str]='\0';
    /* search if string is in accordance with entry*/
    ierr=TRUE_;
    for (i=0;i<nentries;i++)
    {
     if (strcmp(C2F(cha1).buf,entry[i]) == 0) 
     {
      /* Store string in header of Tlist */
      if ((dyn_char[j+1]=MALLOC(sizeof(char)*strlen(entry[i])+1))==NULL)
      {
       FREE(dyn_char);
       Scierror(999,"%s : No more memory.\n",fname);
       return 0;
      }
      else strcpy(dyn_char[j+1],entry[i]);

      ierr=FALSE_;
      break;
     }
    }
    /* if failed then display an error message and exit*/
    if (ierr==TRUE_)
    {
     FREE(dyn_char);
     Scierror(999,"%s : Undefined field in string matrix position : %d.\n",fname,j+1);
     return 0;
    }
  }

  /* store dyn_char on stack*/
  if (n1*m1>1) str2sci(dyn_char,1,n1*m1+1);

  /* return asked array */
  for (j=0;j<m1*n1;j++)
  {
   sz_str = il_str[5+j]-il_str[4+j]; /* store the length of str */
   /*get current position in the istk of the string*/
   if (j==0)
     ptr_pos=5+m1*n1; 
   else
     ptr_pos += il_str[5+j-1]-il_str[4+j-1];
   l_str = &il_str[ptr_pos]; /* get ptr of rsh 1 */
   /* codetoascii convertion */
   C2F(cha1).buf[0]=' ';
   C2F(cvstr)(&sz_str,&l_str[0],&C2F(cha1).buf[0],(i=1,&i),sz_str);
   C2F(cha1).buf[sz_str]='\0';

   /*****************************************************************
    * entries that can be retrieve by il_state_save global variable
    *****************************************************************/
   if (strcmp(C2F(cha1).buf,"x") == 0)           /* retrieve continuous state */
    ierr=CopyVarFromlistentry(j+2,il_state_save,2);
   else if (strcmp(C2F(cha1).buf,"z") == 0)      /* retrieve discrete state */
    ierr=CopyVarFromlistentry(j+2,il_state_save,3);
   else if (strcmp(C2F(cha1).buf,"oz") == 0)     /* retrieve object discrete state */
    ierr=CopyVarFromlistentry(j+2,il_state_save,4);
   else if (strcmp(C2F(cha1).buf,"outtb") == 0)  /* retrieve outtb */
    ierr=CopyVarFromlistentry(j+2,il_state_save,9);
   else if (strcmp(C2F(cha1).buf,"tevts") == 0)  /* retrieve tevts */
    ierr=CopyVarFromlistentry(j+2,il_state_save,6);

   /***************************************************************
    * entries that can be retrieve by il_sim_save global variable
    ***************************************************************/
   if (strcmp(C2F(cha1).buf,"opar") == 0)           /* retrieve object parameters */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,15);

   /*************************************************
    * int variables coming from import structure
    *************************************************/
   else if ((strcmp(C2F(cha1).buf,"mod") == 0)      || /* retrieve mode */
            (strcmp(C2F(cha1).buf,"nmod") == 0)     || /* retrieve nmode */
            (strcmp(C2F(cha1).buf,"iz") == 0)       || /* label int code of blocks */
            (strcmp(C2F(cha1).buf,"nblk") == 0)     || /* number of block */
            (strcmp(C2F(cha1).buf,"izptr") == 0)    || /* label int code of blocks ptr*/
            (strcmp(C2F(cha1).buf,"outtbptr") == 0) || /* outtb ptr */
            (strcmp(C2F(cha1).buf,"outtbsz") == 0)  || /* outtb size */
            (strcmp(C2F(cha1).buf,"outtbtyp") == 0) || /* outtb type */
            (strcmp(C2F(cha1).buf,"nlnk") == 0)     || /* number of link */
            (strcmp(C2F(cha1).buf,"nsubs") == 0)    || /* length of nsubs */
            (strcmp(C2F(cha1).buf,"nevts") == 0)    || /* length of evtspt & tevts */
            (strcmp(C2F(cha1).buf,"niord") == 0)    || /* length of iord */
            (strcmp(C2F(cha1).buf,"noord") == 0)    || /* length of oord */
            (strcmp(C2F(cha1).buf,"nzord") == 0)    || /* length of zord */
            (strcmp(C2F(cha1).buf,"funptr") == 0)   || /* retrieve function ptr */
            (strcmp(C2F(cha1).buf,"ncord") == 0)    || /* retrieve ncord */
            (strcmp(C2F(cha1).buf,"nordptr") == 0)  || /* retrieve nordptr */
            (strcmp(C2F(cha1).buf,"iwa") == 0)      || /* retrieve iwa */
            (strcmp(C2F(cha1).buf,"ng") == 0)       || /* retrieve ng */
            (strcmp(C2F(cha1).buf,"nx") == 0)       || /* retrieve nx */
            (strcmp(C2F(cha1).buf,"nz") == 0)       || /* retrieve nz */
            (strcmp(C2F(cha1).buf,"noz") == 0)      || /* retrieve noz */
            (strcmp(C2F(cha1).buf,"ozptr") == 0)    || /* retrieve ozptr */
            (strcmp(C2F(cha1).buf,"ozsz") == 0)     || /* retrieve ozsz */
            (strcmp(C2F(cha1).buf,"oztyp") == 0)    || /* retrieve oztyp */
            (strcmp(C2F(cha1).buf,"nelem") == 0)    || /* retrieve nelem */
            (strcmp(C2F(cha1).buf,"xptr") == 0)     || /* retrieve xptr */
            (strcmp(C2F(cha1).buf,"zcptr") == 0)    || /* retrieve zcptr */
            (strcmp(C2F(cha1).buf,"zptr") == 0)     || /* retrieve zptr */
            (strcmp(C2F(cha1).buf,"rpptr") == 0)    || /* retrieve rpptr */
            (strcmp(C2F(cha1).buf,"ipar") == 0)     || /* retrieve ipar */
            (strcmp(C2F(cha1).buf,"ipptr") == 0)    || /* retrieve ipptr */
            (strcmp(C2F(cha1).buf,"opptr") == 0)    || /* retrieve opptr */
            (strcmp(C2F(cha1).buf,"oparsz") == 0)   || /* retrieve oparsz */
            (strcmp(C2F(cha1).buf,"opartyp") == 0)  || /* retrieve opartyp */
            (strcmp(C2F(cha1).buf,"inpptr") == 0)   || /* retrieve inpptr */
            (strcmp(C2F(cha1).buf,"outptr") == 0)   || /* retrieve outptr */
            (strcmp(C2F(cha1).buf,"inplnk") == 0)   || /* retrieve inplnk */
            (strcmp(C2F(cha1).buf,"outlnk") == 0)   || /* retrieve outlnk */
            (strcmp(C2F(cha1).buf,"subs") == 0)     || /* retrieve subs */
            (strcmp(C2F(cha1).buf,"iord") == 0)     || /* retrieve iord */
            (strcmp(C2F(cha1).buf,"oord") == 0)     || /* retrieve iord */
            (strcmp(C2F(cha1).buf,"zord") == 0)     || /* retrieve iord */
            (strcmp(C2F(cha1).buf,"funtyp") == 0)   || /* retrieve funtyp */
            (strcmp(C2F(cha1).buf,"ztyp") == 0)     || /* retrieve ztyp */
            (strcmp(C2F(cha1).buf,"cord") == 0)     || /* retrieve cord */
            (strcmp(C2F(cha1).buf,"ordclk") == 0)   || /* retrieve ordclk */
            (strcmp(C2F(cha1).buf,"clkcpr") == 0)   || /* retrieve clkcpr */
            (strcmp(C2F(cha1).buf,"ordptr") == 0)   || /* retrieve ordptr */
            (strcmp(C2F(cha1).buf,"critev") == 0)   || /* retrieve critev */
            /* state */
            (strcmp(C2F(cha1).buf,"evtspt") == 0)   || /* retrieve evtspt */
            (strcmp(C2F(cha1).buf,"pointi") == 0)      /* retrieve nelem */
                                                     )
   {
    /* retrieve dims and prt of asked array with getscicosvarsfromimport */
    ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

    /* check ierr flag */
    if (ierr==TRUE_)
    {
     l_tmp = I_INT32; /* define type of int */
     CreateVar(j+2,"I",&nv,&mv,&l_tmp); /* Create int32 variable at the top+j+1 pos in the stack */
     il_tmp = (int *) istk(l_tmp);      /* Store value of address of istk(l_tmp) in il_tmp */
     ptr_int = (int *) ptr;             /* cast void* ptr to int* ptr */
     for (i=0;i<nv*mv;i++) il_tmp[i] = ptr_int[i]; /* copy returned array in istk */
    }
   }

  /*************************************************
   * double variables coming from import structure
   *************************************************/
   else if ((strcmp(C2F(cha1).buf,"rpar") == 0)   || /* retrieve rpar      */
            (strcmp(C2F(cha1).buf,"g") == 0)      || /* retrieve g      */
            (strcmp(C2F(cha1).buf,"t0") == 0)     || /* retrieve t0     */
            (strcmp(C2F(cha1).buf,"tf") == 0)     || /* retrieve tf     */
            (strcmp(C2F(cha1).buf,"Atol") == 0)   || /* retrieve Atol   */
            (strcmp(C2F(cha1).buf,"rtol") == 0)   || /* retrieve rtol   */
            (strcmp(C2F(cha1).buf,"ttol") == 0)   || /* retrieve ttol   */
            (strcmp(C2F(cha1).buf,"deltat") == 0) || /* retrieve deltat */
            (strcmp(C2F(cha1).buf,"hmax") == 0)      /* retrieve hmax   */
                                                 )
   {
    /* retrieve dims and prt of asked array with getscicosvarsfromimport */
    ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

    /* check ierr flag */
    if (ierr==TRUE_)
    {
     ptr_dd = (double *) ptr;
     CreateVar(j+2,"d",&nv,&mv,&l_tmp); /* Create double variable at the top+j+1 addr. of the stack */
     for (i=0;i<nv*mv;i++) *stk(l_tmp+i) = ptr_dd[i]; /* copy returned array in istk */
    }
   }

   /*************************************************
    * scicos_block ptr coming from import structure
    *************************************************/
   else if ((strcmp(C2F(cha1).buf,"blocks") == 0)
                                                )
   {
    /* retrieve scicos_block prt of asked array with getscicosvarsfromimport */
    ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

    /* check ierr flag */
    if (ierr==TRUE_)
    {
     /* store ptr in ptrscs_blk */
     ptr_scsblk = (scicos_block *) ptr;

     /* retrieve nblk by import structure */
     strcpy(C2F(cha1).buf,"nblk");
     ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
     ptr_int = (int *) ptr; /* cast void* ptr to int* ptr */
     nblk = *ptr_int;

     /* retrieve ng by import structure */
     strcpy(C2F(cha1).buf,"ng");
     ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
     ptr_int = (int *) ptr; /* cast void* ptr to int* ptr */
     ng = *ptr_int;

     /*retrieve xptr by import structure*/
     strcpy(C2F(cha1).buf,"xptr");
     ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
     ptr_int = (int *) ptr;
     xptr = ptr_int;

     /*retrieve zcptr by import structure*/
     strcpy(C2F(cha1).buf,"zcptr");
     ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
     ptr_int = (int *) ptr;
     zcptr = ptr_int;

     /*retrieve x and xd by import structure*/
     strcpy(C2F(cha1).buf,"x");
     ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
     ptr_dd = (double *) ptr;
     x = ptr_dd;
     xd = &x[xptr[nblk]-1];

     /*retrieve g by import structure*/
     strcpy(C2F(cha1).buf,"g");
     ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
     ptr_dd = (double *) ptr;
     g = ptr_dd;

     /* store blklst on stack */
     Topsave=Top;     /* save Top counter */
     Top=Top-Rhs+1+j; /* adjust Top counter */

     for (k=0;k<nblk;k++)
     {/* for each block, call createblklist */

      /* set flag_imp <0 for createblklst */
      i=-1;

      /* the following test is done in order to know if block k
       * have been already called with callf in scicos.c
       */
      if (ptr_scsblk[k].nx!=0)
      {
       if (ptr_scsblk[k].x!=&x[xptr[k]-1])
       {
         /*fprintf(stderr,"k=%d,X,xd Non initialise",k);*/
        /* set flag_imp=k for createblklst <0 */
        i=k;
       }
      }
      if (ptr_scsblk[k].ng!=0)
      {
       if ((ptr_scsblk[k].g!=&g[zcptr[k]-1]) && (ptr_scsblk[k].g!=&x[xptr[k]-1]))
       {
        /*fprintf(stderr,"k=%d,g Non initialise",k);*/
        /* set flag_imp=k for createblklst <0 */
        i=k;
       }
      }
      /* call createblklist */
      ierr=createblklist(&ptr_scsblk[k], &errc,i,k+1);

      /* if an error occurs in createblklist */
      if (ierr==FALSE_)
      {
       Top=Topsave;
       break;
      }
     }
     /* if success, create a list of Typed list scicos_block */
     if (ierr==TRUE_)
     {
      C2F(mklist)(&nblk);
      Top=Topsave; /* adjust Top counter */
      CreateVar(j+2,"l",&nblk,(i=1,&i),&l_tmp); /* this is done to inform common intersci */
     }

    }
   }

  /*******************************************
   * outtb_elem coming from import structure
   *******************************************/
   else if ((strcmp(C2F(cha1).buf,"outtb_elem") == 0)) /* retrieve outtb_elem */
   {
    /* retrieve dims and prt of asked array with getscicosvarsfromimport */
    ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

    /* check ierr flag */
    if (ierr==TRUE_)
    {
     l_tmp = I_INT32; /* define type of int */
     CreateVar(j+2,"I",&nv,&mv,&l_tmp); /* Create int32 variable at the top+j+1 addr. of the stack */
     il_tmp = (int *) istk(l_tmp);      /* Store value of address of istk(l_tmp) in il_tmp */
     ptr_elem = (outtb_el *) ptr;       /* cast void* ptr to int* ptr */
     for (i=0;i<nv;i++) /* copy returned array in istk */
     {
      il_tmp[i] = ptr_elem[i].lnk + 1; /* +1 is for the connection with outtb list */
      il_tmp[nv+i] = ptr_elem[i].pos + 1;
     }
    }
   }

   /* if return a FALSE_ value in
    * error flag then display an error message.
    */
   if(ierr!=TRUE_)
   {
    Scierror(999,"%s : Error.\n",fname);
    FREE(dyn_char);
    return 0;
   }
  }
  /**********************
   * Create lsh argument
   **********************/
   /* create a tlist if number of string in rhs 1 is >1*/
   if (j>1) 
   {
    Top=Top+j;
    j++;
    C2F(mktlist)(&j);
    Top=Top-1;
    CreateVar(2,"t",&j,(i=1,&i),&l_tmp); /* this is done to inform common intersci */
   }

   /* put new variable in lsh argument */
   LhsVar(1)=2;

   /* end */
   FREE(dyn_char);
   return 0;
}

/* intcurblkc curblock interface routine
 *
 * [nblk]=curblock()
 *
 * rhs : empty
 * lhs : nblk : the current block (double)
 *
 * 20/06/06, Alan : Rewritten from original fortran
 * source code intcurblk in coselm.f.
 *
 * 12/02/07 : update lhs to double output
 *
 */
int intcurblkc(char *fname,unsigned long fname_len)
{
  /***********************
   * variables declaration
   ***********************/
  /* address of the data of the output parameter */
  int l1;
  /* local counter variable */
  int j,k;
  /* definition of min/max output argument */
  static int minlhs=1, maxlhs=1;

  /**************************
   * Check number of  outputs
   **************************/
  CheckLhs(minlhs,maxlhs);

  /************************
   * Create double variable
   ************************/
  /* Create int32 variable at the top addr. of the stack */
  CreateVar(1,"d",(j=1,&j),(k=1,&k),&l1);
  /* Store value of C2F(curblk).kfun at the l1 address in istk */
  *stk(l1) = (double)C2F(curblk).kfun;

  /* return the value stored at Top address to lhs variable */
  LhsVar(1) = 1;

  /* return 0 as default value */
  return 0;
}

/* intbuildouttb build an initialized outtb list
 *
 * [outtb]=buildouttb(lnksz,lnktyp)
 *
 * rhs 1 : lnksz, give the size of scilab object in outtb
 *         can be all int type or double matrix
 *         can have n,2 or 2,n size
 *
 * rhs 2 : lnktyp, gives the type of scilab objetc in outtb
 *         1 : double
 *         2 : complex
 *         3 : int32
 *         4 : int16
 *         5 : int8
 *         6 : uint32
 *         7 : uint16
 *         8 : uint8
 *         else : double
 *         can be all int type or double matrix
 *         can have n,1 or 1,n size
 *
 * lhs 1 : a list of size n
 *
 * 02/07/06, Alan : Initial version.
 * 21/11/06, Alan : Allow void rhs input param.
 *
 */

int intbuildouttb(char *fname,unsigned long fname_len)
{
 static int l1, m1,   n1;
 static int l2, m2,   n2;
 static int l3, n3=1;
 SciIntMat M1,M2,M3;

 int n_lnksz,n_lnktyp;
 int *lnksz=NULL,*lnktyp=NULL;

 double *ptr_d;
 double *ptr_dc;
 int *ptr_i;
 short *ptr_s;
 char *ptr_c;
 int *ptr_ui;
 short *ptr_us;
 char *ptr_uc;

 int nm,i,j,ierr=0;

 static int minlhs=1, maxlhs=1, minrhs=2, maxrhs=2;

 /*check number of lhs/rhs*/
 CheckLhs(minlhs,maxlhs);
 CheckRhs(minrhs,maxrhs);

 /*check type of Rhs 1*/
 if (VarType(1)==1)
 {
  GetRhsVar(1, "d", &m1, &n1, &l1);
 }
 else if (VarType(1)==8)
 {
  GetRhsVar(1, "I", &m1, &n1, &M1);
 }
 else
 {
  Scierror(888,"%s : lnksz argument must be double or integer.\n",fname);
  return 0;
 }

 /*check type of Rhs 2*/
 if (VarType(2)==1)
 {
  GetRhsVar(2, "d", &m2, &n2, &l2);
 }
 else if (VarType(2)==8)
 {
  GetRhsVar(2, "I", &m2, &n2, &M2);
 }
 else
 {
  Scierror(888,"%s : lnktyp argument must be double or integer.\n",fname);
  if (lnksz!=NULL) FREE(lnksz);
  return 0;
 }

 /*check size of Rhs 1*/
 if (m1==2) n_lnksz=n1;
 else if (n1==2) n_lnksz=m1;
 /*void double input give void list output*/
 else if ((n1==0)||(m1==0))
 {
  if ((n2==0)||(m2==0))
  {
    /*manually code a void list on rhs(1)*/
    ptr_i=(int*)GetData(1);
    ptr_i[0]=15;
    ptr_i[1]=0;
    ptr_i[2]=1;
    LhsVar(1)=1;
    return 0;
  }
  else
  {
   Scierror(888,"%s : inconsistent dimension between lnksz and lnktyp.\n",fname);
   return 0;
  }
 }
 else
 {
  Scierror(888,"%s : bad dimension for lnksz argument.\n",fname);
  return 0;
 }

 /*check size of Rhs 2*/
 if (m2==1) n_lnktyp=n2;
 else if (n2==1) n_lnktyp=m2;
 else if ((n2==0)||(m2==0))
 {
  if ((n1!=0)&&(m1!=0))
  {
   Scierror(888,"%s : inconsistent dimension between lnksz and lnktyp.\n",fname);
   return 0;
  }
 }
 else
 {
  Scierror(888,"%s : bad dimension for lnktyp argument.\n",fname);
  return 0;
 }

 /*cross size checking*/
 if (n_lnksz!=n_lnktyp)
 {
  Scierror(888,"%s : lnksz and lnktyp argument must have "
               "the same length.\n",fname);
  return 0;
 }

 /*allocate lnksz*/
 if ((lnksz=MALLOC(2*n_lnksz*sizeof(int)))==NULL)
 {
  Scierror(999,"%s : No more free memory.\n",fname);
  return 0;
 }

 /* Allocate lnktyp*/
 if ((lnktyp=MALLOC(n_lnktyp*sizeof(int)))==NULL)
 {
  Scierror(999,"%s : No more free memory.\n",fname);
  if (lnksz!=NULL) FREE(lnksz);
  return 0;
 }

 /*store rhs 1 in lnksz */
 if ((m1==n1)&&(m2==m1)) m1=-1; /* this is done for matrix 2,2 */
 if (VarType(1)==8)
 {
  switch (M1.it)
  {
   case I_CHAR   : if (m1==2)
                   {
                    for(j=0;j<n_lnksz;j++)
                    {
                     lnksz[j]=(int) IC_CHAR(M1.D)[j*2];
                     lnksz[n_lnksz+j]=(int) IC_CHAR(M1.D)[2*j+1];
                    }
                   }
                   else
                   {
                    for(j=0;j<2*n_lnksz;j++)
                      lnksz[j]=(int) IC_CHAR(M1.D)[j];
                   }
                   break;

   case I_INT16  : if (m1==2)
                   {
                    for(j=0;j<n_lnksz;j++)
                    {
                     lnksz[j]=(int) IC_INT16(M1.D)[j*2];
                     lnksz[n_lnksz+j]=(int) IC_INT16(M1.D)[2*j+1];
                    }
                   }
                   else
                   {
                    for(j=0;j<2*n_lnksz;j++)
                      lnksz[j]=(int) IC_INT16(M1.D)[j];
                   }
                   break;

   case I_INT32  : if (m1==2)
                   {
                    for(j=0;j<n_lnksz;j++)
                    {
                     lnksz[j]=(int) IC_INT32(M1.D)[j*2];
                     lnksz[n_lnksz+j]=(int) IC_INT32(M1.D)[2*j+1];
                    }
                   }
                   else
                   {
                    for(j=0;j<2*n_lnksz;j++)
                      lnksz[j]=(int) IC_INT32(M1.D)[j];
                   }
                   break;

   case I_UCHAR  : if (m1==2)
                   {
                    for(j=0;j<n_lnksz;j++)
                    {
                     lnksz[j]=(int) IC_UCHAR(M1.D)[j*2];
                     lnksz[n_lnksz+j]=(int) IC_UCHAR(M1.D)[2*j+1];
                    }
                   }
                   else
                   {
                    for(j=0;j<2*n_lnksz;j++)
                      lnksz[j]=(int) IC_UCHAR(M1.D)[j];
                   }
                   break;

   case I_UINT16 : if (m1==2)
                   {
                    for(j=0;j<n_lnksz;j++)
                    {
                     lnksz[j]=(int) IC_UINT16(M1.D)[j*2];
                     lnksz[n_lnksz+j]=(int) IC_UINT16(M1.D)[2*j+1];
                    }
                   }
                   else
                   {
                    for(j=0;j<2*n_lnksz;j++)
                      lnksz[j]=(int) IC_UINT16(M1.D)[j];
                   }
                   break;

   case I_UINT32 : if (m1==2)
                   {
                    for(j=0;j<n_lnksz;j++)
                    {
                     lnksz[j]=(int) IC_UINT32(M1.D)[j*2];
                     lnksz[n_lnksz+j]=(int) IC_UINT32(M1.D)[2*j+1];
                    }
                   }
                   else
                   {
                    for(j=0;j<2*n_lnksz;j++)
                      lnksz[j]=(int) IC_UINT32(M1.D)[j];
                   }
                   break;
  }
 }
 else
 {
  if (m1==2)
  {
   for(j=0;j<n_lnksz;j++)
   {
    lnksz[j]=(int) ((double *) stk(l1))[j*2];
    lnksz[n_lnksz+j]=(int) ((double *) stk(l1))[2*j+1];
   }
  }
  else
  {
   for(j=0;j<2*n_lnksz;j++)
     lnksz[j]=(int) ((double *) stk(l1))[j];
  }
 }

 /*store rhs 2 in lnktyp */
 if (VarType(2)==8)
 {
  switch (M2.it)
  {
   case I_CHAR   : for(j=0;j<n_lnktyp;j++)
                     lnktyp[j]=(int) IC_CHAR(M2.D)[j];
                   break;

   case I_INT16  : for(j=0;j<n_lnktyp;j++)
                     lnktyp[j]=(int) IC_INT16(M2.D)[j];
                   break;

   case I_INT32  : for(j=0;j<n_lnktyp;j++)
                     lnktyp[j]=(int) IC_INT32(M2.D)[j];
                   break;

   case I_UCHAR  : for(j=0;j<n_lnktyp;j++)
                     lnktyp[j]=(int) IC_UCHAR(M2.D)[j];
                   break;

   case I_UINT16 : for(j=0;j<n_lnktyp;j++)
                     lnktyp[j]=(int) IC_UINT16(M2.D)[j];
                   break;

   case I_UINT32 : for(j=0;j<n_lnktyp;j++)
                     lnktyp[j]=(int) IC_UINT32(M2.D)[j];
                   break;
  }
 }
 else
 {
  for(j=0;j<n_lnktyp;j++)
    lnktyp[j]=(int) ((double *) stk(l2))[j];
 }

 /* build output list */
 CreateVar(3,"l",&n_lnktyp,&n3,&l3);

 for(i=0;i<n_lnktyp;i++)
 {
  nm=lnksz[i]*lnksz[i+n_lnktyp];
  switch (lnktyp[i])
  {
   case 1  : if ((ptr_d=MALLOC(nm*sizeof(double)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_d[j]=0;
             CreateListVarFromPtr(3,i+1,"d",&lnksz[i],&lnksz[i+n_lnktyp], &ptr_d);
             FREE(ptr_d);
             break;

   case 2  : if ((ptr_d=MALLOC(2*nm*sizeof(double)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<2*nm;j++) ptr_d[j]=0;
             ptr_dc = &ptr_d[nm];
             CreateListCVarFromPtr(3,i+1,"d",(j=1,&j),&lnksz[i],&lnksz[i+n_lnktyp],&ptr_d,&ptr_dc);
             FREE(ptr_d);
             break;

   case 3  : if ((ptr_i=MALLOC(nm*sizeof(int)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_i[j]=0;
             M3.m = lnksz[i];
             M3.n = lnksz[i+n_lnktyp];
             M3.it = 4;
             M3.l = -1;
             M3.D = ptr_i;
             CreateListVarFromPtr(3,i+1,"I",&M3.m,&M3.n,&M3);
             FREE(ptr_i);
             break;

   case 4  : if ((ptr_s=MALLOC(nm*sizeof(short)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_s[j]=0;
             M3.m = lnksz[i];
             M3.n = lnksz[i+n_lnktyp];
             M3.it = 2;
             M3.l = -1;
             M3.D = ptr_s;
             CreateListVarFromPtr(3,i+1,"I",&M3.m,&M3.n,&M3);
             FREE(ptr_s);
             break;

   case 5  : if ((ptr_c=MALLOC(nm*sizeof(char)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_c[j]=0;
             M3.m = lnksz[i];
             M3.n = lnksz[i+n_lnktyp];
             M3.it = 1;
             M3.l = -1;
             M3.D = ptr_c;
             CreateListVarFromPtr(3,i+1,"I",&M3.m,&M3.n,&M3);
             FREE(ptr_c);
             break;

   case 6  : if ((ptr_ui=MALLOC(nm*sizeof(unsigned int)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_ui[j]=0;
             M3.m = lnksz[i];
             M3.n = lnksz[i+n_lnktyp];
             M3.it = 14;
             M3.l = -1;
             M3.D = ptr_ui;
             CreateListVarFromPtr(3,i+1,"I",&M3.m,&M3.n,&M3);
             FREE(ptr_ui);
             break;

   case 7  : if ((ptr_us=MALLOC(nm*sizeof(unsigned short)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_us[j]=0;
             M3.m = lnksz[i];
             M3.n = lnksz[i+n_lnktyp];
             M3.it = 12;
             M3.l = -1;
             M3.D = ptr_us;
             CreateListVarFromPtr(3,i+1,"I",&M3.m,&M3.n,&M3);
             FREE(ptr_us);
             break;

   case 8  : if ((ptr_uc=MALLOC(nm*sizeof(unsigned char)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_uc[j]=0;
             M3.m = lnksz[i];
             M3.n = lnksz[i+n_lnktyp];
             M3.it = 11;
             M3.l = -1;
             M3.D = ptr_uc;
             CreateListVarFromPtr(3,i+1,"I",&M3.m,&M3.n,&M3);
             FREE(ptr_uc);
             break;

   default : if ((ptr_d=MALLOC(nm*sizeof(double)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_d[j]=0;
             CreateListVarFromPtr(3,i+1,"d",&lnksz[i],&lnksz[i+n_lnktyp], &ptr_d);
             FREE(ptr_d);
             break;
  }

  if (ierr==-1)
  {
   Scierror(999,"%s : No more free memory.\n",fname);
   FREE(lnksz);
   FREE(lnktyp);
   return 0;
  }
 }

 LhsVar(1)=3;

 FREE(lnksz);
 FREE(lnktyp);
 return 0;
}

/* permutobj : concurrent version
 * of swap_handles for scicos editor.
 *
 * Copyright INRIA
 * A. Layec/S. Mannori - 13/12/06
 */


/* intpermutobj_c
 *
 * -->permutobj(hdl1,hdl2);
 *
 * exchange graphicals objects
 * of two children handles
 *
 * hdl1,hdl2 : valid children handles
 *             in valid parent handle(s)
 *
 * no outputs
 *
 * Alan-02/12/06, initial rev.
 */

int intpermutobj_c(char *fname,unsigned long fname_len)
{
 /*local variables declaration*/
 unsigned long hdl_1, hdl_2;
 sciPointObj *pobj_1, *pparent_1;
 sciPointObj *pobj_2,* pparent_2;
 static int m1, n1, l1;
 static int m2, n2, l2;

 /*check number of rhs parameters*/
 CheckRhs(2,2);

 /*get/check rhs 1*/
 GetRhsVar(1,"h",&m1,&n1,&l1);
 if (m1!=1||n1!=1) {
  Scierror(999,"%s : Bad size "
               "for Rhs(1).\n",fname);
  return 0; }
 hdl_1=(unsigned long)*hstk(l1);
 pobj_1 = sciGetPointerFromHandle(hdl_1);
 if (pobj_1==NULL) {
  Scierror(999,"%s :the handle is not "
               "or no more valid.\n",fname);
  return 0; }
 pparent_1=sciGetParentSubwin(pobj_1);

 /*get/check rhs 2*/
 GetRhsVar(2,"h",&m2,&n2,&l2);
 if (m2!=1||n2!=1) {
  Scierror(999,"%s : Bad size "
               "for Rhs(2).\n",fname);
  return 0; }
 hdl_2=(unsigned long)*hstk(l2);
 pobj_2=sciGetPointerFromHandle(hdl_2);
 if (pobj_2==NULL) {
  Scierror(999,"%s :the handle is not "
               "or no more valid.\n",fname);
  return 0; }
 pparent_2=sciGetParentSubwin(pobj_2);

 /*call permutobj*/
 permutobj(pobj_1,pparent_1,pobj_2,pparent_2);

 /* end */
 return 0;
}
