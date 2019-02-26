// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
// Copyright (C) 2019 - Federico MIYARA      // mu-law sample file
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 15981 -->
//
// <-- Bugzilla URL -->
// https://bugzilla.scilab.org/15981
//
// <-- Short Description -->
// wavread()
//  - weakly managed the input filename (extension, existence).
//  - never closed the wav opened file before emitting any error and quitting
//    wavread(), leaving the file locked.
//  - yielded some errors messages referring to find_cktype() instead of to the current macro
//  - claimed that some wav formats are invalid instead of are unsupported.
//    The supported wav formats were not indicated in the error message.

// Input file name management
// --------------------------
File = SCI+"/modules/sound/demos/chimes";  // should be completed to chimes.wav
assert_checktrue(execstr("wavread(File)", "errcatch")==0);

File2 = TMPDIR+"/chimes.test"         // should be completed to chimes.test.wav
copyfile(File+".wav", File2);
assert_checktrue(execstr("wavread(File2)", "errcatch")==0);

copyfile(File+".wav", File2+".wav");  // should be read as is = chimes.test.wav
assert_checktrue(execstr("wavread(File2)", "errcatch")==0);

// Management of unsupported wav format
// ------------------------------------
File = "SCI/modules/sound/tests/nonreg_tests/mu_law_format_7.wav";
refmsg = "read_wavefmt: wav format #7 detected. Only wav formats #1|3 are supported."
assert_checkerror("wavread(File)", refmsg);

