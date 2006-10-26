/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GETMEMORY__
#define __GETMEMORY__

#ifdef WIN32
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
   #if defined(__alpha)	&& !defined(linux)
     #include <sys/sysinfo.h> 
     #include <machine/hal_sysinfo.h>
   #endif
   #include <stdio.h>
   #include <string.h>
#endif /*GETMEMORY*/
/*-----------------------------------------------------------------------------------*/ 
