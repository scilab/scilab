// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function setTicksTList(XYZ,locations,labels)
    global ged_handle;h= ged_handle;

    TL=tlist(["ticks","locations","labels"],locations,labels);

    if XYZ=="X"
        h.x_ticks=TL;
    elseif XYZ=="Y"
        h.y_ticks=TL;
    elseif XYZ=="Z"
        h.z_ticks=TL;
    end

endfunction
