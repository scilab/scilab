function blk=scicos_block(v1,v2,v3,v4,v5)
// Copyright INRIA
//Block data structure initialization
  if exists('graphics','local')==0 then graphics=scicos_graphics(),end
  if exists('model','local')==0 then model=scicos_model(),end
  if exists('gui','local')==0 then gui='',end
  if exists('doc','local')==0 then doc=list(),end
  
  blk=mlist(['Block','graphics','model','gui','doc'],graphics,model,gui,doc)
endfunction
