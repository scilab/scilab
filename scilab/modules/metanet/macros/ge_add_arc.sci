function GraphList=ge_add_arc(GraphList,head,tail)
//Copyright INRIA
//Author : Serge Steer 2002

  n=size(GraphList.tail,'*')+1
  GraphList.tail(1,n)=tail
  GraphList.head(1,n)=head
  GraphList.edge_name(1,n)=""
  GraphList.edge_color(1,n)=1
  GraphList.edge_width(1,n)=0
  GraphList.edge_hi_width(1,n)=0
  GraphList.edge_font_size(1,n)=0
  GraphList.edge_length(1,n)=0
  GraphList.edge_cost(1,n)=0
  GraphList.edge_min_cap(1,n)=0
  GraphList.edge_max_cap(1,n)=0
  GraphList.edge_q_weight(1,n)=0
  GraphList.edge_q_orig(1,n)=0
  GraphList.edge_weight(1,n)=0
  GraphList.edge_label(1,n)=''
  ge_add_history(list('add_arc',n))
endfunction
