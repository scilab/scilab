function [x,y,typ]=INIMPL_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  x=[];y=[];typ=[]
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  if size(exprs,'*')==2 then exprs=exprs(1),end //compatibility
  while %t do
    [ok,prt,exprs]=getvalue('Set Input block parameters',..
	'Port number',list('vec',1),exprs)
    if ~ok then break,end
    prt=int(prt)
    if prt<=0 then
      message('Port number must be a positive integer')
    else
      if model.ipar<>prt then needcompile=4;y=needcompile,end
      model.ipar=prt
      graphics.exprs=exprs
      x.graphics=graphics
      x.model=model
      break
    end
  end
case 'define' then
  model=scicos_model()
  model.sim='inimpl'
  model.out=[-1]
  model.out2=[1]
  model.ipar=[1]
  model.dep_ut=[%f %f]
  model.blocktype='c'
  mo=modelica()
  mo.model='PORT'
  mo.outputs='n'
  model.equations=mo
  prt=1
  exprs='1'
  gr_i=['prt=string(model.ipar);xstringb(orig(1),orig(2),prt,sz(1),sz(2))']
  x=standard_define([1 1],model,exprs,gr_i)
  x.graphics.out_implicit=['I']

//  x.graphics.flip=%f //flip it
end
endfunction
