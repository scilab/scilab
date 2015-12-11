// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2072 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2072
//
// <-- Short Description -->
//   incorrect clause uncorrectly handled under errcatch

// <-- CLI SHELL MODE -->

mkdir(TMPDIR,"b2072");

txt=["function test()"
"if 1 ~=1 then"
"   a=3;"
"end"
"else"
"   a=7;"
"end"
"endfunction"];
mputl(txt,TMPDIR+"/b2072/test.sci");
txt=["function testOK()"
"a=3;"
"   end"
"endfunction   "];
mputl(txt,TMPDIR+"/b2072/testOK.sci");

if execstr("exec(TMPDIR+''/b2072/test.sci'');","errcatch")==0 then pause,end
if execstr("exec(TMPDIR+''/b2072/testOK.sci'');","errcatch")==0 then pause,end

