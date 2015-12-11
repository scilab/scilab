// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [converted_instr,nblines]=instruction2sci(mtlb_instr,nblines)

    // Convertion of empty lines
    if mtlb_instr==list("EOL") then
        converted_instr=list("EOL")
        nblines = nblines + 1;
        return
    end

    if typeof(mtlb_instr)=="equal" then
        [converted_instr]=equal2sci(mtlb_instr)
    elseif or(typeof(mtlb_instr)==["ifthenelse","selectcase","for","while","trycatch"]) then
        [converted_instr,nblines]=clause2sci(mtlb_instr,nblines)
    elseif or(typeof(mtlb_instr)==["funcall","cste","operation","variable"]) then
        [converted_instr]=expression2sci(mtlb_instr)
    elseif typeof(mtlb_instr)=="comment" then
        [converted_instr]=mtlb_instr

    else
        error(gettext("unknown instruction type %s."),typeof(mtlb_instr))
    end

endfunction
