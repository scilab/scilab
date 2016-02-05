// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->
// <-- WINDOWS ONLY -->

// <-- Non-regression test for bug 3647 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3769
//
// <-- Short Description -->
// Dynamic link doesn't detect intel fortran compiler version 11 on Windows

// Please be sure that you have installed Intel fortran 11
// launch scilab

r = findmsifortcompiler();
if r == "unknown" then pause,end

// And checks that others tests about dynamic link work
