// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009 - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4468 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4468
//
// <-- Short Description -->
// If <path> represents the path of a directory and has a trailing separator, pathconvert(<path>,%F) should remove this trailing separator.

if pathconvert('/tmp/dir_1/',%f, %t, 'u') <> '/tmp/dir_1' then pause,end

if pathconvert('/tmp/dir_1/',%t, %t, 'u') <> '/tmp/dir_1/' then pause,end
