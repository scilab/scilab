function [x,y,typ]=EVTGEN_f(job,arg1,arg2)
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
    [ok,tt,label]=getvalue('Set Event time',..
	['Event Time'],list('vec',1),label)
    if ~ok then break,end
    graphics(4)=label
    if model(11)<>tt then
      model(11)=tt
    end
    x(2)=graphics;x(3)=model
    break
  end
case 'define' then
  tt=0
  model=list('trash',[],[],[],1,[],[],[],[],'d',tt,[%f %f],' ',list())
  label=string(tt)
  gr_i=['tt=model(11);';
    'txt=[''Event at'';''time ''+string(tt)];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end




