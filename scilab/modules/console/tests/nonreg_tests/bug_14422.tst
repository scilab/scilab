// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Tan C.L. ByteCode
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14422 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14422
//
// <-- INTERACTIVE TEST -->
// after clc(); mprintf("abcd"); prints " abcd" with a leading blank.
// clc(0) now just clearing its own line, and redisplay the prompt --> on the same line.

// Test 1
clc(); mprintf("abcd");
// abcd
// -->
//
// but not
//  abcd
// -->
//

// Test 2
clc(0)
// should clear its own line and redisplay the prompt at the begining of the same line



