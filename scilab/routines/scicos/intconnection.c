
#include <string.h>
#include "../stack-c.h"
#include "../machine.h"


int connection(int* path_out,int* path_in);
int badconnection(int* path_out,int prt_out,int nout,int* path_in,int prt_in,int nin);
int Message(char* code);


int connection(int* path_out,int* path_in) 
{
  int mlhs,mrhs,ibegin,i;
  static int l1, m1, n1, m2, n2, l2, il, l, l5,ninnout;
  static char name[] = "under_connection" ;
  
  m1=path_out[0];
  n1=n2=1;
  m2=path_in[0];
  
  CreateVar(1, "i", &n1, &m1, &l1);  
  for (i=1; i<=m1; i++)
    *istk(l1+i-1)=path_out[i];
  CreateVar(2, "i", &n2, &m2, &l2);
  for (i=1; i<=m2; i++)
    *istk(l2+i-1)=path_in[i];
  
  Convert2Sci(1);
  Convert2Sci(2);
  
  ibegin=1;  mlhs=1; mrhs=2 ;  
  SciString(&ibegin,name,&mlhs,&mrhs);
  l5=Top - Rhs + ibegin;
  il=iadr(*Lstk(l5));
  l=sadr(il+4);
  ninnout=(int) *stk(l);  
  return ninnout;
}
int badconnection(int* path_out,int prt_out, int nout,int* path_in,int prt_in,int nin) 
{
  int mlhs,mrhs,ibegin,i;
  static int l1, m1, n1, m2, l2, l3, l4, l6, l5;
  static char name[] = "bad_connection" ;
  
  m1=path_out[0];
  n1=1;
  m2=path_in[0];
  
  CreateVar(1, "i", &n1, &m1, &l1);  
  for (i=1; i<=m1; i++)
    *istk(l1+i-1)=path_out[i];
  CreateVar(2, "i", &n1, &n1, &l2);
  *istk(l2)=prt_out;
  CreateVar(3, "i", &n1, &n1, &l3);
  *istk(l3)=nout;
  CreateVar(4, "i", &n1, &m2, &l4);
  for (i=1; i<=m2; i++)
    *istk(l4+i-1)=path_in[i];
  CreateVar(5, "i", &n1, &n1, &l5);
  *istk(l5)=prt_in;
  CreateVar(6, "i", &n1, &n1, &l6);
  *istk(l6)=nin;
  
  Convert2Sci(1);
  Convert2Sci(2);
  Convert2Sci(3);
  Convert2Sci(4);
  Convert2Sci(5);
  Convert2Sci(6);

  ibegin=1;  mlhs=0; mrhs=6 ;  
  SciString(&ibegin,name,&mlhs,&mrhs);
  
  return 0;
}

int Message(char* code) 
{
  int mlhs=0,mrhs=1,ibegin=1;
  int l1, m1=strlen(code), n1=1;
  static char name[] = "x_message" ;
  CreateVar(1, "c", &m1, &n1, &l1);
  strcpy(cstk(l1),code);
  Convert2Sci(1);
  SciString(&ibegin,name,&mlhs,&mrhs);
  return 0;
}




