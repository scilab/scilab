function [x,y,typ]=BOUNCEXY_f(job,arg1,arg2)
//Scicos 2D animated visualization block
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(o)
case 'getoutputs' then
  x=[];y=[];typ=[];
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;dstate=model.dstate
  while %t do
    [ok,clrs,siz,win,imode,xmin,xmax,ymin,ymax,exprs]=getvalue(..
	'Set Scope parameters',..
	['colors';
	 'radii';
	 'window number';
	 'animation mode (0,1)';
	'Xmin';
	'Xmax';
	'Ymin';
	'Ymax';
	],..
	 list('vec',-1,'vec',-1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end //user cancel modification

    mess=[]
    if size(clrs,'*')<>size(siz,'*') then
      mess=[mess;'colors and radii must have equal size (number of balls)';' ']
      ok=%f
    end
    if win<0 then
      mess=[mess;'Window number cannot be negative';' ']
      ok=%f
    end
    if ymin>=ymax then
      mess=[mess;'Ymax must be greater than Ymin';' ']
      ok=%f
    end
    if xmin>=xmax then
      mess=[mess;'Xmax must be greater than Xmin';' ']
      ok=%f
    end
    if ~ok then
      message(mess)
    else
      rpar=[xmin;xmax;ymin;ymax]
      ipar=[win;imode;clrs(:)]
      z=[]
      for i=1:size(clrs,'*')
	z(6*(i-1)+1)=0
	z(6*(i-1)+2)=0
	z(6*(i-1)+3)=2*siz(i)
	z(6*(i-1)+4)=2*siz(i)
	z(6*(i-1)+5)=0.000
	z(6*(i-1)+6)=64.0*360.000;
      end
      model.dstate=z;model.rpar=rpar;model.ipar=ipar
      graphics.exprs=exprs;
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  win=1; imode=1;clrs=[1;2];
  siz=[1;1]
  xmin=-5;xmax=5;ymin=0;ymax=15

  model=scicos_model()
  model.sim=list('bouncexy',1)
  model.in=[-1;-1]
  model.evtin=1
  z=[]
  for i=1:size(clrs,'*')
    z(6*(i-1)+1)=0
    z(6*(i-1)+2)=0
    z(6*(i-1)+3)=2*siz(i)
    z(6*(i-1)+4)=2*siz(i)
    z(6*(i-1)+5)=0.000
    z(6*(i-1)+6)=64.0*360.000;
  end
  model.dstate=z
  model.rpar=[xmin;xmax;ymin;ymax]
  model.ipar=[win;imode;clrs(:)]
  model.blocktype='d'
  model.firing=[]
  model.dep_ut=[%f %f]
 
  exprs=[strcat(sci2exp(clrs));
	 strcat(sci2exp(siz));
	  strcat(sci2exp(win));
	 strcat(sci2exp(1));
	strcat(sci2exp(xmin));
      strcat(sci2exp(xmax));
      strcat(sci2exp(ymin));
      strcat(sci2exp(ymax))]
  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
        't=(0:0.3:2*%pi)'';';
        'xx=orig(1)+(1/5+(cos(2.2*t)+1)*3/10)*sz(1);';
        'yy=orig(2)+(1/4.3+(sin(t)+1)*3/10)*sz(2);';
        'xpoly(xx,yy,''lines'');'
        'xset(''thickness'',thick);']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction

