function [converted_instr,nblines]=instruction2sci(mtlb_instr,nblines)
// Copyright INRIA
// V.C.

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
  error("instruction2sci: unknown instruction type "+typeof(mtlb_instr))
end

endfunction
