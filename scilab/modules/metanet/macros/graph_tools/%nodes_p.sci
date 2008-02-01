function %nodes_p(nodes)
//Overloads node  data structure display
//Author: Serge Steer; Copyright INRIA


 
  w=format_struct(nodes.graphics,['name','type','x','y'],['names','types','x','y'])+..
    format_struct(nodes.data)+'|'
  mprintf("  %s\n",w)
endfunction
