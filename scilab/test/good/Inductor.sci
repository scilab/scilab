function [x,y,typ]=Inductor(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
  case 'plot' then
  L=arg1.graphics.exprs;
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
    [ok,L,exprs]=getvalue('Set Inductor block parameter',..
    			   'L (T)',list('vec',1),exprs)
    if ~ok then break,end
    model.rpar=L
    model.equations.parameters(2)=list(L)
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
    break
  end
case 'define' then
  model=scicos_model()
  model.in=[1];
  model.out=[1];
  L=1.d-5
  model.rpar=L
  model.sim='Inductor'
  model.blocktype='c'
  model.dep_ut=[%t %f]
  mo=modelica();
  mo.model='Inductor'
  mo.inputs='p';
  mo.outputs='n';
  mo.parameters=list('L',list(L))
  model.equations=mo;
  exprs=string(L)

  gr_i=['tt=linspace(0.04,0.96,100)'';'
	'xpoly(tt*sz(1)+orig(1),+orig(2)+abs(sin(18*(tt-0.04)))*sz(2),""lines"");';
	'xx=orig(1)+[0 0.04 0.04 0.04 0]*sz(1);';
	'yy=orig(2)+[1/2 1/2 0  1/2 1/2]*sz(2);';
	'xpoly(xx,yy) ';
	'xx=orig(1)+[0.96 0.96 1   0.96 0.96 ]*sz(1);';
	'yy=orig(2)+[abs(sin(18*0.92))   1/2   1/2 1/2 abs(sin(18*0.92))]*sz(2);';
	'xpoly(xx,yy) ';
	'rect=xstringl(0,0,''L=''+L)'
	'xstring(orig(1)+(sz(1)-rect(3))/2,orig(2)-rect(4)*1.2,''L=''+L)' ] 
 
  x=standard_define([2 0.9],model,exprs,list(gr_i,0))
  x.graphics.in_implicit=['I']
  x.graphics.out_implicit=['I']
end
endfunction
