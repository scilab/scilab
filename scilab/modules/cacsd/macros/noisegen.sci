// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function noisegen(pas,Tmax,sig)
    // This function returns in the calling env
    // a macro  [b]=Noise(t)
    // Noize(t) is a piecewise constant function [k*pas,(k+1)*pas]
    //The value on each constant interval are random values from
    // i.i.d  Gaussian variables of standard deviation sig.
    // The function is constant for t<=0 and t>=Tmax.
    //!

    dua_g=sig*rand(0:pas:Tmax,"n");
    [nn1,nn2]=size(dua_g);
    deff("[b]=Noise(t)","b=dua_g(min(max((t/"+string(Tmax)+...
    ")*"+string(nn2)+",1),"+string(nn2)+"))");
    [dua_g,Noise]=resume(dua_g,Noise);
endfunction
