function [x,y,typ]=CLINDUMMY_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
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
  x=arg1
  x(3)(11)=[] //compatibility
case 'define' then
  x0=0;
  model=list('cdummy',[],[],[],[],x0,[],[],[],'c',[],[%f %t],' ',list())
  gr_i=['xstringb(orig(1),orig(2),[''DUMMY'';''CLSS''],sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,[],gr_i)
end




