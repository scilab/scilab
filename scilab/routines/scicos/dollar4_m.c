#include "scicos_block4.h"
#include <memory.h>

typedef struct
{         int s;
} dol_struct ;

void dollar4_m(scicos_block *block,int flag)
{
  /* c     Copyright INRIA
     
  Scicos block simulator
  Ouputs delayed input */


  int m,n;
  double *y,*u,*oz;
  dol_struct *ptr;
  m=GetInPortRows(block,1);
  n=GetInPortCols(block,1);
  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);
  oz=GetRealOzPtrs(block,1);

  if (flag==4)
      {*(block->work)=(dol_struct*) scicos_malloc(sizeof(dol_struct));
        ptr=*(block->work);
/*	ptr->s=(int) scicos_malloc(sizeof(int));*/
        ptr->s=GetSizeOfOz(block,1);}
  if (flag ==1 || flag ==6)
      {ptr=*(block->work);
       memcpy(y,oz,m*n*(ptr->s));}
  if (flag == 2)
      {ptr=*(block->work);
       memcpy(oz,u,m*n*ptr->s);}
  if (flag == 5)
      {ptr=*(block->work);
       if(ptr!=NULL) {
          scicos_free(ptr);
       }
      }

}
