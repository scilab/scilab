//==============================================================================
// Benchmark for ascii function
// Copyright INRIA 2007
//==============================================================================

code_test_mat = int( 128 * rand(100,100) );

// <-- BENCH START -->
a = ascii(code_test_mat);
// <-- BENCH END -->

