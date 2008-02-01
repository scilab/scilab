function show_arcs(p,sup,leg)
//Copyright INRIA
//Author : Serge Steer 2008
  if exists('leg','local')==0 then leg='nothing',end
  if exists('sup','local')==0 then sup='no',end
  show_edges(p,sup=sup,leg=leg)
endfunction

