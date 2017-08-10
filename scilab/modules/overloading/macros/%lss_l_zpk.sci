// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function r=%lss_l_zpk(a,b)
    //r=a\b
    if size(a,"*")==1 then
        a=zpk(a)
        r=zpk(a.P,a.Z,1/a.K,a.dt)*b
    else
        r=a\zpk2ss(b)
    end
endfunction
