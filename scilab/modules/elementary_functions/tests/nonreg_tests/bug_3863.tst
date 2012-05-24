// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3863 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3863
//
// <-- Short Description -->
// expm broken on Windows (32 and 64 bits) with some values.

r = expm([1/30, 0.1;0,0]);
if or(isnan(r)) then pause,end
if size(r,'*') <> 4 then pause,end

