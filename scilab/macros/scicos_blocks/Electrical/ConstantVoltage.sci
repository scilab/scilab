function [x,y,typ]=ConstantVoltage(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
  standard_draw(arg1,%f) 
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
    [ok,V,exprs]=getvalue('Set ConstantVoltage block parameter',..
    			   'V (volt)',list('vec',1),exprs)
    if ~ok then break,end
    model.rpar=V
    model.equations.parameters(2)=list(V)
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
    break
  end

case 'define' then
  V=0.01

  model=scicos_model()
  model.rpar=V
  model.in=1;model.out=1;
  model.sim='ConstantVoltage';
  model.blocktype='c'
  model.dep_ut=[%f %f]
  mo=modelica()
  mo.model='ConstantVoltage';
  mo.inputs='p';
  mo.outputs='n';
  mo.parameters=list('V',list(V))
  model.equations=mo
  exprs=string(V)

  gr_i=['xset(''thickness'',2)'
	'xx=[0 5 5 5]/12;';
	'if orient then yy=[1 1 2 0]/2;else yy=[2,2,3,1]/4,end'
	'xpoly(orig(1)+xx*sz(1),orig(2)+yy*sz(2));';
	'xx=[2 2 2 3]/3;';
	'if orient then yy=[1,3,2,2]/4;else yy=[0 2 1 1]/2;end'
	'xpoly(orig(1)+xx*sz(1),orig(2)+yy*sz(2));']
  
  x=standard_define([1.5 1.1],model,exprs,list(gr_i,0))
  x.graphics.in_implicit=['I']
  x.graphics.out_implicit=['I']  
end
endfunction
