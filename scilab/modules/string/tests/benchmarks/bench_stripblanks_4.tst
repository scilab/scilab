//==============================================================================
// Benchmark for stripblanks function
// Copyright INRIA 2007
//==============================================================================

text = mgetl(SCI+"/modules/string/tests/benchmarks/ascii_text.txt");

// <-- BENCH START -->
a = stripblanks(text,%T);
// <-- BENCH END -->
