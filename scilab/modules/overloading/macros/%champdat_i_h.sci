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
function h=%champdat_i_h(i,v,h)
    if type(i)==10 then
        set(h,i,v)
    elseif type(i)==15 then
        hdl=h
        p=i($)
        if type(p)==1|type(p)==4|type(p)==2|type(p)==129|type(p)==15 then
            index=i($)
            i($)=null()
        else
            index=:
        end
        n=lstsize(i)
        for k=1:n-1
            p=i(k)
            if type(p)==10 then
                hdl=get(hdl,p),
            elseif type(p)==1|type(p)==4|type(p)==2|type(p)==129 then
                hdl=hdl(p)
            elseif type(p)==15 then
                hdl=hdl(p(:))
            else
                error(msprintf(_("%s: Invalid path.\n"),"%champdat_i_h"));
            end
        end
        if type(index)==15 then
            hdl=hdl(index(:))
        else
            hdl=hdl(index)
        end
        set(hdl,i($),v)
    else
        error(msprintf(_("%s: Wrong type for input argument #%d.\n"),"%champdat_i_h",1));
    end
endfunction
