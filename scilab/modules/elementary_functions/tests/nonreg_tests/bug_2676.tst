// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2676 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2676
//
// <-- Short Description -->
// oct2dec gives wrong results when first argument is a matrix.

if oct2dec(["10";"20";"4000"]) <> [ 8 ; 16 ; 2048 ] then pause,end
