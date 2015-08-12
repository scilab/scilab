/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string.h>

#include "matfile_manager.h"

#include "sciprint.h"
#include "MALLOC.h"

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
