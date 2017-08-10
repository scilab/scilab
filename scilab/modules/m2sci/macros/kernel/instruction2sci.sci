// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
