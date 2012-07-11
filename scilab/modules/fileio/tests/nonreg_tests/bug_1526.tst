// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1526 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1526
//
// <-- Short Description -->
//   An ascii file (created 'by hand' or a fprintfMat primitive with Scilab/Windows)
//   using windows encoding is read badly on Scilab/Linux with the fscanfMat primitive.
//   This problem seems to come from the bad read of end-line characters.
//   A work-around of this bug, is to use 'dos2unix' on Linux to convert the file.

mat_ref = [ ..
    0.15    0.19    0.96 ;
    0.55    0.81    0.57 ;
    0.28    0.33    0.43 ];

mat = fscanfMat("SCI/modules/fileio/tests/nonreg_tests/bug_1526.mat")

if or( mat <> mat_ref ) then pause, end
