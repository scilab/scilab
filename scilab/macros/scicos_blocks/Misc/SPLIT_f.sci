function [x,y,typ]=SPLIT_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[],typ=[];

select job
case 'plot' then
  graphics=arg1(2); [orig,sz,orient]=graphics(1:3)
  thick=xget('thickness');xset('thickness',2)
  pat=xget('pattern');xset('pattern',default_color(1))
  sz=10*sz;
  p=1 //pixel sizes ratio
  rx=sz(1)*p/2
  ry=sz(2)/2
  xfarc(orig(1)-rx,orig(2)+ry,sz(1)*p,sz(2),0,360*64)
  xset('thickness',thick)
  xset('pattern',pat)
case 'getinputs' then
  graphics=arg1(2); orig=graphics(1)
  x=orig(1)
  y=orig(2)
  typ=ones(x)
case 'getoutputs' then
  graphics=arg1(2); orig=graphics(1)
  x=[1 1]*orig(1)
  y=[1 1]*orig(2)
  typ=ones(x)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  x(3)(11)=[] //compatibility
case 'define' then
  model=list('lsplit',-1,[-1;-1;-1],[],[],[],[],[],[],'c',[],[%t %f],' ',list())
  x=standard_define([1 1]/3,model,[],[])
end




