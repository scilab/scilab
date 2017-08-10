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

function [mu]=lin2mu(y)
    //Utility fct: converts linear signal to mu-law encoding.
    //  mu = lin2mu(y) converts linear audio signal amplitudes
    //   in the range  -1 <= y <= 1 to mu-law in the range 0 <= mu <= 255.
    SCALE = 32768;
    BIAS = 132;
    CLIP = 32635;
    OFFSET = 335;
    y = SCALE*y;
    sig = sign(y)+bool2s(y==0);
    y = min(abs(y),CLIP);
    [f,e] = frexp(y+BIAS);
    mu = 64*sig-16*e-fix(32*f)+OFFSET;
endfunction
