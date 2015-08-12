// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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

    if typeof(z) == "hypermat" then
        phi = matrix(phi, size(z));
    end
endfunction
