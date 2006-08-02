function [tree]=sci_grid(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab grid()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_grid()
// V.C.

// [1 1]
on=Operation("rc",list(Cste(1),Cste(1)),list())
// [-1 -1]
off=Operation("rc",list(Cste(-1),Cste(-1)),list())
if rhs==0 then // grid toggle
  tree.name="mtlb_grid";
elseif rhs==1 then
  ax=Funcall("gca",1,list(),list())
  opt=tree.rhs(1)
  if typeof(opt)=="cste" then
    if opt.value=="on" then
      tree.name="set";
      tree.rhs=Rhs(ax,"grid",on);
    elseif opt.value=="off" then
      tree.name="set";
      tree.rhs=Rhs(ax,"grid",off);
    else
      set_infos("No minor grid in Scilab",2);
      tree.name="mtlb_grid";
    end
  else
    tree.name="mtlb_grid";
  end
else
  ax=tree.rhs(1)
  opt=tree.rhs(2)
  if typeof(opt)=="cste" then
    if opt.value=="on" then
      insert(Equal(list(Operation("ins",list(ax,Cste("grid")),list())),on))
      tree=list()
    elseif opt.value=="off" then
      insert(Equal(list(Operation("ins",list(ax,Cste("grid")),list())),off))
      tree=list()
    else
      set_infos("No minor grid in Scilab",2);
      tree.name="mtlb_grid";
    end
  else
    tree.name="mtlb_grid";
  end
end
endfunction
