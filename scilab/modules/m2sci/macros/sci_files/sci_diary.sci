// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_diary(tree)
// M2SCI function
// Conversion function for Matlab diary()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

// diary toggle
if rhs<=0 then
  no_equiv("diary toggle")
else
  str=getrhs(tree)
  if typeof(str)=="variable" then
    str=str.name
  elseif typeof(str)=="cste" then    
    str=str.value
  end
  if convstr(str,"l")=="""off""" then // diary off
    tree.rhs=Cste(0)
  elseif convstr(str,"l")=="""on""" then // diary on
    no_equiv("diary on")
  else // diary(filename)
    // Nothing to do
  end
end
endfunction
