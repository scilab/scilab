/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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
#ifndef __GATEWAY_PROTOTYPE_H__
#define __GATEWAY_PROTOTYPE_H__

typedef void* scilabEnv;
typedef int* scilabVar;
typedef int* scilabOpt;

typedef int (*OLDGW_FUNC)(char *fname, void* pvApiCtx);
typedef int(*GW_C_FUNC)(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt opt, int nout, scilabVar* out);
typedef int (*MEXGW_FUNC)(int nlhs, int* plhs[], int nrhs, int* prhs[]);

#define STACK_GATEWAY_PROTOTYPE(__gateway__) int __gateway__(char* fname, void* pvApiCtx)
#define C_GATEWAY_PROTOTYPE(__gateway__) int __gateway__(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt opt, int nout, scilabVar* out)
#define MEX_GATEWAY_PROTOTYPE(__gateway__) int (__gateway__) (int nlhs, int* plhs[], int nrhs, int* prhs[]);

#endif /* !__GATEWAY_PROTOTYPE_H__ */
