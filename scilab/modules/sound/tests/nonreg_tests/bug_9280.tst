//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 9280 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9280
//
// <-- Short Description -->
// wavread failed to read PCM 32 bit float file format.

filename = SCI + '/modules/sound/tests/nonreg_tests/bug_9280.wav';
ierr = execstr('[Ip, Fs, bits] = wavread(filename);', 'errcatch');
if ierr <> 0 then pause, end
sIp = size(Ip);
if sIp(1) <> 1 then pause, end
if sIp(2) <> 1000 then pause, end
if Fs <> 96000 then pause, end
if bits <> 32 then pause, end
