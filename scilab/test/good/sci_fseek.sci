function [tree]=sci_fseek(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab fseek()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

[fid,offset,origin]=getrhs(tree)

if typeof(origin)=="cste" then
  select origin.value
  case "bof" then
    flag="set"
  case "cof" then
    flag="cur"
  case "eof" then
    flag="end"
  case -1 then
    flag="set" // Should not happen because -1 is an operation...
  case 0 then
    flag="cur"
  case 1 then
    flag="end"
  else
    set_infos("Not enough information on "+expression2code(origin)+" to set the proper flag",1)
    flag=Funcall("fseek_origin",1,list(origin),list())
  end
else
  if typeof(origin)=="operation" then
    if origin.operator=="-" then
      if origin.operands==list(Cste(1)) then
	flag="set"
      end
    end
  else
    set_infos("Not enough information on "+expression2code(origin)+" to set the proper flag",1)
    flag=Funcall("fseek_origin",1,list(origin),list())
  end
end

tree.name="mseek"
tree.rhs=Rhs(offset,fid,flag)

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)

endfunction
