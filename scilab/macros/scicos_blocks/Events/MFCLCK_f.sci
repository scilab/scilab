function [x,y,typ]=MFCLCK_f(job,arg1,arg2)
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
  [ok,dt,nn,exprs]=getvalue('Set Multifrequency clock parameters',..
      ['basic period (1/f)';'multiply by (n)'],list('vec',1,'vec',1),exprs)
  if ok then
    model.ipar=nn
    model.rpar=dt;
    hh=model.firing;hh(2)=0;model.firing=hh //compatibility
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
  end
case 'define' then
  nn=2
  dt=0.1
  model=scicos_model()
  model.sim='mfclck'
  model.evtin=1
  model.evtout=[1;1]
  model.dstate=0
  model.rpar=dt
  model.ipar=nn
  model.blocktype='d'
  model.firing=[-1 0]
  model.dep_ut=[%f %f]

  exprs=[string(dt);string(nn)]
  gr_i=['txt=[''M. freq'';''clock''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
