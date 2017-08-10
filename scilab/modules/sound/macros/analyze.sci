// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function analyze (w,fmin,fmax,rate,points)
    // Make a frequency plot of the signal w with sampling rate.
    // The data must be at least points long.
    // The maximal frequency plotted will be fmax, the minimal fmin.
    [lhs,rhs]=argn(0);
    if ( rhs <= 4 ) then points=8192 ; end ;
    if ( rhs <= 3 ) then rate=0; end ;
    if ( rhs <= 2 ) then fmax=1500; end ;
    if ( rhs <= 1 ) then fmin=10;  end ;
    defaultrate=22050;
    if rate==0; rate=defaultrate; end;
    v=w(1:points);
    f=abs(fft(v,1));
    i=fmin/rate*points:fmax/rate*points;
    fr=i/points*rate;
    plot2d(fr',f(i)');
endfunction
