// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Tan C.L. ByteCode
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14642 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14642
//
// <-- INTERACTIVE TEST -->
// no more "\r" carriage return with print -> this patch fixes this problem
// partially. The case where the "\r" located in the same line with the text
// inside a loop still unhandled for STD console.


// Test 1
clc;
for i = 1:10, mprintf("%d",i), sleep(100), mprintf("\r"), end;
// should leave
// --> for i = 1:10, mprintf("%d",i), sleep(100), mprintf("\r"), end;
// 10
// -->

// Test 2
clc;
mprintf("tititatatutu,\rtoto");
// should leave
// --> mprintf("tititatatutu,\rtoto");
// tototatatutu,


