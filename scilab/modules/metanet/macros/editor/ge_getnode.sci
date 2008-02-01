function k=ge_getnode(pt)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
  ge_axes_handle=gca();
  gindex=ge_axes_handle.user_data;
  hnodes=gindex.node;
  x=pt(1);y=pt(2)
  nn=size(hnodes,'*');
  k=[]
  if nn>0 then
    for i=1:nn
      e=get(hnodes(i),'children');e=e(2)
      if get(e,'type')=='Compound' then 
	e=get(e,'children');e=e(2),
      end
      d=get(e,'data');d=d(1:4);
      if x>=d(1)&x<=d(1)+d(3)&y>=d(2)-d(4)&y<=d(2) then 
	k=i
	break
      end
    end
  end
  //  d=GraphList.nodes.graphics.diam;d(d==0)=GraphList.default_node_diam;
  //  k=find(sqrt((GraphList.nodes.graphics.x-x)^2+(GraphList.nodes.graphics.y-y)^2) <= d/2)
  if k<>[] then k=k(1),end
endfunction

