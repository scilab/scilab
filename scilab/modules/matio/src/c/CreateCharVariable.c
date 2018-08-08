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

#include "api_scilab.h"
#include "CreateMatlabVariable.h"
#include "freeArrayOfString.h"
#include "os_string.h"
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"

int CreateCharVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position)
{
    SciErr sciErr;
    if (matVariable->rank == 2) /* 2-D array */
    {
        int nbRow  = (int)matVariable->dims[0];
        int strLen = (int)matVariable->dims[1];
        int i     = 0;
        int j     = 0;
        char** ppcData = NULL;

        // empty matrix string
        nbRow = nbRow == 0 ? 1 : nbRow;
        ppcData = (char**) MALLOC(sizeof(char*) * (nbRow));
        /* Fill items: data in Matlab file is stored columnwise */
        for (i = 0; i < nbRow; i++) /* Loop over items */
        {
            ppcData[i] = (char*) MALLOC(sizeof(char) * (strLen + 1));
            for (j = 0; j < strLen; j++)
            {
                ppcData[i][j] = ((char *)matVariable->data)[j * nbRow + i];
            }
            ppcData[i][strLen] = '\0';
        }

        if (parent == NULL)
        {
            sciErr = createMatrixOfString(pvApiCtx, iVar, nbRow, 1, ppcData);
        }
        else
        {
            sciErr = createMatrixOfStringInList(pvApiCtx, iVar, parent, item_position, nbRow, 1, ppcData);
        }

        for (i = 0; i < nbRow; i++)
        {
            FREE(ppcData[i]);
        }

        FREE(ppcData);

        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

    }
    else /* Multi-dimension array -> Scilab HyperMatrix */
    {
        Scierror(999, _("%s: N-D arrays of chars not implemented.\n"), "CreateCharVariable");
        return FALSE;
    }

    return TRUE;
}
