function [x,y,typ]=TRASH_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(o)
case 'getoutputs' then
  x=[];y=[];typ=[];
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  x(3)(11)=[] //comaptibility
case 'define' then
  in=-1
  model=list('trash',in,[],1,[],[],[],[],[],'d',[],[%f %f],' ',list())
  label=' '
  gr_i=['xstringb(orig(1),orig(2),''Trash'',sz(1),sz(2),''fill'')']
  x=standard_define([2 2],model,label,gr_i)
end




