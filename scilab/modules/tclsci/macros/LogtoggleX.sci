// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function LogtoggleX( tog)
    global ged_handle; h=ged_handle

    h.log_flags=tog+part(h.log_flags,2);


    //tst=execstr("global h;h.log_flags=tog+part(h.log_flags,2)",'errcatch','n');

    //if tst<>0 then
    //   disp 'Warning: X bounds must be strictly positive'
    //end
endfunction

