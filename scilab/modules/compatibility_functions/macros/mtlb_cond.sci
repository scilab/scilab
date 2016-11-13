// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
