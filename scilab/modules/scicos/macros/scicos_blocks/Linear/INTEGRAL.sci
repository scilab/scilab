function [x,y,typ]=INTEGRAL(job,arg1,arg2)
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
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,x0,reinit,satur,maxp,lowp,exprs]=getvalue('Set Integral block parameters',..
	['Initial Condition';
	 'With re-intialization (1:yes, 0:no)';'With saturation (1:yes, 0:no)';
	 'Upper limit';'Lower limit'],..
		list('vec',-1,'vec',1,'vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    x0=x0(:);maxp=maxp(:),lowp=lowp(:)
    if satur<>0 then satur=1;end
    if reinit<>0 then 
      reinit=1;
      if size(maxp,'*')==1 then maxp=maxp*ones(x0),end
      if size(lowp,'*')==1 then lowp=lowp*ones(x0),end
    end
    if reinit&(size(x0,1)<>size(maxp,1) | size(x0,1)<>size(lowp,1)) then
      message('x0 and Upper limit and Lower limit must have same size')
      ok=%f
    elseif or(maxp<=lowp)  then
      message('Upper limits must be > Lower limits')
      ok=%f
    else
      rpar=[maxp;lowp]
      model.rpar=rpar
      model.state=x0
      if satur<>0 then 
	model.nzcross=size(x0,1)
	model.nmode=1
      else
	model.nzcross=0
	model.nmode=0
      end
      if ok then
	[model,graphics,ok]=check_io(model,graphics,..
		size(x0,1)*[1;ones(reinit,1)],size(x0,1),ones(reinit,1),[])
      end	   
      if ok then
	graphics.exprs=exprs
	x.graphics=graphics;x.model=model
	break
      end
    end
  end
case 'define' then
  minp=-1;maxp=1;rpar=[maxp;minp]
  model=scicos_model()
  model.state=0
  model.sim=list('integral_func',4)
  model.in=1
  model.out=1
  model.rpar=rpar
  model.blocktype='c'
  model.dep_ut=[%f %t]
  
  exprs=string([0;0;0;maxp;minp])
  gr_i=['thick=xget(''thickness'')'
'pat=xget(''pattern'')'
'fnt=xget(''font'')'
'xpoly(orig(1)+[0.7;0.62;0.549;0.44;0.364;0.291]*sz(1),orig(2)+[0.947;0.947;0.884;0.321;0.255;0.255]*sz(2),"'lines"')'
'xset(''thickness'',thick)'
'xset(''pattern'',pat)'
'xset(''font'',fnt(1),fnt(2))'
	]
  x=standard_define([2 2],model,exprs,gr_i)
  x.graphics.id="1/s"
end
endfunction

