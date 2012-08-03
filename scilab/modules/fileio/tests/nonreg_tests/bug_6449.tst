// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 6449 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6449
//
// <-- Short Description -->
// regexp() does not support ANSI-encoded input strings 
// mgetl problem in fact

fid = mopen("SCI/modules/fileio/tests/nonreg_tests/bug_6449_ANSI.txt", "rt");
lines_result = mgetl(fid);
mclose(fid);
r = regexp(lines_result(2), "/src=.*?[\s|\>]/m");
if r <> [] then pause, end
r2 = regexp(lines_result(2), "/é/");
ref = [7 19 27];
if ~and(r2 == ref) then pause, end
fid = mopen("SCI/modules/fileio/tests/nonreg_tests/bug_6449_UTF.txt", "rt");
lines_result = mgetl(fid);
mclose(fid);
r = regexp(lines_result(2), "/src=.*?[\s|\>]/m");
if r <> [] then pause, end
r2 = regexp(lines_result(2), "/é/");
ref = [7 19 27];
if ~and(r2 == ref) then pause, end

