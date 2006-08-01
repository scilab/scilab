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
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  x0=model.dstate;ns=prod(size(x0));
  z=poly(0,'z')
  while %t do
    [ok,num,den,exprs]=getvalue('Set discrete SISO transfer parameters',..
	['Numerator (z)';
	 'Denominator (z)'],..
 	list('pol',1,'pol',1),exprs)
    if ~ok then break,end
    if degree(num)>degree(den) then
      message('Transfer must be proper')
      ok=%f
    end
    if ok then
      H=cont_frm(num,den)
      [A,B,C,D]=H(2:5);
      graphics.exprs=exprs;
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
      model.dstate=x0
      model.rpar=rpar
      if norm(D,1)<>0 then
	mmm=[%t %f];
      else
	mmm=[%f %f];
      end
      if or(model.dep_ut<>mmm) then 
	  model.dep_ut=mmm,end
      x.graphics=graphics;x.model=model
      break
    end
  end
  x.model.firing=[] //compatibility
case 'define' then
  x0=0;A=-1;B=1;C=1;D=0;
  exprs=['1';'1+z']
  
  model=scicos_model()
  model.sim='dsslti'
  model.in=1
  model.out=1
  model.evtin=1
  model.dstate=x0(:)
  model.rpar=[A(:);B(:);C(:);D(:)]
  model.blocktype='d'
  model.dep_ut=[%f %f]

  gr_i=['xstringb(orig(1),orig(2),[''num(z)'';''den(z)''],sz(1),sz(2),''fill'')';
	'xpoly([orig(1)+.1*sz(1),orig(1)+.9*sz(1)],[1,1]*(orig(2)+sz(2)/2))']
  x=standard_define([2.5 2.5],model,exprs,gr_i)
end
endfunction
