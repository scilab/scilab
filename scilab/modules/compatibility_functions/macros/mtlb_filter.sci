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

function [y,zf] = mtlb_filter(b,a,x,zi)
    [lhs,rhs]=argn()
    [m,n]=size(x)
    x=matrix(x,1,m*n)
    a=poly(a($:-1:1),"z","c")
    b=poly(b($:-1:1),"z","c")
    z=poly(0,"z")
    da = degree(a)
    db = degree(b)
    if (da-db) > 0 then b=b*z^(da-db); end
    if (db-da) > 0 then a=a*z^(db-da); end
    if  lhs==1 then
        if rhs==4 then
            y=rtitr(b,a,x,zi)
        else
            y=rtitr(b,a,x);
        end
    else
        sl=tf2ss(rlist(b,a,[]))
        if rhs==4 then
            [y,zf]=flts(x,sl,zi)
        else
            [y,zf]=flts(x,sl)
        end
    end
    y=matrix(y,m,n)
endfunction
