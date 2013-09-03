/*
 * ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "localization.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "stackinfo.h"
#include "inffic.h"
#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif
#include "errorTable.h"
#include "lasterror.h"
#include "Scierror.h"
#include "strsubst.h"
/*--------------------------------------------------------------------------*/
typedef enum
{
    CVNAME_READING_TYPE_1 = 0,
    CVNAME_READING_TYPE_2 = 1,
    CVNAME_READING_TYPE_3 = 2,
    CVNAME_READING_TYPE_4 = 3,
    CVNAME_READING_TYPE_5 = 4,
    CVNAME_READING_TYPE_6 = 5,
} CVNAME_TYPE;
/*--------------------------------------------------------------------------*/
extern int C2F(errmgr)(); /* fortran */
/*--------------------------------------------------------------------------*/
static void strip_blank(char *source);
static char *getConvertedNameFromStack(int cvnametype);
static char *defaultStringError(void);
/*--------------------------------------------------------------------------*/
int errorTable(int iErr)
{
    int errtyp = 0; /* by default error can be caught */
    int iValueReturned = 0;

    /* clean last error */
    clearInternalLastError();

    switch (iErr)
    {
        case 1:
        {
            iValueReturned = Scierror(iErr, _("Incorrect assignment.\n"));
        }
        break;
        case 2:
        {
            iValueReturned = Scierror(iErr, _("Invalid factor.\n"));
            errtyp = 1;
            C2F(errmgr)(&iErr, &errtyp);
        }
        break;
        case 3:
        {
            iValueReturned = Scierror(iErr, _("Waiting for right parenthesis.\n"));
            errtyp = 1;
            C2F(errmgr)(&iErr, &errtyp);
        }
        break;
        case 4:
        {
            /* bug 6435 */
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_6);
            if (NameVarOnStack)
            {
                if (NameVarOnStack[0] != '0')
                {
                    iValueReturned = Scierror(iErr, _("Undefined variable: %s\n"), NameVarOnStack);
                    FREE(NameVarOnStack);
                    NameVarOnStack = NULL;
                    break;
                }

                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
            iValueReturned = Scierror(iErr, _("Undefined variable.\n"));
        }
        break;
        case 5:
        {
            iValueReturned = Scierror(iErr, _("Inconsistent column/row dimensions.\n"));
        }
        break;
        case 6:
        {
            iValueReturned = Scierror(iErr, _("Inconsistent row/column dimensions.\n"));
        }
        break;
        case 7:
        {
            iValueReturned = Scierror(iErr, _("Dot cannot be used as modifier for this operator.\n"));
        }
        break;
        case 8:
        {
            iValueReturned = Scierror(iErr, _("Inconsistent addition.\n"));
        }
        break;
        case 9:
        {
            iValueReturned = Scierror(iErr, _("Inconsistent subtraction.\n"));
        }
        break;
        case 10:
        {
            iValueReturned = Scierror(iErr, _("Inconsistent multiplication.\n"));
        }
        break;
        case 11:
        {
            iValueReturned = Scierror(iErr, _("Inconsistent right division.\n"));
        }
        break;
        case 12:
        {
            iValueReturned = Scierror(iErr, _("Inconsistent left division.\n"));
        }
        break;
        case 13:
        {
            iValueReturned = Scierror(iErr, _("Redefining permanent variable.\n"));
        }
        break;
        case 14:
        {
            iValueReturned = Scierror(iErr, _("Eye variable undefined in this context.\n"));
        }
        break;
        case 15:
        {
            iValueReturned = Scierror(iErr, _("Submatrix incorrectly defined.\n"));
        }
        break;
        case 16:
        {
            iValueReturned = Scierror(iErr, _("Incorrect command!\n"));
            errtyp = 1; /* error not recoverable */
            C2F(errmgr)(&iErr, &errtyp);
        }
        break;
        case 17:
        {
            int Memory_used_for_variables = 0;
            int Intermediate_Memory = 0;
            int Total_Memory_available = 0;
            char msgErr[bsiz];
            char msgTmp[bsiz];

            Intermediate_Memory = getIntermediateMemoryNeeded();
            C2F(getstackinfo)(&Total_Memory_available, &Memory_used_for_variables);

            strcpy(msgErr, _("stack size exceeded!\n"));
            strcat(msgErr, _("Use stacksize function to increase it.\n"));

            sprintf(msgTmp, _("Memory used for variables: %d\n"), Memory_used_for_variables);
            strcat(msgErr, msgTmp);

            sprintf(msgTmp, _("Intermediate memory needed: %d\n"), Intermediate_Memory);
            strcat(msgErr, msgTmp);

            sprintf(msgTmp, _("Total memory available: %d\n"), Total_Memory_available);
            strcat(msgErr, msgTmp);

            iValueReturned = Scierror(iErr, msgErr);
        }
        break;
        case 18:
        {
            iValueReturned = Scierror(iErr, _("Too many variables!\n"));
        }
        break;
        case 19:
        {
            iValueReturned = Scierror(iErr, _("Problem is singular.\n"));
        }
        break;
        case 20:
        {
            if (Err == 1)
            {
                iValueReturned = Scierror(iErr, _("Wrong type for first argument: Square matrix expected.\n"));
            }
            else
            {
                iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: Square matrix expected.\n"), Err);
            }
        }
        break;
        case 21:
        {
            iValueReturned = Scierror(iErr, _("Invalid index.\n"));
        }
        break;
        case 22:
        {
            iValueReturned = Scierror(iErr, _("Recursion problems. Sorry...\n"));
        }
        break;
        case 23:
        {
            iValueReturned = Scierror(iErr, _("Matrix norms available are 1, 2, inf, and fro.\n"));
        }
        break;
        case 24:
        {
            iValueReturned = Scierror(iErr, _("Convergence problem...\n"));
        }
        break;
        case 25:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_2);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("Bad call to primitive: %s\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 26:
        {
            iValueReturned = Scierror(iErr, _("Too complex recursion! (recursion tables are full)\n"));
            /* break recursion */
            C2F(recu).pt = Min(C2F(recu).pt, psiz);
            errtyp = 1; /* error not recoverable */
            C2F(errmgr)(&iErr, &errtyp);
        }
        break;
        case 27:
        {
            iValueReturned = Scierror(iErr, _("Division by zero...\n"));
        }
        break;
        case 28:
        {
            iValueReturned = Scierror(iErr, _("Empty function...\n"));
        }
        break;
        case 29:
        {
            iValueReturned = Scierror(iErr, _("Matrix is not positive definite.\n"));
        }
        break;
        case 30:
        {
            iValueReturned = Scierror(iErr, _("Invalid exponent.\n"));
        }
        break;
        case 31:
        {
            iValueReturned = Scierror(iErr, _( "Incorrect string.\n"));
            errtyp = 1; /* error not recoverable */
            C2F(errmgr)(&iErr, &errtyp);
        }
        break;
        case 32:
        {
            iValueReturned = Scierror(iErr, _("Singularity of log or tan function.\n"));
        }
        break;
        case 33:
        {
            iValueReturned = Scierror(iErr, _("Too many ':'\n"));
        }
        break;
        case 34:
        {
            iValueReturned = Scierror(iErr, _("Incorrect control instruction syntax.\n"));
            errtyp = 1; /* error not recoverable */
            C2F(errmgr)(&iErr, &errtyp);
        }
        break;
        case 35:
        {
            switch (C2F(recu).ids[C2F(recu).pt * nsiz - nsiz]) /* find instruction type */
            {
                case 1: /* if */
                    iValueReturned = Scierror(iErr, _("Syntax error in a '%s' instruction.\n"), "if");
                    break;

                case 2: /* while */
                    iValueReturned = Scierror(iErr, _("Syntax error in a '%s' instruction.\n"), "while");
                    break;

                default: /* select/case */
                    iValueReturned = Scierror(iErr, _("Syntax error in a '%s' instruction.\n"), "select/case");
                    break;
            }
            errtyp = 1; /* error not recoverable */
            C2F(errmgr)(&iErr, &errtyp);
        }
        break;
        case 36:
        {
            if (Err == 1)
            {
                iValueReturned = Scierror(iErr, _("Wrong first argument.\n"));
            }
            else
            {
                iValueReturned = Scierror(iErr, _("Wrong input argument #%d.\n"), Err);
            }
        }
        break;
        case 37:
        {
            if (Err > 0)
            {
                iValueReturned = Scierror(iErr, _("Incorrect function at line %d.\n"), Err);
            }
            else
            {
                iValueReturned = Scierror(iErr, _("Incorrect function.\n"));
            }
        }
        break;
        case 38:
        {
            iValueReturned = Scierror(iErr, _("Wrong file name.\n"));
        }
        break;
        case 39:
        {
            iValueReturned = Scierror(iErr, _("Incorrect number of input arguments.\n"));
        }
        break;
        case 40:
        {
            iValueReturned = Scierror(iErr, _("Waiting for end of command.\n"));
            errtyp = 1; /* error not recoverable */
            C2F(errmgr)(&iErr, &errtyp);
        }
        break;
        case 41:
        {
            iValueReturned = Scierror(iErr, _("Incompatible output argument.\n"));
        }
        break;
        case 42:
        {
            iValueReturned = Scierror(iErr, _("Incompatible input argument.\n"));
        }
        break;
        case 43:
        {
            iValueReturned = Scierror(iErr, _("Not implemented in scilab...\n"));
        }
        break;
        case 44:
        {
            if (Err == 1)
            {
                iValueReturned = Scierror(iErr, _("Wrong first argument.\n"));
            }
            else
            {
                iValueReturned = Scierror(iErr, _("Wrong argument #%d.\n"), Err);
            }
        }
        break;
        case 45:
        {
            iValueReturned = Scierror(iErr, _("null matrix (argument # %d).\n"), Err);
        }
        break;
        case 46:
        {
            iValueReturned = Scierror(iErr, _("Incorrect syntax.\n"));
            errtyp = 1; /* error not recoverable */
            C2F(errmgr)(&iErr, &errtyp);
        }
        break;
        case 47:
        {
            iValueReturned = Scierror(iErr, _(" end or else is missing...\n"));
            errtyp = 1; /* error not recoverable */
            C2F(errmgr)(&iErr, &errtyp);
        }
        break;
        case 48:
        {
            iValueReturned = Scierror(iErr, _(" input line longer than buffer size: %d\n"), bsiz);
        }
        break;
        case 49:
        {
            iValueReturned = Scierror(iErr, _("Incorrect file or format.\n"));
        }
        break;
        case 50:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_5);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: subroutine not found.\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 51:
        {
            /* no message  */
            iValueReturned = Scierror(iErr, "");
        }
        break;
        case 52:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: Real matrix expected.\n"), Err);
        }
        break;
        case 53:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for input argument #%d: Real or complex matrix expected.\n"), Err);
        }
        break;
        case 54:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for input argument #%d: Polynomial expected.\n"), Err);
        }
        break;
        case 55:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: String expected.\n"), Err);
        }
        break;
        case 56:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: List expected.\n"), Err);
        }
        break;
        case 57:
        {
            iValueReturned = Scierror(iErr, _("Problem with comparison symbol...\n"));
        }
        break;
        case 58:
        {
            if (Rhs == 0)
            {
                iValueReturned = Scierror(iErr, _("Wrong number of input arguments: This function has no input argument.\n"));
            }
            else
            {
                iValueReturned = Scierror(iErr, _("Wrong number of input arguments."));
            }
        }
        break;
        case 59:
        {
            if (Lhs == 0)
            {
                iValueReturned = Scierror(iErr, _("Wrong number of output arguments: This function has no output argument.\n"));
            }
            else
            {
                iValueReturned = Scierror(iErr, _("Wrong number of output arguments.\n"));
            }
        }
        break;
        case 60:
        {
            iValueReturned = Scierror(iErr, _("Wrong size for argument: Incompatible dimensions.\n"));
        }
        break;
        case 61:
        {
            iValueReturned = Scierror(iErr, _("Direct access : give format.\n"));
        }
        break;
        case 62:
        {
            iValueReturned = Scierror(iErr, _("End of file at line %d.\n"), Err);
        }
        break;
        case 63:
        {
            iValueReturned = Scierror(iErr, _("%d graphic terminal?\n"), Err);
        }
        break;
        case 64:
        {
            iValueReturned = Scierror(iErr, _("Integration fails.\n"));
        }
        break;
        case 65:
        {
            iValueReturned = Scierror(iErr, _("%d: logical unit already used.\n"), Err);
        }
        break;
        case 66:
        {
            iValueReturned = Scierror(iErr, _("Too many files opened!\n"));
        }
        break;
        case 67:
        {
            iValueReturned = Scierror(iErr, _("Unknown file format.\n"));
        }
        break;
        case 68:
        {
            char msgErr[bsiz];
            sprintf(msgErr, _("Fatal error!!! Your variables have been saved in the file : %s\n"), get_sci_data_strings(SAVE_ID));
            strcat(msgErr, _("Bad call to a scilab function ?\n"));
            strcat(msgErr, _("Otherwise, send a bug report to :"));
            strcat(msgErr, "http://bugzilla.scilab.org/\n");
            iValueReturned = Scierror(iErr, msgErr);
        }
        break;
        case 69:
        {
            iValueReturned = Scierror(iErr, _("Floating point exception.\n"));
        }
        break;
        case 70:
        {
            iValueReturned = Scierror(iErr, _("Too many arguments in fort (max 30).\n"));
        }
        break;
        case 71:
        {
            iValueReturned = Scierror(iErr, _("This variable is not valid in fort.\n"));
        }
        break;
        case 72:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_2);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s is not valid in this context.\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 73:
        {
            iValueReturned = Scierror(iErr, _("Error while linking.\n"));
        }
        break;
        case 74:
        {
            iValueReturned = Scierror(iErr, _("Leading coefficient is zero.\n"));
        }
        break;
        case 75:
        {
            iValueReturned = Scierror(iErr, _("Too high degree (max 100).\n"));
        }
        break;
        case 76:
        {
            iValueReturned = Scierror(iErr, _("for x=val with type(val)=%d  is not implemented in Scilab.\n"), Err);
        }
        break;
        case 77:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong number of input arguments.\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 78:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong number of output arguments.\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 79:
        {
            iValueReturned = Scierror(iErr, _("Indexing not allowed for output arguments of resume.\n"), Err);
        }
        break;
        case 80:
        {
            iValueReturned = Scierror(iErr, _("Incorrect function (argument n: %d).\n"), Err);
        }
        break;
        case 81:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Real or complex matrix expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 82:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 83:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Real vector expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 84:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Scalar expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 85:
        {
            iValueReturned = Scierror(iErr, _("Host does not answer...\n"));
        }
        break;
        case 86:
        {
            iValueReturned = Scierror(iErr, _("Uncontrollable system.\n"));
        }
        break;
        case 87:
        {
            iValueReturned = Scierror(iErr, _("Unobservable system.\n"));
        }
        break;
        case 88:
        {
            iValueReturned = Scierror(iErr, _("%s: singular or asymmetric problem.\n"), "sfact");
        }
        break;
        case 89:
        {
            iValueReturned = Scierror(iErr, _("Wrong size for argument #%d.\n"), Err);
        }
        break;
        case 90:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: Transfer matrix expected.\n"), Err);
        }
        break;
        case 91:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: In state space form expected.\n"), Err);
        }
        break;
        case 92:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: Rational matrix expected.\n"), Err);
        }
        break;
        case 93:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: In continuous time expected.\n"), Err);
        }
        break;
        case 94:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: In discrete time expected.\n"), Err);
        }
        break;
        case 95:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: SISO expected.\n"), Err);
        }
        break;
        case 96:
        {
            iValueReturned = Scierror(iErr, _("time domain of argument #%d is not defined.\n"), Err);
        }
        break;
        case 97:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: A system in state space or transfer matrix form expected.\n"), Err);
        }
        break;
        case 98:
        {
            iValueReturned = Scierror(iErr, _("Variable returned by scilab argument function is incorrect.\n"));
        }
        break;
        case 99:
        {
            if (Err != 1)
            {
                iValueReturned = Scierror(iErr, _("Elements of %dth argument must be in increasing order.\n"), Err);
            }
            else
            {
                iValueReturned = Scierror(iErr, _("Elements of first argument are not (strictly) increasing.\n"));
            }
        }
        break;
        case 100:
        {
            if (Err != 1)
            {
                iValueReturned = Scierror(iErr, _("Elements of %dth argument are not in (strictly) decreasing order.\n"), Err);
            }
            else
            {
                iValueReturned = Scierror(iErr, _("Elements of first argument are not in (strictly) decreasing order.\n"));
            }
        }
        break;
        case 101:
        {
            if (Err != 1)
            {
                iValueReturned = Scierror(iErr, _("Last element of %dth argument <> first.\n"), Err);
            }
            else
            {
                iValueReturned = Scierror(iErr, _("Last element of first argument does not match the first one.\n"));
            }
        }
        break;
        case 102:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_2);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("Variable or function %s are not in file.\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 103:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_3);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("Variable %s is not a valid rational function.\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 104:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_3);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("Variable %s is not a valid state space representation.\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 105:
        {
            iValueReturned = Scierror(iErr, _("Undefined function.\n"));
        }
        break;
        case 106:
        {
            iValueReturned = Scierror(iErr, _("Function name already used.\n"));
        }
        break;
        case 107:
        {
            iValueReturned = Scierror(iErr, _("Too many functions are defined (maximum #:%d).\n"), Err);
        }
        break;
        case 108:
        {
            iValueReturned = Scierror(iErr, _("Too complex for scilab, may be a too long control instruction.\n"));
        }
        break;
        case 109:
        {
            iValueReturned = Scierror(iErr, _("Too large, can't be displayed.\n"));
        }
        break;
        case 110:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_2);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s was a function when compiled but is now a primitive!\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 111:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_2);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("Trying to re-define function %s.\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 112:
        {
            iValueReturned = Scierror(iErr, _("No more memory.\n"));
        }
        break;
        case 113:
        {
            iValueReturned = Scierror(iErr, _("Too large string.\n"));
        }
        break;
        case 114:
        {
            iValueReturned = Scierror(iErr, _("Too many linked routines.\n"));
        }
        break;
        case 115:
        {
            char msgErr[bsiz];
            strcpy(msgErr, _("Stack problem detected within a loop.\nA primitive function has been called with a wrong number of output arguments.\nNo output argument test has been made for this function.\nPlease report this bug :\n"));
            strcat(msgErr, "http://bugzilla.scilab.org/\n");
            iValueReturned = Scierror(iErr, msgErr);
        }
        break;
        case 116:
        {
            iValueReturned = Scierror(iErr, _("Wrong value for argument #%d.\n"), Err);
        }
        break;
        case 117:
        {
            iValueReturned = Scierror(iErr, _("List element number %d is Undefined.\n"), Err);
        }
        break;
        case 118:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: Named variable not an expression expected.\n"), Err);
        }
        break;
        case 119:
        {
            /* no message  */
            iValueReturned = Scierror(iErr, "");
        }
        break;
        case 120:
        {
            iValueReturned = Scierror(iErr, _("Indices for non-zero elements must be given by a 2 column matrix.\n"));
        }
        break;
        case 121:
        {
            iValueReturned = Scierror(iErr, _("Incompatible indices for non-zero elements.\n"));
        }
        break;
        case 122:
        {
            iValueReturned = Scierror(iErr, _("Logical unit number should be larger than %d.\n"), Err);
        }
        break;
        case 123:
        {
            iValueReturned = Scierror(iErr, _("Function not bounded from below.\n"));
        }
        break;
        case 124:
        {
            /* no message  */
            iValueReturned = Scierror(iErr, "");
        }
        break;
        case 125:
        {
            iValueReturned = Scierror(iErr, _("Problem may be unbounded: too high distance between two consecutive iterations.\n"));
        }
        break;
        case 126:
        {
            iValueReturned = Scierror(iErr, _("Inconsistent constraints.\n"));
        }
        break;
        case 127:
        {
            iValueReturned = Scierror(iErr, _("No feasible solution.\n"));
        }
        break;
        case 128:
        {
            iValueReturned = Scierror(iErr, _("Degenerate starting point.\n"));
        }
        break;
        case 129:
        {
            iValueReturned = Scierror(iErr, _("No feasible point has been found.\n"));
        }
        break;
        case 130:
        {
            iValueReturned = Scierror(iErr, _("Optimization fails: back to initial point.\n"));
        }
        break;
        case 131:
        {
            iValueReturned = Scierror(iErr, _("%s: Stop requested by simulator (ind=0)\n"), "optim");
        }
        break;
        case 132:
        {
            iValueReturned = Scierror(iErr, _("%s: Wrong input parameters.\n"), "optim");
        }
        break;
        case 133:
        {
            iValueReturned = Scierror(iErr, _("Too small memory.\n"));
        }
        break;
        case 134:
        {
            iValueReturned = Scierror(iErr, _("%s: Problem with initial constants in simul.\n"), "optim");
        }
        break;
        case 135:
        {
            iValueReturned = Scierror(iErr, _("%s: Bounds and initial guess are incompatible.\n"), "optim");
        }
        break;
        case 136:
        {
            iValueReturned = Scierror(iErr, _("%s: This method is NOT implemented.\n"), "optim");
        }
        break;
        case 137:
        {
            iValueReturned = Scierror(iErr, _("NO hot restart available in this method.\n"));
        }
        break;
        case 138:
        {
            iValueReturned = Scierror(iErr, _("%s: Incorrect stopping parameters.\n"), "optim");
        }
        break;
        case 139:
        {
            iValueReturned = Scierror(iErr, _("%s: Incorrect bounds.\n"), "optim");
        }
        break;
        case 140:
        {

            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_6);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("Variable : %s must be a list\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }

        }
        break;
        case 141:
        {
            iValueReturned = Scierror(iErr, _("Incorrect function (argument n: %d).\n"), Err);
        }
        break;
        case 142:
        {
            iValueReturned = Scierror(iErr, _("Hot restart: dimension of working table (argument n:%d).\n"), Err);
        }
        break;
        case 143:
        {
            iValueReturned = Scierror(iErr, _("%s: df0 must be positive !\n"), "optim");
        }
        break;
        case 144:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
            if (NameVarOnStack)
            {
                char msgFormat[bsiz * 2];
                sprintf(msgFormat, "%s%s", _("Undefined operation for the given operands.\n"), _("check or define function %s for overloading.\n"));
                iValueReturned = Scierror(iErr, msgFormat, NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;

        /* error 145 to 199  not used */

        /*
        messages from 200 to 203 and 205 to 214 are no more used by error
        (see Scierror in stack1.c)
        */
        case 200:
        {
            /* not used */
            /* only for compatibility */
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Matrix of handle expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 201:
        {
            /* not used */
            /* only for compatibility */
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Real or Complex matrix expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 202:
        {
            /* not used */
            /* only for compatibility */
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 203:
        {
            /* not used */
            /* only for compatibility */
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Real vector expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 204:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Scalar expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 205:
        {
            int minvalue = C2F(recu).pstk[C2F(recu).pt];
            int maxvalue = C2F(recu).pstk[C2F(recu).pt + 1];

            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong size for argument #%d: (%d,%d) expected.\n"), NameVarOnStack, Err, minvalue, maxvalue);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 206:
        {
            int vectorsize = C2F(recu).pstk[C2F(recu).pt];

            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong size for argument #%d: %d expected.\n"), NameVarOnStack, Err, vectorsize);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 207:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Matrix of strings expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 208:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Boolean matrix expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 209:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Matrix expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 210:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: List expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 211:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Function or string (external function) expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 212:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Polynomial matrix expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 213:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Working int matrix expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 214:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("%s: Wrong type for argument #%d: Vector expected.\n"), NameVarOnStack, Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 215:
        {
            if (Err != 1)
            {
                iValueReturned = Scierror(iErr, _("%dth argument type must be boolean.\n"), Err);
            }
            else
            {
                iValueReturned = Scierror(iErr, _("Argument type must be boolean.\n"));
            }
        }
        break;
        case 216:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: Boolean or scalar expected.\n"), Err);
        }
        break;
        case 217:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: Sparse matrix of scalars expected.\n"), Err);
        }
        break;
        case 218:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for argument #%d: Handle to sparse lu factors expected.\n"), Err);
        }
        break;
        case 219:
        {
            iValueReturned = Scierror(iErr, _("Wrong type argument #%d: Sparse or full scalar matrix expected.\n"), Err);
        }
        break;
        case 220:
        {
            iValueReturned = Scierror(iErr, _("Null variable cannot be used here.\n"));
        }
        break;
        case 221:
        {
            iValueReturned = Scierror(iErr, _("A sparse matrix entry is defined with two different values.\n"));
        }
        break;
        case 222:
        {
            iValueReturned = Scierror(iErr, _("%s not yet implemented for full input parameter.\n"), "lusolve");
        }
        break;
        case 223:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("It is not possible to redefine the %s primitive this way (see clearfun).\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 224:
        {
            iValueReturned = Scierror(iErr, _("Type data base is full.\n"));
        }
        break;
        case 225:
        {
            iValueReturned = Scierror(iErr, _("This data type is already defined.\n"));
        }
        break;
        case 226:
        {
            iValueReturned = Scierror(iErr, _("Inequality comparison with empty matrix.\n"));
        }
        break;
        case 227:
        {
            iValueReturned = Scierror(iErr, _("Missing index.\n"));
        }
        break;
        case 228:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("reference to the cleared global variable %s.\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 229:
        {
            iValueReturned = Scierror(iErr, _("Operands of / and \\ operations must not contain NaN of Inf.\n"));
        }
        break;
        case 230:
        {
            iValueReturned = Scierror(iErr, _("semi def fails.\n"));
        }
        break;
        case 231:
        {
            iValueReturned = Scierror(iErr, _("Wrong type for first input argument: Single string expected.\n"));
        }
        break;
        case 232:
        {
            iValueReturned = Scierror(iErr, _("Entry name not found.\n"));
        }
        break;
        case 233:
        {
            iValueReturned = Scierror(iErr, _("Maximum number of dynamic interfaces reached.\n"));
        }
        break;
        case 234:
        {
            iValueReturned = Scierror(iErr, _("%s: expecting more than one argument.\n"), "link");
        }
        break;
        case 235:
        {
            iValueReturned = Scierror(iErr, _("%s: problem with one of the entry point.\n"), "link");
        }
        break;
        case 236:
        {
            iValueReturned = Scierror(iErr, _("%s: the shared archive was not loaded.\n"), "link");
        }
        break;
        case 237:
        {
            iValueReturned = Scierror(iErr, _("%s: Only one entry point allowed on this operating system.\n"), "link");
        }
        break;
        case 238:
        {
            iValueReturned = Scierror(iErr, _("%s: First argument cannot be a number.\n"), "link");
        }
        break;
        case 239:
        {
            iValueReturned = Scierror(iErr, _("You cannot link more functions, maxentry reached.\n"));
        }
        break;
        case 240:
        {
            /* OBSOLETE */
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_5);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("File \"%s\" already exists or directory write access denied.\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 241:
        {
            /* OBSOLETE */
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_5);
            if (NameVarOnStack)
            {
                iValueReturned = Scierror(iErr, _("File \"%s\" does not exist or read access denied.\n"), NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 242:
        {
            iValueReturned = Scierror(iErr, _("Binary direct access files must be opened by 'file'.\n"));
        }
        break;
        case 243:
        {
            iValueReturned = Scierror(iErr, _("C file logical unit not allowed here.\n"));
        }
        break;
        case 244:
        {
            iValueReturned = Scierror(iErr, _("Fortran file logical unit not allowed here.\n"));
        }
        break;
        case 245:
        {
            iValueReturned = Scierror(iErr, _("No input file associated to logical unit %d.\n"), Err);
        }
        break;
        case 246:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_6);
            if (NameVarOnStack)
            {
                char msgFormat[bsiz * 2];
                sprintf(msgFormat, "%s%s", _("Function not defined for given argument type(s),\n"), _("  check arguments or define function %s for overloading.\n"));
                iValueReturned = Scierror(iErr, msgFormat, NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
            else
            {
                iValueReturned = Scierror(iErr, _("Function not defined for given argument type(s),\n"));
            }
        }
        break;
        case 247:
        {
            iValueReturned = Scierror(iErr, _("Wrong value for argument #%d: the lu handle is no more valid.\n"), Err);
        }
        break;
        case 248:
        {
            iValueReturned = Scierror(iErr, _("Wrong value for argument #%d: Valid variable name expected.\n"), Err);
        }
        break;
        case 249:
        {
            iValueReturned = Scierror(iErr, _("Wrong value for argument #%d: Empty string expected.\n"), Err);
        }
        break;
        case 250:
        {
            iValueReturned = Scierror(iErr, _("Recursive extraction is not valid in this context.\n"));
        }
        break;
        case 251:
        {
            iValueReturned = Scierror(iErr, _("%s: ipar dimensioned at least 11.\n"), "bvode");
        }
        break;
        case 252:
        {
            iValueReturned = Scierror(iErr, _("%s: ltol must be of size ipar(4).\n"), "bvode");
        }
        break;
        case 253:
        {
            iValueReturned = Scierror(iErr, _("%s: fixpnt must be of size ipar(11).\n"), "bvode");
        }
        break;
        case 254:
        {
            iValueReturned = Scierror(iErr, _("%s: ncomp < 20 requested.\n"), "bvode");
        }
        break;
        case 255:
        {
            iValueReturned = Scierror(iErr, _("%s: m must be of size ncomp.\n"), "bvode");
        }
        break;
        case 256:
        case 257:
        {
            iValueReturned = Scierror(iErr, _("%s: sum(m) must be less than 40.\n"), "bvode");
        }
        break;
        case 258:
        {
            iValueReturned = Scierror(iErr, _("%s: input data error.\n"), "bvode");
        }
        break;
        case 259:
        {
            iValueReturned = Scierror(iErr, _("%s: no. of subintervals exceeds storage.\n"), "bvode");
        }
        break;
        case 260:
        {
            iValueReturned = Scierror(iErr, _("%s: The colocation matrix is singular.\n"), "bvode");
        }
        break;
        case 261:
        {
            iValueReturned = Scierror(iErr, _("Interface property table is full.\n"));
        }
        break;
        case 262:
        {
            int maxglobalvariables = isizt - C2F(vstk).isiz - 1;
            iValueReturned = Scierror(iErr, _("Too many global variables! Max number is %d.\n"), maxglobalvariables);
        }
        break;
        case 263:
        {
            iValueReturned = Scierror(iErr, _("Error while writing in file: disk full or deleted file.\n"));
        }
        break;
        case 264:
        {
            iValueReturned = Scierror(iErr, _("Wrong value for argument #%d: Must not contain NaN or Inf.\n"), Err);
        }
        break;
        case 265:
        {
            iValueReturned = Scierror(iErr, _("%s and %s must have equal number of rows.\n"), "A", "B");
        }
        break;
        case 266:
        {
            iValueReturned = Scierror(iErr, _("%s and %s must have equal number of columns.\n"), "A", "B");
        }
        break;
        case 267:
        {
            iValueReturned = Scierror(iErr, _("%s and %s must have equal dimensions.\n"), "A", "B");
        }
        break;
        case 268:
        {
            iValueReturned = Scierror(iErr, _("Invalid return value for function passed in arg %d.\n"), Err);
        }
        break;
        case 269:
        {
            iValueReturned = Scierror(iErr, _("Wrong value for argument #%d: eigenvalues must have negative real parts.\n"), Err);
        }
        break;
        case 270:
        {
            iValueReturned = Scierror(iErr, _("Wrong value for argument #%d: eigenvalues modulus must be less than one.\n"), Err);
        }
        break;
        case 271:
        {
            iValueReturned = Scierror(iErr, _("Size varying argument a*eye(), (arg %d) not allowed here.\n"), Err);
        }
        break;
        case 272:
        {
            iValueReturned = Scierror(iErr, _("endfunction is missing.\n"));
        }
        break;
        case 273:
        {
            iValueReturned = Scierror(iErr, _("Instruction left hand side: waiting for a dot or a left parenthesis.\n"));
        }
        break;
        case 274:
        {
            iValueReturned = Scierror(iErr, _("Instruction left hand side: waiting for a name.\n"));
        }
        break;
        case 275:
        {
            iValueReturned = Scierror(iErr, _("varargout keyword cannot be used here.\n"));
        }
        break;
        case 276:
        {
            iValueReturned = Scierror(iErr, _("Missing operator, comma, or semicolon.\n"));
            errtyp = 1; /* error not recoverable */
            C2F(errmgr)(&iErr, &errtyp);
        }
        break;
        case 277:
        {
            iValueReturned = Scierror(iErr, _("Too many commands defined.\n"));
        }
        break;
        case 278:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                if (NameVarOnStack[0] != '0')
                {
                    iValueReturned = Scierror(iErr, _("%s: Input arguments should have the same formal variable name.\n"), NameVarOnStack);
                }
                else
                {
                    iValueReturned = Scierror(iErr, _("Input arguments should have the same formal variable name.\n"));
                }
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
        case 279:
        case 280:
        {
            /* no message  */
            iValueReturned = Scierror(iErr, "");
        }
        break;
        case 281:
        {
            char localbuf[3];
            strncpy(localbuf, C2F(cha1).buf, 2);
            localbuf[2] = '\0';
            iValueReturned = Scierror(iErr, _("%s: Wrong value for input argument #%d: Wrong value for element %s.\n"), "bvode", 7, localbuf);
        }
        break;

        default:
        {
            char *buffer = defaultStringError();
            if (buffer)
            {
                iValueReturned = Scierror(iErr, buffer);
                FREE(buffer);
                buffer = NULL;
            }
        }
        break;
    }
    return iValueReturned;
}
/*--------------------------------------------------------------------------*/
static void strip_blank(char *source)
{
    char *p;
    p = source;
    /* look for end of string */
    while (*p != '\0')
    {
        p++;
    }
    while (p != source)
    {
        p--;
        if (*p != ' ')
        {
            break;
        }
        *p = '\0';
    }
}
/*--------------------------------------------------------------------------*/
static char *getConvertedNameFromStack(int cvnametype)
{
    int one = 1;

    char local_variable_buffer[bsiz];

    switch (cvnametype)
    {
        case CVNAME_READING_TYPE_1 :
        {
            C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], C2F(cha1).buf, &one, bsiz);
            strncpy(local_variable_buffer, C2F(cha1).buf, nlgh);
            local_variable_buffer[nlgh - 1] = '\0';
        }
        break;

        case CVNAME_READING_TYPE_2 :
        {
            C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], local_variable_buffer, &one, nlgh);
            local_variable_buffer[nlgh] = '\0';
        }
        break;

        case CVNAME_READING_TYPE_3 :
        {
            C2F(cvname)(&C2F(recu).ids[C2F(recu).pt * nsiz - nsiz], C2F(cha1).buf, &one, bsiz);
            strncpy(local_variable_buffer, C2F(cha1).buf, nlgh);
            local_variable_buffer[nlgh - 1] = '\0';
        }
        break;

        case CVNAME_READING_TYPE_4 :
        {
#define SHIFT_CHAR 3
            C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], C2F(cha1).buf + SHIFT_CHAR, &one, nlgh + 1);
            strncpy(local_variable_buffer, C2F(cha1).buf + SHIFT_CHAR, nlgh);
            local_variable_buffer[nlgh] = '\0';
        }
        break;

        case CVNAME_READING_TYPE_5 :
        {
            strncpy(local_variable_buffer, C2F(cha1).buf, bsiz);
            local_variable_buffer[bsiz - 1] = '\0';
        }
        break;

        case CVNAME_READING_TYPE_6 :
        {
            C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], C2F(cha1).buf, &one, nlgh + 1);
            strncpy(local_variable_buffer, C2F(cha1).buf, nlgh);
            local_variable_buffer[nlgh - 1] = '\0';
        }
        break;

        default:
            strcpy(local_variable_buffer, _("Unknown data.\n"));
            break;
    }

    strip_blank(local_variable_buffer);
    /* bug 9571: % duplicated for variable name (used in sprintf) */
    return strsub(local_variable_buffer, "%", "%%");
}
/*--------------------------------------------------------------------------*/
static char *defaultStringError(void)
{
    char *buffer = NULL;
    int bufl = 1;

    while (*(unsigned char *)&C2F(cha1).buf[bufl - 1] != '\0' && bufl < 80)
    {
        ++bufl;
    }
    --bufl;


    buffer = (char*)MALLOC( ((int)strlen(C2F(cha1).buf) + 1) * sizeof(char));

    /* remove blank */
    if (buffer)
    {
        strcpy(buffer, C2F(cha1).buf);
        strip_blank(buffer);
    }
    return buffer;
}
/*--------------------------------------------------------------------------*/
