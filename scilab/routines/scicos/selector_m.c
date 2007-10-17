#include "scicos_block4.h"
#include "../machine.h"
#include <memory.h>

void selector_m(scicos_block *block,int flag)
 {
  void *u;
  void *y;
  double *z;
  int nu,mu,ic,nev,nin,so;

  z=GetDstate(block);
  nin=GetNin(block);
  ic=(int)z[0];
  if (flag<3)
     {ic=0;
      nev=GetNevIn(block);
      while (nev>=1) 
           {
     	    ic=ic+1;
     	    nev=nev/2;
    	   }
     }
  if (nin>1)
     {
      mu=GetInPortRows(block,ic);
      nu=GetInPortCols(block,ic);
      u=GetInPortPtrs(block,ic);
      so=GetSizeOfOut(block,1);
      y=GetOutPortPtrs(block,1);
      memcpy(y,u,mu*nu*so);
      }
   else
      {
       mu=GetInPortRows(block,1);
       nu=GetInPortCols(block,1);
       u=GetRealInPortPtrs(block,1);
       y=GetRealOutPortPtrs(block,ic);
       so=GetSizeOfIn(block,1);
       memcpy(y,u,mu*nu*so);
       }
}
