// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9838 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9838
//
// <-- Short Description -->
// evstr freezes on some specific string
//
assert_checkequal(evstr('//xxx'),[]);
t=["1";
   "2";
   "//xxx"];
assert_checkequal(evstr(t),[1;2]); 

nl=ascii(10);
t=["//comment";
   "function y=foo(),"
   " y=1;"
   "endfunction"];

t=strcat(t,nl); //make a single string that appears as a string array
assert_checkequal(evstr(t),[]); 
