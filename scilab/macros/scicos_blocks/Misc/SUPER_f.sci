function [x,y,typ]=SUPER_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[],typ=[]
select job
case 'plot' then
  standard_draw(arg1)
  s_port_names(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  while %t do
    [x,newparameters,needcompile,edited]=scicos(arg1(3)(8))
    arg1(3)(8)=x
    [ok,arg1]=adjust_s_ports(arg1)
    if ok then
      x=arg1
      y=needcompile
      typ=newparameters
      break
    end
  end
  edited=resume(edited)
case 'define' then
  model=list('super',1,1,[],[],[],' ',..
      list(list([600,450,0,0],'Super Block',[],[],[])),[],'h',[],[%f %f])
  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=orig(1)+      [2 4 4]*(sz(1)/7);';
    'yy=orig(2)+sz(2)-[2 2 6]*(sz(2)/10);';
    'xrects([xx;yy;[sz(1)/7;sz(2)/5]*ones(1,3)]);';
    'xx=orig(1)+      [1 2 3 4 5 6 3.5 3.5 3.5 4 5 5.5 5.5 5.5]*sz(1)/7;';
    'yy=orig(2)+sz(2)-[3 3 3 3 3 3 3   7   7   7 7 7   7   3  ]*sz(2)/10;';
    'xsegs(xx,yy,0);';
    'xset(''thickness'',thick)']
  x=standard_define([2 2],model,[],gr_i)
end




