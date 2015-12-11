// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function mapsound (w,dt,fmin,fmax,simpl,rate)
    // Plots a sound map for a sound.
    // It does FFT at time increments dt.
    // rate is the sampling rate.
    // simpl points are collected for speed reasons.

    [lhs,rhs]=argn(0);
    if ( rhs <= 5 ) then rate=0;end
    if ( rhs <= 4 ) then simpl=1; end ;
    if ( rhs <= 3 ) then fmax=1500; end ;
    if ( rhs <= 2 ) then fmin=100; end ;
    if ( rhs <= 1 ) then dt=0.1;  end ;
    n=prod(size(w));
    defaultrate=22050;
    if rate==0; rate=defaultrate; end;
    dp=dt*rate;
    points=2^floor(log(dp)/log(2));
    ind=fmin/rate*points/simpl:fmax/rate*points/simpl;
    f=abs(fft(w(1:points),1));
    f=sum(matrix(f,points/simpl,simpl),"c")';
    M=f(ind);
    i=1;
    i=i+dp;
    while i+points <= n;
        f=abs(fft(w(i:i+points-1),1));
        f=sum(matrix(f,points/simpl,simpl),"c")';
        M=[M;f(ind)];
        i=i+dp;
    end;
    [nl,nc]=size(M);
    if nl==1 ; return;end
    x_abs= (0:(nl-1))*(n/rate)/(nl-1);
    y_abs= (0:(nc-1))/(nc-1);
    y_abs= fmin*(1-y_abs) + fmax*(y_abs)
    grayplot(x_abs,y_abs,-M)
endfunction
