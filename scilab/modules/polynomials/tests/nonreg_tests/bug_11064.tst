// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 11064 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11064
//
// <-- Short Description -->
// poly returns a wrong result on x64 (windows)
//

r = clean(coeff(poly(roots(poly([1 1 1],"x","c")),"x","r")));
REF = [1 1 1];
assert_checkalmostequal(r, REF);

