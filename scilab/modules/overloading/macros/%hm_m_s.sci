// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function M1=%hm_m_s(M1,M2)

    siz1 = size(M1);
    siz2 = size(M2);

    if size(M2,"*")<>1 then
        if length(siz1)<>3 then
            error(msprintf(_("%s: Wrong size for input argument #%d: 3D maximum expected.\n"),"%hm_m_s",2));
        end
        if siz1(2)<>siz2(1) then
            error(msprintf(_("%s: Wrong size for argument: Incompatible dimensions.\n"),"hm_m_s"));
        end
        res = zeros(siz1(1), siz2(2), siz1(3));
        for i=1:siz1(3)
            res(:, :, i) = M1(:, :, i)*M2;
        end
        M1 = res;
    else
        M1.entries = M1.entries*M2
    end

endfunction
