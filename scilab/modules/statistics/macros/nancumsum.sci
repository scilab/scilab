// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2003 - INRIA - Carlos Klimann
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
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


function [s]=nancumsum(x,orient)
    //
    //This function returns in scalar or vector s the sum of the
    //values (ignoring the NANs) of  a vector or matrix (real or
    //complex) x.
    //
    //This function  for a vector or a  matrix x, s=nancumsum(x)
    //(or, equivalently  s=nancumsum(x,'*') returns in  scalar s
    //the cumulative sum (ignoring  the NANs) of all the entries
    //of x taken columnwise.
    //
    //s=nancumsum(x,'r')  (or,  equivalently,  s=nancumsum(x,1))
    //returns in  the cols(x) sized vector s  the cumulative sum
    //(ignoring    the    NANs)    of    the    rows    of    x:
    //s(:,i)=nancumsum(x(:,i))
    //
    //s=nancumsum(x,'c')  (or,  equivalently,  s=nancumsum(x,2))
    //returns in  the rows(x) sized vector s  the cumulative sum
    //(ignoring     NANs)    of     the     columns    of     x:
    //s(i,:)=nancumsum(x(i,:))
    //

    [lhs, rhs] = argn(0)
    if lhs > 1 then
        error(msprintf(gettext("%s: Wrong number of output argument: %d expected.\n"),"nancumsum",1))
    end
    
    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"),"nancumsum",1,2))
    end
    
    if or(type(x) == [10 15 16]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex, boolean, polynomial matrix expected.\n"), "nancumsum", 1));
    end
    
    // cumsum([]) == []
    if isempty(x) | and(isnan(x)) then
        s = [];
        return
    end
    
    if rhs == 1 then
        orient = "*";
    else
        if and(type(orient) <> [1  10])  then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string or scalar expected.\n"), "nancumsum", 2));
        end
        
        if ~or(orient == ["r", "*", "c"] | orient == [1, 2]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "nancumsum", 2, """r"", ""c"", ""*"", 1, 2"));
        end
    end
    
    isn=isnan(x)
    x(isn)=0
    s=cumsum(x,orient)

endfunction
