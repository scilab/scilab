// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function r=%s_q_zpk(a,b)
    //r=a.\b
    r=zpk([],[],a,b.dt).\b;
endfunction

