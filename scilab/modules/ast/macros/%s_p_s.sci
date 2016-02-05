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

function x=%s_p_s(a,p)

    x=%s_pow(a,p)

    [m,n]=size(a)
    [mp,np]=size(p)
    if m==n&mp*np==1 then  //A^p  p non integer
        r=and(imag(a)==0)
        if isreal(p) & int(p)==p & r then
            x=real(x);
        end
    end

endfunction
