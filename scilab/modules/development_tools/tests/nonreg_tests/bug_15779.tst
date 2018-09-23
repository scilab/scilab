// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- INTERACTIVE TEST -->
//   The display of durations of tests makes the REF not reproducible
//
// <-- Non-regression test for bug 15779 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15779
//
// <-- Short Description -->
// test_run(..,"*sin*") with a wildcarded pattern was not allowed

test_run elementary_functions *space
test_run elementary_functions sin*
test_run elementary_functions *sin*

// Compare the output to the following lists:

//test_run elementary_functions *space
//   TMPDIR = TMPDIR
//   001/002 - [elementary_functions] logspace....................passed
//   002/002 - [elementary_functions] linspace....................passed
//   --------------------------------------------------------------------------
//   Summary
//   tests              2 - 100 %
//   passed             2 - 100 %
//   failed             0 -   0 %
//   skipped            0
//   length             3.00 sec
//   --------------------------------------------------------------------------
// ans  =
//  T
//
//test_run elementary_functions sin*
//   TMPDIR = TMPDIR
//   001/004 - [elementary_functions] sinh........................passed
//   002/004 - [elementary_functions] sind........................passed
//   003/004 - [elementary_functions] sinc........................passed
//   004/004 - [elementary_functions] sin.........................passed
//   --------------------------------------------------------------------------
//   Summary
//   tests              4 - 100 %
//   passed             4 - 100 %
//   failed             0 -   0 %
//   skipped            0
//   length             8.49 sec
//   --------------------------------------------------------------------------
// ans  =
//  T
//
//test_run elementary_functions *sin*
//   TMPDIR = TMPDIR
//   001/007 - [elementary_functions] sinh........................passed
//   002/007 - [elementary_functions] sind........................passed
//   003/007 - [elementary_functions] sinc........................passed
//   004/007 - [elementary_functions] sin.........................passed
//   005/007 - [elementary_functions] asinh.......................passed
//   006/007 - [elementary_functions] asind.......................passed
//   007/007 - [elementary_functions] asin........................passed
//   --------------------------------------------------------------------------
//   Summary
//   tests              7 - 100 %
//   passed             7 - 100 %
//   failed             0 -   0 %
//   skipped            0
//   length             14.61 sec
//   --------------------------------------------------------------------------
// ans  =
//  T
