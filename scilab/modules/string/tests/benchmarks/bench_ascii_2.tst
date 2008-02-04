//==============================================================================
// Benchmark for ascii function
// Copyright INRIA 2007
//==============================================================================

// <-- BENCH NB RUN : 1000000 -->

str_test_mat =  ["abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz"; ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz", ..
				 "abcdefghijklmnopqrstuvxyz","abcdefghijklmnopqrstuvxyz"];
				 
// <-- BENCH START -->
a = ascii(str_test_mat);
// <-- BENCH END -->

