// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009 - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4499 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4499
//
// <-- Short Description -->
// basename() strips directory and suffix from filenames
// If a file or a directory starts with a dot, basename should not strip the suffix.

files = basename('SCI/modules/fileio/macros/.myfile');
if files <> '.myfile' then pause,end
