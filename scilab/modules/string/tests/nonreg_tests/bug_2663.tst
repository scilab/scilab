// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre .marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2663 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2663
//
// <-- Short Description -->
// length() change the value of the input argument. the input argument is replaced
// by the nearest rounded down integer.
//

y         = [0.3616361 2.2922267  1.5015342];
y_orig    = y;
yl_length = length(y);

if or(y <> y_orig) then pause, end
