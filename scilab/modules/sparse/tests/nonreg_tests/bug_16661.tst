// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16661 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16661
//
// <-- Short Description -->
// x=spzeros(1e10,1e10) yields an incorrect error message

INT_MAX = int32(%inf);

cmd = "spzeros(1e10,1)";
msg = msprintf(_("%s: Wrong value for input argument #%d: Must be less than %d.\n"),"spzeros",1, INT_MAX)
assert_checkerror(cmd,msg);

cmd = "spzeros(1,1e10)";
msg = msprintf(_("%s: Wrong value for input argument #%d: Must be less than %d.\n"),"spzeros",2, INT_MAX)
assert_checkerror(cmd,msg);

cmd = "sparse([1 1],1,[1e10,1e10])";
msg = msprintf(_("%s: Wrong values for input argument #%d: Elements must be less than %d.\n"),"sparse",3,INT_MAX)
assert_checkerror(cmd,msg);
