function [tree]=sci_diary(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab diary()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// diary toggle
if rhs<=0 then
  no_equiv("diary toggle")
else
  str=getrhs(tree)
  if typeof(str)=="variable" then
    str=str.name
  elseif typeof(str)=="cste" then    
    str=str.value
  end
  if convstr(str,"l")=="""off""" then // diary off
    tree.rhs=Cste(0)
  elseif convstr(str,"l")=="""on""" then // diary on
    no_equiv("diary on")
  else // diary(filename)
    // Nothing to do
  end
end
endfunction
