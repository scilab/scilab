function  Ids=ge_get_arcs_id(sel)
//Copyright INRIA
//Author : Serge Steer 2002

  execstr('ArcId=EGdata_'+string(win)+'.ArcId')
  Ids=[]
  select ArcId
    case 1 then 
      if size(sel,1)==-1 then 
	Ids=string(1:size(GraphList.tail(sel),'*'))
      else
	Ids=string(sel)
      end
    case 2 then Ids=GraphList.edge_name(sel)
    case 3 then Ids=string(GraphList.edge_cost(sel))
    case 4 then Ids=string(GraphList.edge_min_cap(sel))
    case 5 then Ids=string(GraphList.edge_max_cap(sel))
    case 6 then Ids=string(GraphList.edge_length(sel))
    case 7 then Ids=string(GraphList.edge_q_weight(sel))
    case 8 then Ids=string(GraphList.edge_q_orig(sel))
    case 9 then Ids=string(GraphList.edge_weight(sel))
    case 10 then Ids=string(GraphList.edge_label(sel))
  end
endfunction
