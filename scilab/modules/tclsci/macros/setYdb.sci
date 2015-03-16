// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function setYdb(ymin, ymax)
    global ged_handle; h=ged_handle
    tmp=h.data_bounds;
    tmp(1,2)=ymin;
    tmp(2,2)=ymax;
    h.data_bounds=tmp;
    //  tst=execstr('h.data_bounds=tmp','errcatch','n');
    //  if tst<>0 then
    //   disp 'Warning: Y data_bounds must contain double'
    //  end
endfunction
