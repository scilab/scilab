// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
// Copyright (C) - 2010 - DIGITEO - Michael Baudin
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
function x = cosd(x)
    //Element wise cosine , argument in degree.

    rhs = argn(2);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"cosd", 1));
    end

    if type(x) <> 1 | ~isreal(x) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"cosd",1));
    end

    if ~isempty(x)
        n = round(x/90);
        x = x - n*90;
        m = pmodulo(n,4);
        x(m==0) = cos(%pi/180*x(m==0));
        x(m==1) = -sin(%pi/180*x(m==1));
        x(m==2) = -cos(%pi/180*x(m==2));
        x(m==3) = sin(%pi/180*x(m==3));
    end
endfunction
