// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function x = iconvert(a,typeToConvert)

    [lhs, rhs] = argn();

    if rhs <> 2 then
        error(msprintf(_("%s: Wrong number of input argument(s): %d expected."), "iconvert", 2));
    end

    if lhs <> 1 then
        error(msprintf(_("%s: Wrong number of output argument(s): %d expected."), "iconvert", 1));
    end

    if typeof(a) <> "constant" & type(a) <> 8 & typeof(a) <> "boolean" then
        error(msprintf(_("%s: Wrong type for input argument #%d: ''%s'', ''%s'' or ''%s'' expected."), "iconvert", 1, "integer", "boolean", "double"));
    end

    if typeof(typeToConvert) <> "constant" | size(typeToConvert, "*") <> 1 then
        error(msprintf(_("%s: Wrong type for argument %d: Real scalar expected."), "iconvert", 2));
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
        error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set  {%s}."), "iconvert", 2, "0, 1, 2, 4, 8, 11, 12, 14, 18"));
    end
    castResult = castFunction{typeToConvert};
    x = castResult(a);

endfunction
