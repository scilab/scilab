#include "scicos_block4.h"

#if WIN32
#define NULL    0
#endif

void m_frequ(scicos_block *block,int flag)
{
  double *mat;
  double *Dt;
  double *off;
  long *icount;
  double t;
  long long *counter;
  int m;
  mat=GetRealOparPtrs(block,1);
  Dt=GetRealOparPtrs(block,2);
  off=GetRealOparPtrs(block,3);
  icount=Getint32OparPtrs(block,4);
  m=GetOparSize(block,1,1);
  
  switch(flag)
  {

   case 4 :  {/* the workspace is used to store discrete counter value */
              if ((*block->work=scicos_malloc(sizeof(long long int)*2))==NULL) {
                set_block_error(-16);
                return;
              }
              counter=*block->work;
              if (*icount!=0) (*counter)=(int)mat[0];
              else *counter=0;
              (*(counter+1))=*icount;
              break;
             }

   /* event date computation */
   case 3  : {
              counter=*block->work;
              t=get_scicos_time(); 
              *counter+=(int)mat[*(counter+1)]; /*increase counter*/
              block->evout[(int)mat[*(counter+1)+m]-1]=*off+((double)*counter/(*Dt))-t;
              (*(counter+1))++;
	      *(counter+1)=*(counter+1)%m;
              break;
             }

   /* finish */
   case 5  : {
              scicos_free(*block->work); /*free the workspace*/
              break;
             }

   default : break;
  }
}
