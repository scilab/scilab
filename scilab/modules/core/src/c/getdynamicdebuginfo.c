/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
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
#include <string.h>
#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#include <errno.h>
#endif

#ifndef _MSC_VER
#include <fcntl.h>
#include <getopt.h>
#include <ctype.h>
#endif
#include <string.h>

#include <stdlib.h> /* bsearch / strtoul */

#include "sci_malloc.h"
#include "getmemory.h"
#include "getdynamicdebuginfo.h"
#include "api_scilab.h"

// UNAME
#ifdef HAVE_UNAME
#include <sys/utsname.h>
#endif

/**
* Set a debug element
*
* @param msg The debug element
* @param desc The description of the debug element
* @param value The value of the debug element
*/
static void SetDebugMsg(debug_message *msg, char* desc, char* value)
{
    (*msg).value = (char*)MALLOC((strlen(value) + 1) * sizeof(char));
    (*msg).description = (char*)MALLOC((strlen(desc) + 1) * sizeof(char));
    strcpy((*msg).description, desc);
    strcpy((*msg).value, value);
}


#ifndef _MSC_VER
/***********************************************************************/
/*
* Copyright 1999 by Albert Cahalan; all rights reserved.
* This file may be used subject to the terms and conditions of the
* GNU Library General Public License Version 2, or any later version
* at your option, as published by the Free Software Foundation.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Library General Public License for more details.
*/


#define MEMINFO_FILE "/proc/meminfo"
#define S(X) ( ((unsigned long long)(X) << 10) >> shift)

#if defined(k64test) || (defined(_ABIN32) && _MIPS_SIM == _ABIN32)
#define KLONG long long    // not typedef; want "unsigned KLONG" to work
#else
#define KLONG long
#endif


static char buf[1024];
static int meminfo_fd = -1;
/* This macro opens filename only if necessary and seeks to 0 so
* that successive calls to the functions are more efficient.
* It also reads the current contents of the file into the global buf.
*/
#define FILE_TO_BUF(filename, fd) do{				\
	static int local_n;						\
	if (fd == -1 && (fd = open(filename, O_RDONLY)) == -1) {	\
	fputs(BAD_OPEN_MESSAGE, stderr);			\
	fflush(NULL);						\
	_exit(102);						\
	}								\
	lseek(fd, 0L, SEEK_SET);					\
	if ((local_n = read(fd, buf, sizeof buf - 1)) < 0) {	\
	perror(filename);					\
	fflush(NULL);						\
	_exit(103);						\
	}								\
	buf[local_n] = '\0';					\
}while(0)


#define BAD_OPEN_MESSAGE					\
	"Error: /proc must be mounted\n"				\
	"  To mount /proc at boot you need an /etc/fstab line like:\n"	\
	"      /proc   /proc   proc    defaults\n"			\
	"  In the meantime, run \"mount /proc /proc -t proc\"\n"

typedef struct mem_table_struct
{
    const char *name;     /* memory type name */
    unsigned long *slot; /* slot in return struct */
} mem_table_struct;

static int compare_mem_table_structs(const void *a, const void *b)
{
    return strcmp(((const mem_table_struct*)a)->name, ((const mem_table_struct*)b)->name);
}

/* example data, following junk, with comments added:
*
* MemTotal:        61768 kB    old
* MemFree:          1436 kB    old
* MemShared:           0 kB    old (now always zero; not calculated)
* Buffers:          1312 kB    old
* Cached:          20932 kB    old
* Active:          12464 kB    new
* Inact_dirty:      7772 kB    new
* Inact_clean:      2008 kB    new
* Inact_target:        0 kB    new
* Inact_laundry:       0 kB    new, and might be missing too
* HighTotal:           0 kB
* HighFree:            0 kB
* LowTotal:        61768 kB
* LowFree:          1436 kB
* SwapTotal:      122580 kB    old
* SwapFree:        60352 kB    old
* Inactive:        20420 kB    2.5.41+
* Dirty:               0 kB    2.5.41+
* Writeback:           0 kB    2.5.41+
* Mapped:           9792 kB    2.5.41+
* Slab:             4564 kB    2.5.41+
* Committed_AS:     8440 kB    2.5.41+
* PageTables:        304 kB    2.5.41+
* ReverseMaps:      5738       2.5.41+
* SwapCached:          0 kB    2.5.??+
* HugePages_Total:   220       2.5.??+
* HugePages_Free:    138       2.5.??+
* Hugepagesize:     4096 kB    2.5.??+
*/

