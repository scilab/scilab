function [x,y,typ]=EDGETRIGGER(job,arg1,arg2)
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
    [ok,edge,exprs]=getvalue('Set edge trigger block parameters',..
	['rising (1), falling (-1), both (0)'],list('vec',1),exprs)
    if ~ok then break,end
    model.ipar=sign(edge)
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
    break
  end
 case 'define' then
  edge=1
  model=scicos_model()
  model.sim=list('edgetrig',4)
  model.in=1
  model.out=1
  model.dstate=0
  model.nzcross=1
  model.ipar=sign(edge)
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[string(edge)]
  gr_i=['xstringb(orig(1),orig(2),[''Edge'';''trigger''],sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,exprs,gr_i)
end
endfunction
