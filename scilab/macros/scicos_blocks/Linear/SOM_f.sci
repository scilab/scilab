function [x,y,typ]=SOM_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
p=1 //pixel sizes ratio
select job
case 'plot' then
  wd=xget('wdim')
  graphics=arg1(2); [orig,sz,orient]=graphics(1:3)
  thick=xget('thickness');xset('thickness',2)
  patt=xget('dashes');xset('dashes',27)
  rx=sz(1)*p/2
  ry=sz(2)/2
  gr_i=arg1(2)(9);
  if type(gr_i)==15 then 
    xfarcs([orig(1);orig(2)+sz(2);sz(1)*p;sz(2);0;360*64],gr_i(2))
  end
  xarc(orig(1),orig(2)+sz(2),sz(1)*p,sz(2),0,360*64)
  xsegs(orig(1)+rx*[1/2.3 1;2-1/2.3 1],orig(2)+ry*[1 2-1/2.3;1,1/2.3],27)
  xset('thickness',thick);
  if orient then  //standard orientation
    out= [0  -1/14
          1/7    0
	  0   1/14]*3
    xfpoly(sz(1)*out(:,1)+orig(1)+sz(1)*p,sz(2)*out(:,2)+orig(2)+sz(2)/2,1)
  else //tilded orientation
    out= [0   -1/14
	  -1/7    0
 	   0   1/14]*3
    xfpoly(sz(1)*out(:,1)+orig(1),sz(2)*out(:,2)+orig(2)+sz(2)/2,1)
  end
  xset('dashes',patt)
case 'getinputs' then
  graphics=arg1(2)
  [orig,sz,orient]=graphics(1:3)
  wd=xget('wdim');
  if orient then
    t=[%pi -%pi/2 0]
  else
    t=[%pi %pi/2 0]
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
  [graphics,model]=arg1(2:3);
  label=graphics(4)
  if size(label,'*')==2 then label=label(2),end
  if size(label,'*')<>3 then label=string(model(8)),end
  if graphics(3) then
    labs=['down','left','up']
  else
    labs=['down','right','up']
  end

  message(['This sum block is obsolete'
      'parameters cannot be modified. Please replace it with new sum block';
      'and gain blocks in the linear palette'
      ' '
      'Input ports are located at up, side and  down positions.'
      'Current gains are:' 
      part(labs(:),1:7)+'  '+label(:)])
case 'define' then
  sgn=[1;1;1]
  model=list(list('sum',2),[-1;-1;-1],-1,[],[],[],[],sgn,[],'c',[],[%t %f],' ',list())
  label=[sci2exp(1);sci2exp(sgn)]
  
  gr_i=['rx=sz(1)*p/2;ry=sz(2)/2'
  'xsegs(orig(1)+rx*[1/2.3 1;2-1/2.3 1],orig(2)+ry*[1 2-1/2.3;1,1/2.3],0)']
  
  x=standard_define([1 1]/1.2,model,label,gr_i)
end




