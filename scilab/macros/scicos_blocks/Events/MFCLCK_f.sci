function [x,y,typ]=MFCLCK_f(job,arg1,arg2)
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
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);
  [ok,dt,nn,label]=getvalue('Set Multifrequency clock parameters',..
      ['basic period (1/f)';'multiply by (n)'],list('vec',1,'vec',1),label)
  if ok then
    model(9)=nn
    model(8)=dt;
    hh=model(11);hh(2)=0;model(11)=hh //compatibility
    graphics(4)=label
    x(2)=graphics;x(3)=model
  end
case 'define' then
  nn=2
  dt=0.1
  model=list('mfclck',[],[],1,[1;1],[],0,dt,nn,'d',[-1 0],[%f %f],' ',list())
  label=[string(dt);string(nn)]
  gr_i=['txt=[''M. freq'';''clock''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([2 2],model,label,gr_i)
end




