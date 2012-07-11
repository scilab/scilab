// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3747 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3747
//
// Short description:
// string(eye()) tries to quit scilab

ierr = execstr("string(eye)","errcatch");
if ierr <> 999 then pause,end

ierr = execstr("string()","errcatch");
if ierr <> 77 then pause,end

ierr = execstr("string(eye())","errcatch");
if ierr <> 0 then pause,end

if typeof(string(eye())) <> typeof('') then pause,end

ierr = execstr("string(eye())+""b""","errcatch");
if ierr <> 0 then pause,end

r = string(eye())+"b";
if r <> ""+"b" then pause,end

r = string(eye(3,3));
ref = ["1" "0" "0";"0" "1" "0";"0" "0" "1"];
if r <> ref then pause,end

r = size(eye());
if or(r <> [-1 -1]) then pause,end

r = size(string(eye()));
if or(r <> [1 1]) then pause,end

