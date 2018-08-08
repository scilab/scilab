// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI-Group - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15485 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15485
//
// <-- Short Description -->
// savematfile crashes Scilab

load('SCI/modules/matio/tests/nonreg_tests/bug_15485.dat');
model = model2;
savematfile(TMPDIR+"/model2.mat", "model2", "-v6");
clear model2;
loadmatfile(TMPDIR+"/model2.mat");

assert_checkequal(model, model2);
