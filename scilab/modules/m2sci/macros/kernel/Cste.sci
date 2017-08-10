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

function const=Cste(value)
    // Create a new constant tlist

    rhs=argn(2)

    if rhs~=1 then
        error(gettext("Wrong number of inputs."));
    end

    dims=list()

    if value=="" then
        dims=list(0,0)
    else
        sz=size(value)
        for k=1:size(sz,"*")
            dims($+1)=sz(k)
            if type(value)==String then
                if k==2 then
                    dims($)=sum(length(value))
                end
            end
        end
    end

    if type(value)==String then
        prop=Real
        value=value
    else
        if type(value)==1 then
            if isreal(value) then
                prop=Real
            else
                prop=Complex
            end
        else
            prop=Real
        end
    end

    const=tlist(["cste","value","infer"],value,Infer(dims,Type(type(value),prop)))
endfunction
