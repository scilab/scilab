function [x,y,typ]=CLSS_f(job,arg1,arg2)
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
  if size(label,'*')==7 then label=label([1:4 7]),end //compatibility
  model=arg1(3);
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
      [model,graphics,ok]=check_io(model,graphics,in,out,[],[])
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
	model(6)=x0(:);model(8)=rpar
	x(2)=graphics;x(3)=model
	break
      end
    end
  end
  x(3)(11)=[] //comptibility
case 'define' then
  x0=0;A=-1;B=1;C=1;D=0;in=1;out=1
  model=list(list('csslti',1),in,out,[],[],x0,[],[A;B;C;D],[],'c',[],[%f %t],' ',list())
  label=[strcat(sci2exp(A));
	    strcat(sci2exp(B));
	    strcat(sci2exp(C));
	    strcat(sci2exp(D));
	    strcat(sci2exp(x0))]
  gr_i=['txt=[''xd=Ax+Bu'';''y=Cx+Du''];';
        'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,label,gr_i)
end




