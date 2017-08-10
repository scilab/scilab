// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
if firstNorm < secondNorm then pause, end
