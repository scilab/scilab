#include "scicos_block4.h"

/*     February 2008 */
/*    Copyright INRIA
 *    Scicos block simulator
 */

#define NX      block->outsz[0]


void constraint_c(scicos_block *block,int flag)
{
  double *y,*y2,*u;
  int i;
  int* property=GetXpropPtrs(block);

    switch(flag)
      {
      case 4  : {
	for (i=0;i<NX;i++) property[i]=-1; /* xproperties*/
 	break;
      }

      case 7  : {
	for (i=0;i<NX;i++){
	  property[i] = block->ipar[i];
	}
	break;
      }

      case 0  : {/* the workspace is used to store discrete counter value */
	u=GetRealInPortPtrs(block,1); 	
	for (i=0;i<NX;i++) block->res[i]=u[i];	
	break;
      }

      case 1  : { 
	y=GetRealOutPortPtrs(block,1);
	for (i=0;i<NX;i++)  y[i]=block->x[i];	

	if (GetNout(block)==2) {
	  y2=GetRealOutPortPtrs(block,2);
	  for (i=0;i<NX;i++)  y2[i]=block->xd[i];
	}
	break;	
      }

      default : break;
      }
}
