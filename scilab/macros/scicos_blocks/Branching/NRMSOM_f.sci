function [x,y,typ]=NRMSOM_f(job,arg1,arg2)
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
    [ok,nin,exprs]=getvalue('Set parameters',..
	['number of inputs'],list('vec',1),exprs)
    if ~ok then break,end
    [model,graphics,ok]=check_io(model,graphics,-ones(nin,1),-1,[],[])
    if ok then
      graphics.exprs=exprs;
      x.graphics=graphics;x.model=model
      break;
    end
  end
case 'define' then
  in=[-1;-1]
  nin=2
  model=scicos_model()
  model.sim='junk'
  model.in=in
  model.out=-1
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[string(nin)]
  gr_i=' '
  x=standard_define([.2 2],model,exprs,gr_i)
end
endfunction
