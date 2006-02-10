
/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/* References
http://nixdoc.net/man-pages/Tru64/man2/getsysinfo.2.html
http://www.opensource.apple.com/darwinsource/projects/other/gccfast-1621.1/libiberty/physmem.c
http://lists.gnu.org/archive/html/bug-gnulib/2003-08/msg00102.html
*/
#include "getmemory.h"

int getfreememory()
{
#define kooctet 1024
#undef MEMOK

#if defined(WIN32)
  {
    #define MEMOK 1
    MEMORYSTATUS stat;
    GlobalMemoryStatus (&stat);
    return stat.dwAvailPhys/kooctet;
  }
#endif

#if defined(hpux)
#define MEMOK 1

 {
   struct pst_static pst;
  /*	pstat_getstatic(&pst, sizeof(pst), (size_t) 1, 0);
	memorysizeKO=(pst.psd_free)/kooctet;*/
  return 0;
 }

#endif
#if defined(__APPLE__) 
#define MEMOK 1
      {
	vm_statistics_data_t page_info;
	vm_size_t pagesize;
	mach_msg_type_number_t count;
	kern_return_t kret;
    
	pagesize = 0;
	kret = host_page_size (mach_host_self(), &pagesize);
	count = HOST_VM_INFO_COUNT;
    
	kret = host_statistics (mach_host_self(), HOST_VM_INFO,(host_info_t)&page_info, &count);
	return page_info.free_count*pagesize / 1024;
      }
#endif

#if HAVE_TABLE && defined TBL_VMSTATS
#define MEMOK 1
   { /* This works on Tru64 UNIX V4/5.  */
     struct tbl_vmstats vmstats;

     if (table (TBL_VMSTATS, 0, &vmstats, 1, sizeof (vmstats)) == 1)
       {
         double pages = vmstats.free_count;
         double pagesize = vmstats.pagesize;

         if (0 <= pages && 0 <= pagesize)
           return pages * pagesize;
	 else
	   return 0;
       }
   }
#endif

#ifndef  MEMOK
/* Linux ,Solaris and others assumed*/
      return (sysconf(_SC_AVPHYS_PAGES)*sysconf(_SC_PAGESIZE))/kooctet;
#endif

}
/*-----------------------------------------------------------------------------------*/ 
int getmemorysize()
{
#define kooctet 1024
#undef MEMOK


#if defined(WIN32)
#define MEMOK 1
  {
    MEMORYSTATUS stat;
    GlobalMemoryStatus (&stat);
    return stat.dwTotalPhys/kooctet;
  }
#endif

#if defined(hpux)
#define MEMOK 1
  {
    struct pst_static pst;
    pstat_getstatic(&pst, sizeof(pst), (size_t) 1, 0);
    return (pst.physical_memory)/kooctet;
  }
#endif

#if defined(__APPLE__) 
#define MEMOK 1
  {
    size_t len;
    int total;
    int mib[2];

    mib[0] = CTL_HW;
    mib[1] = HW_PHYSMEM;
    len = sizeof (total);
 
    sysctl(mib, 2, &total, &len, NULL, 0);
    return  total/1024;
  }
#endif

#if HAVE_GETSYSINFO && defined GSI_PHYSMEM
#define MEMOK 1
   { /* This works on Tru64 UNIX V4/5.  */
     int physmem;

     if (getsysinfo (GSI_PHYSMEM, (caddr_t) &physmem, sizeof (physmem),
                    NULL, NULL, NULL) == 1)
       {
        double kbytes = physmem;

        if (0 <= kbytes)
          return kbytes * 1024.0;
	else
	  return 0;
       }
   }
#endif

#ifndef  MEMOK
  /* Linux ,Solaris and others */
  return (sysconf(_SC_PHYS_PAGES)*sysconf(_SC_PAGESIZE))/kooctet;
#endif
}
/*-----------------------------------------------------------------------------------*/
