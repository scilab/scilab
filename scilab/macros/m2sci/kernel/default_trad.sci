function tree=default_trad(tree)
// Copyright INRIA
// M2SCI function
// Create a default translation function
// V.C.

if or(tree.name==not_yet_converted()) then
  set_infos("Matlab function "+tree.name+" not yet converted, original calling sequence used",2)
elseif isdefinedvar(Variable(tree.name,Infer())) then
  operands=list()
  operands(1)=Variable(tree.name,Infer())
  for krhs=1:lstsize(tree.rhs)
    operands($+1)=tree.rhs(krhs)
  end
  tree=Operation("ext",operands,tree.lhs)
  tree=operation2sci(tree)
else

  set_infos("Unknown function "+tree.name+", original calling sequence used",2)
  
  // If Matlab variable name is a Scilab function name
  if funptr(tree.name)<>0 then
    name1="%"+tree.name
    set_infos("Name conflict: function name changed from "+tree.name+" to "+name1,0)
    tree.name=name1
  end
end
[tree]=sci_generic(tree)
endfunction
