function [x,y,typ]=READAU_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(o)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(o)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);[out,state,ipar]=model([3 7 9])
  imask=9+ipar(1)
  tmask=ipar(imask)
  lunit=state(3)
  fname=label(1)

  while %t do
    [ok,fname1,N,swap,label]=getvalue(..
	['Set READAU block parameters';
	 'Read is done on a binary .au file'],..
	['Input file name';
	 'Buffer size';
	 'Swap mode 0/1'],..
	 list('str',1,'vec',1,'vec',1),..
	 label)
     tmask1=[];outmask=1;frmt1='uc';M=1;offset=1;
    if ~ok then break,end //user cancel modification
    fname1=stripblanks(fname1)
    frmt1=stripblanks(frmt1)
    if alreadyran&fname1<>fname then
      message(['You cannot modify Output file name when running';'End current simulation first'])
    elseif alreadyran&size(tmask1)<>size(tmask) then
      message(['You cannot modify time management when running';'End current simulation first'])
    elseif N<1 then
      message('Buffer size must be at least 1')
    elseif alreadyran&(N<>ipar(6)) then
      message(['You cannot modify buffer size when running';'End current simulation first'])
    elseif swap<>0&swap<>1 then
      message('Swap mode must be 0 or 1')
    else
      [model,graphics,ok]=check_io(model,graphics,[],1,1,[])
      frmt1=part(frmt1,1:3);
      if ok then
	ipar=[length(fname1);
	    str2code(frmt1);
	    0;
	    N;
	    M;
	    swap;
	    offset
	    str2code(fname1);
	    tmask1
	    outmask(:)]
	if prod(size(state))<>(N*M)+3 then
	  state=[-1;-1;lunit;zeros(N*M,1)]
	end
	model(7)=state;model(9)=ipar
	graphics(4)=label;
	x(2)=graphics;x(3)=model
	break
      end
    end
  end
case 'define' then
  frmt='uc '
  fname='test.au'
  lunit=0
  N=20;
  M=1
  rpar=[]
  tmask=[]
  swap=0
  offset=1
  outmask=1
  ievt=0
  nout=size(outmask,'*')
  ipar=[length(fname);str2code(frmt);ievt;N;M;swap;offset;str2code(fname);
      tmask;outmask]
  state=[1;1;lunit;zeros(N*M,1)]
  model=list(list('readau',2),[],nout,1,[],[],state,rpar,ipar,'d',[],[%f %f],' ',list())
  label=[fname;
	string(N);
	string(swap)]
  gr_i=['txt=[''read from .au'';'' sound binary file''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([4 2],model,label,gr_i)
end




