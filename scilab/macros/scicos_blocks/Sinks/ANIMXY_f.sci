function [x,y,typ]=ANIMXY_f(job,arg1,arg2)
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
  if size(exprs,'*')==8 then exprs=[exprs(1:3);'[]';'[]';exprs(4:8)],end
  while %t do
    [ok,clrs,siz,win,wpos,wdim,xmin,xmax,ymin,ymax,N,exprs]=getvalue(..
	'Set Scope parameters',..
	['color (<0) or mark (>0)';
	'line or mark size';
	'Output window number';
	'Output window position';
	'Output window sizes';
	'Xmin';
	'Xmax';
	'Ymin';
	'Ymax';
	'Buffer size'],..
	 list('vec',1,'vec',1,'vec',1,'vec',-1,'vec',-1,'vec',1,..
	 'vec',1,'vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end //user cancel modification

    mess=[]
    if size(wpos,'*')<>0 &size(wpos,'*')<>2 then
      mess=[mess;'Window position must be [] or a 2 vector';' ']
      ok=%f
    end
    if size(wdim,'*')<>0 &size(wdim,'*')<>2 then
      mess=[mess;'Window dim must be [] or a 2 vector';' ']
      ok=%f
    end
    if win<0 then
      mess=[mess;'Window number cannot be negative';' ']
      ok=%f
    end
    if N<1&clrs<0 then
      mess=[mess;'Buffer size must be at least 1';' ']
      ok=%f
    end
    if N<2&clrs>0 then
      mess=[mess;'Buffer size must be at least 2';' ']
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
      if wpos==[] then wpos=[-1;-1];end
      if wdim==[] then wdim=[-1;-1];end
      rpar=[xmin;xmax;ymin;ymax]
      ipar=[win;1;N;clrs;siz;0;wpos(:);wdim(:)]
      if prod(size(dstate))<>2*N+1 then dstate=zeros(2*N+1,1),end
      model.dstate=dstate;model.rpar=rpar;model.ipar=ipar
      graphics.exprs=exprs;
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  win=1; clrs=-4;
  N=2; siz=1;
  wpos=[-1;-1];wdim=[-1;-1];
  xmin=-15;xmax=15;ymin=-15;ymax=+15

  model=scicos_model()
  model.sim='scopxy'
  model.in=[1;1]
  model.evtin=1
  model.dstate=zeros(2*N+1,1)
  model.rpar=[xmin;xmax;ymin;ymax]
  model.ipar=[win;1;N;clrs;siz;0;wpos(:);wdim(:)]
  model.blocktype='d'
  model.firing=[]
  model.dep_ut=[%f %f]
 
  exprs=[string(clrs);
      string(siz);
      string(win);
      '[]';
      '[]';
      string(xmin);
      string(xmax);
      string(ymin);
      string(ymax);
      string(N)]
  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
        't=(0:0.3:2*%pi)'';';
        'xx=orig(1)+(1/5+(cos(2.2*t)+1)*3/10)*sz(1);';
        'yy=orig(2)+(1/4.3+(sin(t)+1)*3/10)*sz(2);';
        'xpoly(xx,yy,''lines'');'
        'xset(''thickness'',thick);']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
