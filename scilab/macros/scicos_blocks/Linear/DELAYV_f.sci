function [x,y,typ]=DELAYV_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then //normal  position 
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
  model=arg1(3);nin=model(2)(1)
  z0=model(7);zz0=z0(1:$-1);told=z0($);
 
   while %t do
    [ok,nin,zz0,T,label]=getvalue('Set delay parameters',..
     	['Number of inputs';
	'Register initial condition';
	'Max delay'],..
	list('vec',1,'vec',-1,'vec',1),..
	label);
    if ~ok then break,end
    if size(zz0,'*')<2 then 
      message('Register length must be at least 2')
      ok=%f
    end 
    if T<=0 then
      message('Delay must be positive')
      ok=%f
    end
   
   if ok then
      [model,graphics,ok]=check_io(model,graphics,[nin;1],nin,1,[1;1])
   end	   
    if ok then
      graphics(4)=label;
      model(7)=[zz0;told];model(8)=T/(size(zz0,'*'));
      x(2)=graphics;x(3)=model
      break
    end
  end
case 'define' then
  nin=1
  z0=zeros(11,1);zz0=z0(1:$-1);
  T=1
  model=list(list('delayv',1),[nin;1],nin,1,[1;1],[],z0,T,[],'d',[0 -1],[%t %t],..
      ' ',list())
  label=[string(nin);strcat(string(zz0),';');string(T)];
  gr_i=['txt=[''Variable'';''delay''];';
        'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([2.5 2.5],model,label,gr_i)
end

