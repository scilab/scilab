// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 731 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=731
//
// <-- Short Description -->
//    fun2string and macr2tree cannot cope with nested function
//    definitions

// =============== Test 1 ===============

function y=foo(x)
	a=sin(x)
	function y=sq(x), y=x^2,endfunction
	y=sq(a)+1
endfunction

CONTENTS = fun2string(foo);

REFCONTENTS=["function y=ans(x)";
		"  a = sin(x)";
		"  function y=sq(x), y=x^2,endfunction";
		"  y = sq(a) + 1";
		"endfunction"];

if or(REFCONTENTS<>CONTENTS) then pause,end

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

REFCONTENTS_II=["function []=ans()";
		"  function two()";
		"      // A comment";
		"  endfunction";
		"  function three()";
		"      // A comment";
		"  endfunction";
		"  //a comment here";
		"endfunction"];

if or(REFCONTENTS_II<>CONTENTS_II) then pause,end

// =============== Test 3 ===============

function a=foo1()
	a = 1 ; function foo2() ; disp("hello") ; endfunction ; disp("zut");
	a = 2
endfunction

CONTENTS_III=fun2string(foo1);

REFCONTENTS_III=["function a=ans()";
		 "  a = 1;function foo2() , disp(""hello"") ;endfunction;disp(''zut'');";
		 "  a = 2";
		 "endfunction"];

if or(REFCONTENTS_III<>CONTENTS_III) then pause,end
