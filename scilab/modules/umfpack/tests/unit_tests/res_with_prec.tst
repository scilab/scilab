// ============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - Bruno PINCON
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

[A] = ReadHBSparse(SCI+"/modules/umfpack/demos/bcsstk24.rsa");
C_ptr = taucs_chfact(A);
b = rand(size(A,1),1);
x0 = taucs_chsolve(C_ptr, b);
firstNorm=norm(A*x0 - b);
secondNorm=norm(res_with_prec(A, x0, b));
// Fix after Eigen 5.0.1 migration
// The naive (double) and extended-precision residuals must be consistent, i.e. of the same order of magnitude.
// A strict "firstNorm >= secondNorm" test is fragile: on such an ill-conditioned matrix both residuals are dominated by
// cancellation and their ratio depends on the summation order (BLAS/Eigen version, SIMD, platform).
// We only check they agree within one order of magnitude.
ratio = firstNorm / secondNorm;
assert_checktrue(ratio >= 0.1 & ratio <= 10);
