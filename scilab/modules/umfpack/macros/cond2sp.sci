//   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
//   <bruno.pincon@iecn.u-nancy.fr>
//
// This set of scilab 's macros provide a few sparse utilities.
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [K2, lm, vm, lM, vM] = cond2sp(A, C, rtol, itermax, verb)
    //
    //  PURPOSE
    //     for a s.p.d. matrix computes the maximum and minimum
    //     eigen element (value and vector) with the power and
    //     inverse power method then the 2-norm condition number
    //     K2 = lM / lm
    //
    //  PARAMETERS
    //    inputs
    //    ------
    //     A       : a sparse s.p.d. matrix
    //     C       : pointer onto a Cholesky factorization (gotten with
    //               taucs_chfact)
    //     rtol     : (optional) relative precision for the output test
    //                   (l_new - l_old)/l_new < rtol
    //     itermax : (optional) maximum number of iteration in each step
    //     verb    : (optional) a boolean must be %t for display result
    //               for each iteration
    //
    //   outputs
    //   -------
    //     K2      : 2-norm condition number
    //     lm      : min eigenvalue
    //     vm      : associated eigenvector
    //     lM      : max eigenvalue
    //     vM      : associated eigenvector

    //
    [lhs, rhs] = argn()
    // no verif
    if ~exists("verb", "local") then , verb = %f , end
    if ~exists("rtol", "local") then , rtol = 1.e-3, end
    if ~exists("itermax","local") then , itermax = 30 , end
    itermax = max(4,itermax)  // 4 iterations are forced

    // 1) computes (with "direct Rayleigh power method") lM, vM
    n = size(A,1)
    x = rand(n,1) ; x = x / norm(x)
    y = A*x
    lM_old = x'*y
    iter = 0
    if verb then
        mprintf(gettext("\n approximate (lM,vM) with the iterative power method \n"));
        mprintf(gettext(" ----------------------------------------------------- \n"));
    end
    while %t
        iter = iter + 1
        x = y / norm(y)
        y = A*x
        lM = x'*y
        if verb then
            mprintf(gettext(" iteration %3d : lM = %e  \n"), iter, lM);
        end
        crit = abs((lM - lM_old)/lM)
        if crit < rtol  &  iter > 3 then
            break
        else
            lM_old = lM
        end
        if iter >= itermax then

            mprintf(gettext(" Warning : for lM ""convergence"" at rtol = %e \n"), rtol);
            mprintf(gettext("           don''t reached after %d iterations (got only %e) \n"), itermax, crit);
            break
        end
    end
    vM = x

    // 2) computes (with "inverse Rayleigh power method") lm, vm
    x = rand(n,1) ; x = x / norm(x)
    y = taucs_chsolve(C,x)
    lm_old = x'*y
    iter = 0
    if verb then
        mprintf(gettext("\n approximate (lm,vm) with the inverse iterative power method \n"));
        mprintf(gettext(" ------------------------------------------------------------\n"));
    end
    while %t
        iter = iter + 1
        x = y / norm(y)
        y = taucs_chsolve(C,x)
        lm = x'*y
        if verb then
            mprintf(gettext(" iteration %3d : lm = %e  \n"), iter, 1/lm)
        end
        crit = abs((lm - lm_old)/lm)
        if crit < rtol  &  iter > 3 then
            break
        else
            lm_old = lm
        end
        if iter >= itermax then
            mprintf(gettext(" Warning : for lm ""convergence"" at rtol = %e \n"), rtol);
            mprintf(gettext("           don''t reached after %d iterations (got only %e) \n"),itermax, crit);
            break
        end
    end
    vm = x
    lm = 1/lm;
    K2 = lM/lm;

endfunction
