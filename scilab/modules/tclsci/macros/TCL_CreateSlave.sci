// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function status = TCL_CreateSlave(slaveName, isSafe)
    // slaveName: name of the slave to create
    // isSafe: %T to create a safe slave, %F otherwise (default)

    // Input arguments tests
    [lhs, rhs] = argn();
    if rhs<>1 & rhs<>2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "TCL_CreateSlave", 1, 2));
    end

    if lhs<>1 then
        error(msprintf(gettext("%s: Wrong number of output argument(s): %d expected.\n"), "TCL_CreateSlave", 1));
    end

    if typeof(slaveName)<>"string" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A String expected.\n"), "TCL_CreateSlave", 1));
    end

    if rhs==1 then
        isSafe = %F; // Create an unsafe slave by default
    end

    if typeof(isSafe)<>"boolean" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A Boolean expected.\n"), "TCL_CreateSlave", 2));
    end

    if TCL_ExistInterp(slaveName) then
        error(msprintf(gettext("%s: Slave interpreter already exists.\n"),"TCL_CreateSlave"));
    end

    if isSafe then
        status = TCL_EvalStr("interp create -safe "+slaveName);
    else
        status = TCL_EvalStr("interp create "+slaveName);
    end

endfunction
