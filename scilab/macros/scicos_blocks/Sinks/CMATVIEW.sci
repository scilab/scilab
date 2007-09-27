function [x,y,typ]=CMATVIEW(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  x=[];y=[];typ=[];
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;
  exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,colormap,cmin,cmax,exprs]=getvalue(..
	'Set Scope parameters',..
	['ColorMap';
	'Minimum level range';
	'Maximum level range'],..
	 list('vec',-1,'vec',1,'vec',1),exprs)
    if ~ok then break,end //user cancel modification
    mess=[]
    if cmax<=cmin then
      mess=[mess;'Error with minimum and maximum value';' ']
      ok=%f
    end
    if ~ok then
      message(['Some specified values are inconsistent:';' ';mess])
    end
    if ok then
	size_c=size(colormap(:),1);
        sol=inv([cmin 1;cmax 1])*[1;size_c/3];
        alpha_c = sol(1);
        beta_c = sol(2);
	ipar=[cmin;cmax;size_c];
  	rpar=[alpha_c;beta_c;colormap(:)];
	model.ipar=ipar;
	model.rpar=rpar;
	graphics.exprs=exprs;
	x.graphics=graphics;
        x.model=model
	break
    end
  end
case 'define' then
  cmin = 0;
  cmax = 100;
  size_c = 25;
  colormap = jetcolormap(size_c);
  alpha_c = 0.24
  beta_c = 1

  model=scicos_model()
  model.sim=list('cmatview',4)
  model.in=-1
  model.in2=-2
  model.intyp=1
  model.evtin=1
  model.ipar=[cmin;cmax;size_c]
  model.rpar=[alpha_c;beta_c;colormap(:)]
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[string('jetcolormap(25)');
	 string(cmin);
	 string(cmax)];
   gr_i='xstringb(orig(1),orig(2),''Mat. View'',sz(1),sz(2),''fill'')'
   x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
