//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Bernard DUJARDIN <develop.dujardin@numericable.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
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

