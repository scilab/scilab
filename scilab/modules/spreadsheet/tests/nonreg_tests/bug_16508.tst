// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 16508 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16508
//
// <-- Short Description -->
// csvTextScan and csvRead did not handle well complex data.

text = ["Inf" "1-i" "i" "Nan-i"];
expected = [%inf; 1-%i; %i; %nan-%i];
M = csvTextScan(text);
assert_checkequal(M, expected);

File = TMPDIR + "/bug_16508.txt";
mputl(text, File);
M = csvRead(File);
assert_checkequal(M, expected);

text =["i" "1*i" "0*i" "2i"];
expected = [%i; %i; 0; 2*%i];
M = csvTextScan(text);
assert_checkequal(M, expected);

text =["%i" "1*%i" "0*%i" "2%i"];
expected = [%i; %i; 0; 2*%i];
M = csvTextScan(text);
assert_checkequal(M, expected);

text = ["Infi" "1+Nani"];
csvTextScan(text)