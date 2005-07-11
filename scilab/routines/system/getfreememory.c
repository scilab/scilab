/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "getfreememory.h"
#ifdef WIN32
#include "../wsci/win_mem_alloc.h" /* MALLOC */
#else
#include "../sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(intgetfreememory) _PARAMS((char *fname))
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
	memorysizeKO=stat.dwAvailPhys/kooctet;
#else
	#if defined(hpuxx)
	/*	pstat_getstatic(&pst, sizeof(pst), (size_t) 1, 0);
		memorysizeKO=(pst.psd_free)/kooctet;*/
		memorysizeKO=0;
	#else /* Linux ,Solaris and others */
		#if defined(__APPLE__) 
		{
			vm_statistics_data_t page_info;
			vm_size_t pagesize;
			mach_msg_type_number_t count;
			kern_return_t kret;

			pagesize = 0;
			kret = host_page_size (mach_host_self(), &pagesize);
			count = HOST_VM_INFO_COUNT;

			kret = host_statistics (mach_host_self(), HOST_VM_INFO,(host_info_t)&page_info, &count);
			memorysizeKO = page_info.free_count*pagesize / 1024;
		}
		#else /* Linux ,Solaris and others */
			memorysizeKO=(sysconf(_SC_AVPHYS_PAGES)*sysconf(_SC_PAGESIZE))/kooctet;
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
