function [x,y,typ]=INTEGRAL_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
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
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);
  while %t do
   [ok,x0,label]=getvalue('Set continuous linear system parameters',..
	'Initial state',list('vec',1),label)


    if ~ok then break,end
    graphics(4)=label;
    model(6)=x0;
    x(2)=graphics;x(3)=model
    break
  end
  x(3)(11)=[] //compatibility
case 'define' then
  x0=0
  model=list('integr',1,1,[],[],0,[],[],[],'c',[],[%f %t],' ',list())
  label=strcat(sci2exp(x0))
  gr_i=['xstringb(orig(1),orig(2),''  1/s  '',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end




