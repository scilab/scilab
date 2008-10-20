function [x,y,typ]=EVTVARDLY(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
 case 'plot' then
  standard_draw(arg1)
case 'getinputs' then 
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,fir,exprs]=getvalue('Set parameter of variable event delay',..
			    'Initial event firing time (<0 if absent)',..
			    list('vec',1),exprs)
    if ~ok then break,end
    graphics.exprs=exprs
    model.firing=fir
    x.graphics=graphics;x.model=model
    break
  end
case 'define' then
  model=scicos_model()
  model.sim=list('evtvardly',4)
  model.in=1
  model.evtin=1
  model.evtout=1
  model.blocktype='d'
  model.firing=-1
  model.blocktype='c'
  model.dep_ut=[%t %f]
  exprs=string(model.firing)
  gr_i=['xstringb(orig(1),orig(2),[''Event'';''Delay''],sz(1),sz(2),''fill'');']
  x=standard_define([1.5 2],model,exprs,gr_i)
end
endfunction
