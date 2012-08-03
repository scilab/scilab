// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6452 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6452
//
// <-- Short Description -->
// part() does not support properly ANSI-encoded chars
// mgetl problem in fact

fid = mopen("SCI/modules/fileio/tests/nonreg_tests/bug_6449_ANSI.txt", "rt");
lines_result = mgetl(fid);
mclose(fid);

r = part(lines_result(2),1:10);
ref = "Caractéres";
if r <> ref then pause, end

fid = mopen("SCI/modules/fileio/tests/nonreg_tests/bug_6449_UTF.txt", "rt");
lines_result = mgetl(fid);
mclose(fid);

r = part(lines_result(2),1:10);
ref = "Caractéres";
if r <> ref then pause, end

r = part(lines_result(1),1:4);
ref = "Just";
if r <> ref then pause, end

r = part(lines_result(3),1:2);
ref = "ùà";
if r <> ref then pause, end

r = part(lines_result(4),1:12);
ref = "&é""''(-è_çà)=";
if r <> ref then pause, end

r = part(lines_result(9),2:4);
ref = "ロー・";
if r <> ref then pause, end 