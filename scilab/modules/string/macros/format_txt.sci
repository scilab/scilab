// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [txt,space]=format_txt(txt,instr,bprettyprintformat,nextinstr)
    // Add EOL and returns spaces to have good indentation (called by tree2code)
    // Inputs:
    // - txt: code to format
    // - instr: instruction to evaluate as a possible EOL
    // - bprettyprintformat = %T -> code generated is formated %F -> code generated has same format as original code
    // - nextinstr: next instruction after instr (format EOL between instructions)
    // Outputs:
    // - txt: updated txt
    // - space: indentation margin for control instructions

    rhs=argn(2)

    // Indentation and EOL in control clauses
    if rhs==3 then
        if instr<>list("EOL") & bprettyprintformat then
            txt = cat_code(txt,"")
            space="  "
        elseif instr<>list("EOL") then
            space=" "
        else
            space="  "
        end
        // EOL between instructions
    else
        if bprettyprintformat & instr<>list("EOL") & nextinstr<>list("EOL") then
            txt = cat_code(txt,"")
        end
    end
endfunction
