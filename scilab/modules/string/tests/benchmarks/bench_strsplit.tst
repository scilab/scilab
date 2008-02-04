//==============================================================================
// Benchmark for strsplit function
// Copyright INRIA 2007
//==============================================================================

text     = mgetl(SCI+"/modules/string/tests/benchmarks/ascii_text.txt");
text_cat = strcat(text);

// <-- BENCH START -->
a = strsplit(text_cat,[1000 2000 3000 4000 5000 6000 7000]);
// <-- BENCH END -->

