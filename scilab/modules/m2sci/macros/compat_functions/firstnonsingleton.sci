// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function dim=firstnonsingleton(in,opt)
    // opt="str" then return "r" or "c" if possible
    // opt="num" then return numerical values
    rhs=argn(2)
    if rhs==1 then
        opt="str"
    end

    if opt=="num" then
        dim=1
    else
        dim="r"
    end
    sz=size(in)
    for k=1:size(sz,"*")
        if sz(k)==0 then
            if opt=="num" then
                dim=1
            else
                dim="r"
            end
            break
        elseif sz(k)>1 then
            if k==1 then
                if opt=="num" then
                    dim=1
                else
                    dim="r"
                end
            elseif k==2 then
                if opt=="num" then
                    dim=2
                else
                    dim="c"
                end
            else
                dim=sz(k)
            end
            break
        end
    end

endfunction
