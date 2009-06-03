#include "scicos_block4.h"

/*     Februery 2008 */
/*    Copyright INRIA
 *    Scicos block simulator
 */

#define NX      block->nx


void diffblk_c(scicos_block *block,int flag)
{
  double *y,*u;
  int i;
  int* property=GetXpropPtrs(block);
    switch(flag)
      {
      case 4  : {
        for (i=0;i<NX;i++) property[i]=-1; /* xproperties*/
        break;
      }

      case 6  : { 
	u=GetRealInPortPtrs(block,1);
	//	for (i=0;i<NX;i++)  block->x[i]=u[i];
	break;	
      }

      case 7  : {
	for (i=0;i<NX;i++) property[i]=-1; /* xproperties*/
	break;
      }

      case 0  : {
	u=GetRealInPortPtrs(block,1);
	for (i=0;i<NX;i++) block->res[i]=u[i]-block->x[i];
	break;
      }

      case 1  : {
	y=GetRealOutPortPtrs(block,1);
	for (i=0;i<NX;i++)  y[i]=block->xd[i];
	break;	
      }

      default : break;
      }
}
