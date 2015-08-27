//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 5367 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5367
//
// <-- Short Description -->
// Incorrect %nan multiplication returned with vectors on 64-bit Vista using Scilab 5.1.1.
// bug with MKL 10.1


R1 = [1 2] * %nan;
if ~and(isnan(R1)) then pause,end

R2 = %nan * [1 2];
if ~and(isnan(R2)) then pause,end
