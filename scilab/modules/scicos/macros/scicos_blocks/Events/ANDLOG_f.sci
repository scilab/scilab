function [x,y,typ]=ANDLOG_f(job,arg1,arg2)
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
  x=arg1;
case 'define' then
  model=scicos_model()
  model.sim='andlog'
  model.out=1
  model.evtin=[1;1]
  model.blocktype='d'
  model.firing=[]
  model.dep_ut=[%f %f]
  
  gr_i=['txt=[''LOGICAL'';'' '';'' AND ''];';
	'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([3 3],model,[],gr_i)
end
endfunction
