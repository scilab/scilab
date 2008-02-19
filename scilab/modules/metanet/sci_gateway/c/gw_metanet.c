
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "gw_metanet.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
{C2F(intsm6loadg),"m6loadg"},
{C2F(intsm6saveg),"m6saveg"},
{C2F(intsm6prevn2p),"m6prevn2p"},
{C2F(intsm6ns2p),"m6ns2p"},
{C2F(intsm6p2ns),"m6p2ns"},
{C2F(intsm6edge2st),"m6edge2st"},
{C2F(intsm6prevn2st),"m6prevn2st"},
{C2F(intsm6compc),"m6compc"},
{C2F(intsm6concom),"m6concom"},
{C2F(intsm6compfc),"m6compfc"},
{C2F(intsm6sconcom),"m6sconcom"},
{C2F(intsm6pcchna),"m6pcchna"},
{C2F(intsm6fordfulk),"m6fordfulk"},
{C2F(intsm6johns),"m6johns"},
{C2F(intsm6dijkst),"m6dijkst"},
{C2F(intsm6frang),"m6frang"},
{C2F(intsm6chcm),"m6chcm"},
{C2F(intsm6transc),"m6transc"},
{C2F(intsm6dfs),"m6dfs"},
{C2F(intsm6umtree),"m6umtree"},
{C2F(intsm6umtree1),"m6umtree1"},
{C2F(intsm6dmtree),"m6dmtree"},
{C2F(intsm6tconex),"m6tconex"},
{C2F(intsm6flomax),"m6flomax"},
{C2F(intsm6kilter),"m6kilter"},
{C2F(intsm6busack),"m6busack"},
{C2F(intsm6floqua),"m6floqua"},
{C2F(intsm6relax),"m6relax"},
{C2F(intsm6findiso),"m6findiso"},
{C2F(intsm6ta2lpd),"m6ta2lpd"},
{C2F(intsm6ta2lpu),"m6ta2lpu"},
{C2F(intsm6lp2tad),"m6lp2tad"},
{C2F(intsm6lp2tau),"m6lp2tau"},
{C2F(intsm6dfs2),"m6dfs2"},
{C2F(intsm6diam),"m6diam"},
{C2F(intsm6cent),"m6cent"},
{C2F(intsm6hullcvex),"m6hullcvex"},
{C2F(intsm6clique),"m6clique"},
{C2F(intsm6clique1),"m6clique1"},
{C2F(intsm6hamil),"m6hamil"},
{C2F(intsm6visitor),"m6visitor"},
{C2F(intsm6bmatch),"m6bmatch"},
{C2F(intsm6knapsk),"m6knapsk"},
{C2F(intsm6prfmatch),"m6prfmatch"},
{C2F(intsm6permuto),"m6permuto"},
{C2F(intsm6mesh2b),"m6mesh2b"},
{C2F(intsm6deumesh),"m6deumesh"},
{C2F(intsm6bandred),"m6bandred"},
{C2F(intsm6meshmesh),"m6meshmesh"},
{C2F(intsm6ford),"m6ford"}
};
/*--------------------------------------------------------------------------*/
int gw_metanet(void)
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
