// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009 - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4316 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4316
//
// <-- Short Description -->
// mfprintf(0,'toto') does not use stderr (0) as fprintf

// <-- INTERACTIVE TEST -->

mfprintf(0,'bug 4316 --> redirection in stderr');
mfprintf(6,'bug 4316 --> redirection in stdout');

ierr = execstr('mfprintf(5,''bug 4316 --> redirection in stdin'');','errcatch');
if ierr <> 999 then pause,end
