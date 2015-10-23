// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function CloseEditorSaveData()
    // Called when closing data editor

    global ged_handle; // To leave here because used when doing execstr(ged_tmp_string...
    global ged_tmp;
    global ged_tmp_string;
    global WINDOW;

    Nan = %nan // to avoid error message because of special Nan display

    ged_tmp=GEDeditvar_get(WINDOW);

    execstr(ged_tmp_string+"= ged_tmp");

    clearglobal ged_tmp ged_tmp_string WINDOW
    clear ged_tmp ged_tmp_string WINDOW

endfunction
