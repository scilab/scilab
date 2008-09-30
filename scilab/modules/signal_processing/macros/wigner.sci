// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function tab=wigner(x,h,deltat,zp)
//macro which computes the 'time-frequency' wigner
//spectrum of a signal.
//
//	tab    : wigner spectrum (lines correspond to the time variable)
//	x      : analysed signal
//	h      : data window
//      deltat : analysis time increment (in samples)
//      zp     : length of FFT's. %pi/zp gives the frequency increment.
//!
//
//   Initializations
//
l=prod(size(x));
n=prod(size(h));
npr=2*n;
h=h.*conj(h);tab=[];
//
//   Analytical signal computation using Hilbert transform
//
[y,y1]=convol(hilb(127),x);
z=x+%i*y;
//
//   Wigner distribution computation
//
t=n;
while t<=l-n,
  z1=h.*z(t:t+n-1).*conj(z(t:-1:t-n+1));
  z1(zp)=0;
  w=fft(z1,-1);
  w=2*(2*real(w)-z(t)*z(t)'*ones(w));tab=[tab;w];
  t=t+deltat;
end;
tab=tab(:,1:zp);
endfunction
