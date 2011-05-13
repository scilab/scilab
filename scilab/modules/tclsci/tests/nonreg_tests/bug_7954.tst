// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7954 -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7954
//
// <-- Short Description -->
//
// i=0;
// while [ true ]; do
//    ./bin/scilab  -e "TCL_EvalStr \"set a 3\"; exit()" -nw;
//    i=$(($i + 1));
//    echo $i;
// done
// # Bug present => it should freeze at some point
