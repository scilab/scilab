function [x,y,typ]=RAMP(job,arg1,arg2)
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
    [ok,slope,stt,iout,exprs]=getvalue('Set ramp parameters',..
	['Slope';'Start time';'Initial output'],list('vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    if stt<0  then
      message('Time must be > 0')
    else
      model.rpar=[slope;stt;iout];
      graphics.exprs=exprs
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  slope=0;iout=0;stt=0;rpar=[slope;stt;iout];
  model=scicos_model()
  model.sim=list('ramp',4)
  model.in=[]
  model.out=1
  model.rpar=rpar
  model.blocktype='c'
  model.nmode=1
  model.nzcross=1
  model.dep_ut=[%f %t]
  
  exprs=[string(rpar)]
  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=orig(1)+[4/5;3/5;2/5]*sz(1);';
    'yy=orig(2)+[4/5;1/2;1/2]*sz(2);';
    'xpoly(xx,yy,''lines'');';
    'xset(''thickness'',thick)']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
