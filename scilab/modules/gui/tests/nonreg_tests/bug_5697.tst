// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 5697 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5697
//
// <-- Short Description -->
// In Help Browser, external hyperlinks were opened in it rather
// than in Web Browser.

help fftw;

// Click on the link http://www.fftw.org, that should open it in the default
// Web Browser.
// Click on the link for 'fft function in scilab', that should open it in the
// Help Browser.
