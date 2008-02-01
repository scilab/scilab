function GraphList=ge_delete_node(GraphList,K)
//Copyright INRIA
//Author : Serge Steer 2002

  if K==[] then return,end

  I=1:GraphList.nodes.number
  GraphList.nodes(K)=[]

  //renumber arcs head and tail

  for k=1:size(K,'*')
    I(K(k)+1:$)=I(K(k)+1:$)-1
  end
  GraphList.edges.head=I(GraphList.edges.head)
  GraphList.edges.tail=I(GraphList.edges.tail)
endfunction
