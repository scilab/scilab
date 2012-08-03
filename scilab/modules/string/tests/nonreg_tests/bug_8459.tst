// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8459 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8459
//
// <-- Short Description -->
// strsplit failed when second input argument was a "/"
//

ref = [""; "foo"; "bla"];

ierr = execstr("r1 = strsplit(""/foo/bla"", [""/"" ""\"" "":""], 256);", "errcatch");
if ierr <> 0 then pause, end
if ~or(r1 == ref) then pause, end

ierr = execstr("r2 = strsplit(""|foo|bla"", [""|""]);", "errcatch");
if ierr <> 0 then pause, end
if ~or(r2 == ref) then pause, end
