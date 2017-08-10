//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//where
deff("[a,b]=foo()","[a,b]=where()")
[a,b]=foo();
if a(1)<>2|b(1)<>"foo" then pause,end

function [line, fun]=fun1()
    4
    5
    6
    7
    [line, fun] = where();
endfunction
function [line, fun]=fun2()
    k=4
    [line, fun] = fun1();
endfunction
function [line, fun] = fun3()
    a=2;
    b=3;
    [line, fun] = fun2();
    c=5;
endfunction
[line, fun] = fun3();

// the last line represent the exec of this file.
// the line number is not the same when the test is execute
// with exec or test_run.
assert_checkequal(line(1:3), [6;3;4]);
assert_checkequal(fun(1:3), ["fun1";"fun2";"fun3"]);
