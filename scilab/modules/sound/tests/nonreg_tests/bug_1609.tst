//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 1609 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1609
//
// <-- Short Description -->
// I can't freely choose "fs" parameter in the following example with 'sound'

y=loadwave("SCI/modules/sound/demos/chimes.wav");
sound(y,44100);
sound(y,22500);
