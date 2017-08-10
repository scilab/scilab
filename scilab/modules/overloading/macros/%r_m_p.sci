// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2009 - 2016 - Samuel GOUGEON
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [f1] = %r_m_p(r1,p2)
    // r = %r_m_p(r1, p2)  <=> r = r1*p2
    // r1 = rational p2 = polynomial
    if size(p2, "*") == 1 then
        [num, den] = simp(r1.num*p2, r1.den)
        f1=rlist(num,den,r1("dt"))
    else
        if isrow(r1) then
            f1 = p2.'*r1.';
        else
            f1 = (p2.'*r1.').';
        end
    end
endfunction
