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

function t=%st_string(s)
    f=getfield(1,s)
    ll=lines()
    t=[]
    for k=3:size(f,"*")
        var=f(k)
        value=s(var)
        tp=typeof(value)
        if tp=="ce" then
            t=[t;%ce_string(value)]
        elseif tp=="st" then
            t=[t;%st_string(value)]
        else
            str=sci2exp(value,ll(1))
            txt="   "+var+": "
            if size(str,"*")==1 then
                txt=txt+str
            else
                tp=typeof(value)
                txt=txt+"{"+strcat(string(size(value)),"x")+" "+tp+"}"
            end
            t=[t;txt]
        end
    end
endfunction
