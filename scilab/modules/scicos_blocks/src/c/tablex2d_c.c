#include <math.h>
#include "scicos_block4.h"



//  10/2007 --------
// Copyright INRIA



#define InterpExtrap    0
#define InterpEndValue  1
#define InputNearest    2
#define InputBelow      3
#define InputAbove      4


double computeZ(double *X, double *Y, double *Z, int nx, int ny, int EXTRM, double x, double y);
int indexfinder(double x, int n, double *LT);

void tablex2d_c(scicos_block *block,int flag)
{
  double *y, *u1, *u2;
  double *X, *Y, *Z;

  int Nx= block->ipar[0];
  int Ny= block->ipar[1];
  int EXTRM= block->ipar[2];
 
  X = block->rpar;
  Y = X+Nx; 
  Z = Y+Ny;

  switch(flag)
  {
   /* init */
   case 4  :

   case 1  : 
     u1=GetRealInPortPtrs(block,1);
     u2=GetRealInPortPtrs(block,2);
     y =GetRealOutPortPtrs(block,1);
     y[0]=computeZ(X, Y, Z, Nx, Ny, EXTRM, u1[0], u2[0]);
     break;
   case 3  : 
   case 5  :
   default : break;
  }
}

double computeZ(double *X, double *Y, double *Z, int nx, int ny, int EXTRM, double x, double y)
{
  int i,j,im,jm;
  double z,fq11,fq12,fq21,fq22,z1,z2,T;

  i=indexfinder(x, nx, X); 
  j=indexfinder(y, ny, Y);  

  if (EXTRM==InputNearest){
    if (x>=X[nx-1]) { x=X[nx-1];}
    else if(x<=X[0]){x=X[0];}
    else if ((X[i]-x)>(x-X[i-1])){x=X[i-1];}
    else {x=X[i];};
    
    if (y>=Y[ny-1]){y=Y[ny-1];}
    else if(y<=Y[0]){y=Y[0];}
    else if ((Y[j]-y)>(y-Y[j-1])){y=Y[j-1];}
    else {y=Y[j];};
    
  }else if (EXTRM==InputBelow){
    if (x>=X[nx-1]){x=X[nx-1];}
    else if(x<=X[0]){x=X[0];}
    else {if (x<X[i]) x=X[i-1];}
    
    if (y>=Y[ny-1]){y=Y[ny-1];}
    else if(y<=Y[0]){y=Y[0];}
    else {if (y<Y[j]) y=Y[j-1];}
    
  }else if (EXTRM==InputAbove){
    if (x>=X[nx-1]){x=X[nx-1];}
    else if(x<=X[0]){x=X[0];}
    else {if (x>X[i-1]) x=X[i];}
    
    if (y>=Y[ny-1]){y=Y[ny-1];}
    else if(y<=Y[0]){y=Y[0];}
    else {if (y>Y[j-1]) y=Y[j];}
    
  }else if (EXTRM==InterpEndValue){
    if (x>=X[nx-1]){x=X[nx-1];}
    else if(x<=X[0]){x=X[0];};
    
    if (y>=Y[ny-1]){y=Y[ny-1];}
    else if(y<=Y[0]){y=Y[0];};
      
  }else{ /* InterpExtrap */
  } 
  
  im=i-1; jm=j-1;  
  fq11=Z[im+jm*nx];
  fq21=Z[i+jm*nx];
  fq12=Z[im+j*nx];
  fq22=Z[i+j*nx];

  z=(X[i]-X[im])*(Y[j]-Y[jm]);
  z1=(fq11*(X[i]-x)+fq21*(x-X[im]))*(Y[j]-y);
  z2=(fq12*(X[i]-x)+fq22*(x-X[im]))*(y-Y[jm]);
  T=(z1+z2)/z;
  return T;
}

int indexfinder(double x, int n, double *LT)
{
  int i1, i2, i_mid;
  
  /* if X(k-1)<= x < X(k) then i2=k */
  if (x<=LT[0]  ) return 1;
  if (x>=LT[n-1]) return n-1;
  i1=0;
  i2=n-1;

  while (i1!=i2-1){
    i_mid=(int)((i1+i2)/2);
    if (x>=LT[i_mid])  i1=i_mid;
    else          i2=i_mid;
  }	
  return i2;
}	
