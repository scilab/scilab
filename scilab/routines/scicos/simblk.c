/* Copyright INRIA */
#include "../machine.h"
#include "import.h"
extern struct {
    integer nblk, nordptr, nout, ng, nrwp, niwp, ncord, noord, nzord;
} C2F(cossiz);

extern struct {
    integer iero;
} C2F(ierode);

ScicosImport  scicos_imp;
extern struct {
    integer kfun;
} C2F(curblk);

int C2F(simblk)(neq, t, xc, xcdot)
     integer *neq;
     double *t, *xc, *xcdot;
    
     
     /* 
	!purpose 
	compute state derivative of the continuous part
	!calling sequence 
	neq   : integer the size of the  continuous state
	t     : current time 
	xc    : double precision vector whose  contains the continuous state. 
	xcdot : double precision vector, contain the computed derivative 
	of the state 
	*/
    { integer nordclk;
	C2F(ierode).iero = 0;
	nordclk=scicos_imp.ordptr[C2F(cossiz).nordptr-1]-1;
	C2F(odoit)(neq, xcdot, xc, scicos_imp.xptr, scicos_imp.z, scicos_imp.zptr, scicos_imp.iz, 
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
	    
    }
 
