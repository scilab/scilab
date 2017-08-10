// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

function [r] = strange(x,orien)
    //
    //The range  is  the distance between   the largest  and smaller  value,
    //[r]=range(x) computes the range of vector or matrix x.
    //
    //[r]=range(x,'r')  (or equivalently  [r]=range(x,1)) give a  row vector
    //with the range of each column.
    //
    //[r]=range(x,'c') (or equivalently [r]=range(x,2)) give a column vector
    //with the range of each row.
    //
    //
    [lhs,rhs] = argn(0)
    if rhs==0 then 
        msg = _("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg,"strange",0,2))
    end
    if rhs==1 then
        if x==[]
            r = %nan
        else
            r = max(x) - min(x)
        end
    elseif rhs==2 then
        if orien=="r" | orien==1 then
            r = max(x,"r")-min(x,"r")
        elseif orien=="c" | orien==2 then
            r = max(x,"c") - min(x,"c")
        else
            msg = _("%s: Wrong value for %d input argument: ''%s'', ''%s'', %d or %d expected.\n")
            error(msprintf(msg,"strange",2,"r","c",1,2))
        end
    end
endfunction
