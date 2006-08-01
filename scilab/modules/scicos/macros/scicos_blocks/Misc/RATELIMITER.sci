function [x,y,typ]=RATELIMITER(job,arg1,arg2)
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
    [ok,maxp,minp,exprs]=getvalue('Set rate limiter parameters',..
	['max slope';'min slope';],list('vec',1,'vec',1),exprs)
    if ~ok then break,end
    if maxp<=minp|maxp<=0|minp>=0  then
      message('We must have max_slope> 0 > min_slope.')
    else
      rpar=[maxp;minp]
      model.rpar=rpar
      graphics.exprs=exprs
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  minp=-1;maxp=1;rpar=[maxp;minp]
  model=scicos_model()
  model.sim=list('ratelimiter',4)
  model.in=1
  model.out=1
  model.rpar=rpar
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[string(maxp);string(minp)]
  gr_i='xstringb(orig(1),orig(2),''Rate limiter'',sz(1),sz(2),''fill'')'
  x=standard_define([3 2],model,exprs,gr_i)
end
endfunction
