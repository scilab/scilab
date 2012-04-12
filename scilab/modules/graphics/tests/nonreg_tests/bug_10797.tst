// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
//
// <-- Non-regression test for bug 10797 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10797
//
// <-- Short Description -->
// contour, contour2d, contourf did not check input arguments

X = 1:10;
Y = 1:10;
Z = rand(10, 10);
NZ = 5;
rect = [0, 0, 11, 11];

msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "contour2d", 1);
assert_checkerror ("contour2d(''X'', Y, Z, NZ , rect);", msgerr);
msgerr = msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "contour2d", 1);
assert_checkerror ("contour2d(1, Y, Z, NZ , rect);", msgerr);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "contour2d", 2);
assert_checkerror ("contour2d(X, ''Y'', Z, NZ , rect);", msgerr);
msgerr = msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "contour2d", 2);
assert_checkerror ("contour2d(X, 1, Z, NZ , rect);", msgerr);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix or Scilab function expected.\n"), "contour2d", 3);
assert_checkerror ("contour2d(X, Y, ''Z'', NZ , rect);", msgerr);

msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "contour", 1);
assert_checkerror ("contour(''X'', Y, Z, NZ , rect);", msgerr);
msgerr = msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "contour", 1);
assert_checkerror ("contour(1, Y, Z, NZ , rect);", msgerr);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "contour", 2);
assert_checkerror ("contour(X, ''Y'', Z, NZ , rect);", msgerr);
msgerr = msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "contour", 2);
assert_checkerror ("contour(X, 1, Z, NZ , rect);", msgerr);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix or Scilab function expected.\n"), "contour", 3);
assert_checkerror ("contour(X, Y, ''Z'', NZ , rect);", msgerr);

msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "contourf", 1);
assert_checkerror ("contourf(''X'', Y, Z, NZ , rect);", msgerr);
msgerr = msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "contourf", 1);
assert_checkerror ("contourf(1, Y, Z, NZ , rect);", msgerr);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "contourf", 2);
assert_checkerror ("contourf(X, ''Y'', Z, NZ , rect);", msgerr);
msgerr = msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "contourf", 2);
assert_checkerror ("contourf(X, 1, Z, NZ , rect);", msgerr);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "contourf", 3);
assert_checkerror ("contourf(X, Y, ''Z'', NZ , rect);", msgerr);
