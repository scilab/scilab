function [x,y,typ]=DEADBAND(job,arg1,arg2)
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
    [ok,maxp,minp,zeroc,exprs]=getvalue('Set Deadband parameters',..
	['End of dead band';'Start of dead band';'zero crossing (0:no, 1:yes)'],list('vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    if maxp<=minp  then
      message('Upper limit must be > Lower limit')
    else
      rpar=[maxp;minp]
      model.rpar=rpar
      if zeroc<>0 then 
	model.nzcross=2
	model.nmode=1
      else
	model.nzcross=0
	model.nmode=0
      end
      graphics.exprs=exprs
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  minp=-.5;maxp=.5;rpar=[maxp;minp]
  model=scicos_model()
  model.sim=list('deadband',4)
  model.in=1
  model.nzcross=2
  model.nmode=1
  model.out=1
  model.rpar=rpar
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[string(maxp);string(minp);string(model.nmode)]
  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=orig(1)+[4/5;3/5;2/5;1/5]*sz(1);';
    'yy=orig(2)+[1-1/5;1/2;1/2;1/5]*sz(2);';
    'xpoly(xx,yy,''lines'');';
    'xset(''thickness'',thick)']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
