/* Copyright INRIA */
#include "../machine.h"
#include "import.h"
extern struct {
    integer nblk, nordptr, nout, ng, nrwp, niwp, ncord, noord, 
	    nzord;
} C2F(cossiz);

extern struct {
    integer iero;
} C2F(ierode);

ScicosImport  scicos_imp;

extern struct {
    integer kfun;
} C2F(curblk);

int C2F(grblkdassl)(neq, t, xc, xd,ng1, g,rpar,ipar)
     integer *neq;
     double *t, *xc, *xd;
     integer *ng1,*ipar;
     double *g,*rpar;
{
  integer nordclk, iig;

 
  C2F(ierode).iero = 0;
  nordclk=scicos_imp.ordptr[C2F(cossiz).nordptr-1]-1;
  C2F(zdoit)(neq,g,scicos_imp.zcptr, xd, xc, scicos_imp.xptr, scicos_imp.z, scicos_imp.zptr, scicos_imp.iz, 
	     scicos_imp.izptr, t, 
	     scicos_imp.tevts, scicos_imp.evtspt, &scicos_imp.nevts, &scicos_imp.pointi, 
	     scicos_imp.inpptr, scicos_imp.inplnk, scicos_imp.outptr, scicos_imp.outlnk, 
	     scicos_imp.lnkptr, 
	     scicos_imp.clkptr, scicos_imp.ordptr, &C2F(cossiz).nordptr, scicos_imp.ordclk, 
	     &nordclk, scicos_imp.cord, 
	     scicos_imp.oord, scicos_imp.zord, 
	     scicos_imp.critev, scicos_imp.rpar, scicos_imp.rpptr, scicos_imp.ipar, 
	     scicos_imp.ipptr, scicos_imp.funptr, scicos_imp.funtyp, scicos_imp.outtb, 
	     &C2F(ierode).iero);
  if(*ng1>-1){
    for (iig=0;iig<*ng1;++iig){
      if (scicos_imp.mask[iig]>0){
	g[iig]=1;
      }
    }
  }
  
  return 0;
}


int C2F(grblk)(neq, t, xc, ng1, g)
     integer *neq;
     double *t, *xc;
     integer *ng1;
     double *g;
     
     
     /*
       !purpose 
       interface to grbl1 at the lsodar format 
       !calling sequence 
       neq   : integer  the size of the continuous state
       t     : current time 
       xc    : double precision vector contains the continuous state
       g     : computed zero crossing surface (see lsodar) 
       ng1   : size ng or -1 if without mask 
       !
     */
     
     /* Local variables */
     
{ integer nordclk, iig;
 C2F(ierode).iero = 0;
 nordclk=scicos_imp.ordptr[C2F(cossiz).nordptr-1]-1;
 C2F(zdoit)(neq, g,scicos_imp.zcptr,xc, xc, scicos_imp.xptr, scicos_imp.z, scicos_imp.zptr, scicos_imp.iz, 
	    scicos_imp.izptr, t, 
	    scicos_imp.tevts, scicos_imp.evtspt, &scicos_imp.nevts, &scicos_imp.pointi, 
	    scicos_imp.inpptr, scicos_imp.inplnk, scicos_imp.outptr, scicos_imp.outlnk, 
	    scicos_imp.lnkptr, 
	    scicos_imp.clkptr, scicos_imp.ordptr, &C2F(cossiz).nordptr, scicos_imp.ordclk, 
	    &nordclk, scicos_imp.cord, 
	    scicos_imp.oord, scicos_imp.zord, 
	    scicos_imp.critev, scicos_imp.rpar, scicos_imp.rpptr, scicos_imp.ipar, 
	    scicos_imp.ipptr, scicos_imp.funptr, scicos_imp.funtyp, scicos_imp.outtb, 
	    scicos_imp.iwa,&C2F(ierode).iero);

 /* printf("surface_g1 %f \n", g[0]);
    printf("surface_g1 %f \n\n", g[1]);*/
 if(*ng1>-1){
   for (iig=0;iig<*ng1;++iig){
     if (scicos_imp.mask[iig]>0){
       g[iig]=1;
     }
   }
 }
 return 0;
}

