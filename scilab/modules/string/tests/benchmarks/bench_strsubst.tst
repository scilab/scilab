//==============================================================================
// Benchmark for strsubst function
// Copyright INRIA 2007
//==============================================================================

text     = mgetl(SCI+"/modules/STRING_V2/tests/unit_tests/text.txt");

// <-- BENCH START -->
a = strsubst(text,"fonction","function");
// <-- BENCH END -->

