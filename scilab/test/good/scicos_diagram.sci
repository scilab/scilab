function scs_m=scicos_diagram(v1,v2)
// Copyright INRIA
  if exists('props','local')==0 then props=scicos_params(),end
  if exists('objs','local')==0 then objs=list(),end
    
  scs_m=mlist(['diagram','props','objs'],props,objs)
endfunction
