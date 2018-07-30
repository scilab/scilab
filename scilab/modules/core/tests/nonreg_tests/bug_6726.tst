// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2009 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6726 -->
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/6726
//
// <-- Short Description -->
//Variables whose name starts by a $  cannot be retrieved in libraries

//create a library of variables
Dir = TMPDIR + "/bug_6726/";
mkdir(Dir);

mputl("function y=$a(x), y=x+1, endfunction", Dir+"$a.sci");
genlib("testlib", Dir);
testlib = lib(Dir);

clear $a
assert_checkequal($a(2), 3);
