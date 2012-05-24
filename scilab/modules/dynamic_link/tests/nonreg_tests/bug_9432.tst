// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 9432 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9432
//
// <-- Short Description -->
// link a 64 bit dynamic library on a 32 bit platform returned a wrong error
// message on Windows.

// install a Scilab 64 bit and a Scilab 32 bit
// compile a dll with 64 bit version
// and try to load with 32 bit version

// same thing
// compile a dll with 32 bit version
// and try to load with 64 bit version
