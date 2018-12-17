// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
//
// <-- Non-regression test for bug 15310 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15310
//
// <-- Short Description -->
// unlike isdef(varargin(k),"l"), isdef(argin,"l") now always returns %t even for unpassed arguments (REGRESSION)

function out=f(_x,_y)
  out=isdef(['_x','_y'],'local')
endfunction

function out=g(_x,_y)
  out=exists(['_x','_y'],'local')
endfunction

assert_checkequal(f(,),  [%f,%f])
assert_checkequal(f(,2), [%f,%t])
assert_checkequal(f(1,), [%t,%f])

assert_checkequal(g(,),  [0,0])
assert_checkequal(g(,2), [0,1])
assert_checkequal(g(1,), [1,0])
