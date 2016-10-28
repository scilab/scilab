/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2016 - Scilab Enterprises - Adeline CARNIS
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
#include "diary_manager.hxx"
#include <vector>

extern "C"
{
#include "gw_io.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_output_stream.h"
}

static const char fname[] = "getio";
/*--------------------------------------------------------------------------*/
int sci_getio(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt opt, int nout, scilabVar* out)
{

	if (nin != 0)
	{
		Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), fname, 0);
		return 1;
	}

	if (nout != 1)
	{
		Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), fname, 1);
		return 1;
	}

    //get diarys count
    int diary = 0;
    int* diarys = getDiaryIDs(&diary);
    //don't care about ids
    delete[] diarys;

    //create data values array
    std::vector<double> val = {5, 5, (double)diary, 6};
    //create output matrix
    scilabVar var = scilab_createDoubleMatrix2d(env, 1, 4, 0);
    //fill it
    scilab_setDoubleArray(env, var, val.data());
    //return it to scilab
    out[0] = var;
	return 0;
}