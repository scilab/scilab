// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for ascii function
//==============================================================================

exec(SCI+"/modules/string/tests/benchmarks/ascii_code.txt",-1);

// <-- BENCH START -->
a = ascii(ref);
// <-- BENCH END -->

