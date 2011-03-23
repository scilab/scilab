// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 4874 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4874
//
// <-- Short Description -->
//    "query-replace" insert unwanted spaces in the middle of lines that have nothing to do with the searched pattern.


editor SCI/modules/atoms/macros/atomsInstall.sci
// <ctrl+F>
// Find : result 
// Replace : result10
// See the line before last







