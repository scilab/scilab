// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for justify function
//==============================================================================

in = ["1234","x","adfdfgdfghfgj" ;"1","354556","dgf";"sdfgd","","sdfsf"];

// <-- BENCH START -->
out = justify(in,'c');
// <-- BENCH END -->

