// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 16178 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16178
//
// <-- Short Description -->
// sci2exp() yielded an error for any input macro without argout
// -------------------------------------------------------------

function Q1()
    a.b
endfunction
assert_checkequal(sci2exp(Q1), "createfun([""%fun()"";""a.b""])");
assert_checkequal(sci2exp(Q1,"Q"), "createfun([""Q()"";""a.b""])");


// The used internal createfun() failed for a function returning nothing:
// ---------------------------------------------------------------------
//--> f=createfun(["b=fun(a)";"b=a.b"])
// f  =
// [b]=f(a)     // OK
//
//--> f=createfun(["fun(a)";"b=a.b"])
// at line     1 of executed string
// at line    10 of function        deff      ( SCI\modules\functions\macros\deff.sci line 10 )
// at line    15 of function        createfun ( SCI\modules\functions\macros\createfun.sci line 27 )
//
// function f%fun(a)
//           ^~~~^
// Error: syntax error, unexpected identifier, expecting end of line or "," or ; or line comment

// NR Test:

%fghw = createfun(["fun(a)";"b=a.b"]);    // without output
assert_checkequal(type(%fghw), 13)

%fghw2 = createfun(["b=fun(a)";"b=a.b"]); // with output
assert_checkequal(type(%fghw2), 13)
s.b = %pi;
assert_checkequal(%fghw2(s), %pi)