/* obsolete */
unsigned long kb_main_shared;
/* old but still kicking -- the important stuff */
unsigned long kb_main_buffers;
unsigned long kb_main_cached;
unsigned long kb_main_free;
unsigned long kb_main_total;
unsigned long kb_swap_free;
unsigned long kb_swap_total;
/* recently introduced */
unsigned long kb_high_free;
unsigned long kb_high_total;
unsigned long kb_low_free;
unsigned long kb_low_total;
/* 2.4.xx era */
unsigned long kb_active;
unsigned long kb_inact_laundry;
unsigned long kb_inact_dirty;
unsigned long kb_inact_clean;
unsigned long kb_inact_target;
unsigned long kb_swap_cached;  /* late 2.4 and 2.6+ only */
/* derived values */
unsigned long kb_swap_used;
unsigned long kb_main_used;
/* 2.5.41+ */
unsigned long kb_writeback;
unsigned long kb_slab;
unsigned long nr_reversemaps;
unsigned long kb_committed_as;
unsigned long kb_dirty;
unsigned long kb_inactive;
unsigned long kb_mapped;
unsigned long kb_pagetables;
// seen on a 2.6.x kernel:
static unsigned long kb_vmalloc_chunk;
static unsigned long kb_vmalloc_total;
static unsigned long kb_vmalloc_used;

static void meminfo(void)
{
    char namebuf[16]; /* big enough to hold any row name */
    mem_table_struct findme = { namebuf, NULL};
    mem_table_struct *found;
    char *head;
    char *tail;
    static const mem_table_struct mem_table[] =
    {
        {"Active",       &kb_active},       // important
        {"Buffers",      &kb_main_buffers}, // important
        {"Cached",       &kb_main_cached},  // important
        {"Committed_AS", &kb_committed_as},
        {"Dirty",        &kb_dirty},        // kB version of vmstat nr_dirty
        {"HighFree",     &kb_high_free},
        {"HighTotal",    &kb_high_total},
        {"Inact_clean",  &kb_inact_clean},
        {"Inact_dirty",  &kb_inact_dirty},
        {"Inact_laundry", &kb_inact_laundry},
        {"Inact_target", &kb_inact_target},
        {"Inactive",     &kb_inactive},     // important
        {"LowFree",      &kb_low_free},
        {"LowTotal",     &kb_low_total},
        {"Mapped",       &kb_mapped},       // kB version of vmstat nr_mapped
        {"MemFree",      &kb_main_free},    // important
        {"MemShared",    &kb_main_shared},  // important, but now gone!
        {"MemTotal",     &kb_main_total},   // important
        {"PageTables",   &kb_pagetables},   // kB version of vmstat nr_page_table_pages
        {"ReverseMaps",  &nr_reversemaps},  // same as vmstat nr_page_table_pages
        {"Slab",         &kb_slab},         // kB version of vmstat nr_slab
        {"SwapCached",   &kb_swap_cached},
        {"SwapFree",     &kb_swap_free},    // important
        {"SwapTotal",    &kb_swap_total},   // important
        {"VmallocChunk", &kb_vmalloc_chunk},
        {"VmallocTotal", &kb_vmalloc_total},
        {"VmallocUsed",  &kb_vmalloc_used},
        {"Writeback",    &kb_writeback},    // kB version of vmstat nr_writeback
    };
    const int mem_table_count = sizeof(mem_table) / sizeof(mem_table_struct);

    FILE_TO_BUF(MEMINFO_FILE, meminfo_fd);

    kb_inactive = ~0UL;

    head = buf;
    for (;;)
    {
        tail = strchr(head, ':');
        if (!tail)
        {
            break;
        }
        *tail = '\0';
        if (strlen(head) >= sizeof(namebuf))
        {
            head = tail + 1;
            goto nextline;
        }
        strcpy(namebuf, head);
        found = bsearch(&findme, mem_table, mem_table_count,
                        sizeof(mem_table_struct), compare_mem_table_structs
                       );
        head = tail + 1;
        if (!found)
        {
            goto nextline;
        }
        *(found->slot) = strtoul(head, &tail, 10);
nextline:
        tail = strchr(head, '\n');
        if (!tail)
        {
            break;
        }
        head = tail + 1;
    }
    if (!kb_low_total)  /* low==main except with large-memory support */
    {
        kb_low_total = kb_main_total;
        kb_low_free  = kb_main_free;
    }
    if (kb_inactive == ~0UL)
    {
        kb_inactive = kb_inact_dirty + kb_inact_clean + kb_inact_laundry;
    }
    kb_swap_used = kb_swap_total - kb_swap_free;
    kb_main_used = kb_main_total - kb_main_free;
}
/*****************************************************************/
#endif

