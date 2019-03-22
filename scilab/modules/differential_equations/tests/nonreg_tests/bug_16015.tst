// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16015 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16015
//
// <-- Short Description -->
// intg(a,b,f)  makes a first calls to g(x) with x=1 regardless of a and b

deff('y=f(t)','if t<2 | t>3 then error(''bad argument'') else y=1 end');
intg(2,3,f)
