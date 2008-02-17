// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for ascii function7
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

