function k=ge_getedge(GraphList,pt)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
  ge_axes_handle=gca();
  gindex=ge_axes_handle.user_data
  hedges=gindex.edge;
  found=%f
  for k=1:size(hedges,'*')
    ak=get(hedges(k),'children');ak=ak(3)
    xy=get(ak,'data');//path
    if get(ak,'type')=='Polyline' then
      if ge_dist2polyline(xy(:,1),xy(:,2),pt) < 10 then return,end
    else
      if ge_dist2edge(xy,pt) < 10 then return,end
    end
  end
  k=[]
endfunction


