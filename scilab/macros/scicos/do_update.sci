function [%cpr,%state0,needcompile,ok]=do_update(%cpr,%state0,needcompile)
//Update an already compiled scicos diagram compilation result according to 
//parameter changes
//!
// Copyright INRIA
ok=%t 
select needcompile
case 0 then  // only parameter changes 
  if size(newparameters)<>0 then
    cor=%cpr(3)
    [%state0,state,sim]=modipar(newparameters,%state0,%cpr(1),%cpr(2))
    %cpr(1)=state,%cpr(2)=sim
  end
case 1 then // parameter changes and/or port sizes changes
  if size(newparameters)<>0 then
    // update parameters or states
    cor=%cpr(3)
    [%state0,state,sim]=modipar(newparameters,%state0,%cpr(1),%cpr(2))
    %cpr(1)=state,%cpr(2)=sim
  end
  //update port sizes
  bllst=list();
  corinv=%cpr(4)
  sim=%cpr(2)
  for k=1:size(corinv)
    if size(corinv(k),'*')==1 then
      bllst(k)=scs_m(corinv(k))(3);
    else
      path=list();
      for l=corinv(k)(1:$-1),path($+1)=l;path($+1)=3;path($+1)=8;end
      path($+1)=corinv(k)($);
      path($+1)=3;
      bllst(k)=scs_m(path);
    end
  end
  [ok,bllst]=adjust(bllst,sim('inpptr'),sim('outptr'),sim('inplnk'),..
      sim('outlnk'))
  if ok then
    %cpr(2)('lnkptr')=lnkptrcomp(bllst,sim('inpptr'),sim('outptr'),..
	sim('inplnk'),sim('outlnk'))
    needcompile=0
  end
case 2 then // partial recompilation
  [%cpr,ok]=c_pass3(scs_m,%cpr)
  %state0=%cpr(1)
  if ~ok then return,end
  needcompile=0
  alreadyran=%f
case 4 then  // full compilation
  needstart=%t
  [%cpr,ok]=do_compile(scs_m)
  if ok then
    %state0=%cpr(1)
    newparameters=list()
    %tcur=0 //temps courant de la simulation
    alreadyran=%f
    needcompile=0
  else
    alreadyran=%f
    %tcur=0
    //%state0=%cpr(1)
  end
end