char **getDynamicDebugInfo(int *sizeArray, int* pvApiCtx)
{
    int i = 0;
    char *value = NULL;
    char **outputDynamicList = NULL;
    int position = 0;
    int iType = 0;
    static debug_message dynamicDebug[NB_DEBUG_ELEMENT];

#ifndef _MSC_VER
    /* Stuff for the function meminfo() */
    int shift = 10;
    unsigned KLONG buffers_plus_cached = 0;
    SciErr sciErr;
#endif

#ifdef _MSC_VER
    *sizeArray = 0;
    return outputDynamicList;
#else

#ifdef HAVE_UNAME
    /* Host info */
    struct utsname name;
#endif
    value = (char*)MALLOC(255 * sizeof(char));


#ifndef _MSC_VER
    if (meminfo_fd == -1 && (meminfo_fd = open(MEMINFO_FILE, O_RDONLY)) == -1)
    {

        sprintf(value, "%u", getfreememory());
        SetDebugMsg(&dynamicDebug[position], "Total free memory", value);
        position++;

        sprintf(value, "%u", getmemorysize() );
        SetDebugMsg(&dynamicDebug[position], "Total memory", value);
        position++;
    }
    else
    {
        meminfo();

        sprintf(value, "%10Lu", S(kb_main_total));
        SetDebugMsg(&dynamicDebug[position], "Total memory", value);
        position++;


        sprintf(value, "%10Lu", S(kb_main_used));
        SetDebugMsg(&dynamicDebug[position], "Used memory", value);
        position++;

        sprintf(value, "%10Lu", S(kb_main_free));
        SetDebugMsg(&dynamicDebug[position], "Free memory", value);
        position++;

        sprintf(value, "%10Lu", S(kb_main_shared));
        SetDebugMsg(&dynamicDebug[position], "Shared memory", value);
        position++;

        sprintf(value, "%10Lu", S(kb_main_buffers));
        SetDebugMsg(&dynamicDebug[position], "Buffers memory", value);
        position++;

        sprintf(value, "%10Lu", S(kb_main_cached));
        SetDebugMsg(&dynamicDebug[position], "Cached memory", value);
        position++;

        buffers_plus_cached = kb_main_buffers + kb_main_cached;


        sprintf(value, "%10Lu", S(kb_main_used - buffers_plus_cached));
        SetDebugMsg(&dynamicDebug[position], "Used -/+ buffers/cache", value);
        position++;


        sprintf(value, "%10Lu", S(kb_main_free + buffers_plus_cached));
        SetDebugMsg(&dynamicDebug[position], "Free -/+ buffers/cache", value);
        position++;

        sprintf(value, "%10Lu", S(kb_swap_total));
        SetDebugMsg(&dynamicDebug[position], "Total swap", value);
        position++;

        sprintf(value, "%10Lu", S(kb_swap_used));
        SetDebugMsg(&dynamicDebug[position], "Used swap", value);
        position++;

        sprintf(value, "%10Lu", S(kb_swap_free));
        SetDebugMsg(&dynamicDebug[position], "Free swap", value);
        position++;


    }

#endif

#ifdef HAVE_UNAME
    if (uname(&name) < 0)
    {
        sprintf(value, "Unknown OS version (uname failed - %s)", strerror(errno));
    }

    if (strcmp(name.sysname, "AIX") == 0)
    {
        /*
        * Because IBM is doing something different
        */
        sprintf(value, "%s %s.%s", name.sysname, name.version, name.release);
    }
    else
    {
        /*
        * Get the information
        */
        sprintf(value, "%s %s", name.sysname, name.release);
    }
    SetDebugMsg(&dynamicDebug[position], "OS version", value);
    position++;

#endif

    FREE(value);
    sciErr = getNamedVarType(pvApiCtx, "SCI", &iType);
    if ((sciErr.iErr == 0) && (iType == 10))
    {
        char *SCI_value = NULL;
        int SCI_length = 0;
        int m = 0, n = 0;

        sciErr = readNamedMatrixOfString(pvApiCtx, "SCI", &m, &n, &SCI_length, &SCI_value);
        if ( (sciErr.iErr == 0) && ((m == 1) && (n == 1)) )
        {
            SCI_value = (char*)MALLOC(sizeof(char) * (SCI_length + 1));
            if (SCI_value)
            {
                sciErr = readNamedMatrixOfString(pvApiCtx, "SCI", &m, &n, &SCI_length, &SCI_value);
                if (sciErr.iErr == 0)
                {
                    SetDebugMsg(&dynamicDebug[position], "SCI", SCI_value);
                    position++;
                }
                FREE(SCI_value);
                SCI_value = NULL;
            }
        }
    }

    sciErr = getNamedVarType(pvApiCtx, "SCIHOME", &iType);
    if ((sciErr.iErr == 0) && (iType == 10))
    {
        char * SCIHOME_value = NULL;
        int SCIHOME_length = 0;
        int m = 0, n = 0;

        sciErr = readNamedMatrixOfString(pvApiCtx, "SCIHOME", &m, &n, &SCIHOME_length, &SCIHOME_value);
        if ( (sciErr.iErr == 0) && ((m == 1) && (n == 1)) )
        {
            SCIHOME_value = (char*)MALLOC(sizeof(char) * (SCIHOME_length + 1));
            if (SCIHOME_value)
            {
                sciErr = readNamedMatrixOfString(pvApiCtx, "SCIHOME", &m, &n, &SCIHOME_length, &SCIHOME_value);
                if (sciErr.iErr == 0)
                {
                    SetDebugMsg(&dynamicDebug[position], "SCIHOME", SCIHOME_value);
                    position++;
                }
                FREE(SCIHOME_value);
                SCIHOME_value = NULL;
            }
        }
    }

    sciErr = getNamedVarType(pvApiCtx, "TMPDIR", &iType);
    if ((sciErr.iErr == 0) && (iType == 10))
    {
        char * TMPDIR_value = NULL;
        int TMPDIR_length = 0;
        int m = 0, n = 0;

        sciErr = readNamedMatrixOfString(pvApiCtx, "TMPDIR", &m, &n, &TMPDIR_length, &TMPDIR_value);
        if ( (sciErr.iErr == 0) && ((m == 1) && (n == 1)) )
        {
            TMPDIR_value = (char*)MALLOC(sizeof(char) * (TMPDIR_length + 1));
            if (TMPDIR_value)
            {
                sciErr = readNamedMatrixOfString(pvApiCtx, "TMPDIR", &m, &n, &TMPDIR_length, &TMPDIR_value);
                if (sciErr.iErr == 0)
                {
                    SetDebugMsg(&dynamicDebug[position], "TMPDIR", TMPDIR_value);
                    position++;
                }
                FREE(TMPDIR_value);
                TMPDIR_value = NULL;
            }
        }
    }

    outputDynamicList = (char**)MALLOC(sizeof(char*) * (position + 1));

    for (i = 0; i < NB_DEBUG_ELEMENT; i++)
    {
        debug_message msg = dynamicDebug[i];

        if (msg.description == NULL) /* We reach the end of the dynamic list */
        {
            break;
        }

        /* Create the element in the array */

        outputDynamicList[i] = (char*) MALLOC((strlen(msg.description) + strlen(msg.value) + 3) * sizeof(char)); /* 3 for :, space and \0 */
        sprintf(outputDynamicList[i], "%s: %s", msg.description, msg.value);
    }
    *sizeArray = i;
    return outputDynamicList;
#endif
}



