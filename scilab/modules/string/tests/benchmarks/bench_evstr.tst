// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON <sgougeon@free.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Benchmark test for evstr function
m = rand(5000, 5);
s = string(m);

// <-- BENCH NB RUN : 10 -->
// <-- BENCH START -->
e = evstr(s);
// <-- BENCH END -->
