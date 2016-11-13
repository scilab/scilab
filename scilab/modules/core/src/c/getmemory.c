/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * Copyright (C) 2009 - Dan McMahill (Fix of bug 4299 and 4312)
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/* References
@sa http://nixdoc.net/man-pages/Tru64/man2/getsysinfo.2.html
@sa http://www.opensource.apple.com/darwinsource/projects/other/gccfast-1621.1/libiberty/physmem.c
@sa http://lists.gnu.org/archive/html/bug-gnulib/2003-08/msg00102.html
@sa http://netbsd.gw.com/cgi-bin/man-cgi?sysctl+3+NetBSD-4.0
@sa http://cvsweb.netbsd.org/bsdweb.cgi/pkgsrc/math/scilab/patches/patch-aj?annotate=1.9
*/

#if defined(__linux__)
#define _GNU_SOURCE /* Bug 5673 fix: avoid dependency on GLIBC_2.7 */
#endif

#include "getmemory.h"

#ifdef _MSC_VER
#include <windows.h>
#endif

#if defined(__NetBSD__) || defined(__DragonFly__)
#include <sys/param.h>
#include <sys/sysctl.h>
#define ARRAY_SIZE(a) (sizeof (a) / sizeof ((a)[0]))
#define PAGESHIFT_UNDEF -100
#elif defined(__FreeBSD__)
#include <sys/param.h>
#include <sys/sysctl.h>
#endif


#include <string.h>
#include <stdio.h>
#define kooctet 1024
int getfreememory(void)
{

#if defined(_MSC_VER)
    {
        MEMORYSTATUS stat;
        GlobalMemoryStatus (&stat);
        return (int)(stat.dwAvailPhys / kooctet);
    }
#elif defined(hpux)
    {
        struct pst_static pst;
        /*        pstat_getstatic(&pst, sizeof(pst), (size_t) 1, 0);
              memorysizeKO=(pst.psd_free)/kooctet;*/
        return 0;
    }
#elif defined(__APPLE__)
    {
        vm_statistics_data_t page_info;
        vm_size_t pagesize;
        mach_msg_type_number_t count;
        kern_return_t kret;

        pagesize = 0;
        kret = host_page_size (mach_host_self(), &pagesize);
        count = HOST_VM_INFO_COUNT;

        kret = host_statistics (mach_host_self(), HOST_VM_INFO, (host_info_t)&page_info, &count);
        return page_info.free_count * pagesize / 1024;
    }
#elif HAVE_TABLE && defined TBL_VMSTATS
    {
        /* This works on Tru64 UNIX V4/5.  */
        struct tbl_vmstats vmstats;

        if (table (TBL_VMSTATS, 0, &vmstats, 1, sizeof (vmstats)) == 1)
        {
            double pages = vmstats.free_count;
            double pagesize = vmstats.pagesize;

            if (0 <= pages && 0 <= pagesize)
            {
                return pages * pagesize;
            }
            else
            {
                return 0;
            }
        }
    }
#elif defined(__linux__)
    {
        char field[9]  = {0};
        long long data =  0;
        char unit[4]   = {0};

        long long free    = -1,
                  buffers = -1,
                  cached  = -1;

        FILE *fp = fopen("/proc/meminfo", "r");
        if (fp != NULL)
        {
            /* Read Cached, Buffers and MemFree from /proc/meminfo */
            while (fscanf(fp, "%8s %lld %3s\n", field, &data, unit) != EOF)
            {
                if (!strncmp("MemFree:", field, 8))
                {
                    free = data;
                }
                else if (!strncmp("Buffers:", field, 8))
                {
                    buffers = data;
                }
                else if (!strncmp("Cached:", field, 8))
                {
                    cached = data;
                }
            }
            fclose(fp);

            /* Read successful, convert unit and return the result */
            if (buffers >= 0 && cached >= 0 && free >= 0)
            {
                free += cached + buffers;
                switch (unit[0])
                {
                    case 'g':
                    case 'G':
                        free *= kooctet*kooctet;
                        break;
                    case 'm':
                    case 'M':
                        free *= kooctet;
                        break;
                    case 'o':
                    case 'O':
                        free /= kooctet;
                        break;
                }
                return (int)free;
            }
        }

        /* Strange, /proc not mounted ? new and unknown format ?
           fall back to inaccurate sysconf() */
        return (sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGESIZE)) / kooctet;
    }
