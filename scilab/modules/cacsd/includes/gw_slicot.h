/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_SLICOT_H__
#define __GW_SLICOT_H__
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
#ifndef __DEF_MXARRAY__
#define __DEF_MXARRAY__
typedef int mxArray;
typedef int Gatefunc __PARAMS((int nlhs,mxArray *plhs[],int nrhs,mxArray *prhs[]));
#endif
/*-----------------------------------------------------------------------------------*/ 
extern Gatefunc C2F(sident);
extern Gatefunc C2F(sorder);
extern Gatefunc C2F(findbd);
extern Gatefunc C2F(intmucomp);
extern Gatefunc C2F(intricc2);
extern Gatefunc C2F(inthinf);
extern Gatefunc C2F(intdhinf);
extern Gatefunc C2F(intlinmeq);
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_slicot)(void);
/*-----------------------------------------------------------------------------------*/
#endif /*  __GW_SLICOT_H__ */
/*-----------------------------------------------------------------------------------*/

