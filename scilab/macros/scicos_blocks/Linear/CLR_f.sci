function [x,y,typ]=CLR_f(job,arg1,arg2)
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
  [x0,rpar]=model([6 8])
  ns=prod(size(x0));nin=1;nout=1
  s=poly(0,'s');z=poly(0,'s')
  while %t do
    [ok,num,den,label]=getvalue('Set continuous SISO transfer parameters',..
	['Numerator (s)';
	 'Denominator (s)'],..
	list('pol',1,'pol',1),label)


    if ~ok then break,end
    if degree(num)>degree(den) then
      message('Transfer must be proper or strictly proper')
      ok=%f
    end
    if ok then
      H=cont_frm(num,den)
      [A,B,C,D]=H(2:5);
      graphics(4)=label;
      [ns1,ns1]=size(A)
      rpar=[matrix(A,ns1*ns1,1);
	    matrix(B,ns1,1);
	    matrix(C,ns1,1);
	    D]
      if norm(D,1)<>0 then
	mmm=[%t %t];
      else
	mmm=[%f %t];
      end
      if or(model(12)<>mmm) then 
	  model(12)=mmm,end
      if ns1<=ns then
	x0=x0(1:ns1)
      else
	x0(ns1,1)=0
      end
      model(6)=x0
      model(8)=rpar
      x(2)=graphics;x(3)=model
      break
    end
  end
  x(3)(11)=[] //compatibility
case 'define' then
  x0=0;A=-1;B=1;C=1;D=0;
  label=['1';'1+s']
  model=list(list('csslti',1),1,1,[],[],x0,[],[A;B;C;D],[],'c',[],[%f %t],' ',list())
  gr_i=['xstringb(orig(1),orig(2),[''Num(s)'';''-----'';''Den(s)''],sz(1),sz(2),''fill'');']
  x=standard_define([2.5 2.5],model,label,gr_i)
end




