#include "scicos_block4.h"
# include "../machine.h"

/* A swithcing mechansim for building hybrid automata */
/* Masoud Najafi, 2007, INRIA */

void automat(scicos_block *block,int flag)
{
  double * y0, *y1, *ui;
  double* g=block->g;
  double* x=block->x;
  double* xd=block->xd;
  double* res=block->res; 
  void**   work=block->work;
  double* rpar=block->rpar;
  double* evout=block->evout;

  int* ipar=block->ipar;
  int* jroot=block->jroot;
  int  nevprt=block->nevprt;
  int* insz=block->insz;
  int ng=block->ng;

  int* Mode;
  int  NMode, NX, Minitial,i,j,k,Mi,Mf,indice;
  int* property;
  int* iparXp;
  int* iparCx;
  double* rparX0;
  int test;
  NMode=ipar[0];
  Minitial=ipar[1];
  NX=ipar[2];
  iparXp=ipar+3;
  iparCx=iparXp+NX*NMode;
  rparX0=rpar;


  if (flag ==4){/*----------------------------------------------------------*/
    if ((*work=scicos_malloc(sizeof(int)*(2+NX)))== NULL ) {set_block_error(-16); return; }
    Mode=*work;
    property=Mode+2;
    Mode[0]=Minitial;/*Current Mode;*/
    Mode[1]=Minitial;/* Previous Mode*/
    for (i=0;i<NX;i++) property[i]=0; /* xproperties*/
    for (i=0;i<NX;i++) x[i]=rparX0[i];

  }else if (flag ==5){/**----------------------------------------------------------*/
    scicos_free(*work);
  }else if  (flag ==1 || flag ==6){/*----------------------------------------------------------*/
    y0=GetRealOutPortPtrs(block,1);
    y1=GetRealOutPortPtrs(block,2);

    Mode=*work;
    Mi=Mode[0];
    y0[0]=Mi;/*current Mode*/
    y0[1]=Mode[1];/*prevous Mode*/
    for (i=0;i<NX;i++){
      y1[i]=x[i];
      y1[i+NX]=xd[i];
    }
  }else if (flag==0){ /*----------------------------------------------------------*/
    Mode=*work;
    Mi=Mode[0];
    ui=GetRealInPortPtrs(block,Mi);
    for (i=0;i<NX;i++)
      res[i]= ui[i];

  }else if (flag==7){/*----------------------------------------------------------*/
    Mode=*work;
    property=Mode+2;
    Mi=Mode[0];
    for (i=0;i<NX;i++)
      property[i] = iparXp[(Mi-1)*NX+i];

    set_pointer_xproperty(property);
  }else if (flag==9){/*----------------------------------------------------------*/
    Mode=*work;
    Mi=Mode[0];
    ui=GetRealInPortPtrs(block,Mi);

    for (j=0;j<ng;j++) g[j]=0;
    for (j=0;j<insz[Mi-1]-2*NX;j++) {
      g[j]=ui[j+2*NX];
      }

  }else if ((flag==3)&&(nevprt<0)){
    Mode=*work;
    Mi=Mode[0];
    indice=0;
    for (i=1;i<Mi;i++)  
      indice+=insz[i-1]-2*NX; /*number of modes before Mi_th Mode */
    for (k=0;k<insz[Mi-1]-2*NX;k++)
      if(jroot[k]==1){
	evout[0]=0.0;
	break;
      }
  }else if ((flag==2)&&(nevprt<0)){/*----------------------------------------------------------*/
    Mode=*work;
    Mi=Mode[0];
    indice=0;
    Mf=Mi;/* in case where the user has defined a wrong mode destination or ZC direction.*/
    for (i=1;i<Mi;i++)  
      indice+=insz[i-1]-2*NX; /*number of modes before Mi_th Mode*/
    test=0;
    for (k=0;k<insz[Mi-1]-2*NX;k++){
      if(jroot[k]==1){
	Mf=iparCx[indice+k]; 
	Mode[0]=Mf;/* saving the new Mode*/
	Mode[1]=Mi;/* saving the previous Mode*/
	test=1;
	break;
      }
    }
    if (test==0){
      for (k=0;k<insz[Mi-1]-2*NX;k++) if(jroot[k]==-1) break;
      /*      sciprint("\n\r Warning!: In Mode=%d, the jump condition #%d has crossed zero in negative dierction",Mi,k+1); */
    }
    ui=GetRealInPortPtrs(block,Mf);
    for (i=0;i<NX;i++)
      x[i]=ui[i+NX]; /*reinitialize the states*/    
  }
}

