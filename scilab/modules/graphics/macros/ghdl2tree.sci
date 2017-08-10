// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Serge Steer
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [t] = ghdl2tree(h)
    //  converts a graphic handle into a scilab tree (resursive tlist

    if argn(2)<>1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "ghdl2tree", 1));
    end

    if size(h,"*")> 1 then
        t=list();for k=1:size(h,"*"),t(k)=ghdl2tree(h(k)),end
        return
    end

    typ=h.type

    //get the vector of fields names and make it a row
    fields=matrix(ghdl_fields(h),1,-1)

    if fields<>[] then
        t=tlist([typ,fields])
        for name=fields
            if execstr("p=get(h,name)","errcatch")<>0 then
                disp("error ghdl2tree "+typ+" "+name)
            else
                if type(p)==9 then // current field is a handle or a vector of handle
                    if size(p,"*")== 1 then
                        p=ghdl2tree(p),
                    else
                        chl=list();for k=1:size(p,"*"),chl(k)=ghdl2tree(p(k)),end
                        p=chl
                    end
                end
                t($+1)=p
            end
        end
    else //no fields defined
        t=tlist(typ)
    end
endfunction

