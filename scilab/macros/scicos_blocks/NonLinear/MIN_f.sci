function [x,y,typ]=MIN_f(job,arg1,arg2)
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
  x(3)(11)=[] //compatibility
case 'define' then
  in=-1
  model=list('minblk',in,1,[],[],[],[0;0],[],[],'c',[],[%t %f],' ',list())
  label=sci2exp(in)
  gr_i=['xstringb(orig(1),orig(2),''MIN'',sz(1),sz(2),''fill'')']
  x=standard_define([2 2],model,label,gr_i)
end




