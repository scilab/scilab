// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function x = %_iconvert(a,typeToConvert)

    if typeof(a) == "constant" & isreal(a) == %f then
        error(msprintf(_("%s: Wrong type for argument #%d: Real matrix expected.\n"), "iconvert", 1));
    end

    if floor(typeToConvert) <> typeToConvert then
        error(msprintf(_("%s: Wrong value for input argument #%d: An integer value expected.\n"), "iconvert", 2));
    end

    if typeof(typeToConvert) <> "constant" | size(typeToConvert, "*") <> 1 then
        error(msprintf(_("%s: Wrong size for argument #%d: Real scalar expected.\n"), "iconvert", 2));
    end

    if typeToConvert == 0 then
        typeToConvert = 19;
    end

    castFunction(1)  = {int8};
    castFunction(2)  = {int16};
    castFunction(4)  = {int32};
    castFunction(8)  = {int64};
    castFunction(11) = {uint8};
    castFunction(12) = {uint16};
    castFunction(14) = {uint32};
    castFunction(18) = {uint64};
    castFunction(19) = {double};

    if or(typeToConvert == [1,2,4,8,11,12,14,18,19]) == %f then
        error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "iconvert", 2, "0, 1, 2, 4, 8, 11, 12, 14, 18"));
    end
    castResult = castFunction{typeToConvert};
    x = castResult(a);

endfunction
