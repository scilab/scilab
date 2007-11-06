/*
 * int MlistGetFieldNumber(int *ptr, const char *string)
 * int inttimescicos(fname,fname_len)
 * int intduplicate(fname,fname_len)
 * int intdiffobjs(fname,fname_len)
 * int inttree2(fname,fname_len)
 * int inttree3(fname,fname_len)
 * int inttree4 _PARAMS((char *fname,unsigned long fname_len))
 * int intxproperty(fname,fname_len)
 * int intphasesim(fname,fname_len)
 * int intsetxproperty(fname,fname_len)
 * int intsetblockerror(fname,fname_len)
 * void duplicata(n,v,w,ww,nw)
 * void comp_size(v,nw,n)
 * int intcpass2(fname,fname_len) 
 * int intscicosimc(fname,fname_len)
 * int CopyVarFromlistentry(int lw, int *header, int i)
 * int var2sci(void *x,int n,int m,int typ_var)
 * int createblklist(scicos_block *Blocks, int *ierr)
 * int intgetscicosvarsc(fname,fname_len)
 * int intcurblkc(fname,fname_len)
 * int intbuildouttb(fname)
 */

/*     Copyright INRIA */
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "machine.h"
#include "scicos_block4.h"
#include "intcscicos.h"
#include "sci_mem_alloc.h"  /* malloc */
#include "stack-c.h"
#include "sciprint.h"

#ifdef _MSC_VER
extern int ctree2(int* vect,int nb,int* deput,int* depuptr,int* outoin,int* outoinptr, int* ord,int* nord,int* ok);
extern int ctree3(int*vec,int nb,int* depu,int* depuptr,int* typl,int* bexe,int* boptr,int* blnk,int* blptr,int* ord,int* nord,int* ok);
extern int ctree4(int* vec,int nb,int *nd,int nnd,int* typ_r,int* outoin,int* outoinptr,int* r1,int* r2, int* nr);
extern int C2F(scicos)();
extern int C2F(namstr)();
#endif

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

int inttimescicos(fname,fname_len)
     /* renvoi le temps de simulation t=get_scicos_time() */
     char *fname;
     unsigned long fname_len;
{
  int un,l1;
  CheckRhs(-1,0);
  CheckLhs(1,1);
  CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,(un=1,&un),(un=1,&un),&l1);
  *stk(l1)=get_scicos_time();
  LhsVar(1)=Rhs+1;
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
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
  n=m1*n1;
  if (n==0) {
    m3=0;
    CreateVar(Rhs+3,MATRIX_OF_DOUBLE_DATATYPE, &m3, &m3, &l3);
    LhsVar(1) = Rhs+3;
    return 0;
  }
  if (n!=m2*n2) 
    {
      Scierror(999,"%s: 1st and 2nd argument must have equal size\n",fname);
      return 0;
    }
  comp_size(stk(l2),&n3,n);
  m3=1;
  CreateVar(Rhs+3,MATRIX_OF_DOUBLE_DATATYPE, &n3, &m3, &l3);
  duplicata(&n,stk(l1),stk(l2),stk(l3),&n3);
  LhsVar(1) = Rhs+3;
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
  CreateVar(Rhs+3,MATRIX_OF_DOUBLE_DATATYPE,(un=1,&un),(un=1,&un),&l3);
  LhsVar(1) = Rhs+3;
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

  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&nvec,&mvec,&ipvec);
  GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&noin,&moin,&ipoin);
  GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&noinr,&moinr,&ipoinr);
  GetRhsVar(4,MATRIX_OF_INTEGER_DATATYPE,&ndep,&mdep,&ipdep);
  GetRhsVar(5,MATRIX_OF_INTEGER_DATATYPE,&ndepuptr,&mdepuptr,&ipdepuptr);
  n=nvec*mvec;
  CreateVar(Rhs+6,MATRIX_OF_INTEGER_DATATYPE,&n,&un,&ipord);
  CreateVar(Rhs+7,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&ipok);

  ctree2(istk(ipvec),n,istk(ipdep),istk(ipdepuptr),istk(ipoin),istk(ipoinr),
	 istk(ipord),&nord,istk(ipok));
  *istk(iadr(C2F(intersci).iwhere[5])+1)=nord;

  LhsVar(1)=Rhs+6;
  LhsVar(2)=Rhs+7;

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

  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&nvec,&mvec,&ipvec);
  GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&ndep,&mdep,&ipdep);
  GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&ndepuptr,&mdepuptr,&ipdepuptr);
  GetRhsVar(4,MATRIX_OF_INTEGER_DATATYPE,&ntyp,&mtyp,&iptyp);
  GetRhsVar(5,MATRIX_OF_INTEGER_DATATYPE,&nbex,&mbex,&ipbex);
  GetRhsVar(6,MATRIX_OF_INTEGER_DATATYPE,&nbop,&mbop,&ipbop);
  GetRhsVar(7,MATRIX_OF_INTEGER_DATATYPE,&nbln,&mbln,&ipbln);
  GetRhsVar(8,MATRIX_OF_INTEGER_DATATYPE,&nblr,&mblr,&ipblr);
  
  n=nvec*mvec;
  CreateVar(Rhs+9,MATRIX_OF_INTEGER_DATATYPE,&n,&un,&ipord);
  CreateVar(Rhs+10,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&ipok);

  ctree3(istk(ipvec),n,istk(ipdep),istk(ipdepuptr),istk(iptyp),istk(ipbex),
	 istk(ipbop),istk(ipbln),istk(ipblr),istk(ipord),&nord,istk(ipok));
  *istk(iadr(C2F(intersci).iwhere[8])+1)=nord;

  LhsVar(1)=Rhs+9;
  LhsVar(2)=Rhs+10;

  return 0;
}

int inttree4 _PARAMS((char *fname,unsigned long fname_len))
     /* [r1,r2]=ctree4(vec,outoin,outoinptr,nd,ddd) */
{
  int un=1,ipvec,nvec,mvec,noin,moin,ipoin,noinr,moinr,ipoinr;
  int nnd,mnd,ipnd,ntyp,mtyp,iptyp,ipr1,ipr2,n,nr,nn;
  
  CheckRhs(5,5);
  CheckLhs(2,2);

  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&nvec,&mvec,&ipvec);
  GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&noin,&moin,&ipoin);
  GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&noinr,&moinr,&ipoinr);
  GetRhsVar(4,MATRIX_OF_INTEGER_DATATYPE,&nnd,&mnd,&ipnd);
  GetRhsVar(5,MATRIX_OF_INTEGER_DATATYPE,&ntyp,&mtyp,&iptyp);
  n=nvec*mvec;
  nn=nnd*mnd;
  CreateVar(Rhs+6,MATRIX_OF_INTEGER_DATATYPE,&un,&nn,&ipr1);
  CreateVar(Rhs+7,MATRIX_OF_INTEGER_DATATYPE,&un,&nn,&ipr2);

  ctree4(istk(ipvec),n,istk(ipnd),mnd,istk(iptyp),istk(ipoin),
	 istk(ipoinr),istk(ipr1),istk(ipr2),&nr);
  
  LhsVar(1)=Rhs+6;
  LhsVar(2)=Rhs+7;
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
  CheckRhs(-1,0);
  CheckLhs(1,1);
  CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&n_pointer_xproperty,(un=1,&un),&pointer_xproperty);
  LhsVar(1)=Rhs+1;
  return 0;
}
 
int intphasesim(fname,fname_len)
     /* renvoi la phase de simulation phase=get_phase_simulation() */
     char *fname;
     unsigned long fname_len;
{ 
  int un,l1;
  CheckRhs(-1,0);
  CheckLhs(1,1);
  CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,(un=1,&un),(un=1,&un),&l1);
  *istk(l1)=get_phase_simulation();
  LhsVar(1)=Rhs+1;
  return 0;
}

int intsetxproperty(fname,fname_len)
     /* renvoi le type d'equation get_pointer_xproperty() 
      *	(-1: algebriques, +1 differentielles) */
     char *fname;
     unsigned long fname_len;
{
  int un,l1,m1;
  CheckRhs(1,1);
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,(un=1,&un),&l1);
  set_pointer_xproperty(istk(l1));
  LhsVar(1)=0; 
  return 0;
}

int intsetblockerror(fname,fname_len)
     /* renvoi le type d'equation get_pointer_xproperty() 
      *	(-1: algebriques, +1 differentielles) */
     char *fname;
     unsigned long fname_len;
{
  int un,l1;
  CheckRhs(1,1);
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,(un=1,&un),(un=1,&un),&l1);
  set_block_error(*istk(l1));
  LhsVar(1)=0; 
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

