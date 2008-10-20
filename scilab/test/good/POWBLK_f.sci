function [x,y,typ]=POWBLK_f(job,arg1,arg2)
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
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  if size(exprs,'*')==2 then exprs=exprs(2),end //compatibility
  while %t do
    [ok,a,exprs]=getvalue('Set u^a block parameters',..
	'to the power of',list('vec',1),exprs)
    if ~ok then break,end
    graphics.exprs=exprs
    if a==int(a) then
      model.ipar=a;
      model.rpar=[]
    else
      model.rpar=a;
      model.ipar=[]
    end
    model.firing=[] //compatibility
    x.graphics=graphics;x.model=model
    break
  end
case 'define' then
  in=1
  a=1.5
  model=scicos_model()
  model.sim='powblk'
  model.in=-1
  model.out=-1
  model.rpar=a
  model.blocktype='c'
  model.dep_ut=[%t %f]

  exprs=string(a)
  gr_i=['xstringb(orig(1),orig(2),''u^a'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
