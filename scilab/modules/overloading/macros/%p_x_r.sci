// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r=%p_x_r(p,r)
    // r=%p_x_r(p,r)  <=> r=p.*r   polynomial.* rational
    //author Serge Steer, INRIA
    //!

    r.num=p.*r.num
    sz=size(r.num)
    if size(sz,"*")<=2 then
        r=simp(r)
    else
        [num,den]=simp(r.num(:),r.den(:))
        r.num=matrix(num,sz)
        r.den=matrix(den,sz)
    end
endfunction
