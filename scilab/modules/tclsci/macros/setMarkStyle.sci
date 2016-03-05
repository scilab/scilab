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

function setMarkStyle(sty)
    global ged_handle; h=ged_handle
    h.mark_style=find(sty==["dot" "plus" "cross" "star" "filled diamond" ..
    "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
    "asterisk" "square" "triangle right" "triangle left" "pentagram"])-1
endfunction
