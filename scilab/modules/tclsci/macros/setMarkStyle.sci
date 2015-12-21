// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function setMarkStyle(sty)
    global ged_handle; h=ged_handle
    h.mark_style=find(sty==["dot" "plus" "cross" "star" "filled diamond" ..
    "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
    "asterisk" "square" "triangle right" "triangle left" "pentagram"])-1
endfunction
