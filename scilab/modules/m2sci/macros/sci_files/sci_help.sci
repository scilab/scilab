// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_help(tree)
// M2SCI function
// Conversion function for Matlab help()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

if rhs==0 then
  tree.rhs=list()
else
  topic=getrhs(tree)
  k=strindex(topic.value,"/")
  if k<>[] & min(k)<>2 then // help toolbox/
    no_equiv(expression2code(tree));
  elseif topic.value=="syntax" then
    tree.rhs=Rhs_tlist("names")
  else
    // Nothing to do
  end
end

endfunction
