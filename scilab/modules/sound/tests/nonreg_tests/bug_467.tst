//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 467 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=467
//
// <-- Short Description -->
//    Bug Report Id: 12022200362265140
//    If you don't mind, where are read_dat_pcm or read_wavedat?
//
//
//
//    On Scilab 2-7 with " wavread " function
//    The Error Messages are:
//       !--error     4
//    undefined variable : erro
//    at line      26 of function read_dat_pcm             called by :
//    line     6 of function read_wavedat             called by :
//    line    93 of function wavread                  called by :
//    [x,y] = wavread('c:\b.wav','size'); 
//
//
//     !--error   246
//    function not defined for given argument type(s)
//    Check arguments or define function %s_convstr               for overloading
//    at line      28 of function wavread                  called by :
//    z = wavread('c:\b.wav',100000);   
// ...

result = execstr("[x,y] = wavread(SCI+''/modules/sound/tests/nonreg_tests/bug_467.wav'',''size'')","errcatch","n");

if result <> 0 then pause,end

if result == 0 then 
   if execstr("z = wavread(SCI+""/modules/sound/tests/nonreg_tests/bug_467.wav"",10)","errcatch","n") <> 0 then pause,end
end
