// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_fclose(tree)
// M2SCI function
// Conversion function for Matlab fclose()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

fid=getrhs(tree)

tree.name="mclose"

if fid.vtype==Unknown then
  set_infos(gettext("Unknown type for input parameter: consider fclose(fid)."),2)
end

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)

endfunction
