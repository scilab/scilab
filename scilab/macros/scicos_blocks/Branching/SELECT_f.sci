function [x,y,typ]=SELECT_f(job,arg1,arg2)
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
    [ok,nin,z0,exprs]=getvalue('Set parameters',..
	['number of inputs';'initial connected input'],..
	list('vec',1,'vec',1),exprs)
    if ~ok then break,end
    if z0>nin|z0<=0 then
      message('initial connected input is not a valid input port number')
    else
      [model,graphics,ok]=check_io(model,graphics,-ones(nin,1),-1,ones(nin,1),[])
      if ok then
	graphics.exprs=exprs;
	model.dstate=z0-1,
	x.graphics=graphics;x.model=model
	break
      end
    end
  end
case 'define' then
  z0=0
  in=[-1;-1]
  nin=2

  model=scicos_model()
  model.sim=list('selector',2)
  model.in=in
  model.out=-1
  model.evtin=ones(in)
  model.dstate=z0
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[string(nin);string(z0+1)]
  gr_i=['xstringb(orig(1),orig(2),''Selector'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
