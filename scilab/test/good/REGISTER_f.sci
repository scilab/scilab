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
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,z0,exprs]=getvalue('Set delay parameters',..
	'Register initial condition',list('vec',-1),exprs)
    if ~ok then break,end
    if prod(size(z0))<2 then
      message('Register length must be at least 2')
      ok=%f
    end
    if ok then
      graphics.exprs=exprs;
      model.dstate=z0
      x.graphics=graphics;x.model=model
      break
    end
  end

case 'define' then
  z0=zeros(10,1)
  model=scicos_model()
  model.sim='delay'
  model.in=1
  model.out=1
  model.evtin=1
  model.dstate=z0
  model.blocktype='d'
  model.dep_ut=[%f %f]

  exprs=strcat(string(z0),';')
  gr_i=['dly=model.rpar;';
    'txt=[''Shift'';''Register'';string(dly)];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([2.5 2.5],model,exprs,gr_i)
end
endfunction
