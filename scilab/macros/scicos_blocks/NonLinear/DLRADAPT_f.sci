function [x,y,typ]=DLRADAPT_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
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
  while %t do
    [ok,p,rn,rd,g,last_u,last_y,label]=getvalue('Set block parameters',..
	['Vector of p mesh points';
	'Numerator roots (one line for each mesh)';
	'Denominator roots (one line for each mesh)';
        'Vector of gain at mesh points' ;
	'past inputs (Num degree values)';
	'past outputs (Den degree values)'],..
	list('vec',-1,'mat',[-1,-1],'mat',..
	['size(x1,''*'')','-1'],'vec','size(x1,''*'')',..
	'vec','size(x2,2)','vec','size(x3,2)'),label)


    if ~ok then break,end
    m=size(rn,2)
    [npt,n]=size(rd)
    if m>=n then
      message('Transfer must be strictly proper'),
    elseif size(rn,1)<>0&size(rn,1)<>size(p,'*') then
      message('Numerator roots matrix row size''s is incorrect')
    else
      rpar=[p(:);real(rn(:));imag(rn(:));real(rd(:));imag(rd(:));g(:)]
      ipar=[m;n;npt]
      model(7)=[last_u(:);last_y(:)]
      model(8)=rpar
      model(9)=ipar
      graphics(4)=label
      x(2)=graphics;x(3)=model
      break;
    end
  end
  x(3)(11)=[] //compatibility
case 'define' then
  p=[0;1]
  rn=[]
  rd=[0.2+0.8*%i,0.2-0.8*%i;0.3+0.7*%i,0.3-0.7*%i]
  g=[1;1]
  rpar=[p(:);real(rn(:));imag(rn(:));real(rd(:));imag(rd(:));g(:)]
  ipar=[0;2;2]
  last_u=[]
  last_y=[0;0]
  model=list('dlradp',[1;1],1,1,[],[],[last_u;last_y],rpar,ipar,'d',[],[%t %f],' ',list())
  label=[sci2exp(p);
	sci2exp(rn);
	sci2exp(rd,0);
	sci2exp(g);
	sci2exp(last_u);
	sci2exp(last_y)]
  gr_i=['txt=[''N(z,p)'';''-----'';''D(z,p)''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end


