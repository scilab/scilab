function [x,y,typ]=ESELECT_f(job,arg1,arg2)
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
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,out,exprs]=getvalue('Set ESELECT block parameters',..
	'number of output event ports',list('vec',1),exprs)
    if ~ok then break,end
    if out<2 then
      message('Block must have at least two output ports')
    else
      [model,graphics,ok]=check_io(model,graphics,1,[],[1],[ones(out,1)])
      if ok then
	graphics.exprs=exprs;model.evtout=ones(out,1);model.ipar=out
	model.sim(2)=-1
	x.graphics=graphics;x.model=model
	break
      end
    end
  end
case 'define' then
  model=scicos_model()
  model.sim=list('eselect',-2)
  model.in=1
  model.evtin=1
  model.evtout=[1;1]
  model.ipar=2
  model.blocktype='l'
  model.firing=[-1 -1]
  model.dep_ut=[%f %f]
  
  gr_i=['txt=[''event select''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  exprs=string(2)
  x=standard_define([3 2],model,exprs,gr_i)
end
endfunction
