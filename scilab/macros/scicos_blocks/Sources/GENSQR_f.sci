function [x,y,typ]=GENSQR_f(job,arg1,arg2)
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
  if size(label,'*')==2 then label=label(2),end //compatibility
  while %t do
    [ok,Amplitude,label]=getvalue([
	'Set Square generator block parameters'],..
	['Amplitude'],..
	list('vec',1),label)
    if ~ok then break,end
    graphics(4)=label
    model(7)=Amplitude
    x(2)=graphics;x(3)=model
    break
  end
  x(3)(11)=[] //comptibility
case 'define' then
  Amplitude=1
  model=list('gensqr',[],1,1,[],[],1,[],[],'d',[],[%f %f],' ',list())
  label=string(Amplitude)
  gr_i=['txt=[''square wave'';''generator''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,label,gr_i)
end


