// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [instr_list]=update_instr_list(instr_list,instr)
// M2SCI function

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
