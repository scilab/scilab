// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 11860 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11860
//
// <-- Short Description -->
// wavwrite failed for 24 bits files (error in savewave function)
//
filename=SCI +'/modules/sound/tests/nonreg_tests/bug_11860.wav';
[y_ref,Fs_ref,nbits_ref]=wavread(filename);
wavwrite(y_ref,Fs_ref,nbits_ref,SCI+'/modules/sound/tests/nonreg_tests/bug_11860_written.wav');
filename_written=SCI+'/modules/sound/tests/nonreg_tests/bug_11860_written.wav';
[y,Fs,nbits]=wavread(filename_written);
assert_checktrue(abs(y_ref-y)<1e-6);
assert_checkalmostequal(Fs,Fs_ref);
assert_checkalmostequal(nbits,nbits_ref);
mdelete(filename_written);
