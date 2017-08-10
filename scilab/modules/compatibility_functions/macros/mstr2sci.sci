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

function y=mstr2sci(x)
    // Conversion of a Matlab format string into a Scilab string
    // In Matlab : 'string' has size [1 6]
    // In Scilab : 'string' has size [1 1]
    // This function solves this problem by converting a string into a character matrix
    // Ex : 'string' is converted into ['s' 't' 'r' 'i' 'n' 'g']

    if type(x)==17 then
        error(msprintf(gettext("%s: This feature has not been implemented: Support for hypermatrices.\n"),"mstr2sci"));
        return
    end
    if type(x)<>10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),"mstr2sci", 1));
        return
    end
    nr=size(x,1);
    nc=max(length(x));
    y=[]
    for k=1:nr
        x(k,:)=strcat(x(k,:));
        nc=max(length(x(k,:)));
        tmp = ascii(x(k,:));
        for l=1:nc
            y(k,l)=ascii(tmp(l));
        end
    end

endfunction
