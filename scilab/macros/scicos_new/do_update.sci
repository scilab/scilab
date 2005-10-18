function [%cpr,%state0,needcompile,alreadyran,ok]=do_update(%cpr,%state0,needcompile)
//Update an already compiled scicos diagram compilation result according to 
//parameter changes
//!
// Copyright INRIA
ok=%t 
select needcompile
 case 0 then  // only parameter changes 
  if size(newparameters)<>0 then
    cor=%cpr.cor
    [%state0,state,sim]=modipar(newparameters,%state0,%cpr.state,%cpr.sim)
    %cpr.state=state,%cpr.sim=sim
  end
 case 1 then // parameter changes and/or port sizes change
  if size(newparameters)<>0 then
    // update parameters or states
    cor=%cpr.cor
    [%state0,state,sim]=modipar(newparameters,%state0,%cpr.state,%cpr.sim)
    %cpr.state=state,%cpr.sim=sim
  end
  //update port sizes.
  // NB: if modelica part block size has been changed, the diagram is recompiled
  bllst=list();
  corinv=%cpr.corinv
  sim=%cpr.sim
  for k=1:size(corinv)
    if type(corinv(k))==1 then //dont take care of modelica blocks 
      if size(corinv(k),'*')==1 then
	bllst(k)=scs_m.objs(corinv(k)).model;
      else
	path=list('objs');
	for l=corinv(k)(1:$-1),
	  path($+1)=l;
	  path($+1)='model';
	  path($+1)='rpar';path($+1)='objs';
	end
	path($+1)=corinv(k)($);
	path($+1)='model';
	bllst(k)=scs_m(path);
      end
    else // modelica block
      //build a fake bllst(k) only for in and out fields
      m=scicos_model();
      m.in=ones(1,sim.inpptr(k+1)-sim.inpptr(k))
      m.out=ones(1,sim.outptr(k+1)-sim.outptr(k))
      bllst(k)=m;
    end
  end
  [ok,bllst]=adjust(bllst,sim('inpptr'),sim('outptr'),sim('inplnk'),..
		    sim('outlnk'))
  if ok then
    %cpr.sim('lnkptr')=lnkptrcomp(bllst,sim('inpptr'),sim('outptr'),..
				  sim('inplnk'),sim('outlnk'))
    %cpr.state('outtb')=0*ones(%cpr.sim('lnkptr')($)-1,1)
    %state0('outtb')=0*ones(%cpr.sim('lnkptr')($)-1,1)
    needcompile=0
  end
 
 case 2 then // partial recompilation
  alreadyran=do_terminate()
  [%cpr,ok]=c_pass3(scs_m,%cpr)
  %state0=%cpr.state
  if ~ok then return,end
  needcompile=0
 case 4 then  // full compilation
  alreadyran=do_terminate()
  [%cpr,ok]=do_compile(scs_m)
  if ok then
    %state0=%cpr.state
    needcompile=0
  end
end
endfunction
