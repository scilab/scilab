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

function datatipSetSelected(curve_handles,tip_indices)
    //datatip utility function
    //set the selected datatip(s)

    [curve,ind]=datatipGetSelected(curve_handles)
    //unselect the current datatip if any
    if ind<>[] then
        ud=datatipGetStruct(curve);
        tips=ud.tips.children
        tip_handle=tips(ind);
        datatipHilite(tip_handle) //unhilite
        ud.selected=0
        datatipSetStruct(curve,ud);
    end

    //select the new one
    if tip_indices<>[] then
        k=tip_indices(1);
        l=tip_indices(2);
        curve=curve_handles(k);
        ud=datatipGetStruct(curve);
        tips=ud.tips.children
        tip_handle=tips(l);
        datatipHilite(tip_handle)  //hilite
        ud.selected=l
        datatipSetStruct(curve,ud);
    end
endfunction

