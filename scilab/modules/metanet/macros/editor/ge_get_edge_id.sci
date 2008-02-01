function [Id,loc]=ge_get_edge_id(n)
//Copyright INRIA
//Author : Serge Steer 2002
  Id=[]
  ArcId=GraphList.edges.graphics.display;
  select ArcId
   case 'nothing' then 
    Id=[]
   case 'number' then
    Id=string(n)
   case 'name' then
    Id=GraphList.edges.graphics.name(n)
  else
    data=GraphList.edges.data(ArcId)
    if type(data)==15 then
      val=data(n)
    else
      val=data(:,n)
    end
    Id=string(val)
    if size(Id,'*')>1 then Id='['+strcat(Id,',')+']',end
  end
  loc='bottom'
endfunction
