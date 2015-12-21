// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function setZval(val)
    global ged_handle; h=ged_handle
    ax=getparaxe(h);
    select ax.view
    case "2d"
        drawlater();
        ax.view="3d";
        tmp=h.data;
        tmp(3)=val;
        tst=execstr("h.data=tmp","errcatch","n");
        ax.view="2d";
        drawnow();
        if tst<>0 then
            disp "Warning: Z data must contain double"
        end
    case "3d"
        tmp=h.data;
        tmp(3)=val;
        tst=execstr("h.data=tmp","errcatch","n");
        if tst<>0 then
            disp "Warning: Z data must contain double"
        end
    end
endfunction

