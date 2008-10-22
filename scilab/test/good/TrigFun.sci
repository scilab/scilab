function [x,y,typ]=TrigFun(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
 case 'plot' then
  Txt='  '+arg1.graphics.exprs+' '
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
  %FF=['sin';'cos';'tan';
       'asin';'acos';'atan';
       'sinh';'cosh';'tanh';
       'asinh';'acosh';'atanh'];
  %GG=['Choose among '+strcat(%FF(1:4),', ');strcat(%FF(5:$),', ')]
  while %t do
    [ok,fun,exprs]=getvalue(%GG,'Function',list('str',1),exprs)
    if ~ok then break,end
    if find(%FF==fun)==[] then
      message('Sorry but '+fun +' is not in the list!')
    else
      graphics.exprs=exprs
      execstr('model.sim=list('''+fun+'_blk'',4)')
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  model=scicos_model()
  model.sim=list('sin_blk',4)
  model.in=-1
  model.out=-1
  model.blocktype='c'
  model.dep_ut=[%t %f]
  exprs='sin'
  gr_i=['xstringb(orig(1),orig(2),[''Trig. Function'';Txt],sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,exprs,gr_i)
end
endfunction
