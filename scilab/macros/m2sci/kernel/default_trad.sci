function tree=default_trad(tree)
// Copyright INRIA
// M2SCI function
// Create a default translation function
// V.C.

set_infos("Unknown function "+tree.name+", original calling sequence used",2)

// If Matlab variable name is a Scilab function name
if funptr(tree.name)<>0 then
  name1="%"+tree.name
  set_infos("Name conflict: function name changed from "+tree.name+" to "+name1,0)
  tree.name=name1
end
[tree]=sci_generic(tree)
endfunction
