function [x,y,typ]=HALT_f(job,arg1,arg2)
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
    [ok,n,label]=getvalue('Set Halt block parameters',..
	['State on halt'],list('vec',1),label)
    if ~ok then break,end
    if ok then
      graphics(4)=label
      model(9)=n
      x(2)=graphics;x(3)=model
      break
    end
  end
  x(3)(11)=[] //compatibility
case 'define' then
  n=0
  model=list('hltblk',[],[],1,[],[],[0],[],[0],'d',[],[%f %f],' ',list())
  label=string(n)
  gr_i=['xstringb(orig(1),orig(2),''STOP'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end




