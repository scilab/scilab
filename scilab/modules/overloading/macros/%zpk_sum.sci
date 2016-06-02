// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function r=%zpk_sum(v)

    [z,p]=reduceToCommonDenominator(v.Z,v.P,v.K);
    //find common terms in z
    [sc,z]=findCommonValues(z)
    s=0;
    for k=1:size(v,"*")
        s=s+v.K(k)*real(poly(z{k},"x","r"));
    end
    r=roots(s,"e")
    z=[sc(:);r];
    k=coeff(s,size(r,"*"));//s may have high ordrer coefficients equal to 0
    r=zpk(z,p,k,v.dt);
endfunction
