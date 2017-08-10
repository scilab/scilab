
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

function [hm]=harmean(x,orien)
    //This function computes the harmonic mean of a vector or matrix x.
    //
    //For a vector  or  matrix  x,  hm=harmean(x) returns in  scalar  hm the
    //harmonic mean of all the entries of x.
    //
    //hm=harmean(x,'r')(or, equivalently,  hm=harmean(x,1)) returns  in each
    //entry of the row vector hm the harmonic mean of each column of x.
    //
    //hm=harmean(x,'c')(or, equivalently,  hm=harmean(x,2)) returns  in each
    //entry of the column vector hm the harmonic mean of each row of x.
    //
    //Nota: In the computation of the harmonic mean we consider only the non
    //zero values of x.
    //
    //References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
    //Statistics, J.Wiley & Sons, 1990.
    //
    //
    [lhs, rhs] = argn(0)
    if rhs == 0 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"harmean",1,2))
    end
    if x == [] then
        hm = %nan
        return
    end
    if rhs == 1 then
        hm = 1/(sum(1 ./x(x<>0))/sum(x<>0))
        return
    elseif rhs == 2 then
        if orien == "r" | orien == 1 then
            le = ones(1,size(x,1))*bool2s(x<>0)
        elseif orien == "c" | orien == 2 then
            le = bool2s(x<>0)*ones(size(x,2),1)
        else error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', %d or %d expected.\n"),"harmean",2,"r","c",1,2)), end
        x(x==0) = %inf
        hm = 1 ./(sum(1 ./x,orien) ./le)
    else
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"harmean",1,2)),
    end
endfunction
