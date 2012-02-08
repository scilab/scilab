// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x = iconvert(a,typeToConvert)

    if typeof(a) <> "constant" & type(a) <> 8 then
        error("toto");
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

    castResult = castFunction{typeToConvert};
    x = castResult(a);

endfunction
