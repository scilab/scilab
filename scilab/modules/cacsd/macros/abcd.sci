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

function [A,B,C,D]=abcd(sl)
    // Retrieves [A,B,C,D] matrices from linear system sl

    select typeof(sl)
    case "state-space" then
        [A,B,C,D]=sl(2:5)
        return;
    case "rational" then
        w=tf2ss(sl);
        [A,B,C,D]=w(2:5)
    else
        error(msprintf(_("%s: Wrong type for input argument: Linear dynamical system expected.\n"),"abcd"))
    end
endfunction
