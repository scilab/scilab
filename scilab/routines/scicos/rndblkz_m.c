#include "scicos_block4.h"
#include "../machine.h"
#include <math.h>
extern double C2F(urand)();
void rndblkz_m(scicos_block *block,int flag)
{
  double *y_r;
  double *y_i;
  double *rpar;
  double *z;
  int *ipar;
  int ny,my,i,iy_r,iy_i;
  double sr,si,tl;

  my=GetOutPortRows(block,1);
  ny=GetOutPortCols(block,1);
  ipar=GetIparPtrs(block);
  rpar=GetRparPtrs(block);
  y_r=GetRealOutPortPtrs(block,1);
  y_i=GetImagOutPortPtrs(block,1);
  z=GetDstate(block);

  if (flag==2||flag==4) {
    iy_r=(int)z[0];
    iy_i=(int)z[1];
    if (ipar[0]==0) {
      for (i=0;i<my*ny;i++) {
         *(z+i+2)=C2F(urand)(&iy_r);
         *(z+i+2+my*ny)=C2F(urand)(&iy_i);
      }
    }
    else {
      for (i=0;i<my*ny;i++) {
        do {
         sr=2.0*C2F(urand)(&iy_r)-1.0;
         si=2.0*C2F(urand)(&iy_r)-1.0;
         tl=sr*sr+si*si;
        } while(tl>1.0);
        z[i+2]= sr*(sqrt(-2.0*log(tl)/tl));
        do {
         sr=2.0*C2F(urand)(&iy_i)-1.0;
         si=2.0*C2F(urand)(&iy_i)-1.0;
         tl=sr*sr+si*si;
        } while(tl>1.0);
        z[i+2+my*ny]= sr*(sqrt(-2.0*log(tl)/tl));
      }
    }
    *(z)=iy_r;
    *(z+1)=iy_i;
  }

  if (flag==1||flag==6) {
    for (i=0;i<my*ny;i++) {
      *(y_r+i)=*(rpar+i)+(*(rpar+i+2*my*ny))*(*(z+i+2));
      *(y_i+i)=*(rpar+i+my*ny)+(*(rpar+i+3*my*ny))*(*(z+i+2+my*ny));
    }
  }
}

