// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 5195 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5195
//
// <-- Short Description -->
// "get" crashs with "Lycée" toolboxe

// install and load "Lycée" toolboxe 
// click on menu "ajustement affine"
// you should not have a error message as
//  Scilab has found a critical error (EXCEPTION_ACCESS_VIOLATION)
// with "get" function.

