// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7249 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7249
//
// <-- Short Description -->
// 'string' function did not work with a macro as input argument.
//
// =============================================================================
function myFunc
    disp("myFunc called");
endfunction

ref_txt = [" "; ..
"disp(""myFunc called"");"; ..
" "];

[in, out, txt] = string(myFunc);
assert_checkequal(out, []);
assert_checkequal(in, []);
assert_checkequal(txt, ref_txt);
// =============================================================================
deff("myFund()", "disp(""myFunc called"");");
[in2, out2, txt2] = string(myFund);
assert_checkequal(out2, out);
assert_checkequal(in2, in);
assert_checkequal(txt2, ref_txt);
// =============================================================================
function [x, y, z] = myFunc1(a,b,c,d)
    x = [];
    y = [];
    z = [];
    function k = myFunc2(x)
        k = x + 1;
    endfunction
    y = myFunc2(a);
endfunction

ref_text = [" "; ..
"x = [];"; ..
"y = [];"; ..
"z = [];"; ..
"function k = myFunc2(x)"; ..
"    k = x + 1;"; ..
"endfunction"; ..
"y = myFunc2(a);"; ..
" "];

[in, out, txt] = string(myFunc1);
assert_checkequal(out, ["a","b","c","d"]);
assert_checkequal(in, ["x","y","z"]);
assert_checkequal(txt, ref_text);
// =============================================================================
deff("y = mymacro(x)", "y = x + 1");
[out, in, text] = string(mymacro);
assert_checkequal(out, "y");
assert_checkequal(in, "x");
assert_checkequal(text, [" "; "y = x + 1"; " "]);
// =============================================================================
[out, in, text] = string(sinc);
assert_checkequal(out, "y");
assert_checkequal(in, ["x", "fl"]);
assert_checkequal(text <> [], %t);
// =============================================================================
function y=foo(x);y=x+1;endfunction
[out, in, text] = string(foo);
assert_checkequal(out, "y");
assert_checkequal(in, "x");
assert_checkequal(text, [" "; "y = x + 1;"; " "]);
// =============================================================================
deff("[y,z] = mymacroc(x,i)", "y = x + 1;z = y;");
[out, in, text] = string(mymacroc);
assert_checkequal(out, ["y", "z"]);
assert_checkequal(in, ["x", "i"]);
assert_checkequal(text, [" "; "y = x + 1;z = y;"; " "]);
// =============================================================================

