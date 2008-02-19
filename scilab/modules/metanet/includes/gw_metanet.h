
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_METANET__
#define __GW_METANET__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int gw_metanet(void);
/*--------------------------------------------------------------------------*/
int C2F(intsm6loadg) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6saveg) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6prevn2p) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6ns2p) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6p2ns) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6edge2st) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6prevn2st) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6compc) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6concom) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6compfc) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6sconcom) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6pcchna) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6fordfulk) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6johns) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6dijkst) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6frang) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6chcm) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6transc) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6dfs) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6umtree) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6umtree1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6dmtree) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6tconex) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6flomax) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6kilter) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6busack) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6floqua) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6relax) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6findiso) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6ta2lpd) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6ta2lpu) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6lp2tad) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6lp2tau) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6dfs2) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6diam) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6cent) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6hullcvex) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6clique) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6clique1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6hamil) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6visitor) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6bmatch) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6knapsk) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6prfmatch) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6permuto) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6mesh2b) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6deumesh) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6bandred) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6meshmesh) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intsm6ford) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_METANET__ */
/*--------------------------------------------------------------------------*/

