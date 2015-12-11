// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function datatipSetOrientation(tip_handle,o)
    if argn(2)<>2 then
        error(msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),...
        "datatipSetOrientation",2))
    end
    orientations=["automatic" "upper left" "upper right", "lower left","lower right"]
    if type(tip_handle)<>9|size(tip_handle,"*")<>1|or(tip_handle.type<>"Compound") then
        error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),...
        "datatipSetOrientation",1,"datatip"))
    end

    if o == "upper left" then
        tip_handle.orientation = 0;
    elseif o == "upper right" then
        tip_handle.orientation = 1;
    elseif o == "lower left" then
        tip_handle.orientation = 2;
    elseif o == "lower right" then
        tip_handle.orientation = 3;
    end
endfunction
