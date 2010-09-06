// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 7740 -->
//
// <-- JVM MANDATORY -->
// 
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7740
//
// <-- Short Description -->
// edit() a Scilab macro redefined failed

// executes with script

fun1 = ["function r = toto()";
        "r = ""NOT OK"";";
        "endfunction"];

fun2 = ["function r = toto()";
        "r = ""OK"";";
        "endfunction"];

mkdir(TMPDIR,"fun1lib");
mkdir(TMPDIR,"fun2lib");

mputl(fun1, TMPDIR + filesep() + "fun1lib" + filesep() + "toto.sci");
mputl(fun2, TMPDIR + filesep() + "fun2lib" + filesep() + "toto.sci");
genlib("fun1lib", TMPDIR + filesep() + "fun1lib", %t, %f);
genlib("fun2lib", TMPDIR + filesep() + "fun2lib", %t, %f);

fun1lib = lib(TMPDIR + filesep() + "fun1lib");
fun2lib = lib(TMPDIR + filesep() + "fun2lib");

edit toto

// please check that toto definition is fun2 with r = "OK" and not fun1
