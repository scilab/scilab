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
  if size(exprs,'*')==1 then exprs(2)=string(1);end
  if size(exprs,'*')==2 then exprs(3)=string(0);end
  model=arg1.model;
  while %t do
    [ok,out,inh,nmod,exprs]=getvalue('Set ESELECT block parameters',..
	['number of output event ports';'Inherit (1: no, 0: yes)';
	 'zero-crossing (0: no, 1: yes)'],..
				 list('vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    if nmod<>0 then nmod=1,end
    if inh==0 then inh=[]; else inh=1;end
    out=int(out)
    if out<2 then
      message('Block must have at least two output ports')
    else
      [model,graphics,ok]=check_io(model,graphics,1,[],inh,[ones(out,1)])
      if ok then
	graphics.exprs=exprs;model.evtout=ones(out,1); model.firing=-ones(out,1)
	x.graphics=graphics;
	model.nmode=nmod
	model.nzcross=nmod
	x.model=model
	break
      end
    end
  end
case 'define' then
  out=2
  model=scicos_model()
  model.sim=list('eselect',-2)
  model.in=1
  model.evtin=1
  model.evtout=ones(out,1);
  model.blocktype='l'
  model.firing=-ones(out,1);
  model.dep_ut=[%f %f]
  model.nmode=0
  model.nzcross=0
   
  gr_i=['txt=[''event select''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  exprs=[string(out);string(1);string(model.nmode)]
  x=standard_define([3 2],model,exprs,gr_i)
end
endfunction
