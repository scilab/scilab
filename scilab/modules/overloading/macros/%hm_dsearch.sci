// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Samuel Gougeon
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function [ind, occ, info] = %hm_dsearch(H, r, flag)
    if argn(2)<3 then
        flag = "c";
    end
    [ind, occ, info] = dsearch(H(:), r, flag)
    ind = matrix(ind, size(H))
endfunction
