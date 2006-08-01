function [x,y,typ]=MEMORY_f(job,arg1,arg2)
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
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,a,inh,exprs]=getvalue('Set memory block parameters',..
	['initial condition';'Inherit (1: no, 0: yes)'],list('vec',-1,'vec',1),exprs)
    if ~ok then break,end
    if inh==0 then inh=[]; else inh=1;end
    [model,graphics,ok]=check_io(model,graphics,-1,-1,inh,[])
    out=size(a,'*');if out==0 then ok=%f,x_message('Initial condition empty');end
    in=out
    if ok then
      graphics.exprs=exprs;
      model.rpar=a;model.in=in;model.out=out
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  z=0
  in=1
  exprs=[string(z);string(1)]
  model=scicos_model()
  model.sim='memo'
  model.in=in
  model.out=in
  model.evtin=1
  model.dstate=0
  model.rpar=z
  model.blocktype='m'
  model.dep_ut=[%f %f]

  gr_i='xstringb(orig(1),orig(2),''Mem'',sz(1),sz(2),''fill'')'
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
