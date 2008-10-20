function [x,y,typ]=INTEGRAL_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
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
  x=arg1
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
   [ok,x0,exprs]=getvalue('Set continuous linear system parameters',..
	'Initial state',list('vec',1),exprs)


    if ~ok then break,end
    graphics.exprs=exprs;
    model.state=x0;
    x.graphics=graphics;x.model=model
    break
  end
  x.model.firing=[] //compatibility
case 'define' then
  x0=0
  model=scicos_model()
  model.sim='integr'
  model.in=1
  model.out=1
  model.state=x0
  model.blocktype='c'
  model.dep_ut=[%f %t]
  
  exprs=strcat(sci2exp(x0))
  gr_i=['xstringb(orig(1),orig(2),''  1/s  '',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
