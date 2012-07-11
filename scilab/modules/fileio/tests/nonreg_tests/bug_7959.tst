// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7959 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7959
//
// <-- Short Description -->
// fscanf did not manage file descriptor as input argument
//

text =["blablabla"];
mputl(text, TMPDIR + "/test1.txt");
s1 = fscanf(TMPDIR + "/test1.txt", "%s");
if text <> s1 then pause, end

fd = mopen(TMPDIR + "/test1.txt", "rt");
s2 = fscanf(fd, "%s");
mclose(fd);
if text <> s2 then pause, end

if s1 <> s2 then pause, end
