// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE --> 
//
// <-- Non-regression test for bug 8046 -->
//
// <-- Short Description -->
// msprintf did not manage tabulation as format
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8046
//

s = "a" + ascii(9) + "b";

[nbOuts, k, l] = msscanf(s, "%s\t%s");
if nbOuts <> 2 then pause, end
if k <> "a" then pause, end
if l <> "b" then pause, end

if s <> sprintf("%s\t%s", "a", "b") then pause, end

mputl(s, TMPDIR + "/bug_8046.txt");
fr = mopen(TMPDIR + "/bug_8046.txt", "rt");
[nbOuts, k, l] = mfscanf(fr, "%s\t%s");
mclose(fr);
if nbOuts <> 2 then pause, end
if k <> "a" then pause, end
if l <> "b" then pause, end
