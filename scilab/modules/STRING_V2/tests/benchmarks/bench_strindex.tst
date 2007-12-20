//==============================================================================
// Benchmark for strindex function
// Copyright INRIA 2007
//==============================================================================

text     = mgetl(SCI+"/modules/STRING_V2/tests/unit_tests/text.txt");
text_cat = strcat(text);

// <-- BENCH START -->
a = strindex(text_cat,"fonction");
// <-- BENCH END -->

