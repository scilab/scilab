function [x,y,typ]=EVENTSCOPE_f(job,arg1,arg2)
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
  model=arg1.model;
  while %t do
    [ok,nclock,clrs,win,wpos,wdim,per,exprs]=getvalue(..
	'Set Scope parameters',..
	['Number of event inputs';
	'colors c (>0) or mark (<0)';
	'Output window number';
	'Output window position';
	'Output window sizes';	
	'Refresh period'],..
	 list('vec',1,'vec',-1,'vec',1,'vec',-1,'vec',-1,'vec',1),exprs);
    nclock=int(nclock)
    clrs=int(clrs)
    win=int(win)
    
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
    if nclock<=0 then
      mess=[mess;'Block must have at least one input event';' ']
      ok=%f
    end
    if size(clrs,'*')<>nclock then
      mess=[mess;'Inputs color c size must be equal to Number of inputs';' ']
      ok=%f
    end
    if win<0 then
      mess=[mess;'Window number can''t be negative';' ']
      ok=%f
    end
    if per<=0 then
      mess=[mess;'Refresh period must be positive';' ']
      ok=%f
    end
    if ok then
      [model,graphics,ok]=check_io(model,graphics,[],[],ones(nclock,1),[])
    else
      message(['Some specified values are inconsistent:';
	         ' ';mess])
    end
    if ok then
      if wpos==[] then wpos=[-1;-1];end
      if wdim==[] then wdim=[-1;-1];end
      rpar=per
      ipar=[win;1;clrs(:);wpos(:);wdim(:)]
      model.rpar=rpar;model.ipar=ipar
      graphics.exprs=exprs;
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  nclock=1
  win=1; clrs=[1;3;5;7;9;11;13;15];
  wdim=[600;400]
  wpos=[-1;-1]
  per=30;

  model=scicos_model()
  model.sim='evscpe'
  model.evtin=1
  model.dstate=-1
  model.rpar=per
  model.ipar=[win;1;clrs(nclock);wpos(:);wdim(:)]
  model.blocktype='d'
  model.dep_ut=[%f %f]
  
  exprs=[sci2exp(nclock);
	strcat(sci2exp(clrs(nclock)),' ');
	string(win);
	sci2exp([]);
	sci2exp(wdim);	
	string(per)]
  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
    'xrect(orig(1)+sz(1)/10,orig(2)+(1-1/10)*sz(2),sz(1)*8/10,sz(2)*8/10);';
    'xx=[orig(1)+sz(1)/5,orig(1)+sz(1)/5;';
    'orig(1)+(1-1/5)*sz(1),orig(1)+sz(1)/5];';
    'yy=[orig(2)+sz(2)/5,orig(2)+sz(2)/5;';
    'orig(2)+sz(2)/5,orig(2)+(1-1/5)*sz(2)];';
    'xarrows(xx,yy);';
    't=(0:0.3:2*%pi)'';';
    'xx=orig(1)+(1/5+3*t/(10*%pi))*sz(1);';
    'yy=orig(2)+(1/4.3+(sin(t)+1)*3/10)*sz(2);';
    'xpoly(xx,yy,''lines'');';
    'xset(''thickness'',thick);']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
