/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - INRIA - Pierre Lando
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

/*------------------------------------------------------------------------*/
/* file: get_ticks_utils.c                                                */
/* desc : a set of functions used to return ticks tlist in scilab         */
/*------------------------------------------------------------------------*/

#include <string.h>
#include "get_ticks_utils.h"
#include "returnPropertyList.h"
/*--------------------------------------------------------------------------------*/
void* buildTListForTicks(const double* locations, char * labels[], int nbTics)
{
    char * variable_tlist[] = {"ticks", "locations", "labels"};

    void* tList = createReturnedList(2, variable_tlist);

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

    return tList;
}
/*--------------------------------------------------------------------------------*/
