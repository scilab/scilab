/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GETMEMORY__
#define __GETMEMORY__

#ifdef _MSC_VER
   #include <windows.h>
#else
   #if defined(hpux)
      #include <sys/param.h>
      #include <sys/pstat.h>
   #else
      #if defined(__APPLE__) 
        #import <mach/host_info.h>
        #import <mach/mach_host.h>
        #import <sys/sysctl.h>
      #else
        #include <unistd.h>
        #endif
      #endif
   #endif
   #if defined(__alpha)	
     #include <sys/sysinfo.h> 
     #include <machine/hal_sysinfo.h>
   #endif
   #include <stdio.h>
   #include <string.h>
#endif /*GETMEMORY*/

/**
 * Return the size of free memory in megabytes
 * @return the size of free memory 
 */
int getfreememory(void);


/**
 * Return the size of memory in megabytes
 * @return the size of memory 
 */
int getmemorysize(void);


/*--------------------------------------------------------------------------*/ 
