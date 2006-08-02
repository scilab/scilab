function [instr_list]=update_instr_list(instr_list,instr)
// Copyright INRIA
// M2SCI function
// V.C.

// Global variables for M2SCI
global("m2sci_to_insert_b")
global("m2sci_to_insert_a")

// Get instructions that must be inserted before converted instruction
for k=1:size(m2sci_to_insert_b)
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
for k=1:size(m2sci_to_insert_a)
  if m2sci_to_insert_a(k)<>list() then
    instr_list($+1)=m2sci_to_insert_a(k)
  end
end
m2sci_to_insert_a=list()

endfunction
