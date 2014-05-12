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

function M2 = %s_m_hm(M1,M2)

    siz1 = size(M1);
    siz2 = size(M2);

    if size(M1,"*")<>1 then
        if length(siz2)<>3 then
            error(msprintf(_("%s: Wrong size for input argument #%d: 3D maximum expected.\n"),"%s_m_hm",2));
        end
        if siz1(2)<>siz2(1) then
            error(msprintf(_("%s: Wrong size for argument: Incompatible dimensions.\n"),"s_m_hm"));
        end
        M2 = matrix(M2, [siz2(1) siz2(2)*siz2(3)]);
        M2 = matrix(M1*M2, [siz1(1) siz2(2) siz2(3)]);
    else
        M2 = matrix(M1*M2(:), size(M2))
    end

endfunction
