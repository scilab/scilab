function [x,y,typ]=CLKSPLIT_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[],typ=[];
select job
case 'plot' then
  graphics=arg1(2); [orig,sz,orient]=graphics(1:3)
  sz=10*sz;
  p=1 //pixel sizes ratio
  rx=sz(1)*p/2
  ry=sz(2)/2
  pat=xget('pattern');xset('pattern',default_color(-1))
  xfarc(orig(1)-rx,orig(2)+ry,sz(1)*p,sz(2),0,360*64)
  xset('pattern',pat)
case 'getinputs' then
  graphics=arg1(2); orig=graphics(1)
  x=orig(1)
  y=orig(2)
  typ=-ones(x)
case 'getoutputs' then
  graphics=arg1(2); orig=graphics(1)
  x=[1,1]*orig(1)
  y=[1,1]*orig(2)
  typ=-ones(x)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
case 'define' then
  model=list('split',[],[],1,[1;1],[],[],[],[],'d',[%f,%f,%f],[%f %f],' ',list())
  x=standard_define([1 1]/3,model,[],[])
end




