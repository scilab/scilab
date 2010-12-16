// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM MANDATORY -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8424 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8424
//
// <-- Short Description -->
// [a, b] = dos("git 1>&2") returned a wrong error.
//
// git without  parameter returns same thing git --help.
// except that it is in stderr and not in stdout
// Here dos failed and returned [] and a error in api_scilab.
// This test is interactive because it is require git.

[a1, b1] = dos("git 1>&2");
if b1 <> %f then pause, end

[a2, b2] = dos("git --help");
if b2 <> %t then pause, end

if ~or(a1 == a2) then pause, end
