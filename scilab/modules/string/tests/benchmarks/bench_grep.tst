// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for grep function
//==============================================================================

text = mgetl(SCI+"/modules/string/tests/unit_tests/text.txt");

// <-- BENCH START -->
a = grep(text,"fonction");
// <-- BENCH END -->

