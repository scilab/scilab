function [x,y,typ]=TANBLK_f(job,arg1,arg2)
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
  x.model.firing=[]
case 'define' then
  in=-1
  model=scicos_model()
  model.sim='tanblk'
  model.in=in
  model.out=in
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=sci2exp(in)
  gr_i=['xstringb(orig(1),orig(2),''tan'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
