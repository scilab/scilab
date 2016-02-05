// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 8786 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8786
//
// <-- Short Description -->
// When entering wrong input arguments in 'getcolor' function, the error message has no sense.

ierr = execstr("getcolor(10)", "errcatch");
errmsg = lasterror();

if ierr==0 then pause; end
if errmsg<>msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "getcolor", 1) then pause; end