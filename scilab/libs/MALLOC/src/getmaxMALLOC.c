/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
	#include <Windows.h>
#else
	#include <sys/resource.h>
	#include "machine.h"
	#include "getmemory.h"
	
	#ifdef HAVE_LIMITS_H
	#include <limits.h>
	#elif !defined(LONG_MAX)
	#define LONG_MAX 2147483647L 
	#endif
#endif
#include "../includes/getmaxMALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
IMPORT_EXPORT_MALLOC_DLL unsigned long GetLargestFreeMemoryRegion(void)
{
#define SECURITY_FREE_MEMORY 240000

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
	/* We remove a security size to be sure that MALLOC doesn't fails */
	if (largestSize > SECURITY_FREE_MEMORY) largestSize = largestSize - SECURITY_FREE_MEMORY;

	return largestSize;
#endif
}
/*-----------------------------------------------------------------------------------*/
#else
IMPORT_EXPORT_MALLOC_DLL unsigned long GetLargestFreeMemoryRegion(void)
{
	struct rlimit rlim;
	unsigned long largestSize, freeMem;

	/* HP-UX Use RLIMIT_AIO_MEM instead of RLIMIT_MEMLOCK */
#ifdef solaris
getrlimit(RLIMIT_VMEM,&rlim);
#else	
getrlimit(RLIMIT_AS, &rlim);
#endif
	if(rlim.rlim_max == RLIM_INFINITY)
	{
		largestSize = LONG_MAX;
	}
	else
	{
		largestSize = rlim.rlim_max;
	}
	
	freeMem = getfreememory()*1024;
	if(freeMem < largestSize)
	{
		return freeMem;
	}
	else
	{
		return largestSize;
	}
	
	return largestSize;
}
#endif
/*-----------------------------------------------------------------------------------*/
