function [x,y,typ]=POSTONEG_f(job,arg1,arg2)
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
  x.model.firing=[-1] //compatibility
case 'define' then
  rpar=[-1;-1;-1;0]
  model=scicos_model()
  model.sim=list('zcross',1)
  model.nzcross=1
  model.in=1
  model.evtout=1
  model.rpar=[-1;-1;-1;0]
  model.blocktype='z'
  model.dep_ut=[%t %f]
  model.firing=[-1]
  gr_i=['xstringb(orig(1),orig(2),'' + to - '',sz(1),sz(2),''fill'')']
  x=standard_define([2 2],model,[],gr_i)
end
endfunction
