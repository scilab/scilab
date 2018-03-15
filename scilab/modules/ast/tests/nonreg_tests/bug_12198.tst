// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 12198 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12198
//
// <-- Short Description -->
// control keywords could be overloaded as regular variables.
// In Scilab 6, they are no longer overloadable and are protected

stillUnprotected = ["abort" "exit" "pause" "quit"];
//--> select=1     // same scheme with try=1 and while=1
//  > end
//  > end
//select=1
//      ^^
//Error: syntax error, unexpected =
//
keyws = ["break" "case" "catch" "continue" "do" "else" "elseif" "end" ..
        "endfunction" "for" "function" "if" "resume" ..
        "return" "select" "then" "try" "while"];

for k = keyws
    assert_checktrue(execstr(k+"=1","errcatch")~=0);
end

old = funcprot(0);
assert_checktrue(execstr("clear=1","errcatch")~=0);
assert_checktrue(execstr("clear clear","errcatch")~=0);
funcprot(old);
