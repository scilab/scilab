function [x,y,typ]=VARIABLE_DELAY(job,arg1,arg2)
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
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;nin=model.in(1)
 
   while %t do
    [ok,T,init,N,exprs]=getvalue('Set delay parameters',..
     	['Max delay';'initial input';'Buffer size'],..
	list('vec',1,'vec',1,'vec',1),..
	exprs);
    if ~ok then break,end
    if N<2 then 
      message('Buffer must be larger than 2')
      ok=%f
    end 
    if T<=0 then
      message('Delay must be positive')
      ok=%f
    end
   
   if ok then
      [model,graphics,ok]=check_io(model,graphics,[-1;1],-1,[],[])
   end	   
    if ok then
      graphics.exprs=exprs;
      model.rpar=[T;init];
      model.ipar=N
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  nin=1
  T=1
  init=0
  N=1024
  model=scicos_model()
  model.sim=list('variable_delay',4)
  model.in=[nin;1]
  model.out=nin
  model.rpar=[T,init];
  model.ipar=N
  model.blocktype='d'
  model.dep_ut=[%f %t %f]

  exprs=[string(T);string(init);string(N)];
  gr_i=['txt=[''Variable'';''delay''];';
        'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([2.5 2.5],model,exprs,gr_i)
end
endfunction
