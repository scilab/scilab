// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9659 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9659
//
// <-- Short Description -->
// "error(246)" returned a bad message for overloading.

// assert_checkerror does not manage multiline error, see bug 9572
if execstr("sin(""z"")","errcatch") == 0 then pause, end

msgerror = gettext("Function not defined for given argument type(s),\n");
msgerror = strsubst(msgerror, "\n", "");
msgerror = [msgerror; msprintf(gettext("  check arguments or define function %s for overloading.\n"), "%c_sin")];
msg = lasterror();
if ~and(msg == msgerror) then pause, end

if execstr("sin(int8(12))","errcatch") == 0 then pause, end
msgerror = gettext("Function not defined for given argument type(s),\n");
msgerror = strsubst(msgerror, "\n", "");
msgerror = [msgerror; msprintf(gettext("  check arguments or define function %s for overloading.\n"), "%i_sin")];
msg = lasterror();
if ~and(msg == msgerror) then pause, end

