// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

exec(SCI+'/modules/differential_equations/tests/unit_tests/bvode_tst.sci');


[z,zf]=col1();
if max(abs(z-zf))>1.e-5 then pause,end

[z,zf]=col2(0);
// Fortran Coded version 
if max(abs(z-zf))>1.e-2 then pause,end

[z,zf]=col2(1);
// Scilab coded version 
if max(abs(z-zf))>1.e-2 then pause,end



