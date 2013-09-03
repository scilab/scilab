//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Bernard DUJARDIN <develop.dujardin@numericable.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//

loadXcosLibs // Useless in Xcos block programmming context

ok = %f;

while ~ok do
    // Input the parameter
    [Datatype] = x_mdialog( ..
    "Parameter input", ..
    "Data Type (3:int32, 4:int16, 5:int8, ...) ?", ..
    "3" ..
    );

    if isempty(Datatype) then // Cancel or Ok ?
        break;

    else
        Datatype = evstr(Datatype);

    end
    // Test 'Data Type' parameter and displays error box if necessary
    if Datatype < 3 | Datatype > 8 then
        block_parameter_error( ..
        msprintf("Wrong values for ''Data Type'' parameter: %d.", Datatype), ..
        msprintf("Must be in the interval %s.", "[3, 8]") ..
        );
        ok = %f;

    else // Parameter is valid, continue the job
        ok = %t;
        print(%io(2), "Data Type parameter is valid");
    end
end

