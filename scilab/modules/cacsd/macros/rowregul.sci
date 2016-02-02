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

function [Stmp,Ws]=rowregul(Sl,Alfa,Beta);
    //[Stmp,Ws]=regul(Sl) computes a polynomial-state-space postfilter
    //Ws such that Stmp=Ws*Sl is proper and has D full rank
    //Poles at infinity of Sl are moved to -Alfa;
    //Zeros at infinity of Sl are moved to -Beta;
    //Sl is asummed right invertible i.e. ss2tf(Sl) full row rank
    //!

    [Stmp,Ws]=colregul(Sl',Alfa,Beta);
    Stmp=Stmp';Ws=Ws';
endfunction
