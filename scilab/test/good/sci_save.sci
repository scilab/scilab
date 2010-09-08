// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_save(tree)
// M2SCI function
// Conversion function for Matlab save()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

tree.name="savematfile"

// If no rhs, save Scilab environnement
if rhs<=0 then
  tree.rhs=Rhs_tlist("scilab.mat")
  return
end

for k=1:rhs
  if typeof(tree.rhs(k))=="cste" & tree.rhs(k).value=="-regexp" then
    set_infos(gettext("Option -regexp not yet handled: will be ignored."),2);
  elseif typeof(tree.rhs(k))=="cste" & tree.rhs(k).value=="-append" then
    set_infos(gettext("Option -append not yet handled: will be ignored."),2);
  end
end

endfunction
