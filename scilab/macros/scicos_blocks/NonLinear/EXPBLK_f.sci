function [x,y,typ]=EXPBLK_f(job,arg1,arg2)
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
  if size(label,'*')==2 then label=label(2),end
  while %t do
    [ok,a,label]=getvalue('Set a^u  block parameters',..
	'a (>0)',list('vec',1),label)
    if ~ok then break,end
    if or(a<=0) then
      message('a^u : a must be positive')
    else
      graphics(4)=label
      model(8)=a;
      x(2)=graphics;x(3)=model
      break
    end
  end
  x(3)(11)=[] //compatibility
case 'define' then
  in=1
  a=%e
  model=list('expblk',-1,-1,[],[],[],[],%e,[],'c',[],[%t %f],' ',list())
  label=[string(in);'%e']
  gr_i=['xstringb(orig(1),orig(2),''a^u'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end




