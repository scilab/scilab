// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8912 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8912
//
// <-- Short Description -->
// fullpath did not return same result on Windows and Linux.
//

if fullpath(TMPDIR + "/a/b/c/d/e/f") <> pathconvert(TMPDIR + "/a/b/c/d/e/f", %f) then pause, end
if fullpath(TMPDIR + "/a/b/c/d/e/f/") <> pathconvert(TMPDIR + "/a/b/c/d/e/f/") then pause, end

p = fullpath(TMPDIR + filesep());
if part(p, length(p)) <> filesep() then pause, end
