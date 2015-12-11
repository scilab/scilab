// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function c=mtlb_cond(A,flag)
    // Emulation function for cond() Matlab function

    if A == [] then c = 0, return, end
    if size(A,1) <> size(A,2) &  flag <> 2 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Square matrix expected.\n"),"mtlb_cond", 1));
    end

    if flag == 1
        c = norm(A,1)*norm(inv(A),1);
    elseif flag == 2
        if size(A,1) == size(A,2) then
            c = cond(A);
        else //case rectangular matrix
            s = svd(A)
            if s($) == 0 then
                c = %inf;
            else
                c = s(1)/s($);
            end
        end
    elseif flag == "fro"
        c = norm(A,"fro")*norm(inv(A),"fro");
    elseif flag == "inf"
        c = norm(A,"inf")*norm(inv(A),"inf");
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: %s, %s,''%s'' or ''%s'' expected.\n"),"mtlb_cond",2,"1","2","fro","inf"));
    end
endfunction
