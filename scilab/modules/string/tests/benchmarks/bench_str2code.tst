//==============================================================================
// Benchmark for str2code function
// Copyright INRIA 2007
//==============================================================================

text     = mgetl(SCI+"/modules/string/tests/benchmarks/ascii_text.txt");
text_cat = strcat(text);

// <-- BENCH START -->
a = str2code(text_cat);
// <-- BENCH END -->

