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

if or(REFCONTENTS<>CONTENTS) then
	test1=%F
else
	test1=%T
end

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
		"    // a comment";
		"  endfunction";
		"  function three()";
		"    // a comment";
		"  endfunction";
		"  //a comment here";
		"endfunction"];

if or(REFCONTENTS_II<>CONTENTS_II) then
	test2=%F
else
	test2=%T
end

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

if or(REFCONTENTS_III<>CONTENTS_III) then
	test3=%F
else
	test3=%T
end

// =============== Result =============== 

if test1 & test2 & test3 then
	affich_result(%T,731);
else
	affich_result(%F,731);
end

clear
