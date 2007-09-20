#include "scicos_block4.h"
/*    Copyright INRIA
 *    Scicos block simulator
 *    event delay with discrete counter
 */

#if WIN32
#define NULL    0
#endif

void evtdly4(scicos_block *block,int flag)
{
  double t;
  long long int *i;

  switch(flag)
  {
   /* init */
   case 4  : {/* the workspace is used to store discrete counter value */
              if ((*block->work=scicos_malloc(sizeof(long long int)))==NULL) {
                set_block_error(-16);
                return;
              }
              i=*block->work;
              (*i)=0;
              break;
             }

   /* event date computation */
   case 3  : {
              i=*block->work;
              t=get_scicos_time();
              (*i)++; /*increase counter*/
              block->evout[0]=block->rpar[1]+ \
                              (*i)*block->rpar[0]-t;
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
