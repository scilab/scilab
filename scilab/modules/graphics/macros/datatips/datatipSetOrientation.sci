// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipSetOrientation(tip_handle,o)
    if o == "upper left" then
        tip_handle.tip_orientation = 0;
    elseif o == "upper right" then
        tip_handle.tip_orientation = 1;
    elseif o == "lower left" then
        tip_handle.tip_orientation = 2;
    elseif o == "lower right" then
        tip_handle.tip_orientation = 3;
    end
endfunction
