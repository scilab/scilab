// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function s2=%r_i_lss(i,j,s1,s2)
    //%r_i_lss(i,j,s1,s2) <=> s2(i,j)=s1
    //!
    if type(i)==10|type(j)==10 then
        error(21)
    end
    [s1 s2]=sysconv(s1,s2)
    s2(i,j)=s1
endfunction
