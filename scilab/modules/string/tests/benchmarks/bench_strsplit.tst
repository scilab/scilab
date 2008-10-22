// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for strsplit function
//==============================================================================

text     = mgetl(SCI+"/modules/string/tests/benchmarks/ascii_text.txt");
text_cat = strcat(text);

// <-- BENCH START -->
a = strsplit(text_cat,[1000 2000 3000 4000 5000 6000 7000]);
// <-- BENCH END -->

