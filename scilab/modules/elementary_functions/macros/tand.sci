// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
// Copyright (C) 2010 - DIGITEO - Michael Baudin
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = tand(x)
    // Tangent of x in degrees.
    // tand(x) = tan(x*%pi/180) in the mathematical sense
    // But floating point numbers behave differently.
    // For example, tand(180) = 0 while tan(pi) = -1.225D-16

    rhs = argn(2);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"tand", 1));
    end

    if ~or(type(x)==[1 5]) | ~isreal(x) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"tand",1));
    end

    if ~isempty(x)
        m = pmodulo(x, 360);
        n = round(x / 180);
        x = x - n * 180;
        y = tan(%pi/180*x);
        y(m == 90) = %inf;
        y(m == 270) = -%inf;
    else
        y = []
    end
endfunction

