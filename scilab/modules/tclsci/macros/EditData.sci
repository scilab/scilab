// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function EditData(TheData,datastring)
    // TheData must be a real scalar or matrix
    global ged_handle; h=ged_handle

    // I declare ged_tmp ged_tmp_string WINDOW as global
    global ged_tmp;
    global ged_tmp_string;
    global WINDOW;

    ged_tmp_string = datastring;

    ged_tmp=TheData;

    WINDOW = GEDeditvar("ged_tmp")

endfunction
