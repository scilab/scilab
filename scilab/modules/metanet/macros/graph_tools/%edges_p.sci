function %edges_p(edges)
//Overloads edge  data structure display
//Author: Serge Steer; Copyright INRIA
 
  w='';
  if edges.graphics.name==[] then 
     na=size(edges.tail,2)
    edges.graphics.name=string(1:na),
  end
   
  w=format_struct(edges.graphics,'name','names')+..
    format_struct(edges,['tail','head'])+..
    format_struct(edges.data)+'|'

  mprintf("  %s\n",w)
endfunction

