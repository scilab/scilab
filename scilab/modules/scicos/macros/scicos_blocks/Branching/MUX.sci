function [x,y,typ]=MUX(job,arg1,arg2)
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
    [ok,in,exprs]=getvalue('Set MUX block parameters',..
	'number of input ports or vector of sizes',list('vec',-1),exprs)
    if ~ok then break,end
    if size(in,'*')==1 then
      if in<2|in>31 then
	message('Block must have at least two input ports and at most 31')
	ok=%f
      else
	[model,graphics,ok]=check_io(model,graphics,-[1:in]',0,[],[])
      end
    else
      if size(in,'*')<2| or(in==0)|size(in,'*')>31 then
	message(['Block must have at least two input ports';
		 'and at most 31. Size 0 is not allowed. '])
	ok=%f
      else
	if min(in)<0 then nout=0,else nout=sum(in),end
	[model,graphics,ok]=check_io(model,graphics,in(:),nout,[],[])
	if ok then in=size(in,'*'),end
      end
    end
    if ok then
      graphics.exprs=exprs;model.ipar=in
      x.graphics=graphics;x.model=model
      break
    end
  end
 case 'define' then
  in=2
  model=scicos_model()
  model.sim=list('multiplex',4)
  model.in=-[1:in]'
  model.out=0
  model.ipar=in
  model.blocktype='c'
  model.dep_ut=[%t %f]

  exprs=string(in)
  gr_i=' ' //'xstringb(orig(1),orig(2),''Mux'',sz(1),sz(2),''fill'')'
  x=standard_define([.5 2],model,exprs,gr_i)
  x.graphics.id="Mux"
end
endfunction
