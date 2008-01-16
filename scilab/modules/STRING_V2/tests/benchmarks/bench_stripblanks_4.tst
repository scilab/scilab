//==============================================================================
// Benchmark for stripblanks function
// Copyright INRIA 2007
//==============================================================================

text = mgetl(SCI+"/modules/STRING_V2/tests/benchmarks/ascii_text.txt");

// <-- BENCH START -->
a = stripblanks(text,%T);
// <-- BENCH END -->
