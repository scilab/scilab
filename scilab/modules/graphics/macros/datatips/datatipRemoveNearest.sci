// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
