function [x,y,typ]=EVTGEN_f(job,arg1,arg2)
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
    [ok,tt,exprs]=getvalue('Set Event time',..
	['Event Time'],list('vec',1),exprs)
    if ~ok then break,end
    graphics.exprs=exprs
    if model.firing<>tt then
      model.firing=tt
    end
    x.graphics=graphics;x.model=model
    break
  end
case 'define' then
  tt=0
  model=scicos_model()
  model.sim='trash'
  model.evtout=1
  model.blocktype='d'
  model.firing=tt
  model.dep_ut=[%f %f]
  
  exprs=string(tt)
  gr_i=['tt=model.firing;';
    'txt=[''Event at'';''time ''+string(tt)];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
