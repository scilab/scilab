//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 629 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=629
//
// <-- Short Description -->
//    When trying to use the wavread function I get these errors
//    that make this function totally unuseful, either calling it
//    with ONE parameter (a) or calling it with two parameters.
//    I made myself sure that the parameters would be of the form
//    indicated by help on line.


// exec("e:\testNonReg\629.sce");  
// iReturnCode=execstr("[x,fs,bits] = wavread("e:\testNonReg\t1.wav")','errcatch');

if execstr("wavread(SCI+''/modules/sound/tests/nonreg_tests/bug_629.wav'')","errcatch") <> 0 then pause,end
