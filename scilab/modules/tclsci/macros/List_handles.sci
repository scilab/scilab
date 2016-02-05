// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function List_handles(h)
    global ged_handle_out;

    i = 1;
    //if h.type=="Axes" then
    //   ged_handle_out = [ged_handle_out;h.x_label;h.y_label;h.z_label;h.title];
    //end
    psonstmp = h.children;
    if psonstmp <> [] then
        psonstmp = h.children(1);
    end

    while ((psonstmp <>[]) & ((i) <=size(psonstmp.parent.children,1)))
        i = i+1;
        ged_handle_out = [ged_handle_out;  psonstmp];
        List_handles(psonstmp);

        if ((i) <=size(psonstmp.parent.children,1)) then
            psonstmp = psonstmp.parent.children(i);
        else
            psonstmp=[];
        end

    end

endfunction

