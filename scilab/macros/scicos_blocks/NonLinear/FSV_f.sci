function [x,y,typ]=FSV_f(job,arg1,arg2)
//Absolute value block GUI.
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
  in=1 //One input/output port with undefinite dimension
  model=list(list('fsv',1),in,in,[],[],[],[],[],[],'c',[],[%t %f],' ',list())
  label=' '
  gr_i='xstringb(orig(1),orig(2),''f_sv'',sz(1),sz(2),''fill'')'
  x=standard_define([2 2],model,label,gr_i)
end




