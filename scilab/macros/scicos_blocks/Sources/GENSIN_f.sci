function [x,y,typ]=GENSIN_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  x=[];y=[];typ=[];
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);
  while %t do
    [ok,M,F,P,label]=getvalue(['Set Gen_SIN Block'],..
	    ['Magnitude';'Frequency';'phase'],..
	    list('vec',1,'vec',1,'vec',1),label)
    if ~ok then break,end
    if F<0 then
      message('Frequency must be positive')
      ok=%f
    end
    [model,graphics,ok]=check_io(model,graphics,[],1,[],[])
    if ok then
      model(8)=[M;F;P]
      graphics(4)=label;
      x(2)=graphics;x(3)=model
      break
    end
  end
  x(3)(11)=[] //comptibility
case 'define' then
  rpar=[1;1;0]
  model=list('gensin',[],1,[],[],[],[],rpar,[],'c',[],[%f %t],' ',list())
  label=[string(rpar(1));string(rpar(2));string(rpar(3))]
  gr_i=['txt=[''sinusoid'';''generator''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,label,gr_i)
end




