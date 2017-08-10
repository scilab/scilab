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
    int nbRow = 0, nbCol = 0;
    char **charData = NULL;
    int K = 0, L = 0;
    SciErr sciErr;

    if (matVariable->rank == 2) /* 2-D array */
    {
        nbRow = (int)matVariable->dims[0];
        nbCol = nbRow == 0 ? 0 : 1; /* In Scilab empty string has size 0x0 */

        if (nbRow != 0)
        {
            charData =  (char**) MALLOC(sizeof(char*) * nbRow);
            if (charData == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "CreateCharVariable");
                return FALSE;
            }
        }

        for (K = 0; K < nbRow; K++)
        {
            charData[K] =  (char*) MALLOC(sizeof(char) * (matVariable->dims[1] + 1));
            if (charData[K] == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "CreateCharVariable");
                freeArrayOfString(charData, K);
                return FALSE;
            }
        }

        /* Fill items: data in Matlab file is stored columnwise */
        for (K = 0; K < (int)matVariable->dims[0]; K++) /* Loop over items */
        {
            for (L = 0; L < (int)matVariable->dims[1]; L++) /* Loop over chars */
            {
                charData[K][L] = ((char *)matVariable->data)[L * matVariable->dims[0] + K];
            }
            charData[K][L] = '\0';
        }

        if (nbRow * nbCol != 0)
        {
            if (parent == NULL)
            {
                sciErr = createMatrixOfString(pvApiCtx, iVar, nbRow, nbCol, charData);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    freeArrayOfString(charData, nbRow);
                    return 0;
                }
            }
            else
            {
                sciErr = createMatrixOfStringInList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, charData);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    freeArrayOfString(charData, nbRow);
                    return 0;
                }
            }
        }
        else /* Empty character string */
        {
            if (parent == NULL)
            {
                createSingleString(pvApiCtx, iVar, "\0");
            }
            else
            {
                char ** tmp_char = (char **)MALLOC(sizeof(char *));
                tmp_char[0] = os_strdup("\0");
                sciErr = createMatrixOfStringInList(pvApiCtx, iVar, parent, item_position, 1, 1, tmp_char);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    freeArrayOfString(tmp_char, 1);
                    return 0;
                }
                freeArrayOfString(tmp_char, 1);
            }
        }

        freeArrayOfString(charData, nbRow * nbCol);
    }
    else /* Multi-dimension array -> Scilab HyperMatrix */
    {
        Scierror(999, _("%s: N-D arrays of chars not implemented.\n"), "CreateCharVariable");
        return FALSE;
    }

    return TRUE;
}
