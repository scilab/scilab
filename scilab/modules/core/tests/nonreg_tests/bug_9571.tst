// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9571 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9571
//
// <-- Short Description -->
// "error(144)" returned a wrong variable name.

msgerror = msprintf(gettext("Undefined variable: %s\n"), "%p");
assert_checkerror ( "%p" , msgerror );

if with_module("umfpack") then
    // assert_checkerror does not manage multiline error, see bug 9572

    msgerror = gettext("Undefined operation for the given operands.\n");
    msgerror = strsubst(msgerror, "\n", "");
    msgerror = [msgerror; msprintf(gettext("check or define function %s for overloading.\n"), "%ptr_f_ptr")];

    A = sparse( [ 2  3  0  0  0]);
    Lup = umf_lufact(A);
    if execstr("B = [Lup; Lup];", "errcatch") <> 999 then pause, end
    msg = lasterror();

    if ~and(msg == msgerror) then pause, end
end


ierr = execstr("error(""%p"")", "errcatch");
assert_checkequal(lasterror(), "%p");
assert_checkequal(ierr, 10000);

ierr = execstr("error(""%p"")", "errcatch");
assert_checkequal(lasterror(), "%p");
assert_checkequal(ierr, 10000);

ierr = execstr("error(987, ""%p"")", "errcatch");
assert_checkequal(lasterror(), "%p");
assert_checkequal(ierr, 987);

ierr = execstr("error(""%p"", 986)", "errcatch");
assert_checkequal(lasterror(), "%p");
assert_checkequal(ierr, 986);
