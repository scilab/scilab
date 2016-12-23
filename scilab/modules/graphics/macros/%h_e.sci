// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function r=%h_e(i,h)
    h=h
    if type(i)==10 then
        r=get(h,i)
    elseif type(i)==15 then
        n=lstsize(i)

        for k=1:n
            p=i(k)
            if type(p)==10 then
                if type(h)==9 then
                    h=get(h,p),
                else
                    h=h(p)
                end
            elseif or(type(p)==[1 2 4 8 129]) then
                h=h(p)
            elseif type(p)==15 then
                h=h(p(:))
            else
                error("Invalid path")
            end
        end
        r=h
    elseif or(type(i)==[1,2]) then
        r=h(i)
    else
        error("Invalid path")
    end

    if type(r)==10 then r=stripblanks(r),end
endfunction
