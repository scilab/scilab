#include "gw_metanet.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "MALLOC.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
extern int C2F(intsm6loadg) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6saveg) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6prevn2p) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6ns2p) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6p2ns) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6edge2st) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6prevn2st) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6compc) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6concom) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6compfc) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6sconcom) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6pcchna) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6fordfulk) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6johns) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6dijkst) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6frang) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6chcm) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6transc) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6dfs) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6umtree) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6umtree1) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6dmtree) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6tconex) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6flomax) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6kilter) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6busack) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6floqua) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6relax) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6findiso) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6ta2lpd) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6ta2lpu) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6lp2tad) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6lp2tau) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6dfs2) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6diam) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6cent) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6hullcvex) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6clique) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6clique1) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6hamil) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6visitor) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6bmatch) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6knapsk) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6prfmatch) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6permuto) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6mesh2b) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6deumesh) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6bandred) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6meshmesh) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsm6ford) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
static MetanetTable Tab[]=
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
/*-----------------------------------------------------------------------------------*/
int C2F(gw_metanet)()
{  
	Rhs = Max(0, Rhs);
#if _MSC_VER
	#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{	
			char *ExceptionString=GetExceptionString(GetExceptionCode());
			sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
			if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
		}
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
	#endif
#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
