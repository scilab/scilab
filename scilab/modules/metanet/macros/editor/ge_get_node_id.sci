function [Id,loc]=ge_get_node_id(n)
//Copyright INRIA
//Author : Serge Steer 2002
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
