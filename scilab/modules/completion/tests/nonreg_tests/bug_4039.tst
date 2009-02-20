// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4039 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4039
//
// <-- Short Description -->
// Problem of completion with version 5.1:
// (With NW mode).

// <-- INTERACTIVE TEST -->
                    
wavread("/ho[TAB]
wavread("/home/
wavread("/home/sy[TAB]

// it should be wavread("/home/sylvestre
// if sylvestre home exists