#elif defined(__NetBSD__) || defined(__DragonFly__)
    {
        /* This works on *bsd.  */
        int mib[2];
        struct uvmexp_sysctl uvmexp;
        long freemem;
        size_t lenu = sizeof uvmexp;
        unsigned int pagesize;
        static int pageshift = PAGESHIFT_UNDEF;
        size_t lenp = sizeof pagesize;

        mib[0] = CTL_HW;
        if (pageshift == PAGESHIFT_UNDEF)
        {
            /* Figure out the page size */
            mib[1] = HW_PAGESIZE;
            if (sysctl (mib, ARRAY_SIZE (mib), &pagesize, &lenp, NULL, 0) != 0 || lenp != sizeof (pagesize) )
            {
                /* sysctl failed -- what to do?? */
                return 0;
            }
            pageshift = 0;
            while (pagesize > 1)
            {
                pageshift++;
                pagesize >>= 1;
            }

            /* convert to kB */
            pageshift -= 10;
        }

        mib[0] = CTL_VM;
        mib[1] = VM_UVMEXP2;
        if ( (sysctl (mib, ARRAY_SIZE (mib), &uvmexp, &lenu, NULL, 0) == 0) && (lenu == sizeof (uvmexp)) )
        {
            return  (uvmexp.free << pageshift);
        }

        return 0;
    }
#elif defined(__FreeBSD__)
    int avphys_pages;
    size_t oldlenp = sizeof (avphys_pages);
    sysctlbyname("vm.stats.vm.v_free_count", &avphys_pages, &oldlenp, NULL, NULL);
    return (avphys_pages / kooctet) * sysconf(_SC_PAGESIZE);

#else
    /* Solaris and others assumed*/
    return (sysconf(_SC_AVPHYS_PAGES) / kooctet) * sysconf(_SC_PAGESIZE);
#endif
}
/*--------------------------------------------------------------------------*/
int getmemorysize()
{
#if defined(_MSC_VER)
    {
        MEMORYSTATUS stat;
        GlobalMemoryStatus (&stat);
        return (int)(stat.dwTotalPhys / kooctet);
    }
#elif defined(hpux)
    {
        struct pst_static pst;
        pstat_getstatic(&pst, sizeof(pst), (size_t) 1, 0);
        return (int)((pst.physical_memory) / kooctet);
    }
#elif defined(__APPLE__) || defined(__NetBSD__) || defined(__DragonFly__)
    {
        /* this works on *bsd and darwin */
        size_t len;
        int total;
        int mib[2];

        mib[0] = CTL_HW;
        mib[1] = HW_PHYSMEM;
        len = sizeof (total);

        sysctl(mib, 2, &total, &len, NULL, 0);
        return  total / 1024;
    }
#elif HAVE_GETSYSINFO && defined GSI_PHYSMEM
    {
        /* This works on Tru64 UNIX V4/5.  */
        int physmem;

        if (getsysinfo (GSI_PHYSMEM, (caddr_t) &physmem, sizeof (physmem),
        NULL, NULL, NULL) == 1)
        {
            double kbytes = physmem;

            if (0 <= kbytes)
            {
                return kbytes * 1024.0;
            }
            else
            {
                return 0;
            }
        }
    }
#else
    /* Linux ,Solaris and others */
    return (sysconf(_SC_PHYS_PAGES) / kooctet) * sysconf(_SC_PAGESIZE);
#endif
}
/*--------------------------------------------------------------------------*/
