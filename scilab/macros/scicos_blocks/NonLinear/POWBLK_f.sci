function [x,y,typ]=POWBLK_f(job,arg1,arg2)
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
    [ok,a,label]=getvalue('Set u^a block parameters',..
	'to the power of',list('vec',1),label)
    if ~ok then break,end
    graphics(4)=label
    if a==int(a) then
      model(9)=a;
      model(8)=[]
    else
      model(8)=a;
      model(9)=[]
    end
    model(11)=[] //compatibility
    x(2)=graphics;x(3)=model
    break
  end
case 'define' then
  in=1
  a=1.5
  model=list('powblk',-1,-1,[],[],[],[],a,[],'c',[],[%t %f],' ',list())
  label=string(a)
  gr_i=['xstringb(orig(1),orig(2),''u^a'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end




