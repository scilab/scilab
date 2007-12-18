//==============================================================================
// Benchmark for strindex function
// Copyright INRIA 2007
//==============================================================================

text     = mgetl(SCI+"/modules/string/tests/unit_tests/text.txt");
text_cat = strcat(text);

// <-- BENCH START -->
a = strindex(text_cat,"fonction");
// <-- BENCH END -->

