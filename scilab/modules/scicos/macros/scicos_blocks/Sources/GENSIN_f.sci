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
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,M,F,P,exprs]=getvalue(['Set Gen_SIN Block'],..
	    ['Magnitude';'Frequency';'phase'],..
	    list('vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    if F<0 then
      message('Frequency must be positive')
      ok=%f
    end
    [model,graphics,ok]=check_io(model,graphics,[],1,[],[])
    if ok then
      model.rpar=[M;F;P]
      graphics.exprs=exprs;
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  rpar=[1;1;0]
  model=scicos_model()
  model.sim='gensin'
  model.in=[]
  model.out=1
  model.rpar=[1;1;0]
  model.blocktype='c'
  model.dep_ut=[%f %t]
  
  exprs=[string(rpar(1));string(rpar(2));string(rpar(3))]
  gr_i=['txt=[''sinusoid'';''generator''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,exprs,gr_i)
end
endfunction
