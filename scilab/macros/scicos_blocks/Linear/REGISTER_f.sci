function [x,y,typ]=REGISTER_f(job,arg1,arg2)
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
  x=arg1;
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);
  while %t do
    [ok,z0,label]=getvalue('Set delay parameters',..
	'Register initial condition',list('vec',-1),label)
    if ~ok then break,end
    if prod(size(z0))<2 then
      message('Register length must be at least 2')
      ok=%f
    end
    if ok then
      graphics(4)=label;
      model(7)=z0
      x(2)=graphics;x(3)=model
      break
    end
  end
  x(3)(11)=[] //compatibility
case 'define' then
  z0=zeros(10,1)
  model=list('delay',1,1,1,[],[],z0,[],[],'d',-1,[%f %f],' ',list())
  label=strcat(string(z0),';')
  gr_i=['dly=model(8);';
    'txt=[''Shift'';''Register'';string(dly)];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([2.5 2.5],model,label,gr_i)
end




