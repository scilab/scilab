/* Copyright INRIA */

/************************************
 *     (very) simple example 1 
 *     -->link('ext1c.o','ext1c','C'); 
 *     -->a=[1,2,3];b=[4,5,6];n=3; 
 *     -->c=call('ext1c',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d') 
 *     c=a+b 
 ************************************/

int ext1c(n, a, b, c)
     int *n;
     double *a, *b, *c;
{   
  int k;
  for (k = 0; k < *n; ++k) 
    {
      c[k] = a[k] + b[k];
    }
  return(0);
}

