// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 7109 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7109
//
// <-- Short Description -->
// 'lu' function with some complex matrices crashed on Windows.

x = [0.1523 + 0.7685 * %i, -0.8957 + 0.0342 * %i, 2.4353 + 2.7736 * %i; ..
    -1.1953 + 1.2187 * %i, 1.2118 + 0.2571 * %i, -0.0446 - 1.7768 * %i; ..
     0.8038 + 1.3668 * %i, 1.2950 - 1.6929 * %i, 1.6267 + 0.2844 * %i];
     
ierr = execstr('[l, u] = lu(x);','errcatch');
if ierr <> 0 then pause, end

if or(size(l) <> [3 3]) then pause, end
if or(size(u) <> [3 3]) then pause, end

