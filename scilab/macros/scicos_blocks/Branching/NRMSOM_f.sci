function [x,y,typ]=NRMSOM_f(job,arg1,arg2)
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
  x=arg1;
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);
  while %t do
    [ok,nin,label]=getvalue('Set parameters',..
	['number of inputs'],list('vec',1),label)
    if ~ok then break,end
    [model,graphics,ok]=check_io(model,graphics,-ones(nin,1),-1,[],[])
    if ok then
      graphics(4)=label;
      x(2)=graphics;x(3)=model
      break;
    end
  end
case 'define' then
  in=[-1;-1]
  nin=2
  model=list('junk',in,-1,[],[],[],[],[],[],'c',[],[%t %f],' ',list())
  label=[string(nin)]
  gr_i=' '
  x=standard_define([.2 2],model,label,gr_i)
end




