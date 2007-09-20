function scs_m=scicos_diagram(v1,v2,v3)
// Copyright INRIA
  if exists('props','local')==0 then props=scicos_params(),end
  if exists('objs','local')==0 then objs=list(),end
  if exists('version','local')==0 then version='',end
    
  scs_m=mlist(['diagram','props','objs','version'],props,objs,version)
endfunction
