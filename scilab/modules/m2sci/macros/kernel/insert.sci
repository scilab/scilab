// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function []=insert(instr,opt)

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
