// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


//function h=Get_handle_from_index(index)
function Get_handle_from_index(index)
    global ged_handle;
    global ged_cur_fig_handle

    hl = Get_handles_list(ged_cur_fig_handle);

    ged_handle = hl(index);
    //   h=ged_handle;
    tkged();

endfunction
