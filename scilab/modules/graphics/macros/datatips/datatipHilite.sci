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

function datatipHilite(datatip_handle)
    //datatip utility function
    point_handle=datatip_handle.children(1)
    string_handle=datatip_handle.children(2)
    style=string_handle.user_data(2);
    if style(1)==1 then
        if point_handle.mark_background==point_handle.mark_foreground then
            point_handle.mark_background=addcolor([255 255 238]/255);
        else
            point_handle.mark_background=point_handle.mark_foreground;
        end
    else
        if point_handle.mark_mode=="on" then
            point_handle.mark_mode="off"
        else
            point_handle.mark_mode="on"
        end

    end
endfunction
