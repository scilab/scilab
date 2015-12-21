// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function h_out_list=Get_handles_list(h);

    global ged_handle_out;

    f=getparfig(h);
    ged_handle_out=[f];

    List_handles(f);

    //disp(ged_handle_out);

    h_out_list=ged_handle_out;

endfunction

