// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function Tn=chepol(n,var)
    //Chebychev polynomial
    //  n   :Polynomial order
    //  var :Polynomial variable (character string)
    //  Tn  :Polynomial in var
    //
    //!
    if n==0 then
        Tn=poly(1,var,"coeff"),
    elseif n==1 then
        Tn=poly(0,var);
    else
        T0=poly(1,var,"coeff");
        T1=poly(0,var)
        for k=2:n
            Tn=2*poly(0,var)*T1-T0
            [T1,T0]=(Tn,T1);
        end
    end

endfunction
