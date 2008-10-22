
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [Id,loc]=ge_get_node_id(n)
  Id=[]
  NodeId=GraphList.nodes.graphics.display;
  select NodeId
   case 'nothing' then 
    Id=[]
   case 'number' then
    Id=string(n)
   case 'name' then
    Id=GraphList.nodes.graphics.name(n)
  else
    data=GraphList.nodes.data(NodeId)
    if type(data)==15 then
      val=data(n)
    else
      val=data(:,n)
    end
    Id=string(val)
    if size(Id,'*')>1 then Id='['+strcat(Id,',')+']',end
  end
  loc='center'
endfunction
