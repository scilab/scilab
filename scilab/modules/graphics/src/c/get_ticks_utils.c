/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - INRIA - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_ticks_utils.c                                                */
/* desc : a set of functions used to return ticks tlist in scilab         */
/*------------------------------------------------------------------------*/

#include <string.h>
#include "get_ticks_utils.h"
#include "returnPropertyList.h"
/*--------------------------------------------------------------------------------*/
int buildTListForTicks(const double* locations, char * labels[], int nbTics)
{
    char * variable_tlist[] = {"ticks", "locations", "labels"};

    returnedList * tList = createReturnedList(2, variable_tlist);

    if (nbTics == 0)
    {
        // two empty matrices
        addColVectorToReturnedList(tList, NULL, 0);
        addColVectorToReturnedList(tList, NULL, 0);
    }
    else
    {
        addColVectorToReturnedList(tList, locations, nbTics);
        addStringColVectorToReturnedList(tList, labels, nbTics);
    }

    destroyReturnedList(tList);

    return 0;
}
/*--------------------------------------------------------------------------------*/
