/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
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

extern "C" {
#include "checkoptimerror.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "configvariable_interface.h"
}
// return 0 = ok, return 1 = error
int checkOptimError(int iarret, int indopt, int imp, double dEpsg)
{
    if (iarret || (imp == 0 && indopt > 0))
    {
        return 0;
    }

    switch (indopt)
    {
        // error
        case 0 :
        {
            Scierror(131, _("%s: Stop requested by simulator (ind=0).\n"), "optim");
            return 1;
        }
        break;
        case -7 :
        {
            Scierror(134, _("%s: Problem with initial constants in simul.\n"), "optim");
            return 1;
        }
        break;
        case -14 :
        {
            Scierror(133, _("%s: Too small memory.\n"), "optim");
            return 1;
        }
        break;
        case -10 :
        {
            Scierror(132, _("%s: Wrong input parameters.\n"), "optim");
            return 1;
        }
        break;

        // warning
        case 1 :
            sciprint(_("%s: Norm of projected gradient lower than %lg.\n"), "optim", dEpsg);
            break;
        case 2 :
            sciprint(_("%s: at last iteration f decreases by less than %lg.\n"), "optim", dEpsg);
            break;
        case 3 :
            sciprint(_("%s: Optimization stops because too small variations for x.\n"), "optim");
            break;
        case 4 :
            sciprint(_("%s: Optim stops: maximum number of calls to f is reached.\n"), "optim");
            break;
        case 5 :
            sciprint(_("%s: Optim stops: maximum number of iterations is reached.\n"), "optim");
            break;
        case 6 :
            sciprint(_("%s: Optim stops: too small variations in gradient direction.\n"), "optim");
            break;
        case 7 :
            sciprint(_("%s: Stop during calculation of descent direction.\n"), "optim");
            break;
        case 8 :
            sciprint(_("%s: Stop during calculation of estimated hessian.\n"), "optim");
            break;
        case 9 :
            sciprint(_("%s: End of optimization.\n"), "optim");
            break;
        case 10 :
            sciprint(_("%s: End of optimization (linear search fails).\n"), "optim");
            break;
    }

    return 0;
}

