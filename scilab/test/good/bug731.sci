// Non-regression test file for bug 731
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Date : 3 mai 2005
// last modified : 10 Jan 2006

clear;
mode(-1);

// =============== Test 1 ===============

function y=foo(x)
    a=sin(x)
    function y=sq(x), y=x^2,endfunction
    y=sq(a)+1
endfunction

CONTENTS = fun2string(foo);

REFCONTENTS=["function y=ans(x)";
"  a = sin(x)";
"  function y=sq(x)";
"  , y=x^2,";
"  endfunction";
"  y = sq(a) + 1";
"endfunction"];

test1=~or(REFCONTENTS<>CONTENTS);

// =============== Test 2 ===============

function one()
    function two()
        // A comment
    endfunction
    function three()
        // A comment
    endfunction
    //a comment here
endfunction

CONTENTS_II = fun2string(one);

REFCONTENTS_II=["function []=ans";
"  function two()";
"      // A comment";
"  endfunction";
"  function three()";
"      // A comment";
"  endfunction";
"  //a comment here";
"endfunction"];

test2=~or(REFCONTENTS_II<>CONTENTS_II);

// =============== Test 3 ===============

function a=foo1()
    a = 1 ; function foo2() ; disp("hello") ; endfunction ; disp("zut");
    a = 2
endfunction

CONTENTS_III=fun2string(foo1);

REFCONTENTS_III=["function a=ans";
"  a = 1;function foo2() ";
"  ; disp(""hello"") ; ";
"  endfunction;disp(''zut'');";
"  a = 2";
"endfunction"];

test3=~or(REFCONTENTS_III<>CONTENTS_III);

// =============== Result ===============
affich_result(test1 & test2 & test3,731);

clear
