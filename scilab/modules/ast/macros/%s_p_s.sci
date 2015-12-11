// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
