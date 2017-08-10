// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Manuel Juliachs
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [xy]=scaling(xy,scalingFactor,orig)
    //  xy=xy*scalingFactor+orig
    //
    // xy     : matrix with 2 rows
    // scalingFactor : scaling factor
    // orig   : origin, [0;0] if omitted
    //!
    [lhs,rhs]=argn(0)
    select rhs
    case 2 then orig=[0;0];
    case 3 then orig=matrix(orig,2,1);
    else error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected."), "scaling", 2, 3));
    end;
    //
    [m,n]=size(xy)
    if m<>2 then  error("xy must be a vector with 2 rows [x;y]"),end
    //
    xy=xy-orig*ones(1,n)
    xy=scalingFactor*xy+orig*ones(1,n)
endfunction
