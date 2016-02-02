/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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

#include "sci_getlanguage.h"
#include "setgetlanguage.h"
#include "gw_localization.h"
#include "tableslanguages.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getlanguagestable)(char *fname, unsigned long fname_len)
{
    static int n1, m1;
    char **Output = NULL;

    int i = 0;

    CheckRhs(0, 0);
    CheckLhs(0, 1);

    Output = (char**)MALLOC(sizeof(char*) * 2 * NumberLanguages);

    for (i = 0; i < NumberLanguages; i++)
    {
        char *alphacode = NULL;
        char *language = NULL;

        alphacode = (char*)MALLOC((strlen(LANGUAGE_COUNTRY_TAB[i].alphacode) + 1) * sizeof(char));
        strcpy(alphacode, LANGUAGE_COUNTRY_TAB[i].alphacode);

        language = (char*)MALLOC((strlen(LANGUAGE_COUNTRY_TAB[i].languagedescription) + 1) * sizeof(char));
        strcpy(language, LANGUAGE_COUNTRY_TAB[i].languagedescription);

        Output[i] = alphacode;
        Output[NumberLanguages + i] = language;

    }

    m1 = NumberLanguages;
    n1 = 2;
    CreateVarFromPtr( Rhs + 1, MATRIX_OF_STRING_DATATYPE, &m1, &n1, Output);
    LhsVar(1) = Rhs + 1;
    PutLhsVar();;

    if (Output)
    {
        for (i = 0; i < NumberLanguages; i++)
        {
            FREE(Output[i]);
            Output[i] = NULL;
            FREE(Output[NumberLanguages + i]);
            Output[NumberLanguages + i] = NULL;
        }
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
