#include <string.h>
#include "../stack-c.h"
#include "../machine.h"

void str2sci(char** x,int n,int m)
{
  /* C string  var2vec  to scilab
     x is supposed to be a fortran image of var2vec result
     Copyright INRIA */
      
  int l,il,zero=0,err,n1,i,m1=0;
      
  if (Top >= Bot) {
    i=18;
    C2F(error)(&i);
  } else {
    Top = Top + 1;
    il = iadr(*Lstk(Top));
    l = sadr(il+6);
  }
  
  err = l + n*m - *Lstk(Bot);
  if (err > 0) {
    i=17;
    C2F(error)(&i);
    return;
  }
  *istk(il) = 10;
  *istk(il+1) = n;
  *istk(il+2) = m;
  *istk(il+3) = 0;
  *istk(il+4) = 1;
  for (i = 1; i <= n*m; i++){
    n1=strlen(x[i-1]);
    *istk(il+4+i) =  *istk(il+4+i-1)+n1;
  if (n1 > 0) C2F(cvstr)(&n1,istk(il+m*n+5 -1 + *istk(il+3+i)),x[i-1],&zero,(unsigned long) n1);
  m1=m1+n1;
  }
  *Lstk(Top+1) = l + m1;      
                
} 
      
