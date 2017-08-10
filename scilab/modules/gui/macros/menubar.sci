// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function menubar(win,menus)

    if argn(2) < 2 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "menubar", 2));
    end

    names = menus(1)
    for k=size(names,"*"):-1:1
        delmenu(win, names(k))
    end

    for k=1:size(names,"*")
        addmenu(win, names(k), menus(k+1), list(0, names(k)))
    end

endfunction
