function [x,y,typ]=TCLSS_f(job,arg1,arg2)
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
  x=arg1
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);
  if size(label,'*')==7 then label=label([1:4,7]),end //compatibility
  while %t do
   [ok,A,B,C,D,x0,label]=getvalue('Set continuous linear system parameters',..
	['A matrix';
	'B matrix';
	'C matrix';
	'D matrix';
	'Initial state'],..
	 list('mat',[-1,-1],..
	    'mat',['size(x1,2)','-1'],..
	    'mat',['-1','size(x1,2)'],..
	    'mat',[-1 -1],..
	    'vec','size(x1,2)'),..
	    label)
    if ~ok then break,end
    out=size(C,1);if out==0 then out=[],end
    in=size(B,2);if in==0 then in=[],end
    [ms,ns]=size(A)
    if ms<>ns then
      message('A matrix must be square')
    else
      [model,graphics,ok]=check_io(model,graphics,[in;ms],out,1,[])
      if ok then
	graphics(4)=label;
	rpar=[A(:);B(:);C(:);D(:)];
	if D<>[] then	
	  if norm(D,1)<>0 then
	    mmm=[%t %t];
	  else
	    mmm=[%f %t];
	  end
	  if or(model(12)<>mmm) then 
	      model(12)=mmm,end
	else
	  model(12)=[%f %t];
	end
	model(6)=x0(:);model(8)=rpar;
	if D<>[] then 
	  model(1)=list('tcslti',1) 
	else 
	  model(1)=list('tcsltj',1);
	end
	model(11)=[] //compatibility
	x(2)=graphics;x(3)=model
	break
      end
    end
  end
case 'define' then
  x0=0;A=0;B=1;C=1;D=0;
  in=1;
  nx=size(x0,'*')
  out=1
  model=list(list('tcslti',1),[in;nx],out,1,[],x0,[],[A;B;C;D],[],'c',[],[%f %t],' ',list())
  label=[strcat(sci2exp(A));
	strcat(sci2exp(B));
	strcat(sci2exp(C));
	strcat(sci2exp(D));
	strcat(sci2exp(x0))]
  gr_i=['txt=[''Jump'';''(A,B,C,D)''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([3 2],model,label,gr_i)
end




