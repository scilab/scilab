function [x,y,typ]=DOLLAR_f(job,arg1,arg2)
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
  while %t do
    [ok,a,label]=getvalue('Set dollar block parameters',..
	'initial condition',list('vec',-1),label)
    if ~ok then break,end
    out=size(a,'*');if out==0 then out=[],end
    in=out
    if ok then
      graphics(4)=label;
      model(7)=a;model(2)=in;model(3)=out
      x(2)=graphics;x(3)=model
      break
    end
  end
  x(3)(11)=[] //compatibility
case 'define' then
  z=0
  in=1
  label=string(z)
  model=list('dollar',in,in,1,[],[],z,[],[],'d',[],[%f %f],' ',list())
  gr_i='xstringb(orig(1),orig(2),''1/z'',sz(1),sz(2),''fill'')'
  x=standard_define([2 2],model,label,gr_i)
end




