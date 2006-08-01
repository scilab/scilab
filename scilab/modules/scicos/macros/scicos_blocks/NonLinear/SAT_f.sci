function [x,y,typ]=SAT_f(job,arg1,arg2)
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
    [ok,minp,maxp,pente,exprs]=getvalue('Set Saturation parameters',..
	['Min';'Max';'Slope'],list('vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    if maxp<=0  then
      message('Max must be strictly positive')
    elseif pente<=0 then
      message('Slope must be strictly positive')
    else
      rpar=[minp/pente;maxp/pente;pente]
      model.rpar=rpar
      model.firing=[] //compatibility
      graphics.exprs=exprs
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  minp=-1;maxp=1;slope=1;rpar=[minp;maxp;slope]

  model=scicos_model()
  model.sim=list('lusat',1)
  model.in=1
  model.nzcross=2
  model.out=1
  model.rpar=[minp;maxp;slope]
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[string(minp);string(maxp);string(slope)]
  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=orig(1)+[4/5;1/2+1/5;1/2-1/5;1/5]*sz(1);';
    'yy=orig(2)+[1-1/5;1-1/5;1/5;1/5]*sz(2);';
    'xpoly(xx,yy,''lines'');';
    'xset(''thickness'',thick)']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
