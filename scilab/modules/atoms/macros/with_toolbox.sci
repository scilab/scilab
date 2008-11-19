// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// If a toolbox is registered, return %t and its path ; %f else

function [reg,path] = with_toolbox(name)
  reg = %f
  path = ""
  
  tboxes = getToolboxes()
  for i=1:size(tboxes, 1)
    if tboxes(i,1) == name then
	  reg = %t
	  path = tboxes(i,2)
	  return
	end
  end	
endfunction
