// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 600 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=600
//
// <-- Short Description -->
//    I am not able to hear the sounds using either the 'Sound'
//    or 'playsnd' functions.
//
//       The sound card is working properly with all other
//    software. All other wave functions are working such as
//    wavread loadwave etc...

disp("activate your sound card, and run this file");
disp("if you hear something, that means the playsnd & PlaySnd work");

y=wavread('SCI/modules/sound/demos/chimes.wav');
playsnd(y);

PlaySound('SCI/modules/sound/demos/chimes.wav');
