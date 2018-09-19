/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
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

#include "matfile_manager.h"

#include "sciprint.h"
#include "sci_malloc.h"

static mat_t **openedMatfiles = NULL;
static int numberOfMatfiles = 0;

/*******************************************************************************
   This function manages the set of files:
     - opened using matfile_open
     - closed using matfile_closed
   All pointers to opened files are stored in a static variable
*******************************************************************************/
void matfile_manager(int action, int *fileIndex, mat_t **matfile)
{
    int K = 0; /* Loop index */

    if (action == MATFILEMANAGER_GETFILE) /* Get the mat_t structure corresponding to the fileIndex (fileIndex is an input) */
    {
        if ((*fileIndex >= numberOfMatfiles) || (*fileIndex < 0)) /* FileIndex is too big or invalid */
        {
            *matfile = NULL; /* Return NULL */
        }
        else /* Return corresponding matfile */
        {
            *matfile = openedMatfiles[*fileIndex];
        }
    }
    else if (action == MATFILEMANAGER_ADDFILE) /* Add matfile to the list of opened matfiles (fileIndex is an output) */
    {
        if (numberOfMatfiles == 0) /* No file opened */
        {
            numberOfMatfiles++;
            if (openedMatfiles)
            {
                openedMatfiles = (mat_t**)REALLOC(openedMatfiles, numberOfMatfiles * sizeof(mat_t*));
            }
            else
            {
                openedMatfiles = (mat_t**)MALLOC(numberOfMatfiles * sizeof(mat_t*));
            }
            *fileIndex = numberOfMatfiles - 1;
            openedMatfiles[*fileIndex] = *matfile;
        }
        else
        {
            /* Search for an empty space in openedMatfiles */
            for (K = 0; K < numberOfMatfiles; K++)
            {
                if (openedMatfiles[K] == NULL)
                {
                    openedMatfiles[K] = *matfile;
                    *fileIndex = K;
                    return;
                }
            }
            numberOfMatfiles++;
            openedMatfiles = (mat_t**)REALLOC(openedMatfiles, numberOfMatfiles * sizeof(mat_t*));
            *fileIndex = numberOfMatfiles - 1;
            openedMatfiles[*fileIndex] = *matfile;
        }
    }
    else if (action == MATFILEMANAGER_DELFILE) /* Close matfile (fileIndex is an input) */
    {
        if ((*fileIndex >= numberOfMatfiles) || (*fileIndex < 0)) /* FileIndex is too big or invalid */
        {
            matfile = NULL; /* Return NULL */
        }
        else /* Return corresponding matfile */
        {
            *matfile = openedMatfiles[*fileIndex];
            //sciprint("Have to Close %s\n", (*matfile)->filename);
            openedMatfiles[*fileIndex] = NULL;
        }
    }
    else if (action == MATFILEMANAGER_VIEWFILE) /* Display names of opened matfiles */
    {
        for (K = 0; K < numberOfMatfiles; K++)
        {
            if (openedMatfiles[K] != NULL)
            {
                sciprint("%d -> %s\n", K, Mat_GetFilename(openedMatfiles[K]));
            }
            else
            {
                sciprint("%d -> Empty\n", K);
            }
        }
    }
    else /* Unknown action */
    {
        /* Return NULL */
    }
}

static void clearOpenedMatfiles()
{
    if(openedMatfiles)
    {
        int i = 0;
        for(i = 0; i < numberOfMatfiles; ++i)
        {
            if(openedMatfiles[i])
            {
                Mat_Close(openedMatfiles[i]);
            }
        }
        FREE(openedMatfiles);
        numberOfMatfiles = 0;
    }
}

/*** free memory at library unload ***/
#ifdef _MSC_VER
#include <windows.h>
/*--------------------------------------------------------------------------*/
//for Visual Leak Detector in debug compilation mode
//#define DEBUG_VLD
#if defined(DEBUG_VLD) && defined(_DEBUG)
#include <vld.h>
#endif
/*--------------------------------------------------------------------------*/
#pragma comment(lib,"../../bin/libintl.lib")
#pragma comment(lib,"../../bin/libmatio.lib")
/*--------------------------------------------------------------------------*/
int WINAPI DllMain(HINSTANCE hinstDLL, DWORD flag, LPVOID reserved)
{
    switch (flag)
    {
        case DLL_PROCESS_ATTACH :
            break;
        case DLL_PROCESS_DETACH :
            clearOpenedMatfiles();
            break;
        case DLL_THREAD_ATTACH :
            break;
        case DLL_THREAD_DETACH :
            break;
        default :
            return 0;
    }

    return 1;
}
#else
void __attribute__ ((constructor)) loadmatio(void);
void __attribute__ ((destructor)) unloadmatio(void);

void loadmatio(void)
{

}

void unloadmatio(void)
{
   clearOpenedMatfiles();
}
#endif