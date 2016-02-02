// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [ind, occ, info] = %i_dsearch(H, r, flag)
    if argn(2)<3 then
        flag = "c";
    end
    H = iconvert(H, 0);
    r = iconvert(r, 0);
    [ind, occ, info] = dsearch(H, r, flag);
endfunction
