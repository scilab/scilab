function GraphList=ge_delete_node(GraphList,K)
//Copyright INRIA
//Author : Serge Steer 2002

  if K==[] then return,end

  I=1:size(GraphList.node_x,'*')
  for f=ge_node_fields()
    GraphList(f)(K)= [];
  end
  //renumber arcs head and tail

  //   kept=1:size(GraphList.node_x,'*');kept(K)==[]
  //   GraphList.tail=I(dsearch(GraphList.tail,kept,'d'))
  //   GraphList.head=I(dsearch(GraphList.head,kept,'d'))

  for k=1:size(K,'*')
    I(K(k)+1:$)=I(K(k)+1:$)-1
  end
  GraphList.head=I(GraphList.head)
  GraphList.tail=I(GraphList.tail)
endfunction
