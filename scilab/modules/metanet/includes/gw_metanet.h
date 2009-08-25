
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
#include "api_common.h"
/*--------------------------------------------------------------------------*/
int gw_metanet(void);
/*--------------------------------------------------------------------------*/
int C2F(intsm6loadg) (char *fname,unsigned long fname_len);
int C2F(intsm6saveg) (char *fname,unsigned long fname_len);
int C2F(intsm6prevn2p) (char *fname,unsigned long fname_len);
int C2F(intsm6ns2p) (char *fname,unsigned long fname_len);
int C2F(intsm6p2ns) (char *fname,unsigned long fname_len);
int C2F(intsm6edge2st) (char *fname,unsigned long fname_len);
int C2F(intsm6prevn2st) (char *fname,unsigned long fname_len);
int C2F(intsm6compc) (char *fname,unsigned long fname_len);
int C2F(intsm6concom) (char *fname,unsigned long fname_len);
int C2F(intsm6compfc) (char *fname,unsigned long fname_len);
int C2F(intsm6sconcom) (char *fname,unsigned long fname_len);
int C2F(intsm6pcchna) (char *fname,unsigned long fname_len);
int C2F(intsm6fordfulk) (char *fname,unsigned long fname_len);
int C2F(intsm6johns) (char *fname,unsigned long fname_len);
int C2F(intsm6dijkst) (char *fname,unsigned long fname_len);
int C2F(intsm6frang) (char *fname,unsigned long fname_len);
int C2F(intsm6chcm) (char *fname,unsigned long fname_len);
int C2F(intsm6transc) (char *fname,unsigned long fname_len);
int C2F(intsm6dfs) (char *fname,unsigned long fname_len);
int C2F(intsm6umtree) (char *fname,unsigned long fname_len);
int C2F(intsm6umtree1) (char *fname,unsigned long fname_len);
int C2F(intsm6dmtree) (char *fname,unsigned long fname_len);
int C2F(intsm6tconex) (char *fname,unsigned long fname_len);
int C2F(intsm6flomax) (char *fname,unsigned long fname_len);
int C2F(intsm6kilter) (char *fname,unsigned long fname_len);
int C2F(intsm6busack) (char *fname,unsigned long fname_len);
int C2F(intsm6floqua) (char *fname,unsigned long fname_len);
int C2F(intsm6relax) (char *fname,unsigned long fname_len);
int C2F(intsm6findiso) (char *fname,unsigned long fname_len);
int C2F(intsm6ta2lpd) (char *fname,unsigned long fname_len);
int C2F(intsm6ta2lpu) (char *fname,unsigned long fname_len);
int C2F(intsm6lp2tad) (char *fname,unsigned long fname_len);
int C2F(intsm6lp2tau) (char *fname,unsigned long fname_len);
int C2F(intsm6dfs2) (char *fname,unsigned long fname_len);
int C2F(intsm6diam) (char *fname,unsigned long fname_len);
int C2F(intsm6cent) (char *fname,unsigned long fname_len);
int C2F(intsm6hullcvex) (char *fname,unsigned long fname_len);
int C2F(intsm6clique) (char *fname,unsigned long fname_len);
int C2F(intsm6clique1) (char *fname,unsigned long fname_len);
int C2F(intsm6hamil) (char *fname,unsigned long fname_len);
int C2F(intsm6visitor) (char *fname,unsigned long fname_len);
int C2F(intsm6bmatch) (char *fname,unsigned long fname_len);
int C2F(intsm6knapsk) (char *fname,unsigned long fname_len);
int C2F(intsm6prfmatch) (char *fname,unsigned long fname_len);
int C2F(intsm6permuto) (char *fname,unsigned long fname_len);
int C2F(intsm6mesh2b) (char *fname,unsigned long fname_len);
int C2F(intsm6deumesh) (char *fname,unsigned long fname_len);
int C2F(intsm6bandred) (char *fname,unsigned long fname_len);
int C2F(intsm6meshmesh) (char *fname,unsigned long fname_len);
int C2F(intsm6ford) (char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_METANET__ */
/*--------------------------------------------------------------------------*/

