function [x,y,typ]=DLR_f(job,arg1,arg2)
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
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);
  x0=model(7);ns=prod(size(x0));
  z=poly(0,'z')
  while %t do
    [ok,num,den,label]=getvalue('Set discrete SISO transfer parameters',..
	['Numerator (z)';
	 'Denominator (z)'],..
 	list('pol',1,'pol',1),label)
    if ~ok then break,end
    if degree(num)>degree(den) then
      message('Transfer must be proper')
      ok=%f
    end
    if ok then
      H=cont_frm(num,den)
      [A,B,C,D]=H(2:5);
      graphics(4)=label;
      [ns1,ns1]=size(A)
      if ns1<=ns then
	x0=x0(1:ns1)
      else
	x0(ns1,1)=0
      end
      rpar=[A(:);
	    B(:);
	    C(:);
	    D(:)]
      model(7)=x0
      model(8)=rpar
      if norm(D,1)<>0 then
	mmm=[%t %f];
      else
	mmm=[%f %f];
      end
      if or(model(12)<>mmm) then 
	  model(12)=mmm,end
      x(2)=graphics;x(3)=model
      break
    end
  end
  x(3)(11)=[] //compatibility
case 'define' then
  x0=0;A=-1;B=1;C=1;D=0;
  label=['1';'1+z']
  model=list('dsslti',1,1,1,[],[],x0(:),[A;B;C;D],[],'d',[],[%f %f],' ',list())
  gr_i=['txt=[''Num(z)'';''-----'';''Den(z)''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([2.5 2.5],model,label,gr_i)
end






