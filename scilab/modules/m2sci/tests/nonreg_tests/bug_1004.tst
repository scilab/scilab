// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004      - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 1004 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1004
//
// <-- Short Description -->
//    disp added during translation of Matlab for loop
//
//    Try to convert the following 3-line file ct2.m:
//
//    function ct2()
//    for i=1:5
//    end
//
//
//    The sci file created is:
//
//    function [] = ct2()
//
//    // Display warning for floating point exception
//    ieee(1)
//
//    for i = 1:5,
//      disp(i,"i  =");
//    end
//    endfunction
// ...

mode(-1);
clear;

MFILECONTENTS=["for i=1:5";"end"];

MFILE=TMPDIR+"/bug1004.m";
SCIFILE=TMPDIR+"/bug1004.sci";

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["";
"// Display mode";
"mode(0);";
"";
"// Display warning for floating point exception";
"ieee(1);";
"";
"for i = 1:5";
"";
"end;"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
