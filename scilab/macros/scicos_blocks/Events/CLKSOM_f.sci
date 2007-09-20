function [x,y,typ]=CLKSOM_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
p=1 //pixel sizes ratio
select job
case 'plot' then
  orig=arg1.graphics.orig;
  sz=arg1.graphics.sz;
  orient=arg1.graphics.flip;

  thick=xget('thickness');xset('thickness',2)
  //** dash=xget('dashes');xset('dashes',default_color(-1))
  rx=sz(1)*p/2
  ry=sz(2)/2
  gr_i=arg1.graphics.gr_i;
  if type(gr_i)==15 then 
    xfarcs([orig(1);orig(2)+sz(2);sz(1)*p;sz(2);0;360*64],gr_i(2))
    gh_temp = gce();
    if gh_temp.type=='Compound' then
      gh_temp.children(1).foreground = default_color(-1);
    else
      gh_temp.foreground = default_color(-1);
    end
  end
  xarc(orig(1),orig(2)+sz(2),sz(1)*p,sz(2),0,360*64)
  gh_temp = gce();
  gh_temp.foreground = default_color(-1);
  xsegs(orig(1)+rx*[1/2.3 1;2-1/2.3 1],orig(2)+ry*[1 2-1/2.3;1,1/2.3],0)
  gh_temp = gce();
  gh_temp.segs_color = [default_color(-1), default_color(-1)];

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
  gh_temp = gce(); 
  gh_temp.foreground = default_color(-1);
  gh_temp.background = default_color(-1);
  //** xset('dashes',dash)
case 'getinputs' then
  orig=arg1.graphics.orig;
  sz=arg1.graphics.sz;
  orient=arg1.graphics.flip;
  if orient then
    t=[%pi -%pi/2 0]
  else
    t=[%pi %pi/2 0]
  end
  r=sz(2)/2
  rx=r*p
  x=(rx*sin(t)+(orig(1)+rx)*ones(t))
  y=r*cos(t)+(orig(2)+r)*ones(t)
  typ=-ones(x)
case 'getoutputs' then
  orig=arg1.graphics.orig;
  sz=arg1.graphics.sz;
  orient=arg1.graphics.flip;
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
  typ=-ones(x)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
case 'define' then
  model=scicos_model()
  model.sim='sum'
  model.evtin=[1;1;1]
  model.evtout=1
  model.blocktype='d'
  model.firing=-1
  model.dep_ut=[%f %f]

  gr_i=['rx=sz(1)*p/2;ry=sz(2)/2'
      'xsegs(orig(1)+rx*[1/2.3 1;2-1/2.3 1],orig(2)+ry*[1 2-1/2.3;1,1/2.3],0)']
  x=standard_define([1 1]/1.2,model,[],gr_i)
end
endfunction
