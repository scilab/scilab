// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 14435 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14435
//
// <-- Short Description -->
// Errors not well handled in overloaded functions

t = tlist(["user","x"],0);
m = mlist(["user","x"],0);

message = [msprintf(_("Function not defined for given argument type(s),\n")); msprintf(_("  check arguments or define function %s for overloading.\n"), "%l_e")];
assert_checkerror("t.z",message);
assert_checkerror("m.z",message);
assert_checkerror("t(""z"")",message);
assert_checkerror("m(""z"")",message);

function varargout = %user_e(i,x)
    if or(i==["a" "b"])
        varargout(1) = i + "1"
        varargout(2) = i + "2"
        return;
    end

    if i == "no_output" then
        return;
    end

    error(msprintf("Error: field %s is undefined !",i));
end

assert_checkequal(t.a, "a1");
assert_checkequal(t("a"), "a1");

message = "Error: field z is undefined !";
assert_checkerror("t.z",message);
assert_checkerror("m.z",message);
assert_checkerror("t(""z"")",message);
assert_checkerror("m(""z"")",message);

msg = msprintf(_("%s: Extraction must have at least one output.\n"), "%user_e");
assert_checkerror("t.no_output", msg);
assert_checkerror("t(""no_output"")", msg);