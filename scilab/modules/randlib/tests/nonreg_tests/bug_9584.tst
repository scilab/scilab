// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9584 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9584
//
// <-- Short Description -->
// grand returns non-empty matrix for negative size argument

errMsg1 = msprintf(_("%s: Wrong value for input argument #1: Positive scalar expected.\n"),"Rand");
errMsg2 = msprintf(_("%s: Wrong value for input argument #2: Positive scalar expected.\n"),"Rand");
assert_checkequal(grand(1,0,'def'),[]);
assert_checkerror("grand(-1,1,""def"");", errMsg1);
assert_checkerror("grand(1,-1,""def"");", errMsg2);
assert_checkequal(size(grand(-1,-1,'def')),[-1,-1]); //size(eye())==[-1 -1]
