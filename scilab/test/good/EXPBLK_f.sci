function [x,y,typ]=EXPBLK_f(job,arg1,arg2)
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
  if size(exprs,'*')==2 then exprs=exprs(2),end
  while %t do
    [ok,a,exprs]=getvalue('Set a^u  block parameters',..
	'a (>0)',list('vec',1),exprs)
    if ~ok then break,end
    if or(a<=0) then
      message('a^u : a must be positive')
    else
      graphics.exprs=exprs
      model.rpar=a;
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  in=1
  a=%e
  model=scicos_model()
  model.sim='expblk'
  model.in=-1
  model.out=-1
  model.rpar=a
  model.blocktype='c'
  model.dep_ut=[%t %f]

  exprs=['%e']
  gr_i=['xstringb(orig(1),orig(2),''a^u'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
