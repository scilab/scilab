/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
	#include <Windows.h>
#else
	#include <sys/resource.h>
#endif
#include "../includes/getmaxMALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
IMPORT_EXPORT_MALLOC_DLL unsigned long GetLargestFreeMemoryRegion(void)
{
#if _WIN64
	/* we need to limite values on 32 bits for Scilab :( */
	return MAXLONG32;
#else
	SYSTEM_INFO systemInfo;
	VOID *p = 0;
	MEMORY_BASIC_INFORMATION mbi;
	unsigned long largestSize = 0;

	GetSystemInfo(&systemInfo);

	while(p < systemInfo.lpMaximumApplicationAddress)
	{
		SIZE_T dwRet = VirtualQuery(p, &mbi, sizeof(mbi));
		if (dwRet > 0)
		{
			if (mbi.State == MEM_FREE)
			{
				if (largestSize < mbi.RegionSize)
				{
					largestSize = (unsigned long) mbi.RegionSize;
				}
			}
			p = (void*) (((char*)p) + mbi.RegionSize);
		}
		else
		{
			p = (void*) (((char*)p) + systemInfo.dwPageSize);
		}
	}
	return largestSize;
#endif
}
/*-----------------------------------------------------------------------------------*/
#else
IMPORT_EXPORT_MALLOC_DLL unsigned long GetLargestFreeMemoryRegion(void)
{
	struct rlimit rlim;
	unsigned long largestSize = 0;

	/* HP-UX Use RLIMIT_AIO_MEM instead of RLIMIT_MEMLOCK */
#ifdef solaris
getrlimit(RLIMIT_VMEM,&rlim);
#else	
getrlimit(RLIMIT_MEMLOCK, &rlim);
#endif
	largestSize = rlim.rlim_max;

	return largestSize;
}
#endif
/*-----------------------------------------------------------------------------------*/
