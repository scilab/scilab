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
function str = %ce_string(c)
    str=[];
    ll = lines()
    val = c{:}
    dims = size(c);
    if type(val)==15 then // Whole cell array display
        for k=1:length(val)
            tp = typeof(val(k))
            sz = size(val(k))
            if tp=="st" then
                str(k)="{"+strcat(string(sz),"x")+" struct}"
                //str(k)=%st_string(val(k))
            else
                if prod(sz)==0 then
                    str(k)="{}"
                else
                    tmp=sci2exp(val(k),ll(1))
                    if size(tmp,"*")==1 then
                        str(k)=tmp
                    else
                        str(k)="{"+strcat(string(sz),"x")+" "+tp+"}"
                    end
                end
            end
        end
    else // cell display
        tp = typeof(val)
        sz = size(val)
        dims = [1 1]
        if tp=="st" then
            str="{"+strcat(string(sz),"x")+" struct}"
        else
            if prod(sz)==0 then
                str="{}"
            else
                tmp=sci2exp(val,ll(1))
                if size(tmp,"*")==1 then
                    str=tmp
                else
                    str="{"+strcat(string(sz),"x")+" "+tp+"}"
                end
            end
        end
    end

    if size(dims,"*")>2 then
        str=matrix(str, dims)
    else
        str=matrix(str,dims(1),-1)
    end
endfunction

