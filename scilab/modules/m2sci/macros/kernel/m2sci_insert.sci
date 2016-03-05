// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=m2sci_insert(instr,opt)
    // Global variables for M2SCI
    global("m2sci_to_insert_b")
    global("m2sci_to_insert_a")

    [lhs,rhs]=argn()

    if rhs==1 then
        opt=0 // Insertion before current instruction
    end
    if opt<>1 then
        m2sci_to_insert_b($+1)=instr
    else
        m2sci_to_insert_a($+1)=instr
    end

endfunction
