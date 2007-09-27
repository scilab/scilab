function [x,y,typ]=READC_f(job,arg1,arg2)
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
  x=arg1;model=x.model;
  graphics=arg1.graphics;exprs=graphics.exprs
  out=model.out
  dstate=model.dstate
  ipar=model.ipar
  imask=9+ipar(1)
  tmask=ipar(imask)
  lunit=dstate(3)
  fname=exprs(3)
  frmt=exprs(4)
  while %t do
    [ok,tmask1,outmask,fname1,frmt1,M,N,offset,swap,exprs]=getvalue(..
	['Set READC block parameters';
	 'Read is done on a binary file'],..
	['Time record selection';
	 'Outputs record selection';
	 'Input file name';
	 'Input Format';
	 'Record size';
	 'Buffer size (in records)';
	 'Initial record index';
	 'Swap mode 0/1'],..
	 list('vec',-1,'vec',-1,'str',1,'str',1,'vec',1,'vec',1,'vec',1,'vec',1),..
	 exprs)
    if ~ok then break,end //user cancel modification
    fname1=stripblanks(fname1)
    frmt1=stripblanks(frmt1)
    fmts=['s','l','d','f','c','us','ul','uc','ull','uls','ubl','ubs',..
	   'dl','fl','ll','sl','db','fb','lb','sb']
    if prod(size(tmask1))>1 then
      message('Time record selection must be a scalar or an empty matrix')
    elseif and(frmt1<>fmts) then
      message(['Incorrect format, valid formats are:'
	      strcat(fmts,', ')])
    elseif alreadyran&fname1<>fname then
      message(['You cannot modify Output file name when running';'End current simulation first'])
    elseif N<>ipar(6)&alreadyran then
      message(['You cannot modify buffer length when running';'End current simulation first'])
    elseif alreadyran&size(tmask1)<>size(tmask) then
      message(['You cannot modify time management when running';'End current simulation first'])
    elseif N<1 then
      message('Buffer size must be at least 1')
    elseif swap<>0&swap<>1 then
      message('Swap mode must be 0 or 1')
    else
      if tmask1==[] then ievt=0;tmask1=0;outpt=[];else ievt=1;outpt=1;end
      out=size(outmask,'*')
      [model,graphics,ok]=check_io(model,graphics,[],out,1,outpt)
      frmt1=part(frmt1,1:3);
      if ok then
	if ievt==0 then
	  model.firing=-1
	else
	  model.firing=0
	end
	ipar=[length(fname1);
	    str2code(frmt1);
	    ievt;
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
  frmt='d  '
  fname='foo'
  lunit=0
  N=20;
  M=1
  rpar=[]
  tmask=0
  swap=0
  offset=1
  outmask=1
  ievt=0
  nout=size(outmask,'*')

  ipar=[length(fname);str2code(frmt);ievt;N;M;swap;offset;str2code(fname);
      tmask;outmask]
  model=scicos_model()
  model.sim=list('readc',2)
  model.out=nout
  model.evtin=1
  model.evtout=[]
  model.dstate=[1;1;lunit;zeros(N*M,1)]
  model.ipar=[length(fname);str2code(frmt);ievt;N;M;swap;offset;str2code(fname);
	      tmask;outmask]
  model.blocktype='d'
  model.firing=-1
  model.dep_ut=[%f %f]

  exprs=['[]';
	sci2exp(outmask);
	fname;
	frmt;
	string(M);
	string(N);
	string(offset);
	string(swap)]
  gr_i=['txt=[''read from'';''C binary file''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([3 2],model,exprs,gr_i)
end
endfunction
