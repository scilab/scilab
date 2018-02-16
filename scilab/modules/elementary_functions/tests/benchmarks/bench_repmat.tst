// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for repmat() function
//==============================================================================

// <-- BENCH NB RUN : 10 -->

r = rand(1,10000);

// <-- BENCH START -->

repmat(r, 100,60);

// <-- BENCH END -->

clear ans
