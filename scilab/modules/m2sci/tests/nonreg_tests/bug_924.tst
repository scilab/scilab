// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 924 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=924
//
// <-- Short Description -->
//    Bad handling of variable display in functions.
//    Following Matlab code :
//    function test
//    a='milou'
//    a
//    should be converted to Scilab code :
//    function test
//    a='milou'
//    disp(a)
//    but it is not....

MFILECONTENTS=["function bug924";
"a=''milou'';";
"a=''milou'',";
"a=''milou''";
"a;";
"a,";
"a";
"[c,d]=svd(1);";
"[c,d]=svd(1),";
"[c,d]=svd(1)";
"b=a;";
"b=a,";
"b=a"];

MFILE=TMPDIR+"/bug924.m";
SCIFILE=TMPDIR+"/bug924.sci";

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["function [] = bug924()";
"";
"// Display mode";
"mode(0);";
"";
"// Display warning for floating point exception";
"ieee(1);";
"";
"a = ""milou"";";
"a = ""milou""";
"a = ""milou""";
"a;";
"a";
"a";
"[c,d] = svd(1);";
"[c,d] = svd(1)";
"[c,d] = svd(1)";
"b = a;";
"b = a";
"b = a";
"endfunction"];

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
