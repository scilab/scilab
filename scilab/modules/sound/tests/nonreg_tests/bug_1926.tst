//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1926 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1926
//
// <-- Short Description -->
// wavwrite does not work properly in Scilab 4.0, final version.  The same
//instructions create a correct file in Scilab 3.0.  When the .wav file is written
//for a single channel, from a 1 column vector, the scilab message indicates
//thousands of channels (but still a small fraction of the number of samples, so
//it's not simply one-deep by many channels).


//=====================================================================
r = wavread("SCI/modules/sound/demos/chimes.wav","size");
if r <> [2 13921] then pause,end

[yref,FsRef,bitsRef]= wavread("SCI/modules/sound/demos/chimes.wav");
wavwrite(yref,FsRef,TMPDIR+'/chimes.wav');

r = wavread(TMPDIR+'/chimes.wav',"size");
if r <> [2 13921] then pause,end

[y,Fs,bits]= wavread(TMPDIR+'/chimes.wav');
if yref <> y then pause,end;
if FsRef <> Fs then pause,end;
if bitsRef <> bits then pause,end;
//=====================================================================
r = wavread("SCI/modules/sound/tests/nonreg_tests/bug_1926.wav","size");
if r <> [2 11026] then pause,end

[yref,FsRef,bitsRef]= wavread("SCI/modules/sound/tests/nonreg_tests/bug_1926.wav");
wavwrite(yref,FsRef,TMPDIR+'/bug_1926.wav');

r = wavread(TMPDIR+'/bug_1926.wav',"size");
if r <> [2 11026] then pause,end

[y,Fs,bits]= wavread(TMPDIR+'/bug_1926.wav');
if yref <> y then pause,end;
if FsRef <> Fs then pause,end;
if bitsRef <> bits then pause,end;
//=====================================================================
