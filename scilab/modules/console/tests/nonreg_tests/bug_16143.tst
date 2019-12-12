// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Tan C.L. ByteCode
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 16143 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16143
//
// <-- INTERACTIVE TEST -->
// clc(n) cleared n+1 lines instead of n>0

// Test 1
clc;
mode(0);
mode(0);
mode(0);
clc(1);
// should leave
// --> mode(0);
// --> mode(0);
// -->

// Test 2
clc;
mode(2);
mode(2);
mode(2);
clc(1);
// should leave
// --> mode(2);
//
// --> mode(2);
//
// --> mode(2);
// -->

// Test 3
clc;
for i = 1:10, mprintf("%d\n\n\n",i), sleep(100), clc(1), end;
// should leave
// --> for i = 1:10, mprintf("%d\n\n\n",i), sleep(100), clc(1), end;
// 10
// -->
