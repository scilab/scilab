function [x,y,typ]=WRITEAU_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(o)
case 'getoutputs' then
  x=[];y=[];typ=[];
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);
  ipar=model(9);
  state=model(7)
  lunit=state(2)
  while %t do
    [ok,N,swap,label]=getvalue(..
	'Set WRITEC block parameters',..
	['Buffer size';
	'Swap mode 0/1'],..
	 list('vec',1,'vec',1),label)
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
      if prod(size(state))<>(nin+1)*N+2 then
	state=[-1;lunit;zeros((nin+1)*N,1)]
      end
      model(2)=1
      model(7)=state;model(9)=ipar
      graphics(4)=label;
      x(2)=graphics;x(3)=model
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
  rpar=[]
  ipar=[length(fname);str2code(frmt);N;swap;str2code(fname)]
  state=[-1;lunit;zeros((nin+1)*N,1)]
  model=list(list('writeau',2),in,[],1,[],[],state,rpar,ipar,'d',[],[%t %f],' ',list())
  label=[string(N)
	string(swap)]
  gr_i=['txt=[''write AU to'';''/dev/audio''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([3 2],model,label,gr_i)
end




