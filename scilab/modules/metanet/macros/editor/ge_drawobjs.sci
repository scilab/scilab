
function ge_drawobjs(GraphList,nodesel,arcsel)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
  if exists('nodesel','local')==0 then nodesel=:,end
  if exists('arcsel','local')==0 then arcsel=:,end
  
  ge_drawtitle(GraphList.name)
  ge_drawarcs(arcsel)
  ge_drawnodes(nodesel)
endfunction

