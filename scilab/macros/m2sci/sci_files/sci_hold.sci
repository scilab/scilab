function [tree]=sci_hold(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab hold()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_hold()
// V.C.

if rhs==0 then // hold toggle
  tree.name="mtlb_hold"
else
  opt=tree.rhs(1)
  if typeof(opt)=="cste" then
    if opt.value=="on" then
      // set(gca(),'auto_clear','off')
      gca_funcall=Funcall("gca",1,list(),list())
      tree.name="set"
      tree.rhs=Rhs(gca_funcall,Cste("auto_clear"),Cste("off"))
    elseif opt.value=="off" then
      // set(gca(),'auto_clear','on')
      gca_funcall=Funcall("gca",1,list(),list())
      tree.name="set"
      tree.rhs=Rhs(gca_funcall,Cste("auto_clear"),Cste("on"))
    else
      no_equivalent(opt.value+" option")
    end
  else
    tree.name="mtlb_hold"
  end
end

endfunction
