function [x,y,typ]=WFILE_f(job,arg1,arg2)
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
  state=model(7)
  lunit=state(2)
  fname=label(2)
  frmt=label(3)
  while %t do
    [ok,in,fname1,frmt1,N,label]=getvalue(..
	'Set WFILE block parameters',..
	['Input size';
	'Output file name';
	'Output Format';
	'Buffer size'],..
	 list('vec',1,'str',1,'str',1,'vec',1),label)
    if ~ok then break,end //user cancel modification
    in=int(in)
    nin=in

    fname1=stripblanks(fname1)
    frmt1=stripblanks(frmt1)
    mess=[]
    if lunit>0&min(length(frmt),1)<>min(length(frmt1),1) then
      mess=[mess;'You cannot swich from formatted to unformatted';
	         'or  from unformatted to formatted when running';' ']
    end
    if lunit>0&fname1<>fname then
      mess=[mess;'You cannot modify Output file name when running';' ']
      ok=%f
    end
    if N<2 then
      mess=[mess;'Buffer size must be at least 2';' ']
      ok=%f
    end
    if in<=0 then
      mess=[mess;'Block must have at least one input';' ']
      ok=%f
    end
    if ~ok then
      message(['Some specified values are inconsistent:';
	         ' ';mess])
    end

    if ok then
      ipar=[length(fname1);length(frmt1);0;N;str2code(fname1);str2code(frmt1)]
      if prod(size(state))<>(nin+1)*N+2 then
	state=[-1;lunit;zeros((nin+1)*N,1)]
      end
      model(2)=nin
      model(7)=state;model(9)=ipar
      model(11)=[] //compatibility
      model(12)=[%t %f] //compatibility
      graphics(4)=label;
      x(2)=graphics;x(3)=model
      break
    end
  end
case 'define' then
  in=1;nin=sum(in)
  frmt='(7(e10.3,1x))'
  fname='foo'
  lunit=0
  N=2;
  rpar=[]
  ipar=[length(fname);length(frmt);0;N;str2code(fname);str2code(frmt)]
  state=[-1;lunit;zeros((nin+1)*N,1)]
  model=list('writef',in,[],1,[],[],state,rpar,ipar,'d',[],[%t %f],' ',list())
  label=[sci2exp(in);
	fname;
	frmt;
	string(N)]
  gr_i=['txt=[''write to'';''output file''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([3 2],model,label,gr_i)
end




