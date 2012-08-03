// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 7805 -->
//
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->

// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7805
//
// <-- Short Description -->
// clear called inside an exec while in pause may have unexpected results
 
//copy an paste the following instructions in the Scilab Console.

// pause level must be 0 (-->)
mputl("clear;", TMPDIR + "/bug7805.sce");
a = 1

pause
// pause level must be 1 (-1->)
b = 2
pause

// pause level must be 2 (-2->)
c = 3


exec(TMPDIR+"/bug7805.sce");

// clear executed in pause level 2 then only c is cleared
if exists("c") == 1 then pause,end

// check that 'a' and 'b' exist
if exists("b") == 0 then pause, end
if exists("a") == 0 then pause, end

resume
// pause level must be 1 and b was defined in pause level 2
if exists("b") == 1 then pause, end

resume
// pause level must be 0 only a exists
if exists("a") == 0 then pause, end

