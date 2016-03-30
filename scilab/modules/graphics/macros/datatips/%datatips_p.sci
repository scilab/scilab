// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function %datatips_p(d)
    F=getfield(1,d);
    txt=[];
    L=max(length(F(2:$)))
    for k=2:size(F,"*")
        f=F(k)
        if f=="interpolate" then
            txt=[txt;
            part(f,1:L+1)+"= "+sci2exp(d.interpolate)]
        elseif  f=="replace" then
            txt=[txt;
            part(f,1:L+1)+"= "+sci2exp(d.replace)]
        elseif  f=="formatfunction" then
            tree = macr2tree(d.formatfunction);
            txt=[txt;
            part(f,1:L+1)+": "
            "  "+tree2code(tree)]
        elseif f=="tips" then
            txt=[txt;
            part(f,1:L+1)+": "+string(size(d.tips,"*"))+ " tip handles" ]
        else
            txt=[txt
            part(f,1:L+1)+": "+"["+strcat(string(size(d(f))),"x")+" "+typeof(d(f))+"]"]
        end
    end
    mprintf("  %s\n",txt)
endfunction

