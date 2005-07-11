/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "getmemorysize.h"
#ifdef WIN32
#include "../wsci/win_mem_alloc.h" /* MALLOC */
#else
#include "../sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(intgetmemorysize) _PARAMS((char *fname))
{
#define kooctet 1024
	int memorysizeKO=0;
	static int l1,n1,m1;
	int *paramoutINT=NULL;

#if defined(WIN32)
	MEMORYSTATUS stat;
#endif

#if defined(hpuxx)
	struct pst_static pst;
#endif

	paramoutINT=(int*)MALLOC(sizeof(int));

	Rhs=Max(Rhs,0);
	CheckRhs(0,0) ;
	CheckLhs(0,1);

#if defined(WIN32)
	GlobalMemoryStatus (&stat);
	memorysizeKO=stat.dwTotalPhys/kooctet;
#else
	#if defined(hpuxx)
		pstat_getstatic(&pst, sizeof(pst), (size_t) 1, 0);
		memorysizeKO=(pst.physical_memory)/kooctet;
	#else 
		#if defined(__APPLE__) 
		{
			size_t len;
			int total;
			int mib[2];

			mib[0] = CTL_HW;
			mib[1] = HW_PHYSMEM;
			len = sizeof (total);
 
			sysctl(mib, 2, &total, &len, NULL, 0);
			memorysizeKO = total/1024;
		}
		#else /* Linux ,Solaris and others */
			memorysizeKO=(sysconf(_SC_PHYS_PAGES)*sysconf(_SC_PAGESIZE))/kooctet;
		#endif
	#endif
#endif
	n1=1;
	*paramoutINT=memorysizeKO;
	CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);
	LhsVar(1)=Rhs+1;

	C2F(putlhsvar)();
	if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
