function [x,y,typ]=EVTDLY_f(job,arg1,arg2)
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
    [ok,dt,ff,exprs]=getvalue(['Set Event Delay  block parameters';
	'Delay  is the delay between an input event ';
	'       and the generated output event';
	'Block may initially generate an output event before ';
	'       any input event. ""Date of initial output event""';
	'       gives the date of this event. Set a negative value';
	'       if no initial event required'],..
	['Delay';'Date of initial output event'],..
	list('vec',1,'vec',1),exprs)
    if ~ok then break,end
    if dt<=0 then
      message('Delay must be positive')
      ok=%f
    end
    if ok then
      graphics.exprs=exprs
      model.rpar=dt
      model.firing=ff
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  dt=0.1
  ff=dt
  model=scicos_model()
  model.sim='evtdly'
  model.evtin=1
  model.evtout=1
  model.rpar=dt
  model.blocktype='d'
  model.firing=ff
  model.dep_ut=[%f %f]
  
  exprs=[string(dt);sci2exp(ff)]
  gr_i=['dt=o.model.rpar;';
    'txt=[''Delay'';string(dt)];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
