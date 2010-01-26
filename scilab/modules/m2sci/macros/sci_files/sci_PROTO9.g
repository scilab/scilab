// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_MFUN(tree)
// Prototype for *int**()
// M2SCI function
// Conversion function for Matlab MFUN()
// Input: tree = Matlab funcall tree
// Output: tree = Scilab equivalent for tree
// Emulation function: mtlb_MFUN()

// %c_MFUN and %b_MFUN are not defined in Scilab
x = getrhs(tree)

// Matlab MFUN() can return Boolean type value
if or(x.vtype==[Boolean,Unknown]) then
  vtype=Unknown
else
  vtype=Int
end

x = convert2double(x)

tree.rhs=Rhs_tlist(x)

if is_complex(x) then
  set_infos(msprintf(gettext("Scilab %s() does not work with Complex values: %s() call IGNORED."),"MFUN","MFUN"),2);
  if typeof(x)=="operation" then
    x.out=tree.lhs
  elseif typeof(x)=="funcall" then
    x.lhs=tree.lhs
  end
  tree=x
elseif ~is_real(x) then
  set_infos(msprintf(gettext("Scilab %s() does not work with Complex values: %s() call IGNORED."),"MFUN","MFUN"),2);
  repl_poss(x,tree,x,gettext("is Real."))
  if typeof(x)=="operation" then
    x.out=tree.lhs
  elseif typeof(x)=="funcall" then
    x.lhs=tree.lhs
  end
  tree=x
else
  tree.name="mtlb_MFUN"
  tree.lhs(1).dims=x.dims
  tree.lhs(1).type=x.type
end

endfunction
