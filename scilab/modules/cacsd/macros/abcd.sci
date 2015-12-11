// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