/* ******************cpass2 *************************************/
int intcpass2(fname,fname_len) 
     char *fname;
     unsigned long fname_len;
{
  static int l1,l2,m1,m2,m3; 
  static int n1,n2,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n43;
  static int n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30;
  static int minlhs=5, maxlhs=5, minrhs=5, maxrhs=5;
  static int one=1,deux=2;
  static int n33,n34,n35,n36,n37,n38,n1111,n1313,n41,n42,*y40,l40,l39;
  int m31=8,n31=1,l31,n32=8;
  static int *header,*li,*le1,*le11,*le2,*le3,*le4,*le5,*le6,*le7,*le8,*le9;
  static int *le10,*le12,*le13,*header1,*lii,*le14,*le15;
  static int m,me12,ne2,ne3,ne4,ne5,ne6,ne7,ne8,ne9,ne11,ne12,ne71,ne81;
  static double *le66,*le77,*le88,*le1111,*le121,*le22,*le33,*le44,*le55,*le71;
  static double *le81,*le99,*xd0k,*lc1,*rpark,*le1414,*le1515;
  static int *le1010,*le111,*le1313,nc1,mc1,l33,l3,m4,*vecnull;
  static char *Str1[]={ "xcs","x","z","iz","tevts","evtspt","pointi","outtb"};
  double *y1,*y2,*y4,*y17;
  int *y3,*y5,*y6,*y7,*y9,*y10,*y11,*y12,*y13,*y14,*y15,*y16,*y18,*y19,*y20,*y38,*y39;
  int *y21,*y22,*y23,*y24,*y25,*y26,*y27,*y28,*y29,*y30,*y31,*y32,*y33,*y34,*y35,*y41;
  static char *Str2[]={ "scs","funs","xptr","zptr","zcptr","inpptr","outptr","inplnk",
			"outlnk","lnkptr","rpar","rpptr","ipar","ipptr","clkptr","ordptr",
			"execlk","ordclk","cord","oord","zord","critev","nb","ztyp",
			"nblk","ndcblk","subscr","funtyp","iord","labels","modptr"};
  int m33=31,n39=1,l32,n40=31;
  char **y36,**y8,*y37;
  int i,j,k,ok,zeros=0;
  int *bllst2,*bllst3,*bllst4,*bllst5,*bllst12,*bllst9,*nzcross,*nmode;
  int *bllst2ptr,*bllst3ptr,*bllst4ptr,*bllst112,*bllst6ptr,*bllst7ptr;
  int *bllst5ptr,*typ_x,*bllst8ptr,*bllst9ptr;
  int *bllst11ptr,*connectmat,*clkconnect;
  int *corinvec,*corinvptr,*evtspt,pointi,*outtb,*pointiptr;
  int *zcptr,*modptr,*nbptr,*nblkptr,*ndcblkptr; 
  int *inplnk,*outlnk,*lnkptr,*ordptr;
  int *execlk,*ordclk,*cord,*oord,*zord,*iz0,*subscr;
  int *critev,nb,*ztyp,nblk,ndcblk,*iord,solver,*solverptr;
  double *bllst6,*bllst7,*bllst8,*bllst11,*tevts,*xcd0,*ppd;
  char **bllst111,**bllst10,**bllst13;
  /*unsigned long str_len;*/
  int moinsun=-1, field_num;
  char *field_name;

  xcd0=NULL;
  ppd=NULL;
  pointiptr=NULL;
  nbptr=NULL;
  nblkptr=NULL;
  ndcblkptr=NULL;
  /* Check number of inputs (rhs) and outputs (lhs) */
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  
  header= (int*) GetData(1);
  m=header[1];
  if ((bllst111=MALLOC(sizeof(char*)*(m+1))) ==NULL )  return 0;		  
  ((int*)bllst111)[0]=m;
  if ((bllst112=MALLOC(sizeof(int)*(m+1))) ==NULL )  return 0;		  
  bllst112[0]=m;
  if ((bllst2ptr=MALLOC(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst2ptr[0]=m+1;
  bllst2ptr[1]=1;
  if ((bllst2=MALLOC(sizeof(int))) ==NULL )  return 0;		  
  bllst2[0]=0;
  if ((bllst3ptr=MALLOC(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst3ptr[0]=m+1;
  bllst3ptr[1]=1;
  if ((bllst3=MALLOC(sizeof(int))) ==NULL )  return 0;		  
  bllst3[0]=0;
  if ((bllst4ptr=MALLOC(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst4ptr[0]=m+1;
  bllst4ptr[1]=1;
  if ((bllst4=MALLOC(sizeof(int))) ==NULL )  return 0;		  
  bllst4[0]=0;
  if ((bllst5ptr=MALLOC(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst5ptr[0]=m+1;
  bllst5ptr[1]=1;
  if ((bllst5=MALLOC(sizeof(int))) ==NULL )  return 0;		  
  bllst5[0]=0;
  if ((bllst6ptr=MALLOC(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst6ptr[0]=m+1;
  bllst6ptr[1]=1;
  if ((bllst6=MALLOC(sizeof(double))) ==NULL )  return 0;		  
  ((int*) bllst6)[0]=0;
  if ((bllst7ptr=MALLOC(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst7ptr[0]=m+1;
  bllst7ptr[1]=1;
  if ((bllst7=MALLOC(sizeof(double))) ==NULL )  return 0;		  
  ((int*) bllst7)[0]=0;
  if ((bllst8ptr=MALLOC(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst8ptr[0]=m+1;
  bllst8ptr[1]=1;
  if ((bllst8=MALLOC(sizeof(double))) ==NULL )  return 0;		  
  ((int*) bllst8)[0]=0;
  if ((bllst9ptr=MALLOC(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst9ptr[0]=m+1;
  bllst9ptr[1]=1;
  if ((bllst9=MALLOC(sizeof(int))) ==NULL )  return 0;		  
  bllst9[0]=0;
  if ((nmode=MALLOC(sizeof(int)*(m+1))) ==NULL )  return 0;		  
  nmode[0]=m;
  if ((nzcross=MALLOC(sizeof(int)*(m+1))) ==NULL )  return 0;		  
  nzcross[0]=m;  
  if ((bllst10=(char**) MALLOC(sizeof(char *)*(m+1))) ==NULL )  return 0;		  
  ((int*) bllst10)[0]=m;
  if ((bllst11ptr=MALLOC(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst11ptr[0]=m+1;
  bllst11ptr[1]=1;
  if ((bllst11=MALLOC(sizeof(double))) ==NULL )  return 0;		  
  ((int*) bllst11)[0]=0;
  if ((bllst12=MALLOC(sizeof(int)*(m+m+1))) ==NULL )  return 0;		  
  bllst12[0]=m+m;
  if ((bllst13=MALLOC(sizeof(char *)*(m+1))) ==NULL )  return 0;		  
  ((int*) bllst13)[0]=m;
  if ((xcd0=(double*)MALLOC(sizeof(double))) == NULL) return 0;
  ((int*) xcd0)[0]=0;
  if ((typ_x=CALLOC(m+1,sizeof(int))) ==NULL )  return 0;		  
  typ_x[0]=m;
  
  for (k=1; k <= m; k++)
    {
      li=(int*) listentry(header,k); /*le pointeur sur la kieme sous list (mlists)*/
      /* Le rang du champs*/
      /* MlistGetFieldNumber*/
      
      /* 1ier element de la list sim*/      
      if ((field_name=(char*) MALLOC(sizeof(char)*4)) ==NULL )  return 0;
      ((char*) field_name)[3]='\0';
      strcpy (field_name,"sim");
      field_num=MlistGetFieldNumber(li,field_name);
      le1=(int*) listentry(li,field_num);
      FREE(field_name);
      /*si sim est une list*/
      if (le1[0] == 15)
	{
	  le11=(int*) listentry(le1,1); /*sim(1)*/
	  if (le11[0] == 13)
	    {
	      if ((bllst111[k]=(char*) MALLOC(sizeof(char)*9)) ==NULL )  return 0;
	      ((char*) bllst111[k])[8]='\0';
	      strcpy (bllst111[k],"sciblock");
	      bllst112[k]=3;
	    }
	  else
	    {
	      le111=((int *) (le11+6));
	      n1111=le11[5]-1;
	      if ((bllst111[k]=(char*) MALLOC(sizeof(char)*(n1111+1))) ==NULL )  return 0;
	      ((char*) bllst111[k])[n1111]='\0';
	      C2F(cvstr)(&n1111,le111,bllst111[k],&one,(unsigned long)strlen(bllst111[k]));
	      
	      le12=(int*) listentry(le1,2); /*sim(2)*/
	      le121=((double *) (le12+4));
	      bllst112[k]=(int)le121[0];
	    }
	}
      else
	{
	  le111=((int *) (le1+6));
	  bllst112[k]=0;
	  n1111=le1[5]-1;
	  if ((bllst111[k]=(char*) MALLOC(sizeof(char)*(n1111+1))) ==NULL )  return 0;
	  ((char*) bllst111[k])[n1111]='\0';
	  C2F(cvstr)(&n1111,le111,bllst111[k],&one,(unsigned long)strlen(bllst111[k]));

	}
      /* 2ieme element de la list in */
      if ((field_name=(char*) MALLOC(sizeof(char)*3)) ==NULL )  return 0;
      ((char*) field_name)[2]='\0';
      strcpy (field_name,"in");
      field_num=MlistGetFieldNumber(li,field_name);
      FREE(field_name);
      le2=(int*) listentry(li,field_num);
      ne2=le2[1];
      le22=((double *) (le2+4));
      bllst2ptr[k+1]=bllst2ptr[k]+ne2;
      if ((bllst2=REALLOC(bllst2,sizeof(int)*(bllst2[0]+ne2+1))) ==NULL )  return 0;	
      for (j=0; j< ne2; j++)
	{
	  bllst2[bllst2[0]+j+1]=(int)le22[j];
	}
      bllst2[0]=bllst2[0]+ne2;
      /* 3ieme element de la list out*/
      if ((field_name=(char*) MALLOC(sizeof(char)*4)) ==NULL )  return 0;
      ((char*) field_name)[3]='\0';
      strcpy (field_name,"out");
      field_num=MlistGetFieldNumber(li,field_name);
      FREE(field_name);
      le3=(int*) listentry(li,field_num);
      ne3=le3[1];
      le33=((double *) (le3+4));
      bllst3ptr[k+1]=bllst3ptr[k]+ne3;
      if ((bllst3=REALLOC(bllst3,sizeof(int)*(bllst3[0]+ne3+1))) ==NULL )  return 0;
      for (j=0; j< ne3; j++)
	{
	  bllst3[bllst3[0]+j+1]=(int)le33[j];
	}
      bllst3[0]=bllst3[0]+ne3;
      /* 4ieme element de la list evtin*/
      if ((field_name=(char*) MALLOC(sizeof(char)*6)) ==NULL )  return 0;
      ((char*) field_name)[5]='\0';
      strcpy (field_name,"evtin");
      field_num=MlistGetFieldNumber(li,field_name);
      FREE(field_name);
      le4=(int*) listentry(li,field_num);
      ne4=le4[1];
      le44=((double *) (le4+4));
      bllst4ptr[k+1]=bllst4ptr[k]+ne4;
      if ((bllst4=REALLOC(bllst4,sizeof(int)*(bllst4[0]+ne4+1))) ==NULL )  return 0;
      for (j=0; j< ne4; j++)
	{
	  bllst4[bllst4[0]+j+1]=(int)le44[j];
	}
      bllst4[0]=bllst4[0]+ne4;
      /* 5ieme element de la list evtout*/
      if ((field_name=(char*) MALLOC(sizeof(char)*7)) ==NULL )  return 0;
      ((char*) field_name)[6]='\0';
      strcpy (field_name,"evtout");
      field_num=MlistGetFieldNumber(li,field_name);
      FREE(field_name);
      le5=(int*) listentry(li,field_num);
      ne5=le5[1];
      le55=((double *) (le5+4));
      bllst5ptr[k+1]=bllst5ptr[k]+ne5;
      if ((bllst5=REALLOC(bllst5,sizeof(int)*(bllst5[0]+ne5+1))) ==NULL )  return 0;
      for (j=0; j< ne5; j++)
	{
	  bllst5[bllst5[0]+j+1]=(int)le55[j];
	}
      bllst5[0]=bllst5[0]+ne5;
      /* 6ieme element de la list state*/
      if ((field_name=(char*) MALLOC(sizeof(char)*6)) ==NULL )  return 0;
      ((char*) field_name)[5]='\0';
      strcpy (field_name,"state");
      field_num=MlistGetFieldNumber(li,field_name);
      FREE(field_name);
      le6=(int*) listentry(li,field_num);
      ne6=le6[1];
      le66=((double *) (le6+4));
      if (bllst112[k]<10000)
        {
	  if ((xcd0=(double*)REALLOC(xcd0,sizeof(double)*(((int*) xcd0)[0]+ne6+1))) == NULL ) return 0;
	  if ((bllst6=(double*)REALLOC(bllst6,sizeof(double)*(((int *) bllst6)[0]+ne6+1))) ==NULL )  return 0;
	  for (j=0;j<ne6;j++)
            {
              xcd0[j+((int*) xcd0)[0]+1]=0;
              bllst6[((int *) bllst6)[0]+j+1]=le66[j];
            }  
	  ((int*) xcd0)[0]=((int*) xcd0)[0]+ne6; 
	  ((int *) bllst6)[0]=((int *) bllst6)[0]+ne6;
	  bllst6ptr[k+1]=bllst6ptr[k]+ne6;
	}
      else
	{
	  if ((xcd0=(double*)REALLOC(xcd0,sizeof(double)*(((int*) xcd0)[0]+ne6/2+1))) == NULL ) return 0;
	  if ((bllst6=(double*)REALLOC(bllst6,sizeof(double)*(((int *) bllst6)[0]+ne6/2+1))) ==NULL )  return 0;
	  for (j=0; j < ne6/2; j++)
            {
              xcd0[j+1+((int*) xcd0)[0]]=le66[j+ne6/2];
	      bllst6[((int *) bllst6)[0]+j+1]=le66[j];
            }
	  ((int*) xcd0)[0]=((int*) xcd0)[0]+ne6/2;                          
	  ((int *) bllst6)[0]=((int *) bllst6)[0]+ne6/2;
	  bllst6ptr[k+1]=bllst6ptr[k]+ne6/2;
	}
      if ( ne6 != 0 ) typ_x[k]=1;
      /* 7ieme element de la list dstate et le 8ieme element de la list rpar*/
      if ((field_name=(char*) MALLOC(sizeof(char)*7)) ==NULL )  return 0;
      ((char*) field_name)[6]='\0';
      strcpy (field_name,"dstate");
      field_num=MlistGetFieldNumber(li,field_name);
      FREE(field_name);
      le71=(double *) listentry(li,field_num);      
      le7=(int*) listentry(li,field_num);
      ne7= le7[1];
      le77=((double *) (le7+4));

      if ((field_name=(char*) MALLOC(sizeof(char)*5)) ==NULL )  return 0;
      ((char*) field_name)[4]='\0';
      strcpy (field_name,"rpar");
      field_num=MlistGetFieldNumber(li,field_name);
      FREE(field_name);
      le81=(double *) listentry(li,field_num);
      le8=(int*) listentry(li,field_num);
      ne8=le8[1];
      le88=((double *) (le8+4));
      if ( bllst112[k] == 3 || bllst112[k] == 5 || bllst112[k] == 10005)
	{
	  if (ne7 != 0)
	    {
	      ne71=li[8+2]-li[8+1];
	      if ((xd0k=(double*)MALLOC(sizeof(double)*(ne71+1))) ==NULL )  return 0;
	      ((int *) xd0k)[0]=ne71;

	      C2F(unsfdcopy)(&ne71,le71,&moinsun,(xd0k+1),&moinsun);
	      bllst7ptr[k+1]=bllst7ptr[k]+ne71;
	      if ((bllst7=REALLOC(bllst7,sizeof(double)*(((int *) bllst7)[0]+ne71+1))) ==NULL )  return 0;
	      for (j=1; j< ne71+1; j++)
		{
		  bllst7[((int *) bllst7)[0]+j]=xd0k[j];
		}
	      ((int *) bllst7)[0]=((int *) bllst7)[0]+ne71;
	      FREE(xd0k);
	    }
	  else
	    {
	      bllst7ptr[k+1]=bllst7ptr[k];
	    }
	  if (ne8 != 0)
	    {
	      ne81=li[9+2]-li[9+1];
	      if ((rpark=(double*)MALLOC(sizeof(double)*(ne81+1))) ==NULL )  return 0;
	      ((int *) rpark)[0]=ne81;
	      C2F(unsfdcopy)(&ne81,le81,&moinsun,(rpark+1),&moinsun);

	      bllst8ptr[k+1]=bllst8ptr[k]+ne81;
	      if ((bllst8=REALLOC(bllst8,sizeof(double)*(((int *) bllst8)[0]+ne81+1))) ==NULL )  return 0;
	      for (j=1; j< ne81+1; j++)
		{
		  bllst8[((int *) bllst8)[0]+j]=rpark[j];
		}
	      ((int *) bllst8)[0]=((int *) bllst8)[0]+ne81;
	      FREE(rpark);
	    }
	  else
	    {
	      bllst8ptr[k+1]=bllst8ptr[k];
	    }
	}
      else
	{
	  bllst7ptr[k+1]=bllst7ptr[k]+ne7;
	  if ((bllst7=REALLOC(bllst7,sizeof(double)*(((int *) bllst7)[0]+ne7+1))) ==NULL )  return 0;
	  for (j=0; j< ne7; j++)
	    {
	      bllst7[((int *) bllst7)[0]+j+1]=le77[j];
	    }
	  ((int *) bllst7)[0]=((int *) bllst7)[0]+ne7;

	  bllst8ptr[k+1]=bllst8ptr[k]+ne8;
	  if ((bllst8=REALLOC(bllst8,sizeof(double)*(((int *) bllst8)[0]+ne8+1))) ==NULL )  return 0;
	  for (j=0; j< ne8; j++)
	    {
	      bllst8[((int *) bllst8)[0]+j+1]=le88[j];
	    }
	  ((int *) bllst8)[0]=((int *) bllst8)[0]+ne8;
	}
      /* 9ieme element de la list ipar*/
      if ((field_name=(char*) MALLOC(sizeof(char)*5)) ==NULL )  return 0;
      ((char*) field_name)[4]='\0';
      strcpy (field_name,"ipar");
      field_num=MlistGetFieldNumber(li,field_name);
      FREE(field_name);
      le9=(int*) listentry(li,field_num);
      if (le9[0] == 1)
	{
	  ne9=le9[1];
	  le99=((double *) (le9+4));
	  bllst9ptr[k+1]=bllst9ptr[k]+ne9;
	  if ((bllst9=REALLOC(bllst9,sizeof(int)*(bllst9[0]+ne9+1))) ==NULL )  return 0;
	  for (j=0; j< ne9; j++)
	    {
	      bllst9[bllst9[0]+j+1]=(int)le99[j];
	    }
	  bllst9[0]=bllst9[0]+ne9;
	}
      else 
	{
	  bllst9ptr[k+1]=bllst9ptr[k];
	}
      /* 10ieme element de la list typeblock*/
      if ((field_name=(char*) MALLOC(sizeof(char)*10)) ==NULL )  return 0;
      ((char*) field_name)[9]='\0';
      strcpy (field_name,"blocktype");
      field_num=MlistGetFieldNumber(li,field_name);
      FREE(field_name);
      le10=(int*) listentry(li,field_num);
      le1010=((int *) (le10+6));      
      if ((bllst10[k]=(char*) MALLOC(sizeof(char)*2)) ==NULL )  return 0;
      ((char*) bllst10[k])[1]='\0';
      C2F(cvstr)(&one,le1010,bllst10[k],&one,(unsigned long)strlen(bllst10[k]));
      /* 11ieme element de la list firing*/
      if ((field_name=(char*) MALLOC(sizeof(char)*7)) ==NULL )  return 0;
      ((char*) field_name)[6]='\0';
      strcpy (field_name,"firing");
      field_num=MlistGetFieldNumber(li,field_name);
      FREE(field_name);
      le11=(int*) listentry(li,field_num);
      ne11=le11[2];
      le1111=((double *) (le11+4));
      bllst11ptr[k+1]=bllst11ptr[k]+ne11;
      if ((bllst11=REALLOC(bllst11,sizeof(double)*(((int *) bllst11)[0]+ne11+1))) ==NULL )  return 0;
      for (j=0; j< ne11; j++)
	{
	  bllst11[((int *) bllst11)[0]+j+1]=le1111[j];
	}
      ((int *) bllst11)[0]=((int *) bllst11)[0]+ne11;
      /* 12ieme element de la list dep_ut*/
      if ((field_name=(char*) MALLOC(sizeof(char)*7)) ==NULL )  return 0;
      ((char*) field_name)[6]='\0';
      strcpy (field_name,"dep_ut");
      field_num=MlistGetFieldNumber(li,field_name);
      FREE(field_name);
      le12=(int*) listentry(li,field_num);
      ne12=le12[1];
      me12=le12[2];
      /*le1212=((double *) (le12+3));*/
      bllst12[k]=le12[3];
      bllst12[k+m]=le12[4];
      /* 13ieme element de la list label*/
      if ((field_name=(char*) MALLOC(sizeof(char)*6)) ==NULL )  return 0;
      ((char*) field_name)[5]='\0';
      strcpy (field_name,"label");
      field_num=MlistGetFieldNumber(li,field_name);
      FREE(field_name);
      le13=(int*) listentry(li,field_num);
      le1313=((int *) (le13+6));

      n1313=le13[5]-1;
      if ((bllst13[k]=(char*) MALLOC(sizeof(char)*(n1313+1))) ==NULL )  return 0;
      ((char*) bllst13[k])[n1313]='\0'; 
      C2F(cvstr)(&n1313,le1313,bllst13[k],&one,(unsigned long)strlen(bllst13[k]));
     /* 14ieme element de la list nzcross*/
      if ((field_name=(char*) MALLOC(sizeof(char)*8)) ==NULL )  return 0;
      ((char*) field_name)[7]='\0';
      strcpy (field_name,"nzcross");
      field_num=MlistGetFieldNumber(li,field_name);
      FREE(field_name);
      le14=(int*) listentry(li,field_num);
      le1414=((double *) (le14+4));
      nzcross[k]=(int)le1414[0];
     /* 15ieme element de la list nmode*/
      if ((field_name=(char*) MALLOC(sizeof(char)*6)) ==NULL )  return 0;
      ((char*) field_name)[5]='\0';
      strcpy (field_name,"nmode");
      field_num=MlistGetFieldNumber(li,field_name);
      FREE(field_name);
      le15=(int*) listentry(li,field_num);
      le1515=((double *) (le15+4));
      nmode[k]=(int)le1515[0];
    }
  
  GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);
  GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE, &m2, &n2, &l2);
  if ((connectmat=MALLOC((m1*n1+1)*sizeof(int))) ==NULL )  return 0;		  
  connectmat[0]=m1*n1;
  for(i=0;i<n1;i++)
    {
      for(j=1;j<m1+1;j++)
	{
	  connectmat[j+i*m1]=*istk(l1+j+i*m1-1);
	}
    }
  if ((clkconnect=MALLOC((m2*n2+1)*sizeof(int))) ==NULL )  return 0;		  
  clkconnect[0]=m2*n2;
  for(i=0;i<n2;i++)
    {
      for(j=1;j<m2+1;j++)
	{
	  clkconnect[j+i*m2]=*istk(l2+j+i*m2-1);
	}
    }
 
  if ((corinvec=MALLOC(sizeof(int))) ==NULL )  return 0;
  corinvec[0]=0;
  header1= (int*) GetData(4);
  m3=header1[1];
  if ((corinvptr=MALLOC(sizeof(int)*(m3+2))) ==NULL )  return 0;
  corinvptr[0]=m3+1;
  corinvptr[1]=1;
  for (k=1; k <= m3; k++)
    {
      lii=(int*) listentry(header1,k); /*le pointeur sur la kieme vecteur*/
      lc1=((double *) (lii+4));
      nc1=lii[1];
      mc1=lii[2];
      if ((corinvec=REALLOC(corinvec,sizeof(int)*(corinvec[0]+nc1*mc1+1))) ==NULL )  return 0;
      for (j=0; j< nc1*mc1; j++)
	{
	  corinvec[corinvec[0]+j+1]=(int)lc1[j];
	}
      corinvec[0]=corinvec[0]+nc1*mc1;
      corinvptr[k+1]=corinvptr[k]+nc1*mc1;

    }
  
  GetRhsVar(5,MATRIX_OF_INTEGER_DATATYPE, &one, &m4, &l3); 
  solver=*istk(l3);
  
  cpass2(&bllst111,&bllst112,&bllst2,&bllst3,&bllst4,&bllst5,&bllst9,&bllst10,&bllst11,
	 &bllst12,&bllst13,&nmode,&nzcross,&bllst2ptr,&bllst3ptr,&bllst4ptr,&bllst5ptr,&bllst6ptr,&bllst9ptr,&typ_x,
	 &bllst11ptr,&connectmat,&clkconnect,&corinvec,&corinvptr,
	 &iz0,&tevts,&evtspt,&pointi,&outtb,&zcptr,&modptr,&outlnk,&inplnk,
	 &lnkptr,&ordptr,&execlk,&ordclk,&cord,&oord,&zord,&critev,&nb,&ztyp,
	 &nblk,&ndcblk,&subscr,&iord,&ok);
  if (!ok) 
    {
      CreateVar(Rhs+11,MATRIX_OF_INTEGER_DATATYPE, &one, &zeros, &l31);
      CreateVar(Rhs+12,MATRIX_OF_INTEGER_DATATYPE, &one, &zeros, &l32);
      CreateVar(Rhs+13,MATRIX_OF_INTEGER_DATATYPE, &one, &zeros, &l33);
      CreateVar(Rhs+14,MATRIX_OF_INTEGER_DATATYPE, &one, &zeros, &l39);
      CreateVar(Rhs+15,MATRIX_OF_INTEGER_DATATYPE, &one, &zeros, &l40);
      LhsVar(1) = Rhs+11;
      LhsVar(2) = Rhs+12;
      LhsVar(3) = Rhs+13;
      LhsVar(4) = Rhs+14;
      LhsVar(5) = Rhs+15;
      return 0;
    }
  if (nb > nblk)
    {
      if ((bllst8ptr=REALLOC(bllst8ptr,sizeof(int)*(nb+2))) ==NULL )  return 0;		  
      bllst8ptr[0]=nb+1;
      if ((bllst7ptr=REALLOC(bllst7ptr,sizeof(int)*(nb+2))) ==NULL )  return 0;		  
      bllst7ptr[0]=nb+1;
      if ((bllst6ptr=REALLOC(bllst6ptr,sizeof(int)*(nb+2))) ==NULL )  return 0;		  
      bllst6ptr[0]=nb+1;
      for (i=nblk+1; i<nb+1; i++)
	{

	  bllst8ptr[i+1]=bllst8ptr[i];
	  bllst7ptr[i+1]=bllst7ptr[i];
	  bllst6ptr[i+1]=bllst6ptr[i];
	}
    }
  if ((Max1(bllst112)>10000) && (solver==0))
    {
      Message("Diagram contains implicit blocks, compiling for implicit Solver");
      solver=100;
    }
  if (solver==100)
    {
      if ((bllst6=(double*)REALLOC(bllst6,sizeof(double)*(((int*) bllst6)[0]+((int*) xcd0)[0]+1))) == NULL ) return 0;
      ppd=(double *)&bllst6[1+((int*) bllst6)[0]];
      ppd=memcpy(ppd,&xcd0[1],sizeof(double)*(((int*) xcd0)[0]));
      ((int*) bllst6)[0]=((int*) bllst6)[0]+((int*) xcd0)[0];
    }
  if (xcd0) FREE(xcd0);
  /******************* state **************************/
  
  if ((pointiptr=MALLOC(sizeof(int))) ==NULL )  return 0;		  
  pointiptr[0]=pointi;
  y1 = (double*) (bllst6+1);
  y2 = (double*) (bllst7+1);
  n4=((int*) bllst6)[0];
  n5=((int*) bllst7)[0];
  if (!iz0)
    {
      if ((iz0=MALLOC(sizeof(int))) ==NULL )  return 0;		  
      iz0[0]=0;
    }
  y3=(int*) (iz0+1);
  n6=iz0[0];
  y4 = (double*) (tevts+1);
  y5=(int*) (evtspt+1);
  y6=pointiptr;
  y7=(int*) (outtb+1);
  n7=(int) tevts[0];
  n8= evtspt[0];
  n9= outtb[0];
  CreateVar(6,TYPED_LIST_DATATYPE, &m31, &n31, &l31);
  CreateListVarFromPtr(6,1,MATRIX_OF_STRING_DATATYPE, &one, &n32, Str1);
  CreateListVarFromPtr(6,2,MATRIX_OF_DOUBLE_DATATYPE, &n4, &one,  &y1);
  CreateListVarFromPtr(6,3,MATRIX_OF_DOUBLE_DATATYPE, &n5, &one,  &y2);
  CreateListVarFromPtr(6,4,MATRIX_OF_INTEGER_DATATYPE, &n6, &one,  &y3);
  CreateListVarFromPtr(6,5,MATRIX_OF_DOUBLE_DATATYPE, &n7, &one,  &y4);
  CreateListVarFromPtr(6,6,MATRIX_OF_INTEGER_DATATYPE, &n8, &one,  &y5);
  CreateListVarFromPtr(6,7,MATRIX_OF_INTEGER_DATATYPE, &one, &one, &y6);
  CreateListVarFromPtr(6,8,MATRIX_OF_INTEGER_DATATYPE, &n9, &one,  &y7);
  LhsVar(1) = 6;
  FREE(iz0);
  FREE(tevts);
  FREE(evtspt);
  FREE(outtb);
  /********************** sim *****************/
  CreateVar(7,TYPED_LIST_DATATYPE, &m33, &n39, &l32);
  CreateListVarFromPtr(7,1,MATRIX_OF_STRING_DATATYPE, &one, &n40, Str2);
  y8=(char**) (bllst111+1);
  n10=(int) bllst111[0];
  y9=(int*) (bllst6ptr+1);
  n11=bllst6ptr[0];
  y10=(int*) (bllst7ptr+1);
  n12=bllst7ptr[0];
  y11=(int*) (zcptr+1);
  n13=zcptr[0];
  y41=(int*) (modptr+1);
  n43=modptr[0];
  y12=(int*) (bllst2ptr+1);
  n14=bllst2ptr[0];
  y13=(int*) (bllst3ptr+1);
  n15=bllst3ptr[0];
  y14=(int*) (inplnk+1);
  n16=inplnk[0];
  y15=(int*) (outlnk+1);
  n17=outlnk[0];
  y16=(int*) (lnkptr+1);
  n18=lnkptr[0];
  y17=(double*) (bllst8+1);
  n19=((int*) bllst8)[0];
  y18=(int*) (bllst8ptr+1);
  n20=bllst8ptr[0];
  y19=(int*) (bllst9+1);
  n21=bllst9[0];
  y20=(int*) (bllst9ptr+1);
  n22=bllst9ptr[0];
  y21=(int*) (bllst5ptr+1);
  n23=bllst5ptr[0];
  y22=(int*) (ordptr+1);
  n24=ordptr[0];
  y23=(int*) (execlk+1);
  n25=execlk[0]/2;
  y24=(int*) (ordclk+1);
  n26=ordclk[0]/2;
  if (!cord)
    {
      if ((cord=MALLOC(sizeof(int))) ==NULL )  return 0;		  
      cord[0]=0;
    }
  y25=(int*) (cord+1);
  n27=cord[0]/2;
  if (!oord)
    {
      if ((oord=MALLOC(sizeof(int))) ==NULL )  return 0;		  
      oord[0]=0;
    }
  y26=(int*) (oord+1);
  n28=oord[0]/2;
  if (!zord)
    {
      if ((zord=MALLOC(sizeof(int))) ==NULL )  return 0;		  
      zord[0]=0;
    }
  y27=(int*) (zord+1);
  n29=zord[0]/2;
  y28=(int*) (critev+1);
  n30=critev[0];
  if ((nbptr=MALLOC(sizeof(int))) ==NULL )  return 0;		  
  nbptr[0]=nb;
  y29=nbptr;
  y30=(int*) (ztyp+1);
  n33=ztyp[0];
  if ((nblkptr=MALLOC(sizeof(int))) ==NULL )  return 0;		  
  nblkptr[0]=nblk;
  y31=nblkptr;
  if ((ndcblkptr=MALLOC(sizeof(int))) ==NULL )  return 0;		  
  ndcblkptr[0]=ndcblk;
  y32=ndcblkptr;
  if (!subscr)
    {
      if ((subscr=MALLOC(sizeof(int))) ==NULL )  return 0;		  
      subscr[0]=0;
    }
  y33=(int*) (subscr+1);
  n34=subscr[0]/2;
  y34=(int*) (bllst112+1);
  n35=bllst112[0];
  if (!iord)
    {
      if ((iord=MALLOC(sizeof(int))) ==NULL )  return 0;		  
      iord[0]=0;
    }
  y35=(int*) (iord+1);
  n36=iord[0]/2;
  y36=(char**) (bllst13+1);
  n37=(int) bllst13[0];
  if ((vecnull=MALLOC(sizeof(int))) ==NULL )  return 0;		  
  y38=(int*) (vecnull+1);
  n41=0;
  
  CreateListVarFromPtr(7,2,MATRIX_OF_INTEGER_DATATYPE, &n41, &one, &y38);
  CreateListVarFromPtr(7,3,MATRIX_OF_INTEGER_DATATYPE, &n11, &one, &y9);
  CreateListVarFromPtr(7,4,MATRIX_OF_INTEGER_DATATYPE, &n12,&one,  &y10);
  CreateListVarFromPtr(7,5,MATRIX_OF_INTEGER_DATATYPE, &n13, &one, &y11);
  CreateListVarFromPtr(7,6,MATRIX_OF_INTEGER_DATATYPE, &n14,&one,  &y12);
  CreateListVarFromPtr(7,7,MATRIX_OF_INTEGER_DATATYPE, &n15,&one,  &y13);
  CreateListVarFromPtr(7,8,MATRIX_OF_INTEGER_DATATYPE, &n16, &one, &y14);
  CreateListVarFromPtr(7,9,MATRIX_OF_INTEGER_DATATYPE, &n17, &one, &y15);
  CreateListVarFromPtr(7,10,MATRIX_OF_INTEGER_DATATYPE, &n18, &one, &y16);
  CreateListVarFromPtr(7,11,MATRIX_OF_DOUBLE_DATATYPE, &n19,&one,  &y17);
  CreateListVarFromPtr(7,12,MATRIX_OF_INTEGER_DATATYPE, &n20, &one, &y18);
  CreateListVarFromPtr(7,13,MATRIX_OF_INTEGER_DATATYPE, &n21, &one, &y19);
  CreateListVarFromPtr(7,14,MATRIX_OF_INTEGER_DATATYPE, &n22, &one, &y20);
  CreateListVarFromPtr(7,15,MATRIX_OF_INTEGER_DATATYPE, &n23, &one, &y21);
  CreateListVarFromPtr(7,16,MATRIX_OF_INTEGER_DATATYPE, &n24, &one, &y22);
  CreateListVarFromPtr(7,17,MATRIX_OF_INTEGER_DATATYPE, &n25, &deux, &y23);
  CreateListVarFromPtr(7,18,MATRIX_OF_INTEGER_DATATYPE, &n26, &deux, &y24);
  CreateListVarFromPtr(7,19,MATRIX_OF_INTEGER_DATATYPE, &n27, &deux, &y25);
  CreateListVarFromPtr(7,20,MATRIX_OF_INTEGER_DATATYPE, &n28, &deux,&y26);
  CreateListVarFromPtr(7,21,MATRIX_OF_INTEGER_DATATYPE, &n29, &deux, &y27);
  CreateListVarFromPtr(7,22,MATRIX_OF_INTEGER_DATATYPE, &n30, &one, &y28);
  CreateListVarFromPtr(7,23,MATRIX_OF_INTEGER_DATATYPE, &one, &one, &y29);
  CreateListVarFromPtr(7,24,MATRIX_OF_INTEGER_DATATYPE, &n33, &one, &y30);
  CreateListVarFromPtr(7,25,MATRIX_OF_INTEGER_DATATYPE, &one, &one, &y31);
  CreateListVarFromPtr(7,26,MATRIX_OF_INTEGER_DATATYPE, &one, &one, &y32);
  CreateListVarFromPtr(7,27,MATRIX_OF_INTEGER_DATATYPE, &n34, &one, &y33);
  CreateListVarFromPtr(7,28,MATRIX_OF_INTEGER_DATATYPE, &n35,&one,  &y34);
  CreateListVarFromPtr(7,29,MATRIX_OF_INTEGER_DATATYPE, &n36, &deux, &y35);
  CreateListVarFromPtr(7,30,MATRIX_OF_STRING_DATATYPE, &n37, &one, y36);
  CreateListVarFromPtr(7,31,MATRIX_OF_INTEGER_DATATYPE, &n43, &one, &y41);
  LhsVar(2) = 7;
  
  FREE(zcptr);
  FREE(modptr);
  FREE(inplnk);
  FREE(outlnk);
  FREE(lnkptr);
  FREE(ordptr);
  FREE(execlk);
  FREE(ordclk);
  FREE(cord);
  FREE(oord);
  FREE(zord);
  FREE(critev);
  FREE(ztyp);
  FREE(subscr);
  FREE(iord);
  FREE(bllst112);
  FREE(bllst2);
  FREE(bllst2ptr);
  FREE(bllst3);
  FREE(bllst3ptr);
  FREE(bllst4);
  FREE(bllst4ptr);
  FREE(bllst5);
  FREE(bllst5ptr);
  FREE(bllst6);
  FREE(bllst6ptr);
  FREE(bllst7);
  FREE(bllst7ptr);
  FREE(bllst8);
  FREE(bllst8ptr);
  FREE(bllst9);
  FREE(bllst9ptr);
  for(i = 1; i < ((int*) bllst10)[0]+1; i++)
    FREE(bllst10[i]);
  FREE(bllst10);
  FREE(bllst11);
  FREE(bllst11ptr);
  FREE(bllst12);
  for(i = 1; i < ((int*) bllst13)[0]+1; i++)
    FREE(bllst13[i]);
  FREE(bllst13);  
  FREE(nzcross);
  FREE(nmode);
  if(ndcblkptr) FREE(ndcblkptr);
  if (pointiptr) FREE(pointiptr);
  if (nbptr) FREE(nbptr);
  if (nblkptr) FREE(nblkptr);
  FREE(connectmat); 
  FREE(vecnull);

  CreateVar(8,LIST_DATATYPE, &n10, &one, &l33);
  for (i=0; i < n10; i++)
    {
      n38=(int)strlen(y8[i]);
      y37=*(y8+i);
      CreateListVarFromPtr(8,i+1,STRING_DATATYPE, &n38, &one, &y37); 
    }
  LhsVar(3) = 8;
  for (i = 1; i <= nb; i++)
    FREE(bllst111[i]);
  FREE(bllst111);
  
  if ((solverptr=MALLOC(sizeof(int))) ==NULL )  return 0;		  
  solverptr[0]=solver;
  y39=solverptr;
  CreateVarFromPtr(Rhs+9,MATRIX_OF_INTEGER_DATATYPE, &one, &one, &y39);
  LhsVar(4) = Rhs+9;
  FREE(solverptr);

  CreateVar(Rhs+10,LIST_DATATYPE, &nb, &one, &l40);
  for (i=1; i < nb+1; i++)
    {      
      y40=(int*) (corinvec+corinvptr[i]);
      n42=corinvptr[i+1]-corinvptr[i];
      CreateListVarFromPtr(10,i,MATRIX_OF_INTEGER_DATATYPE, &one, &n42, &y40); 
    }
  LhsVar(5) = Rhs+10;
  FREE(corinvec);
  FREE(corinvptr);
  return 0;
}

/* intsicosimc scicosim interface routine.
 *
 * [state,t] = scicosim(state,tcur,tf,sim,str,tol)
 *
 * rhs 1 state : Tlist
 *        - 1  : state(1)     : !xcs  x  z  iz  tevts  evtspt  pointi  outtb  !
 *        - 2  : state.x      : column vector of real
 *        - 3  : state.z      : column vector of real
 *        - 4  : state.iz     : column vector of real (empty object with flag "finish")
 *        - 5  : state.tevts  : column vector of real
 *        - 6  : state.evtspt : column vector of int32
 *        - 7  : state.pointi : int32 scalar
 *        - 8  : state.outtb  : list of scilab object
 * rhs 2 tcur  : real scalar
 * rhs 3 tf    : real scalar
 * rhs 4 sim   : Tlist
 *        - 1  : sim(1) : !scs     funs    xptr    zptr    zcptr   inpptr
 *                         outptr  inplnk  outlnk  rpar    rpptr   ipar
 *                         ipptr   clkptr  ordptr  execlk  ordclk  cord
 *                         oord    zord    critev  nb      ztyp    nblk
 *                         ndcblk  subscr  funtyp  iord    labels  modptr  !
 *        - 2  : sim.funs   : list of strings and/or scilab function
 *        - 3  : sim.xptr   : column vector of int32
 *        - 4  : sim.zptr   : column vector of int32
 *        - 5  : sim.zcptr  : column vector of int32
 *        - 6  : sim.inpptr : column vector of int32
 *        - 7  : sim.outptr : column vector of int32
 *        - 8  : sim.inplnk : column vector of int32
 *        - 9  : sim.outlnk : column vector of int32
 *        - 10 : sim.rpar   : column vector of real
 *        - 11 : sim.rpptr  : column vector of int32
 *        - 12 : sim.ipar   : column vector of int32
 *        - 13 : sim.ipptr  : column vector of int32
 *        - 14 : sim.clkptr : column vector of int32
 *        - 15 : sim.ordptr : column vector of int32
 *        - 16 : sim.execlk : matrix of int32
 *        - 17 : sim.ordclk : matrix of int32
 *        - 18 : sim.cord   : matrix of int32
 *        - 19 : sim.oord   : matrix of int32
 *        - 20 : sim.zord   : column vector ? of int32
 *        - 21 : sim.critev : column vector of int32
 *        - 22 : sim.nb     : int32 scalar
 *        - 23 : sim.ztyp   : column vector of int32
 *        - 24 : sim.nblk   : int32 scalar
 *        - 25 : sim.ndcblk : int32 scalar
 *        - 26 : sim.subscr : column vector of int32
 *        - 27 : sim.funtyp : column vector of int32
 *        - 28 : sim.iord   : column vector of int32
 *        - 29 : sim.labels : column vector of strings
 *        - 30 : sim.modptr : column vector of int32
 *
 * rhs 5 str   : string flag : 'start','run','finish','linear'
 * rhs 6 tol   : real vector of size (7,1) minimum (4,1)
 *               [atol rtol ttol [deltat realtimescale solver hmax]]'
 *
 * 16/03/06, A.Layec : Rewritten from original fortran
 * source code intsscicos in intcos.f.
 *
 * 29/03/06, Alan    : Improvement in accordance to c_pass2
 * (int32 parameters)
 *
 * 31/05/06, Alan    : Add global variable int *il_state_save
 * and int *il_sim_save in intcscicos.h to store stack address
 * of list %cpr.state and %cpr.sim (to use with
 * get/setscicosvars)
 *
 * 14/06/06, Alan    : Save common intersci before calling scicos
 * (to disable scilab crash with scifunc.f)
 */

/* prototype */
int intscicosimc(fname,fname_len)
                 char *fname;
                 unsigned long fname_len;
{
 /*********************************************
  * external structure and function declaration
  *********************************************/
 /*declaration of funnum (in scicos.c)*/
 extern int C2F(funnum) __PARAMS((char *fname));

 /************************************
  * variables and constants définition
  ************************************/
 static int id[nsiz];

 /*declaration of static structure*/
 static struct {integer idb;} C2F(dbcos);   /*declaration of dbcos*/

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
 static int m1e4,n1e4;           /*state.iz*/
 static int *il_state_iz;
 static double *l_state_iz;
 static int m1e5,n1e5;           /*state.tevts*/
 static int *il_state_tevts;
 static double *l_state_tevts;
 static int m1e6,n1e6;           /*state.evtspt*/
 static int *il_state_evtspt;
 static int *l_state_evtspt;
 static int m1e7,n1e7;           /*state.pointi*/
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
 static int m4e3,n4e3;           /*sim.xptr*/
 static int *il_sim_xptr;
 static int *l_sim_xptr;
 static int m4e4,n4e4;           /*sim.zptr*/
 static int *il_sim_zptr;
 static int *l_sim_zptr;
 static int m4e5,n4e5;           /*sim.zcptr*/
 static int *il_sim_zcptr;
 static int *l_sim_zcptr;
 static int m4e6,n4e6;           /*sim.inpptr*/
 static int *il_sim_inpptr;
 static int *l_sim_inpptr;
 static int m4e7,n4e7;           /*sim.outptr*/
 static int *il_sim_outptr;
 static int *l_sim_outptr;
 static int m4e8,n4e8;           /*sim.inplnk*/
 static int *il_sim_inplnk;
 static int *l_sim_inplnk;
 static int m4e9,n4e9;           /*sim.outlnk*/
 static int *il_sim_outlnk;
 static int *l_sim_outlnk;
 static int m4e10,n4e10;         /*sim.rpar*/
 static int *il_sim_rpar;
 static double *l_sim_rpar;
 static int m4e11,n4e11;         /*sim.rpptr*/
 static int *il_sim_rpptr;
 static int *l_sim_rpptr;
 static int m4e12,n4e12;         /*sim.ipar*/
 static int *il_sim_ipar;
 static int *l_sim_ipar;
 static int m4e13,n4e13;         /*sim.ipptr*/
 static int *il_sim_ipptr;
 static int *l_sim_ipptr;
 static int m4e14,n4e14;         /*sim.clkptr*/
 static int *il_sim_clkptr;
 static int *l_sim_clkptr;
 static int m4e15,n4e15;         /*sim.ordptr*/
 static int *il_sim_ordptr;
 static int *l_sim_ordptr;
 static int m4e16,n4e16;         /*sim.execlk*/
 static int *il_sim_execlk;
 static int *l_sim_execlk;
 static int m4e17,n4e17;         /*sim.ordclk*/
 static int *il_sim_ordclk;
 static int *l_sim_ordclk;
 static int m4e18,n4e18;         /*sim.cord*/
 static int *il_sim_cord;
 static int *l_sim_cord;
 static int m4e19,n4e19;         /*sim.oord*/
 static int *il_sim_oord;
 static int *l_sim_oord;
 static int m4e20,n4e20;         /*sim.zord*/
 static int *il_sim_zord;
 static int *l_sim_zord;
 static int m4e21,n4e21;         /*sim.critev*/
 static int *il_sim_critev;
 static int *l_sim_critev;
 static int m4e22,n4e22;         /*sim.nb*/
 static int *il_sim_nb;
 static int *l_sim_nb;
 static int m4e23,n4e23;         /*sim.ztyp*/
 static int *il_sim_ztyp;
 static int *l_sim_ztyp;
 static int m4e24,n4e24;         /*sim.nblk*/
 static int *il_sim_nblk;
 static int *l_sim_nblk;
 static int m4e25,n4e25;         /*sim.ndcblk*/
 static int *il_sim_ndcblk;
 static int *l_sim_ndcblk;
 static int m4e26,n4e26;         /*sim.subscr*/
 static int *il_sim_subscr;
 static int *l_sim_subscr;
 static int m4e27,n4e27;         /*sim.funtyp*/
 static int *il_sim_funtyp;
 static int *l_sim_funtyp;
 static int m4e28,n4e28;         /*sim.iord*/
 static int *il_sim_iord;
 static int *l_sim_iord;
 static int m4e29,n4e29;         /*sim.labels*/
 static int *il_sim_lab;
 static int *il_sim_labptr;
 static int *l_sim_lab;
 static int m4e30,n4e30;         /*sim.modptr*/
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
  Err=1;
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

    /*4 : state.iz     */
    il_state_iz = (int *) (listentry(il_state,4));
    l_state_iz  = (double *) (il_state_iz+4);
    m1e4 = il_state_iz[1];
    n1e4 = il_state_iz[2];

    /*5 : state.tevts  */
    il_state_tevts = (int *) (listentry(il_state,5));
    l_state_tevts  = (double *) (il_state_tevts+4);
    m1e5 = il_state_tevts[1];
    n1e5 = il_state_tevts[2];

    /*6 : state.evtspt */
    il_state_evtspt = (int *) (listentry(il_state,6));
    l_state_evtspt  = (int *) (il_state_evtspt+4);
    m1e6 = il_state_evtspt[1];
    n1e6 = il_state_evtspt[2];

    /*7 : state.pointi */
    il_pointi = (int *) (listentry(il_state,7));
    l_pointi = (int *) (il_pointi+4);
    m1e7 = il_pointi[1];
    n1e7 = il_pointi[2];

    /*8 : state.outtb  */
    il_state_outtb = (int *) (listentry(il_state,8));
    if(il_state_outtb[0]!=15) /*check if il_state_outtb is a list*/
    {
     Scierror(56,"%s : outtb element of state must be a list.\n",fname);
     Err=4;
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
  Err=2;
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
  Err=3;
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
 if(il_sim[0]!=16)  /*Check if sim is a tlist*/
 {
  Scierror(56,"%s : Fourth argument must be a Tlist.\n",fname);
  Err=4;
  return 0;
 }
 m4 = il_sim[1];
 n4 = il_sim[2];
 il_sim_save = il_sim; /* make a copy of il_sim in a global variabe */

    /*2  : sim.funs*/
    il_sim_fun = (int *) (listentry(il_sim,2));
    if(il_sim_fun[0]!=15) /*check if sim.funs is a list*/
    {
     Scierror(56,"%s : Second element of sim must be a list.\n",fname);
     Err=4;
     return 0;
    }
    nblk = il_sim_fun[1]; /*nblk is the dimension of the list sim.funs*/

    /*3  : sim.xptr   */
    il_sim_xptr = (int *) (listentry(il_sim,3));
    m4e3 = il_sim_xptr[1];
    n4e3 = il_sim_xptr[2];
    l_sim_xptr = (int *) (il_sim_xptr+4);

    /*4  : sim.zptr   */
    il_sim_zptr = (int *) (listentry(il_sim,4));
    m4e4 = il_sim_zptr[1];
    n4e4 = il_sim_zptr[2];
    l_sim_zptr = (int *) (il_sim_zptr+4);

    /*5  : sim.zcptr  */
    il_sim_zcptr = (int *) (listentry(il_sim,5));
    m4e5 = il_sim_zcptr[1];
    n4e5 = il_sim_zcptr[2];
    l_sim_zcptr = (int *) (il_sim_zcptr+4);

    /*6  : sim.inpptr */
    il_sim_inpptr = (int *) (listentry(il_sim,6));
    m4e6 = il_sim_inpptr[1];
    n4e6 = il_sim_inpptr[2];
    l_sim_inpptr = (int *) (il_sim_inpptr+4);

    /*7  : sim.outptr */
    il_sim_outptr = (int *) (listentry(il_sim,7));
    m4e7 = il_sim_outptr[1];
    n4e7 = il_sim_outptr[2];
    l_sim_outptr = (int *) (il_sim_outptr+4);

    /*8  : sim.inplnk */
    il_sim_inplnk = (int *) (listentry(il_sim,8));
    m4e8 = il_sim_inplnk[1];
    n4e8 = il_sim_inplnk[2];
    l_sim_inplnk = (int *) (il_sim_inplnk+4);

    /*9  : sim.outlnk */
    il_sim_outlnk = (int *) (listentry(il_sim,9));
    m4e9 = il_sim_outlnk[1];
    n4e9 = il_sim_outlnk[2];
    l_sim_outlnk = (int *) (il_sim_outlnk+4);

    /*10 : sim.rpar   */
    il_sim_rpar = (int *) (listentry(il_sim,10));
    m4e10 = il_sim_rpar[1];
    n4e10 = il_sim_rpar[2];
    l_sim_rpar = (double *) (il_sim_rpar+4);

    /*11 : sim.rpptr  */
    il_sim_rpptr = (int *) (listentry(il_sim,11));
    m4e11 = il_sim_rpptr[1];
    n4e11 = il_sim_rpptr[2];
    l_sim_rpptr = (int *) (il_sim_rpptr+4);

    /*12 : sim.ipar   */
    il_sim_ipar = (int *) (listentry(il_sim,12));
    m4e12 = il_sim_ipar[1];
    n4e12 = il_sim_ipar[2];
    l_sim_ipar = (int *) (il_sim_ipar+4);

    /*13 : sim.ipptr  */
    il_sim_ipptr = (int *) (listentry(il_sim,13));
    m4e13 = il_sim_ipptr[1];
    n4e13 = il_sim_ipptr[2];
    l_sim_ipptr = (int *) (il_sim_ipptr+4);

    /*14 : sim.clkptr */
    il_sim_clkptr = (int *) (listentry(il_sim,14));
    m4e14 = il_sim_clkptr[1];
    n4e14 = il_sim_clkptr[2];
    l_sim_clkptr = (int *) (il_sim_clkptr+4);

    /*15 : sim.ordptr */
    il_sim_ordptr = (int *) (listentry(il_sim,15));
    m4e15 = il_sim_ordptr[1];
    n4e15 = il_sim_ordptr[2];
    l_sim_ordptr = (int *) (il_sim_ordptr+4);

    /*16 : sim.execlk */
    il_sim_execlk = (int *) (listentry(il_sim,16));
    m4e16 = il_sim_execlk[1];
    n4e16 = il_sim_execlk[2];
    l_sim_execlk = (int *) (il_sim_execlk+4);

    /*17 : sim.ordclk */
    il_sim_ordclk = (int *) (listentry(il_sim,17));
    m4e17 = il_sim_ordclk[1];
    n4e17 = il_sim_ordclk[2];
    l_sim_ordclk = (int *) (il_sim_ordclk+4);

    /*18 : sim.cord   */
    il_sim_cord = (int *) (listentry(il_sim,18));
    m4e18 = il_sim_cord[1];
    n4e18 = il_sim_cord[2];
    l_sim_cord = (int *) (il_sim_cord+4);

    /*19 : sim.oord   */
    il_sim_oord = (int *) (listentry(il_sim,19));
    m4e19 = il_sim_oord[1];
    n4e19 = il_sim_oord[2];
    l_sim_oord = (int *) (il_sim_oord+4);

    /*20 : sim.zord   */
    il_sim_zord = (int *) (listentry(il_sim,20));
    m4e20 = il_sim_zord[1];
    n4e20 = il_sim_zord[2];
    l_sim_zord = (int *) (il_sim_zord+4);

    /*21 : sim.critev */
    il_sim_critev = (int *) (listentry(il_sim,21));
    m4e21 = il_sim_critev[1];
    n4e21 = il_sim_critev[2];
    l_sim_critev = (int *) (il_sim_critev+4);

    /*22 : sim.nb     */
    il_sim_nb = (int *) (listentry(il_sim,22));
    m4e22 = il_sim_nb[1];
    n4e22 = il_sim_nb[2];
    l_sim_nb = (int *) (il_sim_nb+4);
    if (l_sim_nb[0]!=nblk) /*value of nb must be equal to nblk*/
    {
     Scierror(42,"%s : Incompatible sim.nb RHS parameter.\n",fname);
     return 0;
    }

    /*23 : sim.ztyp   */
    il_sim_ztyp = (int *) (listentry(il_sim,23));
    m4e23 = il_sim_ztyp[1];
    n4e23 = il_sim_ztyp[2];
    l_sim_ztyp = (int *) (il_sim_ztyp+4);

    /*24 : sim.nblk   */
    il_sim_nblk = (int *) (listentry(il_sim,24));
    m4e24 = il_sim_nblk[1];
    n4e24 = il_sim_nblk[2];
    l_sim_nblk = (int *) (il_sim_nblk+4);

    /*25 : sim.ndcblk */
    il_sim_ndcblk = (int *) (listentry(il_sim,25));
    m4e25 = il_sim_ndcblk[1];
    n4e25 = il_sim_ndcblk[2];
    l_sim_ndcblk = (int *) (il_sim_ndcblk+4);

    /*26 : sim.subscr */
    il_sim_subscr = (int *) (listentry(il_sim,26));
    m4e26 = il_sim_subscr[1];
    n4e26 = il_sim_subscr[2];
    l_sim_subscr = (int *) (il_sim_subscr+4);

    /*27 : sim.funtyp */
    il_sim_funtyp = (int *) (listentry(il_sim,27));
    m4e27 = il_sim_funtyp[1];
    n4e27 = il_sim_funtyp[2];
    l_sim_funtyp = (int *) (il_sim_funtyp+4);

    /*28 : sim.iord   */
    il_sim_iord = (int *) (listentry(il_sim,28));
    m4e28 = il_sim_iord[1];
    n4e28 = il_sim_iord[2];
    l_sim_iord = (int *) (il_sim_iord+4);

    /*29 : sim.labels */
     il_sim_lab = (int *) (listentry(il_sim,29));
     m4e29 = il_sim_lab[1];
     n4e29 = il_sim_lab[2];
     il_sim_labptr = &il_sim_lab[4];  /*get address-1 of first pointer in labels*/
     l_sim_lab = (int *) (il_sim_lab+m4e29+5); /*get address of first string in labels*/

    /*30 : sim.modptr */
    il_sim_modptr = (int *) (listentry(il_sim,30));
    m4e30 = il_sim_modptr[1];
    n4e30 = il_sim_modptr[2];
    l_sim_modptr= (int *) (il_sim_modptr+4);

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
  Err=5;
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
  Err=6;
  return 0; 
 }
 else if(m6>7) /*Check if tol has a maximum of seven elements*/
 {
  Scierror(89,"%s : Sixth argument must have a maximum of seven elements.\n",fname);
  Err=6;
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
 if (m1e5!=m1e6)       err_check=1;        /*tevts vs evtspt*/
 else if (m4e3!=m4e4)  err_check=2;        /*xptr vs zptr*/
 else if (m4e3!=m4e5)  err_check=3;        /*xptr vs zcptr*/
 else if (m4e3!=m4e6)  err_check=4;        /*xptr vs npptr*/
 else if (m4e3!=m4e7)  err_check=5;        /*xptr vs outptr*/
 else if (m4e3!=m4e11) err_check=6;        /*xptr vs rpptr*/
 else if (m4e3!=m4e13) err_check=7;        /*xptr vs ipptr*/
 else if (m4e3!=m4e14) err_check=8;        /*xptr vs clkptr*/
 else if ((n4e17!=2)&(m4e17!=0)) err_check=9;  /*sim.ordclk*/
 else if ((n4e18!=2)&(m4e18!=0)) err_check=10; /*sim.cord*/
 else if ((n4e19!=2)&(m4e19!=0)) err_check=11; /*sim.oord*/
 else if ((n4e20!=2)&(m4e20!=0)) err_check=12; /*sim.zord*/
 else if ((n4e28!=2)&(m4e28!=0)) err_check=13; /*sim.iord*/
 if (err_check!=0)
 {
  /* please write an error table here  */
  Scierror(42,"%s : error in cross variable size checking : %d\n",\
           fname,err_check);
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
      Fin=0;
      C2F(funs)(id);
      if ((C2F(com).fun==-1)|(C2F(com).fun==-2)) lfunpt[i]=-*Lstk(Fin);
      else 
      {
       C2F(curblk).kfun=i+1;
       Scierror(888,"%s : unknown block : %s\n",fname,C2F(cha1).buf);
       FREE(lfunpt);
       return 0;
      }
    }
   }
   else 
   {
     Err=4;
     Scierror(44,"%s : error\n",fname);
     FREE(lfunpt);
     return 0;
   }
   lf=lf+*istk(il4e2+3+i)-*istk(il4e2+i+2);
 }

 /*******************************
  * set outtbptr,outtbsz,outtbtyp
  *******************************/
 /*Allocation of outtbptr*/
 if ((outtbptr=(void **) MALLOC(nlnk*sizeof(void *)))==NULL )
 {
  FREE(lfunpt);
  return 0;
 }
 /*Allocation of outtbsz*/
 if ((outtbsz=(int *) MALLOC(nlnk*2*sizeof(int)))==NULL )
 {
  FREE(outtbptr);
  FREE(lfunpt);
  return 0;
 }
 /*Allocation of outtbtyp*/
 if ((outtbtyp=(int *) MALLOC(nlnk*sizeof(int)))==NULL )
 {
  FREE(outtbsz);
  FREE(outtbptr);
  FREE(lfunpt);
  return 0;
 }

 /*initalize nelem*/
 nelem=0;

 /*set vectors of outtb*/
 for (j=0;j<nlnk;j++) /*for each link*/
 {
  subheader=(int *)(listentry(il_state_outtb,j+1)); /*get header of outtbl(j+1)*/
  outtbsz[j*2]=subheader[1]; /*store dimensions*/
  outtbsz[(j*2)+1]=subheader[2];

  switch (subheader[0]) /*store type and address*/
  {
   /*matrix of double*/
   case 1  : switch (subheader[3])
             {
              case 0  : outtbtyp[j]=10;  /*double real matrix*/
                        outtbptr[j]=(double *)(subheader+4);
                        break;

              case 1  : outtbtyp[j]=11;  /*double complex matrix*/
                        outtbptr[j]=(double *)(subheader+4);
                        break;

              default : Scierror(888,\
                                "%s : error. Type %d of double scalar matrix not yet supported.\n",\
                                fname,subheader[3]);
                        FREE(outtbptr);
                        FREE(outtbtyp);
                        FREE(outtbsz);
                        FREE(lfunpt);
                        if (outtb_elem!=NULL) FREE(outtb_elem);
                        break;
             }
             break;

   /*matrix of integers*/
   case 8  : switch (subheader[3])
             {
              case 1  : outtbtyp[j]=81;  /*int8*/
                        outtbptr[j]=(char *)(subheader+4);
                        break;

              case 2  : outtbtyp[j]=82;  /*int16*/
                        outtbptr[j]=(short *)(subheader+4);
                        break;

              case 4  : outtbtyp[j]=84;  /*int32*/
                        outtbptr[j]=(long *)(subheader+4);
                        break;

              case 11 : outtbtyp[j]=811; /*uint8*/
                        outtbptr[j]=(unsigned char *)(subheader+4);
                        break;

              case 12 : outtbtyp[j]=812; /*uint16*/
                        outtbptr[j]=(unsigned short *)(subheader+4);
                        break;

              case 14 : outtbtyp[j]=814; /*uint32*/
                        outtbptr[j]=(unsigned long *)(subheader+4);
                        break;

              default : Scierror(888,\
                                "%s : error. Type %d of integer scalar matrix not yet supported.\n",\
                                fname,subheader[3]);
                        FREE(outtbptr);
                        FREE(outtbtyp);
                        FREE(outtbsz);
                        FREE(lfunpt);
                        if (outtb_elem!=NULL) FREE(outtb_elem);
                        break;
             }
             break;


   default : Scierror(888,"%s : error. Type %d not yet supported.\n",fname,subheader[0]);
             FREE(outtbptr);
             FREE(outtbtyp);
             FREE(outtbsz);
             FREE(lfunpt);
             if (outtb_elem!=NULL) FREE(outtb_elem);
             return 0;
             break;
  }

  /* store lnk and pos in outtb_elem */
  k=nelem;
  nelem+=outtbsz[j*2]*outtbsz[(j*2)+1];
  if ((outtb_elem=(outtb_el *) REALLOC(outtb_elem,nelem*sizeof(outtb_el)))==NULL)
  {
   Scierror(999,"%s : No more free memory.\n",fname);
   FREE(outtbptr);
   FREE(outtbtyp);
   FREE(outtbsz);
   FREE(lfunpt);
   if (outtb_elem!=NULL) FREE(outtb_elem);
   return 0;
  }
  for (i=0;i<outtbsz[j*2]*outtbsz[(j*2)+1];i++)
  {
   outtb_elem[k+i].lnk=j;
   outtb_elem[k+i].pos=i;
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
   FREE(outtbptr);
   FREE(outtbtyp);
   FREE(outtbsz);
   FREE(lfunpt);
   FREE(outtb_elem);
   return 0;
  }
  loc = MALLOC(sizeof(intersci_list));
  if (loc == NULL)
  {
   FREE(outtbptr);
   FREE(outtbtyp);
   FREE(outtbsz);
   FREE(lfunpt);
   FREE(outtb_elem);
   FREE(new);
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

 /* Calling sequence :
  *   int C2F(scicos)
  *   (x_in, xptr_in, z__, work,zptr,modptr_in, iz, izptr, t0_in, tf_in, tevts_in, 
  *    evtspt_in, nevts, pointi_in, outtbptr_in, outtbsz_in, outtbtyp_in, nlnk1,
  *    funptr, funtyp_in, inpptr_in, outptr_in,
  *    inplnk_in, outlnk_in, rpar, rpptr, ipar, ipptr, clkptr_in,
  *    ordptr_in, nordptr1, ordclk_in, cord_in, ncord1, iord_in, niord1, oord_in, noord1,
  *    zord_in, nzord1, critev_in, nblk1, ztyp, zcptr_in, subscr, nsubs, simpar,
  *    flag__, ierr_out)
  *
  *   double *x_in,*z__;
  *   void **work;
  *   integer *modptr_in;
  *   integer *xptr_in;
  *   integer *zptr, *iz, *izptr;
  *   double *t0_in, *tf_in, *tevts_in;
  *   integer *evtspt_in, *nevts, *pointi_in;
  *   void **outtbptr_in;
  *   integer *outtbsz_in;
  *   integer *outtbtyp_in;
  *   integer *nlnk1, *funptr, *funtyp_in, *inpptr_in, *outptr_in;
  *   integer *inplnk_in, *outlnk_in;
  *   double *rpar;
  *   integer *rpptr, *ipar, *ipptr, *clkptr_in, *ordptr_in, *nordptr1;
  *   integer *ordclk_in, *cord_in, *ncord1, *iord_in, *niord1, *oord_in;
  *   integer *noord1, *zord_in, *nzord1, *critev_in, *nblk1, *ztyp, *zcptr_in;
  *   integer *subscr, *nsubs;
  *   double *simpar;
  *   integer *flag__, *ierr_out;
  */

C2F(scicos)(l_state_x,l_sim_xptr,l_state_z, \
             l_state_iz,l_sim_zptr, \
             l_sim_modptr, \
             l_sim_lab,il_sim_labptr,l_tcur,l_tf,l_state_tevts, \
             l_state_evtspt,&m1e5,l_pointi,outtbptr,outtbsz,outtbtyp,outtb_elem,&nelem,&nlnk, \
             lfunpt,l_sim_funtyp,l_sim_inpptr, \
             l_sim_outptr,l_sim_inplnk,l_sim_outlnk,  \
             l_sim_rpar,l_sim_rpptr, \
             l_sim_ipar,l_sim_ipptr,l_sim_clkptr, \
             l_sim_ordptr,&m4e15, \
             l_sim_ordclk,l_sim_cord,&m4e18, \
             l_sim_iord,&m4e28, \
             l_sim_oord,&m4e19,l_sim_zord, &m4e20, \
             l_sim_critev,&nblk,l_sim_ztyp,l_sim_zcptr, \
             l_sim_subscr,&m4e26,simpar,&flag,&ierr);

 C2F(dbcos).idb=0;  /*return in normal mode*/

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
 FREE(outtbptr);
 FREE(outtbtyp);
 FREE(outtbsz);
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

   case 6  : strcpy(C2F(cha1).buf,"a block has been called with input out of its domain");
             break;

   case 7  : strcpy(C2F(cha1).buf,"singularity in a block");
             break;

   case 8  : strcpy(C2F(cha1).buf,"block produces an internal error");
             break;

   case 20  : strcpy(C2F(cha1).buf,"initial conditions not converging");
              C2F(curblk).kfun=0;
              break;

   case 21  : sprintf(C2F(cha1).buf, "cannot allocate memory in block=%d", \
                     C2F(curblk).kfun);
              C2F(curblk).kfun=0;
              break;

   case 22  : strcpy(C2F(cha1).buf,"sliding mode condition, cannot integrate");
              C2F(curblk).kfun=0;
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
  if (! (C2F(errgst).err1>0||Err>0))
  {
   Scierror(888,"%s\n",C2F(cha1).buf);
   C2F(curblk).kfun=0;
   C2F(com).fun=0; /*set common fun=0 (this disable bug in debug mode)*/
   return 0;
  }
 }

 if (Err>0) return 0;

 C2F(curblk).kfun=0;
 C2F(com).fun=0;

 /*********************
  * return Lsh variable
  *********************/
 if (Lhs>=1) LhsVar(1) = 1; /*return state in LhsVar(1)*/
 if (Lhs==2) LhsVar(2) = 2; /*return tcur in LhsVar(2)*/

 /* end */
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
 *                    header : integer pointer, a pointer of a list.
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
   /* Local variablle definition*/
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
 *            10  : double real
 *            11  : double complex
 *            80  : int
 *            81  : int8
 *            82  : int16
 *            84  : int32
 *            800 : uint
 *            811 : uint8
 *            812 : uint16
 *            814 : uint32
 *
 *
 * Output parameters : int (<1000), error flag
 *                     (0 if no error)
 *
 * 07/06/06, Alan    : initial version.
 *
 * 23/06/06, Alan    : moved in intcscicos.c to do
 *                     the connection with getscicosvars("blocks")
 */

/* prototype */
int var2sci(void *x,int n,int m,int typ_var)
{
  /************************************
   * variables and constants définition
   ************************************/
  /*counter and address variable declaration*/
  int nm,il,l,j,err;

  /*define all type of accepted ptr */
  double *x_d,*ptr_d;
  char *x_c,*ptr_c;
  unsigned char *x_uc,*ptr_uc;
  short *x_s,*ptr_s;
  unsigned short *x_us,*ptr_us;
  int *x_i,*ptr_i;
  unsigned int *x_ui,*ptr_ui;
  long *x_l,*ptr_l;
  unsigned long *x_ul,*ptr_ul;

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
  if (typ_var==10) nm=n*m; /*double real matrix*/
  else if (typ_var==11)  nm=n*m*2; /*double real matrix*/
  else if (typ_var==80)  nm=(int)(ceil((n*m)/2)+1); /*int*/
  else if (typ_var==81)  nm=(int)(ceil((n*m)/8)+1); /*int8*/
  else if (typ_var==82)  nm=(int)(ceil((n*m)/4)+1); /*int16*/
  else if (typ_var==84)  nm=(int)(ceil((n*m)/2)+1); /*int32*/
  else if (typ_var==800) nm=(int)(ceil((n*m)/2)+1); /*uint*/
  else if (typ_var==811) nm=(int)(ceil((n*m)/8)+1); /*uint8*/
  else if (typ_var==812) nm=(int)(ceil((n*m)/4)+1); /*uint16*/
  else if (typ_var==814) nm=(int)(ceil((n*m)/2)+1); /*uint32*/
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
       case 10  : /* set header */
                  *istk(il) = 1; /*double real matrix*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 0;
                  x_d = (double *) x;
                  ptr_d = (double *) stk(l);
                  for (j=0;j<m*n;j++) ptr_d[j] = x_d[j];
                  break;

       case 11  : /* set header */
                  *istk(il) = 1; /*double complex matrix*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 1;
                  x_d = (double *) x;
                  ptr_d = (double *) stk(l);
                  for (j=0;j<2*m*n;j++) ptr_d[j] = x_d[j];
                  break;

       case 80  : /* set header */
                  *istk(il) = 8; /*int*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 4;
                  x_i = (int *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_i = (int *) istk(il+4);
                   ptr_i[j] = x_i[j];
                  }
                  break;

       case 81  : /* set header */
                  *istk(il) = 8; /*int8*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 1;
                  x_c = (char *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_c = (char *) istk(il+4);
                   ptr_c[j] = x_c[j];
                  }
                  break;

       case 82  : /* set header */
                  *istk(il) = 8; /*int16*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 2;
                  x_s = (short *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_s = (short *) istk(il+4);
                   ptr_s[j] = x_s[j];
                  }
                  break;

       case 84  : /* set header */
                  *istk(il) = 8; /*int32*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 4;
                  x_l = (long *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_l = (long *) istk(il+4);
                   ptr_l[j] = x_l[j];
                  }
                  break;

       case 800 : /* set header */
                  *istk(il) = 8; /*uint*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 14;
                  x_ui = (unsigned int *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_ui = (unsigned int *) istk(il+4);
                   ptr_ui[j] = x_ui[j];
                  }
                  break;

       case 811 : /* set header */
                  *istk(il) = 8; /*uint8*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 11;
                  x_uc = (unsigned char *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_uc = (unsigned char *) istk(il+4);
                   ptr_uc[j] = x_uc[j];
                  }
                  break;

       case 812 : /* set header */
                  *istk(il) = 8; /*uint16*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 12;
                  x_us = (unsigned short *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_us = (unsigned short *) istk(il+4);
                   ptr_us[j] = x_us[j];
                  }
                  break;

       case 814 : /* set header */
                  *istk(il) = 8; /*uint32*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 14;
                  x_ul = (unsigned long *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_ul = (unsigned long *) istk(il+4);
                   ptr_ul[j] = x_ul[j];
                  }
                  break;

       default  : /* set header */
                  *istk(il) = 1; /*double real matrix*/
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
 *                 vvtosci, string.h, C2F(scierr), str2sci
 *
 * input argument : Blocks :scicos_block  ptr on a scicos_block structure
 *                  ierr : int ptr, an error flag
 *                  flag_imp : if flag_imp>=0 then use
 *                             import structure for x, xd and g.
 *                             In this case flag_imp is the block number.
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
 *                  (They are not yed called with callf in scicos.c)
 *
 */

/*prototype*/
int createblklist(scicos_block *Blocks, int *ierr, int flag_imp)
{
  /*local variable declaration*/
  int k;
  int nu,mu,ny,my;
  int u_typ,y_typ;

  /*variable used when imp_flag>=0*/
  int nv,mv;          /* length of data                                        */
  int nblk,ng;        /* to store number of blocks and number of zero cross.   */
  void *ptr;          /* ptr for data comming from import structure            */
  int *ptr_int;       /* ptr to store ptr on integer                           */
  double *ptr_double; /* ptr to store ptr on double                            */
  int *xptr, *zcptr;  /* to retrieve xptr by import and zcptr of scicos_blocks */
  double *x,*xd,*g;   /* ptr for x, xd and g for scicos_blocks              */

  /* set length of block list -please update me-                           */
  static int nblklst=31;
  /* set string of first element of scilab Blocks tlist -please update me- */
  static char *str_blklst[]={ "scicos_block", "nevprt" , "funpt" , "type"  ,
                              "scsptr"      , "nz"     , "z"     , "nx"    ,
                              "x"           , "xd"     , "res"   , "nin"   ,
                              "insz"        , "inptr"  , "nout"  , "outsz" ,
                              "outptr"      , "nevout" , "evout" , "nrpar" ,
                              "rpar"        , "nipar"  , "ipar"  , "ng"    ,
                              "g"           , "ztyp"   , "jroot" , "label" ,
                              "work"        , "nmode"  , "mode"};

  /* char ptr for str2sci - see below - */
  char **str1;


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
  str2sci(str_blklst,1,31);

  /* 2 - nevprt */
  *ierr=var2sci(&Blocks[0].nevprt,1,1,80);
  if (*ierr!=0) return 0;

  /* 3 - funpt */
  *ierr=var2sci(&Blocks[0].funpt,0,1,80); /* !!!! */
  if (*ierr!=0) return 0;

  /* 4 - type */
  *ierr=var2sci(&Blocks[0].type,1,1,80); /* !!!! */
  if (*ierr!=0) return 0;

  /* 5 - scsptr */
  *ierr=var2sci(&Blocks[0].scsptr,0,1,80); /* !!!! */
  if (*ierr!=0) return 0;

  /* 6 - nz */
  *ierr=var2sci(&Blocks[0].nz,1,1,80);
  if (*ierr!=0) return 0;

  /* 7 - z */
  if(Blocks[0].scsptr>0)
  {
   C2F(vvtosci)(Blocks[0].z,&Blocks[0].nz);
   if (C2F(scierr)()!=0) return 0; 
  }
  else
  {
   *ierr=var2sci(Blocks[0].z,Blocks[0].nz,1,10);
   if (*ierr!=0) return 0; 
  }

  /* 8 - nx */
  *ierr=var2sci(&Blocks[0].nx,1,1,80);
  if (*ierr!=0) return 0;

  /* 9 - x */
  if (flag_imp>=0) *ierr=var2sci(&x[xptr[flag_imp]-1],Blocks[0].nx,1,10);
  else *ierr=var2sci(Blocks[0].x,Blocks[0].nx,1,10);
  if (*ierr!=0) return 0; 

  /* 10 - xd */
  if (flag_imp>=0) *ierr=var2sci(&xd[xptr[flag_imp]-1],Blocks[0].nx,1,10);
  else *ierr=var2sci(Blocks[0].xd,Blocks[0].nx,1,10);
  if (*ierr!=0) return 0; 

  /* 11 - res */
  *ierr=var2sci(Blocks[0].res,Blocks[0].nx,1,10);
  if (*ierr!=0) return 0;

  /* 12 - nin */
  *ierr=var2sci(&Blocks[0].nin,1,1,80);
  if (*ierr!=0) return 0;

  /* 13 - insz */
  *ierr=var2sci(Blocks[0].insz,3*Blocks[0].nin,1,80);
  if (*ierr!=0) return 0;

  /* 14 - inptr */
  for (k=0;k<Blocks[0].nin;k++) 
  {
   nu=Blocks[0].insz[k]; /* retrieve number of rows */
   mu=Blocks[0].insz[Blocks[0].nin+k]; /* retrieve number of cols */
   u_typ=Blocks[0].insz[2*Blocks[0].nin+k]; /* retrieve type */
   *ierr=var2sci(Blocks[0].inptr[k],nu,mu,u_typ);
   if (*ierr!=0) return 0;
  }
  C2F(mklist)(&Blocks[0].nin); /*create inptr list*/

  /* 15 - nout */
  *ierr=var2sci(&Blocks[0].nout,1,1,80);
  if (*ierr!=0) return 0;

  /* 16 - outsz */
  *ierr=var2sci(Blocks[0].outsz,3*Blocks[0].nout,1,80);
  if (*ierr!=0) return 0;

  /* 17 - outptr */
  for (k=0;k<Blocks[0].nout;k++) 
  {
   ny=Blocks[0].outsz[k]; /* retrieve number of rows */
   my=Blocks[0].outsz[Blocks[0].nout+k]; /* retrieve number of cols */
   y_typ=Blocks[0].outsz[2*Blocks[0].nout+k]; /* retrieve type */
   *ierr=var2sci(Blocks[0].outptr[k],ny,my,y_typ);
   if (*ierr!=0) return 0;
  }
  C2F(mklist)(&Blocks[0].nout); /*create outptr list*/

  /* 18 - nevout */
  *ierr=var2sci(&Blocks[0].nevout,1,1,80);
  if (*ierr!=0) return 0;

  /* 19 - evout */
  *ierr=var2sci(Blocks[0].evout,Blocks[0].nevout,1,10);
  if (*ierr!=0) return 0; 

  /* 20 - nrpar */
  *ierr=var2sci(&Blocks[0].nrpar,1,1,80);
  if (*ierr!=0) return 0;

  /* 21 - rpar */
  if(Blocks[0].scsptr>0)
  {
   C2F(vvtosci)(Blocks[0].rpar,&Blocks[0].nrpar);
   if (C2F(scierr)()!=0) return 0;
  }
  else
  {
   *ierr=var2sci(Blocks[0].rpar,Blocks[0].nrpar,1,10);
   if (*ierr!=0) return 0; 
  }

  /* 22 - nipar */
  *ierr=var2sci(&Blocks[0].nipar,1,1,80);
  if (*ierr!=0) return 0;

  /* 23 - ipar */
  *ierr=var2sci(Blocks[0].ipar,Blocks[0].nipar,1,80);
  if (*ierr!=0) return 0;

  /* 24 - ng */
  *ierr=var2sci(&Blocks[0].ng,1,1,80);
  if (*ierr!=0) return 0;

  /* 25 - g */
  if (flag_imp>=0) *ierr=var2sci(&g[zcptr[flag_imp]-1],Blocks[0].ng,1,10);
  else *ierr=var2sci(Blocks[0].g,Blocks[0].ng,1,10);
  if (*ierr!=0) return 0;

  /* 26 - ztyp */
  *ierr=var2sci(&Blocks[0].ztyp,1,1,80);
  if (*ierr!=0) return 0;

  /* 27 - jroot */
  *ierr=var2sci(Blocks[0].jroot,Blocks[0].ng,1,80);
  if (*ierr!=0) return 0;

  /* 28 - label */
  if ((str1=MALLOC(sizeof(char*))) ==NULL )  return 0;
  if ((str1[0]=MALLOC(sizeof(char)*(strlen(Blocks[0].label)+1))) ==NULL )  return 0;
  (str1[0])[strlen(Blocks[0].label)]='\0';
  strncpy(str1[0],Blocks[0].label,strlen(Blocks[0].label));
  str2sci(str1,1,1);
  FREE(str1[0]);
  FREE(str1);
  if (C2F(scierr)()!=0) return 0; 

  /* 29 - work*/
  C2F(vvtosci)(*Blocks[0].work,(k=0,&k));
  if (C2F(scierr)()!=0) return 0; 

  /* 30 - nmode*/
  *ierr=var2sci(&Blocks[0].nmode,1,1,80);
  if (*ierr!=0) return 0;

  /* 31 - mode */
  *ierr=var2sci(Blocks[0].mode,Blocks[0].nmode,1,80);
  if (*ierr!=0) return 0;

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
 *               - 'ipar' to retrieve integer parameters
 *               - 'ipptr' to retrieve  ptr of integer parameters
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
 */

int intgetscicosvarsc(fname,fname_len)
                 char *fname;
                 unsigned long fname_len;
{
  /************************************
   * variables and constants définition
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
  int *ptr_int;             /* ptr to store ptr on integer                         */
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
  static int nentries=60;
  /* define accepted entries of getscicosvars -please update me- */
  static char *entry[]={ "x"        , "nx"      , "xptr"     , "zcptr"  , "z"      ,
                         "nz"       , "zptr"    , "rpar"     , "rpptr"  , "ipar"   ,
                         "ipptr"    , "outtb"   , "inpptr"   , "outptr" , "inplnk" ,
                         "outlnk"   , "subs"    , "tevts"    , "evtspt" , "pointi" ,
                         "iord"     , "oord"    , "zord"     , "funtyp" , "ztyp"   ,
                         "cord"     , "ordclk"  , "clkptr"   , "ordptr" , "critev" ,
                         "mod"      , "nmod"    , "iz"       , "nblk"   , "izptr"  ,
                         "outtbptr" , "outtbsz" , "outtbtyp" , "nlnk"   , "nsubs"  ,
                         "nevts"    , "niord"   , "noord"    , "nzord"  , "funptr" ,
                         "ncord"    , "nordptr" , "iwa"      , "blocks" , "ng"     ,
                         "g"        , "t0"      , "tf"       , "Atol"   , "rtol"   ,
                         "ttol"     , "deltat"  , "hmax"     , "nelem"  , "outtb_elem"};

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
  il_str = (int *) GetData(1); /* get ptr of integer header of rsh 1 */
  if(il_str[0]!=10) /* Check if input argument is a character string matrix */
  {
   Scierror(55,"%s : First argument must be a string.\n",fname);
   Err=1;
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

   /*****************************************************************************
    * entries that can be retrieve by il_state_save, il_sim_save global variable
    *****************************************************************************/
   if (strcmp(C2F(cha1).buf,"x") == 0)          /* retrieve continuous state */
    ierr=CopyVarFromlistentry(j+2,il_state_save,2);
   else if (strcmp(C2F(cha1).buf,"xptr") == 0)  /* retrieve ptr of continuous state */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,3);
   else if (strcmp(C2F(cha1).buf,"zcptr") == 0)  /* retrieve ptr of zero crossing array */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,5);
   else if (strcmp(C2F(cha1).buf,"z") == 0)      /* retrieve discrete state */
    ierr=CopyVarFromlistentry(j+2,il_state_save,3);
   else if (strcmp(C2F(cha1).buf,"zptr") == 0)   /* retrieve ptr of discrete state */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,4);
   else if (strcmp(C2F(cha1).buf,"rpar") == 0)   /* retrieve rpar */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,10);
   else if (strcmp(C2F(cha1).buf,"rpptr") == 0)  /* retrieve ptr of rpar */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,11);
   else if (strcmp(C2F(cha1).buf,"ipar") == 0)   /* retrieve ipar */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,12);
   else if (strcmp(C2F(cha1).buf,"ipptr") == 0)  /* retrieve ptr of ipar */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,13);
   else if (strcmp(C2F(cha1).buf,"outtb") == 0)  /* retrieve outtb */
    ierr=CopyVarFromlistentry(j+2,il_state_save,8);
   else if (strcmp(C2F(cha1).buf,"inpptr") == 0) /* retrieve number of input ports */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,6);
   else if (strcmp(C2F(cha1).buf,"outptr") == 0) /* retrieve number of output ports */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,7);
   else if (strcmp(C2F(cha1).buf,"inplnk") == 0) /* retrieve link number of input ports */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,8);
   else if (strcmp(C2F(cha1).buf,"outlnk") == 0) /* retrieve link number of output ports */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,9);
   else if (strcmp(C2F(cha1).buf,"subs") == 0)   /* retrieve subscr */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,26);
   else if (strcmp(C2F(cha1).buf,"tevts") == 0)  /* retrieve tevts */
    ierr=CopyVarFromlistentry(j+2,il_state_save,5);
   else if (strcmp(C2F(cha1).buf,"evtspt") == 0) /* retrieve evtspt */
    ierr=CopyVarFromlistentry(j+2,il_state_save,6);
   else if (strcmp(C2F(cha1).buf,"pointi") == 0) /* retrieve pointi */
    ierr=CopyVarFromlistentry(j+2,il_state_save,7);
   else if (strcmp(C2F(cha1).buf,"iord") == 0)   /* retrieve iord */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,28);
   else if (strcmp(C2F(cha1).buf,"oord") == 0)   /* retrieve oord */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,19);
   else if (strcmp(C2F(cha1).buf,"zord") == 0)   /* retrieve zord */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,20);
   else if (strcmp(C2F(cha1).buf,"funtyp") == 0) /* retrieve funtyp */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,27);
   else if (strcmp(C2F(cha1).buf,"ztyp") == 0)   /* retrieve ztyp */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,23);
   else if (strcmp(C2F(cha1).buf,"cord") == 0)   /* retrieve cord */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,18);
   else if (strcmp(C2F(cha1).buf,"ordclk") == 0) /* retrieve ordclk */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,17);
   else if (strcmp(C2F(cha1).buf,"clkptr") == 0) /* retrieve clkptr */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,14);
   else if (strcmp(C2F(cha1).buf,"ordptr") == 0) /* retrieve ordptr */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,15);
   else if (strcmp(C2F(cha1).buf,"critev") == 0) /* retrieve critev */
    ierr=CopyVarFromlistentry(j+2,il_sim_save,21);

   /*************************************************
    * integer variables coming from import structure
    *************************************************/
   else if ((strcmp(C2F(cha1).buf,"mod") == 0)      || /* retrieve mode */
            (strcmp(C2F(cha1).buf,"nmod") == 0)     || /* retrieve nmode */
            (strcmp(C2F(cha1).buf,"iz") == 0)       || /* label integer code of blocks */
            (strcmp(C2F(cha1).buf,"nblk") == 0)     || /* number of block */
            (strcmp(C2F(cha1).buf,"izptr") == 0)    || /* label integer code of blocks ptr*/
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
            (strcmp(C2F(cha1).buf,"nelem") == 0)          /* retrieve nelem */
                                                     )
   {
    /* retrieve dims and prt of asked array with getscicosvarsfromimport */
    ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

    /* check ierr flag */
    if (ierr==TRUE_)
    {
     l_tmp = I_INT32; /* define type of integer */
     CreateVar(j+2,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&nv,&mv,&l_tmp); /* Create int32 variable at the top+j+1 addr. of the stack */
     il_tmp = (int *) istk(l_tmp);      /* Store value of address of istk(l_tmp) in il_tmp */
     ptr_int = (int *) ptr;             /* cast void* ptr to int* ptr */
     for (i=0;i<nv*mv;i++) il_tmp[i] = ptr_int[i]; /* copy returned array in istk */
    }
   }

  /*************************************************
   * double variables coming from import structure
   *************************************************/
   else if ((strcmp(C2F(cha1).buf,"g") == 0)      || /* retrieve g      */
            (strcmp(C2F(cha1).buf,"t0") == 0)     || /* retrieve t0     */
            (strcmp(C2F(cha1).buf,"tf") == 0)     || /* retrieve tf     */
            (strcmp(C2F(cha1).buf,"Atol") == 0)   || /* retrieve Atol   */
            (strcmp(C2F(cha1).buf,"rtol") == 0)   || /* retrieve rtol   */
            (strcmp(C2F(cha1).buf,"ttol") == 0)   || /* retrieve ttol   */
            (strcmp(C2F(cha1).buf,"deltat") == 0) || /* retrieve deltat */
            (strcmp(C2F(cha1).buf,"hmax") == 0)   /* retrieve hmax   */
                                                 )
   {
    /* retrieve dims and prt of asked array with getscicosvarsfromimport */
    ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

    /* check ierr flag */
    if (ierr==TRUE_)
    {
     ptr_dd = (double *) ptr;
     CreateVar(j+2,MATRIX_OF_DOUBLE_DATATYPE,&nv,&mv,&l_tmp); /* Create double variable at the top+j+1 addr. of the stack */
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
         /*fprintf(stderr,"k=%d,X,xd Non initialisé\n",k);*/
        /* set flag_imp=k for createblklst <0 */
        i=k;
       }
      }
      if (ptr_scsblk[k].ng!=0)
      {
       if ((ptr_scsblk[k].g!=&g[zcptr[k]-1]) && (ptr_scsblk[k].g!=&x[xptr[k]-1]))
       {
        /*fprintf(stderr,"k=%d,g Non initialisé\n",k);*/
        /* set flag_imp=k for createblklst <0 */
        i=k;
       }
      }
      /* call createblklist */
      ierr=createblklist(&ptr_scsblk[k], &errc,i);

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
      CreateVar(j+2,LIST_DATATYPE,&nblk,(i=1,&i),&l_tmp); /* this is done to inform common intersci */
     }

    }
   }

  /*******************************************
   * outtb_elem coming from import structure
   *******************************************/
   else if ((strcmp(C2F(cha1).buf,"outtb_elem") == 0)) /* retrieve outtb_eleme */
   {
    /* retrieve dims and prt of asked array with getscicosvarsfromimport */
    ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

    /* check ierr flag */
    if (ierr==TRUE_)
    {
     l_tmp = I_INT32; /* define type of integer */
     CreateVar(j+2,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&nv,&mv,&l_tmp); /* Create int32 variable at the top+j+1 addr. of the stack */
     il_tmp = (int *) istk(l_tmp);      /* Store value of address of istk(l_tmp) in il_tmp */
     ptr_elem = (outtb_el *) ptr;       /* cast void* ptr to int* ptr */
     for (i=0;i<nv;i++) /* copy returned array in istk */
     {
      il_tmp[i] = ptr_elem[i].lnk + 1; /* +1 is for the connection with outtb list */
      il_tmp[nv+i] = ptr_elem[i].pos + 1;
     }
    }
   }

   /* if input argument doesn't match with any accepted string
    * then display an error message.
    */
   else
   {
    Scierror(999,"%s : Undefined field.\n",fname);
    FREE(dyn_char);
    return 0;
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
    CreateVar(Rhs+2,TYPED_LIST_DATATYPE,&j,(i=1,&i),&l_tmp); /* this is done to inform common intersci */
   }

   /* put new variable in lsh argument */
   LhsVar(1)=Rhs+2;

   /* end */
   FREE(dyn_char);
   return 0;
}

/* intcurblkc curblock interface routine
 *
 * [nblk]=curblock()
 *
 * rhs : empty
 * lhs : nblk : the current block (int32 scilab object)
 *
 * 20/06/06, Alan : Rewritten from original fortran
 * source code intcurblk in coselm.f.
 *
 */
int intcurblkc(fname,fname_len)
                 char *fname;
                 unsigned long fname_len;
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
   * Create int32 variable
   ************************/
  /* define type of integer */
  l1 = I_INT32;
  /* Create int32 variable at the top addr. of the stack */
  CreateVar(Rhs+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,(j=1,&j),(k=1,&k),&l1);
  /* Store value of C2F(curblk).kfun at the l1 address in istk */
  *istk(l1) = C2F(curblk).kfun;

  /* return the value stored at Top address to lhs variable */
  LhsVar(1) = Rhs+1;

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
 *
 */

int intbuildouttb(fname)
 char *fname;
{
 static int l1, m1, n1;
 static int l2, m2, n2;
 static int l3,n3=1;
 SciIntMat M1,M2,M3;

 int n_lnksz,n_lnktyp;
 int *lnksz=NULL,*lnktyp=NULL;

 double *ptr_d,*ptr_dc;
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
 if (VarType(1)==sci_matrix)
 {
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
 }
 else if (VarType(1)==sci_ints)
 {
  GetRhsVar(1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m1, &n1, &M1);
 }
 else
 {
  Scierror(888,"%s : first argument must be double or int32.\n",fname);
  return 0;
 }
 /*check size of Rhs 1*/
 if (m1==2) n_lnksz=n1;
 else if (n1==2) n_lnksz=m1;
 else
 {
  Scierror(888,"%s : bad dimension for first argument.\n",fname);
  return 0;
 }
 /*allocate lnksz*/
 if ((lnksz=MALLOC(2*n_lnksz*sizeof(int)))==NULL)
 {
  Scierror(999,"%s : No more free memory.\n",fname);
  return 0;
 }

 /*check type of Rhs 2*/
 if (VarType(2)==sci_matrix)
 {
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
 }
 else if (VarType(2)==sci_ints)
 {
  GetRhsVar(2,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m2, &n2, &M2);
 }
 else
 {
  Scierror(888,"%s : second argument must be double or int32.\n",fname);
  if (lnksz!=NULL) FREE(lnksz);
  return 0;
 }
 /*check size of Rhs 2*/
 if (m2==1) n_lnktyp=n2;
 else if (n2==1) n_lnktyp=m2;
 else
 {
  Scierror(888,"%s : bad dimension for second argument.\n",fname);
  if (lnksz!=NULL) FREE(lnksz);
  return 0;
 }

 /*cross size checking*/
 if (n_lnksz!=n_lnktyp)
 {
  Scierror(888,"%s : first and second argument must have "
               "the same length.\n",fname);
  if (lnksz!=NULL) FREE(lnksz);
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
 if (VarType(1)==sci_ints)
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
 if (VarType(2)==sci_ints)
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
 CreateVar(3,LIST_DATATYPE,&n_lnktyp,&n3,&l3);

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
             CreateListVarFromPtr(3,i+1,MATRIX_OF_DOUBLE_DATATYPE,&lnksz[i],&lnksz[i+n_lnktyp], &ptr_d);
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
             CreateListVarFromPtr(3,i+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&M3.m,&M3.n,&M3);
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
             CreateListVarFromPtr(3,i+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&M3.m,&M3.n,&M3);
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
             CreateListVarFromPtr(3,i+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&M3.m,&M3.n,&M3);
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
             CreateListVarFromPtr(3,i+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&M3.m,&M3.n,&M3);
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
             CreateListVarFromPtr(3,i+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&M3.m,&M3.n,&M3);
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
             CreateListVarFromPtr(3,i+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&M3.m,&M3.n,&M3);
             FREE(ptr_uc);
             break;

   default : if ((ptr_d=MALLOC(nm*sizeof(double)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_d[j]=0;
             CreateListVarFromPtr(3,i+1,MATRIX_OF_DOUBLE_DATATYPE,&lnksz[i],&lnksz[i+n_lnktyp], &ptr_d);
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
