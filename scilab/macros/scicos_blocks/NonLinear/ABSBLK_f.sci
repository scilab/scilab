function [x,y,typ]=ABSBLK_f(job,arg1,arg2)
//Absolute value block GUI.
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
  case 'define' then
  model=scicos_model()
  model.sim=list('absblk',1)
  model.in=-1
  model.out=-1
  model.blocktype='c'
  model.dep_ut=[%t %f]
  gr_i='xstringb(orig(1),orig(2),''Abs'',sz(1),sz(2),''fill'')'
  x=standard_define([2 2],model,[],gr_i)
end
endfunction
