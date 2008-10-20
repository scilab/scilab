function [x,y,typ]=BOUNCE4_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  x=[];y=[];typ=[];
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  if size(exprs,'*')<9 then exprs(8)='9.81';exprs(9)='0';end
  while %t do
    [ok,rpar1,rpar2,walls,xt,xd,y,yd,g,C,exprs]=getvalue(['Set Bounce Block'],..
	    ['Mass';'Radius';'[xmin,xmax,ymin,ymax]';'xpos';'xdpos';'ypos';..
	     'ydpos';'g (gravity)';'C (aerodynamic coeff'],..
	    list('vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,..
		 'vec',-1,'vec',-1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    xt=xt(:);y=y(:);xd=xd(:);yd=yd(:);
    rpar1=rpar1(:);rpar2=rpar2(:);
    n=size(xt,'*');
    walls=walls(:);
    if walls(1)>walls(2) then walls=walls([2,1]);end
    if walls(3)>walls(3) then walls=walls([3,4]);end
    if n<>size(y,'*')|n<>size(rpar1,'*')|n<>size(rpar2,'*')|..
    n<>size(xd,'*')|n<>size(yd,'*') then
      message('All vectors must have equal size')
      ok=%f
    elseif  ~(min([rpar1;rpar2])>0) then
      message('Mass and radius must be >0')
      ok=%f
    end
    if ~ok then break,end
    [model,graphics,ok]=check_io(model,graphics,[],[n,n],[],[])
    if ok then
      k=1;ipar=[];
      for i=1:n
	for j=i+1:n
	  ipar(k)=i
	  k=k+1
	  ipar(k)=j
	  k=k+1
	end
      end
      model.rpar=[rpar1;rpar2;walls;g;C];
      model.ipar=ipar;
      state=[xt,xd,y,yd];state=state';
      model.state=state(:)
      model.nzcross=n*(n-1)/2+4*n;
      graphics.exprs=exprs;
      x.graphics=graphics;x.model=model
      break
    end
  end
 case 'define' then
  n=2
  k=1;ipar=[];
  for i=1:n
    for j=i+1:n
      ipar(k)=i
      k=k+1
      ipar(k)=j
      k=k+1
    end
  end
  walls=[0;5;0;5]
  x=[2;2.5];xd=[0;0];y=[3;5];yd=[0;0];g=9.81;C=0
  rpar1=ones(n,1);rpar2=rpar1;
  state=[x,xd,y,yd];state=state';
  model=scicos_model()
  model.sim=list('bounce4',4)
  model.in=[]
  model.out=[n;n]
  model.state=state(:)
  model.rpar=[rpar1;rpar2;walls;g;C];
  model.ipar=ipar;
  model.nzcross=n*(n-1)/2+4*n;
  model.blocktype='c'
  model.dep_ut=[%f %t]
  
  exprs=[strcat(sci2exp(rpar1));strcat(sci2exp(rpar2));strcat(sci2exp(walls));strcat(sci2exp(x));strcat(sci2exp(xd));strcat(sci2exp(y));strcat(sci2exp(yd))]
  gr_i=['txt=[''Bouncing'';''balls''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,exprs,gr_i)
end
endfunction

