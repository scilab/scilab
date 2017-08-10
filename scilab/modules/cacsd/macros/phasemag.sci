// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [phi,db]=phasemag(z,mod)
    //

    mod_def="c"     //continuous representation
    //mod_def='m'   //representation modulo 360 degrees
    [lhs,rhs]=argn(0)
    if lhs==2 then
        db=20*log(abs(z))/log(10),
    end
    if rhs<>2 then
        mod=mod_def
    end
    //compute first phase value in  (-pi, pi]
    phi1=atan(imag(z(:,1)),real(z(:,1)))
    //compute phase increments in (-pi, pi]
    ind = find(z==0);
    if ind <> [] then
        z(ind) = 1; // To avoid division by 0 (0 and 1 have the same phase)
    end
    z2=z(:,2:$)./z(:,1:$-1)
    dphi=atan(imag(z2),real(z2))
    phi=cumsum([phi1 dphi],2)

    if part(mod,1)<>"c" then  // reset modulo 360
        phi=modulo(phi,2*%pi)
    end
    phi=phi*180/%pi //transform in degree

    if length(size(z)) > 2 then
        phi = matrix(phi, size(z));
    end
endfunction
