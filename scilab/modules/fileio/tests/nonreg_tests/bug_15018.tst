// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Nimish Kapoor
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15018 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15018
//
// <-- Short Description -->
// '\n' character did not work in mfprintf

fich_optim = mopen(TMPDIR+"/suivi_optim.txt","at");
mfprintf(fich_optim,"\n--- Résultats ---\n\nRMS = \n\nComposants :\t");
mclose(fich_optim);

fd = mopen(TMPDIR+"/suivi_optim.txt","r");
line = mgetl(fd, 1);
assert_checkequal(line, "");
line = mgetl(fd, 1);
assert_checkequal(line, "--- Résultats ---");
mclose(fd);
