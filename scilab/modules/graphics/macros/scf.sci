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

function [f]=scf(h)
    //set default figure, h may be a handle or a figure_id
    // (small correction to set up win number 0 when no
    //  gr window is opened, Bruno 22 nov 2004)
    if argn(2)<1 then
        num=winsid()
        if num == [] then
            h = 0
        else
            h = max(num)+1
        end
    end
    set("current_figure",h);
    f=get("current_figure");
endfunction
