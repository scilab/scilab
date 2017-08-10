// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function datatipSetTipPosition(ud,tip_handle,pt,k)
    //datatip utility function
    point_handle=tip_handle.children(1)
    string_handle=tip_handle.children(2)

    tip_refs=point_handle.user_data
    [curve_handle,point_index]=tip_refs(:)
    formatfunction=ud.formatfunction

    if ud.style==0 | size(curve_handle.data,1) == 1 then
        point_handle.data=pt
    else
        if k<size(curve_handle.data,1)
            dxy=curve_handle.data(k+1,:)-pt;
        else
            dxy=pt-curve_handle.data(k-1,:);
        end
        dxy=dxy/norm(dxy)/10000;
        point_handle.data=[pt;pt+dxy]
    end
    tip_refs(2)=k;
    point_handle.user_data=tip_refs
    string_handle.text=formatfunction(curve_handle,pt,k)
    setStringPosition(tip_handle,pt)

endfunction
