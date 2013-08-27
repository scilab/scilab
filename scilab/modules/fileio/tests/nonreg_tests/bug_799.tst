// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 799 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=799
//
// <-- Short Description -->
//    Reload large matrix file from disk with "fscanfMat" command
//    crashes Scilab.

m = 10593;
n = 696;
a = rand(m,n,'u');

fprintfMat(TMPDIR+'/Mat',a,'%5.2f');
b=fscanfMat(TMPDIR+'/Mat');

if or( size(a) <> size(b) ) then pause, end
