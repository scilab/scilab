function []=insert(instr,opt)
// Copyright INRIA

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
