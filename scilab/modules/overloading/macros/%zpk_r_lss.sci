// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function r=%zpk_r_lss(a,b)
    //r=a/b
    if size(b,"*")==1 then
        b=zpk(b);
        r=a*zpk(b.P,b.Z,1/b.K,b.dt)
    else
        r=zpk2ss(a)/b
    end
endfunction
