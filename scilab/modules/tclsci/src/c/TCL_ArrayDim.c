/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * Copyright (C) 2008-2008 - INRIA - Allan CORNET
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

#if defined(__linux__)
#define _GNU_SOURCE /* Bug 5673 fix: avoid dependency on GLIBC_2.7 */
#endif

#include <string.h>
#include "sci_malloc.h"
#include "TCL_ArrayDim.h"
#include "Scierror.h"
#include "localization.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
#define TCL_ALL_INDEXES "TclScilabTmpVar1"
#define TCL_NUMERICAL_INDEXES "TclScilabTmpVar2"
#define CHAR_BLANK ' '
#define CHAR_PERIOD ','
/*--------------------------------------------------------------------------*/
#define MAX(a,b)	a >= b ? a : b
/*--------------------------------------------------------------------------*/

/*
** TCL arrays are in fact Hastables. But we want to be able to store
** and get arrays as Scilab Matrixes. Then we apply this alogorithm :
** - Get all indexes
** - Get all numerical indexes like ^[1-9][0-9]*,[1-9][0-9]*$
** - IF they have the same length (means all indexes are numerical)
**	-> Take all indexes and complete list ex : ["1,1", "2,2"] -> ["1,1" "1,2" "2,1", "2,2"]
**	-> Return completed index list _AND_ modify value pointed by nb_lines, nb_columns
** - ELSE (means we are accessing a hastable)
**	-> Return all indexes as Vector : nb_columns = 1.
*/

char **TCL_ArrayDim(Tcl_Interp *TCLinterpreter, char *VarName, int *nb_lines, int *nb_columns)
{
    char **index_list = NULL;


    if (strcmp(VarName, TCL_ALL_INDEXES) == 0 || strcmp(VarName, TCL_NUMERICAL_INDEXES) == 0)
    {
        char MyTclCommand[2048];
        char *StrArrayIndexes = NULL;
        char *NumArrayIndexes = NULL;

        /*
        ** TCL Array are Hashtable
        ** Get all keys into TCL_ALL_INDEXES
        */
        sprintf(MyTclCommand, "set %s [lsort -dictionary [array names %s *]];", TCL_ALL_INDEXES, VarName);

        if ( Tcl_Eval(TCLinterpreter, MyTclCommand) == TCL_ERROR  )
        {
            Scierror(999, _("Tcl Error : %s\n"), Tcl_GetStringResult(TCLinterpreter));
            return 0;
        }
        /*
        ** Look if keys are like [1-9][0-9]*,[1-9][0-9]*
        ** And get it into TCL_NUMERICAL_INDEXES
        */
        sprintf(MyTclCommand, "set %s [lsort -dictionary [array names %s -regexp {^[1-9][0-9]*,[1-9][0-9]*$}]];", TCL_NUMERICAL_INDEXES, VarName);

        if ( Tcl_Eval(TCLinterpreter, MyTclCommand) == TCL_ERROR  )
        {
            Scierror(999, _("Tcl Error : %s\n"), Tcl_GetStringResult(TCLinterpreter));
            return 0;
        }


        StrArrayIndexes = (char *) Tcl_GetVar(TCLinterpreter, TCL_ALL_INDEXES, TCL_GLOBAL_ONLY);
        NumArrayIndexes = (char *) Tcl_GetVar(TCLinterpreter, TCL_NUMERICAL_INDEXES, TCL_GLOBAL_ONLY);

        if (!StrArrayIndexes)
        {
            return NULL;
        }

        /*
        ** Look if all indexes are numerical or not
        */
        if (strlen(StrArrayIndexes) == strlen(NumArrayIndexes))
        {
            char *current;
            char **tmp_list = NULL;
            int j , i = 0;
            int current_line = 0;
            int current_column = 0;

            current = strtok(StrArrayIndexes, " ");

            /*
            ** Get all known indexes
            */
            while (current != NULL)
            {
                tmp_list = REALLOC(tmp_list, (i + 1) * sizeof(char *));
                tmp_list[i] = os_strdup(current);
                sscanf(current, "%d,%d", &current_line, &current_column);
                *nb_lines = MAX(*nb_lines, current_line);
                *nb_columns = MAX(*nb_columns, current_column);
                current = strtok (NULL, " ");
                ++i;
            }

            /*
            ** Create a complete list of NULL and insert only index we found at the right place
            */
            index_list = MALLOC(*nb_lines * *nb_columns * sizeof(char *));
            for (j = 0; j < *nb_lines * *nb_columns ; ++j)
            {
                index_list[j] = NULL;
            }
            for (j = 0; j < i ; ++j)
            {
                sscanf(tmp_list[j], "%d,%d", &current_line, &current_column);
                index_list[(current_column - 1) * *nb_lines + current_line - 1] = tmp_list[j];
            }
            FREE(tmp_list);
        }
        else
        {
            /*
            ** We found an Hastable...
            ** Then return a Vector of index.
            */
            char *current;
            *nb_lines = 0;
            *nb_columns = 1;

            current = strtok(StrArrayIndexes, " ");

            while (current != NULL)
            {
                index_list = REALLOC(index_list, (*nb_lines + 1) * sizeof(char *));
                index_list[*nb_lines] = os_strdup(current);
                current = strtok (NULL, " ");
                ++(*nb_lines);
            }
        }

        /*
        ** Unset temporary TCL Variables
        */
        Tcl_UnsetVar(TCLinterpreter, TCL_ALL_INDEXES, TCL_GLOBAL_ONLY);
        Tcl_UnsetVar(TCLinterpreter, TCL_NUMERICAL_INDEXES, TCL_GLOBAL_ONLY);
    }
    return index_list;
}
/*--------------------------------------------------------------------------*/
