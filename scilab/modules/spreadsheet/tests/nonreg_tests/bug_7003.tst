// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7003 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7003
//
// <-- Short Description -->
//
// read_csv function was not be able to convert "NaN" strings to "%nan" string.

N = eye(3,3);
ref = (N == 1);
N(N == 1) = %nan;

write_csv(N,TMPDIR + "/TEST_NAN.csv");
resSTR = read_csv(TMPDIR + "/TEST_NAN.csv");
resVal = evstr(resSTR);

if or(ref <> isnan(resVal)) then pause, end

N = eye(3,3);
ref = (N == 1);
N(N == 1) = %inf;

write_csv(N,TMPDIR + "/TEST_INF.csv");
resSTR = read_csv(TMPDIR + "/TEST_INF.csv");
resVal = evstr(resSTR);

if or(ref <> isinf(resVal)) then pause, end

res = read_csv(SCI + "/modules/spreadsheet/tests/nonreg_tests/bug_7003.csv");
MatA = evstr(res);
if or(size(MatA) <> [400 100]) then pause, end
if or(isnan(MatA(1:$, 1)) <> %t) then pause, end
