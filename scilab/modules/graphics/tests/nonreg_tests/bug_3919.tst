// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// <-- Non-regression test for bug 3919 -->
//

//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3919
//
// <-- Short Description -->
// scilab x64 doesn't display graphics from a remote desktop

// <-- INTERACTIVE TEST -->
// <-- WINDOWS ONLY -->
// <-- 64 BITS ONLY -->

// install scilab x64 on windows
// configure your remote desktop and connect to your pc
// and launch scilab

if win64() <> %t then pause,end
if istssession() <> %t then pause,end

// try to do a plot or plot3d 
// if you have a display it works
