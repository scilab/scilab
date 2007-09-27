function [x,y,typ]=WRITEAU_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  x=[];y=[];typ=[];
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  ipar=model.ipar;
  dstate=model.dstate
  lunit=dstate(2)
  while %t do
    [ok,N,swap,exprs]=getvalue(..
	'Set WRITEAU block parameters',..
	['Buffer size';
	'Swap mode 0/1'],..
	 list('vec',1,'vec',1),exprs)
    if ~ok then break,end //user cancel modification

    nin=1

    fname1='/dev/audio'
    frmt1='uc '
    mess=[]
    if alreadyran&(N<>ipar(5)) then
      mess=[mess;['You cannot modify buffer size when running';'End current simulation first']];
      ok=%f
    end
    if N<1 then
      mess=[mess;'Buffer size must be at least 1';' ']
      ok=%f
    end
    if swap<>0&swap<>1 then
      mess=[mess;'Swap mode must be 0 or 1'];ok=%f
    end
    if ~ok then
      message(['Some specified values are inconsistent:';
	  ' ';mess])
    end

    if ok then
      ipar=[length(fname1);str2code(frmt1);N;swap;str2code(fname1)]
      if prod(size(dstate))<>(nin+1)*N+2 then
	dstate=[-1;lunit;zeros((nin+1)*N,1)]
      end
      model.in=1
      model.dstate=dstate;model.ipar=ipar
      graphics.exprs=exprs;
      x.graphics=graphics;x.model=model
      break
    end
    
  end
case 'define' then
  in=1;nin=sum(in)
  frmt='uc '
  fname='/dev/audio'
  swap=0
  lunit=0
  N=2;

  model=scicos_model()
  model.sim=list('writeau',2)
  model.in=in
  model.evtin=1
  model.dstate=[-1;lunit;zeros((nin+1)*N,1)]
  model.ipar=[length(fname);str2code(frmt);N;swap;str2code(fname)]
  model.blocktype='d'
  model.dep_ut=[%t %f]
  
  exprs=[string(N)
	 string(swap)]
  gr_i=['txt=[''write AU to'';''/dev/audio''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([3 2],model,exprs,gr_i)
end
endfunction
