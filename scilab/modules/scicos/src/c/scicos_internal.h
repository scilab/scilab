/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2013 - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "scicos-def.h"

void makescicosimport(double *x, int *nx,
                      int *xptr, int *zcptr,
                      double *z, int *nz, int *zptr,
                      int *noz, void **oz, int *ozsz, int *oztyp, int *ozptr,
                      double *g, int *ng,
                      int *mod, int *nmod, int *modptr,
                      char **iz, int *izptr, char **uid, int *uidptr, int *inpptr, int *inplnk,
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
