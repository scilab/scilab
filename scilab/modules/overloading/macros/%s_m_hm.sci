// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function M2=%s_m_hm(M1,M2)

    siz1 = size(M1);
    siz2 = size(M2);

    if size(M1,"*")<>1 then
        if length(siz2)<>3 then
            error(msprintf(_("%s: Wrong size for input argument #%d: 3D maximum expected.\n"),"%s_m_hm",2));
        end
        if siz1(2)<>siz2(1) then
            error(msprintf(_("%s: Wrong size for argument: Incompatible dimensions.\n"),"s_m_hm"));
        end
        M2 = hypermat([siz2(1) siz2(2)*siz2(3)], M2);
        M2 = hypermat([siz1(1) siz2(2) siz2(3)], M1*M2);
    else
        M2.entries = M1*M2.entries
    end

endfunction
