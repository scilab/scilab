//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

//The version of model Interface, only 1.0 is implemented in Scilab.

function version = fmiGetVersion(model)
    // Check number of arguments
    if argn(2) <> 1 then
       error(msprintf(_('%s: Wrong number of input arguments: %d expected.\n'),'fmiGetVersion',1));
    end
    // Check type of arguments
    if typeof(model) <> "FMU" then
        error(999, msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiGetVersion", 1, "FMU"));
    end
    // Call the function from library
    version = fmu_call(model.simulationLibrary, "fmiGetVersion");
    if version == "2.0" then 
        error(10000, msprintf(_("%s: Not yet implemented in Scilab, 1.0 expected.\n"), "fmiGetVersion"));
    elseif version <> "1.0" then
        error(10000, msprintf(_("%s: Unknown version of FMU, 1.0 expected.\n"), "fmiGetVersion"));
    end
endfunction
