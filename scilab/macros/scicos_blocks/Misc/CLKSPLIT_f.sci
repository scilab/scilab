function [x,y,typ]=CLKSPLIT_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[],typ=[];
select job
case 'plot' then
case 'getinputs' then
  orig=arg1.graphics.orig;
  x=orig(1)
  y=orig(2)
  typ=-ones(x)
case 'getoutputs' then
  orig=arg1.graphics.orig;
  x=[1,1]*orig(1)
  y=[1,1]*orig(2)
  typ=-ones(x)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
case 'define' then
  model=scicos_model()
  model.sim='split'
  model.evtin=1
  model.evtout=[1;1]
  model.blocktype='d'
  model.firing=[%f,%f,%f] //????
  model.dep_ut=[%f %f]

  x=standard_define([1 1]/3,model,[],[])
end
endfunction
