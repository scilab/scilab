// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 11684 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11684
//
// <-- Short Description -->
// mfft failed due to MKL fftw library restrictions.

y1=matrix(1:6,2,3);
y2=matrix(7:12,2,3);
y=matrix([y1,y2],1,12);
yf=mfft(y,-1,[2 3 2]);