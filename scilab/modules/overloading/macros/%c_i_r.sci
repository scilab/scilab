// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function s2=%c_i_r(i,j,s1,s2)

    if type(i)==10 then  // sl('dt')
        [lhs,rhs]=argn(0)
        if rhs<>3 then  error(21),end
        if i<>"dt" then
            error(msprintf(_("%s: Wrong type for input argument #%d.\n"),"%c_i_r",1));
        end
        s2=s1;kf=4
        if j<>"c"&j<>"d" then
            error(msprintf(_("%s: Wrong value for input argument #%d: ''c'', ''d'' or a scalar expected.\n"),"%c_i_r",2));
        end
        s2(kf)=j
        return
    end
endfunction
