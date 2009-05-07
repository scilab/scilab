#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#define MMAX 1408

double dclock()
{
   double time;
   struct timeval t;

   gettimeofday(&t,NULL);
   time = (double)t.tv_sec + 1.0e-6*(double)t.tv_usec;
   return(time);

}

void main(int argc, char*argv[]){
  int
M1,M2,M11,M21,R,MYPROW1,MYPCOL1,CTXT1,MYID,ICTXT,CTXT2,MYPROW2,MYPCOL2;
  int MYPROW,MYPCOL,NPROW,NPCOL1,NPROW1,NPCOL2,NPROW2,NPCOL,INFO,NBP,
I,J,k;
  double tim1,tim2,timtot;
  int twoone,onetwo;
  int M,DEM;
  double *A,*B,*C,*T,*T1,*T2;
  double alpha=1.0,beta=0.0;
  int DESCC1[9],DESCC2[9],DESCIC[9];
  FILE *str, *dge,*win;

  int zero=0,UN=1,TWO=2,FOUR=4;
  char Ori='r',All='A',No='n';

  NBP=4;
  NPROW1=2;
  NPCOL1=2;
  NPROW2=2;
  NPCOL2=2;
  NPCOL=4;
  CTXT1=-1;
  CTXT2=-1;


  blacs_pinfo_(&MYID, &NBP);
  blacs_get_(&zero, &zero, &ICTXT);
  blacs_gridinit_(&ICTXT,&Ori,&TWO,&TWO);
  blacs_gridinfo_(&ICTXT,&NPROW,&NPCOL,&MYPROW,&MYPCOL);

  A=(double*)malloc(MMAX*MMAX*sizeof(double)/4);
  B=(double*)malloc(MMAX*MMAX*sizeof(double)/4);
  C=(double*)malloc(MMAX*MMAX*sizeof(double)/4);
/*   for (M=128;M<MMAX+1;M+=128) */
/*     { */
  M=MMAX;
  DEM=M/2;
  R=M/4;
  descset_(&DESCC1,&M,&M,&R,&R,&zero,&zero,&ICTXT,&DEM);
  blacs_barrier_(&ICTXT, &All);

  for(J=0;J<DEM;J++)
    for (I=0;I<DEM;I++)
      {
        A[I+(DEM*J)]=MYID+0.0;
        B[I+(DEM*J)]=MYID+1.0;
      }
  blacs_barrier_(&ICTXT, &All);
  /* **************** DEBUT DE PDGEMM ************* */
  tim2=0.0;
  for (k=0;k<10;k++)
    {
      tim1=dclock();

pdgemm_(&No,&No,&M,&M,&M,&alpha,A,&UN,&UN,DESCC1,B,&UN,&UN,DESCC1,&beta,C,

&UN,&UN,DESCC1);
      tim1=dclock()-tim1;
      blacs_barrier_(&ICTXT, &All);
      tim2+=tim1;
    }
  if (MYID==0)
    {
      /*          fprintf(dge,"%d %lf\n",M,tim2/k); */
      printf("dge, %d %lf\n",M,tim2/k);
    }
  /* **************** FIN DE PDGEMM ************* */
  /*    }  */
}
