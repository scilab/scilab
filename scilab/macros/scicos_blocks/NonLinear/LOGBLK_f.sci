function [x,y,typ]=LOGBLK_f(job,arg1,arg2)
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
    [ok,a,label]=getvalue('Set log block parameters',..
	'Basis (>1)',list('vec',1),label)
    if ~ok then break,end
    if a<=1 then
      message('Basis must be larger than 1')
    else
      if ok then
	graphics(4)=label
	model(8)=a
	x(2)=graphics;x(3)=model
	break
      end
    end
  end
  x(3)(11)=[] //compatibility
case 'define' then
  in=1
  a=%e
  model=list('logblk',-1,-1,[],[],[],[],a,[],'c',[],[%t %f],' ',list())
  label='%e'
  gr_i=['xstringb(orig(1),orig(2),''Log'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end




