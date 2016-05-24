// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function datatipSetOrientation(tip_handle,o)
    if argn(2)<>2 then
        error(msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),...
        "datatipSetOrientation",2))
    end
    orientations=["automatic" "upper left" "upper right", "lower left","lower right"]
    if type(tip_handle)<>9|size(tip_handle,"*")<>1|or(tip_handle.type<>"Datatip") then
        error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),...
        "datatipSetOrientation",1,"Datatip"))
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
