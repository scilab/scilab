// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 5081 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5081
//
// <-- Short Description -->
//
// demo "Signal Processing" --> "Spectral Estimation" fails (Windows)

 rand('normal');
 rand('seed',0);
 x = rand(1:1024-33+1);
 nf = 33;
 bedge = [0 .1;.125 .5];
 des = [1 0];
 wate = [1 1];
 h = eqfir(nf,bedge,des,wate);
 h1 = [h 0*ones(1:max(size(x))-1)];
 x1 = [x 0*ones(1:max(size(h))-1)];
 hf = fft(h1,-1);
 xf = fft(x1,-1);
 yf = hf.*xf;
 y = real(fft(yf,1));
 ierr = execstr("[sm2] = cspect(100,200,''tr'',y);","errcatch");
 if ierr <> 0 then pause,end
