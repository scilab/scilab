/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

extern "C"{
#include "checkodeerror.h"
#include "sciprint.h"
#include "localization.h"
#include "sci_warning.h"
}
// return 0 = ok, return 1 = error, return 2 = warning
int checkOdeError(int meth, int istate)
{       
    switch(meth)
    {
        case 0 : // lsoda
        case 3 : // lsodar
        {
            if(istate == -7)
            {
                sciprintW(_W("Work space insufficient to finish (see messages).\n"));
                return 1;
            }
        }
        case 1 : // lsode | lsodi (adams)
        case 2 : // lsode | lsodi (stiff)
        {
            if(istate == -1)
            {
                sciprintW(_W("Excess work done on this call (perhaps wrong jacobian type).\n"));
                return 1;
            }
            else if(istate == -2)
            {
                sciprintW(_W("Excess accuracy requested (tolerances too small).\n"));
                return 1;
            }
            else if(istate == -3)
            {
                sciprintW(_W("Illegal input detected (see printed message).\n"));
                return 1;
            }
            else if(istate == -4)
            {
                sciprintW(_W("Repeated error test failures (check all inputs).\n"));
                return 1;
            }
            else if(istate == -5)
            {
                sciprintW(_W("Repeated convergence failures (perhaps bad jacobian supplied or wrong choice of jacobian type or tolerances).\n"));
                return 1;
            }
            else if(istate == -6)
            {
                sciprintW(_W("Error weight became zero during problem. (solution component i vanished, and atol or atol(i) = 0.)\n"));
                return 1;
            }
            else if(istate == -7) // only used with impl (lsodi)
            {
                sciprintW(_W("The user-supplied subroutine res set its error flag (ires = 3) despite repeated tries by lsodi to avoid that condition.\n"));
                return 1;
            }
            else if(istate == -8) // only used with impl (lsodi)
            {
                sciprintW(_W("istate was 0 on input but lsodi was unable to compute the initial value of dy/dt.\n"));
                return 1;
            }
            break;
        }
        case 4 : // lsdisc (discrete)
        {
            if(istate < 0) // from lsdisc.f :
            {              // istate = 2 if lsdisc was successful, negative otherwise...
                return 1;
            }
            break;
        }
        case 5 : // lsrgk (rk)
        {
            if(istate == -1) // from lsrgk.f
            {
                return 1;
            }
            break;
        }
        case 6 : // rkf45 (rkf)
        {
            switch(istate)
            {
                case 3:
                {
                    if(getWarningMode())
                    {
                        sciprintW(_W("Integration was not completed because relative error tolerance was too small. rerr has been increased appropriately for continuing.\n")); 
                    }
                        return 2;
                }
                case 4:
                {
                    if(getWarningMode())
                    {
                        sciprintW(_W("Integration was not completed because more than 3000 derivative evaluations were needed. This is approximately 500 steps.\n"));
                    }
                        return 2;
                }
                case 5:
                {
                    sciprintW(_W("Integration was not completed because solution vanished making a pure relative error test impossible. Must use non-zero aerr to continue. Using the one-step integration mode for one step is a good way to proceed.\n"));
                        return 1;
                }
                case 6:
                {
                    sciprintW(_W("Integration was not completed because requested accuracy could not be achieved using smallest allowable stepsize. User must increase the error tolerance before continued integration can be attempted.\n"));
                        return 1;
                }
                case 7:
                {
                    sciprintW(_W("It is likely that rkf45 is inefficient for solving this problem. Too much output is restricting the natural stepsize choice. Use the one-step integrator mode.\n"));
                        return 1;
                }
                case 8:
                {
                    sciprintW(_W("invalid input parameters : atol and rtol must be greater than 0.\n"));
                        return 1;
                }
                default : break;
            }
            break;
        }
        case 7 : // rksimp (fix)
        {
            if(istate == 3)
            {
                return 1;
            }
            else if(istate == 4)
            {
                sciprintW(_W("Inappropriate error tolerance.\n"));
                return 1;
            }
            break;
        }
    }
    return 0;
}

int checkDasslError(int idid)
{
    switch(idid)
    {
        case 4 ://Only used in dasrt
        {       //One or more root found at t
            return 2;
        }
        case 3 ://The integration to TOUT was successfully completed (T=TOUT) by 
        {       //stepping past TOUT. Y,ydot are obtained by interpolation.
            return 0;
        }
        case 2 : //The integration to TSTOP was successfully completed (T=TSTOP)
        {
            return 2;
        }
        case 1 : //A step was successfully taken in the intermediate-output mode.
        {        //The code has not yet reached TOUT.
            return 0;
        }
        case -1 : //A large amount of work has been expended (About 500 steps)
        {
            if(getWarningMode())
            {
                sciprintW(_W("To many steps necessary to reached next required time discretization point. Change discretisation of time vector t or decrease accuracy.\n")); 
            }
            return 2;
        }
        case -2 : //The error tolerances are too stringent.
        {
            sciprintW(_W("The error tolerances are too stringent.\n"));
            return 1;
        }
        case -3 :
        {
            //The local error test cannot be satisfied because you specified 
            //a zero component in ATOL and the corresponding computed solution
            //component is zero. Thus, a pure relative error test is impossible 
            //for this component.
            if(getWarningMode())
            {
                sciprintW(_W("Atol and computed test value are zero.\n")); 
            }
            return 2;
        }
        case -6 : //Repeated error test failures on the last attempted step.
        {
            if(getWarningMode())
            {
                sciprintW(_W("A singularity in the solution may be present.\n")); 
            }
            return 2;
        }
        case -7 : //The corrector could not converge.
        {
            if(getWarningMode())
            {
                sciprintW(_W("May be inaccurate or ill-conditioned JACOBIAN.\n")); 
            }
            return 2;
        }
        case -8 : //The matrix of partial derivatives is singular.
        {
            sciprintW(_W("Singular partial derivatives matrix (may be redundant equations).\n"));
            return 1;
        }
        case -9 : //The corrector could not converge.
        {         //There were repeated error test failures in this step.
            if(getWarningMode())
            {
                sciprintW(_W("Either ill-posed problem or discontinuity or singularity encountered.\n")); 
            }
            return 2;
        }
        case -10 :
        {
            if(getWarningMode())
            {
                sciprintW(_W("External ''res'' return many times with ires=-1.\n")); 
            }
            return 2;
        }
        case -11 : //IRES equal to -2 was encountered and
        {          //control is being returned to the calling program.
            sciprintW(_W("Error in external ''res''.\n"));
            return 1;
        }
        case -12 : //DDASSL failed to compute the initial YPRIME.
        {
            sciprintW(_W("dassl failed to compute the initial Ydot.\n"));
            return 1;
        }
        case -33 :
        {
            //The code has encountered trouble from which
            //it cannot recover. A message is printed
            //explaining the trouble and control is returned
            //to the calling program. For example, this occurs
            //when invalid input is detected.
            if(getWarningMode())
            {
                sciprintW(_W("dassl encountered trouble.\n"));
            }
            return 2;
        }
    }

    return 1;
}

