// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
