/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "errmsg.h"
#include "localization.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "stackinfo.h"
#include "core_math.h"
#include "inffic.h"
#include "msgstore.h"
#include "msgout.h"
#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif
/*--------------------------------------------------------------------------*/
extern int C2F(showstack)(); /* used in error 115 */
extern int C2F(prntid)(); /* to print variables on stack */
/*--------------------------------------------------------------------------*/
static void strip_blank(char *source);
static void displayAndStoreError(const char *msg,...);
static void resetLastError(void);
static char *getConvertedNameFromStack(int cvnametype);
static char *defaultStringError(void);
/*--------------------------------------------------------------------------*/
#define EMPTY_BUFFER "00000000000000000000000"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif
/*--------------------------------------------------------------------------*/
typedef enum {
    CVNAME_READING_TYPE_1 = 0, 
    CVNAME_READING_TYPE_2 = 1, 
    CVNAME_READING_TYPE_3 = 2, 
    CVNAME_READING_TYPE_4 = 3,
    CVNAME_READING_TYPE_5 = 4,
    CVNAME_READING_TYPE_6 = 5,
} CVNAME_TYPE;
/*--------------------------------------------------------------------------*/
int C2F(errmsg)(int *n,int *errtyp)
{
    resetLastError();

    /* store error code (lasterror) */
    C2F(errstore)(n);

    *errtyp = 0; /* by default errors are recoverable */
    /* errors not recoverable aren't catchable by top
    try,catch */ 
    /* errors 2,3,16,26,31,34,35,40,46,47,276*/

    switch ((int)*n)
    {
    case 1:
        {
            displayAndStoreError(_("Incorrect assignment.\n"));

        }
        break;
    case 2:
        {
            displayAndStoreError(_("Invalid factor.\n"));
            *errtyp = 1;
        }
        break;
    case 3:
        {
            displayAndStoreError(_("Waiting for right parenthesis.\n"));
            *errtyp = 1;
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
                    displayAndStoreError(_("Undefined variable: %s\n"), NameVarOnStack);
                    FREE(NameVarOnStack);
                    NameVarOnStack = NULL;
                    break;
                }

                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
            displayAndStoreError(_("Undefined variable.\n"));
        }
        break;
    case 5:
        {
            displayAndStoreError(_("Inconsistent column/row dimensions.\n"));
        }
        break;
    case 6:
        {
            displayAndStoreError(_("Inconsistent row/column dimensions.\n"));
        }
        break;
    case 7:
        {
            displayAndStoreError(_("Dot cannot be used as modifier for this operator.\n"));
        }
        break;
    case 8:
        {
            displayAndStoreError(_("Inconsistent addition.\n"));
        }
        break;
    case 9:
        {
            displayAndStoreError(_("Inconsistent subtraction.\n"));
        }
        break;
    case 10:
        {
            displayAndStoreError(_("Inconsistent multiplication.\n"));
        }
        break;
    case 11:
        {
            displayAndStoreError(_("Inconsistent right division.\n"));
        }
        break;
    case 12:
        {
            displayAndStoreError(_("Inconsistent left division.\n"));
        }
        break;
    case 13:
        {
            displayAndStoreError(_("Redefining permanent variable.\n"));
        }
        break;
    case 14:
        {
            displayAndStoreError(_("Eye variable undefined in this context.\n"));
        }
        break;
    case 15:
        {
            displayAndStoreError(_("Submatrix incorrectly defined.\n"));
        }
        break;
    case 16:
        {
            displayAndStoreError(_("Incorrect command!\n"));
            *errtyp = 1; /* error not recoverable */
        }
        break;
    case 17:
        {
            int Memory_used_for_variables = 0;
            int Intermediate_Memory = 0;
            int Total_Memory_available = 0;

            Intermediate_Memory = getIntermediateMemoryNeeded();
            C2F(getstackinfo)(&Total_Memory_available,&Memory_used_for_variables);

            displayAndStoreError(_("stack size exceeded!\n"));
            displayAndStoreError(_("Use stacksize function to increase it.\n"));
            displayAndStoreError(_("Memory used for variables: %d\n"),Memory_used_for_variables);
            displayAndStoreError(_("Intermediate memory needed: %d\n"),Intermediate_Memory);
            displayAndStoreError(_("Total memory available: %d\n"),Total_Memory_available);
        }
        break;
    case 18:
        {
            displayAndStoreError(_("Too many variables!\n"));
        }
        break;
    case 19:
        {
            displayAndStoreError(_("Problem is singular.\n"));
        }
        break;
    case 20:
        {
            if (Err == 1)
            {
                displayAndStoreError(_("Wrong type for first argument: Square matrix expected.\n"));
            }
            else
            {
                displayAndStoreError(_("Wrong type for argument %d: Square matrix expected.\n"),Err);
            }
        }
        break;
    case 21:
        {
            displayAndStoreError(_("Invalid index.\n"));
        }
        break;
    case 22:
        {
            displayAndStoreError(_("Recursion problems. Sorry...\n"));
        }
        break;
    case 23:
        {
            displayAndStoreError(_("Matrix norms available are 1, 2, inf, and fro.\n"));
        }
        break;
    case 24:
        {
            displayAndStoreError(_("Convergence problem...\n"));
        }
        break;
    case 25:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_2);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("Bad call to primitive: %s\n"),NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 26:
        {
            displayAndStoreError(_("Too complex recursion! (recursion tables are full)\n"));
            /* break recursion */
            C2F(recu).pt = Min(C2F(recu).pt,psiz);
            *errtyp = 1; /* error not recoverable */
        }
        break;
    case 27:
        {
            displayAndStoreError(_("Division by zero...\n"));
        }
        break;
    case 28:
        {
            displayAndStoreError(_("Empty function...\n"));
        }
        break;
    case 29:
        {
            displayAndStoreError(_("Matrix is not positive definite.\n"));
        }
        break;
    case 30:
        {
            displayAndStoreError(_("Invalid exponent.\n"));
        }
        break;
    case 31:
        {
            displayAndStoreError(_( "Incorrect string.\n"));
            *errtyp = 1; /* error not recoverable */
        }
        break;
    case 32:
        {
            displayAndStoreError(_("Singularity of log or tan function.\n"));
        }
        break;
    case 33:
        {
            displayAndStoreError(_("Too many ':'\n"));
        }
        break;
    case 34:
        {
            displayAndStoreError(_("Incorrect control instruction syntax.\n"));
            *errtyp = 1; /* error not recoverable */
        }
        break;
    case 35:
        {
            switch (C2F(recu).ids[C2F(recu).pt * nsiz - nsiz]) /* find instruction type */
            {
            case 1: /* if */
                displayAndStoreError(_("Syntax error in a '%s' instruction.\n"),"if");
                break;

            case 2: /* while */
                displayAndStoreError(_("Syntax error in a '%s' instruction.\n"),"while");
                break;

            default: /* select/case */
                displayAndStoreError(_("Syntax error in a '%s' instruction.\n"),"select/case");
                break;
            }
            *errtyp = 1; /* error not recoverable */
        }
        break;
    case 36:
        {
            if (Err == 1)
            {
                displayAndStoreError(_("Wrong first argument.\n"));
            }
            else
            {
                displayAndStoreError(_("Wrong input argument %d.\n"),Err);
            }
        }
        break;
    case 37:
        {
            if (Err>0) 
                displayAndStoreError(_("Incorrect function at line %d.\n"),Err);
            else
                displayAndStoreError(_("Incorrect function.\n"));
        }
        break;
    case 38:
        {
            displayAndStoreError(_("Wrong file name.\n"));
        }
        break;
    case 39:
        {
            displayAndStoreError(_("Incorrect number of input arguments.\n"));
        }
        break;
    case 40:
        {
            displayAndStoreError(_("Waiting for end of command.\n"));
            *errtyp = 1; /* error not recoverable */
        }
        break;
    case 41:
        {
            displayAndStoreError(_("Incompatible output argument.\n"));
        }
        break;
    case 42:
        {
            displayAndStoreError(_("Incompatible input argument.\n"));
        }
        break;
    case 43:
        {
            displayAndStoreError(_("Not implemented in scilab...\n"));
        }
        break;
    case 44:
        {
            if (Err == 1)
            {
                displayAndStoreError(_("Wrong first argument.\n"));
            }
            else
            {
                displayAndStoreError(_("Wrong argument %d.\n"),Err);
            }

        }
        break;
    case 45:
        {
            displayAndStoreError(_("null matrix (argument # %d).\n"),Err);
        }
        break;
    case 46:
        {
            displayAndStoreError(_("Incorrect syntax.\n"));
            *errtyp = 1; /* error not recoverable */
        }
        break;
    case 47:
        {
            displayAndStoreError(_(" end or else is missing...\n"));
            *errtyp = 1; /* error not recoverable */
        }
        break;
    case 48:
        {
            displayAndStoreError(_(" input line longer than buffer size: %d\n"),bsiz);
        }
        break;
    case 49:
        {
            displayAndStoreError(_("Incorrect file or format.\n"));
        }
        break;
    case 50:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_5);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("subroutine not found: %s\n"),NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 51:
        {
            /* no message  */
        }
        break;
    case 52:
        {
            displayAndStoreError(_("Wrong type for argument %d: Real matrix expected.\n"),Err);
        }
        break;
    case 53:
        {
            displayAndStoreError(_("Wrong type for input argument %d: Real or complex matrix expected.\n"),Err);
        }
        break;
    case 54:
        {
            displayAndStoreError(_("Wrong type for input argument %d: Polynomial expected.\n"),Err);
        }
        break;
    case 55:
        {
            displayAndStoreError(_("Wrong type for argument %d: String expected.\n"),Err);
        }
        break;
    case 56:
        {
            displayAndStoreError(_("Wrong type for argument %d: List expected.\n"),Err);
        }
        break;
    case 57:
        {
            displayAndStoreError(_("Problem with comparison symbol...\n"));
        }
        break;
    case 58:
        {
            if (Rhs == 0)
            {
                displayAndStoreError(_("Wrong number of input arguments: This function has no input argument.\n"));
            }
            else
            {
                displayAndStoreError(_("Wrong number of input arguments:\n"));
                displayAndStoreError(_("Arguments are :\n"));
                /* print variables name on stack :( */
                C2F(prntid)(istk(C2F(recu).pstk[C2F(recu).pt - 1]), &Rhs, &C2F(iop).wte);
            }

        }
        break;
    case 59:
        {
            if (Lhs == 0)
            {
                displayAndStoreError(_("Wrong number of output arguments: This function has no output argument.\n"));
            }
            else
            {
                displayAndStoreError(_("Wrong number of output arguments\n"));
                displayAndStoreError(_("Arguments are :\n"));
                /* print variables name on stack :( */
                C2F(prntid)(istk(C2F(recu).pstk[C2F(recu).pt - 1]), &Lhs, &C2F(iop).wte);
            }
        }
        break;
    case 60:
        {
            displayAndStoreError(_("Wrong size for argument: Incompatible dimensions.\n"));
        }
        break;
    case 61:
        {
            displayAndStoreError(_("Direct access : give format.\n"));
        }
        break;
    case 62:
        {
            displayAndStoreError(_("End of file at line %d.\n"),Err);
        }
        break;
    case 63:
        {
            displayAndStoreError(_("%d graphic terminal?\n"),Err);
        }
        break;
    case 64:
        {
            displayAndStoreError(_("Integration fails.\n"));
        }
        break;
    case 65:
        {
            displayAndStoreError(_("%d: logical unit already used.\n"),Err);
        }
        break;
    case 66:
        {
            displayAndStoreError(_("Too many files opened!\n"));
        }
        break;
    case 67:
        {
            displayAndStoreError(_("Unknown file format.\n"));
        }
        break;
    case 68:
        {
            displayAndStoreError(_("Fatal error!!! Your variables have been saved in the file : %s\n\
                                   Bad call to a scilab function ?\n\
                                   Otherwise, send a bug report to :\n"),get_sci_data_strings(SAVE_ID));
            displayAndStoreError("http://bugzilla.scilab.org/\n");
        }
        break;
    case 69:
        {
            displayAndStoreError(_("Floating point exception.\n"));
        }
        break;
    case 70:
        {
            displayAndStoreError(_("Too many arguments in fort (max 30).\n"));
        }
        break;
    case 71:
        {
            displayAndStoreError(_("This variable is not valid in fort.\n"));
        }
        break;
    case 72:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_2);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("%s is not valid in this context.\n"),NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 73:
        {
            displayAndStoreError(_("Error while linking.\n"));
        }
        break;
    case 74:
        {
            displayAndStoreError(_("Leading coefficient is zero.\n"));
        }
        break;
    case 75:
        {
            displayAndStoreError(_("Too high degree (max 100).\n"));
        }
        break;
    case 76:
        {
            displayAndStoreError(_("for x=val with type(val)=%d  is not implemented in Scilab.\n"),Err);
        }
        break;
    case 77:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("%s: Wrong number of input arguments.\n"),NameVarOnStack);
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
                displayAndStoreError(_("%s: Wrong number of output arguments.\n"),NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 79:
        {
            displayAndStoreError(_("Indexing not allowed for output arguments of resume.\n"),Err);

        }
        break;
    case 80:
        {
            displayAndStoreError(_("Incorrect function (argument n: %d).\n"),Err);
        }
        break;
    case 81:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("%s: Wrong type for argument %d: Real or complex matrix expected.\n"),NameVarOnStack,Err);
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
                displayAndStoreError(_("%s: Wrong type for argument %d: Real matrix expected.\n"),NameVarOnStack, Err);
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
                displayAndStoreError(_("%s: Wrong type for argument %d: Real vector expected.\n"),NameVarOnStack,Err);
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
                displayAndStoreError(_("%s: Wrong type for argument %d: Scalar expected.\n"),NameVarOnStack,Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 85:
        {
            displayAndStoreError(_("Host does not answer...\n"));
        }
        break;
    case 86:
        {
            displayAndStoreError(_("Uncontrollable system.\n"));
        }
        break;
    case 87:
        {
            displayAndStoreError(_("Unobservable system.\n"));
        }
        break;
    case 88:
        {
            displayAndStoreError(_("%s: singular or asymmetric problem.\n"),"sfact");
        }
        break;
    case 89:
        {
            displayAndStoreError(_("Wrong size for argument %d.\n"),Err);
        }
        break;
    case 90:
        {
            displayAndStoreError(_("Wrong type for argument %d: Transfer matrix expected.\n"),Err);
        }
        break;
    case 91:
        {
            displayAndStoreError(_("Wrong type for argument %d: In state space form expected.\n"),Err);
        }
        break;
    case 92:
        {
            displayAndStoreError(_("Wrong type for argument %d: Rational matrix expected.\n"),Err);
        }
        break;
    case 93:
        {
            displayAndStoreError(_("Wrong type for argument %d: In continuous time expected.\n"),Err);
        }
        break;
    case 94:
        {
            displayAndStoreError(_("Wrong type for argument %d: In discrete time expected.\n"),Err);
        }
        break;
    case 95:
        {
            displayAndStoreError(_("Wrong type for argument %d: SISO expected.\n"),Err);
        }
        break;
    case 96:
        {
            displayAndStoreError(_("time domain of argument %d is not defined.\n"),Err);
        }
        break;
    case 97:
        {
            displayAndStoreError(_("Wrong type for argument %d: A system in state space or transfer matrix form expected.\n"),Err);
        }
        break;
    case 98:
        {
            displayAndStoreError(_("Variable returned by scilab argument function is incorrect.\n"));
        }
        break;
    case 99:
        {
            if (Err != 1)
            {
                displayAndStoreError(_("Elements of %dth argument must be in increasing order.\n"),Err);
            }
            else
            {
                displayAndStoreError(_("Elements of first argument are not (strictly) increasing.\n"));
            }
        }
        break;
    case 100:
        {
            if (Err != 1)
            {
                displayAndStoreError(_("Elements of %dth argument are not in (strictly) decreasing order.\n"),Err);
            }
            else
            {
                displayAndStoreError(_("Elements of first argument are not in (strictly) decreasing order.\n"));
            }
        }
        break;
    case 101:
        {
            if (Err != 1)
            {
                displayAndStoreError(_("Last element of %dth argument <> first.\n"),Err);
            }
            else
            {
                displayAndStoreError(_("Last element of first argument does not match the first one.\n"));
            }
        }
        break;
    case 102:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_2);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("Variable or function %s are not in file.\n"),NameVarOnStack);
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
                displayAndStoreError(_("Variable %s is not a valid rational function.\n"),NameVarOnStack);
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
                displayAndStoreError(_("Variable %s is not a valid state space representation.\n"),NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 105:
        {
            displayAndStoreError(_("Undefined fonction.\n"));
        }
        break;
    case 106:
        {
            displayAndStoreError(_("Function name already used.\n"));
        }
        break;
    case 107:
        {
            displayAndStoreError(_("Too many functions are defined (maximum #:%d).\n"),Err);
        }
        break;
    case 108:
        {
            displayAndStoreError(_("Too complex for scilab, may be a too long control instruction.\n"));
        }
        break;
    case 109:
        {
            displayAndStoreError(_("Too large, can't be displayed.\n"));
        }
        break;
    case 110:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_2);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("%s was a function when compiled but is now a primitive!\n"),NameVarOnStack);
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
                displayAndStoreError(_("Trying to re-define function %s.\n"),NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 112:
        {
            displayAndStoreError(_("No more memory.\n"));
        }
        break;
    case 113:
        {
            displayAndStoreError(_("Too large string.\n"));
        }
        break;
    case 114:
        {
            displayAndStoreError(_("Too many linked routines.\n"));
        }
        break;
    case 115:
        {
            displayAndStoreError(_("Stack problem detected within a loop.\nA primitive function has been called with a wrong number of output arguments.\nNo output argument test has been made for this function.\nPlease report this bug :\n"));
            displayAndStoreError("http://bugzilla.scilab.org/\n");

            C2F(showstack)(); /* display of calling tree */
        }
        break;
    case 116:
        {

            displayAndStoreError(_("Wrong value for argument %d.\n"),Err);
        }
        break;
    case 117:
        {
            displayAndStoreError(_("List element number %d is Undefined.\n"),Err);
        }
        break;
    case 118:
        {

            displayAndStoreError(_("Wrong type for argument %d: Named variable not an expression expected.\n"),Err);
        }
        break;
    case 119:
        {
            /* no message  */
        }
        break;
    case 120:
        {
            displayAndStoreError(_("Indices for non-zero elements must be given by a 2 column matrix.\n"));
        }
        break;
    case 121:
        {
            displayAndStoreError(_("Incompatible indices for non-zero elements.\n"));
        }
        break;
    case 122:
        {
            displayAndStoreError(_("Logical unit number should be larger than %d.\n"),Err);
        }
        break;
    case 123:
        {
            displayAndStoreError(_("Function not bounded from below.\n"));
        }
        break;
    case 124:
        {
            /* no message  */
        }
        break;
    case 125:
        {
            displayAndStoreError(_("Problem may be unbounded: too high distance between two consecutive iterations.\n"));
        }
        break;
    case 126:
        {
            displayAndStoreError(_("Inconsistent constraints.\n"));
        }
        break;
    case 127:
        {
            displayAndStoreError(_("No feasible solution.\n"));
        }
        break;
    case 128:
        {
            displayAndStoreError(_("Degenerate starting point.\n"));
        }
        break;
    case 129:
        {
            displayAndStoreError(_("No feasible point has been found.\n"));
        }
        break;
    case 130:
        {
            displayAndStoreError(_("Optimization fails: back to initial point.\n"));
        }
        break;
    case 131:
        {
            displayAndStoreError(_("%s: Stop requested by simulator (ind=0)\n"),"optim");
        }
        break;
    case 132:
        {
            displayAndStoreError(_("%s: Wrong input parameters.\n"),"optim");
        }
        break;
    case 133:
        {
            displayAndStoreError(_("Too small memory.\n"));
        }
        break;
    case 134:
        {
            displayAndStoreError(_("%s: Problem with initial constants in simul.\n"),"optim");
        }
        break;
    case 135:
        {
            displayAndStoreError(_("%s: Bounds and initial guess are incompatible.\n"),"optim");
        }
        break;
    case 136:
        {
            displayAndStoreError(_("%s: This method is NOT implemented.\n"),"optim");
        }
        break;
    case 137:
        {
            displayAndStoreError(_("NO hot restart available in this method.\n"));
        }
        break;
    case 138:
        {
            displayAndStoreError(_("%s: Incorrect stopping parameters.\n"),"optim");
        }
        break;
    case 139:
        {
            displayAndStoreError(_("%s: Incorrect bounds.\n"),"optim");
        }
        break;
    case 140:
        {

            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_6);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("Variable : %s must be a list\n"),NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }

        }
        break;
    case 141:
        {
            displayAndStoreError(_("Incorrect function (argument n: %d).\n"),Err);
        }
        break;
    case 142:
        {
            displayAndStoreError(_("Hot restart: dimension of working table (argument n:%d).\n"),Err);
        }
        break;
    case 143:
        {
            displayAndStoreError(_("%s: df0 must be positive !\n"),"optim");
        }
        break;
    case 144:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("Undefined operation for the given operands.\n"));
                displayAndStoreError(_("check or define function %s for overloading.\n"),NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;

        /* error 145 to 199  not used */

        /*
        messages from 201 to 203 and 205 to 214 are no more used by error
        (see Scierror  in stack1.c)
        */
    case 200:
        {
            /* no message  */
        }
        break;
    case 201:
        {
            /* not used  */
            /* only for compatibility */
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("%s: Wrong type for argument %d: Real or complex matrix expected.\n"),NameVarOnStack,Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 202:
        {
            /* not used  */
            /* only for compatibility */
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("%s: Wrong type for argument %d: Real matrix expected.\n"),NameVarOnStack,Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 203:
        {
            /* not used  */
            /* only for compatibility */
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("%s: Wrong type for argument %d: Real vector expected.\n"),NameVarOnStack,Err);
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
                displayAndStoreError(_("%s: Wrong type for argument %d: Scalar expected.\n"),NameVarOnStack,Err);
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
                displayAndStoreError(_("%s: Wrong size for argument %d: (%d,%d) expected.\n"),NameVarOnStack,Err,minvalue,maxvalue);
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
                displayAndStoreError(_("%s: Wrong size for argument %d: %d expected.\n"),NameVarOnStack,Err,vectorsize);
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
                displayAndStoreError(_("%s: Wrong type for argument %d: Matrix of strings expected.\n"),NameVarOnStack,Err);
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
                displayAndStoreError(_("%s: Wrong type for argument %d: Boolean matrix expected.\n"),NameVarOnStack,Err);
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
                displayAndStoreError(_("%s: Wrong type for argument %d: Matrix expected.\n"),NameVarOnStack,Err);
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
                displayAndStoreError(_("%s: Wrong type for argument %d: List expected.\n"),NameVarOnStack, Err);
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
                displayAndStoreError(_("%s: Wrong type for argument %d: Function or string (external function) expected.\n"),NameVarOnStack,Err);
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
                displayAndStoreError(_("%s: Wrong type for argument %d: Polynomial expected.\n"),NameVarOnStack,Err);
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
                displayAndStoreError(_("%s: Wrong type for argument %d: Working int matrix expected.\n"),NameVarOnStack,Err);
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
                displayAndStoreError(_("%s: Wrong type for argument %d: Vector expected.\n"),NameVarOnStack,Err);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 215:
        {
            if (Err != 1)
            {
                displayAndStoreError(_("%dth argument type must be boolean.\n"),Err);
            }
            else
            {
                displayAndStoreError(_("Argument type must be boolean.\n"));
            }
        }
        break;
    case 216:
        {
            displayAndStoreError(_("Wrong type for argument %d: Boolean or scalar expected.\n"),Err);
        }
        break;
    case 217:
        {
            displayAndStoreError(_("Wrong type for argument %d: Sparse matrix of scalars expected.\n"),Err);
        }
        break;
    case 218:
        {
            displayAndStoreError(_("Wrong type for argument %d: Handle to sparse lu factors expected.\n"),Err);
        }
        break;
    case 219:
        {

            displayAndStoreError(_("Wrong type argument %d: Sparse or full scalar matrix expected.\n"),Err);
        }
        break;
    case 220:
        {
            displayAndStoreError(_("Null variable cannot be used here.\n"));
        }
        break;
    case 221:
        {
            displayAndStoreError(_("A sparse matrix entry is defined with two different values.\n"));
        }
        break;
    case 222:
        {
            displayAndStoreError(_("%s not yet implemented for full input parameter.\n"),"lusolve");
        }
        break;
    case 223:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("It is not possible to redefine the %s primitive this way (see clearfun).\n"),NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 224:
        {
            displayAndStoreError(_("Type data base is full.\n"));
        }
        break;
    case 225:
        {
            displayAndStoreError(_("This data type is already defined.\n"));
        }
        break;
    case 226:
        {
            displayAndStoreError(_("Inequality comparison with empty matrix.\n"));
        }
        break;
    case 227:
        {
            displayAndStoreError(_("Missing index.\n"));
        }
        break;
    case 228:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("reference to the cleared global variable %s.\n"),NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 229:
        {
            displayAndStoreError(_("Operands of / and \\ operations must not contain NaN of Inf.\n"));
        }
        break;
    case 230:
        {
            displayAndStoreError(_("semi def fails.\n"));
        }
        break;
    case 231:
        {
            displayAndStoreError(_("Wrong type for first input argument: Single string expected.\n"));
        }
        break;
    case 232:
        {
            displayAndStoreError(_("Entry name not found.\n"));
        }
        break;
    case 233:
        {
            displayAndStoreError(_("Maximum number of dynamic interfaces reached.\n"));
        }
        break;
    case 234:
        {
            displayAndStoreError(_("%s: expecting more than one argument.\n"),"Link");
        }
        break;
    case 235:
        {
            displayAndStoreError(_("%s: problem with one of the entry point.\n"),"Link");
        }
        break;
    case 236:
        {
            displayAndStoreError(_("%s: the shared archive was not loaded.\n"),"Link");
        }
        break;
    case 237:
        {
            displayAndStoreError(_("%s: Only one entry point allowed on this operating system.\n"),"Link");
        }
        break;
    case 238:
        {
            displayAndStoreError(_("%s: First argument cannot be a number.\n"),"Link");
        }
        break;
    case 239:
        {
            displayAndStoreError(_("You cannot link more functions, maxentry reached.\n"));
        }
        break;
    case 240:
        {
            /* OBSOLETE */
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_5);
            if (NameVarOnStack)
            {
                displayAndStoreError(_("File \"%s\" already exists or directory write access denied.\n"),NameVarOnStack);
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
                displayAndStoreError(_("File \"%s\" does not exist or read access denied.\n"),NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 242:
        {
            displayAndStoreError(_("Binary direct access files must be opened by 'file'.\n"));
        }
        break;
    case 243:
        {
            displayAndStoreError(_("C file logical unit not allowed here.\n"));
        }
        break;
    case 244:
        {
            displayAndStoreError(_("Fortran file logical unit not allowed here.\n"));
        }
        break;
    case 245:
        {
            displayAndStoreError(_("No input file associated to logical unit %d.\n"),Err);
        }
        break;
    case 246:
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_6);
            displayAndStoreError(_("Function not defined for given argument type(s),\n"));
            if (NameVarOnStack)
            {
                displayAndStoreError(_("  check arguments or define function %s for overloading.\n"),NameVarOnStack);
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 247:
        {
            displayAndStoreError(_("Wrong value for argument %d: the lu handle is no more valid.\n"),Err);
        }
        break;
    case 248:
        {			
            displayAndStoreError(_("Wrong value for argument %d: Valid variable name expected.\n"),Err);
        }
        break;
    case 249:
        {
            displayAndStoreError(_("Wrong value for argument %d: Empty string expected.\n"),Err);
        }
        break;
    case 250:
        {
            displayAndStoreError(_("Recursive extraction is not valid in this context.\n"));
        }
        break;
    case 251:
        {
            displayAndStoreError(_("%s: ipar dimensioned at least 11.\n"),"bvode");
        }
        break;
    case 252:
        {
            displayAndStoreError(_("%s: ltol must be of size ipar(4).\n"),"bvode");
        }
        break;
    case 253:
        {
            displayAndStoreError(_("%s: fixpnt must be of size ipar(11).\n"),"bvode");
        }
        break;
    case 254:
        {
            displayAndStoreError(_("%s: ncomp < 20 requested.\n"),"bvode");
        }
        break;
    case 255:
        {
            displayAndStoreError(_("%s: m must be of size ncomp.\n"),"bvode");
        }
        break;
    case 256: case 257:
        {
            displayAndStoreError(_("%s: sum(m) must be less than 40.\n"),"bvode");
        }
        break;
    case 258:
        {
            displayAndStoreError(_("%s: input data error.\n"),"bvode");
        }
        break;
    case 259:
        {
            displayAndStoreError(_("%s: no. of subintervals exceeds storage.\n"),"bvode");
        }
        break;
    case 260:
        {
            displayAndStoreError(_("%s: The colocation matrix is singular.\n"),"bvode");
        }
        break;
    case 261:
        {
            displayAndStoreError(_("Interface property table is full.\n"));
        }
        break;
    case 262:
        {
            int maxglobalvariables = isizt - C2F(vstk).isiz - 1;
            displayAndStoreError(_("Too many global variables! Max number is %d.\n"),maxglobalvariables);
        }
        break;
    case 263:
        {
            displayAndStoreError(_("Error while writing in file: disk full or deleted file.\n"));
        }
        break;
    case 264:
        {
            displayAndStoreError(_("Wrong value for argument %d: Must not contain NaN or Inf.\n"),Err);
        }
        break;
    case 265:
        {
            displayAndStoreError(_("%s and %s must have equal number of rows.\n"),"A","B");
        }
        break;
    case 266:
        {
            displayAndStoreError(_("%s and %s must have equal number of columns.\n"),"A","B");
        }
        break;
    case 267:
        {
            displayAndStoreError(_("%s and %s must have equal dimensions.\n"),"A","B");
        }
        break;
    case 268:
        {
            displayAndStoreError(_("Invalid return value for function passed in arg %d.\n"),Err);
        }
        break;
    case 269:
        {
            displayAndStoreError(_("Wrong value for argument %d: eigenvalues must have negative real parts.\n"),Err);
        }
        break;
    case 270:
        {
            displayAndStoreError(_("Wrong value for argument %d: eigenvalues modulus must be less than one.\n"),Err);
        }
        break;
    case 271:
        {
            displayAndStoreError(_("Size varying argument a*eye(), (arg %d) not allowed here.\n"),Err);
        }
        break;
    case 272:
        {
            displayAndStoreError(_("endfunction is missing.\n"));
        }
        break;
    case 273:
        {
            displayAndStoreError(_("Instruction left hand side: waiting for a dot or a left parenthesis.\n"));
        }
        break;
    case 274:
        {
            displayAndStoreError(_("Instruction left hand side: waiting for a name.\n"));
        }
        break;
    case 275:
        {
            displayAndStoreError(_("varargout keyword cannot be used here.\n"));
        }
        break;
    case 276:
        {
            displayAndStoreError(_("Missing operator, comma, or semicolon.\n"));
            *errtyp = 1; /* error not recoverable */
        }
        break;
    case 277:
        {
            displayAndStoreError(_("Too many commands defined.\n"));
        }
        break;
    case 278: 
        {
            char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
            if (NameVarOnStack)
            {
                if (NameVarOnStack[0] != '0')
                {
                    displayAndStoreError(_("%s: Input arguments should have the same formal variable name.\n"),NameVarOnStack);
                }
                else
                {
                    displayAndStoreError(_("Input arguments should have the same formal variable name.\n"));
                }
                FREE(NameVarOnStack);
                NameVarOnStack = NULL;
            }
        }
        break;
    case 279: case 280:
        {
            /* no message  */
        }
        break;

    default:
        {
            char *buffer = defaultStringError();
            if (buffer)
            {
                displayAndStoreError(buffer);
                FREE(buffer);
                buffer = NULL;
            }
        }
        break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static void strip_blank(char *source)
{
    char *p;
    p = source;
    /* look for end of string */
    while(*p != '\0') p++;
    while(p != source)
    {
        p--;
        if(*p != ' ') break;
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
            local_variable_buffer[nlgh-1] = '\0';
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
            local_variable_buffer[nlgh-1] = '\0';
        }
        break;

    case CVNAME_READING_TYPE_4 :
        {
#define SHIFT_CHAR 3
            C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], C2F(cha1).buf + SHIFT_CHAR, &one, nlgh+1);
            strncpy(local_variable_buffer, C2F(cha1).buf+SHIFT_CHAR, nlgh);
            local_variable_buffer[nlgh] = '\0';
        }
        break;

    case CVNAME_READING_TYPE_5 :
        {
            strncpy(local_variable_buffer, C2F(cha1).buf,bsiz);
            local_variable_buffer[bsiz-1] = '\0';
        }
        break;

    case CVNAME_READING_TYPE_6 :
        {
            C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], C2F(cha1).buf, &one, nlgh+1);
            strncpy(local_variable_buffer, C2F(cha1).buf, nlgh);
            local_variable_buffer[nlgh-1] = '\0';
        }
        break;

    default:
        strcpy(local_variable_buffer,_("\"unknown data\""));
        break;
    }

    strip_blank(local_variable_buffer);
    return strdup(local_variable_buffer);
}
/*--------------------------------------------------------------------------*/
static void displayAndStoreError(const char *msg,...)
{
    int io = 0;
    int len = 0;

    va_list ap;
    static char s_buf[bsiz];

    va_start(ap,msg);

#if defined(linux) || defined(_MSC_VER)
    {
        int count=0;
        count= vsnprintf(s_buf,bsiz, msg, ap );
        if (count == -1) s_buf[bsiz-1]='\0';
    }
#else
    (void )vsprintf(s_buf, msg, ap );
#endif
    va_end(ap);

    /* store and display error(lasterror) */

    len = (int)strlen(s_buf);
    C2F(msgout)(&io,&C2F(iop).wte,s_buf,len);
}
/*--------------------------------------------------------------------------*/
static void resetLastError(void)
{
#define SPACE_CHAR " "
    int zero = 0;
    int lenspace = (int)strlen(SPACE_CHAR);

    /* reset lasterror */
    C2F(linestore)(&zero);
    C2F(funnamestore)(SPACE_CHAR, &zero, lenspace);
    C2F(freemsgtable)();
}
/*--------------------------------------------------------------------------*/
static char *defaultStringError(void)
{
    char *buffer = NULL;
    int bufl = 1;

    while(*(unsigned char *)&C2F(cha1).buf[bufl - 1] != '\0' && bufl < 80)
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
