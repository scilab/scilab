// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3014 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3014
//
// <-- Short Description -->
//    gsort may make scilab crash in particlaur circumstance

H=rand(10,10);
evH = spec(H);
//[OUT1,OUT2] = gsort([],'g'); no crash
F='g';[OUT1,OUT2] = gsort([],F); //crash here
