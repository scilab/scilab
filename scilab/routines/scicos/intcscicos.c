#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../stack-c.h"
#include "../machine.h"
#include "intcscicos.h"
#include "scicos_block.h"

typedef int (*des_interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct {
  des_interf f;    /** function **/
  char *name;      /** its name **/
} intcscicosTable;

static intcscicosTable Tab[]={
  {inttimescicos,"scicos_time"},
  {intduplicate,"duplicate"},
  {intdiffobjs,"diffobjs"},
  {intxproperty,"pointer_xproperty"},
  {intphasesim,"phase_simulation"},
  {intsetxproperty,"set_xproperty"},
  {intcpass2,"scicos_cpass2"},
  {intsetblockerror,"set_blockerror"},
};
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

/* interface for the previous function Table */ 

int C2F(intcscicos)()
{  
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
  C2F(putlhsvar)();
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
  size1=2*(*lstk(Top-Rhs+2)-*lstk(Top-Rhs+1)); 
  size2=2*(*lstk(Top-Rhs+3)-*lstk(Top-Rhs+2));

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
  CreateVarFromPtr(1,"i",&n_pointer_xproperty,(un=1,&un),&pointer_xproperty);
  LhsVar(1)=1;
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
  CreateVar(1,"i",(un=1,&un),(un=1,&un),&l1);
  *istk(l1)=get_phase_simulation();
  LhsVar(1)=1;
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
  GetRhsVar(1,"i",&m1,(un=1,&un),&l1);
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
  GetRhsVar(1,"i",(un=1,&un),(un=1,&un),&l1);
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
  if ((bllst111=malloc(sizeof(char*)*(m+1))) ==NULL )  return 0;		  
  ((int*)bllst111)[0]=m;
  if ((bllst112=malloc(sizeof(int)*(m+1))) ==NULL )  return 0;		  
  bllst112[0]=m;
  if ((bllst2ptr=malloc(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst2ptr[0]=m+1;
  bllst2ptr[1]=1;
  if ((bllst2=malloc(sizeof(int))) ==NULL )  return 0;		  
  bllst2[0]=0;
  if ((bllst3ptr=malloc(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst3ptr[0]=m+1;
  bllst3ptr[1]=1;
  if ((bllst3=malloc(sizeof(int))) ==NULL )  return 0;		  
  bllst3[0]=0;
  if ((bllst4ptr=malloc(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst4ptr[0]=m+1;
  bllst4ptr[1]=1;
  if ((bllst4=malloc(sizeof(int))) ==NULL )  return 0;		  
  bllst4[0]=0;
  if ((bllst5ptr=malloc(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst5ptr[0]=m+1;
  bllst5ptr[1]=1;
  if ((bllst5=malloc(sizeof(int))) ==NULL )  return 0;		  
  bllst5[0]=0;
  if ((bllst6ptr=malloc(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst6ptr[0]=m+1;
  bllst6ptr[1]=1;
  if ((bllst6=malloc(sizeof(double))) ==NULL )  return 0;		  
  ((int*) bllst6)[0]=0;
  if ((bllst7ptr=malloc(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst7ptr[0]=m+1;
  bllst7ptr[1]=1;
  if ((bllst7=malloc(sizeof(double))) ==NULL )  return 0;		  
  ((int*) bllst7)[0]=0;
  if ((bllst8ptr=malloc(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst8ptr[0]=m+1;
  bllst8ptr[1]=1;
  if ((bllst8=malloc(sizeof(double))) ==NULL )  return 0;		  
  ((int*) bllst8)[0]=0;
  if ((bllst9ptr=malloc(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst9ptr[0]=m+1;
  bllst9ptr[1]=1;
  if ((bllst9=malloc(sizeof(int))) ==NULL )  return 0;		  
  bllst9[0]=0;
  if ((nmode=malloc(sizeof(int)*(m+1))) ==NULL )  return 0;		  
  nmode[0]=m;
  if ((nzcross=malloc(sizeof(int)*(m+1))) ==NULL )  return 0;		  
  nzcross[0]=m;  
  if ((bllst10=(char**) malloc(sizeof(char *)*(m+1))) ==NULL )  return 0;		  
  ((int*) bllst10)[0]=m;
  if ((bllst11ptr=malloc(sizeof(int)*(m+2))) ==NULL )  return 0;		  
  bllst11ptr[0]=m+1;
  bllst11ptr[1]=1;
  if ((bllst11=malloc(sizeof(double))) ==NULL )  return 0;		  
  ((int*) bllst11)[0]=0;
  if ((bllst12=malloc(sizeof(int)*(m+m+1))) ==NULL )  return 0;		  
  bllst12[0]=m+m;
  if ((bllst13=malloc(sizeof(char *)*(m+1))) ==NULL )  return 0;		  
  ((int*) bllst13)[0]=m;
  if ((xcd0=(double*)malloc(sizeof(double))) == NULL) return 0;
  ((int*) xcd0)[0]=0;
  if ((typ_x=calloc(m+1,sizeof(int))) ==NULL )  return 0;		  
  typ_x[0]=m;
  
  for (k=1; k <= m; k++)
    {
      li=(int*) listentry(header,k); /*le pointeur sur la kieme sous list (mlists)*/
      /* Le rang du champs*/
      /* MlistGetFieldNumber*/
      
      /* 1ier element de la list sim*/      
      if ((field_name=(char*) malloc(sizeof(char)*4)) ==NULL )  return 0;
      ((char*) field_name)[3]='\0';
      strcpy (field_name,"sim");
      field_num=MlistGetFieldNumber(li,field_name);
      le1=(int*) listentry(li,field_num);
      free(field_name);
      /*si sim est une list*/
      if (le1[0] == 15)
	{
	  le11=(int*) listentry(le1,1); /*sim(1)*/
	  if (le11[0] == 13)
	    {
	      if ((bllst111[k]=(char*) malloc(sizeof(char)*9)) ==NULL )  return 0;
	      ((char*) bllst111[k])[8]='\0';
	      strcpy (bllst111[k],"sciblock");
	      bllst112[k]=3;
	    }
	  else
	    {
	      le111=((int *) (le11+6));
	      n1111=le11[5]-1;
	      if ((bllst111[k]=(char*) malloc(sizeof(char)*(n1111+1))) ==NULL )  return 0;
	      ((char*) bllst111[k])[n1111]='\0';
	      C2F(cvstr)(&n1111,le111,bllst111[k],&one,strlen(bllst111[k]));
	      
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
	  if ((bllst111[k]=(char*) malloc(sizeof(char)*(n1111+1))) ==NULL )  return 0;
	  ((char*) bllst111[k])[n1111]='\0';
	  C2F(cvstr)(&n1111,le111,bllst111[k],&one,strlen(bllst111[k]));
	  
	}
      /* 2ieme element de la list in */
      if ((field_name=(char*) malloc(sizeof(char)*3)) ==NULL )  return 0;
      ((char*) field_name)[2]='\0';
      strcpy (field_name,"in");
      field_num=MlistGetFieldNumber(li,field_name);
      free(field_name);
      le2=(int*) listentry(li,field_num);
      ne2=le2[1];
      le22=((double *) (le2+4));
      bllst2ptr[k+1]=bllst2ptr[k]+ne2;
      if ((bllst2=realloc(bllst2,sizeof(int)*(bllst2[0]+ne2+1))) ==NULL )  return 0;	
      for (j=0; j< ne2; j++)
	{
	  bllst2[bllst2[0]+j+1]=(int)le22[j];
	}
      bllst2[0]=bllst2[0]+ne2;
      /* 3ieme element de la list out*/
      if ((field_name=(char*) malloc(sizeof(char)*4)) ==NULL )  return 0;
      ((char*) field_name)[3]='\0';
      strcpy (field_name,"out");
      field_num=MlistGetFieldNumber(li,field_name);
      free(field_name);
      le3=(int*) listentry(li,field_num);
      ne3=le3[1];
      le33=((double *) (le3+4));
      bllst3ptr[k+1]=bllst3ptr[k]+ne3;
      if ((bllst3=realloc(bllst3,sizeof(int)*(bllst3[0]+ne3+1))) ==NULL )  return 0;
      for (j=0; j< ne3; j++)
	{
	  bllst3[bllst3[0]+j+1]=(int)le33[j];
	}
      bllst3[0]=bllst3[0]+ne3;
      /* 4ieme element de la list evtin*/
      if ((field_name=(char*) malloc(sizeof(char)*6)) ==NULL )  return 0;
      ((char*) field_name)[5]='\0';
      strcpy (field_name,"evtin");
      field_num=MlistGetFieldNumber(li,field_name);
      free(field_name);
      le4=(int*) listentry(li,field_num);
      ne4=le4[1];
      le44=((double *) (le4+4));
      bllst4ptr[k+1]=bllst4ptr[k]+ne4;
      if ((bllst4=realloc(bllst4,sizeof(int)*(bllst4[0]+ne4+1))) ==NULL )  return 0;
      for (j=0; j< ne4; j++)
	{
	  bllst4[bllst4[0]+j+1]=(int)le44[j];
	}
      bllst4[0]=bllst4[0]+ne4;
      /* 5ieme element de la list evtout*/
      if ((field_name=(char*) malloc(sizeof(char)*7)) ==NULL )  return 0;
      ((char*) field_name)[6]='\0';
      strcpy (field_name,"evtout");
      field_num=MlistGetFieldNumber(li,field_name);
      free(field_name);
      le5=(int*) listentry(li,field_num);
      ne5=le5[1];
      le55=((double *) (le5+4));
      bllst5ptr[k+1]=bllst5ptr[k]+ne5;
      if ((bllst5=realloc(bllst5,sizeof(int)*(bllst5[0]+ne5+1))) ==NULL )  return 0;
      for (j=0; j< ne5; j++)
	{
	  bllst5[bllst5[0]+j+1]=(int)le55[j];
	}
      bllst5[0]=bllst5[0]+ne5;
      /* 6ieme element de la list state*/
      if ((field_name=(char*) malloc(sizeof(char)*6)) ==NULL )  return 0;
      ((char*) field_name)[5]='\0';
      strcpy (field_name,"state");
      field_num=MlistGetFieldNumber(li,field_name);
      free(field_name);
      le6=(int*) listentry(li,field_num);
      ne6=le6[1];
      le66=((double *) (le6+4));
      if (bllst112[k]<10000)
        {
	  if ((xcd0=(double*)realloc(xcd0,sizeof(double)*(((int*) xcd0)[0]+ne6+1))) == NULL ) return 0;
	  if ((bllst6=(double*)realloc(bllst6,sizeof(double)*(((int *) bllst6)[0]+ne6+1))) ==NULL )  return 0;
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
	  if ((xcd0=(double*)realloc(xcd0,sizeof(double)*(((int*) xcd0)[0]+ne6/2+1))) == NULL ) return 0;
	  if ((bllst6=(double*)realloc(bllst6,sizeof(double)*(((int *) bllst6)[0]+ne6/2+1))) ==NULL )  return 0;
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
      if ((field_name=(char*) malloc(sizeof(char)*7)) ==NULL )  return 0;
      ((char*) field_name)[6]='\0';
      strcpy (field_name,"dstate");
      field_num=MlistGetFieldNumber(li,field_name);
      free(field_name);
      le71=(double *) listentry(li,field_num);      
      le7=(int*) listentry(li,field_num);
      ne7= le7[1];
      le77=((double *) (le7+4));
      
      if ((field_name=(char*) malloc(sizeof(char)*5)) ==NULL )  return 0;
      ((char*) field_name)[4]='\0';
      strcpy (field_name,"rpar");
      field_num=MlistGetFieldNumber(li,field_name);
      free(field_name);
      le81=(double *) listentry(li,field_num);
      le8=(int*) listentry(li,field_num);
      ne8=le8[1];
      le88=((double *) (le8+4));
      if ( bllst112[k] == 3 || bllst112[k] == 5 || bllst112[k] == 10005)
	{
	  if (ne7 != 0)
	    {
	      ne71=li[8+2]-li[8+1];
	      if ((xd0k=(double*)malloc(sizeof(double)*(ne71+1))) ==NULL )  return 0;
	      ((int *) xd0k)[0]=ne71;
	      
	      C2F(unsfdcopy)(&ne71,le71,&moinsun,(xd0k+1),&moinsun);
	      bllst7ptr[k+1]=bllst7ptr[k]+ne71;
	      if ((bllst7=realloc(bllst7,sizeof(double)*(((int *) bllst7)[0]+ne71+1))) ==NULL )  return 0;
	      for (j=1; j< ne71+1; j++)
		{
		  bllst7[((int *) bllst7)[0]+j]=xd0k[j];
		}
	      ((int *) bllst7)[0]=((int *) bllst7)[0]+ne71;
	      free(xd0k);
	    }
	  else
	    {
	      bllst7ptr[k+1]=bllst7ptr[k];
	    }
	  if (ne8 != 0)
	    {
	      ne81=li[9+2]-li[9+1];
	      if ((rpark=(double*)malloc(sizeof(double)*(ne81+1))) ==NULL )  return 0;
	      ((int *) rpark)[0]=ne81;
	      C2F(unsfdcopy)(&ne81,le81,&moinsun,(rpark+1),&moinsun);
	      
	      bllst8ptr[k+1]=bllst8ptr[k]+ne81;
	      if ((bllst8=realloc(bllst8,sizeof(double)*(((int *) bllst8)[0]+ne81+1))) ==NULL )  return 0;
	      for (j=1; j< ne81+1; j++)
		{
		  bllst8[((int *) bllst8)[0]+j]=rpark[j];
		}
	      ((int *) bllst8)[0]=((int *) bllst8)[0]+ne81;
	      free(rpark);
	    }
	  else
	    {
	      bllst8ptr[k+1]=bllst8ptr[k];
	    }
	}
      else
	{
	  bllst7ptr[k+1]=bllst7ptr[k]+ne7;
	  if ((bllst7=realloc(bllst7,sizeof(double)*(((int *) bllst7)[0]+ne7+1))) ==NULL )  return 0;
	  for (j=0; j< ne7; j++)
	    {
	      bllst7[((int *) bllst7)[0]+j+1]=le77[j];
	    }
	  ((int *) bllst7)[0]=((int *) bllst7)[0]+ne7;

	  bllst8ptr[k+1]=bllst8ptr[k]+ne8;
	  if ((bllst8=realloc(bllst8,sizeof(double)*(((int *) bllst8)[0]+ne8+1))) ==NULL )  return 0;
	  for (j=0; j< ne8; j++)
	    {
	      bllst8[((int *) bllst8)[0]+j+1]=le88[j];
	    }
	  ((int *) bllst8)[0]=((int *) bllst8)[0]+ne8;
	}
      /* 9ieme element de la list ipar*/
      if ((field_name=(char*) malloc(sizeof(char)*5)) ==NULL )  return 0;
      ((char*) field_name)[4]='\0';
      strcpy (field_name,"ipar");
      field_num=MlistGetFieldNumber(li,field_name);
      free(field_name);
      le9=(int*) listentry(li,field_num);
      if (le9[0] == 1)
	{
	  ne9=le9[1];
	  le99=((double *) (le9+4));
	  bllst9ptr[k+1]=bllst9ptr[k]+ne9;
	  if ((bllst9=realloc(bllst9,sizeof(int)*(bllst9[0]+ne9+1))) ==NULL )  return 0;
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
      if ((field_name=(char*) malloc(sizeof(char)*10)) ==NULL )  return 0;
      ((char*) field_name)[9]='\0';
      strcpy (field_name,"blocktype");
      field_num=MlistGetFieldNumber(li,field_name);
      free(field_name);
      le10=(int*) listentry(li,field_num);
      le1010=((int *) (le10+6));      
      if ((bllst10[k]=(char*) malloc(sizeof(char)*2)) ==NULL )  return 0;
      ((char*) bllst10[k])[1]='\0';
      C2F(cvstr)(&one,le1010,bllst10[k],&one,strlen(bllst10[k]));
      /* 11ieme element de la list firing*/
      if ((field_name=(char*) malloc(sizeof(char)*7)) ==NULL )  return 0;
      ((char*) field_name)[6]='\0';
      strcpy (field_name,"firing");
      field_num=MlistGetFieldNumber(li,field_name);
      free(field_name);
      le11=(int*) listentry(li,field_num);
      ne11=le11[2];
      le1111=((double *) (le11+4));
      bllst11ptr[k+1]=bllst11ptr[k]+ne11;
      if ((bllst11=realloc(bllst11,sizeof(double)*(((int *) bllst11)[0]+ne11+1))) ==NULL )  return 0;
      for (j=0; j< ne11; j++)
	{
	  bllst11[((int *) bllst11)[0]+j+1]=le1111[j];
	}
      ((int *) bllst11)[0]=((int *) bllst11)[0]+ne11;
      /* 12ieme element de la list dep_ut*/
      if ((field_name=(char*) malloc(sizeof(char)*7)) ==NULL )  return 0;
      ((char*) field_name)[6]='\0';
      strcpy (field_name,"dep_ut");
      field_num=MlistGetFieldNumber(li,field_name);
      free(field_name);
      le12=(int*) listentry(li,field_num);
      ne12=le12[1];
      me12=le12[2];
      /*le1212=((double *) (le12+3));*/
      bllst12[k]=le12[3];
      bllst12[k+m]=le12[4];
      /* 13ieme element de la list label*/
      if ((field_name=(char*) malloc(sizeof(char)*6)) ==NULL )  return 0;
      ((char*) field_name)[5]='\0';
      strcpy (field_name,"label");
      field_num=MlistGetFieldNumber(li,field_name);
      free(field_name);
      le13=(int*) listentry(li,field_num);
      le1313=((int *) (le13+6));

      n1313=le13[5]-1;
      if ((bllst13[k]=(char*) malloc(sizeof(char)*(n1313+1))) ==NULL )  return 0;
      ((char*) bllst13[k])[n1313]='\0'; 
      C2F(cvstr)(&n1313,le1313,bllst13[k],&one,strlen(bllst13[k]));
     /* 14ieme element de la list nzcross*/
      if ((field_name=(char*) malloc(sizeof(char)*8)) ==NULL )  return 0;
      ((char*) field_name)[7]='\0';
      strcpy (field_name,"nzcross");
      field_num=MlistGetFieldNumber(li,field_name);
      free(field_name);
      le14=(int*) listentry(li,field_num);
      le1414=((double *) (le14+4));
      nzcross[k]=(int)le1414[0];
     /* 15ieme element de la list nmode*/
      if ((field_name=(char*) malloc(sizeof(char)*6)) ==NULL )  return 0;
      ((char*) field_name)[5]='\0';
      strcpy (field_name,"nmode");
      field_num=MlistGetFieldNumber(li,field_name);
      free(field_name);
      le15=(int*) listentry(li,field_num);
      le1515=((double *) (le15+4));
      nmode[k]=(int)le1515[0];
    }
  
  GetRhsVar(2, "i", &m1, &n1, &l1);
  GetRhsVar(3, "i", &m2, &n2, &l2);
  if ((connectmat=malloc((m1*n1+1)*sizeof(int))) ==NULL )  return 0;		  
  connectmat[0]=m1*n1;
  for(i=0;i<n1;i++)
    {
      for(j=1;j<m1+1;j++)
	{
	  connectmat[j+i*m1]=*istk(l1+j+i*m1-1);
	}
    }
  if ((clkconnect=malloc((m2*n2+1)*sizeof(int))) ==NULL )  return 0;		  
  clkconnect[0]=m2*n2;
  for(i=0;i<n2;i++)
    {
      for(j=1;j<m2+1;j++)
	{
	  clkconnect[j+i*m2]=*istk(l2+j+i*m2-1);
	}
    }
 
  if ((corinvec=malloc(sizeof(int))) ==NULL )  return 0;
  corinvec[0]=0;
  header1= (int*) GetData(4);
  m3=header1[1];
  if ((corinvptr=malloc(sizeof(int)*(m3+2))) ==NULL )  return 0;
  corinvptr[0]=m3+1;
  corinvptr[1]=1;
  for (k=1; k <= m3; k++)
    {
      lii=(int*) listentry(header1,k); /*le pointeur sur la kieme vecteur*/
      lc1=((double *) (lii+4));
      nc1=lii[1];
      mc1=lii[2];
      if ((corinvec=realloc(corinvec,sizeof(int)*(corinvec[0]+nc1*mc1+1))) ==NULL )  return 0;
      for (j=0; j< nc1*mc1; j++)
	{
	  corinvec[corinvec[0]+j+1]=(int)lc1[j];
	}
      corinvec[0]=corinvec[0]+nc1*mc1;
      corinvptr[k+1]=corinvptr[k]+nc1*mc1;

    }
  
  GetRhsVar(5, "i", &one, &m4, &l3); 
  solver=*istk(l3);
  
  cpass2(&bllst111,&bllst112,&bllst2,&bllst3,&bllst4,&bllst5,&bllst9,&bllst10,&bllst11,
	 &bllst12,&bllst13,&nmode,&nzcross,&bllst2ptr,&bllst3ptr,&bllst4ptr,&bllst5ptr,&bllst6ptr,&bllst9ptr,&typ_x,
	 &bllst11ptr,&connectmat,&clkconnect,&corinvec,&corinvptr,
	 &iz0,&tevts,&evtspt,&pointi,&outtb,&zcptr,&modptr,&outlnk,&inplnk,
	 &lnkptr,&ordptr,&execlk,&ordclk,&cord,&oord,&zord,&critev,&nb,&ztyp,
	 &nblk,&ndcblk,&subscr,&iord,&ok);
  if (!ok) 
    {
      CreateVar(11,"i", &one, &zeros, &l31);
      CreateVar(12,"i", &one, &zeros, &l32);
      CreateVar(13,"i", &one, &zeros, &l33);
      CreateVar(14,"i", &one, &zeros, &l39);
      CreateVar(15,"i", &one, &zeros, &l40);
      LhsVar(1) = 11;
      LhsVar(2) = 12;
      LhsVar(3) = 13;
      LhsVar(4) = 14;
      LhsVar(5) = 15;
      return 0;
    }
  if (nb > nblk)
    {
      if ((bllst8ptr=realloc(bllst8ptr,sizeof(int)*(nb+2))) ==NULL )  return 0;		  
      bllst8ptr[0]=nb+1;
      if ((bllst7ptr=realloc(bllst7ptr,sizeof(int)*(nb+2))) ==NULL )  return 0;		  
      bllst7ptr[0]=nb+1;
      if ((bllst6ptr=realloc(bllst6ptr,sizeof(int)*(nb+2))) ==NULL )  return 0;		  
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
      if ((bllst6=(double*)realloc(bllst6,sizeof(double)*(((int*) bllst6)[0]+((int*) xcd0)[0]+1))) == NULL ) return 0;
      ppd=(double *)&bllst6[1+((int*) bllst6)[0]];
      ppd=memcpy(ppd,&xcd0[1],sizeof(double)*(((int*) xcd0)[0]));
      ((int*) bllst6)[0]=((int*) bllst6)[0]+((int*) xcd0)[0];
    }
  if (xcd0) free(xcd0);
  /******************* state **************************/
  
  if ((pointiptr=malloc(sizeof(int))) ==NULL )  return 0;		  
  pointiptr[0]=pointi;
  y1 = (double*) (bllst6+1);
  y2 = (double*) (bllst7+1);
  n4=((int*) bllst6)[0];
  n5=((int*) bllst7)[0];
  if (!iz0)
    {
      if ((iz0=malloc(sizeof(int))) ==NULL )  return 0;		  
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
  CreateVar(6,"t", &m31, &n31, &l31);
  CreateListVarFromPtr(6,1,"S", &one, &n32, Str1);
  CreateListVarFromPtr(6,2,"d", &n4, &one,  &y1);
  CreateListVarFromPtr(6,3,"d", &n5, &one,  &y2);
  CreateListVarFromPtr(6,4,"i", &n6, &one,  &y3);
  CreateListVarFromPtr(6,5,"d", &n7, &one,  &y4);
  CreateListVarFromPtr(6,6,"i", &n8, &one,  &y5);
  CreateListVarFromPtr(6,7,"i", &one, &one, &y6);
  CreateListVarFromPtr(6,8,"i", &n9, &one,  &y7);
  LhsVar(1) = 6;
  free(iz0);
  free(tevts);
  free(evtspt);
  free(outtb);
  /********************** sim *****************/
  CreateVar(7,"t", &m33, &n39, &l32);
  CreateListVarFromPtr(7,1,"S", &one, &n40, Str2);
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
      if ((cord=malloc(sizeof(int))) ==NULL )  return 0;		  
      cord[0]=0;
    }
  y25=(int*) (cord+1);
  n27=cord[0]/2;
  if (!oord)
    {
      if ((oord=malloc(sizeof(int))) ==NULL )  return 0;		  
      oord[0]=0;
    }
  y26=(int*) (oord+1);
  n28=oord[0]/2;
  if (!zord)
    {
      if ((zord=malloc(sizeof(int))) ==NULL )  return 0;		  
      zord[0]=0;
    }
  y27=(int*) (zord+1);
  n29=zord[0]/2;
  y28=(int*) (critev+1);
  n30=critev[0];
  if ((nbptr=malloc(sizeof(int))) ==NULL )  return 0;		  
  nbptr[0]=nb;
  y29=nbptr;
  y30=(int*) (ztyp+1);
  n33=ztyp[0];
  if ((nblkptr=malloc(sizeof(int))) ==NULL )  return 0;		  
  nblkptr[0]=nblk;
  y31=nblkptr;
  if ((ndcblkptr=malloc(sizeof(int))) ==NULL )  return 0;		  
  ndcblkptr[0]=ndcblk;
  y32=ndcblkptr;
  if (!subscr)
    {
      if ((subscr=malloc(sizeof(int))) ==NULL )  return 0;		  
      subscr[0]=0;
    }
  y33=(int*) (subscr+1);
  n34=subscr[0]/2;
  y34=(int*) (bllst112+1);
  n35=bllst112[0];
  if (!iord)
    {
      if ((iord=malloc(sizeof(int))) ==NULL )  return 0;		  
      iord[0]=0;
    }
  y35=(int*) (iord+1);
  n36=iord[0]/2;
  y36=(char**) (bllst13+1);
  n37=(int) bllst13[0];
  if ((vecnull=malloc(sizeof(int))) ==NULL )  return 0;		  
  y38=(int*) (vecnull+1);
  n41=0;
  
  CreateListVarFromPtr(7,2,"i", &n41, &one, &y38);
  CreateListVarFromPtr(7,3,"i", &n11, &one, &y9);
  CreateListVarFromPtr(7,4,"i", &n12,&one,  &y10);
  CreateListVarFromPtr(7,5,"i", &n13, &one, &y11);
  CreateListVarFromPtr(7,6,"i", &n14,&one,  &y12);
  CreateListVarFromPtr(7,7,"i", &n15,&one,  &y13);
  CreateListVarFromPtr(7,8,"i", &n16, &one, &y14);
  CreateListVarFromPtr(7,9,"i", &n17, &one, &y15);
  CreateListVarFromPtr(7,10,"i", &n18, &one, &y16);
  CreateListVarFromPtr(7,11,"d", &n19,&one,  &y17);
  CreateListVarFromPtr(7,12,"i", &n20, &one, &y18);
  CreateListVarFromPtr(7,13,"i", &n21, &one, &y19);
  CreateListVarFromPtr(7,14,"i", &n22, &one, &y20);
  CreateListVarFromPtr(7,15,"i", &n23, &one, &y21);
  CreateListVarFromPtr(7,16,"i", &n24, &one, &y22);
  CreateListVarFromPtr(7,17,"i", &n25, &deux, &y23);
  CreateListVarFromPtr(7,18,"i", &n26, &deux, &y24);
  CreateListVarFromPtr(7,19,"i", &n27, &deux, &y25);
  CreateListVarFromPtr(7,20,"i", &n28, &deux,&y26);
  CreateListVarFromPtr(7,21,"i", &n29, &deux, &y27);
  CreateListVarFromPtr(7,22,"i", &n30, &one, &y28);
  CreateListVarFromPtr(7,23,"i", &one, &one, &y29);
  CreateListVarFromPtr(7,24,"i", &n33, &one, &y30);
  CreateListVarFromPtr(7,25,"i", &one, &one, &y31);
  CreateListVarFromPtr(7,26,"i", &one, &one, &y32);
  CreateListVarFromPtr(7,27,"i", &n34, &one, &y33);
  CreateListVarFromPtr(7,28,"i", &n35,&one,  &y34);
  CreateListVarFromPtr(7,29,"i", &n36, &deux, &y35);
  CreateListVarFromPtr(7,30,"S", &n37, &one, y36);
  CreateListVarFromPtr(7,31,"i", &n43, &one, &y41);
  LhsVar(2) = 7;
  
  free(zcptr);
  free(modptr);
  free(inplnk);
  free(outlnk);
  free(lnkptr);
  free(ordptr);
  free(execlk);
  free(ordclk);
  free(cord);
  free(oord);
  free(zord);
  free(critev);
  free(ztyp);
  free(subscr);
  free(iord);
  free(bllst112);
  free(bllst2);
  free(bllst2ptr);
  free(bllst3);
  free(bllst3ptr);
  free(bllst4);
  free(bllst4ptr);
  free(bllst5);
  free(bllst5ptr);
  free(bllst6);
  free(bllst6ptr);
  free(bllst7);
  free(bllst7ptr);
  free(bllst8);
  free(bllst8ptr);
  free(bllst9);
  free(bllst9ptr);
  for(i = 1; i < ((int*) bllst10)[0]+1; i++)
    free(bllst10[i]);
  free(bllst10);
  free(bllst11);
  free(bllst11ptr);
  free(bllst12);
  for(i = 1; i < ((int*) bllst13)[0]+1; i++)
    free(bllst13[i]);
  free(bllst13);  
  free(nzcross);
  free(nmode);
  if(ndcblkptr) free(ndcblkptr);
  if (pointiptr) free(pointiptr);
  if (nbptr) free(nbptr);
  if (nblkptr) free(nblkptr);
  free(connectmat); 
  free(vecnull);

  CreateVar(8,"l", &n10, &one, &l33);
  for (i=0; i < n10; i++)
    {
      n38=strlen(y8[i]);
      y37=*(y8+i);
      CreateListVarFromPtr(8,i+1,"c", &n38, &one, &y37); 
    }
  LhsVar(3) = 8;
  for (i = 1; i <= nb; i++)
    free(bllst111[i]);
  free(bllst111);
  
  if ((solverptr=malloc(sizeof(int))) ==NULL )  return 0;		  
  solverptr[0]=solver;
  y39=solverptr;
  CreateVarFromPtr(9, "i", &one, &one, &y39);
  LhsVar(4) = 9;
  free(solverptr);

  CreateVar(10,"l", &nb, &one, &l40);
  for (i=1; i < nb+1; i++)
    {      
      y40=(int*) (corinvec+corinvptr[i]);
      n42=corinvptr[i+1]-corinvptr[i];
      CreateListVarFromPtr(10,i,"i", &one, &n42, &y40); 
    }
  LhsVar(5) = 10;
  free(corinvec);
  free(corinvptr);
  return 0;
}


