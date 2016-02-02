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

function datatipRemoveNearest(curve,pt)
    //datatip utility function
    ud=datatipGetStruct(curve)
    if typeof(ud)=="datatips" then
        tips=ud.tips.children
        dmin=%inf;l=[];
        pt=pt(:);
        for tip_index=1:size(tips,"*")
            d=norm(tips(tip_index).children(1).data(1:2)-pt(1:2))
            if d<dmin then
                l=tip_index;dmin=d;
            end
        end
        if l<>[] then
            datatipRemove(curve,l);
        end
    end
endfunction
