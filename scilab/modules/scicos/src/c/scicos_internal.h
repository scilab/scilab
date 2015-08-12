/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2013 - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "scicos-def.h"

void makescicosimport(double *x, int *nx,
                      int *xptr, int *zcptr,
                      double *z, int *nz, int *zptr,
                      int *noz, void **oz, int *ozsz, int *oztyp, int *ozptr,
                      double *g, int *ng,
                      int *mod, int *nmod, int *modptr,
                      int *iz, int *izptr, int *uid, int *uidptr, int *inpptr, int *inplnk,
                      int *outptr, int *outlnk, void **outtbptr, int *outtbsz, int *outtbtyp,
                      outtb_el *outtb_elem, int *nelem,
                      int *nlnk, double *rpar, int *rpptr, int *ipar, int *ipptr,
                      void **opar, int *oparsz, int *opartyp, int *opptr,
                      int *nblk, int *subs, int *nsubs,
                      double *tevts, int *evtspt, int *nevts, int *pointi,
                      int *iord, int *niord, int *oord, int *noord, int *zord, int *nzord,
                      int *funptr, int *funtyp, int *ztyp,
                      int *cord, int *ncord, int *ordclk, int *nordclk, int *clkptr,
                      int *ordptr, int *nordptr, int *critev,  int *iwa, scicos_block *blocks,
                      double *t0, double *tf, double *Atol, double *rtol, double *ttol, double *deltat, double *hmax,
                      int *xprop, double *xd);

extern SCICOS_IMPEXP COSDEBUGCOUNTER_struct C2F(cosdebugcounter);
extern SCICOS_IMPEXP RTFACTOR_struct C2F(rtfactor);
extern SCICOS_IMPEXP SOLVER_struct C2F(cmsolver);
extern SCICOS_IMPEXP CURBLK_struct C2F(curblk);
extern SCICOS_IMPEXP COSDEBUG_struct C2F(cosdebug);
extern SCICOS_IMPEXP COSHLT_struct C2F(coshlt);
extern SCICOS_IMPEXP DBCOS_struct C2F(dbcos);
extern SCICOS_IMPEXP COSTOL_struct C2F(costol);
extern SCICOS_IMPEXP COSERR_struct coserr;
