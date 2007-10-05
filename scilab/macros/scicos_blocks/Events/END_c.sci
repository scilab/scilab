function [x,y,typ]=END_c(job,arg1,arg2)
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
    [ok,tf,exprs]=getvalue('Set final simulation time',..
       ['Final simulation time'],list('vec',1),exprs)
    if ~ok then break,end
    if ok then
      graphics.exprs=exprs
      model.firing=tf
      x.graphics=graphics;
      x.model=model
      break
    end
  end
case 'define' then
  tf=100000000
  model=scicos_model()
  model.sim=list('scicosexit',4)
  model.evtin=1
  model.evtout=1
  model.firing=tf
  model.blocktype='d'
  model.dep_ut=[%f %f]

  exprs=string(tf)
  gr_i=['xstringb(orig(1),orig(2),'' END '',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
