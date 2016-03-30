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

function f1=%r_x_p(f1,f2)
    // %r_x_p(r,p)=r.*p
    //author Serge Steer, INRIA
    //!
    f1.num=f1.num.*f2
    sz=size(f1.num)
    if size(sz,"*")<=2 then

        f1=simp(f1)
    else
        [num,den]=simp(f1.num(:),f1.den(:))
        f1.num=matrix(num,sz)
        f1.den=matrix(den,sz)
    end
endfunction
