function [x,y,typ]=ZCROSS_f(job,arg1,arg2)
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
  while %t do
    [ok,in,exprs]=getvalue(['Set Zero-Crossing parameters';..
	                    'All surfaces must cross together'],..
			    'Input size',list('vec',1),exprs)
    if ~ok then break,end
    in=int(in)
    if in<=0 then
      message('Block must have at least one input')
    else
      kk=0
      for jj=1:in
	kk=kk+2^(in+jj-1)
      end
      model.rpar=[-ones(kk,1);zeros(2^(2*in)-kk,1)]
      graphics.exprs=exprs
      model.in=in
      model.nzcross=in
      model.firing=-1 //compatibility
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  rpar=[-1;-1;0;0]
  in=1

  model=scicos_model()
  model.sim=list('zcross',1)
  model.in=in
  model.nzcross=in
  model.evtout=1
  model.rpar=[-1;-1;0;0]
  model.blocktype='z'
  model.firing=-1
  model.dep_ut=[%t %f]
  
  exprs=strcat(sci2exp(in))
  gr_i=['xstringb(orig(1),orig(2),''Zcross'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
