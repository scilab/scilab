// Non-regression test file for bug 670
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 10 janvier 2006

mode(-1);
clear
deff("a = foo()", ...
"b = tlist([''b''; ''c''], 0); " + ...
"a = tlist([''a''; ''b''], b); " + ...
"a.b.c = getText() + ''text 2'' ");
t=fun2string(foo);
t_ref=["function a=ans";
"  b = tlist([''b'';''c''], 0);a = tlist([''a'';''b''], b);a.b.c = getText() + ''text 2''";
"endfunction"];
ok=and(t==t_ref);
affich_result(ok,670);
