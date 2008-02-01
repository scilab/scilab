function ge_do_circuit(GraphList)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
  [p,r] = circuit(GraphList)
  if p==[] then message('The graph has no circuit"),return,end
  show_arcs(p,'sup')
endfunction
