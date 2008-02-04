//==============================================================================
// Benchmark for grep function
// Copyright INRIA 2007
//==============================================================================

text = mgetl(SCI+"/modules/string/tests/unit_tests/text.txt");

// <-- BENCH START -->
a = grep(text,"fonction");
// <-- BENCH END -->

