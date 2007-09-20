
#include "../machine.h"
#include "scicos_block4.h"

extern int C2F(dmmul)();
extern int C2F(dmmul1)();


void gainblk(scicos_block *block,int flag)
{
  int i;

  double *u; double *y;
  int nu,ny,my;
  double *rpar;
  int nrpar;

  nu=GetInPortRows(block,1);
  ny=GetOutPortRows(block,1);
  my=GetOutPortCols(block,1);

  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);

  nrpar=GetNrpar(block);

  rpar=GetRparPtrs(block);

  if (nrpar==1){
    for (i=0;i<nu*my;++i){
     y[i]=rpar[0]*u[i];
    }
  }else{
    C2F(dmmul)(rpar,&ny,u,&nu,y,&ny,&ny,&nu,&my);
  }
}
