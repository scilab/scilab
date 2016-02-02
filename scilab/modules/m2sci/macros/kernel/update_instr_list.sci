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

function [instr_list]=update_instr_list(instr_list,instr)
    // M2SCI function

    // Global variables for M2SCI
    global("m2sci_to_insert_b")
    if isempty(m2sci_to_insert_b)
        m2sci_to_insert_b = list();
    end
    global("m2sci_to_insert_a")
    if isempty(m2sci_to_insert_a)
        m2sci_to_insert_a = list();
    end

    // Get instructions that must be inserted before converted instruction
    for k=1:lstsize(m2sci_to_insert_b)
        if m2sci_to_insert_b(k)<>list() then
            instr_list($+1)=m2sci_to_insert_b(k)
        end
    end
    m2sci_to_insert_b=list()

    // Insert converted instruction
    // instr can be an empty list if a call to 'm2scideclare' or convertion made in m2sci_to_insert_b
    if instr<>list() then
        instr_list($+1)=instr
    end

    // Get instructions that must be inserted after converted instruction
    for k=1:lstsize(m2sci_to_insert_a)
        if m2sci_to_insert_a(k)<>list() then
            instr_list($+1)=m2sci_to_insert_a(k)
        end
    end
    m2sci_to_insert_a=list()

endfunction
