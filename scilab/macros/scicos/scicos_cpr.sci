function cpr=scicos_cpr(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10)
// Copyright INRIA
//initialisation de cpr tlist
  if exists('state','local')==0 then state=scicos_state(),end
  if exists('sim','local')==0 then sim=scicos_sim(),end
  if exists('cor','local')==0 then cor=list(),end
  if exists('corinv','local')==0 then corinv=list(),end
  cpr=tlist(['cpr','state','sim','cor','corinv'],..
		 state,sim,cor,corinv)
endfunction
