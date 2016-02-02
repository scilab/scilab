// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Serge STEER
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r=mtlb_e(a,i)
    if type(a)==10 then
        a=strcat(a);
        if i==$ then
            r=part(a,length(a))
        else
            r=part(a,i)
        end
    else
        r=a(i)
        if size(i,2)>1 then //index is a row vector
            if min(size(a))>1 then // a is'nt a vector
                r=matrix(r,1,size(r,"*")) //return a row vector
            end
        end
    end
endfunction
