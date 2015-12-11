// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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

