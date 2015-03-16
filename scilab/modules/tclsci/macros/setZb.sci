// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


//Fec
function setZb(min, max)
    global ged_handle; h=ged_handle
    tmp=h.z_bounds;
    tmp(1,1)=min;
    tmp(1,2)=max;
    tst=execstr("h.z_bounds=tmp","errcatch","n");
    if tst<>0 then
        disp "Warning: X data_bounds must contain double"
    end
endfunction
