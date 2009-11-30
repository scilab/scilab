#include <scicos_block.h>
#include <math.h>
void pendel_c(scicos_block *block, int flag) { if(flag==1){
block->outptr[0][0]=block->x[0];
}
if(flag==0){
double g=9.81;
double m = block->rpar[0];
double l = block->rpar[1];
double c = block->rpar[2];
double d = block->rpar[3];
double phi=block->x[0]; //phi
double omega=block->x[1]; //omega
double M = block->inptr[0][0];
block->xd[0]=block->x[1];
block->xd[1]=1/m/l/l*(g*l*cos(phi)*m-c*phi-M-d*omega);
}
}
