function %graph_p(g)
//Overload graph display
//Author: Serge Steer; Copyright INRIA
  yn=['no','yes']
  txt=['graph name      : '+g.name
       'version         : '+g.version
       'oriented        : '+yn(g.directed+1)
       'number of nodes : '+string(node_number(g))
       'number of edges : '+string(edge_number(g))]

  mprintf("  %s\n",txt)
endfunction
