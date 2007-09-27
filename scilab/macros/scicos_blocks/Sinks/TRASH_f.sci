function [x,y,typ]=TRASH_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  x=[];y=[];typ=[];
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
case 'define' then
  in=-1

  model=scicos_model()
  model.sim='trash'
  model.in=in
  model.evtin=1
  model.blocktype='d'
  model.dep_ut=[%f %f]
  
  exprs=' '
  gr_i=['xstringb(orig(1),orig(2),''Trash'',sz(1),sz(2),''fill'')']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
