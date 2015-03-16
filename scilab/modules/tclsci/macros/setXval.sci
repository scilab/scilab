// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Rectangle data
function setXval(val)
    global ged_handle; h=ged_handle
    tmp=h.data;
    tmp(1)=val;
    tst=execstr("h.data=tmp","errcatch","n");
    if tst<>0 then
        disp "Warning: Y data must contain double"
    end
endfunction
