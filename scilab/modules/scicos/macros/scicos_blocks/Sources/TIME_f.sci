function [x,y,typ]=TIME_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[],typ=[]
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
  x=arg1
case 'define' then

  model=scicos_model()
  model.sim='timblk'
  model.out=1
  model.blocktype='c'
  model.dep_ut=[%f %t]
  
  gr_i=['wd=xget(''wdim'').*[1.016,1.12];';
    'thick=xget(''thickness'');xset(''thickness'',2);';
    'p=wd(2)/wd(1);p=1;';
    'rx=sz(1)*p/2;ry=sz(2)/2;';
    'xarc(orig(1)+0.05*sz(1),orig(2)+0.95*sz(2),0.9*sz(1)*p,0.9*sz(2),0,360*64);';
    'xset(''thickness'',1);';
    'xx=[orig(1)+rx    orig(1)+rx;';
    'orig(1)+rx    orig(1)+rx+0.6*rx*cos(%pi/6)];';
    'yy=[orig(2)+ry    orig(2)+ry ;';
    '	  orig(2)+1.8*ry  orig(2)+ry+0.6*ry*sin(%pi/6)];';
    'xsegs(xx,yy,0);';
    'xset(''thickness'',thick);']
   x=standard_define([2 2],model,[],gr_i)
end
endfunction
