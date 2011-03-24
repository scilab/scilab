// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 9189 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9189
//
// <-- Short Description -->
// Certains keywords were not helpable.

mputl('fft', TMPDIR + '/bug_9189.sce')
scinotes(TMPDIR + '/bug_9189.sce');

// Right click on fft and at Help on 'fft' should be available.