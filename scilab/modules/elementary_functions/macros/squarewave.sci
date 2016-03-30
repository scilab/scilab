// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function s = squarewave(t, percentPositive)
    //square(t) generates a square wave with period 2*Pi
    //square(t) is like sin(t), only it creates a square wave
    //with peaks of +1 to -1 instead of a sine wave.

    //square(t,%) generates an unsymmetric  square wave: %, is the
    //percent of the period in which the signal is positive.
    [lhs,rhs] = argn();
    if rhs < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "squarewave", 1));
    end
    if rhs < 2 then
        percentPositive=50;
    end
    s = 2*bool2s(pmodulo(t,2*%pi) < 2*%pi*percentPositive/100)-1;
endfunction
