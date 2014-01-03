// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
