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

function [curve,ind]=datatipGetSelected(curve_handles)
    //datatip utility function
    //get the selected datatip(s)
    ind=[]
    for kc=1:size(curve_handles,"*")
        curve=curve_handles(kc);
        ud=datatipGetStruct(curve);
        sel=ud.selected
        if sel>0 then ind=sel,break,end
    end
    if ind==[] then curve=[],end
endfunction

