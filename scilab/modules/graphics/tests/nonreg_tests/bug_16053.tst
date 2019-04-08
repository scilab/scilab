// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 16053 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16053
//
// <-- Short Description -->
// plot() no longer supported standard abbreviated color names

for c = ["r" "g" "GRE" "b" "c" "m" "ma" "y" "Yel" "w" "k"]
	plot(1:10, "color",c)
end
