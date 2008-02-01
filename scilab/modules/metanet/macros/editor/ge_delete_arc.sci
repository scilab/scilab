function GraphList=ge_delete_arc(GraphList,K)
//Copyright INRIA
//Author : Serge Steer 2002
  for f=ge_arc_fields()
    GraphList(f)(K)=[]
  end
endfunction
