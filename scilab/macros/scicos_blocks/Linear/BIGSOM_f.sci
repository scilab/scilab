function [x,y,typ]=BIGSOM_f(job,arg1,arg2)
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
  [graphics,model]=arg1(2:3);
  label=graphics(4)
  while %t do
    [ok,sgn,label]=getvalue('Set sum block parameters',..
    			   'Inputs ports signs/gain',list('vec',-1),label)
    if ~ok then break,end
    in=-ones(size(sgn,'*'),1)
    [model,graphics,ok]=check_io(model,graphics,in,-1,[],[])
    model(8)=sgn(:)
    graphics(4)=label
    x(2)=graphics;x(3)=model
    break
  end
case 'define' then
  sgn=[1;1]
  model=list(list('sum',2),[-1;-1],-1,[],[],[],[],sgn,[],'c',[],[%t %f],' ',list())
  label=sci2exp(sgn)
   gr_i=['[x,y,typ]=standard_inputs(o) ';
          'sgn=o(3)(8)';
          'for k=1:size(x,''*'')';
          '  if sgn(k)>0 then';
          '    xstring(orig(1)+sz(1)/8 ,y(k)-4,''+'')';
          '  else';
          '    xstring(orig(1)+sz(1)/8 ,y(k)-4,''-'')';
          '  end';
          'end';
          'xx=sz(1)*[.8 .4 0.75 .4 .8]+orig(1)';
          'yy=sz(2)*[.8 .8 .5 .2 .2]+orig(2)';
          'xpoly(xx,yy,''lines'')']
  x=standard_define([2 3],model,label,gr_i)
end




