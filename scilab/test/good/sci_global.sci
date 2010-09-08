// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_global(tree)
// M2SCI function
// Conversion function for Matlab global()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

// Global variable for M2SCI
global("varslist");

for k=1:rhs
  nam=tree.rhs(k).value
  if nam<>"" then
    exist=%F
    for l=1:size(varslist)
      if varslist(l).matname==nam then
	exist=%T
	break
      end
    end
    if exist==%F then
      varslist($+1)=M2scivar(nam,nam,Infer(list(Unknown,Unknown),Type(Unknown,Unknown)))
      write(%io(2),msprintf(gettext("Global variable %s added to M2SCI variable list."),nam))
    end
  end
end

endfunction
