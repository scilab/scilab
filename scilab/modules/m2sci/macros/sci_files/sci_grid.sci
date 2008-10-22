// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_grid(tree)
// M2SCI function
// Conversion function for Matlab grid()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_grid()

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
      tree.rhs=Rhs_tlist(ax,"grid",on);
    elseif opt.value=="off" then
      tree.name="set";
      tree.rhs=Rhs_tlist(ax,"grid",off);
    else
      set_infos(gettext("No minor grid in Scilab."),2);
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
      set_infos(gettext("No minor grid in Scilab."),2);
      tree.name="mtlb_grid";
    end
  else
    tree.name="mtlb_grid";
  end
end
endfunction
