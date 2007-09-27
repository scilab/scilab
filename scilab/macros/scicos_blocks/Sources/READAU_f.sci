function [x,y,typ]=READAU_f(job,arg1,arg2)
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
  out=model.out
  dstate=model.dstate
  ipar=model.ipar

  imask=9+ipar(1)
  tmask=ipar(imask)
  lunit=dstate(3)
  fname=exprs(1)

  while %t do
    [ok,fname1,N,swap,exprs]=getvalue(..
	['Set READAU block parameters';
	 'Read is done on a binary .au file'],..
	['Input file name';
	 'Buffer size';
	 'Swap mode 0/1'],..
	 list('str',1,'vec',1,'vec',1),..
	 exprs)
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
	if prod(size(dstate))<>(N*M)+3 then
	  dstate=[-1;-1;lunit;zeros(N*M,1)]
	end
	model.dstate=dstate;model.ipar=ipar
	graphics.exprs=exprs;
	x.graphics=graphics;x.model=model
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
  tmask=[]
  swap=0
  offset=1
  outmask=1
  ievt=0
  nout=size(outmask,'*')

  model=scicos_model()
  model.sim=list('readau',2)
  model.out=nout
  model.evtin=1
  model.dstate=[1;1;lunit;zeros(N*M,1)]
  model.ipar=[length(fname);str2code(frmt);ievt;N;M;swap;offset;str2code(fname);
	      tmask;outmask]
  model.blocktype='d'
  model.dep_ut=[%f %f]

  exprs=[fname;
	string(N);
	string(swap)]
  gr_i=['txt=[''read from .au'';''sound file''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([4 2],model,exprs,gr_i)
end
endfunction
