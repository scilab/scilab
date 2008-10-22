function [x,y,typ]=CLR(job,arg1,arg2)
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
  x0=model.state
  rpar=model.rpar
  ns=prod(size(x0));nin=1;nout=1
  s=poly(0,'s');z=poly(0,'s')
  while %t do
    [ok,num,den,exprs]=getvalue('Set continuous SISO transfer parameters',..
	['Numerator (s)';
	 'Denominator (s)'],..
	list('pol',1,'pol',1),exprs)


    if ~ok then break,end
    if degree(num)>degree(den) then
      message('Transfer function must be proper or strictly proper.')
      ok=%f
    end
    if ok then
      H=cont_frm(num,den)
      [A,B,C,D]=H(2:5);
      graphics.exprs=exprs;
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
      if or(model.dep_ut<>mmm) then 
	  model.dep_ut=mmm,end
      if ns1<=ns then
	x0=x0(1:ns1)
      else
	x0(ns1,1)=0
      end
      model.state=x0
      model.rpar=rpar
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  x0=0;A=-1;B=1;C=1;D=0;
  exprs=['1';'1+s']
  model=scicos_model()
  model.sim=list('csslti4',4)
  model.in=1
  model.out=1
  model.state=x0
  model.rpar=[A(:);B(:);C(:);D(:)]
  model.blocktype='c'
  model.dep_ut=[%f %t]

  gr_i=['xstringb(orig(1),orig(2),[''num(s)'';''den(s)''],sz(1),sz(2),''fill'')';
	'xpoly([orig(1)+.1*sz(1),orig(1)+.9*sz(1)],[1,1]*(orig(2)+sz(2)/2))']

  x=standard_define([2.5 2.5],model,exprs,gr_i)
end
endfunction
