// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [y]=%asn(x,m)
    //Calculates the elliptic integral:
    //  y = integral from 0 to x of
    //       [1/(((1-t*t)^(1/2))(1-m*t*t)^(1/2))]
    //For vector arguments y is a vector
    //  x :Upper limit of integral (x>0)
    //  m :Parameter of integral (0<m<1)
    //  y :Value of the integral
    //
    //!
    warnobsolete("delip","5.5.1");
    m=real(m);

    if m<0|m>1 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"),"%%asn",2,"0","1"));
    end

    y=delip(x,sqrt(m));
endfunction
