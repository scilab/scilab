function [x,y,typ]=PROD_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
p=1 //pixel sizes ratio
select job


case 'plot' then
  [orig,sz]=arg1(1:2)
  wd=xget('wdim')
  graphics=arg1(2); [orig,sz,orient]=graphics(1:3)
  thick=xget('thickness');xset('thickness',2)
  patt=xget('dashes');xset('dashes',default_color(1))
  rx=sz(1)*p/2
  ry=sz(2)/2
  xarc(orig(1),orig(2)+sz(2),sz(1)*p,sz(2),0,23040) // (23040=360*64)

  t=%pi/4
  xx=(orig(1)+rx)+..
      [sin(5*t) , sin(-t);
       sin(t) ,   sin(3*t)]*diag([rx;rx]/1.7)
  yy=(orig(2)+ry)+..
      [cos(5*t) , cos(-t);
       cos(t) ,   cos(3*t)]*diag([ry;ry]/1.7)
  xsegs(xx,yy,0)
  xset('thickness',1)
  if orient then  //standard orientation
    out= [0  -1/14
	1/7    0
	0   1/14]*3
    xfpoly(sz(1)*out(:,1)+ones(3,1)*(orig(1)+sz(1)*p),..
	sz(2)*out(:,2)+ones(3,1)*(orig(2)+sz(2)/2),1)
  else //tilded orientation
    out= [0   -1/14
	-1/7    0
	0   1/14]*3
    xfpoly(sz(1)*out(:,1)+ones(3,1)*orig(1),..
	sz(2)*out(:,2)+ones(3,1)*(orig(2)+sz(2)/2),1)
  end
    xset('dashes',patt)
case 'getinputs' then
  graphics=arg1(2)
  [orig,sz,orient]=graphics(1:3)
  wd=xget('wdim');
  if orient then
    t=[%pi -%pi/2]
  else
    t=[%pi %pi/2]
  end
  r=sz(2)/2
  rx=r*p
  x=(rx*sin(t)+(orig(1)+rx)*ones(t))
  y=r*cos(t)+(orig(2)+r)*ones(t)
  typ=ones(x)
case 'getoutputs' then
  graphics=arg1(2)
  [orig,sz,orient]=graphics(1:3)
  wd=xget('wdim');
  if orient then
    t=%pi/2
    dx=sz(1)/7
  else
    t=-%pi/2
    dx=-sz(1)/7
  end
  r=sz(2)/2
  rx=r*p
  x=(rx*sin(t)+(orig(1)+rx)*ones(t))+dx
  y=r*cos(t)+(orig(2)+r)*ones(t)
  typ=ones(x)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  x(3)(11)=[] //compatibility
case 'define' then
  model=list(list('prod',2),[-1;-1],-1,[],[],[],[],[],[],'c',[],[%t %f],' ',list())
  x=standard_define([1 1]/1.2,model,[],[])
end




