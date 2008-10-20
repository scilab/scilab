function [txt,space]=format_txt(txt,instr,prettyprint,nextinstr)
// Copyright INRIA
// Scilab Project - V. Couvert
// Add EOL and returns spaces to have good indentation (called by tree2code)
// Inputs:
// - txt: code to format
// - instr: instruction to evaluate as a possible EOL
// - prettyprint = %T -> code generated is formated %F -> code generated has same format as original code
// - nextinstr: next instruction after instr (format EOL between instructions)
// Outputs:
// - txt: updated txt
// - space: indentation margin for control instructions

rhs=argn(2)

// Indentation and EOL in control clauses
if rhs==3 then
  if instr<>list("EOL") & prettyprint then
    txt = cat_code(txt,"")
    space="  "
  elseif instr<>list("EOL") then
    space=" "
  else
    space="  "
  end
// EOL between instructions
else
  if prettyprint & instr<>list("EOL") & nextinstr<>list("EOL") then
    txt = cat_code(txt,"")
  end
end
endfunction
