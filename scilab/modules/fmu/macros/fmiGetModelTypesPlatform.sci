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

//The type of platform which FMU was compiled for.

function platform = fmiGetModelTypesPlatform(model)
    // Check number of arguments
    if argn(2) <> 1 then
       error(msprintf(_('%s: Wrong number of input arguments: %d expected.\n'),'fmiGetModelTypesPlatform',1));
    end
    // Check type of arguments
    if typeof(model) <> "FMU" then
        error(999, msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiGetModelTypesPlatform", 1, "FMU"));
    end
    // Call the function from library 
    platform = fmu_call(model.simulationLibrary, "fmiGetModelTypesPlatform");    
endfunction


