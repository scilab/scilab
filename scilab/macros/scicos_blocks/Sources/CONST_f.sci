function [x,y,typ]=CONST_f(job,arg1,arg2)
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
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);
  while %t do
    [ok,C,label]=getvalue(['Set Contant Block'],..
	    'Contants',list('vec',-1),label)
    if ~ok then break,end
    nout=size(C,'*')
    if nout==0 then
      message('C must have at least one element')
    else
      model(8)=C(:);model(3)=nout
      graphics(4)=label;
      x(2)=graphics;x(3)=model
      break;
    end
  end
  x(3)(11)=[] //compatibility
case 'define' then
  C=1
  out=1
  rpar=C
  model=list(list('cstblk',1),[],out,[],[],[],[],rpar,[],'d',..
      [],[%f %f],' ',list())
  label=[strcat(sci2exp(C))]
  gr_i=['model=arg1(3);C=string(model(8));';
    'dx=sz(1)/5;dy=sz(2)/10;';
    'w=sz(1)-2*dx;h=sz(2)-2*dy;';
    'if size(C,''*'')==1 then ';
    '  txt=C;'
    '  if length(txt)>4 then txt=''C'',end;'
    'else ';
    '  txt=''C'';'
    'end';
    'xstringb(orig(1)+dx,orig(2)+dy,txt,w,h,''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end


