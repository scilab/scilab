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
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);
  while %t do
    [ok,dt,ff,label]=getvalue(['Set Event Delay  block parameters';
	'Delay  is the delay between an input event ';
	'       and the generated output event';
	'Block may initially generate an output event before ';
	'       any input event. ""Date of initial output event""';
	'       gives the date of this event. Set a negative value';
	'       if no initial event required'],..
	['Delay';'Date of initial output event'],..
	list('vec',1,'vec',1),label)
    if ~ok then break,end
    if dt<=0 then
      message('Delay must be positive')
      ok=%f
    end
    if ok then
      graphics(4)=label
      model(8)=dt
      model(11)=ff
      x(2)=graphics;x(3)=model
      break
    end
  end
case 'define' then
  dt=0.1
  ff=dt
  model=list('evtdly',[],[],1,1,[],[],dt,[],'d',ff,[%f %f],' ',list())
  label=[string(dt);sci2exp(ff)]
  gr_i=['dt=model(8);';
    'txt=[''Delay'';string(dt)];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end




