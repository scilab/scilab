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


// complement for Arc entity
function setA1val(val)
    global ged_handle; h=ged_handle
    ax=getparaxe(h);
    select ax.view
    case "2d"
        drawlater();
        ax.view="3d";
        tmp=h.data;
        tmp(6)=val;
        tst=execstr("h.data=tmp","errcatch","n");
        ax.view="2d";
        drawnow();
        if tst<>0 then
            disp "Warning: Width data must contain double"
        end
    case "3d"
        tmp=h.data;
        tmp(6)=val;
        tst=execstr("h.data=tmp","errcatch","n");
        if tst<>0 then
            disp "Warning: a1 data must contain double"
        end
    end
endfunction
