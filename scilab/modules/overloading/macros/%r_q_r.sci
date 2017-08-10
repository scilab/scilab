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

function s1=%r_q_r(s1,s2)
    // %r_q_r(s1,s2) <=> s1= s1.\s2   for rationals
    //author Serge Steer, INRIA
    //!
    if ndims(s1)<=2& ndims(s2)<=2 then
        [s1,s2]=sysconv(s1,s2)
        [num,den]=simp(s1.den.*s2.num,s1.num.*s2.den)
        s1=rlist(num,den,s1.dt)
    else
        if size(s1,"*")==1 then
            sz=size(s2)
        else
            sz=size(s1)
        end
        [num,den]=simp(s1.den(:).*s2.num(:),s1.num(:).*s2.den(:))
        s1=rlist(matrix(num,sz),matrix(den,sz),s1.dt)
    end

endfunction
