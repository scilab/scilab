// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


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
