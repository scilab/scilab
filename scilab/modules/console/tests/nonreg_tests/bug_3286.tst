// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3286 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3286
//
// <-- Short Description -->


// <-- INTERACTIVE TEST -->
// try : 
// ./bin/scilab -nw
//--> fa=gcf()
//--> fa.[TAB]
// it must return nothing (no files)
