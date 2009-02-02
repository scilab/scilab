// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4037 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4037
//
// <-- Short Description -->
// waveread can not read broadcast wave format
// a error is returned

// <-- INTERACTIVE TEST -->

// create or download a bfw file
// try to load wavread

namebfw = "bfw.wav"
ierr = execstr('wavread(namebfw)','errcatch');
if ierr<> 999 then pause,end
