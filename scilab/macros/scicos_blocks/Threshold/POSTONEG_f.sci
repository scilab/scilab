function [x,y,typ]=POSTONEG_f(job,arg1,arg2)
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
  x(3)(11)=[-1] //compatibility
case 'define' then
  rpar=[-1;-1;-1;0]
  model=list('zcross',1,[],[],1,[],[],rpar,[],'z',-1,[%t %f],' ',list())
  gr_i=['xstringb(orig(1),orig(2),'' + to - '',sz(1),sz(2),''fill'')']
  x=standard_define([2 2],model,[],gr_i)
end




