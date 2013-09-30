// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10216 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10216
//
// <-- Short Description -->
// Invalid syntaxes for zeros, ones, eye, rand like zeros(2,:)

errmsg0=msprintf(_("Invalid index.\n"));

errmsg1=msprintf(_("%s: Wrong type for input argument #%d: Real scalar expected.\n"),"zeros",1);
errmsg2=msprintf(_("%s: Wrong type for input argument #%d: Real scalar expected.\n"),"zeros",2);
assert_checkerror("zeros(:)", errmsg0, 21);
assert_checkerror("zeros(:,2)", errmsg1, 999);
assert_checkerror("zeros(2,:)", errmsg2, 999);
assert_checkerror("zeros(:,:)", errmsg1, 999);

errmsg1=msprintf(_("%s: Wrong type for input argument #%d: Real scalar expected.\n"),"ones",1);
errmsg2=msprintf(_("%s: Wrong type for input argument #%d: Real scalar expected.\n"),"ones",2);
assert_checkerror("ones(:)", errmsg0, 21);
assert_checkerror("ones(:,2)", errmsg1, 999);
assert_checkerror("ones(2,:)", errmsg2, 999);
assert_checkerror("ones(:,:)", errmsg1, 999);

errmsg1=msprintf(_("%s: Wrong type for input argument #%d: Real scalar expected.\n"),"eye",1);
errmsg2=msprintf(_("%s: Wrong type for input argument #%d: Real scalar expected.\n"),"eye",2);
assert_checkerror("eye(:)", errmsg0, 21);
assert_checkerror("eye(:,2)", errmsg1, 999);
assert_checkerror("eye(2,:)", errmsg2, 999);
assert_checkerror("eye(:,:)", errmsg1, 999);

errmsg1=msprintf(_("%s: Wrong type for input argument #%d: Real scalar expected.\n"),"rand",1);
errmsg2=msprintf(_("%s: Wrong type for input argument #%d: Real scalar expected.\n"),"rand",2);
assert_checkerror("rand(:)", errmsg0, 21);
assert_checkerror("rand(:,""uniform"")", errmsg0, 21);
assert_checkerror("rand(:,""normal"")", errmsg0, 21);
assert_checkerror("rand(:,2)", errmsg1, 999);
assert_checkerror("rand(2,:)", errmsg2, 999);
assert_checkerror("rand(:,2,""uniform"")", errmsg1, 999);
assert_checkerror("rand(2,:,""uniform"")", errmsg2, 999);
assert_checkerror("rand(:,2,""normal"")", errmsg1, 999);
assert_checkerror("rand(2,:,""normal"")", errmsg2, 999);
