/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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
/*--------------------------------------------------------------------------*/

#include "checkodeerror.hxx"

extern "C"
{
#include "sciprint.h"
#include "localization.h"
#include "configvariable_interface.h"
}
// return 0 = ok, return 1 = error, return 2 = warning
int checkOdeError(int meth, int istate)
{
    switch (meth)
    {
        case 0 : // lsoda
        case 3 : // lsodar
        {
            if (istate == -7)
            {
                sciprint(_("Work space insufficient to finish (see messages).\n"));
                return 1;
            }
        }
        case 1 : // lsode | lsodi (adams)
        case 2 : // lsode | lsodi (stiff)
        {
            if (istate == -1)
            {
                sciprint(_("Excess work done on this call (perhaps wrong jacobian type).\n"));
                return 1;
            }
            else if (istate == -2)
            {
                sciprint(_("Excess accuracy requested (tolerances too small).\n"));
                return 1;
            }
            else if (istate == -3)
            {
                sciprint(_("Illegal input detected (see printed message).\n"));
                return 1;
            }
            else if (istate == -4)
            {
                sciprint(_("Repeated error test failures (check all inputs).\n"));
                return 1;
            }
            else if (istate == -5)
            {
                sciprint(_("Repeated convergence failures (perhaps bad jacobian supplied or wrong choice of jacobian type or tolerances).\n"));
                return 1;
            }
            else if (istate == -6)
            {
                sciprint(_("Error weight became zero during problem. (solution component i vanished, and atol or atol(i) = 0.)\n"));
                return 1;
            }
            else if (istate == -7) // only used with impl (lsodi)
            {
                sciprint(_("The user-supplied subroutine res set its error flag (ires = 3) despite repeated tries by lsodi to avoid that condition.\n"));
                return 1;
            }
            else if (istate == -8) // only used with impl (lsodi)
            {
                sciprint(_("istate was 0 on input but lsodi was unable to compute the initial value of dy/dt.\n"));
                return 1;
            }
            break;
        }
        case 4 : // lsdisc (discrete)
        {
            if (istate < 0) // from lsdisc.f :
            {
                // istate = 2 if lsdisc was successful, negative otherwise...
                return 1;
            }
            break;
        }
        case 5 : // lsrgk (rk)
        {
            if (istate == -1) // from lsrgk.f
            {
                return 1;
            }
            break;
        }
        case 6 : // rkf45 (rkf)
        {
            switch (istate)
            {
                case 3:
                {
                    if (getWarningMode())
                    {
                        sciprint(_("Integration was not completed because relative error tolerance was too small. rerr has been increased appropriately for continuing.\n"));
                    }
                    return 2;
                }
                case 4:
                {
                    if (getWarningMode())
                    {
                        sciprint(_("Integration was not completed because more than 3000 derivative evaluations were needed. This is approximately 500 steps.\n"));
                    }
                    return 2;
                }
                case 5:
                {
                    sciprint(_("Integration was not completed because solution vanished making a pure relative error test impossible. Must use non-zero aerr to continue. Using the one-step integration mode for one step is a good way to proceed.\n"));
                    return 1;
                }
                case 6:
                {
                    sciprint(_("Integration was not completed because requested accuracy could not be achieved using smallest allowable stepsize. User must increase the error tolerance before continued integration can be attempted.\n"));
                    return 1;
                }
                case 7:
                {
                    sciprint(_("It is likely that rkf45 is inefficient for solving this problem. Too much output is restricting the natural stepsize choice. Use the one-step integrator mode.\n"));
                    return 1;
                }
                case 8:
                {
                    sciprint(_("invalid input parameters : atol and rtol must be greater than 0.\n"));
                    return 1;
                }
                default :
                    break;
            }
            break;
        }
        case 7 : // rksimp (fix)
        {
            if (istate == 3)
            {
                return 1;
            }
            else if (istate == 4)
            {
                sciprint(_("Inappropriate error tolerance.\n"));
                return 1;
            }
            break;
        }
    }
    return 0;
}

int checkError(int idid, std::string strName)
{
    switch (idid)
    {
        case 4 ://Only used in dasrt
        {
            //One or more root found at t
            return 2;
        }
        case 3 ://The integration to TOUT was successfully completed (T=TOUT) by
        {
            //stepping past TOUT. Y,ydot are obtained by interpolation.
            return 0;
        }
        case 2 : //The integration to TSTOP was successfully completed (T=TSTOP)
        {
            return 2;
        }
        case 1 : //A step was successfully taken in the intermediate-output mode.
        {
            //The code has not yet reached TOUT.
            return 0;
        }
        case -1 : //A large amount of work has been expended (About 500 steps)
        {
            if (getWarningMode())
            {
                sciprint(_("Too many steps necessary to reached next required time discretization point. Change discretisation of time vector t or decrease accuracy.\n"));
            }
            return 2;
        }
        case -2 : //The error tolerances are too stringent.
        {
            sciprint(_("The error tolerances are too stringent.\n"));
            return 1;
        }
        case -3 :
        {
            //The local error test cannot be satisfied because you specified
            //a zero component in ATOL and the corresponding computed solution
            //component is zero. Thus, a pure relative error test is impossible
            //for this component.
            if (getWarningMode())
            {
                sciprint(_("Atol and computed test value are zero.\n"));
            }
            return 2;
        }
        case -5 : // only used in daskr
        {
            // There were repeated failures in the evaluation
            // or processing of the preconditioner (in JAC).
            sciprint(_("Cannot evaluate the preconditioner.\n"));
            return 1;
        }
        case -6 : //Repeated error test failures on the last attempted step.
        {
            if (getWarningMode())
            {
                sciprint(_("A singularity in the solution may be present.\n"));
            }
            return 2;
        }
        case -7 : //The corrector could not converge.
        {
            if (getWarningMode())
            {
                sciprint(_("May be inaccurate or ill-conditioned JACOBIAN.\n"));
            }
            return 2;
        }
        case -8 : //The matrix of partial derivatives is singular.
        {
            sciprint(_("Singular partial derivatives matrix (may be redundant equations).\n"));
            return 1;
        }
        case -9 : //The corrector could not converge.
        {
            //There were repeated error test failures in this step.
            if (getWarningMode())
            {
                sciprint(_("Either ill-posed problem or discontinuity or singularity encountered.\n"));
            }
            return 2;
        }
        case -10 :
        {
            if (getWarningMode())
            {
                sciprint(_("External ''res'' return many times with ires=-1.\n"));
            }
            return 2;
        }
        case -11 : //IRES equal to -2 was encountered and
        {
            //control is being returned to the calling program.
            sciprint(_("Error in external ''res''.\n"));
            return 1;
        }
        case -12 : //DDASSL, dasrt or daskr failed to compute the initial YPRIME.
        {
            sciprint(_("failed to compute the initial Ydot.\n"));
            return 1;
        }
        case -13 : // only used in daskr
        {
            // An unrecoverable error was encountered inside the user's PSOL routine,
            // and control is being returned to the calling program.
            sciprint(_("Error in external psol.\n"));
            return 1;
        }
        case -14 : // only used in daskr
        {
            // The Krylov linear system solver could not achieve convergence.
            sciprint(_("The Krylov linear system did not converge.\n"));
            return 1;
        }
        case -33 :
        {
            //The code has encountered trouble from which
            //it cannot recover. A message is printed
            //explaining the trouble and control is returned
            //to the calling program. For example, this occurs
            //when invalid input is detected.
            if (getWarningMode())
            {
                sciprint(_("%s encountered trouble.\n"), strName.c_str());
            }
            return 2;
        }
    }

    return 1;
}

