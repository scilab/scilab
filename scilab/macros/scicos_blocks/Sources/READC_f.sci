function [x,y,typ]=READC_f(job,arg1,arg2)
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
  fname=label(3)
  frmt=label(4)
  while %t do
    [ok,tmask1,outmask,fname1,frmt1,M,N,offset,swap,label]=getvalue(..
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
	 label)
    if ~ok then break,end //user cancel modification
    fname1=stripblanks(fname1)
    frmt1=stripblanks(frmt1)
    fmts=['s','l','d','f','c','us','ul','uc','ull','uls','ubl','ubs',..
	   'dl','fl','ll','sl','db','fb','lb','sb']
    if prod(size(tmask1))>1 then
      message('Time record selection must be a scalar or an empty matrix')
    elseif and(frmt1<>fmts)then
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
      if tmask1==[] then ievt=0;tmask1=0;else ievt=1;end
      out=size(outmask,'*')
      [model,graphics,ok]=check_io(model,graphics,[],out,1,1)
      frmt1=part(frmt1,1:3);
      if ok then
	if ievt==0 then
	  model(11)=-1
	else
	  model(11)=0
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
  frmt='d  '
  fname='foo'
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
  model=list(list('readc',2),[],nout,1,[1],[],state,rpar,ipar,'d',-1,[%f %f],' ',list())
  label=['[]';
	sci2exp(outmask);
	fname;
	frmt;
	string(M);
	string(N);
	string(offset);
	string(swap)]
  gr_i=['txt=[''read from'';''C binary file''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([3 2],model,label,gr_i)
end




