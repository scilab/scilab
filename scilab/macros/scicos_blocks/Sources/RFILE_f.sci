function [x,y,typ]=RFILE_f(job,arg1,arg2)
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
  nout=sum(out)
  ievt=ipar(3);N=ipar(4);
  imask=5+ipar(1)+ipar(2)
  tmask=ipar(imask)
  outmask=ipar(imask+1:imask+nout)
  lunit=state(3)
  fname=label(3)
  frmt=label(4)
  while %t do
    [ok,tmask1,outmask,fname1,frmt1,N,out,label]=getvalue(..
	['Set RFILE block parameters';
	 'Read is done on';
	 '  -  a binary file if no format given';
	 '  -  a formatted file if a  format (fortran type) is given'],..
	['Time record selection';
	 'Outputs record selection';
	 'Input file name';
	 'Input Format';
	 'Buffer size';
	 'Output size'],..
	 list('vec',-1,'vec',-1,'str',1,'str',1,'vec',1,'vec',1),..
	 label)
    if ~ok then break,end //user cancel modification
    fname1=stripblanks(fname1)
    frmt1=stripblanks(frmt1)
    out=int(out)
    nout=out
    if prod(size(tmask1))>1 then
      message('Time record selection must be a scalar or an empty matrix')
    elseif lunit>0&min(length(frmt),1)<>min(length(frmt1),1) then
      message(['You cannot swich from formatted to unformatted';
	         'or  from unformatted to formatted when running';' '])
    elseif lunit>0&fname1<>fname then
      message('You cannot modify Output file name when running')
    elseif lunit>0&size(tmask1)<>size(tmask) then
      message('You cannot modify time management when running')
    elseif N<2 then
      message('Buffer size must be at least 2')
    elseif out==[] then
      message('Block must have at least one output port')
    else
      if tmask1==[] then ievt=0;cout=[];tmask1=0;else ievt=1,cout=1;end
      [model,graphics,ok]=check_io(model,graphics,[],out,1,cout)
      if ok then
	if ievt==0 then
	  model(11)=[]
	else
	  model(11)=0
	end
	ipar=[length(fname1);
	    length(frmt1);
	    ievt;
	    N;
	    str2code(fname1);
	    str2code(frmt1);
	    tmask1
	    outmask(:)]
	if prod(size(state))<>(nout+ievt)*N+3 then
	  state=[-1;-1;lunit;zeros((nout+ievt)*N,1)]
	end
	model(7)=state;model(9)=ipar
	graphics(4)=label;
	x(2)=graphics;x(3)=model
	break
      end
    end
  end
case 'define' then
  out=1;nout=sum(out)
  frmt='(7(e10.3,1x))'
  fname='foo'
  lunit=0
  N=2;
  rpar=[]
  tmask=0
  outmask=1
  ipar=[length(fname);length(frmt);0;N;str2code(fname);str2code(frmt);
      tmask;outmask]
  state=[1;1;lunit;zeros((nout)*N,1)]
  model=list('readf',[],nout,1,[],[],state,rpar,ipar,'d',[],[%f %f],' ',list())
  label=[sci2exp([]);
	sci2exp(outmask);
	fname;
	frmt;
	string(N);
	sci2exp(out)]
  gr_i=['txt=[''read from'';''input file''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([3 2],model,label,gr_i)
end




