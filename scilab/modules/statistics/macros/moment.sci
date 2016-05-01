
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1999 - INRIA - Carlos Klimann
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


function [mom]= moment(x,ord,orien)
    //
    //This function computes  the  non  central  moment of all  orders  of a
    //vector or matrix x.
    //
    //For a vector or matrix x, mom=moment(x,ord) returns in the scalar mom
    //the moment of order ord of all the entries of x.
    //
    //mom=moment(x,ord,'r')(or,  equivalently,  mom=moment(x,ord,1)) returns
    //in each  entry of the row  vector mom the  moment of order ord of each
    //column of x.
    //
    //mom=moment(x,ord,'c')(or, equivalently,  mom=cmoment(x,ord,2)) returns
    //in each entry of the column vector mom the moment of order ord of each
    //row of x.
    //
    //Nota: In the calculations  the divisor is  n, where n is the dimension
    //of the data vector.
    //
    //References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
    //Statistics, J.Wiley & Sons, 1990.
    //
    //
    [lhs,rhs]=argn(0)
    if rhs==2 then
        if x==[] then mom=%nan, return, end
        le=length(x)
        mom=sum(x.^ord)/le
    elseif rhs==3 then
        if x==[] then mom=%nan, return, end
        le=size(x,orien)
        mom=sum((x.^ord),orien)/le
    else
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"moment",2,3)),
    end
endfunction
