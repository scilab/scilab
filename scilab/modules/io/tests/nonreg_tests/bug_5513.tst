// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 5513 -->
// <-- INTERACTIVE TEST -->

// <-- Short Description -->
// input("message") did not restore the original prompt after the input job is done.
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5513
// 

a = rand(2,2)

// type a string example:  This is a test
// check that you have good prompt

b = input("Give a string : ","string")
