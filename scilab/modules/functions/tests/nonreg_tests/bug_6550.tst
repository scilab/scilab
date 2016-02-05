// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 6550 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6550
//
// <-- Short Description -->
// lib(".") used relative path in library variable created
// <-- CLI SHELL MODE -->
// =============================================================================
f = [
"function bug_6550()"
"  1+1;"
"endfunction"
""
];
cd TMPDIR;
mkdir("bug_6550");
cd("bug_6550");
mputl(f,"bug_6550.sci");
if genlib("bug_6550lib",TMPDIR + "/bug_6550") <> %t then pause,end;
// =============================================================================
AAlib = lib(".");
[f, p] = libraryinfo("AAlib");
if pathconvert(p, %f, %f) <> pathconvert(pwd(), %f, %f) then pause,end
// =============================================================================
BBlib = lib(pwd() + "/../bug_6550/../bug_6550");
[f, p] = libraryinfo("BBlib");
if pathconvert(p, %f, %f) <> pathconvert(pwd(), %f, %f) then pause,end
// =============================================================================
