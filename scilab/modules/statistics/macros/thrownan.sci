// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// Copyright (C) 2010 - DIGITEO - Michael Baudin
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

function [nonan,numb]=thrownan(x)
    //
    //
    //This  function  returns  in  vector  nonan  the  values
    //(ignoring the NANs) of a  vector or matrix x and in the
    //corresponding places of vector  numb the indexes of the
    //value.
    //
    //For  a  vector  or matrix  x,  [nonan,numb]=thrownan(x)
    //considers x, whatever his dimensions are, like a linear
    //vector (columns  first).
    //
    //
    [lhs,rhs]=argn(0)
    if ( rhs<>1 ) then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"thrownan",1))
    end
    tf = ~isnan(x)
    numb=find(bool2s(tf))
    nonan=x(tf)

endfunction

