// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2001 - INRIA - Scilab 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

mode(1)
// At first we create 0.5 seconds of sound parameters.

t=soundsec(0.5);
// Then we generate the sound.

s=sin(440*t)+sin(220*t)/2+sin(880*t)/2;
[nr,nc]=size(t);
s(nc/2:nc)=sin(330*t(nc/2:nc));

// We can easily make a Fourier analysis of it.
my_handle = scf(100001);
clf(my_handle,"reset");
analyze(s);
halt();

// Save the file in WAV format.
// we renormalize s in order to check that save+load is invariant
s=s-sum(s)/prod(size(s)); s=s/max(abs(s));
savewave(TMPDIR+"/test.wav",s);
// Load it back.
s1=loadwave(TMPDIR+"/test.wav");
if maxi(abs(s1-s)) < 1.e-4;end 
// Now we can make a complete picture of the sound.
clf(my_handle,"reset");
mapsound(s);
halt();
// Or a Fourier analysis.
clf(my_handle,"reset");
analyze(s);
