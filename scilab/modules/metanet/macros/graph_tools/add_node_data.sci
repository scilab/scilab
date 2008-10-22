
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g=add_node_data(g,name,value)
  F=getfield(1,g.nodes.data)
  if and(F<>name) then 
    data=g.nodes.data
    setfield(1,[F name],data)
    g.nodes.data=data
  end
  if argn(2)==3 then 
    if size(value,'*')<>g.nodes.number then
      error('the third argument should be a vector with '+ ...
	    string(g.nodes.number)+' elements')
    end
    g.nodes.data(name)=matrix(value,1,-1)
  end
endfunction
